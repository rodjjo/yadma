#ifndef WAITSIGNAL_H_
#define WAITSIGNAL_H_

#include <pthread.h>
#include "iwaitsignal.h"

//--------------------------------------------------------------------------------------
class CWaitSignal: public IWaitSignal
{
public:
    CWaitSignal();
    virtual ~CWaitSignal();
    void Wait();
    void Signal();
    
private:
    pthread_cond_t            m_Condition;
    pthread_mutex_t            m_Mutex;
};

#endif /*WAITSIGNAL_H_*/
