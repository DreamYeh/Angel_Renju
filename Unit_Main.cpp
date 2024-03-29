//---------------------------------------------------------------------------
#include <stdio.h>
#include <stdlib.h>
#include <vcl.h>
#pragma hdrstop

#include "Unit_Covert.h"
#include "Unit_Gomoku.h"
#include "Uni_BestMoveThread.h"
#include "Unit_AI.h"

#include "Unit_Main.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma link "SUIForm"
#pragma link "SUIMainMenu"
#pragma link "SUIButton"
#pragma link "SUIMemo"
#pragma link "SUIToolBar"
#pragma link "SUIPageControl"
#pragma link "SUITabControl"
#pragma link "SUIStatusBar"
#pragma link "CGAUGES"
#pragma link "SUIImagePanel"
#pragma resource "*.dfm"
TForm_Main *Form_Main;


TBestMoveThread *BestMoveThread;
//---------------------------------------------------------------------------
__fastcall TForm_Main::TForm_Main(TComponent* Owner)
        : TForm(Owner)
{
        memset(Renju,0,sizeof(Renju));       Index_Move=0;
        for(int x=0;x<15;x++)       for(int y=0;y<15;y++)
        {
                GOMOKU[x][y]   = 0;
        }
        MyTime = 100;
}
//---------------------------------------------------------------------------
void __fastcall TForm_Main::FormCreate(TObject *Sender)
{
        Jpeg_Board = new TJPEGImage();
        Jpeg_Board->LoadFromFile("Applet//Board.jpg");
        Image1->Canvas->Draw(0,0,Jpeg_Board);
        Image1->Canvas->Brush->Style = bsClear;//透明背景
        Image1->Canvas->Font->Size=14;

        Bmp_Black = new Graphics::TBitmap;
        Bmp_Black->LoadFromFile("Applet\\black32.bmp");
        Bmp_Black->TransparentColor = Bmp_Black->Canvas->Pixels[0][0];
        Bmp_Black->Transparent = true;

        Bmp_Write = new Graphics::TBitmap;
        Bmp_Write->LoadFromFile("Applet\\white32.bmp");
        Bmp_Write->TransparentColor = Bmp_Write->Canvas->Pixels[0][0];
        Bmp_Write->Transparent = true;

        suiForm1->DoubleBuffered = true;//防止閃爍

        BestMoveThread = new TBestMoveThread(true);
}
//---------------------------------------------------------------------------

void __fastcall TForm_Main::GiveNewStone(unsigned char Value)
{
        if(Value>224)   return; //avoid error 2008.10.31

        Renju[++Index_Move] = Value;
        Renju[0] = Index_Move;
        BBSFormProduce();
        DrawBoard();
}
//---------------------------------------------------------------------------

void __fastcall TForm_Main::GiveNewStone(unsigned char x,unsigned char y)
{
        Renju[++Index_Move] = x + 225 - (y+1)*15;
        Renju[0] = Index_Move;
        BBSFormProduce();
        DrawBoard();
}
//---------------------------------------------------------------------------

