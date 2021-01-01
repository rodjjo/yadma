#ifndef YADMAPTR_H_
#define YADMAPTR_H_

#include "../thread/mutex.h"

//thread safe counter
class yadmaptr_counter
{
public:
    yadmaptr_counter()
    {
        m_Count = 1;
    }
    
    ~yadmaptr_counter()
    {
        m_mtxCounter.Unlock();
    }
    
    int GetCount()
    {
        int RetVal = m_Count;
        return RetVal;
    }
    
    void Increment()
    {
        m_Count++;
    }
    
    void Decrement()
    {
        m_Count--;
    }
    
    void Lock()
    {
        m_mtxCounter.Lock();
    }
    
    void Unlock()
    {
        m_mtxCounter.Unlock();
    }
    
private:
    int m_Count;
    CMutex m_mtxCounter;
};

template<typename PtrType>
class yadmaptr
{
public:
    //-----------------------------------------------------------
    yadmaptr()
    {
        m_Counter = new yadmaptr_counter();
        m_PtrVar  = NULL;
    }
    
    //-----------------------------------------------------------
    yadmaptr( PtrType* newPtrVariable )
    {
        m_Counter = new yadmaptr_counter();
        m_PtrVar = newPtrVariable;
    }

    //-----------------------------------------------------------
    yadmaptr( const yadmaptr<PtrType>& aOther )
    {
        m_Counter = new yadmaptr_counter();
        m_PtrVar  = NULL;
        *this       = aOther;
    }

    //-----------------------------------------------------------
    yadmaptr<PtrType>& operator=( const yadmaptr<PtrType>& aOther )
    {
        yadmaptr<PtrType>& Other = (yadmaptr<PtrType>&)aOther;
         
        Other.m_Counter->Lock();
        
        if( ( Other.m_PtrVar != m_PtrVar ) && ( (&Other) != this ) )
        {
            fflush( stdout );
            
            if( Other.m_PtrVar != NULL ) 
            {
                Release();
                
                m_Counter     = Other.m_Counter;
                m_PtrVar     = Other.m_PtrVar;
                
                m_Counter->Increment();
            }
            else
            {
                Release();
                
                m_Counter = new yadmaptr_counter();
                m_PtrVar  = NULL;
            }
        }
            
        Other.m_Counter->Unlock();
        return *this;
    }
    
    //-----------------------------------------------------------
    operator bool()
    {
        return ( m_PtrVar != NULL );
    }

    //-----------------------------------------------------------
    PtrType* operator->()
    {
        return m_PtrVar;
    }
    
    //-----------------------------------------------------------
    PtrType& operator*()
    {
        return &(*m_PtrVar);
    }

    //-----------------------------------------------------------
    ~yadmaptr()
    {
        m_Counter->Lock();
        
        Release( );
        
        if( m_Counter )
        {
            m_Counter->Unlock();
        }
    }
    
private:
    void Release()
    {
        if( m_Counter == NULL )
        {
            return;
        }
        
        m_Counter->Decrement();
                
        if( m_Counter->GetCount() == 0 )
        {
            delete m_PtrVar;
            m_PtrVar = NULL;
    
            delete m_Counter;
            m_Counter = NULL;
        }
        
    }
    
private:
    PtrType*                 m_PtrVar;
    yadmaptr_counter*         m_Counter;
};

#endif /*YADMAPTR_H_*/
