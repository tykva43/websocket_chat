#ifndef CHATROOM_H
#define CHATROOM_H

#include <QMainWindow>

namespace Ui {
class ChatRoom;
}

class ChatRoom : public QMainWindow
{
    Q_OBJECT

public:
    explicit ChatRoom(QWidget *parent = nullptr);
    ~ChatRoom();

private:
    Ui::ChatRoom *ui;
};

#endif // CHATROOM_H
