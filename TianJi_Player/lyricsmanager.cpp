#include "lyricsmanager.h"
#include <QFile>
#include <QTextStream>

//- 实现歌词显示等功能。

LyricsManager::LyricsManager(QObject *parent) : QObject(parent) {
    // 歌词管理器的初始化
}

void LyricsManager::loadLyricsForSong(const QString &songPath) {
    QString lyricsPath = songPath + ".lrc"; // 假设歌词文件与音乐文件同名，仅扩展名不同
    QFile lyricsFile(lyricsPath);

    if (lyricsFile.exists() && lyricsFile.open(QIODevice::ReadOnly)) {
        currentLyrics = parseLyricsFile(lyricsPath);
        emit lyricsChanged(currentLyrics);
        lyricsFile.close();
    } else {
        currentLyrics = tr("No lyrics file found.");
        emit lyricsChanged(currentLyrics);
    }
}

QString LyricsManager::getCurrentLyrics() const {
    return currentLyrics;
}

QString LyricsManager::parseLyricsFile(const QString &lyricsPath) {
    QFile file(lyricsPath);
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text))
        return tr("Unable to open lyrics file.");

    QTextStream in(&file);
    QString lyrics = in.readAll();
    // 此处可以进一步解析歌词文件，提取时间戳和歌词文本

    return lyrics;
}
