#ifndef NETWORK_H
#define NETWORK_H

#include <QObject>
#include <QNetworkReply>
#include <QNetworkAccessManager>

#include <QSslSocket>
#include <QSslConfiguration>

#include "tobject.h"

class Network : public TObject
{
    Q_OBJECT
public:
    explicit Network(QObject *parent = nullptr);

    void GET(QString url);
    void POST(QString url, QVariant data);


signals:
    void loaded(QString);
    void errorOccured(QString);

private slots:
    void handleData(QNetworkReply*);

private:
    QNetworkAccessManager *m_manager;
    QSslConfiguration m_conf;

};

#endif // NETWORK_H
