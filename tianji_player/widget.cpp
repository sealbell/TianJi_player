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

            // 等待子进程结束
            //wait(NULL);

            // 清理资源
            //::close(fd_pip[0]);
            //unlink(fifo_cmd);
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
    write(fifo_fd, cmd, strlen(cmd));

    // 处理选中的文件，例如播放音乐
    printf("%s",cmd);
    fflush(stdout);
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
                int minutes = static_cast<int>(timeVal) / 60;
                int seconds = static_cast<int>(timeVal) % 60;
                QString formattedTime = QString("%1:%2").arg(minutes, 2, 10, QLatin1Char('0')).arg(seconds, 2, 10, QLatin1Char('0'));

            // 发射信号以更新UI
            emit widget->updateTimeSignal(formattedTime);
            }
        }
        sleep(1); // 每秒更新一次
    }
    return NULL;
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
    strcpy(cmd,"pause\n");

    fifoMutex.lock();
    write(fifo_fd,cmd,strlen(cmd));
    fifoMutex.unlock();
    printf("%s",cmd);
    fflush(stdout);
}
