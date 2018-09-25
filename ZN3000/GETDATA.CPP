#include "stdafx.h"

#include <stdio.h>
#include <conio.h>
#include <stdlib.h>
#include <math.h>
#include "hospital.hpp"
#include "const.hpp"
short  FirstX=-1;
short  SecondX=-1;
short  CurrentMark=-1;
extern short WaveZero1;
extern short *DataBuffer;
extern short *HeartBuffer;
extern unsigned short Scale;
extern float fScale;
extern unsigned long DataPointer;
extern Person person;
extern double Interval;
unsigned short DataCount;
unsigned short DataRight;
double iInterval;
short *ShowBuffer1=NULL;
short *ShowBuffer2=NULL;
void Smooth(float*,unsigned short);
short Ap,Bp,Op,Xp,Yp,Zp,Rp1,Rp2,Cp,Qp,Bp2;
extern double Cs,Xs,Al,Sc;
extern double HD,HS,HA,L,Oy,ST,DT,H,H1,H2,H3,Hb; 
extern double Ay,By,Cy,Zy,Z0,HBZTZ,LBZTZ,BZTZ,TA,Alpha,Beta,Y0;
extern double HeartB[];
extern double C[];
short BeatRate;
extern bool Part[];
extern short Mark[];
extern unsigned short BodyPart;
const short left=35;
const short right=605; 
extern short MarkNum;
float MHBuffer[571];
float MDBuffer[571];

void DtoV(float *,unsigned short);
void VtoO(float *,unsigned short);
void PrintWave(void);
char MarkName[][10][3]={
{ "Q","R","B","Z","C","X","Y","O","R",""},
{ "a","Q","Z","D","Q","", "", "", "", ""},
{ "a","Q","Z","C","D","Q","", "", "", ""},
{ "Q","Z","C","D","Q","", "", "", "", ""},
{ "Q","B","Z","Q","", "", "", "", "", ""},
{ "Q","B","Z","Q","", "", "", "", "", ""},
{ "Q","B","Z","Q","", "", "", "", "", ""},
{ "Q","B","Z","Q","", "", "", "", "", ""},
{ "Q","Z","Q","","", "", "", "", "", ""},
{ "Q","Z","Q","","", "", "", "", "", ""},
{ "Q","Z","Q","","", "", "", "", "", ""}
};
char MarkMention[11][80]={
"Q(1),R(2),B(3),Z(4),C(5),X(6),Y(7),O(8),R(9)",
"a(1),Q(2),Z(3),D(4),Q(5)",
"a(1),Q(2),Z(3),C(4),D(5),Q(6)",
"Q(1),Z(2),C(3),D(4),Q(5)",
"Q(1),B(2),Z(3),Q(4)",
"Q(1),B(2),Z(3),Q(4)",
"Q(1),B(2),Z(3),Q(4)",
"Q(1),B(2),Z(3),Q(4)",
"Q(1),Z(2),Q(3)",
"Q(1),Z(2),Q(3)",
"Q(1),Z(2),Q(3)"
};

    
    

void InsValue(short *source,unsigned short sCount,float *target,unsigned short tCount);
void AutoMark(float *,float *,short,short);                            
void moveline(short x,short y1,short y2,char *name=NULL);                            
short bbase;
void DrawBase(void)
{
    short i;
    short y;
    short tmp;
    Mouse.Off();
    if(FirstX>SecondX)
    {
        tmp=SecondX;
        SecondX=FirstX;
        FirstX=SecondX;
    }
    short max=0;
    for(i=FirstX;i<SecondX;i++)
    {
        if(ShowBuffer1[i]>max)
        {
            max=ShowBuffer1[i];
        }
    }
    y=2047*fScale/max-2047*fScale/bbase;
//    if(y>WaveBottom||y<WaveTop)return;        
    _setcolor(BLUE^YELLOW);
    _setplotaction(_GXOR);
    _moveto(WaveLeft,WaveZero1-y);
    _lineto(WaveRight,WaveZero1-y);
//    OutText16(x-6,y2+2,0,name);
    _setplotaction(_GPSET);
    Mouse.On();
}

