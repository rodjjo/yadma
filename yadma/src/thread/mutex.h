#ifndef MUTEX_H_
#define MUTEX_H_

#include <stdio.h>
#include <pthread.h>

//---------------------------------------------------------------------------------
class CMutex
{
public:
	CMutex()
	{
		 pthread_mutex_init( &m_Mutex, NULL);
	}
	
	~CMutex()
	{
		pthread_mutex_destroy( &m_Mutex );
	}
	
	void Lock()
	{
		pthread_mutex_lock( &m_Mutex );
	}
	
	void Unlock()
	{
		pthread_mutex_unlock( &m_Mutex );
	}
	
	bool TryLock()
	{
		return pthread_mutex_trylock( &m_Mutex ) == 0;
	}
	
private:
	pthread_mutex_t m_Mutex;
};

//---------------------------------------------------------------------------------
class CMutexLocker
{
public:
	CMutexLocker( CMutex& Mutex ) : m_Mutex( Mutex)
	{
		m_Mutex.Lock();
	}
	
	~CMutexLocker()
	{
		m_Mutex.Unlock();
	}
	
	
private:
	CMutex& m_Mutex;
};


#endif /*MUTEX_H_*/
