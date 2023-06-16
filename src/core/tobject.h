#ifndef TOBJECT_H
#define TOBJECT_H

#define INFO "[ ]"
#define WARNING "[!]"
#define ERROR "[-]"

#include <QObject>
#include <QVariant>
#include <QJsonArray>
#include <QJsonObject>
#include <QCryptographicHash>

enum MessageTypes {
    MESSAGE_INFO,
    MESSAGE_WARNING,
    MESSAGE_ERROR
};

class TObject : public QObject
{
    Q_OBJECT
public:
    explicit TObject(QObject *parent = nullptr);

public slots:
    void logData( MessageTypes type, QVariant data );

};

#endif // TOBJECT_H
