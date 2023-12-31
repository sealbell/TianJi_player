#ifndef PLAYERCONTROLLER_H
#define PLAYERCONTROLLER_H

#include <QObject>
#include <QProcess>

//- 定义控制音乐播放（调用 mplayer）的类。

class PlayerController : public QObject {
    Q_OBJECT

public:
    explicit PlayerController(QObject *parent = nullptr);
    ~PlayerController();

    void play(const QString &filePath);
    void pause();
    void stop();
    void next();
    void previous();
    void setVolume(int volume);
    void setPosition(int seconds);

private:
    QProcess *mplayerProcess;
};

#endif // PLAYERCONTROLLER_H
