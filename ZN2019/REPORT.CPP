#include <stdlib.h>
#include <i86.h>
#include "hospital.hpp"
#include "general.hpp"
#include "popup.hpp"
#include "const.hpp"
#include "mouse.hpp"
#include "editbar.hpp"
#include <graph.h>
#include <bios.h>

extern double HeartB[];
extern double LungB[];
extern double GanB[];
extern double ShenB[];
extern double LeftBrainEB[];
extern double LeftBrainZB[];
extern double RightBrainEB[];
extern double RightBrainZB[];
extern double ArmB[];
extern double LegB[];
extern char   *Csav[];

extern MOUSE Mouse;
extern short WaveZero1;
extern unsigned short Scale;
extern float fScale;
extern unsigned long DataPointer;
extern Person person;
extern double Interval;
extern double iInterval;
extern short Ap,Bp,Op,Xp,Yp,Zp,Rp1,Rp2,Cp,Qp,Bp2;
extern double Cs,Xs,Al,Sc;
extern double HD,HS,HA,L,Oy,ST,DT,H,H1,H2,H3,Hb; 
extern double Ay,By,Cy,Zy,Z0,HBZTZ,LBZTZ,BZTZ,TA,Alpha,Beta,Y0;
extern short BeatRate;
extern short Mark[];
extern unsigned short BodyPart;
const short left=35;
const short right=605; 
extern short MarkNum;
extern char MarkName[][10][3];
extern float MHBuffer[571];
extern float MDBuffer[571];

