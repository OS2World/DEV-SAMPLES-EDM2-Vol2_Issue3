#ifndef __QUEUEOBJ_H__
#define __QUEUEOBJ_H__


#ifndef OS2_INCLUDED
   #ifndef INCL_DOS
     #define INCL_DOS
   #endif
   #ifndef INCL_DOSQUEUES
      #define INCL_DOSQUEUES
   #endif
	#include <os2.h>
#endif


#ifndef _exp_
   #ifdef __DLL__
      #define _exp_ _Export
   #else
      #define _exp_
   #endif
#endif




class EventSemaphore;


class QueueObj{

public:

enum _Action{None, Create,Open};

   _exp_ QueueObj(char *ShMemNm,unsigned long MemSize,
                  char *QueueNm,unsigned long flag=QUE_PRIORITY,
                  _Action Mem=None,_Action Queue=None);
   virtual _exp_ ~QueueObj();

   unsigned long GetError(){return Error;}

   void _exp_ SetMemName(char *N);
   void _exp_ OpenMem(unsigned long MemSize);
   void _exp_ CreateMem(unsigned long MemSize);

   void _exp_ SetQueueName(char *N);
   void _exp_ OpenQueue();
   void _exp_ CreateQueue(unsigned long flag=QUE_PRIORITY);

   PID GetOwner(){return pidQueueOwner;}

protected:

typedef unsigned long HQUEUE;

   HQUEUE hqueue;

   char *ShareMemName, *QueueName;
   char *MemBase;
   unsigned long ShMemSize;

   PID pidQueueOwner;
 
   unsigned long Error;


};





class ClientQueueObj:public QueueObj{

public:
   _exp_ ClientQueueObj(char *ShMemNm,unsigned long MemSize,
          char *QueueNm,unsigned long flag=QUE_PRIORITY,
          int Reserv=0,_Action Mem=Open,_Action Queue=Open);

   int _exp_ Write(ULONG Req, ULONG Priority=15);
   int _exp_ Write(ULONG Req,char *Buff,ULONG BuffSize, ULONG Priority=15);
 
   void * _exp_ RequestMemBlock(unsigned long BlSize);
   void SetReserveLen(int Len){ReservedLen=Len;}

protected:

   char *CurrLoc;
   int CurrLenght;
   int ReservedLen;
};


class ServerQueueObj:public QueueObj{

public:
   _exp_ ServerQueueObj(char *ShMemNm,unsigned long MemSize,
           char *QueueNm,unsigned long flag=QUE_PRIORITY,
           EventSemaphore *RS=NULL,_Action Mem=Create,_Action Queue=Create);

   PID       GetPID(){return ReqDat.pid;}
   ULONG     UserData(){return ReqDat.ulData;}
   void _exp_ PostSem();
   void _exp_ ResetSem();
   void _exp_ WaitOnSem();

   void* _exp_ Read(ULONG ElCode=0, BOOL wait=0);
   void* _exp_ Peek(ULONG ElCode=0);

   ULONG       GetDataLen(){return DataLen;}
   BYTE        GetPriority(){return Priority;}

protected:

   EventSemaphore *ReadSem;
   REQUESTDATA    ReqDat;
   ULONG          DataLen;
   void           *Entry;
   BYTE           Priority;
};


#endif

