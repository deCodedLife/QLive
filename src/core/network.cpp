#include "network.h"

Network::Network(QObject *parent)
    : TObject{parent}
{
    m_manager = new QNetworkAccessManager();
    connect( m_manager, &QNetworkAccessManager::finished, this, &Network::handleData );
    qDebug() << "Device supports OpenSSL: " << QSslSocket::supportsSsl();

    m_conf = QSslConfiguration::defaultConfiguration();
    m_conf.setProtocol(QSsl::AnyProtocol);
    m_conf.setPeerVerifyMode(QSslSocket::VerifyNone);
    m_conf.setCaCertificates({});
    QSslConfiguration::setDefaultConfiguration(m_conf);
    m_manager->setStrictTransportSecurityEnabled(false);
}

void Network::GET(QString url)
{
    QNetworkRequest request;
    request.setSslConfiguration(m_conf);
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
    reply->ignoreSslErrors();

    if ( reply->error() && reply->error() != QNetworkReply::SslHandshakeFailedError )
    {
        emit errorOccured( reply->errorString() );
        emit logData( MESSAGE_ERROR, reply->errorString() );
        return;
    }
    emit loaded( reply->readAll() );
}
