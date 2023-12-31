#include "musiclibrary.h"
#include <QDir>
#include <QFileInfoList>

//- 实现音乐文件夹扫描和播放列表管理等功能。

MusicLibrary::MusicLibrary(QObject *parent) : QObject(parent) {
    // 初始化，可能还会包括一些配置加载等
}

void MusicLibrary::setMusicFolder(const QString &path) {
    if (QDir(path).exists()) {
        musicFolderPath = path;
        scanMusicFolder();
    } else {
        // 可能需要错误处理，比如发送信号或记录日志
    }
}

QStringList MusicLibrary::getSongList() const {
    return songList;
}

void MusicLibrary::scanMusicFolder() {
    QDir musicDir(musicFolderPath);
    QFileInfoList files = musicDir.entryInfoList(QStringList() << "*.mp3" << "*.wav", QDir::Files);
    QStringList newSongList;
    foreach (const QFileInfo &file, files) {
        newSongList << file.absoluteFilePath();
    }

    if (newSongList != songList) {
        songList = newSongList;
        emit songListChanged(songList);
    }
}
