//---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop

#include "Unit_Covert.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma link "SUIButton"
#pragma link "SUIForm"
#pragma link "SUIMemo"
#pragma resource "*.dfm"
TForm_CreateRenju *Form_CreateRenju;
//---------------------------------------------------------------------------
__fastcall TForm_CreateRenju::TForm_CreateRenju(TComponent* Owner)
        : TForm(Owner)
{
}
//---------------------------------------------------------------------------

void __fastcall TForm_CreateRenju::FormShow(TObject *Sender)
{
        BlackMan="";    WriteMan="";
        for(int index=0;index<=225;index++)       Renju[index] = 0;
}
//---------------------------------------------------------------------------
void __fastcall TForm_CreateRenju::OutputRenju(AnsiString *InputBlackMan,AnsiString *InputWriteMan, unsigned char *InputRenju)
{
        *InputBlackMan = BlackMan;
        *InputWriteMan = WriteMan;
        for(int index=0;index<=225;index++)       InputRenju[index] = Renju[index];
}
//---------------------------------------------------------------------------

void __fastcall TForm_CreateRenju::suiButton_OKClick(TObject *Sender)
{
                unsigned char index,Value;
                AnsiString Dream,Test;

                Test = suiMemo1->Text;  //avoid to

                if(Test.Pos("<gomokulog>"))
                {
                        BlackMan = Test.SubString(Test.Pos("black:")+6,Test.Pos("white:")-Test.Pos("black:")-7);
                        WriteMan = Test.SubString(Test.Pos("white:")+6,Test.Pos("[ 1]")-Test.Pos("white:")-7);

                        for(index=1;index<=225;index++)
                        {
                                Test = Test.Delete(1,Test.Pos("==>"));
                                Dream = Test.SubString(4,3);
                                Value = 224 + Dream.c_str()[0]-64;
                                if(Dream.c_str()[2]>=48 && Dream.c_str()[2]<=57) Value -= (10*(Dream.c_str()[1]-48)+(Dream.c_str()[2]-48))*15;
                                else Value -= (Dream.c_str()[1]-48)*15;
                                Renju[index] = Value;
                                if(Test.Pos("==>")==0)  break;
                        }
                        Renju[0] = index;
                }
                else if(suiMemo1->Text.Pos("moves\" VALUE="))
                {
                        Test = Test.SubString(Test.Pos("h8"),1000);
                        Test = Test.SubString(0,Test.Pos(">")-2);
                        for(index=1;index<=225;index++)
                        {
                                Dream = Test.SubString(0,3);
                                Value = 224 + Dream.c_str()[0]-96;
                                if(Dream.c_str()[2]>=48 && Dream.c_str()[2]<=57) Value -= (10*(Dream.c_str()[1]-48)+(Dream.c_str()[2]-48))*15;
                                else Value -= (Dream.c_str()[1]-48)*15;

                                Renju[index] = Value;
                                if(Test.Pos(" ")==0)  break;
                                Test = Test.Delete(1,Test.Pos(" "));
                        }
                        Renju[0] = index;
                }
                else if(suiMemo1->Text.Pos("var moves"))
                {
                        Test = Test.SubString(Test.Pos("h8"),680);
                        Test = Test.SubString(0,Test.Pos("\";var")-1);

                        for(index=1;index<=225;index++)
                        {
                                Dream = Test.SubString(0,3);

                                Value = 224 + Dream.c_str()[0]-96;
                                if(Dream.c_str()[2]>=48 && Dream.c_str()[2]<=57)
                                {
                                        Value -= (10*(Dream.c_str()[1]-48)+(Dream.c_str()[2]-48))*15;
                                        Test = Test.Delete(1,3);
                                }
                                else
                                {
                                        Value -= (Dream.c_str()[1]-48)*15;
                                        Test = Test.Delete(1,2);
                                }
                                Renju[index] = Value;
                                if(Test.Length()<1)     break;
                        }
                        Renju[0] = index;
                }
                if(Renju[0]!=0)  this->Close();

}
//---------------------------------------------------------------------------
void __fastcall TForm_CreateRenju::suiButton_CancelClick(TObject *Sender)
{
        this->Close();
}
//---------------------------------------------------------------------------


