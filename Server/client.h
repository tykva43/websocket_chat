#ifndef CLIENT_H
#define CLIENT_H


#include <QtWebSockets/QWebSocket>

class Client
{
public:
    Client();
    Client(Client&);
    QWebSocket * socket;
    QString nickName;
    int roomId;
    Client(QWebSocket *, QString, int i = -1);
};
//Q_DECLARE_METATYPE(Client)

inline bool operator==(Client& cl1, Client& cl2) {
    if (cl1.socket == cl2.socket)
        return true;
    return false;
}


#endif // CLIENT_H
