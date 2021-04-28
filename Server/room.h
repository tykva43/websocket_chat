#ifndef ROOM_H
#define ROOM_H

#include <QtCore/QList>
#include "QtWebSockets/QWebSocket"

#include "client.h"

class Room
{
public:
    Room();
    Room(int, QString, QString);
    void setId(int);
    void setName(QString);
    void setPassword(QString);

    int getId();
    QString getName();
    QString getPassword();

    void addClient(Client);
    void removeClient(Client);
    QList<Client> getClients();

    friend QDataStream& operator<<(QDataStream&,  Room&);
    friend QDataStream& operator>>(QDataStream&, Room&);

private:
    QList<Client> _clients;
    int _id;
    QString _name;
    QString _password;
};

QDataStream& operator<<(QDataStream& in, Room& r) {
    QString id(QString::number(r._id));
    in << id << r._name << r._password;
    return in;
}

QDataStream& operator>>(QDataStream& out, Room& r) {
    QString id;
    out >> id >> r._name >> r._password;
    r._id = id.toInt();
    return out;
}

inline bool operator==(Room r1,  Room r2) {
    if ((r1.getId()== r2.getId()) && (r1.getPassword() == r2.getPassword()))
        return true;
    return false;
}

#endif // ROOM_H

