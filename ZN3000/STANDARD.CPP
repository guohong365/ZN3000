#include <graph.h>
#include <io.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <bios.h>

#include "mouse.hpp"
#include "hospital.hpp"
#include "const.hpp"
#include "general.hpp"
#include "popup.hpp"
extern double Interval;
extern double Interval1;
extern double Interval2;
extern double Z0;
extern MOUSE Mouse;
extern bool Pause;
extern unsigned long DataPointer;
extern short *DataBuffer;
extern short WaveZero1;
extern short *HeartBuffer;
extern unsigned short Scale;
extern unsigned short BodyPart;
extern bool Part[];
extern short Port;
void InitScreen(void);
void Pick1(void);
void Pick2(void);
unsigned short GetL();
void Standard(void)
{
    char *ScrBuf=new char[_imagesize(0,0,639,479)];
    Mouse.Off();
    _getimage(0,0,639,479,ScrBuf);
    InitScreen();
    time_t Time1;
    time_t Time2;
    _setcolor(GREEN);
//    _moveto(WaveLeft,110);
//    _lineto(WaveRight,110);
//    _moveto(WaveLeft,210);
//    _lineto(WaveRight,210);
    _rectangle(_GBORDER,WaveWinLeft,WaveWinTop,WaveWinRight,WaveWinBottom);
    Mouse.On();
    Pause=False;
    bool mouseon=True;
    DataPointer=0;
    unsigned long count=0;
    POINT CurPos;
    short Y1,Y2,off;
    register i;
    Port=28;
    BodyPart=0;
    while(!Pause)
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
       if(DataPointer<WinWidth)
       {
           Pick1();
           Y1=(DataBuffer[0]-2048)*Scale/2048;
//                                     if(WaveZero1-Y1<WaveTop) Y1=WaveZero1-WaveTop;
           if(Y1) SetPixel(WaveRight-DataPointer,WaveZero1-Y1,GREEN);
           for(i=0;i<DataPointer;i++)
           {
               off=i%TOTAL_DATA;
               Y1=(DataBuffer[off]-2048)*Scale/2048;
//                                          if(-Y1+WaveZero1<WaveTop) Y1=-WaveTop+WaveZero1;                                          
               if(Y1) SetPixel(WaveRight-DataPointer+i+1,-Y1+WaveZero1,BLUE);
               off=(i+1)%TOTAL_DATA;
               Y1=(DataBuffer[off]-2048)*Scale/2048;                                          
//                                          if(-Y1+WaveZero2<WaveTop) Y1=-WaveTop+WaveZero1;
              if(Y1) SetPixel(WaveRight-DataPointer+i+1,-Y1+WaveZero1,GREEN);
           }
           
if(BodyPart==0)
{
           Pick2();
           Y2=(HeartBuffer[0]-2048)*Scale/4096;
//                                     if(WaveZero2-Y2>WaveBottom) Y2=WaveZero2-WaveBottom;
           if(Y2) SetPixel(WaveRight-DataPointer,WaveZero2-Y2,GREEN);
           for(i=0;i<DataPointer;i++)
           {
               off=i%TOTAL_DATA;
               Y2=(HeartBuffer[off]-2048)*Scale/4096;                                          
//                                          if(-Y2+WaveZero2>WaveBottom) Y2=-WaveBottom+WaveZero2;
               if(Y2) SetPixel(WaveRight-DataPointer+i+1,-Y2+WaveZero2,BLUE);
               off=(i+1)%TOTAL_DATA;
               Y2=(HeartBuffer[off]-2048)*Scale/4096;
//                                          if(-Y2+WaveZero2>WaveBottom) Y2=-WaveBottom+WaveZero2;
              if(Y2) SetPixel(WaveRight-DataPointer+i+1,-Y2+WaveZero2,GREEN);
           }
}           
           DataPointer++;
       }
       else
       {
           Pick1();
           for(i=0;i<WinWidth;i++)
           {
               off=(DataPointer-WinWidth+i)%TOTAL_DATA;
               Y1=(DataBuffer[off]-2048)*Scale/2048;                                          
//                                          if(-Y1+WaveZero2<WaveTop) Y1=WaveTop-WaveZero1;
              if(Y1) SetPixel(WaveRight-WinWidth+i+1,-Y1+WaveZero1,BLUE);
              off=(DataPointer-WinWidth+i+1)%TOTAL_DATA;
              Y1=(DataBuffer[off]-2048)*Scale/2048;                                          
//                                          if(-Y1+WaveZero2<WaveTop) Y1=-WaveTop+WaveZero1;
             if(Y1) SetPixel(WaveRight-WinWidth+i+1,-Y1+WaveZero1,GREEN);
           }
if(BodyPart==0)
{
           Pick2();
           for(i=0;i<WinWidth;i++)
           {
               off=(DataPointer-WinWidth+i)%TOTAL_DATA;
               Y2=(HeartBuffer[off]-2048)*Scale/4096;
//                                          if(-Y2+WaveZero2>WaveBottom) Y2=WaveBottom-WaveZero2;
              if(Y2) SetPixel(WaveRight-WinWidth+i+1,-Y2+WaveZero2,BLUE);
              off=(DataPointer-WinWidth+i+1)%TOTAL_DATA;
              Y2=(HeartBuffer[off]-2048)*Scale/4096;
//                                          if(-Y2+WaveZero2>WaveBottom) Y2=-WaveBottom+WaveZero2;
             if(Y2) SetPixel(WaveRight-WinWidth+i+1,-Y2+WaveZero2,GREEN);
           }
}           
           DataPointer++;
       }
       if(DataPointer==1000)
       {
            Time1=time(NULL);
       }
       if(DataPointer==8000)
       {
           Time2=time(NULL);
           Pause=True;
       }
       
    }
    
    Interval1=difftime(Time2,Time1)/7000.;
    _setcolor(BLUE);
    _rectangle(_GFILLINTERIOR,WaveWinLeft,WaveWinTop,WaveWinRight,WaveWinBottom);
    _setcolor(GREEN);
    _rectangle(_GBORDER,WaveWinLeft,WaveWinTop,WaveWinRight,WaveWinBottom);    
    Pause=False;
    Port=26;
    BodyPart=1;
    DataPointer=0;
    while(!Pause)
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
       if(DataPointer<WinWidth)
       {
           Pick1();
           Y1=(DataBuffer[0]-2048)*Scale/2048;
//                                     if(WaveZero1-Y1<WaveTop) Y1=WaveZero1-WaveTop;
           if(Y1) SetPixel(WaveRight-DataPointer,WaveZero1-Y1,GREEN);
           for(i=0;i<DataPointer;i++)
           {
               off=i%TOTAL_DATA;
               Y1=(DataBuffer[off]-2048)*Scale/2048;
//                                          if(-Y1+WaveZero1<WaveTop) Y1=-WaveTop+WaveZero1;                                          
               if(Y1) SetPixel(WaveRight-DataPointer+i+1,-Y1+WaveZero1,BLUE);
               off=(i+1)%TOTAL_DATA;
               Y1=(DataBuffer[off]-2048)*Scale/2048;                                          
//                                          if(-Y1+WaveZero2<WaveTop) Y1=-WaveTop+WaveZero1;
              if(Y1) SetPixel(WaveRight-DataPointer+i+1,-Y1+WaveZero1,GREEN);
           }
if(BodyPart==0)
{           
           Pick2();
           Y2=(HeartBuffer[0]-2048)*Scale/4096;
//                                     if(WaveZero2-Y2>WaveBottom) Y2=WaveZero2-WaveBottom;
           if(Y2) SetPixel(WaveRight-DataPointer,WaveZero2-Y2,GREEN);
           for(i=0;i<DataPointer;i++)
           {
               off=i%TOTAL_DATA;
               Y2=(HeartBuffer[off]-2048)*Scale/4096;                                          
//                                          if(-Y2+WaveZero2>WaveBottom) Y2=-WaveBottom+WaveZero2;
               if(Y2) SetPixel(WaveRight-DataPointer+i+1,-Y2+WaveZero2,BLUE);
               off=(i+1)%TOTAL_DATA;
               Y2=(HeartBuffer[off]-2048)*Scale/4096;
//                                          if(-Y2+WaveZero2>WaveBottom) Y2=-WaveBottom+WaveZero2;
              if(Y2) SetPixel(WaveRight-DataPointer+i+1,-Y2+WaveZero2,GREEN);
           }
 }          
           DataPointer++;
       }
       else
       {
           Pick1();
           for(i=0;i<WinWidth;i++)
           {
               off=(DataPointer-WinWidth+i)%TOTAL_DATA;
               Y1=(DataBuffer[off]-2048)*Scale/2048;                                          
//                                          if(-Y1+WaveZero2<WaveTop) Y1=WaveTop-WaveZero1;
              if(Y1) SetPixel(WaveRight-WinWidth+i+1,-Y1+WaveZero1,BLUE);
              off=(DataPointer-WinWidth+i+1)%TOTAL_DATA;
              Y1=(DataBuffer[off]-2048)*Scale/2048;                                          
//                                          if(-Y1+WaveZero2<WaveTop) Y1=-WaveTop+WaveZero1;
             if(Y1) SetPixel(WaveRight-WinWidth+i+1,-Y1+WaveZero1,GREEN);
           }
if(BodyPart==0)
{
           Pick2();
           for(i=0;i<WinWidth;i++)
           {
               off=(DataPointer-WinWidth+i)%TOTAL_DATA;
               Y2=(HeartBuffer[off]-2048)*Scale/4096;
//                                          if(-Y2+WaveZero2>WaveBottom) Y2=WaveBottom-WaveZero2;
              if(Y2) SetPixel(WaveRight-WinWidth+i+1,-Y2+WaveZero2,BLUE);
              off=(DataPointer-WinWidth+i+1)%TOTAL_DATA;
              Y2=(HeartBuffer[off]-2048)*Scale/4096;
//                                          if(-Y2+WaveZero2>WaveBottom) Y2=-WaveBottom+WaveZero2;
             if(Y2) SetPixel(WaveRight-WinWidth+i+1,-Y2+WaveZero2,GREEN);
           }
}          
           DataPointer++;
       }
       if(DataPointer==1000)
       {
            Time1=time(NULL);
       }
       if(DataPointer==8000)
       {
           Time2=time(NULL);
           Pause=True;
       }
       
    }
    
    Interval2=difftime(Time2,Time1)/7000.;
    GetL();
    FILE *std=fopen("std.ini","wb");
    if(!std)return;
    fwrite(&Interval1,1,sizeof(double),std);
    fwrite(&Interval2,1,sizeof(double),std);
    fwrite(&Z0,1,sizeof(double),std);
    fclose(std);
    Port=28;
    BodyPart=0;
/*    
///---------------------------------------------------    
    char name[20];
    sprintf(name,"%d.tet",BodyPart);
    std=fopen(name,"wb");
    if(std)
    {
        fwrite(DataBuffer,5000,sizeof(short),std);
        if(BodyPart==HEART)
        {
            fwrite(HeartBuffer,5000,sizeof(short),std);
        }
        fclose(std);
    }
    Part[BodyPart]=True;
//------------------------------------------------------
*/
    Mouse.Off();
    _putimage(0,0,ScrBuf,_GPSET);
    Mouse.On();
    delete ScrBuf;
};





