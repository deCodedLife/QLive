#include "tobject.h"

TObject::TObject(QObject *parent)
    : QObject{parent}
{

}

void TObject::logData(MessageTypes type, QVariant data)
{
    QString prefix = "";
    if ( type == MESSAGE_INFO ) prefix = INFO;
    if ( type == MESSAGE_WARNING ) prefix = WARNING;
    if ( type == MESSAGE_ERROR ) prefix = ERROR;
    qDebug() << prefix << data.toString();
}
