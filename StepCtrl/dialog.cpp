#include "dialog.h"
#include "ui_dialog.h"
#include <QtSerialPort/QSerialPort>
#include <QSerialPortInfo>
#include <QDebug>
#include <vector>

Dialog::Dialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Dialog)
{
    int i = 0;
    ui->setupUi(this);


    foreach (const QSerialPortInfo &serialPortInfo, QSerialPortInfo::availablePorts())
    {
        qDebug() << i++;
        ui->cbxPortNr->addItem(serialPortInfo.portName());
        qDebug() << serialPortInfo.description();

    }

    connect(step_cmd.my_serial, &QSerialPort::readyRead, this, &Dialog::readData);
}

Dialog::~Dialog()
{
    delete ui;
}

void Dialog::on_btnOpenSerialPort_clicked()
{

    if(!step_cmd.my_serial->isOpen()) {
        step_cmd.my_serial->setDataBits(QSerialPort::Data8);
        step_cmd.my_serial->setStopBits(QSerialPort::OneStop);
        step_cmd.my_serial->setPortName(ui->cbxPortNr->itemText(ui->cbxPortNr->currentIndex()));
        step_cmd.my_serial->setBaudRate(ui->cbxSpeed->currentText().toInt());
        //        switch(ui->cbx_PortNr->currentIndex()) {
        //        case 0:
        //            my_serial->setPortName(ui->cbx_PortNr->itemText(0)); break;

        //        }

        if(step_cmd.my_serial->open(QIODevice::ReadWrite)) {
            ui->btnOpenSerialPort->setText("Close");
            ui->btnOpenSerialPort->setStyleSheet("color: green");
        }
        else
            ui->btnOpenSerialPort->setStyleSheet("color: red");
    }
    else if(step_cmd.my_serial->isOpen()) {
        step_cmd.my_serial->close();
        ui->btnOpenSerialPort->setText("Open");
        ui->btnOpenSerialPort->setStyleSheet("color: black");
    }

}

void Dialog::on_cbxPortNr_activated(int index)
{
    int i = 0;
//    qDebug() << index << "<<";
    foreach (const QSerialPortInfo &serialPortInfo, QSerialPortInfo::availablePorts())
    {
//        qDebug() << index << i;

        if(i == index) {
            ui->lblDscr->setText(serialPortInfo.description());
            qDebug() << "Serial Port: " << serialPortInfo.portName() << "(" << serialPortInfo.description() << ")";
//            qDebug() << serialPortInfo.portName();

        }
        i++;
    }

    ui->btnOpenSerialPort->setStyleSheet("color: black");

}

void Dialog::on_btnClearInputScreen_clicked()
{
    ui->teInputScreen->clear();
}

void Dialog::on_cbxPortNr_currentIndexChanged(int index)
{
    qDebug() << "PortNr index changed";
    step_cmd.my_serial->close();
    ui->btnOpenSerialPort->setText("Open");
    ui->btnOpenSerialPort->setStyleSheet("color: black");

}

void Dialog::on_cbxSpeed_currentIndexChanged(const QString &arg1)
{
    qDebug() << "Speed index changed: " << arg1;
    step_cmd.my_serial->setBaudRate(ui->cbxSpeed->currentText().toInt());
}

void Dialog::on_btnSendData_clicked()
{
   QVector<char> data;
   data.push_back(0x55);
   data.push_back(ui->cbxCommand->currentIndex() + 1);
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
   step_cmd.my_serial->write(data.data());
   qDebug() << data.data();
   step_cmd.setCommand(data);

}

void Dialog::readData()
{
    const QByteArray data = step_cmd.my_serial->readAll();
    ui->teInputScreen->insertPlainText(data);
    qDebug() << data;
}


void Dialog::on_rbHex_clicked(bool checked)
{
    if(checked)
        step_cmd.setFormat(StepCommand::HEX);
}

void Dialog::on_rbAscii_clicked(bool checked)
{
    if(checked)
        step_cmd.setFormat(StepCommand::ASCII);
}

void Dialog::on_rbDec_clicked(bool checked)
{
    if(checked)
        step_cmd.setFormat(StepCommand::DEC);
}