void __fastcall TForm_Main::BBSFormProduce()
{
        AnsiString Dream;
        suiMemo_BBS->Clear();

        suiMemo_BBS->Lines->Append("走了"+AnsiString(Renju[0])+"著");

        suiMemo_BBS->Lines->Append("<gomokulog>");
        suiMemo_BBS->Lines->Append("black:"+BlackMan);
        suiMemo_BBS->Lines->Append("white:"+WriteMan);
        for(int index=1;index<=Renju[0];index+=2)
        {
                Dream = "["+AnsiString(index)+"]● ==> "+ AnsiString((char)(Renju[index]%15+65))+StrToInt((224-Renju[index])/15+1);

                if(Renju[index+1]!=0)
                {
                        Dream += " ["+AnsiString(index+1)+"]○ ==> "+AnsiString((char)(Renju[index+1]%15+65))+StrToInt((224-Renju[index+1])/15+1);
                        suiMemo_BBS->Lines->Append(Dream);
                }
                else
                {
                        suiMemo_BBS->Lines->Append(Dream);
                        break;
                }
        }

        suiMemo_BBS->Lines->Append("</gomokulog>");
}
//---------------------------------------------------------------------------
void __fastcall TForm_Main::DrawBoard()
{

        int x,y,index;
        //Draw=================================================================
        Image1->Canvas->Draw(0,0,Jpeg_Board);
        for(index=1;index<=Renju[0] && index<=Index_Move;index++)
        {
                x = (Renju[index]%15+1) * 32;
                y = ((224-Renju[index])/15+1)*32;
                Image1->Canvas->Draw(x-16,512-y-16,index%2?Bmp_Black:Bmp_Write);

                Image1->Canvas->Font->Color = index%2?clWhite:clBlack;
                if(index>9)x-=5;
                if(index>99)x-=5;
                Image1->Canvas->TextOutA(x-6,512-y-12,AnsiString(index));
        }
        suiStatusBar1->Panels->Items[2]->Text="Move"+AnsiString(Index_Move);

        //=====================================================================
        ToolButton_FirstMove->Enabled = (Index_Move>0);
        ToolButton_Back->Enabled = (Index_Move>0);
        ToolButton_Forward->Enabled = (Index_Move<Renju[0]);
        ToolButton_LastMove->Enabled = (Index_Move<Renju[0]);
        //=====================================================================
        //Win and Lose=========================================================
        if(Index_Move%2)
        {
                if(MyStyle[Renju[Index_Move]%15][(224-Renju[Index_Move])/15]==13){       ShowMessage("Black Win");        ToolButton_Update->Down = false;}
                if(MyStyle[Renju[Index_Move]%15][(224-Renju[Index_Move])/15]>=10 && MyStyle[Renju[Index_Move]%15][(224-Renju[Index_Move])/15]<=12){       ShowMessage("Write Win because Black JinShou");    ToolButton_Update->Down = false;}

        }
        else
        {
                if(MyStyle[Renju[Index_Move]%15][(224-Renju[Index_Move])/15]==13){       ShowMessage("Write Win");    ToolButton_Update->Down = false;}
        }
        //Get New Broad========================================================
        for(x=0;x<15;x++)       for(y=0;y<15;y++)
        {
                GOMOKU[x][y] = 0;
        }
        for(index=1;index<=Index_Move/*Renju[0]*/;index++)
        {
                x = Renju[index]%15;
                y = (224-Renju[index])/15;
                GOMOKU[x][y] = index%2? 1:-1;
        }

        //Get new Style========================================================
        for(x=0;x<15;x++)       for(y=0;y<15;y++)
        {
                MyStyle[x][y] = getstyle(GOMOKU, x, y, Index_Move%2?-1:1, 1);
                //TStyle[x][y] = Test_dirchk(GOMOKU, x, y, Index_Move%2?-1:1, 1,1,0);
        }
        //=====================================================================
        if(Menu_ViewShowFordiee->Checked)
        {
                int MX,MY,ForMyStyle;
                for(x=0;x<15;x++)       for(y=0;y<15;y++)
                {
                        ForMyStyle = getstyle(GOMOKU, x, y, 1, 1);
                        if(ForMyStyle>=10 && ForMyStyle<=12)
                        {
                                MX=x*32+32;     MY=480-y*32;
                                Image1->Canvas->Pen->Width=2;
                                Image1->Canvas->Pen->Color=clRed;
                                Image1->Canvas->MoveTo(MX-5,MY-5);    Image1->Canvas->LineTo(MX+5,MY+5);
                                Image1->Canvas->MoveTo(MX-5,MY+5);    Image1->Canvas->LineTo(MX+5,MY-5);
                        }
                }
        }
}
//---------------------------------------------------------------------------
void __fastcall TForm_Main::Menu_FileNewClick(TObject *Sender)
{
        memset(Renju,0,sizeof(Renju));
        ToolButton_Update->Down = true;
        Index_Move = 0;
        DrawBoard();
}
//---------------------------------------------------------------------------

void __fastcall TForm_Main::Menu_FileOpenClick(TObject *Sender)
{
        unsigned char Value,index;
        if(OpenDialog1->Execute())
        {
                if(!FileExists(OpenDialog1->FileName))  return;

                if (ExtractFileExt(OpenDialog1->FileName).LowerCase() == ".pos")
                {
                        FILE *fp;
                        fp = fopen( OpenDialog1->FileName.c_str(), "rb");
                        Renju[0] = fgetc(fp);
                        for(unsigned char index=1;index<=Renju[0];index++)  Renju[index] = fgetc(fp);
                        fclose(fp);
                        BBSFormProduce();

                        Index_Move = Renju[0];
                        DrawBoard();
                }
                else if(ExtractFileExt(OpenDialog1->FileName).LowerCase() == ".sgf")
                {
                        suiMemo_Commet->Clear();
                        suiMemo_Commet->Lines->LoadFromFile(OpenDialog1->FileName);
                        AnsiString Dream = suiMemo_Commet->Text;
                        for(index=1;index<225;index+=2)
                        {
                                if(Dream.Pos(";B[")==0) break;
                                Dream = Dream.Delete(1,Dream.Pos(";B[")+2);
                                Value = 224 + Dream.c_str()[0]-96;
                                Value -= (Dream.c_str()[1]-96)*15;
                                Renju[index] = Value;

                                if(Dream.Pos(";W[")==0) break;
                                Dream = Dream.Delete(1,Dream.Pos(";W[")+2);
                                Value = 224 + Dream.c_str()[0]-96;
                                Value -= (Dream.c_str()[1]-96)*15;
                                Renju[index+1] = Value;
                        }
                        Renju[0]=index;
                        suiMemo_Commet->Clear();
                        BBSFormProduce();

                        Index_Move = Renju[0];
                        DrawBoard();
                }
        }

}
//---------------------------------------------------------------------------

