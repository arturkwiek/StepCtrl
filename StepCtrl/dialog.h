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

private slots:
    void on_btn_OpenSerialPort_clicked();

    void on_cbx_PortNr_activated(int index);

private:
    Ui::Dialog *ui;
    QSerialPort *my_serial;
};

#endif // DIALOG_H
