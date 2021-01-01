#include <stdio.h>
#include <unistd.h>
#include "thread.h"

//--------------------------------------------------------------------------------------------
CThread::CThread()
{
    m_Running     = false;
    m_Stopped     = false;
    m_Created     = false;
    
    pthread_attr_init( &m_attr );
        
       pthread_attr_setdetachstate( &m_attr, PTHREAD_CREATE_JOINABLE );
}

//-----------------------------------------------------------------------------------    
CThread::~CThread()
{
    Stop();
    CleanUp();  
}
    
//-----------------------------------------------------------------------------------    
bool CThread::IsRunning()
{
    return m_Running;
}

//-----------------------------------------------------------------------------------    
bool CThread::Start()
{
    if( !m_Running )
    {
        CleanUp();
        
        m_Stopped = false;
           
        m_Created = pthread_create( &m_Thread, &m_attr,  &CThread::_Run, this ) == 0;
        
        return true;
    }
    
    return false;
}

//-----------------------------------------------------------------------------------    
void CThread::Stop()
{
    m_Stopped = true;
    
    if( !m_Running )
    {
        return;
    }
    
    pthread_join( m_Thread, NULL );
    
    while( m_Running )
    {
        sleep(1);
    }
    
    m_Created = false;
}

//-----------------------------------------------------------------------------------    
bool CThread::StopRequested()
{
    return m_Stopped;
}

//-----------------------------------------------------------------------------------    
void CThread::CleanUp()
{
    if( m_Created )
    {
        m_Stopped = true;
        pthread_join( m_Thread, NULL);
    }
    
    m_Stopped = false;
}

//-----------------------------------------------------------------------------------    
void* CThread::_Run( void* Object )
{
    ((CThread*)Object)->m_Running = true;
    ((CThread*)Object)->Run();
    ((CThread*)Object)->m_Running = false;
    return NULL;
}

