#ifndef __SEMTIMOBJ_H__
#define __SEMTIMOBJ_H__

 



#ifndef OS2_INCLUDED
   #ifndef INCL_DOS
     #define INCL_DOS
   #endif
   #ifndef INCL_WINMESSAGEMGR
      #define INCL_WINMESSAGEMGR
   #endif
	#include <os2.h>
#endif

#define DC_SEM_PRIVATE 0

#ifndef _exp_
   #ifdef __DLL__
      #define _exp_ _Export
   #else
      #define _exp_
   #endif
#endif

class Semaphore{
protected:
	APIRET Error;
	char *Name;
	ULONG Attributes;

	_exp_ Semaphore(ULONG attr=DC_SEM_PRIVATE,char *Nm=NULL);

public:
	enum SemStates {SemDoNothing,SemOpen,SemCreate};

	APIRET GetError(){return Error;}
	void SetCreateAttributes(ULONG x){Attributes=x;}
};




class EventSemaphore:public Semaphore{
	HEV hev;
	ULONG Timeout;

public:
	_exp_ EventSemaphore(char *Nm=NULL,SemStates Action=SemCreate,
			ULONG attr=DC_SEM_PRIVATE,
			ULONG time=SEM_INDEFINITE_WAIT);
	_exp_ EventSemaphore(HEV hv,SemStates Action=SemOpen,
			ULONG attr=DC_SEM_PRIVATE,			ULONG time=SEM_INDEFINITE_WAIT);

	void SetTimeout(ULONG time){Timeout=time;}

	void Open(){
		if (Name==NULL){
			hev=0;
			Error=DosOpenEventSem((PSZ)Name,&hev);
		}
		else
			Error=DosOpenEventSem((PSZ)Name,&hev);
	}

	void Create(){
		Error=DosCreateEventSem(Name, &hev,Attributes,FALSE);
	}

	void Close(){Error=DosCloseEventSem(hev);}
	ULONG Querry(){
		ULONG count;
		Error=DosQueryEventSem(hev,&count);
		return count;
	}
	void Post(){Error=DosPostEventSem(hev);}
	void Reset(){
		ULONG count;
		Error=DosResetEventSem(hev,&count);
	}

	HEV GetHandle(){return hev;}

	void Wait(){Error=DosWaitEventSem(hev,Timeout);}
friend ULONG WinWaitEventSemaphore(EventSemaphore &Sem,ULONG time=SEM_INDEFINITE_WAIT);
friend class AsyncTimer;
};


struct MutexQueryInfo{
	PID pid;
	TID tid;
	ULONG count;
};

class MutexSemaphore:public Semaphore{
	HMTX hmtx;
	ULONG Timeout;

public:
	_exp_ MutexSemaphore(char *Nm=NULL,SemStates Action=SemCreate,
			ULONG attr=DC_SEM_PRIVATE,
			ULONG time=SEM_INDEFINITE_WAIT);
	_exp_ MutexSemaphore(HMTX hmtx,SemStates Action=SemOpen,
			ULONG attr=DC_SEM_PRIVATE,
			ULONG time=SEM_INDEFINITE_WAIT);

	void SetTimeout(ULONG time){Timeout=time;}

	void Open(){
		if (Name==NULL){
			hmtx=0;
			Error=DosOpenMutexSem((PSZ)Name,&hmtx);
		}
		else
			Error=DosOpenMutexSem((PSZ)Name,&hmtx);
	}

	void Create(){
		Error=DosCreateMutexSem(Name,&hmtx,Attributes,FALSE);
	}

	void Close(){Error=DosCloseMutexSem(hmtx);}
	MutexQueryInfo Querry(){
		MutexQueryInfo Info;
		Error=DosQueryMutexSem(hmtx,&Info.pid,&Info.tid,&Info.count);
		return Info;
	}
	void Release(){Error=DosReleaseMutexSem(hmtx);}
	void Request(){
		Error=DosRequestMutexSem(hmtx,Timeout);
	}
friend ULONG WinRequestMutexSemaphore(MutexSemaphore &Sem,ULONG time=SEM_INDEFINITE_WAIT);

};



ULONG _exp_ WinRequestMutexSemaphore(MutexSemaphore &Sem,ULONG time);
ULONG _exp_ WinWaitEventSemaphore(EventSemaphore &Sem,ULONG time);


class Timer{
protected:
	HTIMER HTimer;
	EventSemaphore *Sem;
	APIRET Error;
	ULONG TimeInterval;

	_exp_ Timer(EventSemaphore *SEM,ULONG TimeInt);

public:

};


class AsyncTimer:public Timer{


public:
   _exp_ AsyncTimer(EventSemaphore *S,ULONG time);
   void _exp_ Start();
   void _exp_ Stop();
   void SetTime(ULONG time){TimeInterval=time;}
};

class SingleEventTimer:public Timer{

public:
	_exp_ SingleEventTimer(EventSemaphore *SEM,ULONG TimeInt);


};


#endif
