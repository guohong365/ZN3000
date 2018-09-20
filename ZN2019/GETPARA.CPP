#include <graph.h>
#include <stdio.h>
#include <conio.h>
#include <stdlib.h>
#include <ctype.h>
#include <time.h>
#include "mouse.hpp"
#include "general.hpp"
#include "hospital.hpp"
#include "Editbar.hpp"
#include "popup.hpp"
#include "const.hpp"
extern MOUSE Mouse;
extern double A[];
extern double L;
char IsPrint(short in);
char IsFloat(short in);
char IsNumber(short in);
void CalculateA();
void CalculateB();
bool FirstZero(void *buf);
unsigned short GetPara(Person &PersonAttr )
{
    
    POINT point;
    char buf[30];
    short sex;
    Person man;
    FILE *last=fopen("last.inf","rb");
    if(last)
    {
        fread(&man,1,sizeof(Person),last);
        if(atol(man.No)>atol(PersonAttr.No))
           PersonAttr=man;
        else man=PersonAttr;
        fclose(last);   
    }
        time_t tt;
        tt=time(NULL);
        strftime(PersonAttr.Date,9,"%Y%m%d",localtime(&tt));

    char *ScrBuf=new char [_imagesize(20,15,619,434)];
    Mouse.Off();
    _getimage(20,15,619,434,ScrBuf);

    _setcolor(LIGHTGRAY);
    for(int i=0;i<300;i++)
    {
        _rectangle(_GBORDER,320-i,i>210?15:225-i,320+i,i>210?434:224+i);
    }
    Border3D(20,15,619,434);
    strcpy(buf,"检查日期:");
    strcat(buf,PersonAttr.Date);
    StaticText*date=new StaticText(0,POINT(380,85),buf);    
    Object *Head=new Object(
            *(new EditBar(1,POINT(75,85),"姓名:",IsPrint,NULL,8))+
            *(new EditBar(2,POINT(230,85),"编号:",IsNumber,FirstZero,8))+
          (
            *(new SingleSlct(4,POINT(75,135),"性别:"))-(
            *(new SingleSlctItem(1,POINT(130,127),"男",True))-
            *(new SingleSlctItem(2,POINT(130,147),"女")) )
          )+
            *(new EditBar(5,POINT(200,135),"年龄(岁):",IsNumber,NULL,3))+
            *(new EditBar(6,POINT(390,135),"体重(公斤):",IsNumber,NULL,5))+
            *(new EditBar(7,POINT(75,180),"身高(厘米):",IsNumber,NULL,5))+
            *(new EditBar(8,POINT(275,180),"血压(毫米汞柱):",IsNumber,NULL,5))+
            *(new EditBar(9,POINT(490,180),"/",IsNumber,NULL,5))+
            *(new EditBar(12,POINT(75,225),"心率(次/秒):",IsNumber,NULL,3))+                        
            *(new EditBar(11,POINT(275,225),"电极间距(厘米):",IsFloat,NULL,4))+
            *(new EditBar(10,POINT(75,265),"海拔(米):",IsNumber,NULL,7))+            
            *(new Button(13,POINT(200,330),"确  定",evOk,True))+
            *(new Button(14,POINT(350,330),"取  消",evCancel,True)));
                                  
    StaticText*mention=new StaticText(0,POINT(35,35),"请输入下列参数:");
        strcpy(buf,PersonAttr.Name);    
        GetObject(Head,1)->SetData(buf);
        strcpy(buf,PersonAttr.No);
        GetObject(Head,2)->SetData(buf);

        if(PersonAttr.Sex==Person::Male)
          sex=1;
        else sex=2;
        GetObject(Head,4)->SetData(&sex);
        itoa(PersonAttr.Age,buf,10);
        GetObject(Head,5)->SetData(buf);
        sprintf(buf,"%-d",(int)PersonAttr.Weight);
        GetObject(Head,6)->SetData(buf);
        sprintf(buf,"%-d",(int)PersonAttr.Height);
        GetObject(Head,7)->SetData(buf);
        itoa(PersonAttr.HiBloodPresure,buf,10);
        GetObject(Head,8)->SetData(buf);
        itoa(PersonAttr.LowBloodPresure,buf,10);
        GetObject(Head,9)->SetData(buf);
        sprintf(buf,"%-d",(int)PersonAttr.HaiBa);
        GetObject(Head,10)->SetData(buf);
        sprintf(buf,"%-3.1f",PersonAttr.DjInterval);
        GetObject(Head,11)->SetData(buf);
        sprintf(buf,"%-d",PersonAttr.BeatRate);
        GetObject(Head,12)->SetData(buf);
        
    mention->Draw();
    date->Draw();
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
    delete date;
    Item::Message=evNothing;
    Mouse.Off();
    _putimage(20,15,ScrBuf,_GPSET);
    Mouse.On();
    if(!abondant)
    {
        GetObject(Head,1)->GetData(buf);
        strcpy(PersonAttr.Name,buf);
        GetObject(Head,2)->GetData(buf);
        strcpy(PersonAttr.No,buf);
//        GetObject(Head,3)->GetData(buf);
//        strcpy(PersonAttr.Date,buf);
        GetObject(Head,4)->GetData(&sex);
        if(sex==1)PersonAttr.Sex=Person::Male;
        else PersonAttr.Sex=Person::Famale;
        GetObject(Head,5)->GetData(buf);
        PersonAttr.Age=atoi(buf);
        GetObject(Head,6)->GetData(buf);
        PersonAttr.Weight=atoi(buf);
        GetObject(Head,7)->GetData(buf);
        PersonAttr.Height=atoi(buf);
        GetObject(Head,8)->GetData(buf);
        PersonAttr.HiBloodPresure=atoi(buf);
        GetObject(Head,9)->GetData(buf);
        PersonAttr.LowBloodPresure=atoi(buf);
        GetObject(Head,10)->GetData(buf);
        A[8]=PersonAttr.HaiBa=atoi(buf);
        GetObject(Head,11)->GetData(buf);
        PersonAttr.DjInterval=L=atof(buf);
        GetObject(Head,12)->GetData(buf);
        PersonAttr.BeatRate=A[0]=atoi(buf);
        CalculateA();
        last=fopen("last.inf","wb");
        if(last)
        {
            if(atol(PersonAttr.No)>atol(man.No))
            {
                fwrite(&PersonAttr,1,sizeof(Person),last);
            }
            else fwrite(&man,1,sizeof(Person),last);
            fclose(last);
        }
    }
    delete ScrBuf;
    delete Head;
    if(!abondant)
        return evOk;
    return evCancel;
}

char IsNumber(short in)
{
    char c=in&0xff;
    if(!isdigit(c))
      c='\0';
    return c;
};

char IsFloat(short in)
{
    char c=in&0xff;
    if(!isdigit(c)&&c!='.')
      c='\0';
    return c;
}

char IsPrint(short in)
{
    char c=in&0xff;
    if(!isprint(c))
      c='\0';
    return c;
}

bool FirstZero(void *buf)
{
    char *d=(char*)buf;
   if(d[0]=='0')
   {
      Warning("编号第一位数字不能为零!");
      return False;
   }
   return True;
}
   