void __fastcall TForm_Main::Menu_FileSaveClick(TObject *Sender)
{
        AnsiString SaveFileName;

        if(!Renju[0])   return;
        if(SaveDialog1->Execute())
        {
                if (SaveDialog1->FilterIndex==1)
                {
                        SaveFileName = SaveDialog1->FileName;
                        if(ExtractFileExt(SaveDialog1->FileName).LowerCase()!=".pos")   SaveFileName = SaveDialog1->FileName+".pos";

                        FILE *fp;
                        fp = fopen( SaveFileName.c_str(), "wb");
                        fputc(Renju[0],fp);
                        for(unsigned char index=1;index<=Renju[0];index++)  fputc(Renju[index],fp);
                        fclose(fp);
                }
                else if (SaveDialog1->FilterIndex==2)
                {
                        suiMemo_Commet->Clear();
                        AnsiString Dream =
                        "(;\
                           GM[5]FF[4]AP[NNRS:1.1.14]\
                           US[Brought to you by Online Renju Class Game Server]\
                           CP[\
                           Copyright This game was played on the\
                           Online Renju Class Game Server.\
                           Permission to reproduce this game is given,\
                           as long as this copyright notice is preserved.]\
                           PC[NNRS Worldwide: compute.com.tw 9696]\
                           SZ[15]\
                           SW[NO]\
                           5A[ih]\
                           GN[DreamYeh(B) : RIFrule]\
                           PW["+WriteMan+"]WR[9d*]\
                           PB["+BlackMan+"]BR[9d*]\
                           DT[2008-03-30]";

                        for(int index=1;index<=Renju[0];index+=2)
                        {
                                if(Renju[index]==0)break;
                                Dream = Dream+";B[";
                                Dream = Dream+AnsiString((char)((Renju[index]+1)%15+96))+AnsiString((char)((224-Renju[index])/15+97))+"]";
                                if(Renju[index+1]==0)break;
                                Dream = Dream+";W[";
                                Dream = Dream+AnsiString((char)((Renju[index+1]+1)%15+96))+AnsiString((char)((224-Renju[index+1])/15+97))+"]";
                        }
                        suiMemo_Commet->Text = Dream;
                        suiMemo_Commet->Lines->SaveToFile(SaveDialog1->FileName);
                }
        }
}
//---------------------------------------------------------------------------

void __fastcall TForm_Main::Menu_FileExitClick(TObject *Sender)
{
        Close();
}
//---------------------------------------------------------------------------
void __fastcall TForm_Main::ToolButton_UpdateClick(TObject *Sender)
{
        Menu_EditUpdateMode->Checked = !Menu_EditUpdateMode->Checked;
        if(Menu_EditUpdateMode->Checked)        suiStatusBar1->Panels->Items[4]->Text = "Update Mode";
        else    suiStatusBar1->Panels->Items[4]->Text = "Display Mode";
}
//---------------------------------------------------------------------------
void __fastcall TForm_Main::Menu_EditConvertBoardClick(TObject *Sender)
{
        TForm_CreateRenju *Form_CreateRenju = new TForm_CreateRenju(Application);
        Form_CreateRenju->ShowModal();
        Form_CreateRenju->OutputRenju(&BlackMan,&WriteMan,Renju);
        delete Form_CreateRenju;

        if(Renju[0]==0) return;

        BBSFormProduce();

        Index_Move = Renju[0];
        DrawBoard();
}
//---------------------------------------------------------------------------

