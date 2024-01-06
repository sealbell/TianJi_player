#include "widget.h"
#include "ui_widget.h"

#include <QDir>
#include <QStringListModel>

#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <assert.h>
#include <fcntl.h>
#include <sys/wait.h>
#include <stdlib.h>
#include <string.h>
#include <pthread.h>

// 全局变量
QStringListModel *model;
pid_t ch_id;

// 函数声明
QStringListModel *show_music_list(Widget *widget);

Widget::Widget(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::Widget)
{
    // 显示ui
    ui->setupUi(this);

    // 将模型设置到 QListView 控件上
    ui->song_list->setModel(show_music_list(this));

    int fd_fifo_cmd;
    pid_t pid;
    const char* fifo_cmd = "./fifo_cmd";
    cmd = (char*)calloc(1024,sizeof (char));

    // 创建有名管道
    mkfifo(fifo_cmd, 0666);

     // 创建无名管道
     if (pipe(fd_pip) == -1) {
         perror("pipe");
         exit(1);
     }

     // 创建子进程
     pid = fork();
     if (pid == -1) {
        perror("fork");
        exit(1);
     }

     if (pid == 0) { // 子进程
        // 关闭无名管道的读端
        ::close(fd_pip[0]);

        // 打开有名管道的读端
        fd_fifo_cmd = open(fifo_cmd, O_RDONLY);
        if (fd_fifo_cmd == -1) {
            perror("open");
            exit(1);
        }

        // 重定向标准输入到有名管道
        dup2(fd_fifo_cmd, 0);
        ::close(fd_fifo_cmd);

        // 重定向标准输出到无名管道的写端
        dup2(fd_pip[1], 1);
        ::close(fd_pip[1]);

        // 执行 mplayer
        execlp("mplayer", "mplayer", "-slave", "-quiet", "-idle", NULL);
        perror("execlp");
        exit(1);
    } else { // 父进程
        ch_id = pid;

        // 关闭无名管道的写端
        //::close(fd_pip[1]);

        // 打开有名管道
        fifo_fd = open("./fifo_cmd", O_WRONLY);
        if (fifo_fd == -1) {
            perror("open");
            exit(1);
        }

        // 此处可以通过 fd_fifo_cmd 写入命令到 mplayer
        // 通过 fd_pip[0] 读取 mplayer 的输出

        // 创建线程
        pthread_create(&timeThread, NULL, updateTimeThread, this);

        // 连接信号和槽
        connect(this, &Widget::updateTimeSignal, this, &Widget::updateTimeUI);
        connect(this,&Widget::songChanged,this,&Widget::updateTotalTimeUI);
        connect(this,&Widget::setMaximumSignal,this,&Widget::set_song_progress_Maximum);
        connect(this,&Widget::progressChanged,this,&Widget::on_song_progress_changed);


    }
}

Widget::~Widget()
{
    delete ui;
    kill(ch_id,9);
    // 关闭有名管道
    if (fifo_fd != -1) {
        ::close(fifo_fd);
    }
}

QStringListModel *show_music_list(Widget *widget){
    QDir musicDir("../song");
    // 过滤出音频文件
    QStringList audioFiles = musicDir.entryList(QStringList() << "*.mp3" << "*.wav", QDir::Files);

    // 创建一个模型并将文件列表填充到模型中
    model = new QStringListModel(widget);
    model->setStringList(audioFiles);

    return model;
}

void Widget::on_song_list_clicked(const QModelIndex &index)
{
    //修改为播放状态
    pauseMutex.lock();
    pauseThreads = 0;
    pauseMutex.unlock();
    //将按钮变为暂停按钮
    btn_status = 1;
    ui->control_btn->setStyleSheet("QPushButton{image: url(:/icon/暂停)}");
    ui->control_btn->setToolTip("暂停");

    QString selectedFile = model->data(index, Qt::DisplayRole).toString();
    // 移除引号
    selectedFile.remove('\"');

    // 构建 mplayer 的播放命令
    QString command = "loadfile " + QDir("../song").absoluteFilePath(selectedFile) + "\n";

    char* cmd = command.toUtf8().data();
    // 向有名管道写入命令
    fifoMutex.lock();
    clearPipe(fd_pip[0]);
    write(fifo_fd, cmd, strlen(cmd));
    fifoMutex.unlock();

    // 处理选中的文件，例如播放音乐
    printf("%s",cmd);
    fflush(stdout);

    emit songChanged();
}

