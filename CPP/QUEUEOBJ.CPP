#include "QueueObj.h"

#ifndef OS2_INCLUDED
   #ifndef INCL_DOS
     #define INCL_DOS
   #endif
   #ifndef INCL_DOSQUEUES
      #define INCL_DOSQUEUES
   #endif
	#include <os2.h>
#endif


#include "semtimobj.h"

#include <string.h>
#include <stdlib.h>

QueueObj::QueueObj(char *ShMemNm,unsigned long MemSize,
                  char *QueueNm,unsigned long flag,
                  _Action Mem,_Action Queue){

   ShareMemName=0;
   QueueName=0;
   MemBase=0;
   hqueue=0;

   SetMemName(ShMemNm);

   switch(Mem){
      case Create:
         CreateMem(MemSize);
         break;
      case Open:
         OpenMem(MemSize);
         break;
   }

   SetQueueName(QueueNm);

   switch(Queue){
      case Create:
         CreateQueue(flag);
         break;
      case Open:
         OpenQueue();
         break;
   }

}

QueueObj::~QueueObj(){
   if(ShareMemName)
      delete[] ShareMemName;

   if(MemBase)
      DosFreeMem(MemBase);

   if(QueueName)
      delete[] QueueName;

   if(hqueue)
      DosCloseQueue(hqueue);
}

void QueueObj::SetMemName(char *N){
   if(ShareMemName)
      delete[] ShareMemName;

   ShareMemName=new char[strlen(N)+1];
   strcpy(ShareMemName,N);
}

void QueueObj::OpenMem(unsigned long MemSize){
   char *Temp=(char*)_alloca(strlen(ShareMemName) + 20);

   strcpy(Temp,"\\SHAREMEM\\");
   strcat(Temp,ShareMemName);
 
   Error=DosGetNamedSharedMem((PPVOID)&MemBase,(PSZ)Temp,PAG_READ|PAG_WRITE);

   ShMemSize=MemSize;
}

void QueueObj::CreateMem(unsigned long MemSize){
   char *Temp=(char*)_alloca(strlen(ShareMemName) + 20);

   strcpy(Temp,"\\SHAREMEM\\");
   strcat(Temp,ShareMemName);

   ShMemSize=MemSize;

   Error=DosAllocSharedMem((PPVOID)&MemBase,
                           (PSZ)Temp,
                           ShMemSize,
                           PAG_READ|PAG_WRITE|PAG_COMMIT);
}



void QueueObj::SetQueueName(char *N){
   if(QueueName)
      delete[] QueueName;

   QueueName=new char[strlen(N)+1];
   strcpy(QueueName,N);

}

void QueueObj::OpenQueue(){
   char *Temp=(char*)_alloca(strlen(ShareMemName) + 20);

   strcpy(Temp,"\\QUEUES\\");
   strcat(Temp,QueueName);



   Error=DosOpenQueue(&pidQueueOwner,&hqueue,(PSZ)Temp);
}

void QueueObj::CreateQueue(unsigned long flag){
   char *Temp=(char*)_alloca(strlen(ShareMemName) + 20);

   strcpy(Temp,"\\QUEUES\\");
   strcat(Temp,QueueName);

   Error=DosCreateQueue(&hqueue,flag,(PSZ)Temp);
}


//**********************************************************************************

ClientQueueObj::ClientQueueObj(char *ShMemNm,unsigned long MemSize,
       char *QueueNm,unsigned long flag,int Reserv,_Action Mem,_Action Queue):
    QueueObj(ShMemNm,MemSize,QueueNm,flag,Mem,Queue){
   CurrLoc=0;
   CurrLenght=0;
   ReservedLen=Reserv;
}


void * ClientQueueObj::RequestMemBlock(unsigned long BlSize){
   if(CurrLoc==0)
      CurrLoc=MemBase+ReservedLen;

   CurrLenght=BlSize;

   if( (CurrLoc+BlSize) < (MemBase+ShMemSize) )
      CurrLoc+=BlSize;
   else
      CurrLoc=MemBase+ReservedLen;

return CurrLoc;
}

int ClientQueueObj::Write(ULONG Req, ULONG Priority){
  return Error=DosWriteQueue(hqueue,Req,CurrLenght,CurrLoc,Priority);
}

int ClientQueueObj::Write(ULONG Req,char *Buff,ULONG BuffSize, ULONG Priority){
  return Error=DosWriteQueue(hqueue,Req,BuffSize,Buff,Priority);
}

//**********************************************************************************

ServerQueueObj::ServerQueueObj(char *ShMemNm,unsigned long MemSize,
           char *QueueNm,unsigned long flag,
           EventSemaphore *RS,_Action Mem,_Action Queue):
    QueueObj(ShMemNm,MemSize,QueueNm,flag,Mem,Queue){

   ReadSem=RS;
}


void ServerQueueObj::PostSem(){
   if(ReadSem)
      ReadSem->Post();
}

void ServerQueueObj::ResetSem(){
   if(ReadSem)
      ReadSem->Reset();
}

void ServerQueueObj::WaitOnSem(){
   if(ReadSem)
      ReadSem->Wait();
}

void* ServerQueueObj::Read(ULONG ElCode, BOOL wait){
   Error=DosReadQueue(hqueue,&ReqDat,&DataLen,&Entry,ElCode,wait,
            &Priority,(ReadSem?ReadSem->GetHandle() :0) );
return Entry;
}

