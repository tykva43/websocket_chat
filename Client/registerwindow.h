#ifndef REGISTERWINDOW_H
#define REGISTERWINDOW_H

#include <QMainWindow>
#include <QtWebSockets/QWebSocket>
#include <QUrl>

namespace Ui {
class RegisterWindow;
}

class RegisterWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit RegisterWindow(const QUrl, QWidget *parent = nullptr);
    ~RegisterWindow();

public slots:
    void signUpClicked();
    void backClicked();
    void passwordOrLoginIsEdited();
    void registrationDataIsEdited();
    void onConnected();
    void onDisconnected();
    void connection();
    void onTextMessageReceived(QString);
    void registrate();

private:
    Ui::RegisterWindow *ui;
    QUrl _url;
    QWebSocket * _webSocket;
    int _state;                  // Состояние клиента: 1 - аутентификация; 0 - регистрация

protected:
    void closeEvent(QCloseEvent *event) override;
};

#endif // REGISTERWINDOW_H
