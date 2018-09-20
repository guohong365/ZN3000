#include <bios.h>
#include <math.h>
#include <sys\types.h>
#include <sys\stat.h>
#include <fcntl.h>
#include <io.h>
#include <string.h>
#include <ctype.h>
#include <stdio.h>
#include <conio.h>
#include <graph.h>
#include <time.h>
#include "mouse.hpp"
#include "general.hpp"
#include "popup.hpp"
#include "const.hpp"
#include "hospital.hpp"
#include "editbar.hpp"
#include "printer.hpp"
extern unsigned long LineTable[480];

const double Pi=3.14159265;
extern Person person;
extern short *DataBuffer;
extern short *HeartBuffer;
extern unsigned long DataPointer;
extern unsigned short Scale;
extern unsigned short BodyPart;
extern char* HzkBuffer;

extern struct _fontinfo FontInfo;

extern MOUSE Mouse;
bool LoadData(char *,short n);
void CalculateA();
void Calculate();
void Std2();
void InitData()
{
    char name[20];
/*    
    sprintf(name,"%s.znd",person.No);
    if(LoadData(name,BodyPart))
      if(Mesure()==evNextStep)
      {
        CalculateA();
        Calculate();
      }
*/      

    sprintf(name,"%d.sav",BodyPart);
    FILE *fp=fopen(name,"rb");
    if(!fp)
    {
        Warning("cannot found saved file.");
        return;
    }

    fread(DataBuffer,5000,sizeof(short),fp);
    if(BodyPart==HEART)
    {
        fread(HeartBuffer,5000,sizeof(short),fp);
    }
    DataPointer=5000;
    fclose(fp);
    
    Warning("Saved data loaded.");
    
}

    

    

extern unsigned short BodyPart;
void InitScreen(void)
{
    register i;
    for (i=0;i<480;i++)
    {
        LineTable[i]=i*80;
    }

    _setvideomode(_VRES16COLOR);
    int d=_registerfonts("font.fon");
    _setfont("n0");
    _getfontinfo(&FontInfo);
    _setcolor(LIGHTGRAY);
    _rectangle(_GFILLINTERIOR,0,0,639,479);
    _setcolor(WHITE);
    _moveto(639,0);
    _lineto(0,0);
    _lineto(0,479);
    _moveto(3,455);
    _lineto(636,455);
    _lineto(636,3);
    _moveto(636,457);
    _lineto(636,477);
    _lineto(591,477);
    _moveto(588,457);
    _lineto(588,477);
    _lineto(3,477);

    _setcolor(DARKGRAY);
    _moveto(639,0);
    _lineto(639,479);
    _lineto(0,479);
    _moveto(3,477);
    _lineto(3,457);
    _lineto(588,457);
    _moveto(591,477);
    _lineto(591,457);
    _lineto(636,457);
    _moveto(3,455);
    _lineto(3,3);
    _lineto(636,3);
    _setcolor(BLUE);
    _rectangle(_GFILLINTERIOR,4,4,635,454);
};


void StdWav(void)
{
    if(Warning("进行定标吗?")==evCancel) return;
    MESSAGE *m=new MESSAGE("请打开定标开关，然后按任意键...");
    getch();
    delete m;
    Std2();
}        
