//---------------------------------------------------------------------------

#ifndef Uni_BestMoveThreadH
#define Uni_BestMoveThreadH
//---------------------------------------------------------------------------
#include <Classes.hpp>
//---------------------------------------------------------------------------
class TBestMoveThread : public TThread
{            
private:
        bool StartToFind;
        unsigned char MyBoard[250];
protected:
        void __fastcall Execute();
public:
   int MyValue;
   void __fastcall StartFind(unsigned char Index_Move,unsigned char *Renju);
   void __fastcall EndFind(unsigned char *VCTThread);

        __fastcall TBestMoveThread(bool CreateSuspended);
};
//---------------------------------------------------------------------------
extern "C" int __stdcall StopThinking();
extern "C" int __stdcall Vc3Solver(unsigned char*);
#endif
