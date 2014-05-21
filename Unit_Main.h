//---------------------------------------------------------------------------

#ifndef Unit_MainH
#define Unit_MainH
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
#include <ExtCtrls.hpp>
#include <Menus.hpp>
#include <ComCtrls.hpp>
#include <ImgList.hpp>
#include <ToolWin.hpp>
#include <jpeg.hpp>
#include <Dialogs.hpp>

#include "SUIForm.hpp"
#include "SUIMainMenu.hpp"
#include "SUIButton.hpp"
#include "SUIMemo.hpp"
#include "SUIToolBar.hpp"
#include "SUIPageControl.hpp"
#include "SUITabControl.hpp"
#include "SUIStatusBar.hpp"
#include "CGAUGES.h"
#include "SUIImagePanel.hpp"
//---------------------------------------------------------------------------
class TForm_Main : public TForm
{
__published:	// IDE-managed Components
        TsuiForm *suiForm1;
        TsuiMainMenu *suiMainMenu1;
        TMenuItem *Menu_File;
        TMenuItem *Menu_FileSave;
        TMenuItem *N1;
        TMenuItem *Menu_FileExit;
        TMenuItem *Menu_Edit;
        TMenuItem *View1;
        TMenuItem *Move1;
        TMenuItem *Option1;
        TMenuItem *Help1;
        TsuiToolBar *suiToolBar_New;
        TToolButton *ToolButton_New;
        TToolButton *ToolButton_Open;
        TToolButton *ToolButton_Save;
        TImageList *ImageList1;
        TImage *Image1;
        TsuiPageControl *suiPageControl_Show;
        TsuiTabSheet *suiTabSheet1;
        TsuiTabSheet *suiTabSheet2;
        TsuiStatusBar *suiStatusBar1;
        TSaveDialog *SaveDialog1;
        TsuiMemo *suiMemo_BBS;
        TsuiMemo *suiMemo_Commet;
        TToolButton *ToolButton3;
        TToolButton *ToolButton4;
        TMenuItem *Menu_FileNew;
        TMenuItem *Menu_FileOpen;
        TOpenDialog *OpenDialog1;
        TMenuItem *Menu_EditFindVCF1;
        TLabel *Label1;
        TLabel *Label2;
        TLabel *Label3;
        TLabel *Label4;
        TLabel *Label5;
        TLabel *Label6;
        TLabel *Label7;
        TLabel *Label8;
        TLabel *Label9;
        TLabel *Label10;
        TLabel *Label11;
        TLabel *Label12;
        TLabel *Label13;
        TLabel *Label14;
        TLabel *Label15;
        TToolButton *ToolButton_Update;
        TToolButton *ToolButton6;
        TToolButton *ToolButton_FirstMove;
        TToolButton *ToolButton_Up;
        TToolButton *ToolButton_Back;
        TToolButton *ToolButton_Forward;
        TToolButton *ToolButton_Down;
        TToolButton *ToolButton_LastMove;
        TToolButton *ToolButton1;
        TLabel *Label16;
        TLabel *Label17;
        TLabel *Label18;
        TLabel *Label19;
        TLabel *Label20;
        TLabel *Label21;
        TLabel *Label22;
        TLabel *Label23;
        TLabel *Label24;
        TLabel *Label25;
        TLabel *Label26;
        TLabel *Label27;
        TLabel *Label28;
        TLabel *Label29;
        TLabel *Label30;
        TMenuItem *Menu_EditUpdateMode;
        TMenuItem *N4;
        TMenuItem *Menu_MoveBack;
        TMenuItem *Menu_MoveForward;
        TMenuItem *N5;
        TMenuItem *Menu_MoveDown;
        TMenuItem *Menu_MoveUp;
        TMenuItem *N6;
        TMenuItem *Menu_MoveFirstMove;
        TMenuItem *Menu_MoveLastMove;
        TMenuItem *Menu_HelpProduce;
        TMenuItem *Menu_EditConvertBoard;
        TMenuItem *N2;
        TsuiButton *suiButton_AttackDefense;
        TMenuItem *Menu_ViewShowFordiee;
        TsuiButton *suiButton_Defense;
        TsuiButton *suiButton_Attack;
        TToolButton *ToolButton2;
        TToolButton *ToolButton5;
        TToolButton *ToolButton7;
        TMenuItem *Menu_Play;
        TMenuItem *Menu_PlayAttack;
        TMenuItem *Menu_PlayAttackandDefense;
        TMenuItem *Menu_PlayDefense;
        TMenuItem *Menu_EditFindVCT;
        TTimer *Timer1;
        TsuiButton *suiButton_Stop;
        TToolButton *ToolButton8;
        TToolButton *ToolButton_VCF;
        TToolButton *ToolButton10;
        TToolButton *ToolButton_Din;
        TMenuItem *Menu_HelpGetMyScore;
        TMenuItem *Menu_EditFindNextVCF;
        TsuiPanel *suiPanel1;
        TLabel *Label31;
        TLabel *Label32;
        TLabel *Label33;
        TLabel *Label34;
        TLabel *Label_Depth;
        TLabel *Label_Value;
        TLabel *Label37;
        TLabel *Label_Nodes;
        TMenuItem *N3;
        TMenuItem *InputTimeMenu_Edit;
        void __fastcall Menu_FileSaveClick(TObject *Sender);
        void __fastcall Menu_FileOpenClick(TObject *Sender);
        void __fastcall FormCreate(TObject *Sender);
        void __fastcall Menu_EditFindVCF1Click(TObject *Sender);
        void __fastcall Image1MouseMove(TObject *Sender, TShiftState Shift,
          int X, int Y);
        void __fastcall Image1MouseDown(TObject *Sender,
          TMouseButton Button, TShiftState Shift, int X, int Y);
        void __fastcall Menu_MoveBackClick(TObject *Sender);
        void __fastcall Menu_MoveForwardClick(TObject *Sender);
        void __fastcall Menu_MoveDownClick(TObject *Sender);
        void __fastcall Menu_MoveUpClick(TObject *Sender);
        void __fastcall Menu_MoveFirstMoveClick(TObject *Sender);
        void __fastcall Menu_MoveLastMoveClick(TObject *Sender);
        void __fastcall Menu_FileExitClick(TObject *Sender);
        void __fastcall ToolButton_UpdateClick(TObject *Sender);
        void __fastcall Menu_HelpProduceClick(TObject *Sender);
        void __fastcall Menu_EditConvertBoardClick(TObject *Sender);
        void __fastcall Menu_FileNewClick(TObject *Sender);
        void __fastcall Menu_PlayAttackClick(TObject *Sender);
        void __fastcall Menu_PlayAttackandDefenseClick(TObject *Sender);
        void __fastcall Menu_PlayDefenseClick(TObject *Sender);
        void __fastcall Timer1Timer(TObject *Sender);
        void __fastcall FormClose(TObject *Sender, TCloseAction &Action);
        void __fastcall Menu_EditFindVCTClick(TObject *Sender);
        void __fastcall suiButton_StopClick(TObject *Sender);
        void __fastcall ToolButton_DinClick(TObject *Sender);
        void __fastcall Menu_EditFindNextVCFClick(TObject *Sender);
        void __fastcall InputTimeMenu_EditClick(TObject *Sender);
        void __fastcall ToolButton7Click(TObject *Sender);
private:	// User declarations
        Graphics::TBitmap *Bmp_Black,*Bmp_Write;
        TJPEGImage   *Jpeg_Board;

        AnsiString BlackMan,WriteMan;
        unsigned char Renju[226],Index_Move;

        char GOMOKU[15][15],MyStyle[15][15];
        int TStyle[15][15];                   //debug

        int ProcessNum,MyTime;

        void __fastcall GiveNewStone(unsigned char Value);
        void __fastcall GiveNewStone(unsigned char x,unsigned char y);
        void __fastcall BBSFormProduce();
        void __fastcall DrawBoard();

        //unsigned char Index_Move;
public:		// User declarations
        __fastcall TForm_Main(TComponent* Owner);
};
//---------------------------------------------------------------------------
extern PACKAGE TForm_Main *Form_Main;
//---------------------------------------------------------------------------
#endif
