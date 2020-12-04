#include "mainwindow.h"

#include <QApplication>
#include <QFont>
#include <QTranslator>

#include <QDebug>

void installFont()
{
    qApp->setFont(QFont("楷体"));
}

void installTranslator()
{
    QTranslator *translator = new QTranslator(qApp);

    if (translator->load(":/language/language/qt_zh_CN.qm") )
    {
        qDebug() << qApp->installTranslator(translator);
    }
}

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    installFont();

    MainWindow w;
    w.show();

    return a.exec();
}
