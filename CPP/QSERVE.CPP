#include <QueueObj.h>
#include <iostream.h>
#include "QDefs.h"

#include "semtimobj.h"
   




int main(){

   int loop;
   QueueMessage *Mesg;
   EventSemaphore Sem((char*)NULL,Semaphore::SemCreate,DC_SEM_SHARED);;

   ServerQueueObj InQueue(SMemName,NemSize,QName,0,&Sem);


   loop=1;

   Sem.Reset();

   cout<<"Queue Server Active !"<<endl;

   while(loop){

      Mesg=(QueueMessage *) InQueue.Read(0,1);
      while(InQueue.GetError() ){
         Sem.Wait();
         Mesg=(QueueMessage *) InQueue.Read(0,1);
      }

      if(Mesg->Number == -1){
         loop=0;
         cout<<"Terminating"<<endl;
      }
      else{
         cout<<"Number= "<<Mesg->Number<<endl;
      }
   }

return 0;
}
