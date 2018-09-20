#include <graph.h>
#include <stdio.h>
#include <conio.h>
#include <stdlib.h>
#include "mouse.hpp"
#include "general.hpp"
#include "hospital.hpp"
#include "Editbar.hpp"
#include "popup.hpp"
#include "const.hpp"
extern MOUSE Mouse;
extern double Z0;
extern Person person;
char IsFloat(short);
void VtoO(float*,unsigned short );
unsigned short GetL()
{
    POINT point;
    char buf[30];
    char *ScrBuf=new char [_imagesize(150,100,490,300)];
    float ttmp;
    Mouse.Off();
    _getimage(150,100,490,300,ScrBuf);

    _setcolor(LIGHTGRAY);
    for(int i=0;i<170;i++)
    {
        _rectangle(_GBORDER,320-i,i>100?100:200-i,320+i,i>100?300:200+i);
    }
    Border3D(150,100,490,300);
    Object *Head=new Object(
            *(new EditBar(1,POINT(185,160),"请输入基础导纳(姆欧):",IsFloat,NULL,4))+
            *(new Button(12,POINT(200,240),"确  定",evOk,True))+
            *(new Button(13,POINT(350,240),"取  消",evCancel,True)));
                                  
    StaticText*mention=new StaticText(0,POINT(165,125),"请输入下列参数:");
    sprintf(buf,"%-3.1f",Z0);
    GetObject(Head,1)->SetData(buf);        
    mention->Draw();                                  
    Head->Draw();
    Mouse.On();
    Object *Cur=Head->Next,*tmp;
    bool over=False;
    bool abondant=False;
    while(!over)
    {
         switch(Item::Message)
         {
             case evNext:
                      if(Cur->Next==NULL)
                          Cur=Head->Next;
                      else Cur=Cur->Next;
                      Item::Message=evNothing;    
                      break; 
             case evMouseLDown:
                      Mouse.GetPosition(point);
                      if(!Cur->IsInObject(point))
                      {
                          tmp=Head->Next;
                          while(tmp)
                          {
                              if(tmp->IsInObject(point))
                              {
                                  Cur->Update(False);
                                  Cur=tmp;
                                  break;
                              }
                              tmp=tmp->Next;
                          }
                      }
                      Item::Message=evNothing;
                      break;
             case evOk:     
                  if(Warning("所有参数都正确吗?")==evOk)
                    over=True;
                  break;    
             case evCancel:         
             case evMouseRDown:
                      if(Warning("放弃输入的参数吗?")==evOk)
                      {
                         Cur->Update(False);
                         abondant=True;
                         over=True;
                      }
                      break; 
             default:
                     Cur->Do();
                     break;
         }
    }
    delete mention;
    Item::Message=evNothing;
    Mouse.Off();
    _putimage(150,100,ScrBuf,_GPSET);
    Mouse.On();
    if(!abondant)
    {
        GetObject(Head,1)->GetData(buf);
        ttmp=atof(buf);
        VtoO(&ttmp,1);
        Z0=ttmp;
    }
    delete ScrBuf;
    delete Head;
    if(!abondant)return evOk;
    return evCancel;
}
