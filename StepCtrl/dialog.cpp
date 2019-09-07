#include "dialog.h"
#include "ui_dialog.h"
#include <QtSerialPort/QSerialPort>
#include <QSerialPortInfo>
#include <QDebug>

Dialog::Dialog(QWidget *parent) :
    QDialog(parent),
    my_serial(new QSerialPort),
    ui(new Ui::Dialog)
{
    int i = 0;
    ui->setupUi(this);


    foreach (const QSerialPortInfo &serialPortInfo, QSerialPortInfo::availablePorts())
    {
        qDebug() << i++;
        ui->cbx_PortNr->addItem(serialPortInfo.portName());
        qDebug() << serialPortInfo.description();

    }
}

Dialog::~Dialog()
{
    delete ui;
}

void Dialog::on_btn_OpenSerialPort_clicked()
{

    if(!my_serial->isOpen()) {
        my_serial->setDataBits(QSerialPort::Data8);
        my_serial->setStopBits(QSerialPort::OneStop);
        my_serial->setPortName(ui->cbx_PortNr->itemText(ui->cbx_PortNr->currentIndex()));
        my_serial->setBaudRate(ui->cbx_Speed->currentText().toInt());
        //        switch(ui->cbx_PortNr->currentIndex()) {
        //        case 0:
        //            my_serial->setPortName(ui->cbx_PortNr->itemText(0)); break;

        //        }

        if(my_serial->open(QIODevice::ReadWrite)) {
            ui->btn_OpenSerialPort->setText("Close");
            ui->btn_OpenSerialPort->setStyleSheet("color: black");
        }
        else
            ui->btn_OpenSerialPort->setStyleSheet("color: red");
    }
    else if(my_serial->isOpen()) {
        my_serial->close();
        ui->btn_OpenSerialPort->setText("Open");
        ui->btn_OpenSerialPort->setStyleSheet("color: black");
    }

}

void Dialog::on_cbx_PortNr_activated(int index)
{
    int i = 0;
    qDebug() << index << "<<";
    foreach (const QSerialPortInfo &serialPortInfo, QSerialPortInfo::availablePorts())
    {
        qDebug() << index << i;

        if(i == index) {
            ui->lbl_dscr->setText(serialPortInfo.description());
            qDebug() << serialPortInfo.description();
        }
        i++;
    }

    ui->btn_OpenSerialPort->setStyleSheet("color: black");

}
