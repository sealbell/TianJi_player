#ifndef MUSICLIBRARY_H
#define MUSICLIBRARY_H

#include <QObject>
#include <QStringList>

//- 定义处理音乐文件和播放列表的类。

class MusicLibrary : public QObject {
    Q_OBJECT

public:
    explicit MusicLibrary(QObject *parent = nullptr);
    void setMusicFolder(const QString &path);
    QStringList getSongList() const;

signals:
    void songListChanged(const QStringList &songs);

private:
    void scanMusicFolder();
    QString musicFolderPath;
    QStringList songList;
};

#endif // MUSICLIBRARY_H