void __fastcall TForm_Main::Menu_EditFindVCF1Click(TObject *Sender)
{
 	unsigned char temp[250];
        int index,x,y;
        BestMoveThread->StartFind(Index_Move,Renju);
        for(index=0;index<1000 && BestMoveThread->MyValue==0;index++)  Sleep(1);
        BestMoveThread->EndFind(temp);
 	if(BestMoveThread->MyValue>=20000)  //found VCF,
 	{
                for(index=0;index<=BestMoveThread->MyValue-20000;index++)
                {
                        x = (temp[index]%15+1) * 32;
                        y = ((224-temp[index])/15+1)*32;
                        suiStatusBar1->Panels->Items[0]->Text="VCF!";
                        Image1->Canvas->Font->Color = clNavy;
                        if(index>9)x-=5;
                        if(index>99)x-=5;
                        Image1->Canvas->TextOutA(x-6,512-y-12,AnsiString(index+1));
                }
 	}
        else // no VCF.
 	{
                suiStatusBar1->Panels->Items[0]->Text="No VCF thread";
 	}
}
//---------------------------------------------------------------------------
void __fastcall TForm_Main::Menu_EditFindVCTClick(TObject *Sender)
{
 	unsigned char temp[250];

        BestMoveThread->StartFind(Index_Move,Renju);

        ProcessNum=0;
        Timer1->Enabled=true;
        suiButton_Stop->Visible=true;
        suiButton_Attack->Visible=false; suiButton_AttackDefense->Visible=false; suiButton_Defense->Visible=false;

}
//---------------------------------------------------------------------------
void __fastcall TForm_Main::Menu_EditFindNextVCFClick(TObject *Sender)
{
 	unsigned char temp[250],gku[15][15];
        int index,x,y,tPos[225],tNum=0,tVCF[225];

        Image1->Canvas->Pen->Width=3;


        //Get New Broad========================================================
        for(x=0;x<15;x++)       for(y=0;y<15;y++)
        {
                gku[x][y] = 0;
        }
        for(index=1;index<=Index_Move;index++)
        {
                x = Renju[index]%15;
                y = (224-Renju[index])/15;
                gku[x-1][y-1] = 1;
                gku[x][y-1] = 1;
                gku[x+1][y-1] =1;
                gku[x-1][y] = 1;
                gku[x+1][y] = 1;
                gku[x-1][y+1] = 1;
                gku[x][y+1] = 1;
                gku[x+1][y+1] = 1;

        }
        for(index=1;index<=Index_Move;index++)
        {
                x = Renju[index]%15;
                y = (224-Renju[index])/15;
                gku[x][y] = 0;
        }
        //=====================================================================
        for(x=0;x<15;x++)for(y=0;y<15;y++)
        {
                if(gku[x][y]==0)continue;
                //if(MyStyle[x][y]>5)continue;
                Renju[Index_Move+2] = x + 225 - (y+1)*15;
                BestMoveThread->StartFind(Index_Move+2,Renju);
                for(index=0;index<50 && BestMoveThread->MyValue==0;index++)  Sleep(1);
                BestMoveThread->EndFind(temp);
 	        if(BestMoveThread->MyValue>=10000)  //found VCF,
 	        {
                        tPos[tNum] = temp[0];
                        tVCF[tNum] = BestMoveThread->MyValue;
                        tNum++;
 	        }
                Renju[Index_Move+2] = 0;
        }

        for(index=0;index<tNum;index++)
        {
                Image1->Canvas->Pen->Color = tVCF[index]>=20000?clGreen:clBlue;
                x = (tPos[index]%15+1) * 32;
                y = ((224-tPos[index])/15+1)*32;
                Image1->Canvas->Ellipse(x-5,512-y-5,x+5,512-y+5);
        }



        BestMoveThread->StartFind(Index_Move,Renju);
        for(index=0;index<200 && BestMoveThread->MyValue==0;index++)  Sleep(1);
        BestMoveThread->EndFind(temp);
 	if(BestMoveThread->MyValue>=10000)  //found VCF,
 	{
                Image1->Canvas->Pen->Color = BestMoveThread->MyValue>=20000?clRed:clYellow;
                x = (tPos[index]%15+1) * 32;
                y = ((224-tPos[index])/15+1)*32;
                Image1->Canvas->Ellipse(x-5,512-y-5,x+5,512-y+5);
 	}        

}
//---------------------------------------------------------------------------

void __fastcall TForm_Main::ToolButton_DinClick(TObject *Sender)
{
        Renju[0]=Index_Move;
        for(int Index=Index_Move+1;Index<sizeof(Renju);Index++)   Renju[Index]=0;
        BBSFormProduce();
        DrawBoard();
}
//---------------------------------------------------------------------------


