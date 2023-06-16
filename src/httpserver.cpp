#include "httpserver.h"

HttpServer::HttpServer( QString workingDirectory, QObject *parent )
    : TObject{parent},
    m_workingDirectory( workingDirectory )
{
    m_server.route( "/", [&] ( const QHttpServerRequest &req  ) {
        return HandleConnection();
    } );

    m_server.route( "/.*", [&]( const QHttpServerRequest &req ) {
        QString url = req.url().toString();
        QString filePath = m_workingDirectory + '/' + url.split( ":0/" )[ 1 ];

        if ( !QFile().exists( filePath ) ) {
            QString error = "[ LOCAL SERVER ] File not found: " + filePath;
            emit logData( MESSAGE_ERROR, error );
            return QHttpServerResponse(
                QHttpServerResponder::StatusCode::Forbidden
            );
        }

        QFile file( filePath );

        if ( !file.open( QIODevice::ReadOnly | QIODevice::Text ) )
        {
            QString error = "[ LOCAL SERVER ] Can't open file: " + filePath;
            logData( MESSAGE_ERROR, error );
            return QHttpServerResponse( error );
        }

        QString data = file.readAll();
        file.close();

        return QHttpServerResponse( data );
    }  );

    m_server.listen( QHostAddress( HOST ), PORT );
}

QJsonObject HttpServer::HandleConnection()
{
    QJsonArray files;

    if ( !QDir().exists( m_workingDirectory ) )
    {
        QString error = "[ LOCAL SERVER ] Working dirrectory doesn't exists";
        logData( MESSAGE_ERROR, error );
        return SendData( error, 500 );
    }

    QDirIterator it( m_workingDirectory, QDir::Files, QDirIterator::Subdirectories );

    while ( it.hasNext() )
    {
        QString filePath = it.next();
        QFile file( filePath );

        if ( !file.open( QIODevice::ReadOnly | QIODevice::Text ) )
        {
            logData( MESSAGE_WARNING, "Can't open " + filePath );
            continue;
        }

        QString fileContent = file.readAll();
        file.close();

        QCryptographicHash hash( QCryptographicHash::Sha512 );
        hash.addData( fileContent.toUtf8() );
        QString hashed = QString::fromUtf8( hash.result().toHex() );

        QJsonObject fileObject;
        fileObject[ "file" ] = filePath;
        fileObject[ "hash" ] = hashed;

        files.append( fileObject );

    }

    return SendData( files );
}

QJsonObject HttpServer::SendData( QVariant data, int statusCode )
{
    QJsonObject response;
    response[ "status_code" ] = statusCode;
    response[ "data" ] = QJsonValue::fromVariant( data );
    return response;
}


