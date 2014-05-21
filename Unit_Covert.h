//---------------------------------------------------------------------------

#ifndef Unit_CovertH
#define Unit_CovertH
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
#include "SUIButton.hpp"
#include "SUIForm.hpp"
#include "SUIMemo.hpp"
#include <ExtCtrls.hpp>
//---------------------------------------------------------------------------
class TForm_CreateRenju : public TForm
{
__published:	// IDE-managed Components
        TsuiForm *suiForm1;
        TsuiMemo *suiMemo1;
        TsuiButton *suiButton_OK;
        TsuiButton *suiButton_Cancel;
        void __fastcall suiButton_OKClick(TObject *Sender);
        void __fastcall suiButton_CancelClick(TObject *Sender);
        void __fastcall FormShow(TObject *Sender);
private:	// User declarations
        AnsiString BlackMan;
        AnsiString WriteMan;

        unsigned char Renju[226]; 
public:		// User declarations
        __fastcall TForm_CreateRenju(TComponent* Owner);

        void __fastcall OutputRenju(AnsiString *InputBlackMan,AnsiString *InputWriteMan,unsigned char *Renju);
};
//---------------------------------------------------------------------------
extern PACKAGE TForm_CreateRenju *Form_CreateRenju;
//---------------------------------------------------------------------------
#endif
