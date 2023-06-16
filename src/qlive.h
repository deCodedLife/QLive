#ifndef LIVECLIENT_H
#define LIVECLIENT_H

#include <QObject>
#include <QJsonArray>
#include <QJsonObject>
#include <QJsonDocument>
#include <QQmlApplicationEngine>

#include "core/timer.h"
#include "core/tobject.h"
#include "core/network.h"

enum WorkingTypes
{
    QLIVE_LOCAL,
    QLIVE_EXTERN
};

class QLive : public TObject
{
    Q_OBJECT

public:
    explicit QLive( QQmlApplicationEngine*, WorkingTypes = QLIVE_LOCAL, QObject * = nullptr );
    void SetServer( QString url );
    void SetErrorPage( QString url );
    void SetLocalServerDir( QString dir );

private slots:
    void handleRequest( QString data );
    void reloadQML();
    void unavailable();

private:
    Timer m_timer;
    Network m_net;
    QThread *m_thread;

private:
    bool shouldUpdate( QString file, QString hash );

private:
    QString m_errorPage;
    QString m_currentPage;
    QString m_serverAddress;
    QString m_localServerDir;

private:
    QMap<QString, QString> m_files;
    QQmlApplicationEngine *m_engine;

};

#endif // LIVECLIENT_H
