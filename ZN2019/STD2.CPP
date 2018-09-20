#include <graph.h>
#include <io.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <bios.h>
#include <i86.h>
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
void Std2(void)
{
    
    char *ScrBuf=new char[_imagesize(0,0,639,479)];
    short Min=4095,Max=0;
    double inter;
    char buf[50];
    Mouse.Off();
    _getimage(0,0,639,479,ScrBuf);
    InitScreen();
    time_t Time1;
    time_t Time2;
    _setcolor(GREEN);
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
           DataPointer++;
       }
       if(DataPointer==1000)
       {
            Time1=time(NULL);
       }
       if(DataPointer==2000)
       {
           Time2=time(NULL);
           inter=difftime(Time2,Time1);
           for(i=1000;i<2000;i++)
           {
               if(DataBuffer[i%TOTAL_DATA]>Max)
                  Max=DataBuffer[i%TOTAL_DATA];
               if(DataBuffer[i%TOTAL_DATA]<Min)
                  Min=DataBuffer[i%TOTAL_DATA];   
           }
           _setcolor(BLUE);
           _rectangle(_GFILLINTERIOR,WaveWinLeft,WaveWinTop,WaveWinRight,WaveWinBottom);
           _setcolor(GREEN);
           _rectangle(_GBORDER,WaveWinLeft,WaveWinTop,WaveWinRight,WaveWinBottom);
           sprintf(buf,"最大:%d   最小:%d",Max,Min);
           OutText16(WaveWinLeft+50,WaveWinTop+20,0,buf);
           sprintf(buf,"记时:%7.3fms",inter);
           OutText16(WaveWinLeft+50,WaveWinTop+40,0,buf);
           delay(3000);
           if(Warning("定标完成吗?")==evOk)
           {
               Pause=True;
           }
           else
           {
           _setcolor(BLUE);
           _rectangle(_GFILLINTERIOR,WaveWinLeft,WaveWinTop,WaveWinRight,WaveWinBottom);
           _setcolor(GREEN);
           _rectangle(_GBORDER,WaveWinLeft,WaveWinTop,WaveWinRight,WaveWinBottom);
           DataPointer=0;
           Max=0;
           Min=4096;
           }
       }
    }      
    Mouse.Off();
    _putimage(0,0,ScrBuf,_GPSET);
    Mouse.On();
    delete ScrBuf;
};