void __fastcall TForm_Main::Menu_PlayAttackClick(TObject *Sender)
{
        int FinalValue;
        unsigned char Max_x,Max_y;
        suiStatusBar1->Panels->Items[0]->Text="";
        srand(time(NULL));
        if(Index_Move==0)
        {
                GiveNewStone(7,7);
                return;
        }
        else if(Index_Move==1)
        {
                if(GOMOKU[7][7]==0)    GiveNewStone(7,7);
                else if(rand()%2)                GiveNewStone(7,8);
                else GiveNewStone(8,8);
                return;
        }
        GetMyBestMove(&Max_x,&Max_y,GOMOKU,Index_Move%2?-1:1,3);
        GiveNewStone(Max_x,Max_y);

        /*
        FinalValue = GetBestMove(Renju,GOMOKU,Index_Move,2);
        GiveNewStone(Renju[Index_Move+1]);
        */
        Label_Value->Caption=AnsiString(FinalValue);
}
//---------------------------------------------------------------------------
void __fastcall TForm_Main::Menu_PlayAttackandDefenseClick(TObject *Sender)
{//2008.10.30
        unsigned char temp[250],x,y;
        int Attack[15][15],Defense[15][15],Max_x,Max_y,index;
        int color=Index_Move%2?-1:1,FinalValue,tNodes=0;


        suiForm1->Refresh();
        suiStatusBar1->Panels->Items[0]->Text="";

        srand(time(NULL));

        if(Index_Move==0)
        {
                GiveNewStone(7,7);
                return;
        }
        else if(Index_Move==1)
        {
                if(GOMOKU[7][7]==0)    GiveNewStone(7,7);
                else if(rand()%2)                GiveNewStone(7,8);
                else GiveNewStone(8,8);
                return;
        }

        Label_Depth->Caption = "1";     suiPanel1->Refresh();
        //5connect=====================================================
        for(x=0;x<15;x++)       for(y=0;y<15;y++)
        {
                Attack[x][y]=-1024;
                if(GOMOKU[x][y]!=0)     continue;
                Attack[x][y] = GetMyScore(GOMOKU, x, y, color);
                if(Attack[x][y]>=1024){GiveNewStone(x,y);  return;}//連五當然
        }
        for(x=0;x<15;x++)       for(y=0;y<15;y++)
        {
                Defense[x][y]=-1024;
                if(GOMOKU[x][y]!=0)     continue;
                Defense[x][y] = GetMyScore(GOMOKU, x, y, -color);
                if(Defense[x][y]>=1024){GiveNewStone(x,y);  return;}//對方連五當然擋
        }
        //VCF mode====================================================

        BestMoveThread->StartFind(Index_Move,Renju);
        for(index=0;index<MyTime*10 && BestMoveThread->MyValue==0;index++)
        {
                tNodes++;       Label_Nodes->Caption = AnsiString(tNodes);     suiPanel1->Refresh();
                Sleep(1);
        }
        BestMoveThread->EndFind(temp);
        if(BestMoveThread->MyValue>=10000)
        {
                GiveNewStone(temp[0]);
                suiStatusBar1->Panels->Items[0]->Text=BestMoveThread->MyValue>=20000?"VCF!!":"VCT";
                return;
        }
        //==========================================================
        Label_Depth->Caption = "3";     suiPanel1->Refresh();

        for(x=0;x<15;x++)       for(y=0;y<15;y++)
        {
                tNodes+=100;       Label_Nodes->Caption = AnsiString(tNodes);     suiPanel1->Refresh();
                if(/*MyStyle[x][y]>3 ||*/ MyStyle[x][y]==0)     continue;
                Renju[Index_Move+1] = x + 225 - (y+1)*15;
                BestMoveThread->StartFind(Index_Move+2,Renju);
                for(index=0;index<MyTime && BestMoveThread->MyValue==0;index++)     Sleep(1);
                BestMoveThread->EndFind(temp);
 	        if(BestMoveThread->MyValue>=10000)  //found VCF,
 	        {
                        Attack[x][y]+=512;
                        suiMemo_Commet->Lines->Append(AnsiString((char)(x+65))+AnsiString(y+1));
 	        }
                Renju[Index_Move+2] = 0;
        }
        //==========================================================
        Label_Depth->Caption = "5";     suiPanel1->Refresh();

        for(x=0;x<15;x++)       for(y=0;y<15;y++)
        {
                tNodes+=100;       Label_Nodes->Caption = AnsiString(tNodes);     suiPanel1->Refresh();
                if(Attack[x][y]<10)     continue;
                Index_Move++;    Renju[Index_Move]=x + 225 - (y+1)*15;
                BestMoveThread->StartFind(Index_Move,Renju);
                for(index=0;index<MyTime && BestMoveThread->MyValue==0;index++)  Sleep(1);
                BestMoveThread->EndFind(temp);
                if(BestMoveThread->MyValue>=10000)
                {
                        Attack[x][y]-=1024;
                }
                Renju[Index_Move]=0;   Index_Move--;
        }
        //==========================================================
        Label_Depth->Caption = "7";     FinalValue=-1000;
        for(x=0;x<15;x++)       for(y=0;y<15;y++)
        {
                tNodes+=100;       Label_Nodes->Caption = AnsiString(tNodes);     suiPanel1->Refresh();

                if(Attack[x][y]<512)
                {
                        if(Attack[x][y]<10 && Defense[x][y]<10)     continue;
                        GOMOKU[x][y]=color;   //下出AI想的第一子
                        Defense[x][y]=GetMyBestMove2(GOMOKU,-color,2);//對方找出自己最強點位與分數
                        Attack[x][y]-=Defense[x][y];
                        GOMOKU[x][y]=0;
                }
                if(Attack[x][y]>FinalValue)      {Max_x=x; Max_y=y;  FinalValue=Attack[x][y];}
        }
        GiveNewStone(Max_x,Max_y);
        Label_Value->Caption=AnsiString(FinalValue);
}
//---------------------------------------------------------------------------

