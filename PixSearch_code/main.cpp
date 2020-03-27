#include "mainwindow.h"

#include <QApplication>

int main(int argc, char *argv[])
{
  //  QGuiApplication::setAttribute(Qt::AA_EnableHighDpiScaling);

    QApplication a(argc, argv);

    QFile qss(":/skin.qss");
    if(qss.open(QFile::ReadOnly)){
    QString styleSheet = QLatin1String(qss.readAll());
    a.setStyleSheet(styleSheet);
    qss.close();
    }
    else
    {
        qDebug()<<"load qss failed";
    }
    MainWindow w;
    w.show();
    return a.exec();
}
