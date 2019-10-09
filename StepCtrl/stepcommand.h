#ifndef STEPCOMMAND_H
#define STEPCOMMAND_H
#include <QSerialPort>
#include <QVector>
#include <QDebug>

#define DBG(x) qDebug() << x

class StepCommand
{

    public:

    typedef enum {
        ASCII,
        HEX,
        DEC
    }en_DataFormat_t;

    StepCommand();
    ~StepCommand(){}
    void setCommand(QVector<char>& data);
    void setFormat(en_DataFormat_t format);
    bool isHex() {
        return b_HexFormat;
    }
    bool isDec() {
        return b_DecFormat;
    }
    bool isAscii() {
        return b_AsciiFormat;
    }
    QSerialPort *my_serial;    
    qint64 iSendedChars;


    enum {
        STEP_LEFT = 1,
        STEP_RIGHT,
        START,
        STOP,
        ONE_TURN,
        HALF_STEP,
        FULL_STEP,
        SPEED_SLOW,
        SPEED_MIDLE,
        SPEED_FAST

    };

private:


    QVector<char> data; /* ramka danych */
    bool b_AsciiFormat = true; /* przesyłanie danych w formacie Ascii */
    bool b_HexFormat = true; /* przesyłanie danych w formacie Hex */
    bool b_DecFormat = true; /* przesyłanie danych w formacie Dec */

};

#endif // STEPCOMMAND_H
