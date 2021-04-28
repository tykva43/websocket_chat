#include "roomchoose.h"
#include "ui_roomchoose.h"



RoomChoose::RoomChoose(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::RoomChoose) {
    ui->setupUi(this);
    ui->stackedWidget->setCurrentIndex(1);
}

RoomChoose::~RoomChoose() {
    delete ui;
}

void RoomChoose::roomIsChoosed(QModelIndex index) {

}
