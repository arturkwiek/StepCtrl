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
    //    step_cmd.iSendedChars = 0;
    //    ui->lblSendedChars->setText(QString::number(step_cmd.iSendedChars));
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
    int iData;
    char tab[12];
    QVector<char> data;
    data.clear();
    data.push_back(0x55); // ---> 0x55
    qDebug() << "[" << i++ << "] -> " << QString::number(data.back(),16);
    switch(ui->cbxCommand->currentIndex()) {
    case StepCommand::STEP_LEFT:
        iData = StepCommand::STEP_LEFT;
        data.push_back(iData); // ---> StepCommand::STEP_LEFT
        qDebug() << "[" << i++ << "] -> " << QString::number(data.back(),16);

        iData = ui->leData_0->text().toInt() >> 8;
        data.push_back(iData); // ---> ui->leData_0->text().toShort();
        qDebug() << "[" << i++ << "] -> " << QString::number(data.back(),16);
        iData = ui->leData_0->text().toInt() & 0xff;
        data.push_back(iData); // ---> ui->leData_0->text().toShort();
        qDebug() << "[" << i++ << "] -> " << QString::number(data.back(),16);
        qDebug() << "[" << i++ << "] -> " << QString::number(iData,16);
        break;
    case StepCommand::STEP_RIGHT:
        iData = StepCommand::STEP_RIGHT;
        data.push_back(iData); // ---> StepCommand::STEP_RIGHT
        qDebug() << "[" << i++ << "] -> " << QString::number(data.back(),16);

        iData = ui->leData_0->text().toShort();
        data.push_back(iData); // ---> ui->leData_0->text().toShort();
        qDebug() << "[" << i++ << "] -> " << QString::number(data.back(),16);
        break;
    default:

        iData = ui->cbxCommand->currentIndex() + 1;
        data.push_back(iData); // ---> ui->cbxCommand->currentIndex() + 1;
        qDebug() << "[" << i++ << "] -> " << QString::number(data.back(),16);

        if(!ui->leData_0->text().isEmpty())
            iData = ui->leData_0->text().toShort();
        else
            iData = 0;
        data.push_back(iData); // ---> ui->leData_0->text().data()->toLatin1();
        qDebug() << "[" << i++ << "] leData_0 -> " << QString::number(data.back(),16);

    }

    if(!ui->leData_1->text().isEmpty())
        iData = ui->leData_1->text().toShort();
    else
        iData = 0;
    data.push_back(iData); // ---> ui->leData_1->text().data()->toLatin1();
    qDebug() << "[" << i++ << "] leData_1 -> " << ui->leData_1->text() << "|" << QString::number(data.back(),16);

    if(!ui->leData_2->text().isEmpty())
        iData = ui->leData_2->text().toShort();
    else
        iData = 0;
    data.push_back(iData); // ---> ui->leData_2->text().data()->toLatin1();
    qDebug() << "[" << i++ << "] leData_2 -> " << QString::number(data.back(),16);

    if(!ui->leData_3->text().isEmpty())
        iData = ui->leData_3->text().toShort();
    else
        iData = 0;
    data.push_back(iData); // ---> ui->leData_3->text().data()->toLatin1();
    qDebug() << "[" << i++ << "] leData_3 -> " << QString::number(data.back(),16);

    if(!ui->leData_4->text().isEmpty())
        iData = ui->leData_4->text().toShort();
    else
        iData = 0;
    data.push_back(iData); // ---> ui->leData_4->text().data()->toLatin1();
    qDebug() << "[" << i++ << "] leData_4 -> " << QString::number(data.back(),16);

    if(!ui->leData_5->text().isEmpty())
        iData = ui->leData_5->text().toShort();
    else
        iData = 0;
    data.push_back(iData); // ---> ui->leData_5->text().data()->toLatin1();
    qDebug() << "[" << i++ << "] leData_5 -> " << QString::number(data.back(),16);

    if(!ui->leData_6->text().isEmpty())
        iData = ui->leData_6->text().toShort();
    else
        iData = 0;
    data.push_back(iData); // ---> ui->leData_6->text().data()->toLatin1();
    qDebug() << "[" << i++ << "] leData_6 -> " << QString::number(data.back(),16);

    if(!ui->leData_7->text().isEmpty())
        iData = ui->leData_7->text().toShort();
    else
        iData = 0;
    data.push_back(iData); // ---> ui->leData_7->text().data()->toLatin1();
    qDebug() << "[" << i++ << "] leData_7 -> " << QString::number(data.back(),16);

    qDebug() << data.size();
    data.push_back('\0'); // ---> '\0'
    qDebug() << "[" << i++ << "] -> '\0' " << QString::number(data.back(),16);
    for(int i=0;i<data.size();i++) {
        tab[i] = data.at(i);
    }
    qDebug() << "data: " << data;
    qDebug() << "data.constData: " << data.constData();
    qDebug() << "data.data: " << data.data();
    qDebug() << "tab: " << tab[0];
    qDebug() << "*tab: " << QString::number(*tab);
    qDebug() << "tab[0]: " << QString::number(tab[0]);
    ui->teOutputScreen->insertPlainText("Tx (hex): ");
    for(int i =0;i<data.size();i++) {
        ui->teOutputScreen->insertPlainText("[" + QString::number(data.at(i),16) + "] ");
        step_cmd.iSendedChars += step_cmd.my_serial->write(&tab[i],1);
    }
    ui->teOutputScreen->insertPlainText("\n");
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

void Dialog::on_btnClearOutputScreen_clicked()
{
    ui->teOutputScreen->clear();
    step_cmd.iSendedChars = 0;
    ui->lblSendedChars->setText(QString::number(step_cmd.iSendedChars));
}