void __fastcall TForm_Main::Menu_PlayDefenseClick(TObject *Sender)
{
        unsigned char temp[250],x,y,x2,y2,TestGiven;
        int Attack[15][15],Defense[15][15],Max_x,Max_y,index;
        int color=Index_Move%2?-1:1,FinalValue,tNodes=0;


        suiForm1->Refresh();
        suiStatusBar1->Panels->Items[0]->Text="";

        srand(time(NULL));

        if(Index_Move==0)
        {
                GiveNewStone(7,7);
                return;
        }
        else if(Index_Move==1)
        {
                if(GOMOKU[7][7]==0)    GiveNewStone(7,7);
                else if(rand()%2)                GiveNewStone(7,8);
                else GiveNewStone(8,8);
                return;
        }

        Label_Depth->Caption = "1";     suiPanel1->Refresh();
        //5connect=====================================================
        for(x=0;x<15;x++)       for(y=0;y<15;y++)
        {
                Attack[x][y]=-1024;
                if(GOMOKU[x][y]!=0)     continue;
                Attack[x][y] = GetMyScore(GOMOKU, x, y, color);
                if(Attack[x][y]>=1024){GiveNewStone(x,y);  return;}//連五當然
        }
        for(x=0;x<15;x++)       for(y=0;y<15;y++)
        {
                Defense[x][y]=-1024;
                if(GOMOKU[x][y]!=0)     continue;
                Defense[x][y] = GetMyScore(GOMOKU, x, y, -color);
                if(Defense[x][y]>=1024){GiveNewStone(x,y);  return;}//對方連五當然擋
        }
        //VCF mode====================================================
        TestGiven = (GetBestVCFStone(Index_Move,Renju,1000));

        if(TestGiven<=224)
        {
                GiveNewStone(TestGiven);
                return;
        }
        //我方在第三手有沒有VCF?===================================
        Label_Depth->Caption = "3";     suiPanel1->Refresh();

        for(x=0;x<15;x++)       for(y=0;y<15;y++)
        {
                tNodes+=MyTime;       Label_Nodes->Caption = AnsiString(tNodes);     suiPanel1->Refresh();
                if(MyStyle[x][y]>3 || MyStyle[x][y]==0)     continue;
                Renju[Index_Move+1] = x + 225 - (y+1)*15;
                TestGiven = (GetBestVCFStone(Index_Move+2,Renju,100));

 	        if(TestGiven<=224)  //found VCF,
 	        {
                        Attack[x][y]+=512;
                        suiMemo_Commet->Lines->Append(AnsiString((char)(x+65))+AnsiString(y+1));
 	        }
                Renju[Index_Move+2] = 0;
        }
        for(x=0;x<15;x++)       for(y=0;y<15;y++)
        {
                tNodes+=100;       Label_Nodes->Caption = AnsiString(tNodes);     suiPanel1->Refresh();
                if(Attack[x][y]<10)     continue;
                Index_Move++;    Renju[Index_Move]=x + 225 - (y+1)*15;
                TestGiven = (GetBestVCFStone(Index_Move+2,Renju,100));
                if(TestGiven<=224)
                {
                        Attack[x][y]-=1024;
                }
                Renju[Index_Move]=0;   Index_Move--;
        }
        //==========================================================
        //有? 那就問對方全力反擊是否能阻止我vcf===================
        Label_Depth->Caption = "5";     suiPanel1->Refresh();

        FinalValue = -2048;
        for(x=0;x<15;x++)       for(y=0;y<15;y++)
        {
                tNodes+=MyTime;       Label_Nodes->Caption = AnsiString(tNodes);     suiPanel1->Refresh();
                if(Attack[x][y]<10 && Defense[x][y]<10)     continue;
                else if(Attack[x][y]>=512)
                {
                        Index_Move++;    Renju[Index_Move]=x + 225 - (y+1)*15;
                        for(x2=0;x2<15;x2++)       for(y2=0;y2<15;y2++)
                        {
                                if(x2==x && y2==y)      continue;
                                if(GOMOKU[x2][y2]!=0)  continue;

                                Renju[Index_Move+1]= x2 + 225 - (y2+1)*15;
                                TestGiven = (GetBestVCFStone(Index_Move+2,Renju,100));
                                if(TestGiven<=224)
                                {
                                        Attack[x][y]-=512;
                                        Renju[Index_Move]=0;   Index_Move--;
                                        goto NextPoint;
                                }
                        }
                        Renju[Index_Move]=0;   Index_Move--;
                }
                else if(Attack[x][y]<512)
                {
                        GOMOKU[x][y]=color;   //下出AI想的第一子
                        Defense[x][y]=GetMyBestMove2(GOMOKU,-color,2);//對方找出自己最強點位與分數
                        Attack[x][y]-=Defense[x][y];
                        GOMOKU[x][y]=0;
                }
                if(Attack[x][y]>512)    {GiveNewStone(x,y);  Label_Value->Caption=AnsiString(480);   return;}
                NextPoint:
                if(Attack[x][y]>FinalValue)      {Max_x=x; Max_y=y;  FinalValue=Attack[x][y];}
        }
        //==========================================================

        GiveNewStone(Max_x,Max_y);
        Label_Value->Caption=AnsiString(FinalValue);
}
//---------------------------------------------------------------------------

