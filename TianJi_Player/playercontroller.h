#ifndef PLAYERCONTROLLER_H
#define PLAYERCONTROLLER_H

#include <QObject>

class PlayerController : public QObject
{
    Q_OBJECT
public:
    explicit PlayerController(QObject *parent = nullptr);

signals:

};

#endif // PLAYERCONTROLLER_H