void* Widget::updateTimeThread(void* arg) {
    Widget* widget = static_cast<Widget*>(arg);
    char buffer[1024];
    while (true) {
        //将互斥锁锁住，防止冲突
        widget->pauseMutex.lock();
        //判断共享状态变量
        if(widget->pauseThreads){
            //解锁
            widget->pauseMutex.unlock();
            //挂起一段时间，防止快速循环
            sleep(1);
            continue;
        }
        widget->pauseMutex.unlock();
        // 向 MPlayer 发送获取时间的指令
        widget->fifoMutex.lock();
        widget->clearPipe(widget->fd_pip[0]);
        write(widget->fifo_fd, "get_time_pos\n", strlen("get_time_pos\n"));

        // 从管道读取 MPlayer 的响应
        int nbytes = read(widget->fd_pip[0], buffer, sizeof(buffer) - 1);

        widget->fifoMutex.unlock();

        if (nbytes > 0) {
            buffer[nbytes] = '\0';

            // 解析时间
            QString response(buffer);
            int timePos = response.indexOf("ANS_TIME_POSITION=");
            if (timePos != -1) {
                QString timeStr = response.mid(timePos + 18).split("\n")[0];
                float timeVal = timeStr.toFloat();

                emit widget->progressChanged((int)timeVal);

                int minutes = static_cast<int>(timeVal) / 60;
                int seconds = static_cast<int>(timeVal) % 60;
                QString formattedTime = QString("%1:%2")
                        .arg(minutes, 2, 10, QLatin1Char('0'))
                        .arg(seconds, 2, 10, QLatin1Char('0'));

            // 发射信号以更新UI
            emit widget->updateTimeSignal(formattedTime);
            }
        }
        sleep(1); // 每秒更新一次
    }
    return NULL;
}

int Widget::parseTotalTime(const QString &response)
{
    //MPlayer的响应格式为"ANS_LENGTH=duration"
    QRegExp regex("ANS_LENGTH=(\\d+\\.\\d+)");
    if(regex.indexIn(response) != -1){
        //将持续时间字符串转换为浮点，然后转换为int
        return static_cast<int>(regex.cap(1).toFloat());
    }
    return 0;
}

QString Widget::formatTime(int totalSeconds)
{
    int minutes = totalSeconds / 60;
    int seconds = totalSeconds % 60;

    return QString("%1:%2")
            .arg(minutes,2,10,QChar('0'))
            .arg(seconds,2,10,QChar('0'));
}

void Widget::clearPipe(int pipefd)
{
    // 设置管道为非阻塞模式
    int flags = fcntl(pipefd, F_GETFL, 0);
    fcntl(pipefd, F_SETFL, flags | O_NONBLOCK);

    // 清空管道
    char buffer[1024];
    while (read(pipefd, buffer, sizeof(buffer)) > 0) {
        // 继续读取直到没有更多数据
    }

    // 恢复管道为阻塞模式（如果需要）
    fcntl(pipefd, F_SETFL, flags);
}

void Widget::updateTimeUI(QString time) {
    // 在这里更新 UI
    // 例如：ui->currentTimeLabel->setText(time);
    ui->current_time->setText(time);
}

void Widget::on_control_btn_clicked()
{
    if(btn_status == 1){
        ui->control_btn->setStyleSheet("QPushButton{image: url(:/icon/播放)}");
        ui->control_btn->setToolTip("播放");
        btn_status = 0;
        pauseMutex.lock();
        pauseThreads = 1;
        pauseMutex.unlock();
    }
    else{
        ui->control_btn->setStyleSheet("QPushButton{image: url(:/icon/暂停)}");
        ui->control_btn->setToolTip("暂停");
        btn_status = 1;
        pauseMutex.lock();
        pauseThreads = 0;
        pauseMutex.unlock();
    }
    fifoMutex.lock();

    strcpy(cmd,"pause\n");
    clearPipe(fd_pip[0]);
    write(fifo_fd,cmd,strlen(cmd));
    fifoMutex.unlock();
    printf("%s",cmd);
    fflush(stdout);
}

void Widget::on_previous_btn_clicked()
{
    int currentRow = ui->song_list->currentIndex().row();
    if (currentRow <= 0) return;  // 没有播放音乐或已是第一首歌

    // 选择上一首歌
    QModelIndex previousIndex = ui->song_list->model()->index(currentRow - 1, 0);
    ui->song_list->setCurrentIndex(previousIndex);

    // 获取歌曲路径并播放
    QString selectedFile = ui->song_list->model()->data(previousIndex, Qt::DisplayRole).toString();
    QString command = "loadfile " + QDir("../song").absoluteFilePath(selectedFile) + "\n";

    //strcpy(cmd,command.toUtf8().constData());
    fifoMutex.lock();
    clearPipe(fd_pip[0]);
    write(fifo_fd, command.toUtf8().constData(), command.toUtf8().length());
    fifoMutex.unlock();

    printf("%s",command.toUtf8().constData());
    fflush(stdout);

    emit songChanged();
}