void ShowData(bool clear)
{
    register i;
    short Y1,Y2;
    COLORS color;

    if(!clear) color=BLUE;
    else color=YELLOW;
    
       _setcolor(color);
        for(i=0;i<WinWidth;i++)
        {
            if(BodyPart==HEART)
            {
                Y1=(ShowBuffer1[DataRight-i-1]-2047)*Scale/2047;                   
                Y2=(ShowBuffer2[DataRight-i-1]-2047)*Scale/2047;
                if(WaveZero1-Y1>WaveTop&&WaveZero1-Y1<WaveBottom)
                   _setpixel(WaveRight-i-1,WaveZero1-Y1);
                if(WaveZero2-Y2>WaveTop&&WaveZero2-Y2<WaveBottom)
                   _setpixel(WaveRight-i-1,WaveZero2-Y2);
            }
            else
            {
                Y1=2047*fScale/ShowBuffer1[DataRight-i-1]-
                   2047*fScale/bbase;
                if(WaveZero1-Y1>WaveTop&&WaveZero1-Y1<WaveBottom)
                   _setpixel(WaveRight-i-1,WaveZero1-Y1);
            }
        }
};

void moveline(short x,short y1,short y2,char *name)
{
    if(x>WaveRight-1)return;
    _setcolor(BLUE^YELLOW);
    _setplotaction(_GXOR);
    if(name)
    {
        OutText16(x-6,y1+8,0,name);
    }
    _moveto(x,y1+26);
    _lineto(x,y2);
    _setplotaction(_GPSET);
}

