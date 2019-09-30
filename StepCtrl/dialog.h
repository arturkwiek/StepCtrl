#ifndef DIALOG_H
#define DIALOG_H

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

    QSerialPort *my_serial;

    void readData();

private slots:
    void on_btn_OpenSerialPort_clicked();

    void on_cbx_PortNr_activated(int index);

    void on_btnClearInputScreen_clicked();

    void on_rbAscii_toggled(bool checked);

    void on_rbHex_toggled(bool checked);

    void on_rbDec_toggled(bool checked);

    void on_cbx_PortNr_currentIndexChanged(int index);

    void on_cbx_Speed_currentIndexChanged(const QString &arg1);

    void on_btnSendData_clicked();



private:
    Ui::Dialog *ui;
};

#endif // DIALOG_H
