#include "dialog.h"
#include "stepcommand.h"
#include <QApplication>

#include <QtSerialPort/QSerialPort>


int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    Dialog w;
    w.show();

    return a.exec();
}
