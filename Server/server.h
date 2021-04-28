#ifndef SERVER_H
#define SERVER_H

#include <QtCore/QObject>
#include <QtCore/QList>
#include <QtCore/QByteArray>
#include <QtWebSockets/QWebSocketServer>
#include <QtWebSockets/QWebSocket>

#include "room.h"


const QString USERNAMES_FILE = ".\\db\\users.bin";
const QString ROOMS_FILE = ".\\db\\rooms.bin";

class Server : public QObject
{
    Q_OBJECT
public:
    explicit Server(quint16 port, QObject *parent = Q_NULLPTR);
    virtual ~Server();

private Q_SLOTS:
    void onNewConnection();
    void processMessage(QString message);
    void socketDisconnected();

    bool registration(QString login, QString password);
    bool authentication(QString login, QString password);
    void collectRooms();
    bool createRoom(Room);
    bool enteringInRoom(Room);
    void sendRooms(QWebSocket*);
    void sendRoomsForAllClients();

private:
    QWebSocketServer *_pWebSocketServer;
    QList<QWebSocket *> _anonymous;
    QList<Client> _clients;
    QList<Room> _rooms;
};

#endif //SERVER_H
