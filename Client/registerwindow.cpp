#include "registerwindow.h"
#include "ui_registerwindow.h"
#include <roomchoose.h>

#include <QMessageBox>

RegisterWindow::RegisterWindow(const QUrl url, QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::RegisterWindow),
    _url(url)
{
    ui->setupUi(this);
    ui->stackedWidget->setCurrentIndex(0);
    connect(ui->signUpButton, SIGNAL(clicked()), this, SLOT(signUpClicked()));
    connect(ui->backButton, SIGNAL(clicked()), this, SLOT(backClicked()));
    _webSocket = new QWebSocket();
    connect(_webSocket, SIGNAL(connected()), this, SLOT(onConnected()));
    connect(_webSocket, SIGNAL(disconnected()), this, SLOT(onDisconnected()));
}

RegisterWindow::~RegisterWindow()
{
    delete ui;
}

void RegisterWindow::signUpClicked() {  ui->stackedWidget->setCurrentIndex(1);  }

void RegisterWindow::backClicked() {    ui->stackedWidget->setCurrentIndex(0);  }

void RegisterWindow::passwordOrLoginIsEdited() {
    if ((ui->log_in_login->text().length() > 3)&&(ui->log_in_password->text().length() > 5))
        ui->signInButton->setEnabled(true);
    else
        ui->signInButton->setEnabled(false);
}

void RegisterWindow::registrationDataIsEdited() {
    if ((ui->registrate_login->text().length() > 3)&&(ui->registrate_password->text().length() > 5)&&(ui->registrate_confirm->text() == ui->registrate_password->text()))
        ui->registrateButton->setEnabled(true);
    else
        ui->registrateButton->setEnabled(false);
}

void RegisterWindow::onConnected() {
    connect(_webSocket, SIGNAL(textMessageReceived(QString)), this, SLOT(onTextMessageReceived(QString)));
    if (_state==1)
        _webSocket->sendTextMessage("1" + ui->log_in_login->text() + ";" + ui->log_in_password->text());                // запрос на аутентификацию
    if (!_state)
        _webSocket->sendTextMessage("0" + ui->registrate_login->text() + ";" + ui->registrate_password->text());        // запрос на регистрацию
}

void RegisterWindow::onTextMessageReceived(QString message)
{
    QString messageCode = message.left(1);                              // Выделяем из сообщения его тип
    message.remove(0, 1);

    switch (messageCode.toInt()) {
    case 0:         // Отобразить информационное окно с пришедшим сообщением (случай успешной регистрации)
        QMessageBox::information(this, "Information", message);
        _webSocket->close();            // Закрываем сокет
        break;
    case 1:         // Отобразить окно с ошибкой (в случаях ошибки сервера или неудачной аутентикации/регистрации поьзователя)
        QMessageBox::warning(this, "Warning", message);
        _webSocket->close();            // Закрываем сокет
        break;
    case 2: {
        RoomChoose *roomChoose = new RoomChoose(this);    // Если логин/пароль верны, открываем следуюшее окно
        roomChoose->show();
        this->hide();                   // Прячем родительское окно
        break;
    }
    default:
        break;
    }
}

void RegisterWindow::onDisconnected() { disconnect(_webSocket, SIGNAL(textMessageReceived(QString)), this, SLOT(onTextMessageReceived(QString))); }

void RegisterWindow::connection() {     // Попытка аутентификации
    _state = 1;
    _webSocket->open(_url);
}

void RegisterWindow::registrate() {     // Попытка регистрации
    _state = 0;
    _webSocket->open(_url);
}

void RegisterWindow::closeEvent(QCloseEvent *event)
{
    this->_webSocket->close();
}