unsigned short GetData(void)
{
    register i;
    POINT point;
    bool over=False;
    unsigned short ret;
    short PNumber=0;
    char ln[2][2]={{'1',0},{'2',0}};
    char *ScrBuf=new char[_imagesize(20,10,620,440)];
    StaticText *tscale=new StaticText(-1,POINT(60,25),"请选择测量区域:");
//    StaticText *thelp=new StaticText(-1,POINT(5,458),"左右箭头或移动鼠标移动标尺 ENTER键或鼠标左键确定"); 
    Button *dMesure=new Button(1,POINT(100,380),"测量记时",evMesure,False);
    Button *dContinue=new Button(2,POINT(400,380),"取  消",evCancel,True);
    if(BodyPart!=HEART)WaveZero1=300;
    DataCount=DataPointer<TOTAL_DATA?DataPointer:TOTAL_DATA;
    DataRight=DataCount;
    //copy data to buffer
    ShowBuffer1=new short[DataCount];
    if(BodyPart==HEART)
        ShowBuffer2=new short[DataCount];
    bbase=0;        
    for(i=0;i<DataCount;i++)
    {
           ShowBuffer1[DataCount-i-1]=DataBuffer[(DataPointer-i-1)%TOTAL_DATA];
           if(ShowBuffer1[DataCount-i-1]>bbase)
              bbase=ShowBuffer1[DataCount-i-1];
           if(BodyPart==HEART)
              ShowBuffer2[DataCount-i-1]=HeartBuffer[(DataPointer-i-1)%TOTAL_DATA];           
    }

    
    Mouse.Off();
    _getimage(20,10,620,440,ScrBuf);
    _setcolor(LIGHTGRAY);
    for(i=0;i<300;i++)
    {
        _rectangle(_GBORDER,320-i,i>215?10:225-i,320+i,i>215?440:225+i);
    }
    Border3D(20,10,620,440);
    _setcolor(BLUE);
    _rectangle(_GFILLINTERIOR,WaveWinLeft,WaveWinTop,WaveWinRight,WaveWinBottom);
    Border3D(WaveWinLeft,WaveWinTop,WaveWinRight,WaveWinBottom);

    ShowData(True);
    
    tscale->Draw();
//    thelp->Draw(); 
    dMesure->Draw();
    dContinue->Draw();
    moveline(0,WaveTop,WaveBottom,ln[PNumber]);
    Mouse.On();
    Mouse.SetLimitArea(20,10,620,440);
    Item::Message=evNothing;
    short  CurX=0;
    short dD=0;
    while(!over)
    {
        switch(Item::Message)
        {
            case PGUP:
                     if(PNumber==0)
                     {
                         if(CurX>WaveLeft-1)
                         {
                             Mouse.Where(point);                             
                             Mouse.Off();
                             moveline(CurX,WaveTop,WaveBottom,ln[PNumber]);
                             if(CurX-500<WaveLeft)
                             {
                                 ShowData(False);
                                 CurX=WaveLeft-1;
                                 DataRight=DataRight-WinWidth-500>0?DataRight-500:WinWidth;
                                 ShowData(True);
                                 delay(50);
                             }
                             else
                             {
                                 CurX-=500;
                             }
                             point.X=CurX;
                             Mouse.SetPosition(point.X,point.Y);
                             moveline(CurX,WaveTop,WaveBottom,ln[PNumber]);
                             Mouse.On();
                         }
                         else if(DataRight>WinWidth)
                         {
                             ShowData(False);
                             CurX=WaveLeft-1;
                             DataRight=DataRight-WinWidth-500>0?DataRight-500:WinWidth;
                             ShowData(True);
                             delay(50);
                         }
                     }
                     Item::Message=evNothing;
                     break;
            case PGDOWN:
                 if(PNumber==0)
                 {
                     if(CurX<WaveRight)
                     {
                             Mouse.Where(point);                             
                             Mouse.Off();
                             moveline(CurX,WaveTop,WaveBottom,ln[PNumber]);
                             if(CurX+500>WaveRight)
                             {
                                 ShowData(False);
                                 DataRight=(DataCount-DataRight<500?DataCount:DataRight+500);
                                 CurX=WaveRight;
                                 ShowData(True);
                                 delay(50);
                             }
                             else
                             {
                                    CurX+=500;
                             }
                             point.X=CurX;
                             Mouse.SetPosition(point.X,point.Y);
                             moveline(CurX,WaveTop,WaveBottom,ln[PNumber]);
                             Mouse.On();
                     }
                     else if(DataRight<DataCount)
                     {
                         ShowData(False);
                         CurX=WaveRight;
                         DataRight=DataRight<DataCount-500?DataRight+500:DataCount;
                         ShowData(True);
                         delay(50);
                     }
                 }
                     Item::Message=evNothing;
                     break;
            case RIGHT:
                 if(PNumber==0)
                 {
                     if(CurX<WaveRight)
                     {
                             Mouse.Where(point);                             
                             Mouse.Off();
                             moveline(CurX,WaveTop,WaveBottom,ln[PNumber]);
                             if(CurX+4>WaveRight)
                             {
                                 ShowData(False);
                                 DataRight=(DataCount-DataRight<4?DataCount:DataRight+4);
                                 CurX=WaveRight;
                                 ShowData(True);
                                 delay(50);
                             }
                             else
                             {
                                    CurX+=4;
                             }
                             point.X=CurX;
                             Mouse.SetPosition(point.X,point.Y);
                             moveline(CurX,WaveTop,WaveBottom,ln[PNumber]);
                             Mouse.On();
                     }
                     else if(DataRight<DataCount)
                     {
                         ShowData(False);
                         CurX=WaveRight;
                         DataRight=DataRight<DataCount-4?DataRight+4:DataCount;
                         ShowData(True);
                         delay(50);
                     }
                 }
                     Item::Message=evNothing;
                     break;
            case LEFT:
                     if(PNumber==0)
                     {
                         if(CurX>WaveLeft-1)
                         {
                             Mouse.Where(point);                             
                             Mouse.Off();
                             moveline(CurX,WaveTop,WaveBottom,ln[PNumber]);
                             if(CurX-4<WaveLeft)
                             {
                                 ShowData(False);
                                 CurX=WaveLeft-1;
                                 DataRight=DataRight-WinWidth-4>0?DataRight-4:WinWidth;
                                 ShowData(True);
                                 delay(50);
                             }
                             else
                             {
                                 CurX-=4;
                             }
                             point.X=CurX;
                             Mouse.SetPosition(point.X,point.Y);
                             moveline(CurX,WaveTop,WaveBottom,ln[PNumber]);
                             Mouse.On();
                         }
                         else if(DataRight>WinWidth)
                         {
                             ShowData(False);
                             CurX=WaveLeft-1;
                             DataRight=DataRight-WinWidth-4>0?DataRight-4:WinWidth;
                             ShowData(True);
                             delay(50);
                         }
                     }
                     Item::Message=evNothing;
                     break;
            case ENTER:
                    if(PNumber==0)
                    {
                        FirstX=DataRight-WinWidth+CurX-WaveLeft;
                        PNumber++;
                        CurX=639;
                        moveline(CurX,WaveTop,WaveBottom,ln[PNumber]);
                    }
                    else  if(PNumber==1)
                    {
                        SecondX=DataRight-WinWidth+CurX-WaveLeft;
                        PNumber++;
                        if(BodyPart!=HEART)DrawBase();
                        dMesure->Valid=True;
                        dMesure->Update(False);
                    }
                    else if(PNumber>1)
                    {
                        dMesure->Track();
                        ret=evMesure;
                        over=True;
                    }
                    Item::Message=evNothing;
                    break;
            case evMouseMove:
                    if(PNumber<2)
                    {
                         Mouse.GetPosition(point);                                                            
                         if(point.X<WaveLeft)
                         {
                             point.X=WaveLeft-1;
                             Item::Message=LEFT;
                         }else  if(point.X>WaveRight)
                         {
                              point.X=WaveRight;
                              Item::Message=RIGHT;
                         }
                         else   if(point.X!=CurX)
                         {
                             Mouse.Off();
                             moveline(CurX,WaveTop,WaveBottom,ln[PNumber]);
                             CurX=point.X;
                             moveline(CurX,WaveTop,WaveBottom,ln[PNumber]);
                             Mouse.On();
                             Item::Message=evNothing;
                         }
                         else Item::Message=evNothing;
                    }
                    else   Item::Message=evNothing;
                    break;
            case evMouseLDown:
                     Mouse.GetPosition(point);
                     if(dMesure->IsInObject(point))
                     {
                         if(dMesure->Valid)
                         {
                             dMesure->Track();
                             ret=evMesure;
                             over=True;
                             Item::Message=evNothing;
                         }
                         else Item::Message=ENTER;
                     }
                     else if(dContinue->IsInObject(point))
                     {
                         dContinue->Track();
                         if(Warning("是否真的退出?")==evOk)
                         {
                             delete ShowBuffer1;
                             delete ShowBuffer2;
                             ret=evCancel;
                             over=True;
                         }
                         Item::Message=evNothing;
                     }
                     else Item::Message=ENTER;
                     break;
              case ESC:       
              case evMouseRDown:
                       if(PNumber==0)
                       {
                           if(Warning("是否真的退出?")==evOk)
                           {
                               delete ShowBuffer1;
                               delete ShowBuffer2;
                               ret=evCancel;
                               over=True;
                           }
                       }
                       else if(PNumber==1)
                       {
                           Mouse.GetPosition(point);
                           Mouse.Off();
                           moveline(point.X,WaveTop,WaveBottom,ln[1]);
                           moveline(point.X,WaveTop,WaveBottom,ln[0]);                           
                           moveline(WaveLeft+WinWidth-DataRight+FirstX,
                                    WaveTop,WaveBottom,ln[0]);
                           Mouse.On();
                           FirstX=-1;
                           PNumber--;
                       }
                       else if(PNumber==2)
                       {
                           dMesure->Valid=False;
//                           Mouse.GetPosition(point);
                           Mouse.Off();
//                           moveline(WaveLeft+WinWidth-DataRight+SecondX,WaveTop,WaveBottom,ln[1]);
//                           moveline(point.X,WaveTop,WaveBottom,ln[1]);                           
                           dMesure->Update(False);
                           if(BodyPart!=HEART)DrawBase();
                           Mouse.On();
                           SecondX=-1;
                           PNumber--;
                       }
                       Item::Message=evNothing;
                       break;
              case F1:
                       Mouse.Off();
                       if(FirstX>0)moveline(WaveLeft+WinWidth-DataRight+FirstX,
                                     WaveTop,WaveBottom,ln[0]);
                       if(SecondX>0)moveline(WaveLeft+WinWidth-DataRight+SecondX,
                                     WaveTop,WaveBottom,ln[1]);                                     
                       ShowData(False);
                       if(fScale<1500)
                          fScale+=0.20*fScale;
                       ShowData(True);
                       if(FirstX>0)moveline(WaveLeft+WinWidth-DataRight+FirstX,
                                     WaveTop,WaveBottom,ln[0]);
                       if(SecondX>0)moveline(WaveLeft+WinWidth-DataRight+SecondX,
                                     WaveTop,WaveBottom,ln[1]);                                     
                       
                       Item::Message=evNothing;
                       break;
              case F2:
                       if(FirstX>0)moveline(WaveLeft+WinWidth-DataRight+FirstX,
                                        WaveTop,WaveBottom,ln[0]);
                       if(SecondX>0)moveline(WaveLeft+WinWidth-DataRight+SecondX,
                                        WaveTop,WaveBottom,ln[1]);                                     
                       ShowData(False);
                       if(fScale>10);
                          fScale-=0.20*fScale;
                       ShowData(True);
                       if(FirstX>0)moveline(WaveLeft+WinWidth-DataRight+FirstX,
                                     WaveTop,WaveBottom,ln[0]);
                       if(SecondX>0)moveline(WaveLeft+WinWidth-DataRight+SecondX,
                                     WaveTop,WaveBottom,ln[1]);                                     
                       
                       Item::Message=evNothing;
                       break;                            
              default: Item::Message=evNothing;
        }
        GetMessage();
    }

    if(ret!=evCancel)
    {
        
        if(FirstX>SecondX)
        {
            short tmp=SecondX;
            SecondX=FirstX;
            FirstX=tmp;
        }
        iInterval=Interval*(SecondX-FirstX)/570;        
        short *data=NULL,*heart=NULL;
        data=new short[SecondX-FirstX+1];
        if(BodyPart==HEART)
           heart=new short[SecondX-FirstX+1];
        
        for(i=0;i<SecondX-FirstX+1;i++)
        {
            data[i]=ShowBuffer1[FirstX+i];
            if(BodyPart==HEART)
            {
                heart[i]=ShowBuffer2[FirstX+i];
            }
        }
        InsValue(data,SecondX-FirstX+1,MDBuffer,571);
        DtoV(MDBuffer,571);
        VtoO(MDBuffer,571);
        
        if(BodyPart==HEART)
        {
            InsValue(heart,SecondX-FirstX+1,MHBuffer,571);
            DtoV(MHBuffer,571);
            VtoO(MHBuffer,571);
            //Smooth(MHBuffer,571);
        }
/*        
        else
        {
            for(i=0;i<571;i++)
            {
                MDBuffer[i]=1./MDBuffer[i];
            }
        }
*/        
        delete ShowBuffer1;
        delete ShowBuffer2;
        ShowBuffer1=NULL;
        ShowBuffer2=NULL;
        delete data;
        delete heart;

    };
        
    Mouse.Off();
    delete tscale;
//    delete thelp;  
    delete dMesure;
    delete dContinue;
    
    _putimage(20,10,ScrBuf,_GPSET);
    Mouse.SetLimitArea(0,0,639,479);
    Mouse.On();
    delete ScrBuf;
    return ret;
}

