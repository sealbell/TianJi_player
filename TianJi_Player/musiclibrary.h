#ifndef MUSICLIBRARY_H
#define MUSICLIBRARY_H

#include <QObject>

class MusicLibrary : public QObject
{
    Q_OBJECT
public:
    explicit MusicLibrary(QObject *parent = nullptr);

signals:

};

#endif // MUSICLIBRARY_H