void moveline(short x,short y1,short y2,char *name=NULL);                            
char IsNumber(short);
bool LoadData(char *,short no);
bool LoadData(char *);
void ViewHeart();
void ViewLung();
void ViewGan();
void ViewShen();
void ViewBrain();
void ViewBody();
void PrintWave();
void ViewFigure(short);
void CalculateA();
void Calculate();
short ReSort[][30]=
{
    {7,9,8,10,12,13,11,1,14,21,15,19,16,24, 2,18,20,25, 3,27,22,26,17,28,23, 0, 0, 0, 0},
    {4,0,2, 1, 3, 5, 7,6, 8, 9, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
    {0,6,1, 7, 2, 5, 3,8, 4, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
    {3,1,4, 5, 2, 0, 0,0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
    {0,1,2, 3, 4, 5, 6,7, 8, 9,10,11,12,13,14,15,16,17,18,19,20,21,22,23,24,25,26,27,28},
    {0,1,2, 3, 0, 0, 0,0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
};
char mainitem[][40]=
{
"心血管功能",
"肺血管功能",
"肝血管功能",
"肾血管功能(单侧)",
"脑血管功能",
"肢体血管功能",
};
short LBE[][8]={
    {0,4,8,12,18,22},
    {1,5,9,13,16,17,19,23}
};
char BrainItem[][40]={
"左颈内动脉颅内枝收缩波振幅ohm",
"右颈内动脉颅内枝收缩波振幅ohm",
"左椎动脉颅内枝收缩波振幅ohm",
"右椎动脉颅内枝收缩波振幅ohm",
"左颈内动脉颅内枝流入时间sec",
"右颈内动脉颅内枝流入时间sec",
"左椎动脉颅内枝流入时间sec",
"右椎动脉颅内枝流入时间sec",
"左颈内动脉颅内枝流入容积速度ohm/sec",
"右颈内动脉颅内枝流入容积速度ohm/sec",
"左椎动脉颅内枝流入容积速度ohm/sec",
"右椎动脉颅内枝流入容积速度ohm/sec",
"左颈内动脉颅内枝转折高比值",
"右颈内动脉颅内枝转折高比值",
"左椎动脉颅内枝转折高比值",
"右椎动脉颅内枝转折高比值",
"左右颈内动脉颅内枝波幅差比值",
"右颈内动脉颅内枝波幅差比值",
"左颈内动脉颅内枝血流量ml/min",
"右颈内动脉颅内枝血流量ml/min",
"左椎动脉颅内枝血流量ml/min",
"右椎动脉颅内枝血流量ml/min",
"左颈内动脉颅内枝平均血流速度cm/sec",
"右颈内动脉颅内枝平均血流速度cm/sec",
"左椎动脉颅内枝平均血流速度cm/sec",
"右椎动脉颅内枝平均血流速度cm/sec"
};



char HeartItem[][40]=
{
"每博输出量ml/beat",                     "每博指数ml/beat/m",
"每分输出量L/min",                       "心脏指数L/m^2.m",
"博功指数Kg.m/m^2.beat",                 "心功指数Kg.m/m^2.min",
"心输出量与体重比值(co/wt)L.min/Kg",     "射血时间sec",
"总外周阻力 dyn.s.cm^-5",                "射血时间校正值ns",
"肺小动脉楔嵌压mmHg",                  "射血前时间/射血时间",
"扩张指数ml/mmHg",                       "射血前时间/射血时间校正值",
"射血前时间sec",                         "等容舒张时间sec",
"射血前时间校正值ns",                    "冠脉灌注压 mmHg",
"总机械收缩时间sec",                     "冠脉循环每博量ml/beat",
"总机械收缩时间校正值ns",                "冠脉循环血量ml/min",
"电激动时间及快速射血时间sec",           "冠脉平均流速cm/sec",
"电激动时间及快速射血时间校正值ns"
};

char LungItem[][40]=
{
"房缩波(A波)振幅omh",
"上升时间sec",
"室缩波(S波)振幅omh",
"下降时间sec",
"室舒波(D波)振幅omh",
"流入容积速度ohm/sec",
"房缩波振幅/室缩波振幅(Ha/Hs)",
"流出容积速度ohm/sec",
"室舒波振幅/室缩波振幅(HD/Hs)",
};

char GanItem[][40]=
{
"房缩波(a波)振幅ohm",
"肝阻力指数(Ha/HS)",
"室缩波(s波)振幅ohm",
"舒张指数(HD/HS)",
"室缩波(D波)振幅ohm",
"室缩波上升时间sec",
"收缩波时间(TS)sec",
"降中峡指数",
"舒张时间(TD)sec"
};

char ShenItem[][40]=
{
"收缩波(s波)振幅ohm",
"舒张波时间sec",
"舒张波(D波)振幅ohm",
"舒张指数(HD/HS)",
"收缩波上升时间sec",
"收缩波时间sec"
};

char BodyItem[][40]=
{
  "左小腿",
  "右小腿",
  "前臂",
  "肺/肢阻抗比"
};


extern Person person;    
extern MOUSE Mouse;
extern bool Part[];
void Save(void);
void Report(void)
{

    short i;
    POINT point;
    char buff[80];
    char name1[13],name2[13];
    char name[14];
    MESSAGE *m;
    char *Scr=new char[_imagesize(80,150,460,340)];

    Mouse.Off();
    _getimage(80,150,460,340,Scr);
    _setcolor(LIGHTGRAY);
    _rectangle(_GFILLINTERIOR,80,150,460,340);
    Border3D(80,150,460,340);

    StaticText *st=new StaticText(0,POINT(95,165),"请输入患者编号:");
    StaticText *st2=new StaticText(3,POINT(400,205),"号");    
    Object *Head=new Object(
            *(new EditBar(1,POINT(135,205),"从",IsNumber,NULL,8))+
            *(new EditBar(2,POINT(250,205),"号 到",IsNumber,NULL,8))+
            *(new Button(12,POINT(140,270),"观看结果",evOk,True))+
            *(new Button(13,POINT(320,270),"打印报告",evCancel,True)));
    st->Draw();
    st2->Draw();
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
                  //if(Warning("所有参数都正确吗?")==evOk)
                    over=True;
                  break;
             case ESC:     
             case evMouseRDown:
                  if(Warning("放弃查询吗?")==evOk)
                  {
                      abondant=True;
                      over=True;
                  }
                  break; 
             case evCancel:                      
                      if(Warning("只打印报告吗?")==evOk)
                      {
                         Cur->Update(False);
                         abondant=True;
                         Mouse.Off();
                         char *scc=new char[_imagesize(0,0,639,479)];
                         _getimage(0,0,639,479,scc);
                         sprintf(name,"%s.znd",person.No);
                         sprintf(buff,"print.exe %s>nul",name);
                         m=new MESSAGE("正在打印，请稍等....");
                         system(buff);
                         delete m;
                         _putimage(0,0,scc,_GPSET);
                         delete scc;
                         Mouse.Init();
                         Mouse.On();
                         over=True;
                      }
                      break; 
             default:
                     Cur->Do();
                     break;
         }
    }
    delete st;
    delete st2;
    Item::Message=evNothing;
    GetObject(Head,1)->GetData(name1);
    GetObject(Head,2)->GetData(name2);
    delete Head;
    Mouse.Off();
    _putimage(80,150,Scr,_GPSET);
    Mouse.On();
    delete Scr;
    if(abondant)return;

    
    long No1=atol(name1);
    long No2=atol(name2);
    if(No1==0)
    {
        No1=atol(person.No);
        No2=No1+1;
    }
    else if(No2==0)
    {
        No2=No1+1;
    }
    else if(No2<No1)
    {
        Warning("Invalid data");
        return;
    }
    else
    {
        No2++;
    }
    Scr=new char[_imagesize(0,0,639,479)];
    Mouse.Off();
    _getimage(0,0,639,479,Scr);
    Mouse.On();
while(No1<No2)
{    
    short j;
    sprintf(name,"%ld.ZND",No1);
    if(!LoadData(name))
    {
        No1++;
        continue;
    }
    Mouse.Off();
    _setcolor(LIGHTGRAY);
    _rectangle(_GFILLINTERIOR,0,0,639,479);
    Border3D(0,0,639,479);

    _setcolor(BLACK);
    sprintf(buff,"编号(NO):%8s",person.No);
    OutText16(5,5,0,buff);
    sprintf(buff,"姓名(NAME):%8s",person.Name);
    OutText16(175,5,0,buff);
    if(person.Sex==Person::Male)
       sprintf(buff,"性别(SEX):男");
    else sprintf(buff,"性别(SEX):女");
    OutText16(380,5,0,buff);
    sprintf(buff,"年龄(AGE):%d",person.Age);
    OutText16(500,5,0,buff);
    sprintf(buff,"身高(H):%d",(int)person.Height);
    OutText16(5,25,0,buff);
    sprintf(buff,"体重(WT):%d",(int)person.Weight);
    OutText16(155,25,0,buff);
    sprintf(buff,"血压(BP):%-3d/%-3d",person.HiBloodPresure,person.LowBloodPresure);
    OutText16(310,25,0,buff);
    sprintf(buff,"心率(HR):%-3d", person.BeatRate);
    OutText16(500,25,0,buff);
    sprintf(buff,"海拔(m):%d",(int)person.HaiBa);
    OutText16(5,45,0,buff);
    sprintf(buff,"电极间距(cm):%3.1f",person.DjInterval);
    OutText16(155,45,0,buff);
    i=0;
    if(Part[HEART])
    {
       LoadData(name,HEART);
       ViewFigure(HEART);
       ViewHeart();
       i++;
    }
    if(Part[LUNG])
    {
        LoadData(name,LUNG);
        ViewFigure(LUNG);
        ViewLung();
        i++;
    }
    if(Part[GAN])
    {
        LoadData(name,GAN);
        ViewFigure(GAN);
        ViewGan();
        i++;
    }
    if(Part[SHEN])
    {
        LoadData(name,SHEN);
        ViewFigure(SHEN);
        ViewShen();
        i++;
    }
    if(Part[LEFTBRAINZ]||Part[RIGHTBRAINE]||
       Part[LEFTBRAINE]||Part[RIGHTBRAINZ])
    {
        if(LoadData(name,LEFTBRAINE))
           ViewFigure(LEFTBRAINE);
        if(LoadData(name,RIGHTBRAINE))
           ViewFigure(RIGHTBRAINE);
        if(LoadData(name,LEFTBRAINZ))
           ViewFigure(LEFTBRAINZ);
        if(LoadData(name,RIGHTBRAINZ))
           ViewFigure(RIGHTBRAINZ);
        ViewBrain();
        i++;
    }
    if(Part[LEFTLEG]||Part[RIGHTLEG]||Part[ARM])
    {
        if(LoadData(name,LEFTLEG))
           ViewFigure(LEFTLEG);
        if(LoadData(name,RIGHTLEG))
           ViewFigure(RIGHTLEG);
        if(LoadData(name,ARM))
           ViewFigure(ARM);
        ViewBody();
        i++;
    }
    if(!i)
    {
        Warning("没有测试数据！");
        No1++;
        continue;
    }

    if(Warning("打印报告吗?")==evOk)
    {
        Mouse.Off();
        sprintf(buff,"print.exe %s>nul",name);
        m=new MESSAGE("正在打印，请稍等....");
        system(buff);
        delete m;
        Mouse.Init();
        Mouse.Off();
    }
    No1++;
}
    BodyPart=12;
    Mouse.Off();
    _putimage(0,0,Scr,_GPSET);
    Mouse.On();
    delete Scr;
}

void View(short count,char *Main,char (*name)[40],double *b,double *c,short *d,short *resort)
{
    char buf[80];
    short start=65;
    short key;
    Mouse.Off();
    _setcolor(BLUE);
    _rectangle(_GFILLINTERIOR,5,65,635,474);
    _setcolor(YELLOW);
    _moveto(315,start);
    _lineto(315,473);
    _moveto(450,start);
    _lineto(450,473);
    _moveto(585,start);
    _lineto(585,473);
    OutText16(5+(310-strlen("项   目")*8)/2,start,0,"项   目");
    OutText16(315+(135-strlen("正常值")*8)/2,start,0,"正常值");
    OutText16(450+(135-strlen("实测值")*8)/2,start,0,"实测值");
    OutText16(585+(50-strlen("分 析")*8)/2,start,0,"分 析");        
    OutText16(5+(310-strlen(Main)*8)/2,start+20,0,Main);
    short i=2;
    short item=0;
    short j;
    while(item<count)
    {
        OutText16(5,start+i*20,0,name[item]);
        if(b[resort[item]])
        {
            sprintf(buf,"%-5.4f",b[resort[item]]+0.00005);
            OutText16(315+(135-strlen(buf)*8)/2,start+i*20,0,buf);
            sprintf(buf,"%-5.4f",c[resort[item]]+0.00005);
            OutText16(450+(135-strlen(buf)*8)/2,start+i*20,0,buf);
           if(d[resort[item]]<-3)d[resort[item]]=-3;
           if(d[resort[item]]>3)d[resort[item]]=3;
           j=0;
           if(d[resort[item]]<0)
           {
            for(j=0;j<-d[resort[item]];j++)
               buf[j]='-';
           }
           if(d[resort[item]]>0)
           {
            for(j=0;j<d[resort[item]];j++)
               buf[j]='+';
           }
           buf[j]=0;
           OutText16(585+(50-strlen(buf)*8)/2,start+i*20,0,buf);
        }
           if(i==19||item==count-1)
           {
            while(1)
            {
                if(_bios_keybrd(1))
                {
                   while(_bios_keybrd(1)) key=_bios_keybrd(0);
                   break;
                }
            }
            if(key==ESC)
            {
                if(Warning("取消查询吗?")==evOk)
                {
                    item=count;
                }
            }
            else i=2;
            _setcolor(BLUE);
            _rectangle(_GFILLINTERIOR,5,85,635,474);
            _setcolor(YELLOW);
            _moveto(315,start);
            _lineto(315,473);
            _moveto(450,start);
            _lineto(450,473);
            _moveto(585,start);
            _lineto(585,473);
        }
        i++;
        item++;
    }
    Mouse.On();
};
    
void ViewHeart()
{
    short count;
    char (*name)[40];
    char *Main;
    double *Bbuf,*Cbuf;
    short *Dbuf;
    count=25;
    name=HeartItem;
    Main=mainitem[0];
    Bbuf=HeartB;
    Cbuf=(double*)Csav[HEART];
    Dbuf=(short*)(Csav[HEART]+29*sizeof(double));
    View(count,Main,name,Bbuf,Cbuf,Dbuf,ReSort[0]);             
}
void ViewLung()
{
    short count;
    char (*name)[40];
    char *Main;
    double *Cbuf;
    double *Bbuf;
    short *Dbuf;     
    count=9;
    name=LungItem;
    Main=mainitem[1];
    Bbuf=LungB;
    Cbuf=(double*)Csav[LUNG];
    Dbuf=(short*)(Csav[LUNG]+10*sizeof(double));
    View(count,Main,name,Bbuf,Cbuf,Dbuf,ReSort[1]);             
}
void ViewGan()
{
    short count;
    char (*name)[40],*Main;
    double *Bbuf,*Cbuf;
    short *Dbuf;
    count=9;
    name=GanItem;
    Main=mainitem[2];
    Bbuf=GanB;
    Cbuf=(double*)Csav[GAN];
    Dbuf=(short*)(Csav[GAN]+10*sizeof(double));
    View(count,Main,name,Bbuf,Cbuf,Dbuf,ReSort[2]);
}

void ViewShen()
{
    short count;
    char (*name)[40],*Main;
    double *Bbuf,*Cbuf;
    short *Dbuf;
    count=6;
    name=ShenItem;
    Main=mainitem[3];
    Bbuf=ShenB;
    Cbuf=(double*)Csav[SHEN];
    Dbuf=(short*)(Csav[SHEN]+7*sizeof(double));
    View(count,Main,name,Bbuf,Cbuf,Dbuf,ReSort[3]);
}
void ViewBrain()
{
    short count;
    char (*name)[40],*Main;
    double *Bbuf,*Cbuf;
    short *Dbuf;
    short j;
    double *c1,*c2,*c3,*c4;
    double *b1,*b2,*b3,*b4;
    short *d1,*d2,*d3,*d4;
    char *CB;
    count=26;
    name=BrainItem;
    Main=mainitem[4];
    Bbuf=new double[26*sizeof(double)];
    CB=new char[26*(sizeof(double)+sizeof(short))];
    Cbuf=(double*)CB;
    Dbuf=(short*)(CB+sizeof(double)*26);
    b1=LeftBrainEB;
    b2=RightBrainEB;
    b3=LeftBrainZB;
    b4=RightBrainZB;
    c1=(double*)Csav[LEFTBRAINE];
    d1=(short *)(Csav[LEFTBRAINE]+8*sizeof(double));
    c2=(double*)Csav[RIGHTBRAINE];
    d2=(short *)(Csav[RIGHTBRAINE]+8*sizeof(double));
    c3=(double*)Csav[LEFTBRAINZ];
    d3=(short *)(Csav[LEFTBRAINZ]+8*sizeof(double));
    c4=(double*)Csav[RIGHTBRAINZ];
    d4=(short *)(Csav[RIGHTBRAINZ]+8*sizeof(double));
    for(j=0;j<26;j++)
    {
        Cbuf[j]=0;
        Dbuf[j]=0;
        Bbuf[j]=0;
    }
    Bbuf[0]=b1[0];
    Bbuf[4]=b1[1];
    Bbuf[8]=b1[2];
    Bbuf[12]=b1[3];
    Bbuf[18]=b1[4];
    Bbuf[22]=b1[5];                                                                    
    Cbuf[0]=c1[0];
    Cbuf[4]=c1[1];
    Cbuf[8]=c1[2];
    Cbuf[12]=c1[3];
    Cbuf[18]=c1[4];
    Cbuf[22]=c1[5];
    Dbuf[0]=d1[0];
    Dbuf[4]=d1[1];
    Dbuf[8]=d1[2];
    Dbuf[12]=d1[3];
    Dbuf[18]=d1[4];                 
    Dbuf[22]=d1[5];
    Bbuf[1]=b2[0];
    Bbuf[5]=b2[1];
    Bbuf[9]=b2[2];
    Bbuf[13]=b2[3];
    Bbuf[16]=b2[6];
    Bbuf[19]=b2[4];
                 Bbuf[23]=b2[5];                                                                                     
                 Cbuf[1]=c2[0];
                 Cbuf[5]=c2[1];
                 Cbuf[9]=c2[2];
                 Cbuf[13]=c2[3];
                 Cbuf[16]=c2[6];
                 Cbuf[19]=c2[4];
                 Cbuf[23]=c2[5];
                 Dbuf[1]=d2[0];
                 Dbuf[5]=d2[1];
                 Dbuf[9]=d2[2];
                 Dbuf[13]=d2[3];
                 Dbuf[16]=d2[6];
                 Dbuf[19]=d2[4];
                 Dbuf[23]=d2[5];
                 Bbuf[2]=b3[0];
                 Bbuf[6]=b3[1];
                 Bbuf[10]=b3[2];
                 Bbuf[14]=b3[3];
                 Bbuf[20]=b3[4];
                 Bbuf[24]=b3[5];                                                                    
                 Cbuf[2]=c3[0];
                 Cbuf[6]=c3[1];
                 Cbuf[10]=c3[2];
                 Cbuf[14]=c3[3];
                 Cbuf[20]=c3[4];
                 Cbuf[24]=c3[5];
                 Dbuf[2]=d3[0];
                 Dbuf[6]=d3[1];
                 Dbuf[10]=d3[2];
                 Dbuf[14]=d3[3];
                 Dbuf[20]=d3[4];                 
                 Dbuf[24]=d3[5];
//             if(Part[RIGHTBRAINZ])
                 Bbuf[3]=b4[0];
                 Bbuf[7]=b4[1];
                 Bbuf[11]=b4[2];
                 Bbuf[15]=b4[3];
                 Bbuf[17]=b4[6];
                 Bbuf[21]=b4[4];
                 Bbuf[25]=b4[5];                                                                                     
                 Cbuf[3]=c4[0];
                 Cbuf[7]=c4[1];
                 Cbuf[11]=c4[2];
                 Cbuf[15]=c4[3];
                 Cbuf[17]=c4[6];
                 Cbuf[21]=c4[4];
                 Cbuf[25]=c4[5];
                 Dbuf[3]=d4[0];
                 Dbuf[7]=d4[1];
                 Dbuf[11]=d4[2];
                 Dbuf[15]=d4[3];
                 Dbuf[17]=d4[6];
                 Dbuf[21]=d4[4];
                 Dbuf[25]=d4[5];
    View(count,Main,name,Bbuf,Cbuf,Dbuf,ReSort[4]);
    delete Bbuf;
    delete CB;             
}

void ViewBody()
{
    short j;
    short count;
    char (*name)[40];
    char *Main;
    char *CB;
    double *Bbuf,*Cbuf;
    short *Dbuf;
    double *b1,*b2,*b3;
    double *c1,*c2,*c3;
    short *d1,*d2,*d3;
    count=4;
    name=BodyItem;
    Main=mainitem[5];
    Bbuf=new double[4];
    CB=new char[(sizeof(double)+sizeof(short))*4];
    Cbuf=(double*)CB;
    Dbuf=(short*)(CB+sizeof(double)*4);
    b1=LegB;
    b2=LegB+1;
    b3=ArmB;
    c1=(double*)Csav[LEFTLEG];
    c2=(double*)Csav[RIGHTLEG];
    c3=(double*)Csav[ARM];
    d1=(short*)(Csav[LEFTLEG]+sizeof(double));
    d2=(short*)(Csav[RIGHTLEG]+sizeof(double));
    d3=(short*)(Csav[ARM]+sizeof(double)*2);
    for(j=0;j<4;j++)
    {
       Cbuf[j]=0;
       Dbuf[j]=0;
       Bbuf[j]=0;
    }
    if(Part[LEFTLEG])
    {
        Bbuf[0]=b1[0];
        Cbuf[0]=c1[0];
        Dbuf[0]=d1[0];
    }
    if(Part[RIGHTLEG])
    {
        Bbuf[1]=b2[0];
        Cbuf[1]=c2[0];
        Dbuf[1]=d2[0];
    }
    if(Part[ARM])
    {
        Bbuf[2]=b3[0];
        Bbuf[3]=b3[1];
        Cbuf[2]=c3[0];
        Cbuf[3]=c3[1];
        Dbuf[2]=d3[0];
        Dbuf[3]=d3[1];
    }
    View(count,Main,name,Bbuf,Cbuf,Dbuf,ReSort[5]);
    delete Bbuf;
    delete CB;
};

void ViewFigure(short p)
{
    register i;
    POINT point;
    char *ScrBuf=new char[_imagesize(20,15,619,434)];
    //  copy data
    float dataUnit=0;
    float heartUnit=0;
    char buf[20];
    float dmax=0,dmin=4098,hmax=0,hmin=4098;
    Mouse.Off();
    _getimage(20,15,619,434,ScrBuf);
  
    _setcolor(BLUE);
    _rectangle(_GFILLINTERIOR,20,15,619,434);
    Border3D(20,15,619,434);
    _setcolor(YELLOW);
    strcpy(buf,BodyString[p]);
    strcat(buf,"血流图");
    OutText16(20+(600-strlen(buf)*8)/2,25,5,buf);
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
    int y1,y2;
    if(BodyPart==HEART)
    {
        dataUnit=dmax-dmin;
        heartUnit=hmax-hmin;
        y1=260-((MDBuffer[0]-dmin)/dataUnit)*200;
        _moveto(left,y1);
        for(i=0;i<569;i+=1)
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
        y1=120+((MDBuffer[0]-dmin)/dataUnit)*200;
        _moveto(left,y1);
        _setcolor(GREEN);
        for(i=0;i<565;i+=5)
        {
            y1=120+((MDBuffer[i+5]-dmin)/dataUnit)*200.;
            _lineto(i+left+5,y1);
        }
    }
    int j=0;
    float time=0;
    _setcolor(GREEN);
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
/*        
   _setcolor(BLUE^YELLOW);
   for(i=0;i<MarkNum;i++)
   {
       moveline(Mark[i]+left,30,350,MarkName[BodyPart][i]);
   }
*/   
   Button *reCount=new Button(1,POINT(80,390),"重新计算",evNextStep,True);
   Button *Print=new Button(2,POINT(260,390),"打印波形",evPrint,True);   
   Button *Cancel=new Button(3,POINT(440,390),"取  消",evCancel,True);
   reCount->Draw();
   Print->Draw();
   Cancel->Draw();
    
   Mouse.On();
   bool over=False;
   while(!over)
    {
         switch(Item::Message)
         {
             case evMouseLDown:
                      Mouse.GetPosition(point);
                      if(reCount->IsInObject(point))
                      {
                          if(reCount->Valid)
                          {
                             reCount->Track();
                             Item::Message=evNothing;
                             if(Mesure()==evNextStep)
                             {
                                 CalculateA();
                                 Calculate();
                             }
                          }
                          Item::Message=evNothing;
                      }
                      if(Print->IsInObject(point))
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
                      else Item::Message=evNothing;
                      break;
             case ESC:         
             case evCancel:         
             case evMouseRDown:
             case ENTER:
                  Item::Message=evNothing;
                  over=True;
                  break;
             default:
                  Item::Message=evNothing;
         }
         GetMessage();
    }
    
    delete Cancel;
    delete Print;
    delete reCount;
    Mouse.Off();
    _putimage(20,15,ScrBuf,_GPSET);
    Mouse.On();
    for(i=0;i<10;i++)
    {
        Mark[i]=0;
    }
    delete ScrBuf;
}             
