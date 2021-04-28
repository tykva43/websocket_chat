#ifndef ROOMCHOOSE_H
#define ROOMCHOOSE_H

#include <QMainWindow>
#include <QModelIndex>

namespace Ui {
class RoomChoose;
}

class RoomChoose : public QMainWindow
{
    Q_OBJECT

public:
    explicit RoomChoose(QWidget *parent = nullptr);
    ~RoomChoose();

public slots:
    void roomIsChoosed(QModelIndex);

private:
    Ui::RoomChoose *ui;
};

#endif // ROOMCHOOSE_H
