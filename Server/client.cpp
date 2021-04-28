#include "client.h"

Client::Client()
{

}

Client:: Client(QWebSocket * s, QString n, int i) {
    this->socket = s;
    this->nickName = n;
    this->roomId = i;
}

Client::Client(Client& cl) {
    this->nickName = cl.nickName;
    this->roomId = cl.roomId;
    this->socket = cl.socket;
}
