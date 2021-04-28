#include "server.h"

#include <QtCore/QDebug>
#include <QFile>
#include <QDataStream>

QT_USE_NAMESPACE
// Функция удаления одного клиента из списка
Client removeOne(QList<Client> *clients, QWebSocket *socket) {
    for (auto i : *clients) {
        if (i.socket == socket) {
            clients->removeOne(i);
            return i;
        }
    }
}

void deleteAll(QList<Client>* clients) {
    for (auto i : *clients) {
        clients->removeAll(i);
    }
}

// Функция добавления клиента в конкретную комнату
void addClientInRoom(Client client, int roomId, QList<Room> * rooms) {
    foreach(Room room, *rooms) {
        if (room.getId() == roomId) {
            room.addClient(client);
            break;
        }
    }
}

//! Конструктор
Server::Server(quint16 port, QObject *parent) :
    QObject(parent),
    _pWebSocketServer(Q_NULLPTR) {
    _pWebSocketServer = new QWebSocketServer(QStringLiteral("Chat Server"),
                                              QWebSocketServer::NonSecureMode,
                                              this);
    if (_pWebSocketServer->listen(QHostAddress::Any, port))              // Запуск прослушивания порта port для с любых интерфейсов
    {
        qDebug() << "Chat ChatServer listening on port" << port;
        connect(_pWebSocketServer, &QWebSocketServer::newConnection,
                this, &Server::onNewConnection);
    }
    collectRooms();
}

//! Деструктор
Server::~Server() {
    _pWebSocketServer->close();                                          // Закрыть соединение
    deleteAll(&_clients);                        // Удалить всех клиентов из списка
}

//! Подключение клиента
void Server::onNewConnection() {
    QWebSocket *pSocket = _pWebSocketServer->nextPendingConnection();    // Вернуть следующее ожидающее подключение

    connect(pSocket, &QWebSocket::textMessageReceived, this, &Server::processMessage);
    connect(pSocket, &QWebSocket::disconnected, this, &Server::socketDisconnected);

    _anonymous << pSocket;                                                // Добавить клиента в список анонимов (клиентов, которые не авторизовались)
}

//! Обработка поступившего от клиента сообщения
void Server::processMessage(QString message) {
    QWebSocket *pSender = qobject_cast<QWebSocket *>(sender());

    QString messageCode = message.left(1);                               // Выделяем из сообщения его тип
    message.remove(0, 1);

    switch (messageCode.toInt()) {
    case 0: {
        QStringList data = message.split(';');
        if (registration(data[0], data[1]))
            pSender->sendTextMessage("0You have successfully registered!"); // Отправить сообщение о результате регистрации
        else
            pSender->sendTextMessage("1Something went wrong when registering. Please try again later.");
        break;
    }
    case 1: {
        QStringList data = message.split(';');
        if (authentication(data[0], data[1])) {
            pSender->sendTextMessage("2");              // Отправить код сообщения об успешной аутентификации
            _clients << Client(pSender, data[0]);       // Добавляем сокет подключившегося клиента в список клиентов
            _anonymous.removeAll(pSender);              // Удаляем пользователя из анонимов, т.к. он вошел под логином/паролем
        }
        else
            pSender->sendTextMessage("1Login or password not found. Make sure the entered data is correct.");
        break;
    }
    case 2: { // Запрос на вход в комнату
        QStringList data = message.split(';');
        if (enteringInRoom(Room(data[0].toInt(), data[1], data[2]))) {
            pSender->sendTextMessage("3"); // Отправить код об успешном входе в выбранную комнату

            //Client client = removeOne(&_clients, pSender);
            for (QList<Client>::iterator i = _clients.begin(); i != _clients.end(); i++) {
                if ((*i).socket == pSender) {
                    (*i).roomId = data[0].toInt();
                    addClientInRoom((*i), data[0].toInt(), &_rooms);
                    _clients.removeOne(*i);
                    break;
                }
            }
        }
        else
            pSender->sendTextMessage("1Invalid password. Make sure the entered data is correct.");
        break;
    }
    case 3: { // Запрос на создание комнаты
        QStringList data = message.split(';');
        if (createRoom(Room(_rooms.length()+1, data[0], data[1]))) {
            pSender->sendTextMessage("0You have successfully created a room!"); // Отправить сообщение о результате создания комнаты
            sendRoomsForAllClients();       // обновить скисок комнат у всех клиентов
        }
        else
            pSender->sendTextMessage("1Something went wrong when creating a room. Please try again later.");
        break;
    }
    case 4: { // Запрос на получение списка комнат
        sendRooms(pSender);
        break;
    }
    case 5:
        break;
        for (auto client : _clients) {                    // Проход по всем клиентам
            if (client.socket != pSender)                                         // Не отправить отправителю его же сообщение
            {
                client.socket->sendTextMessage(message);                          // Отправить пришедшее сообщение другому клиенту из списка
            }
        }
        break;
    default: break;
    }
}

//! При выходе клиента из чата
void Server::socketDisconnected() {
    QWebSocket *pClient = qobject_cast<QWebSocket *>(sender());
    if (pClient)
    {
        _anonymous.removeAll(pClient);
        removeOne(&_clients, pClient);
        pClient->deleteLater();
    }
}

bool Server::registration(QString login, QString password) {
    QFile inFile(USERNAMES_FILE);
    if(inFile.open(QIODevice::Append)) {
        QDataStream stream(&inFile);
        stream << login << password;
    }
    else
        return false;
    inFile.close();
    return true;
}

bool Server::authentication(QString login, QString password) {
    QFile inFile(USERNAMES_FILE);
    if (inFile.open(QIODevice::ReadOnly)) {
        QDataStream stream(&inFile);
        QString log, pas;
        while (!stream.atEnd()) {
            stream >> log >> pas;
            if ( (log == login) && (pas == password) ) {
                inFile.close();
                return true;
            }
        }
    }
    else
        return false;
    inFile.close();
    return false;
}

void Server::collectRooms() {       // Сбор списка комнат из файла
    QFile inFile(ROOMS_FILE);
    if (inFile.open(QIODevice::ReadOnly)) {
        QDataStream stream(&inFile);
        Room room;
        while (!stream.atEnd()) {
            stream >> room;
            _rooms.append(room);
        }
        inFile.close();
    }
}

bool Server::createRoom(Room room) {        // Создание новой комнаты
    QFile inFile(USERNAMES_FILE);
    if(inFile.open(QIODevice::Append)) {
        QDataStream stream(&inFile);
        stream << room;
        inFile.close();
        _rooms.append(room);
    }
}

bool Server::enteringInRoom(Room inRoom) {    // Вход в комнату
    foreach (Room room, _rooms) {
        if (inRoom == room) {
            return true;
        }
    }
    return false;
}

void Server::sendRooms(QWebSocket *socket) {    // Отправить клиенту список комнат
    QString response = "4";
    for (auto i : _rooms) {
        response += QString::number(i.getId()) + ";" + i.getName() + ";";
    }
    socket->sendTextMessage(response);
}

void Server::sendRoomsForAllClients() {
    QString response = "4";
    for (auto i : _rooms) {
        response += QString::number(i.getId()) + ";" + i.getName() + ";";
    }
    for (auto i : _clients) {
        i.socket->sendTextMessage(response);
    }
}