void __fastcall TForm_Main::Menu_MoveBackClick(TObject *Sender)
{
        Index_Move--;        DrawBoard();
}
//---------------------------------------------------------------------------

void __fastcall TForm_Main::Menu_MoveForwardClick(TObject *Sender)
{
        Index_Move++;        DrawBoard();
}
//---------------------------------------------------------------------------

void __fastcall TForm_Main::Menu_MoveDownClick(TObject *Sender)
{
//
}
//---------------------------------------------------------------------------

void __fastcall TForm_Main::Menu_MoveUpClick(TObject *Sender)
{
//
}
//---------------------------------------------------------------------------

void __fastcall TForm_Main::Menu_MoveFirstMoveClick(TObject *Sender)
{
        Index_Move=0;    DrawBoard();
}
//---------------------------------------------------------------------------

void __fastcall TForm_Main::Menu_MoveLastMoveClick(TObject *Sender)
{
        Index_Move=Renju[0];    DrawBoard();
}
//---------------------------------------------------------------------------
void __fastcall TForm_Main::Menu_HelpProduceClick(TObject *Sender)
{             /*
        if(Renju[0]<5) return;

 	unsigned char temp[250];
        int vc3=30000,index=51,Ran=0;
        BestMove *BestMoveThread;

        Image1->Visible = false;
        this->Refresh();

        srand(time(NULL));

        for(index=0;index<100;index++)
        {
                vc3 = 0;
                if(index%(Renju[0]-1)==0)       continue; //where we support renju[0]-1 is prime.

                memset(temp,0,sizeof(temp));
                temp[1]=1;
                temp[6]=2;//HashTableSize;0--8M, 1--16M,2--32M,
                temp[8]=Renju[0];
                temp[9]=Renju[1];

                for(Ran=1;Ran<Renju[0];Ran++)
                        temp[Ran+9]=Renju[(index*Ran)%(Renju[0]-1)+2];
                BestMoveThread = new BestMove(true);
                BestMoveThread->StartVCTThread(temp);

                for(int Index=0;Index<50 && vc3<20000;Index++)
                {
                        Sleep(1);
                        vc3 = BestMoveThread->VCTStatus;
                }
                StopThinking();

                if(vc3>20008)   break;
        }
        if(vc3>20008)
        {
                for(Ran=2;Ran<=Renju[0];Ran++)     temp[Ran-2]=Renju[(index*Ran)%(Renju[0]-1)+2];
                memcpy(&Renju[2],temp,Renju[0]-1);
                Index_Move=Renju[0];    DrawBoard();
                BBSFormProduce();
        }
        else ShowMessage("Not Exist VCF process");
        Image1->Visible = true; 
        delete BestMoveThread;  */
}
//---------------------------------------------------------------------------



