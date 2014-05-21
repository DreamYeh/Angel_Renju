//---------------------------------------------------------------------------

#ifndef Unit_GomokuH
#define Unit_GomokuH
//---------------------------------------------------------------------------
#define BRDSIZ 15
#define BBLANK 0
#define BBLACK 1
#define BWHITE -1

#define XYtoPOS (x+211-y*15)

typedef char board_t[BRDSIZ][BRDSIZ];

extern int getstyle(board_t ku, int x, int y, int color, int limit);
extern int GetMyScore(board_t ku, int x, int y,int MyColor);

extern int GetMyBestMove(unsigned char *max_x,unsigned char *max_y,board_t GOMOKU,int color,int dep);
extern int GetMyBestMove2(board_t GOMOKU,int color,int dep);

#endif