unsigned short Mesure(void)
{
    register i;
    POINT point;
    short CurX;
    unsigned short ret=0;

    char *ScrBuf=new char[_imagesize(20,15,619,434)];
    //  copy data
    float dataUnit=0;
    float heartUnit=0;
    float dmax=0,dmin=4098,hmax=0,hmin=4098;
    Mouse.Off();
    _getimage(20,15,619,434,ScrBuf);

    _setcolor(LIGHTGRAY);
    for(i=0;i<300;i++)
    {
        _rectangle(_GBORDER,320-i,i>210?15:225-i,320+i,i>210?434:224+i);
    }
    Border3D(20,15,619,434);
    _setcolor(BLUE);
    _rectangle(_GFILLINTERIOR,30,25,609,374);
    _setcolor(GREEN);

    for(i=0;i<570;i++)
    {
        if(MDBuffer[i]>dmax)dmax=MDBuffer[i];
        if(MDBuffer[i]<dmin)dmin=MDBuffer[i];
        if(BodyPart==HEART)
        {
            if(MHBuffer[i]>hmax)hmax=MHBuffer[i];
            if(MHBuffer[i]<hmin)hmin=MHBuffer[i];
        }
    }         
    //Smooth(MDBuffer,571);
    //AutoMark(MDBuffer,MHBuffer,570,1);
    int y1,y2;
    if(BodyPart==HEART)
    {
        dataUnit=dmax-dmin;
        heartUnit=hmax-hmin;
        y1=260-((MDBuffer[0]-dmin)/dataUnit)*200;
        _moveto(left,y1);
        for(i=0;i<567;i+=1)
        {
            y1=260-((MDBuffer[i+1]-dmin)/dataUnit)*200.;
            _lineto(i+left+1,y1);
        }
        
        y2=320-((MHBuffer[0]-hmin)/heartUnit)*50;
        _moveto(left,y2);        
        for(i=0;i<569;i+=1)
        {
            y2=320-((MHBuffer[i+1]-hmin)/heartUnit)*50.;
            _lineto(i+left+1,y2);
        }
    }
    else
    {
        dataUnit=dmax-dmin;
        y1=120+(MDBuffer[0]-dmin)/dataUnit*200;
        _moveto(left,y1);
        _setcolor(GREEN);
        for(i=0;i<565;i+=7)
        {
            y1=120+((MDBuffer[i+5]-dmin)/dataUnit)*200.;
            _lineto(i+left+5,y1);
        }
    }
    int j=0;
    float time=0;
    _setcolor(YELLOW);
    _rectangle(_GBORDER,left,40,right,360);    
    for(i=0;i<right-left+1;i++)
    {
        if(time>=j)
        {
           if(j%100==0)
           {
              _moveto(left+i,360);
              _lineto(left+i,365);
           }
           else
           {
              _moveto(left+i,360);
              _lineto(left+i,362);
           }
           j+=20;
        }
        time+=iInterval*1000;
    }

   switch(BodyPart)
   {
       case HEART:
            MarkNum=9;
            break;
       case LUNG:
            MarkNum=5;
            break;
       case GAN:
            MarkNum=6;
            break;
       case SHEN:
            MarkNum=5;
            break;
       case LEFTBRAINE:
       case RIGHTBRAINE:
       case LEFTBRAINZ:
       case RIGHTBRAINZ:
            MarkNum=4;
            break;
       case ARM:
       case LEFTLEG:
       case RIGHTLEG:
            MarkNum=3;
            break;
       default:
            MarkNum=0;
            break;
   }
/*   
   _setcolor(BLUE^YELLOW);
   for(i=0;i<MarkNum;i++)
   {
       moveline(Mark[i]+left,30,350,MarkName[BodyPart][i]);
   }
*/   
   StaticText *st=new StaticText(4,POINT(5,458),MarkMention[BodyPart]);
   Button *NextStep=new Button(1,POINT(80,390),"计算结果",evNextStep,True);
   Button *Print=new Button(2,POINT(260,390),"打印波形",evPrint,True);   
   Button *Cancel=new Button(3,POINT(440,390),"取  消",evCancel,True);
   st->Draw();
   NextStep->Draw();
   Print->Draw();
   Cancel->Draw();
    
   Mouse.On();
    //AutoMark(MDBuffer,MHBuffer,570,1);
   bool init=True; 
//   CurrentMark=0;
   Mouse.Off();
   for(i=0;i<MarkNum;i++)
       moveline(Mark[i]+left,30,350,MarkName[BodyPart][i]);
   Mouse.On();

   bool over=False;
   short Bpp;
   while(!over)
    {
         switch(Item::Message)
         {
             case evMouseLDown:
                      Mouse.GetPosition(point);
                      if(NextStep->IsInObject(point))
                      {
                          if(NextStep->Valid)
                          {
                             NextStep->Track();
                             Item::Message=ENTER;
                          }
                          else Item::Message=evNothing;
                      }
                       else if(Print->IsInObject(point))
                      {
                          if(Print->Valid)
                          {
                             Print->Track();
                             PrintWave();
                             Mouse.Init();
                             Mouse.On();
                          }
                          Item::Message=evNothing;
                      }
                      else if(Cancel->IsInObject(point))
                      {
                          Cancel->Track();
                          Item::Message=evCancel;
                      }
                      else if(CurrentMark>=0)
                      {
                          if(init==False)
                          {
                              CurX=point.X;
                              Mark[0]=CurX-left;
                              init=True;
                              AutoMark(MDBuffer,MHBuffer,570,1);
                              Mouse.Off();
                              for(i=1;i<MarkNum;i++)
                              {
                                 moveline(Mark[i]+left,30,350,MarkName[BodyPart][i]);
                              }
                              Mouse.On();
                              CurrentMark=-1;
                              NextStep->Valid=True;
                              NextStep->Update(False);
                              Print->Valid=True;
                              Print->Update(False);
                              Item::Message=evNothing;
                          }
                          else
                          {
                              Item::Message=ENTER;
                              CurX=point.X;
                          }
                      }
                      else Item::Message=evNothing;
                      break;
             case ESC:         
             case evCancel:         
             case evMouseRDown:
                  if(Warning("是否真的退出?")==evOk)
                  {
                      over=True;
                      Item::Message=evCancel;
                  }
                  else Item::Message=evNothing;
                  break;
             case ENTER:
                      if(CurrentMark>=0)
                      {
                          Mark[CurrentMark]=CurX-left;
                          CurrentMark=-1;
                          NextStep->Valid=True;
                          NextStep->Update(False);
                          Print->Valid=True;
                          Print->Update(False);
                          Item::Message=evNothing;
                      }
                      else
                      {
                          NextStep->Track();
                          Item::Message=evNextStep;
                          bool error=False;
                          if(BodyPart!=HEART)
                          {
                              dmax=0;
                              for(i=Mark[0];i<Mark[MarkNum-1];i++)
                              {
                                  if(MDBuffer[i]>dmax)
                                     dmax=MDBuffer[i];
                               }
                          }
                          switch(BodyPart)
                          {
                              case HEART:
                                   short Ap;
                                   Qp=Mark[0];
                                   Rp1=Mark[1];
                                   Bp=Mark[2];
                                   Zp=Mark[3];
                                   Zy=MDBuffer[Zp]-MDBuffer[Bp];
                                   Zy/=0.80;
                                   Bpp=Zp;
                                   for(i=Zp;i>0;i--)
                                   {
                                       if(MDBuffer[Zp]-MDBuffer[i]>Zy)
                                       {
                                           Bpp=i-1;
                                           break;
                                       }
                                   }

                                   Cp=Mark[4];
                                   Cy=MDBuffer[Cp]-MDBuffer[Bpp];
                                   Xp=Mark[5];
                                   Yp=Mark[6];
                                   Op=Mark[7];
                                   Oy=MDBuffer[Op]-MDBuffer[Bpp];
                                   Rp2=Mark[8];
                  
                                   person.BeatRate=(short)(60/((Rp2-Rp1)*iInterval));
/*                                   
                                   if(Cy>0||MDBuffer[Yp]>0)
                                   {
                                       error=True;
                                   }
*/                                   
                                   for(i=Zp;i<Cp;i++)
                                   {
                                       if(MDBuffer[i]<MDBuffer[Bpp])
                                       {
                                           Bp2=i-1;
                                           break;
                                       }
                                   }
                                   Cs=0;
                                   for(i=Bpp;i<Bp2;i++)
                                       Cs+=MDBuffer[i]-MDBuffer[Bpp];
                                   Cs+=0.5*(MDBuffer[Bp2]-MDBuffer[Bpp]);
                                   Cs=Cs*iInterval;
                 
                                   Xs=0;
                                   for(i=Cp+1;i<Yp;i++)
                                      Xs+=MDBuffer[Bpp]-MDBuffer[i];
                                   Xs+=0.5*(MDBuffer[Bpp]-MDBuffer[Bp2]);
                                   Xs=Xs*iInterval;
                 
                                   Sc=100;
                                   for(i=Rp1;i<Zp;i++)
                                   {
                                       if(Sc>MDBuffer[i])
                                       {
                                           Sc=MDBuffer[i];
                                           Ap=i;
                                       }
                                   }
                                   Sc=0;
                                   for(i=Ap+1;i<Xp-1;i++)
                                   {
                                       Sc+= MDBuffer[i]-(MDBuffer[Xp-1]-MDBuffer[Ap])/
                                            (Xp-Ap+1)*i-MDBuffer[Ap];
                                   }
                                   Sc*=iInterval;
                                   break;
            case LUNG:
                     person.BeatRate=C[0]=(short)60/((Mark[4]-Mark[1])*iInterval);
                 //dmax=MDBuffer[Mark[1]];
                 HA=dmax-MDBuffer[Mark[0]];
                 
                 ST=(Mark[2]-Mark[1])*iInterval;
                 DT=(Mark[4]-Mark[2])*iInterval;
                 HS=dmax-MDBuffer[Mark[2]];
                 HD=dmax-MDBuffer[Mark[3]];
                 break;
            case GAN:
                     person.BeatRate=C[0]=(short)(60/((Mark[5]-Mark[1])*iInterval));
                 //dmax=MDBuffer[Mark[1]];
                 H2=dmax-MDBuffer[Mark[0]];
                 Al=(Mark[2]-Mark[1])*iInterval;
                 H=dmax-MDBuffer[Mark[2]];
                 H1=dmax-MDBuffer[Mark[3]];
                 Alpha=(Mark[3]-Mark[1])*iInterval;
                 Beta=(Mark[5]-Mark[3])*iInterval;
                 H3=dmax-MDBuffer[Mark[4]];
                 break;
            case SHEN:
                     person.BeatRate=C[0]=(short)60/((Mark[4]-Mark[0])*iInterval);
//                 dmax=MDBuffer[Mark[0]];
                 Al=(Mark[1]-Mark[0])*iInterval;
                 H=dmax-MDBuffer[Mark[1]];
                 Alpha=(Mark[2]-Mark[0])*iInterval;
                 Beta=(Mark[4]-Mark[2])*iInterval;
                 H3=dmax-MDBuffer[Mark[3]];
                 break;
            case LEFTBRAINE:
            case RIGHTBRAINE:
            case LEFTBRAINZ:
            case RIGHTBRAINZ:
                     person.BeatRate=C[0]=(short)(60/((Mark[3]-Mark[0])*iInterval));
//                 dmax=MDBuffer[Mark[0]];
                 TA=(Mark[2]-Mark[0])*iInterval;
                 Hb=dmax-MDBuffer[Mark[1]];
                 HS=dmax-MDBuffer[Mark[2]]; 
                 break;
            case LEFTLEG:
            case RIGHTLEG:
            case ARM:
                     person.BeatRate=C[0]=(short)60/((Mark[2]-Mark[0])*iInterval);
//                 dmax=MDBuffer[Mark[0]];
                 Zy=dmax-MDBuffer[Mark[1]];
                 break;
            default:
                 break;
        }

        if(error)
        {
            Warning("定点错误!");
                     Item::Message=evNothing;
                     error=False;
               } 
        else  over=True;
                      }
                      break;
             case LEFT:
                  Mouse.GetPosition(point); 
                  if(CurrentMark>=0&&CurX>left)
                  {
                      Mouse.Off();
                      moveline(CurX,30,350,MarkName[BodyPart][CurrentMark]);
                      CurX--;
                      moveline(CurX,30,350,MarkName[BodyPart][CurrentMark]);
                      Mouse.SetPosition(CurX,point.Y);
                      Mouse.On();
                  }
                  Item::Message=evNothing;
                  break;
             case RIGHT:
                  if(CurrentMark>=0&&CurX<right)
                  {
                      Mouse.Off();
                      moveline(CurX,30,350,MarkName[BodyPart][CurrentMark]);
                      CurX++;
                      moveline(CurX,30,350,MarkName[BodyPart][CurrentMark]);
                      Mouse.SetPosition(CurX,point.Y);
                      Mouse.On();
                  }
                  Item::Message=evNothing;
                  break; 
             case evMouseMove:
                  Mouse.GetPosition(point);
                  if(CurrentMark>=0&&point.X!=CurX)
                  {
                      Mouse.Off();
                      moveline(CurX,30,350,MarkName[BodyPart][CurrentMark]);
                      CurX=point.X;
                      moveline(CurX,30,350,MarkName[BodyPart][CurrentMark]);
                      Mouse.On();
                  }
                  Item::Message=evNothing;
                  break;         
             default:
                  Mouse.GetPosition(point);
                  char c=(char)(Item::Message&0x00ff);
                  if((c=='0'&&MarkNum<10)||c-'0'>MarkNum||CurrentMark>=0)
                     Item::Message=evNothing;
                  switch(Item::Message&0x00ff)
                  {
                      case '1':
                           CurrentMark=0;
                           CurX=Mark[0]+left;
                           break; 
                      case '2':
                           CurrentMark=1;
                           CurX=Mark[1]+left;
                           break;
                      case '3':
                           CurrentMark=2;
                           CurX=Mark[2]+left;
                           break; 
                      case '4':
                           CurrentMark=3;
                           CurX=Mark[3]+left;
                           break; 
                      
                      case '5':
                           CurrentMark=4;
                           CurX=Mark[4]+left;
                           break; 
                      
                      case '6':
                           CurrentMark=5;
                           CurX=Mark[5]+left;
                           break;
                      
                      case '7':
                           CurrentMark=6;
                           CurX=Mark[6]+left;
                           break; 
                      
                      case '8':
                           CurrentMark=7;
                           CurX=Mark[7]+left;
                           break;
                      
                      case '9':
                           CurrentMark=8;
                           CurX=Mark[8]+left;
                           break;
                      
                      case '0':
                           CurrentMark=9;
                           CurX=Mark[9]+left;
                           break; 
                  }
                  if(CurrentMark>=0)
                  {
                      NextStep->Valid=False;
                      NextStep->Update(False);
                      Print->Valid=False;
                      Print->Update(False);
                      Mouse.SetPosition(CurX,point.Y);
                  }
                  Item::Message=evNothing;
                  break;
         }
         GetMessage();
    }
    
    if(Item::Message==evNextStep)
    //prepare some parameter by result of mesruing wave figure
    {
if(BodyPart==HEART)
{
        char *screen=new char[_imagesize(0,0,639,479)];
        char mbuf[80];
        Mouse.Off();
        _getimage(0,0,639,479,screen);
        _setcolor(BLUE);
        _rectangle(_GFILLINTERIOR,0,0,639,479);
        Border3D(0,0,639,479);
              sprintf(mbuf,"Sqzx=%f",Sc);
                 _setcolor(YELLOW);
                 OutText16(10,10,0,mbuf);
                 sprintf(mbuf,"Q-B=%3.4f",(Bp-Qp)*iInterval*1000);
                 OutText16(10,30,0,mbuf);
                 sprintf(mbuf,"Q-Z=%3.4f",(Zp-Qp)*iInterval*1000);
                 OutText16(10,50,0,mbuf);
                 sprintf(mbuf,"B-X=%3.4f",(Xp-Bp)*iInterval*1000);
                 OutText16(10,70,0,mbuf);
                 sprintf(mbuf,"X-O=%3.4f",(Op-Xp)*iInterval*1000);
                 OutText16(10,90,0,mbuf);
                 sprintf(mbuf,"R-R=%3.4f",(Rp2-Rp1)*iInterval*1000);
                 OutText16(10,110,0,mbuf);
                 sprintf(mbuf,"R-B=%3.4f",(Bp-Rp1)*iInterval*1000);
                 OutText16(10,130,0,mbuf);
                 sprintf(mbuf,"Xs/Cs=%3.4f",Xs/Cs);
                 OutText16(10,150,0,mbuf);
                 sprintf(mbuf,"Op/Cp=%3.2f",Oy/Cy);
                 OutText16(10,170,0,mbuf);
                 while(!kbhit());
                 getch();
        _putimage(0,0,screen,_GPSET);
        Mouse.On(); 
}            
    }
    delete NextStep;
    delete Cancel;
    delete Print;
    delete st;
    Mouse.Off();
    _putimage(20,15,ScrBuf,_GPSET);
    Mouse.On();
    delete ScrBuf;
    return Item::Message;

}
