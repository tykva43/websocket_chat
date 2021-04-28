#include "chatroom.h"
#include "ui_chatroom.h"

ChatRoom::ChatRoom(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::ChatRoom)
{
    ui->setupUi(this);
}

ChatRoom::~ChatRoom()
{
    delete ui;
}
