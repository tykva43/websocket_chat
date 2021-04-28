#include "room.h"

Room::Room() {

}

void Room::setId(int id) {
    _id = id;
}

void Room::setName(QString name) {
    _name = name;
}

void Room::setPassword(QString password) {
    _password = password;
}

int Room::getId() {
    return _id;
}

QString Room::getName() {
    return _name;
}

QString Room::getPassword() {
    return _password;
}

void Room::addClient(Client client) {
    _clients << client;
}

void Room::removeClient(Client client) {
    for (auto i: _clients) {
        if (i.socket == client.socket) {
            _clients.removeOne(i);
            break;
        }
    }
}


QList<Client> Room::getClients() {
    return _clients;
}

Room::Room(int id, QString name, QString password) {
    this->_id = id;
    this->_name = name;
    this->_password = password;
}

