#ifndef WIDGET_H
#define WIDGET_H

#include <QWidget>
#include <QDir>
#include <QStringListModel>
#include <QDebug>
#include <QString>
#include <QMutex>

#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/wait.h>
#include <fcntl.h>
#include <pthread.h>
#include <stdlib.h>
#include <string.h>

// 全局变量
//QStringListModel *model;
//pid_t ch_id;


QT_BEGIN_NAMESPACE
namespace Ui { class Widget; }
QT_END_NAMESPACE

class Widget : public QWidget
{
    Q_OBJECT


public:
    Widget(QWidget *parent = nullptr);
    ~Widget();

    void music_player(QString file);

    static void* updateTimeThread(void* arg);

    int parseTotalTime(const QString &response);

    QString formatTime(int totalSeconds);

    void clearPipe(int pipefd);

//    // 函数声明
//    QStringListModel *show_music_list(Widget *widget);
//    friend void *get_time(void *arg);

signals:
    void updateTimeSignal(QString time);

    void songChanged();

    void setMaximumSignal(int maxvalue);

    void progressChanged(int newPosition);

private slots:
    void on_song_list_clicked(const QModelIndex &index);

    void updateTimeUI(QString time);

    void on_control_btn_clicked();

    void on_previous_btn_clicked();

    void on_next_btn_clicked();

    void updateTotalTimeUI();

    void on_sound_btn_clicked();

    void on_sound_slider_valueChanged(int value);

    void on_song_progress_sliderReleased();

    void set_song_progress_Maximum(int maxvalue);

    void on_song_progress_changed(int newPosition);

private:
    Ui::Widget *ui;
    bool pauseThreads;// 共享状态变量
    QMutex pauseMutex;// 共享状态互斥锁互斥锁
    QMutex fifoMutex;// 管道互斥锁
    int btn_status = 0;// 按钮状态  1 暂停 0 播放
    int fifo_fd; // 用于有名管道的文件描述符
    pthread_t timeThread; // 用于更新时间的线程
    int fd_pip[2]; // 用于无名管道的文件描述符
    char *cmd;
    bool isMuted = false;//静音按钮
};


#endif // WIDGET_H
