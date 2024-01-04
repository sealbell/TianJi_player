#ifndef WIDGET_H
#define WIDGET_H

#include <QWidget>
#include <QDir>
#include <QStringListModel>
#include <QDebug>
#include <QString>

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

//    // 函数声明
//    QStringListModel *show_music_list(Widget *widget);
//    friend void *get_time(void *arg);

signals:
    void updateTimeSignal(QString time);



public slots:




private slots:
    void on_song_list_clicked(const QModelIndex &index);

    void updateTimeUI(QString time);

private:
    Ui::Widget *ui;
    int fifo_fd; // 用于有名管道的文件描述符
    pthread_t timeThread; // 用于更新时间的线程
    int fd_pip[2];
};


#endif // WIDGET_H
