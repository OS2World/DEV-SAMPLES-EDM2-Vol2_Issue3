#include <QueueObj.h>
#include <iostream.h>
#include "QDefs.h"



int main(){

   int loop,num;
   QueueMessage *Mesg;

   //Create Client Queue
   ClientQueueObj InQueue(SMemName,NemSize,QName);

   cout<<"Queue Client Active !"<<endl;

   loop=1;


   while(loop){
      cout<<"Number Please (-1 to exit) "<<endl;
      cin>>num;
      if(num==-1){
         loop=0;
         cout<<"Have a nice Day!"<<endl;
      }

      Mesg=(QueueMessage *)InQueue.RequestMemBlock(sizeof(QueueMessage));
      Mesg->Number=num;
      InQueue.Write(0);

   }

return 0;
}
