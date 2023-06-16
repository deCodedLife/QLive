#ifndef TIMER_H
#define TIMER_H

#include <QObject>
#include <QThread>

class Timer : public QObject
{
    Q_OBJECT

public:
    explicit Timer(QObject *parent = nullptr);
    void setInterval( int interval );
    void configure( QThread &thread );

signals:
    void triggered();

private slots:
    void Tick();

private:
    int m_interval;

};

#endif // TIMER_H
