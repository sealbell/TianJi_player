#include "playercontroller.h"
#include <QFileInfo>

//- 实现播放控制功能，如播放、暂停、跳转等。

PlayerController::PlayerController(QObject *parent) : QObject(parent), mplayerProcess(new QProcess(this))
{
    // 初始化 mplayer 进程设置
}

PlayerController::~PlayerController() {
    // 确保 mplayer 进程被正确关闭
    if (mplayerProcess->isOpen()) {
        mplayerProcess->terminate();
        mplayerProcess->waitForFinished();
    }
}

void PlayerController::play(const QString &filePath) {
    if (!QFileInfo::exists(filePath)) {
        // 文件不存在的处理
        return;
    }

    if (mplayerProcess->state() == QProcess::NotRunning) {
        QStringList args;
        // 这里添加 mplayer 的启动参数
        args << "-slave";
        args << "-quiet";
        args << filePath;

        mplayerProcess->start("mplayer", args);
    } else {
        // 如果已经在播放，则发送播放命令到 mplayer
        mplayerProcess->write("pause\n");
    }
}

void PlayerController::pause() {
    if (mplayerProcess->state() != QProcess::NotRunning) {
        // 发送暂停命令到 mplayer
        mplayerProcess->write("pause\n");
    }
}

void PlayerController::stop() {
    if (mplayerProcess->state() != QProcess::NotRunning) {
        // 发送停止命令到 mplayer
        mplayerProcess->write("stop\n");
        mplayerProcess->waitForFinished();
    }
}

void PlayerController::next() {
    // 发送播放下一曲命令到 mplayer，需要结合播放列表管理实现
}

void PlayerController::previous() {
    // 发送播放上一曲命令到 mplayer，需要结合播放列表管理实现
}

void PlayerController::setVolume(int volume) {
    // 发送设置音量命令到 mplayer
    QString command = QString("volume %1 1\n").arg(volume);
    mplayerProcess->write(command.toUtf8());
}

void PlayerController::setPosition(int seconds) {
    // 发送跳转到特定位置的命令到 mplayer
    QString command = QString("seek %1 2\n").arg(seconds);
    mplayerProcess->write(command.toUtf8());
}
