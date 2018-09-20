#include <i86.h>
#include <conio.h>
#include <stdlib.h>
#include "const.hpp"
#include "printer.hpp"
#include "editbar.hpp"
#include "general.hpp"
#include "mouse.hpp"
extern MOUSE Mouse;
extern Printer PRN;
void PrintSetup(void)
{
    FILE *cfg=fopen("print.cfg","rb");
    unsigned short ss=0;
    char et[10];
    Printer prnc;//(NoPrinter,Middle,(short)4000,(short)3000,(short)400,(short)300,False,True);
    if(!cfg)
    {
        prnc=Printer(NoPrinter,Middle,(short)4000,(short)3000,(short)400,(short)300,False,True);        
        cfg=fopen("print.cfg","wb");
        if(!cfg)
        {
            Warning("Can not open config file");
            return;
        }
        fwrite(&prnc,sizeof(Printer),1,cfg);
    }
    else fread(&prnc,sizeof(Printer),1,cfg);
    fclose(cfg);

    char *ScrBuf=new char[_imagesize(20,10,620,450)];
    
    Object *Head=new Object(
                                   (*(new SingleSlct(1,POINT(45,25),"打印机型号:"))-(
                                         *(new SingleSlctItem(0,POINT(60,45),(char*)PrinterName[0]))-
                                         *(new SingleSlctItem(1,POINT(60,65),(char*)PrinterName[1]))-
                                         *(new SingleSlctItem(3,POINT(60,85),(char*)PrinterName[3]))-
                                         *(new SingleSlctItem(4,POINT(60,105),(char*)PrinterName[4]))-
                                         *(new SingleSlctItem(5,POINT(60,125),(char*)PrinterName[5]))-
                                         *(new SingleSlctItem(6,POINT(60,145),(char*)PrinterName[6]))-
                                         *(new SingleSlctItem(7,POINT(60,165),(char*)PrinterName[7]))-
                                         *(new SingleSlctItem(10,POINT(60,185),(char*)PrinterName[10])) ))+
                                  (*(new SingleSlct(2,POINT(360,25),"分辨率:"))-(
                                         *(new SingleSlctItem(1,POINT(380,45),(char*)(PrinterModeName[0][0])))-
                                         *(new SingleSlctItem(2,POINT(380,65),(char*)PrinterModeName[0][1]))-
                                         *(new SingleSlctItem(3,POINT(380,85),(char*)PrinterModeName[0][2]))-
                                         *(new SingleSlctItem(4,POINT(380,105),(char*)PrinterModeName[0][3])) ))+
                                  *(new EditBar(3,POINT(360,145),"图宽[1/1000]inch:",NULL,NULL,5))+
                                  *(new EditBar(4,POINT(360,185),"图高[1/1000]inch:",NULL,NULL,5))+
                                  *(new Button(5,POINT(150,390),"确  定",evOk,True))+
                                  *(new Button(6,POINT(350,390),"取  消",evCancel,True)));
    ss=(unsigned short )prnc.Type;
    
   GetObject(Head,1)->SetData(&ss);
   ss=(unsigned short )prnc.Mode;
   GetObject(Head,2)->SetData(&ss);
   sprintf(et,"%hu",prnc.Width);
   GetObject(Head,3)->SetData(et);
   sprintf(et,"%hu",prnc.Height);
   GetObject(Head,4)->SetData(et);
    Mouse.Off();
    _getimage(20,10,620,450,ScrBuf);
    _setcolor(LIGHTGRAY);
    for(int i=0;i<300;i++)
    {
        _rectangle(_GBORDER,320-i,i>220?10:230-i,320+i,i>220?450:230+i);
    }
    Border3D(20,10,620,450);
    Border3D(40,230,600,370);
    Head->Draw();
    Mouse.On();
    while(1)
    {
        if(Head->Do())
       {
           GetObject(Head,1)->GetData(&ss);
           prnc.Type=(PrinterType)ss;
           GetObject(Head,2)->GetData(&ss);
           prnc.Mode=(PrinterMode)ss;
           GetObject(Head,3)->GetData(et);
           prnc.Width=atoi(et);
           prnc.LeftMargin=prnc.Width/10;
           GetObject(Head,4)->GetData(et);
           prnc.Height=atoi(et);
           PRN.TopMargin=prnc.Height/10;
           cfg=fopen("print.cfg","wb");
           if(!cfg)
          {
              Warning("Print Setup failed.");
              return;
          }
          fwrite(&prnc,sizeof(Printer),1,cfg);
          fclose(cfg);
          break;
       }
    else  if(Warning("Are you aboundant ?")==evOk)break;
    }
    Mouse.Off();
    _putimage(20,10,ScrBuf,_GPSET);
    Mouse.On();
    delete Head;
    delete ScrBuf;
};

void Print(void(*prn)(FILE *ff))
{
    FILE *cfg=fopen("prn.cfg","r");
    FILE *PrintFile;
    if(!prn)
    {
        Warning("no print function");
        return;
    }
    if(!cfg)
    {
        if(Warning("please setup printer at first")==evOk)
        {
            PrintSetup();
            cfg=fopen("print.cfg","r");
        }
        else return;
    }
    fread(&PRN,sizeof(Printer),1,cfg);
    fclose(cfg);
    PrintFile=fopen("Print.swp","w");
    if(!PrintFile)
    {
        Warning("can not open print swap file");
        return;
    }
    fwrite(&PRN,sizeof(Printer),1,PrintFile);
    Wait("Prepare print swap file",True);
    prn(PrintFile);
    Wait("Prepare print swap file",False);
    Wait("Printing...",True);
    delay(3000);
    Wait("Printing...",False);
};
