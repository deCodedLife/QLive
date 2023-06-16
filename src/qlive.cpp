#include "qlive.h"

QLive::QLive( QQmlApplicationEngine *engine, WorkingTypes type, QObject *parent )
    : TObject{ parent },
    m_engine{ engine }
{
    connect( &m_net, &Network::loaded, this, &QLive::handleRequest );
    connect( &m_net, &Network::errorOccured, this, &QLive::unavailable );
    connect( &m_timer, &Timer::triggered, this, [&]() {
        if ( m_serverAddress.isEmpty() ) return;
        m_net.GET( m_serverAddress );
    } );

    m_thread = new QThread();
    m_timer.setInterval( 1000 );
    m_timer.configure( *m_thread );
    m_timer.moveToThread( m_thread );
    m_thread->start();

    if ( type == QLIVE_LOCAL )
    {
        m_serverAddress = "http://localhost";
        m_net.GET( m_serverAddress );
    }
}

void QLive::SetServer( QString url )
{
    m_serverAddress = url;
    m_net.GET( m_serverAddress );
}

void QLive::SetErrorPage( QString url )
{
    m_errorPage = url;
}

void QLive::SetLocalServerDir(QString dir)
{
    m_localServerDir = dir;
}

void QLive::handleRequest( QString data )
{
    QJsonObject response = QJsonDocument::fromJson( data.toUtf8() ).object();

    if (
        !response.contains( "status_code" ) ||
        !response.contains( "data" ) ||
        !response[ "data" ].isArray()
        ) {
        emit logData( MESSAGE_ERROR, "SERVER response is not valid" );
        unavailable();
        return;
    }

    QJsonArray files = response[ "data" ].toArray();
    bool shouldReload = false;

    for ( QJsonValueRef fileItem : files )
    {
        QJsonObject fileObject = fileItem.toObject();
        QString fileName = fileObject[ "file" ].toString();
        QString fileHash = fileObject[ "hash" ].toString();

        if ( !shouldUpdate( fileName, fileHash ) ) continue;
        m_files[ fileName ] = fileHash;
        shouldReload = true;
    }

    m_currentPage = m_serverAddress + "/Main.qml";
    if ( shouldReload ) reloadQML();

}

void QLive::reloadQML()
{
    for ( QObject *obj : m_engine->rootObjects() )
    {
        m_engine->rootObjects().removeOne( obj );
        obj->deleteLater();
    }
    m_engine->clearComponentCache();
    m_engine->load( m_currentPage );
}

void QLive::unavailable()
{
    m_currentPage = m_errorPage;
    reloadQML();
}

bool QLive::shouldUpdate( QString file, QString hash )
{
    if ( !m_files.contains( file ) ) return true;
    if ( m_files[ file ] != hash ) return true;
    return false;
}


