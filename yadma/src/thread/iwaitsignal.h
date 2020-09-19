#ifndef ISIGNALWAIT_H_
#define ISIGNALWAIT_H_

class IWaitSignal
{
public:
	virtual ~IWaitSignal(){};
	virtual void Wait() 	= 0;
	virtual void Signal() 	= 0;
};

#endif /*ISIGNALWAIT_H_*/
