#include <stdlib.h>
#include <ctype.h>
#include <stdio.h>
#include <string.h>
#include "general.hpp"
#include "mouse.hpp"
extern double iInterval;
extern unsigned short BodyPart;
extern float MDBuffer[];
extern float MHBuffer[];
extern short Mark[];
extern MOUSE Mouse;
void PrintWave(void)
{
    char buf[20];
    FILE*prt=fopen("prt.swp","wb");
    if(!prt)
    {
        Warning("can not create swap file,print failed.");
        return;
    }

    fwrite(&BodyPart,1,2,prt);
    fwrite(&iInterval,1,8,prt);
    fwrite(Mark,10,sizeof(short),prt);
    fwrite(MDBuffer,570,sizeof(float),prt);
    fwrite(MHBuffer,570,sizeof(float),prt);
    fclose(prt);
    Mouse.Off();
    char *scr=new char[_imagesize(0,0,639,470)];
    _getimage(0,0,639,470,scr);
    MESSAGE *m=new MESSAGE("正在打印，请稍等....");
    strcpy(buf,"prtwav>nul");
    system(buf);
    delete m;
    _putimage(0,0,scr,_GPSET);
    //remove("prt.swp");
};
    
