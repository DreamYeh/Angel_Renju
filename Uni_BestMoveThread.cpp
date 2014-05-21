//---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop

#include "Uni_BestMoveThread.h"
#pragma package(smart_init)
//---------------------------------------------------------------------------

__fastcall TBestMoveThread::TBestMoveThread(bool CreateSuspended)
        : TThread(CreateSuspended)
{
        FreeOnTerminate = true;
        StartToFind = false;
        MyValue = 0;
}
//---------------------------------------------------------------------------
void __fastcall TBestMoveThread::StartFind(unsigned char Index_Move,unsigned char *Renju)
{
        memset(MyBoard,0,sizeof(MyBoard));
        MyBoard[1]=(unsigned char)1;
        MyBoard[2]=(unsigned char)9;
        MyBoard[6]=(unsigned char)2;//HashTableSize;0--8M, 1--16M,2--32M,
        MyBoard[8]=Index_Move;
        memcpy(&MyBoard[9],&Renju[1],Index_Move);
        MyValue = 0;    StartToFind = true;
        Resume();
}
//---------------------------------------------------------------------------

void __fastcall TBestMoveThread::Execute()
{
        //---- Place thread code here ----
        while(!Terminated)
        {
                if(StartToFind && MyValue==0)
                {
                   try{
                        MyValue = Vc3Solver(MyBoard);
                   }
                   catch(...)
                   {
                        StartToFind=false;
                   }
                   StartToFind=false;
                }
        }
}

//---------------------------------------------------------------------------
void __fastcall TBestMoveThread::EndFind(unsigned char *VCTThread)
{
        if(StartToFind)
        {
                while(StopThinking()!=0);
                StartToFind=false;
        }
        memcpy(VCTThread,MyBoard,sizeof(MyBoard));
}
//---------------------------------------------------------------------------