void __fastcall TForm_Main::Image1MouseMove(TObject *Sender,
      TShiftState Shift, int X, int Y)
{
        if(X>16 && X<496 && Y>16 && Y<496)
        {
                suiStatusBar1->Panels->Items[1]->Text = AnsiString((char)((X-16)/32+65)) + AnsiString((512-Y+16)/32);
                //suiStatusBar1->Panels->Items[0]->Text=AnsiString((int)TStyle[(X-16)/32][(512-Y-16)/32]);
                switch(MyStyle[(X-16)/32][(512-Y-16)/32])
                {
                        case 14:
                                suiStatusBar1->Panels->Items[2]->Text = GOMOKU[(X-16)/32][(512-Y-16)/32]==1?"黑":"白";
                                break;
                        case 13:
                                suiStatusBar1->Panels->Items[2]->Text = "連五";
                                break;
                        case 12:
                                suiStatusBar1->Panels->Items[2]->Text = "長連";
                                break;
                        case 11:
                                suiStatusBar1->Panels->Items[2]->Text = "四四";
                                break;
                        case 10:
                                suiStatusBar1->Panels->Items[2]->Text = "三三";
                                break;
                        case 9:
                                suiStatusBar1->Panels->Items[2]->Text = "四三";
                                break;
                        case 8:
                                suiStatusBar1->Panels->Items[2]->Text = "活四";
                                break;
                        case 7:
                                suiStatusBar1->Panels->Items[2]->Text = "跳四";
                                break;
                        case 6:
                                suiStatusBar1->Panels->Items[2]->Text = "死四";
                                break;
                        case 5:
                                suiStatusBar1->Panels->Items[2]->Text = "活三";
                                break;
                        case 4:
                                suiStatusBar1->Panels->Items[2]->Text = "跳三";
                                break;
                        case 3:
                                suiStatusBar1->Panels->Items[2]->Text = "死三";
                                break;
                        case 2:
                                suiStatusBar1->Panels->Items[2]->Text = "活二";
                                break;
                        case 1:
                                suiStatusBar1->Panels->Items[2]->Text = "跳二";
                                break;
                        case 0:
                                suiStatusBar1->Panels->Items[2]->Text = "無";
                                break;
                        case -1:
                                suiStatusBar1->Panels->Items[2]->Text = "無B";
                                break;
                        case -2:
                                suiStatusBar1->Panels->Items[2]->Text = "無W";
                                break;
                        case -3:
                                suiStatusBar1->Panels->Items[2]->Text = "無BW";
                                break;

                }

        }
}
//---------------------------------------------------------------------------

void __fastcall TForm_Main::Image1MouseDown(TObject *Sender,
      TMouseButton Button, TShiftState Shift, int X, int Y)
{
        int Value,index;

        if(ToolButton_Update->Down && X>16 && X<496 && Y>16 && Y<496)
        {
                Value = 224 + ((X-16)/32+1) - ((512-Y+16)/32)*15;
                for(index=1;index<=Index_Move;index++)
                {
                        if(Renju[index]==Value) return;
                }
                Renju[Index_Move+1]=Value;
                Index_Move++;
                Renju[0]=Index_Move;
                BBSFormProduce();
                DrawBoard();
        }
        if(ToolButton_Update->Down && Button==mbLeft)      suiButton_AttackDefense->Click();
}
//---------------------------------------------------------------------------



void __fastcall TForm_Main::Timer1Timer(TObject *Sender)
{
        unsigned char temp[250];
        ProcessNum++;
        Label_Nodes->Caption = AnsiString(ProcessNum);

        if(BestMoveThread->MyValue!=0 || ProcessNum>=1000)
        {
                Timer1->Enabled=false;
                suiButton_Stop->Visible=false;  suiButton_Attack->Visible=true; suiButton_AttackDefense->Visible=true; suiButton_Defense->Visible=true;
                BestMoveThread->EndFind(temp);

                if(BestMoveThread->MyValue>=10000)
                {
                        suiStatusBar1->Panels->Items[0]->Text=BestMoveThread->MyValue>=20000?"VCF!!":"VCT";
                        GiveNewStone(temp[0]);
                }
        }
}
//---------------------------------------------------------------------------
void __fastcall TForm_Main::suiButton_StopClick(TObject *Sender)
{
        ProcessNum=999;
}
//---------------------------------------------------------------------------

void __fastcall TForm_Main::FormClose(TObject *Sender,
      TCloseAction &Action)
{
         BestMoveThread->Terminate();
}
//---------------------------------------------------------------------------




void __fastcall TForm_Main::InputTimeMenu_EditClick(TObject *Sender)
{
        int myTime=0;
        while(myTime==0)
        {
                AnsiString Num = InputBox("請輸入你要讓系統在一階時間跑多久(ms)","1~10000",AnsiString(MyTime));
                myTime = Num.ToIntDef(0);
                if(myTime<=0 || myTime>10000)   ShowMessage("Value must range 1~100000!");
        }
        MyTime = myTime;
}
//---------------------------------------------------------------------------

void __fastcall TForm_Main::ToolButton7Click(TObject *Sender)
{
        GiveNewStone(GetBestVCFStone(Index_Move,Renju,1000));        
}
//---------------------------------------------------------------------------

