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
    step_cmd.iSendedChars = 0;
    ui->lblSendedChars->setText(QString::number(step_cmd.iSendedChars));
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
    int i=0;

    char tab[12];
    QVector<char> data;
    data.push_back(0x55);
    qDebug() << "[" << i++ << "] -> " << data.back();
    data.push_back(ui->cbxCommand->currentIndex() + 1);
    qDebug() << "[" << i++ << "] -> " << data.back();
    if(!ui->leData_0->text().isEmpty())
        data.push_back(ui->leData_0->text().data()->toLatin1());
    else
        data.push_back('0');
    qDebug() << "[" << i++ << "] -> " << data.back();
    if(!ui->leData_1->text().isEmpty())
        data.push_back(ui->leData_1->text().data()->toLatin1());
    else
        data.push_back('0');
    qDebug() << "[" << i++ << "] -> " << data.back();
    if(!ui->leData_2->text().isEmpty())
        data.push_back(ui->leData_2->text().data()->toLatin1());
    else
        data.push_back('0');
    qDebug() << "[" << i++ << "] -> " << data.back();
    if(!ui->leData_3->text().isEmpty())
        data.push_back(ui->leData_3->text().data()->toLatin1());
    else
        data.push_back('0');
    qDebug() << "[" << i++ << "] -> " << data.back();
    if(!ui->leData_4->text().isEmpty())
        data.push_back(ui->leData_4->text().data()->toLatin1());
    else
        data.push_back('0');
    qDebug() << "[" << i++ << "] -> " << data.back();
    if(!ui->leData_5->text().isEmpty())
        data.push_back(ui->leData_5->text().data()->toLatin1());
    else
        data.push_back('0');
    qDebug() << "[" << i++ << "] -> " << data.back();
    if(!ui->leData_6->text().isEmpty())
        data.push_back(ui->leData_6->text().data()->toLatin1());
    else
        data.push_back('0');
    qDebug() << "[" << i++ << "] -> " << data.back();
    if(!ui->leData_7->text().isEmpty())
        data.push_back(ui->leData_7->text().data()->toLatin1());
    else
        data.push_back('0');
    qDebug() << "[" << i++ << "] -> " << data.back();

    qDebug() << data.size();
    data.push_back('\0');
    qDebug() << "[" << i++ << "] -> '\0' " << data.back();
    qDebug() << "[" << i++ << "] -> '\0' " << data.back();

    tab[0] = data.at(0);
    tab[1] = data.at(1);
    tab[2] = data.at(2);
    tab[3] = data.at(3);
    tab[4] = data.at(4);
    tab[5] = data.at(5);
    tab[6] = data.at(6);
    tab[7] = data.at(7);
    tab[8] = data.at(8);
    tab[9] = data.at(9);
    tab[10] = data.at(10);
    qDebug() << "data" << data;
    qDebug() << "data.constData: " << data.constData();
    qDebug() << "data.data: " << data.data();
    qDebug() << "tab: " << tab[0];
    qDebug() << "*tab: " << QString::number(*tab);
    qDebug() << "tab[0]: " << QString::number(tab[0]);
    for(int i =0;i<12;i++)
        step_cmd.iSendedChars += step_cmd.my_serial->write(&tab[i]);
    ui->lblSendedChars->setText(QString::number(step_cmd.iSendedChars));
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
