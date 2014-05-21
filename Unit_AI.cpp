//---------------------------------------------------------------------------
#include <vcl.h>
#pragma hdrstop
#include "Unit_AI.h"
#pragma package(smart_init)


extern "C" int __stdcall StopThinking();
extern "C" int __stdcall Vc3Solver(unsigned char*);
int MyValue;
unsigned char MyBoard[250];
//---------------------------------------------------------------------------
class TMyThread:public TThread
{
        public:
                __fastcall TMyThread(void);
        private:
                void __fastcall Execute(void);
};
//---------------------------------------------------------------------------

_fastcall TMyThread::TMyThread(void):TThread(true)
{
        FreeOnTerminate=true;
        Resume();
}
//---------------------------------------------------------------------------
void __fastcall TMyThread::Execute()//°õ¦æºü function
{
        //MyValue = Vc3Solver(MyBoard);
                   try{
                        MyValue = Vc3Solver(MyBoard);
                   }
                   catch(...)
                   {
                        Terminate();
                   }
}
//---------------------------------------------------------------------------
unsigned char __fastcall GetBestVCFStone(unsigned char Index_Move,unsigned char *Renju,int milliseconds)
{
        MyValue = 0;
        memset(MyBoard,0,sizeof(MyBoard));
        MyBoard[1]=(unsigned char)1;
        MyBoard[2]=(unsigned char)9;
        MyBoard[6]=(unsigned char)2;//HashTableSize;0--8M, 1--16M,2--32M,
        MyBoard[8]=Index_Move;
        memcpy(&MyBoard[9],&Renju[1],Index_Move);
        DWORD Time1 = GetTickCount();
        TMyThread *MyThread = new TMyThread();
        while((GetTickCount()-Time1)<milliseconds && MyValue<10000);
        StopThinking();
        MyThread->Terminate();

        if(MyValue>=10000)      return MyBoard[0];
        else return 255;
}
//---------------------------------------------------------------------------

