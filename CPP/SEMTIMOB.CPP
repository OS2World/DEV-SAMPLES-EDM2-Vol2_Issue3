#include "semtimobj.h"


Semaphore::Semaphore(ULONG attr,char *Nm){
	Name=Nm;
	Attributes=attr;
}

EventSemaphore::EventSemaphore(char *Nm,SemStates Action,ULONG attr,ULONG time):
	Semaphore(attr,Nm){

	hev=0;
	Timeout=time;
	switch (Action){
		case SemOpen:
			Open();
			break;
		case SemCreate:
			Create();
	}
}

EventSemaphore::EventSemaphore(HEV hv,SemStates Action,ULONG attr, ULONG time):
	Semaphore(attr){

	hev=hv;
	Timeout=time;

	if (Action==SemOpen)
		Open();
}

MutexSemaphore::MutexSemaphore(char *Nm,SemStates Action,ULONG attr,ULONG time):
	Semaphore(attr,Nm){

	hmtx=0;
	Timeout=time;
	switch (Action){
		case SemOpen:
			Open();
			break;
		case SemCreate:
			Create();
	}
}

MutexSemaphore::MutexSemaphore(HEV hv,SemStates Action,ULONG attr, ULONG time):
	Semaphore(attr){

	hmtx=hv;
	Timeout=time;

	if (Action==SemOpen)
		Open();
}


ULONG WinRequestMutexSemaphore(MutexSemaphore &Sem, ULONG time){
   return WinRequestMutexSem(Sem.hmtx,time);
}       
 
 
ULONG WinWaitEventSemaphore(EventSemaphore &Sem,ULONG time){
   return WinWaitEventSem(Sem.hev,time);
}


Timer::Timer(EventSemaphore *SEM,ULONG TimeInt){
     Sem=SEM;
     TimeInterval=TimeInt;
};

AsyncTimer::AsyncTimer(EventSemaphore *S,ULONG time):Timer(S,time){
}

void AsyncTimer::Start(){
 Error= DosStartTimer(TimeInterval,(HSEM) (Sem->hev),&HTimer);
}

void AsyncTimer::Stop(){
   Error=DosStopTimer(HTimer);
}

