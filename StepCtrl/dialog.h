#ifndef DIALOG_H
#define DIALOG_H

#include "stepcommand.h"

#include <QDialog>
#include <QtSerialPort/QSerialPort>

namespace Ui {
class Dialog;
}

class Dialog : public QDialog
{
    Q_OBJECT

public:
    explicit Dialog(QWidget *parent = 0);
    ~Dialog();

    void readData();

private slots:
    void on_btnOpenSerialPort_clicked();

    void on_cbxPortNr_activated(int index);

    void on_btnClearInputScreen_clicked();

    void on_cbxPortNr_currentIndexChanged(int index);

    void on_cbxSpeed_currentIndexChanged(const QString &arg1);

    void on_btnSendData_clicked();

    void on_rbHex_clicked(bool checked);

    void on_rbAscii_clicked(bool checked);

    void on_rbDec_clicked(bool checked);

    void on_btnClearOutputScreen_clicked();

    void on_cbxCommand_currentIndexChanged(int index);

private:
    Ui::Dialog *ui;
    StepCommand step_cmd;
};

#endif // DIALOG_H
