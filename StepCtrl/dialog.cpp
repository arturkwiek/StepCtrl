#include "dialog.h"
#include "ui_dialog.h"
#include <QtSerialPort/QSerialPort>
#include <QSerialPortInfo>
#include <QDebug>
#include <vector>

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

    connect(my_serial, &QSerialPort::readyRead, this, &Dialog::readData);
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
//    qDebug() << index << "<<";
    foreach (const QSerialPortInfo &serialPortInfo, QSerialPortInfo::availablePorts())
    {
//        qDebug() << index << i;

        if(i == index) {
            ui->lbl_dscr->setText(serialPortInfo.description());
            qDebug() << "Serial Port: " << serialPortInfo.portName() << "(" << serialPortInfo.description() << ")";
//            qDebug() << serialPortInfo.portName();

        }
        i++;
    }

    ui->btn_OpenSerialPort->setStyleSheet("color: black");

}

void Dialog::on_btnClearInputScreen_clicked()
{
    ui->teInputScreen->clear();
}

void Dialog::on_rbAscii_toggled(bool checked)
{
    qDebug() << " Ascii toogled";

}

void Dialog::on_rbHex_toggled(bool checked)
{
    qDebug() << " Hex toogled";

}

void Dialog::on_rbDec_toggled(bool checked)
{
    qDebug() << " Dec toogled";

}

void Dialog::on_cbx_PortNr_currentIndexChanged(int index)
{
    qDebug() << "PortNr index changed";
    my_serial->close();
    ui->btn_OpenSerialPort->setText("Open");
    ui->btn_OpenSerialPort->setStyleSheet("color: black");

}

void Dialog::on_cbx_Speed_currentIndexChanged(const QString &arg1)
{
    qDebug() << "Speed index changed: " << arg1;
    my_serial->setBaudRate(ui->cbx_Speed->currentText().toInt());
}

void Dialog::on_btnSendData_clicked()
{
   QVector<char> data;
   if(!ui->leData_0->text().isEmpty())
       data.push_back(ui->leData_0->text().data()->toLatin1());
   if(!ui->leData_1->text().isEmpty())
       data.push_back(ui->leData_1->text().data()->toLatin1());
   if(!ui->leData_2->text().isEmpty())
       data.push_back(ui->leData_2->text().data()->toLatin1());
   if(!ui->leData_3->text().isEmpty())
       data.push_back(ui->leData_3->text().data()->toLatin1());
   if(!ui->leData_4->text().isEmpty())
       data.push_back(ui->leData_4->text().data()->toLatin1());
   if(!ui->leData_5->text().isEmpty())
       data.push_back(ui->leData_5->text().data()->toLatin1());
   if(!ui->leData_6->text().isEmpty())
       data.push_back(ui->leData_6->text().data()->toLatin1());
   if(!ui->leData_7->text().isEmpty())
       data.push_back(ui->leData_7->text().data()->toLatin1());
   qDebug() << data.size();
   data.push_back('\0');
   my_serial->write(data.data());
   qDebug() << data.data();

}

void Dialog::readData()
{
    const QByteArray data = my_serial->readAll();
    ui->teInputScreen->insertPlainText(data);
    qDebug() << data;
}
