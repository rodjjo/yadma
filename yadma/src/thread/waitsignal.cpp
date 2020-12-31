#include "waitsignal.h"

//-------------------------------------------------------
CWaitSignal::CWaitSignal()
{
    pthread_cond_init( &m_Condition, NULL );
    pthread_mutex_init( &m_Mutex, NULL );
}

//-------------------------------------------------------
CWaitSignal::~CWaitSignal()
{
    pthread_cond_destroy( &m_Condition );
    pthread_mutex_destroy( &m_Mutex );
}

//-------------------------------------------------------
void CWaitSignal::Wait()
{
    pthread_mutex_lock( &m_Mutex );
    pthread_cond_wait( &m_Condition, &m_Mutex ); //esperar a hora de sair sleep eterno até condição for setada
    pthread_mutex_unlock( &m_Mutex );
}
//-------------------------------------------------------
void CWaitSignal::Signal()
{
    pthread_mutex_lock( &m_Mutex );
    pthread_cond_broadcast( &m_Condition );
    pthread_mutex_unlock( &m_Mutex );
}
