#include <bios.h>
#include <math.h>
#include <sys\types.h>
#include <sys\stat.h>
#include <fcntl.h>
#include <io.h>
#include <stdlib.h>
#include <stdio.h>
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
extern bool Part[];
extern char *Csav[];
extern double Z0;
const double Pi=3.14159265;
bool Paramed=False;
short *DataBuffer=NULL;
short *HeartBuffer=NULL;
unsigned long DataPointer=0;
unsigned short Scale=200;
float fScale=10;
double Interval=0;
double Interval1=0;
double Interval2=0;
extern char* HzkBuffer;
extern float MDBuffer[];
extern float MHBuffer[];
unsigned short BodyPart=20;
short WaveZero1=160;
short Base=0x280;
short Port=28;
Person person;
enum {NOTHING,GETDATA,GETPARAMETER} Operation;
struct _fontinfo FontInfo;



MOUSE Mouse;
POINT CurPos;

short Update=1;
bool Pause=True;
bool Begin=False;
bool DataIn=False;
void InitScreen();
void InitData();
void Pick1();
void Pick2();
void Standard(void);
void StdWav();
void Calculate(void);
void CalculateB();
void Report(void);
void clearArea(void);
void title();
unsigned short GetL();
void MAIN(void)
{
    register i;
    unsigned short off;
    bool mouseon=True;

    DataBuffer=new short[TOTAL_DATA];
    HeartBuffer=new short[TOTAL_DATA];

    InitScreen();
    if((Handle=open("std.ini",O_RDONLY))==-1)
    {
        Warning("首次运行，必须定标!");
        Standard();
        Handle=open("std.ini",O_RDONLY);
    }
    read(Handle,&Interval1,sizeof(double));
    read(Handle,&Interval2,sizeof(double));
    read(Handle,&Z0,sizeof(double));
    close(Handle);
    StdWav();
    //build button menu
    StaticText *bodytext=new StaticText(-1,POINT(20,20),"测试部位:",0,0,YELLOW,BLUE);
    Button *bBodyPart=new Button(0,POINT(100,17),"",evBodyPart,True);  
    Button *bBegin=new Button(1,POINT(20,400),"开始检查",evBegin,False);
    Button *bPause=new Button(2,POINT(120,400),"暂停采样",evPause,False);
    Button *bGetData=new Button(3,POINT(220,400),"测量计算",evGetData,False);
    Button *bParam=new Button(4,POINT(320,400),"输入参数",evParameter,True);
    Button *bPrintSet=new Button(5,POINT(420,400),"查询结果",evPrintSet,True);
    Button *bQuit=new Button(6,POINT(520,400),"退出程序",evQuit,True);
    bodytext->Draw();
    bBodyPart->Draw();
    bBegin->Draw();
    bPause->Draw();
    bGetData->Draw();
    bParam->Draw();
    bPrintSet->Draw();
    bQuit->Draw();
    short Quit=0;
    int Y1=80;
    int Y2=180;
    char tt[50];
    _setcolor(GREEN);
    _rectangle(_GBORDER,WaveWinLeft,WaveWinTop,
                          WaveWinRight,WaveWinBottom);
    int j=0;
    float time=0;                      
    Mouse.On();
    while(!Quit)
    {
        switch(Item::Message)
        {
            case evParameter:
                 if(DataIn)
                 {
                     if(Warning("最后一次检查数据没有计算，放弃吗?")==evOk)
                     {
                         DataIn=False;
                     }
                 }
                 if(!DataIn)
                 {
                     if(GetPara(person)==evOk)
                     {
                         for(i=0;i<11;i++)
                         {
                             Part[i]=False;
                             delete Csav[i];
                             Csav[i]=NULL;
                         }
                         Begin=False;
                         Paramed=True;
                         BodyPart=0;
                         bBodyPart->SetData((void*)BodyString[::BodyPart]);
                         bBodyPart->Draw();
                         bBegin->Valid=True;                                 
                         bBegin->Update(False);
                     }
                 }
                 Item::Message=evNothing;
                 break;
            case evBodyPart:
                 if(DataIn)
                 {
                     if(Warning("最后一次检查数据没有计算，放弃吗?")==evOk)
                     {
                         DataIn=False;
                     }
                 }
                 if(!DataIn)
                 {
                     int ttmp=GetBodyPart();
                     if(ttmp>0)
                     {
                         BodyPart=ttmp-1;
                     }
                     else
                     {
                         Item::Message=evNothing;
                         break;
                     }
                     if(BodyPart==HEART)
                     {
                         WaveZero1=160;
                         Port=28;
                     }
                     else
                     {
                         Port=26;
                         WaveZero1=400;
                     }
                     bBodyPart->SetData((void*)BodyString[::BodyPart]);
                     bBodyPart->Draw();
                     bBegin->Valid=True;                                 
                     bBegin->Update(False);
                     //InitData();
                 }
                 Item::Message=evNothing;
                 break;
            case evBegin:
                 if(DataIn)
                 {
                     if(Warning("最后一次检查数据没有计算，放弃吗?")==evOk)
                     {
                         DataIn=False;
                     }
                 }
                 if(!DataIn)
                 {
                     if(!Paramed)
                     {
                         if(GetPara(person)==evCancel)
                            break;
                         Paramed=True;
                     }
                     if(Part[BodyPart]==True||BodyPart>10)
                     {
                         Warning("请选择检测部位。");
                         Item::Message=evBodyPart;
                     }
                     else 
                     {
                         if(BodyPart==HEART)
                         {
                             Interval=Interval1;
                             Scale=200;
                             WaveZero1=160;
                         }
                         else
                         {
                             Interval=Interval2;
                             fScale=200;
                             WaveZero1=400;
                         }
                         _setcolor(BLUE);
                         _rectangle(_GFILLINTERIOR,WaveWinLeft-10,WaveWinBottom+1,
                                    WaveWinRight+20,WaveWinBottom+25);
                         j=0;
                         time=0;
                         _setcolor(GREEN);
                         for(i=0;i<WaveRight-WaveLeft+1;i++)
                         {
                             if(time>=j)
                             {
                                 if(j%100==0)
                                 {
                                     _moveto(WaveLeft+i,WaveWinBottom);
                                     _lineto(WaveLeft+i,WaveWinBottom+5);
                                 }
                                 else
                                 {
                                     _moveto(WaveLeft+i,WaveWinBottom);
                                     _lineto(WaveLeft+i,WaveWinBottom+2);
                                 }
                                 j+=50;
                             }
                             time+=Interval*1000;
                         }
//                         OutText16(WaveRight,WaveBottom,0,"ms");
                         clearArea();
                         DataPointer=0;
                         Pause=False;
                         Begin=True;
                         bBodyPart->Valid=False;
                         bBegin->Valid=False;
                         bPause->Valid=True;
                         bPrintSet->Valid=False;
                         bParam->Valid=False;
                         bGetData->Valid=False;
                         bQuit->Valid=False;
                         bBegin->Update(False);
                         bPause->Update(False);
                         bBodyPart->Update(False);
                         bPrintSet->Update(False);
                         bParam->Update(False);
                         bGetData->Update(False);
                         bQuit->Update(False);
                         Mouse.Off();
                         mouseon=False;
                         Item::Message=evNothing;
                     }
                 }
                 else Item::Message=evNothing;
                 break;
            case evPause:
                 Pause=True;
                 DataIn=True;
                 bPause->Valid=False;
                 bPause->Update(False);                                 
                 bGetData->Valid=True;
                 bGetData->Update(False);                                 
                 bQuit->Valid=True;
                 bQuit->Update(False);
                 bBodyPart->Valid=True;
                 bBodyPart->Update(False);                                                                  
                 Mouse.On();
                 mouseon=True;
                 Item::Message=evNothing;
                 break;
            case evPrintSet:
                 Report();
                 bGetData->Valid=False;
                 bGetData->Update(False);                                 
                 Item::Message=evNothing;                                  
                 break;
            case evGetData:
                 if(BodyPart==HEART)
                    Interval=Interval1;
                 else Interval=Interval2;  
                 if(GetData()==evMesure)      //get sample
                 {
                     if(Mesure()==evNextStep)
                     {
                         Part[BodyPart]=True;                                        
                         Calculate();
                         DataIn=False;
                         bPrintSet->Valid=True;                                 
                         bPrintSet->Update(False);
                     }
                 }
                 bParam->Valid=True;                                 
                 bParam->Update(False);
                 bBodyPart->Valid=True;                                 
                 bBodyPart->Update(False);
                 Begin=False;
                 Item::Message=evNothing;
                 break;
//================                        
            case evNothing:
                 if(!Pause)
                 {
                     if(CurPos.X<WaveWinLeft|| CurPos.X>WaveWinRight ||
                        CurPos.Y<WaveWinTop||CurPos.Y>WaveWinBottom)
                     {
                         if(!mouseon)
                         {
                             Mouse.On();
                             mouseon=True;
                         }
                     }
                     else if(mouseon)
                     {
                         Mouse.Off();
                         mouseon=False;
                     }
//---------------------------
                     if(BodyPart==HEART)
                     {
                         Pick1();
                         Pick2();                                     
                                  
                         if(DataPointer<WinWidth)
                         {
                             Y1=(DataBuffer[0]-2047)*Scale/2047;
                             Y2=(HeartBuffer[0]-2047)*Scale/3/2047;
                             if(WaveZero1-Y1>WaveTop&&WaveZero1-Y1<WaveBottom)
                                SetPixel(WaveRight-DataPointer,WaveZero1-Y1,YELLOW);
                             if(WaveZero2-Y2<WaveBottom&&WaveZero2-Y2>WaveTop)
                                SetPixel(WaveRight-DataPointer,WaveZero2-Y2,YELLOW);                                        
                             for(i=0;i<DataPointer;i++)
                             {
                                 off=i%TOTAL_DATA;
                                 Y1=(DataBuffer[off]-2047)*Scale/2047;
                                 Y2=(HeartBuffer[off]-2047)*Scale/3/2047;                                          
                                 if(WaveZero1-Y1>WaveTop&&WaveZero1-Y1<WaveBottom)
                                    SetPixel(WaveRight-DataPointer+i+1,WaveZero1-Y1,BLUE);                                     
                                 if(WaveZero2-Y2<WaveBottom&&WaveZero2-Y2>WaveTop)
                                    SetPixel(WaveRight-DataPointer+i+1,-Y2+WaveZero2,BLUE);                                                                                                         
                                 off=(i+1)%TOTAL_DATA;
                                 Y1=(DataBuffer[off]-2047)*Scale/2047;
                                 Y2=(HeartBuffer[off]-2047)*Scale/3/2047;
                                 if(WaveZero1-Y1>WaveTop&&WaveZero1-Y1<WaveBottom)                                                                           
                                    SetPixel(WaveRight-DataPointer+i+1,WaveZero1-Y1,YELLOW);
                                 if(WaveZero2-Y2<WaveBottom&&WaveZero2-Y2>WaveTop)
                                    SetPixel(WaveRight-DataPointer+i+1,-Y2+WaveZero2,YELLOW);                                                                               
                             }
                             DataPointer++;
                         }
                         else
                         {
                             for(i=0;i<WinWidth;i++)
                             {
                                 off=(DataPointer-WinWidth+i)%TOTAL_DATA;
                                 Y1=(DataBuffer[off]-2047)*Scale/2047;
                                 Y2=(HeartBuffer[off]-2047)*Scale/3/2047;
                                 if(WaveZero1-Y1>WaveTop&&WaveZero1-Y1<WaveBottom)
                                    SetPixel(WaveRight-WinWidth+i+1,WaveZero1-Y1,BLUE);
                                 if(WaveZero2-Y2<WaveBottom&&WaveZero2-Y2>WaveTop)
                                    SetPixel(WaveRight-WinWidth+i+1,WaveZero2-Y2,BLUE);                                                          
                                 off=(DataPointer-WinWidth+i+1)%TOTAL_DATA;
                                 Y1=(DataBuffer[off]-2047)*Scale/2047;
                                 Y2=(HeartBuffer[off]-2047)*Scale/3/2047;
                                 if(WaveZero1-Y1>WaveTop&&WaveZero1-Y1<WaveBottom)                                                                  
                                    SetPixel(WaveRight-WinWidth+i+1,-Y1+WaveZero1,YELLOW);
                                 if(WaveZero2-Y2<WaveBottom&&WaveZero2-Y2>WaveTop)
                                    SetPixel(WaveRight-WinWidth+i+1,WaveZero2-Y2,YELLOW);
                             }
                             DataPointer++;                             
                         }
                                      
                     }
//-----------------------------------------------------
                     else
                     {
                         Pick1();
                                  
                         if(DataPointer<WinWidth)
                         {
                             Y1=4095*fScale/DataBuffer[0];
                             if(WaveZero1-Y1>WaveTop&&WaveZero1-Y1<WaveBottom)
                                SetPixel(WaveRight-DataPointer,WaveZero1-Y1,YELLOW);
                             for(i=0;i<DataPointer;i++)
                             {
                                 off=i%TOTAL_DATA;
                                 Y1=4095*fScale/DataBuffer[off];
                                 if(WaveZero1-Y1>WaveTop&&WaveZero1-Y1<WaveBottom)
                                    SetPixel(WaveRight-DataPointer+i+1,WaveZero1-Y1,BLUE);
                                 off=(i+1)%TOTAL_DATA;
                                 Y1=4095*fScale/DataBuffer[off];
                                 if(WaveZero1-Y1>WaveTop&&WaveZero1-Y1<WaveBottom)
                                    SetPixel(WaveRight-DataPointer+i+1,WaveZero1-Y1,YELLOW);
                             }
                             DataPointer++;
                         }
                         else
                         {
                             for(i=0;i<WinWidth;i++)
                             {
                                 off=(DataPointer-WinWidth+i)%TOTAL_DATA;
                                 Y1=4095*fScale/DataBuffer[off];
                                 if(WaveZero1-Y1>WaveTop&&WaveZero1-Y1<WaveBottom)
                                    SetPixel(WaveRight-WinWidth+i+1,WaveZero1-Y1,BLUE);
                                 off=(DataPointer-WinWidth+i+1)%TOTAL_DATA;
                                 Y1=4095*fScale/DataBuffer[off];
                                 if(WaveZero1-Y1>WaveTop&&WaveZero1-Y1<WaveBottom)                                                                  
                                    SetPixel(WaveRight-WinWidth+i+1,-Y1+WaveZero1,YELLOW);
                             }
                             DataPointer++;                             
                         }
                                      
                     }

//------------------------------
                 }
                 break;
            case evQuit:
                 if(DataIn)
                 {
                     if(Warning("最后一次检查数据没有计算，放弃吗?")==evOk)
                     {
                         DataIn=False;
                     }
                 }
                 if(!DataIn)
                 {
                     Quit=1;
                     _setvideomode(_DEFAULTMODE);
                 }
                 Item::Message=evNothing;
                 break;
            case evMouseLDown:
                 Mouse.GetPosition(CurPos);
                 if(bBodyPart->IsInObject(CurPos))
                 {
                     if(bBodyPart->Valid)
                     {
                         bBodyPart->Track();
                         Item::Message=evBodyPart;
                     }
                     else Item::Message=evNothing;
                 }
                 else if(bBegin->IsInObject(CurPos))
                 {
                     if(bBegin->Valid)
                     {
                         bBegin->Track();
                         Item::Message=evBegin;
                     }
                     else Item::Message=evNothing;
                 }
                 else if(bPause->IsInObject(CurPos))
                 {
                     if(bPause->Valid)
                     {
                         bPause->Track();
                         Item::Message=evPause;
                     }
                     else Item::Message=evNothing;
                 }
                 else if(bGetData->IsInObject(CurPos))
                 {
                     if(bGetData->Valid)
                     {
                         bGetData->Track();
                         Item::Message=evGetData;
                     }
                     else Item::Message=evNothing;
                 }
                 else if(bParam->IsInObject(CurPos))
                 {
                     if(bParam->Valid)
                     {
                         bParam->Track();
                         Item::Message=evParameter;
                     }
                     else Item::Message=evNothing;
                 }
                 else if(bPrintSet->IsInObject(CurPos))
                 {
                     if(bPrintSet->Valid)
                     {
                         bPrintSet->Track();
                         Item::Message=evPrintSet;
                     }
                     else Item::Message=evNothing;
                 }
                 else if(bQuit->IsInObject(CurPos))
                 {
                     if(bQuit->Valid)
                     {
                         bQuit->Track();
                         Item::Message=evQuit;
                     }
                     else Item::Message=evNothing;
                 }
                 else Item::Message=evNothing;
                 break;
            case ENTER:
                 if(!Pause)
                    Item::Message=evPause;
                 else
                 {
                     if(BodyPart<11)
                     {
                         if(Begin)Item::Message=evGetData;
                         else Item::Message=evBegin;
                     }
                     else Item::Message=evBodyPart;
                 }
                 break;
          default:
                 Item::Message=evNothing;
                 break;
        }
        if(Item::Message==evNothing)
        {
            if(Mouse.ButtonStatus(0)) Item::Message=evMouseLDown;
            else if(Mouse.ButtonStatus(1)) Item::Message=evMouseRDown;
            else
            {
                Mouse.Where(CurPos);
                Item::Message=evNothing;
            }
            if(_bios_keybrd(_KEYBRD_READY))
            {
                unsigned short key=_bios_keybrd(_KEYBRD_READ);
                if(key==ENTER) Item::Message=ENTER;
                if(BodyPart!=HEART)
                {
                    if(key==UP)
                    {
                    WaveZero1-=10;
                    clearArea();
                    DataPointer=0;
                    Item::Message=evNothing;
                    }
                    if(key==DOWN)
                    {
                    WaveZero1+=10;
                    clearArea();
                    DataPointer=0;
                    Item::Message=evNothing;
                    }
                    if(key==RIGHT)
                    {
                    if(fScale<500)fScale+=0.10*fScale;
                    clearArea();
                    DataPointer=0;
                    Item::Message=evNothing;
                    }
                    if(key==LEFT)
                    {
                    if(Scale>50)fScale-=0.10*fScale;
                    clearArea();
                    DataPointer=0;
                    Item::Message=evNothing;
                    }
                }
            }
        }                
    }
    delete DataBuffer;
    delete HeartBuffer;
    delete bodytext;
    delete bBodyPart;  
    delete bBegin;
    delete bPause;
    delete bGetData;
    delete bParam;
    delete bPrintSet;
    delete bQuit;
}

void clearArea(void)
{
    _setcolor(BLUE);
    _rectangle(_GFILLINTERIOR,WaveWinLeft+1,WaveWinTop+1,
               WaveWinRight-1,WaveWinBottom-1);
    _setcolor(GREEN);           
    for(int i=WaveWinTop;i<WaveWinBottom;i+=5)
    {
        _moveto(WaveWinLeft,i);
        _lineto(WaveWinLeft+5,i);
    }           
};           
