#include <stdio.h>
#include <stdlib.h>
#include <conio.h>
#include <graph.h>
#include <dos.h>
#include <i86.h>
#include "hospital.hpp"
extern unsigned long DataPointer;
extern short *DataBuffer;
extern short *HeartBuffer;
extern unsigned short Scale;
extern short Base;
extern short Port;
void Pick1(void)
{
    unsigned long num=DataPointer%TOTAL_DATA;
    outp(Base,Port);
    for(int i=0;i<1500;i++);
    while(!inp(Base+5)&0x80);
    DataBuffer[num]=inp(Base+1)|((inp(Base+2)&0x0f)<<8);
};
void Pick2(void)
{
    unsigned long num=DataPointer%TOTAL_DATA;
    outp(Base,30);
    for(int i=0;i<1500;i++);    
    while(!inp(Base+5)&0x80);
    HeartBuffer[num]=inp(Base+1)|((inp(Base+2)&0x0f)<<8);
//    if(DataPointer>3)
//      HeartBuffer[num]=(HeartBuffer[num]+HeartBuffer[num-1])/2;
};
      
      
        
        

