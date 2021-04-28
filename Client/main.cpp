#include "registerwindow.h"

#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    RegisterWindow w(QUrl(QStringLiteral("ws://localhost:8080")));
    w.show();
    return a.exec();
}