void Widget::on_next_btn_clicked()
{
    int currentRow = ui->song_list->currentIndex().row();
    int totalRows = ui->song_list->model()->rowCount();
    if (currentRow < 0 || currentRow >= totalRows - 1) return;  // 没有播放音乐或已是最后一首歌

    // 选择下一首歌
    QModelIndex nextIndex = ui->song_list->model()->index(currentRow + 1, 0);
    ui->song_list->setCurrentIndex(nextIndex);

    // 获取歌曲路径并播放
    QString selectedFile = ui->song_list->model()->data(nextIndex, Qt::DisplayRole).toString();
    QString command = "loadfile " + QDir("../song").absoluteFilePath(selectedFile) + "\n";

    fifoMutex.lock();
    write(fifo_fd, command.toUtf8().constData(), command.toUtf8().length());
    fifoMutex.unlock();

    printf("%s",command.toUtf8().constData());
    fflush(stdout);

    emit songChanged();
}

void Widget::updateTotalTimeUI()
{
    const char* command = "get_time_length\n";
    QString response;
    int tryCount = 0;
    const int maxTries = 10; // 重试次数上限

    while (tryCount < maxTries) {
        fifoMutex.lock();
        clearPipe(fd_pip[0]); // 清空管道中的信息
        write(fifo_fd, command, strlen(command));

        char buffer[1024];
        ssize_t nbytes = read(fd_pip[0], buffer, sizeof(buffer) - 1);
        fifoMutex.unlock();

        if (nbytes > 0) {
            buffer[nbytes] = '\0';
            response = QString(buffer);
            if (response.contains("ANS_LENGTH=")) {
                break; // 成功获取到响应
            }
        }

        tryCount++;
        sleep(1); // 短暂等待再次尝试
    }

    if (!response.isEmpty()) {
        int totalTime = parseTotalTime(response); // 解析时长
        emit setMaximumSignal(totalTime);
        QString formattedTime = formatTime(totalTime); // 格式化时长
        ui->all_time->setText(formattedTime); // 更新 UI
    }
}

void Widget::on_sound_btn_clicked()
{
    const char *command = NULL;
    // 检查当前是否已经静音
    if (isMuted) {
        // 如果已经静音，取消静音并更改按钮样式
        ui->sound_btn->setStyleSheet("QPushButton { image: url(:/icon/音量); }");
        command = "mute 0\n"; // 发送取消静音命令
        isMuted = false;
    } else {
        // 如果未静音，设置静音并更改按钮样式
        ui->sound_btn->setStyleSheet("QPushButton { image: url(:/icon/静音); }");
        command = "mute 1\n"; // 发送静音命令
        isMuted = true;
    }
    fifoMutex.lock();
    clearPipe(fd_pip[0]);
    write(fifo_fd, command, strlen(command)); // 向管道发送命令
    fifoMutex.unlock();
}

void Widget::on_sound_slider_valueChanged(int value)
{
    // 发送音量调整命令
    QString command = QString("volume %1 1\n").arg(value);

    fifoMutex.lock();
    clearPipe(fd_pip[0]);
    write(fifo_fd, command.toUtf8().constData(), command.toUtf8().length());
    fifoMutex.unlock();

    // 根据音量值更改按钮图标
    if (value == 0) {
        // 静音
        ui->sound_btn->setStyleSheet("QPushButton { image: url(:/icon/静音); }");
        isMuted = true;
    } else {
        // 非静音
        ui->sound_btn->setStyleSheet("QPushButton { image: url(:/icon/音量); }");
        isMuted = false;
    }
}

void Widget::on_song_progress_sliderReleased()
{
    // 获取滑块当前位置
    int value = ui->song_progress->value();
    // 计算对应的时间
    QString command = QString("seek %1 2\n").arg(value);

    // 发送命令
    fifoMutex.lock();
    clearPipe(fd_pip[0]);
    write(fifo_fd, command.toUtf8().constData(), command.toUtf8().length());
    fifoMutex.unlock();
}

void Widget::set_song_progress_Maximum(int maxvalue)
{
    ui->song_progress->setMaximum(maxvalue);
}

void Widget::on_song_progress_changed(int newPosition)
{
        ui->song_progress->setValue(newPosition); // 更新滑动条位置
        //QString command = QString("seek %1 2\n").arg(newPosition); // 发送新位置命令
        //fifoMutex.lock();
        //clearPipe(fd_pip[0]);
        //write(fifo_fd, command.toUtf8().constData(), command.toUtf8().length());
        //fifoMutex.unlock();
}









