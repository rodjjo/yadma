#ifndef THREAD_H_
#define THREAD_H_

#include <pthread.h>

//--------------------------------------------------------------------------------------------
class CThread
{
public:
    CThread();
    
    virtual ~CThread();
    
    bool IsRunning();
    
    bool Start();
    
    void Stop();
    
    bool StopRequested();
    
protected:
    virtual void Run() = 0;
    
private:
    void CleanUp();
    static void* _Run( void* Object );
    
private:
    bool             m_Created;
    bool             m_Running;
    bool             m_Stopped;
    pthread_attr_t     m_attr;
    pthread_t         m_Thread;
};


#endif /*THREAD_H_*/
