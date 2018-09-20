#include "stdafx.h"
#include "hospital.hpp"
#include "popup.hpp"
#include "const.hpp"

extern MOUSE Mouse;
extern unsigned short BodyPart;
bool Part[11];

short GetBodyPart(void)
{
    
    short ret;
    if(BodyPart<11)
       ret=BodyPart+1;
    else ret=1;
    bool over=False;
    char *ScrBuf=new char[_imagesize(130,20,300,310)];
    StaticText *st[11];
    Object*BodyHead=new Object(
                     *(new SingleSlct(0,POINT(135,35),"请选择要测量的部位:"))-(
                     *(new SingleSlctItem(1,POINT(150,55),(char*)BodyString[0]))-
                     *(new SingleSlctItem(2,POINT(150,75),(char*)BodyString[1]))-
                     *(new SingleSlctItem(3,POINT(150,95),(char*)BodyString[2]))-
                     *(new SingleSlctItem(4,POINT(150,115),(char*)BodyString[3]))-
                     *(new SingleSlctItem(5,POINT(150,135),(char*)BodyString[4]))-
                     *(new SingleSlctItem(6,POINT(150,155),(char*)BodyString[5]))-
                     *(new SingleSlctItem(7,POINT(150,175),(char*)BodyString[6]))-
                     *(new SingleSlctItem(8,POINT(150,195),(char*)BodyString[7]))-
                     *(new SingleSlctItem(9,POINT(150,215),(char*)BodyString[8]))-
                     *(new SingleSlctItem(10,POINT(150,235),(char*)BodyString[9]))-
                     *(new SingleSlctItem(11,POINT(150,255),(char*)BodyString[10])) ));
    GetObject(BodyHead,0)->SetData(&ret);
    Mouse.Off();
    _getimage(130,20,300,310,ScrBuf);
    _setcolor(LIGHTGRAY);
     for(int i=0;i<145;i++)
    {
        _rectangle(_GBORDER,i>85?130:215-i,165-i,i>85?300:215+i,165+i);
    }
    Border3D(130,20,300,310);

    BodyHead->Draw();
    for(i=0;i<11;i++)
    {
        if(Part[i]==True)
        {
            st[i]=new StaticText(0,POINT(230,55+20*i),"已检测");
            st[i]->Draw();
        }
    }
    Item::Message=evNothing;
    short  CurX=0;
    Mouse.On();
    while(!over)
    {
        ret=BodyHead->Next->Do();
        if(ret==-1)
        {
            over=True;
        }
        else
        {
            if(Part[ret-1]==True)
            {
                if(Warning("此部位已检测,重新检测吗?")==evOk)
                {
                    Part[ret-1]=False;
                    over=True;
                }
            }
            else over=True;
        }
    }
    Item::Message=evNothing;
    Mouse.Off();
    for(i=0;i<11;i++)
    {
        if(Part[i]==True)
          delete st[i];
    }
    
    _putimage(130,20,ScrBuf,_GPSET);
    Mouse.On();
    delete BodyHead;
    delete ScrBuf;
    return ret;
}                                       
