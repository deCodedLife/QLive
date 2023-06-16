#include "network.h"

Network::Network(QObject *parent)
    : TObject{parent}
{
    m_manager = new QNetworkAccessManager();
    connect( m_manager, &QNetworkAccessManager::finished, this, &Network::handleData );
}

void Network::GET(QString url)
{
    QNetworkRequest request;
    request.setUrl( QUrl( url ) );
    m_manager->get( request );
}

void Network::POST(QString url, QVariant data)
{
    QNetworkRequest request;
    request.setUrl( QUrl( url ) );
    QByteArray byteData = QByteArray::fromRawData( (char*) data.data(), data.metaType().sizeOf() );
    m_manager->post( request, byteData );
}

void Network::handleData( QNetworkReply *reply )
{
    if ( reply->error() ) {
        emit errorOccured( reply->errorString() );
        emit logData( MESSAGE_ERROR, reply->errorString() );
        return;
    }
    emit loaded( reply->readAll() );
}
