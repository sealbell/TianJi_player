#ifndef WIDGET_H
#define WIDGET_H

#include <QWidget>
#include <QDir>
#include <QStringListModel>
#include <QDebug>
#include <QString>
#include <QMutex>
#include <QTimer>

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

signals:
    void updateTimeSignal(QString time);

    void songChanged();

    void setMaximumSignal(int maxvalue);

    void progressChanged(int newPosition);

    void loadLyricsSignal(const QString& songName);

    void updateLyricsDisplaySignal(int currentTime);

    void wrStartSignal();

    void wrOverSignal();

    void timeOutSignal();

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

    void load_lyrics(const QString& songName);

    void update_lyrics_dispaly(int currentTime);

    void on_wr_start();

    void on_wr_over();

    void on_time_out();

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
    struct LyricLine{
        QString text;// 歌词文本
        int time;// 对应时间（秒）
    };
    QList<LyricLine> lyrics;// 存储歌词的列表

    QTimer *timer;
};


#endif // WIDGET_H
