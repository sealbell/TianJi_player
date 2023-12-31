#ifndef LYRICSMANAGER_H
#define LYRICSMANAGER_H

#include <QObject>
#include <QString>

//- 定义处理歌词文件的类。

class LyricsManager : public QObject {
    Q_OBJECT

public:
    explicit LyricsManager(QObject *parent = nullptr);
    void loadLyricsForSong(const QString &songPath);
    QString getCurrentLyrics() const;

signals:
    void lyricsChanged(const QString &currentLyrics);

private:
    QString parseLyricsFile(const QString &lyricsPath);
    QString currentLyrics;
};

#endif // LYRICSMANAGER_H
