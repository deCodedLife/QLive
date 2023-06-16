#include "timer.h"

Timer::Timer(QObject *parent)
    : QObject{parent}
{

}

void Timer::setInterval( int interval )
{
    m_interval = interval;
}

void Timer::configure( QThread &thread )
{
    connect( &thread, &QThread::started, this, &Timer::Tick );
}

void Timer::Tick()
{
    while ( true )
    {
        this->thread()->msleep( m_interval );
        emit triggered();
    }
}
