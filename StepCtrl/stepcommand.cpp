#include "stepcommand.h"

StepCommand::StepCommand():
    my_serial(new QSerialPort),
    iSendedChars(0)
{

}


void StepCommand::setCommand(QVector<char>& data) {
    data = data;
    this->data.push_front(0x55);
}

void StepCommand::setFormat(StepCommand::en_DataFormat_t format)
{
    switch (format) {
    case ASCII:
        b_AsciiFormat = true;
        b_DecFormat = b_HexFormat = !b_AsciiFormat;
        DBG("ASCII format");
        break;
    case HEX:
        b_HexFormat = true;
        b_DecFormat = b_AsciiFormat = !b_HexFormat;
        DBG("HEX format");
        break;
    case DEC:
        b_DecFormat = true;
        b_AsciiFormat = b_HexFormat = !b_DecFormat;
        DBG("DEC format");
        break;
    default:
        break;
    }
}
