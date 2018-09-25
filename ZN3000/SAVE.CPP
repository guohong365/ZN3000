#include <stdlib.h>
#include "hospital.hpp"
#include "editbar.hpp"
#include "printer.hpp"
#include "const.hpp"
#include <fcntl.h>
#include <io.h>
#include <stdio.h>
extern short Mark[];
extern double iInterval;
extern float MDBuffer[];
extern float MHBuffer[];
extern unsigned short BodyPart;
extern double HeartB[29];
extern double LungB[10];
extern double GanB[10];
extern double ShenB[7];
extern double LeftBrainEB[6];
extern double LeftBrainZB[6];
extern double RightBrainEB[7];
extern double RightBrainZB[7];
extern double ArmB[2];
extern double LegB[2];
extern char   *Csav[11];
extern Person person;
extern bool Part[];

void Save(void)
{
    FILE *out;
    char name[13];
    short i;
    char count;
    long offset;
    short size;
    double *Bbuf;
    char *buf;
    char ID[4]="ZND";

    _makepath(name,NULL,NULL,person.No,"ZND");
    
    out=fopen(name,"rb");
    if(out)
    {
        fclose(out);
        out=fopen(name,"rb+");
        if(!out)
        {
           Warning("磁盘写错误,数据没有保存!");
           return;
        }
    }
    else
    {
        out=fopen(name,"wb+");
        if(!out)
        {
           Warning("磁盘写错误,数据没有保存!");
           return;
        }
        buf=new char[30100];
        memset(buf,0,30100);
        rewind(out);        
        fwrite(buf,1,30100,out);
        rewind(out);
        fwrite(ID,1,4,out); //write id        4 bytes
        fwrite(&person,1,sizeof(Person),out);
    }
    fseek(out,4,SEEK_SET);
    fwrite(&person,1,sizeof(Person),out);
    offset=56+BodyPart*4;
    fseek(out,offset,SEEK_SET);
    
    switch(BodyPart)
    {
        case HEART:
             size=29;
             Bbuf=HeartB;
             offset=100;
             break;
        case LUNG:
             size=10;
             offset=5210;                              
             Bbuf=LungB;
             break;
        case GAN:
             size=10;
             Bbuf=GanB;
             offset=7698;                              
             break;
        case SHEN:
             size=7;
             Bbuf=ShenB;
             offset=10186;
             break;
        case LEFTBRAINE:
             size=8;
             Bbuf=LeftBrainEB;
             offset=12674;
             break;
        case RIGHTBRAINE:
             size=8;
             Bbuf=RightBrainEB;
             offset=15162;
             break;
        case LEFTBRAINZ:
             size=8;
             Bbuf=LeftBrainZB;
             offset=17650;
             break;
        case RIGHTBRAINZ:
             size=8;
             Bbuf=RightBrainZB;
             offset=20138;
             break;
        case ARM:
             size=2;
             Bbuf=ArmB;
             offset=22626;             
             break;
        case LEFTLEG:
             size=1;
             Bbuf=LegB;
             offset=25114;             
             break;
        case RIGHTLEG:
             size=1;
             Bbuf=LegB+1;
             offset=27602;
             break;
    }
    fwrite(&offset,1,4,out);
    fseek(out,offset,SEEK_SET);
    fwrite(&iInterval,8,1,out);
    fwrite(Mark,2,10,out);
    fwrite(MDBuffer,4,570,out);    //write wave info
    if(BodyPart==HEART)
       fwrite(MHBuffer,4,570,out);  //write heart wave info
    fwrite(Bbuf,size,sizeof(double),out);
    Bbuf=(double*)Csav[BodyPart];
    fwrite(Csav[BodyPart],size,sizeof(double)+sizeof(short),out);
    fclose(out);
    for(i=0;i<10;i++)
    {
        Mark[i]=0;
    }
}



bool LoadData(char *name,short no)
{
    int i,j;
    long offset;
    char buf[80];
    double *Bbuf=NULL;
    double *tbuf;
    short size,bsize;
    BodyPart=no;
    if(no<0||no>10)
     return False;
    FILE *In=fopen(name,"rb");

    if(!In)
    {
        sprintf(buf,"文件 %s 不存在。",name);
        Warning(buf);
        return False;
    }

    fread(buf,1,4,In);
    if(strcmp(buf,"ZND"))
    {
        sprintf(buf,"文件 %s 不是ZN-2000存储文件。",name);
        Warning(buf);
        return False;
    }

    offset=56+no*4;
    fseek(In,offset,SEEK_SET);
    fread(&offset,1,4,In);
    if(offset)
    {
            Part[no]=True;
            fseek(In,offset,SEEK_SET);
            switch(no)
            {
                case HEART:
                     bsize=size=29;
                     Bbuf=HeartB;
                     break;
                case LUNG:
                     bsize=size=10;
                     Bbuf=LungB;
                     break;
                case GAN:
                     bsize=size=10;
                     Bbuf=GanB;
                     break;
                case SHEN:
                     bsize=size=7;
                     Bbuf=ShenB;
                     break;
                case LEFTBRAINE:
                     size=8;
                     bsize=6;
                     Bbuf=LeftBrainEB;
                     break;
                case RIGHTBRAINE:
                     size=8;
                     bsize=7;
                     Bbuf=RightBrainEB;
                     break;
                case LEFTBRAINZ:
                     size=8;
                     bsize=6;
                     Bbuf=LeftBrainZB;
                     break; 
                case RIGHTBRAINZ:
                     size=8;
                     bsize=7;
                     Bbuf=RightBrainZB;
                     break;
                case ARM:
                     bsize=size=2;
                     Bbuf=ArmB;
                     break;
                case LEFTLEG:
                     bsize=size=1;
                     Bbuf=LegB;
                     break;
                case RIGHTLEG:
                     bsize=size=1;
                     Bbuf=LegB+1;
                     break;
            }
            fread(&iInterval,1,8,In);
            fread(Mark,2,10,In);
            fread(MDBuffer,570,sizeof(float),In);
            if(no==HEART)
            {
                fread(MHBuffer,570,sizeof(float),In);
            }
            tbuf=new double[size];
            fread(tbuf,size,sizeof(double),In);
            for(j=0;j<bsize;j++)
            {
                Bbuf[j]=tbuf[j];
            }
            delete tbuf;
            if(!Csav[no])
                Csav[no]=new char[(sizeof(double)+sizeof(short))*size];
            fread(Csav[no],size,sizeof(double)+sizeof(short),In);
            fclose(In);
            return True;
        }
        if(Part[no])
        {
            Part[no]=False;
            delete Csav[no];
            Csav[no]=NULL;
        }
    return False;
}

bool LoadData(char *name)
{
    int i,j;
    long offset;
    char buf[80];
    double *Bbuf=NULL;
    double *tbuf;
    short size,bsize;
    FILE *In=fopen(name,"rb");

    if(!In)
    {
        sprintf(buf,"文件 %s 不存在。",name);
        Warning(buf);
        return False;
    }

    fread(buf,1,4,In);
    if(strcmp(buf,"ZND"))
    {
        sprintf(buf,"文件 %s 不是ZN-2000存储文件。",name);
        Warning(buf);
        return False;
    }

    fread(&person,1,sizeof(Person),In);

    for(i=0;i<11;i++)
    {
        offset=56+i*4;
        fseek(In,offset,SEEK_SET);
        fread(&offset,1,4,In);
        if(offset)
        {
            Part[i]=True;
            fseek(In,offset,SEEK_SET);
            switch(i)
            {
                case HEART:
                     bsize=size=29;
                     Bbuf=HeartB;
                     break;
                case LUNG:
                     bsize=size=10;
                     Bbuf=LungB;
                     break;
                case GAN:
                     bsize=size=10;
                     Bbuf=GanB;
                     break;
                case SHEN:
                     bsize=size=7;
                     Bbuf=ShenB;
                     break;
                case LEFTBRAINE:
                     size=8;
                     bsize=6;
                     Bbuf=LeftBrainEB;
                     break;
                case RIGHTBRAINE:
                     size=8;
                     bsize=7;
                     Bbuf=RightBrainEB;
                     break;
                case LEFTBRAINZ:
                     size=8;
                     bsize=6;
                     Bbuf=LeftBrainZB;
                     break; 
                case RIGHTBRAINZ:
                     size=8;
                     bsize=7;
                     Bbuf=RightBrainZB;
                     break;
                case LEFTLEG:
                     bsize=size=1;
                     Bbuf=LegB;
                     break;
                case RIGHTLEG:
                     bsize=size=1;
                     Bbuf=LegB+1;
                     break;
                case ARM:
                     bsize=size=2;
                     Bbuf=ArmB;
                     break;
            }
            fread(&iInterval,8,1,In);
            fread(Mark,2,10,In);
            fread(MDBuffer,570,sizeof(float),In);
            if(i==HEART)
            {
                fread(MHBuffer,570,sizeof(float),In);
            }
            tbuf=new double[size];
            fread(tbuf,size,sizeof(double),In);
            for(j=0;j<bsize;j++)
            {
                Bbuf[j]=tbuf[j];
            }
            delete tbuf;
            if(!Csav[i])
                Csav[i]=new char[(sizeof(double)+sizeof(short))*size];
            fread(Csav[i],size,sizeof(double)+sizeof(short),In);
        }
        else
        {
            if(Part[i])
            {
                Part[i]=False;
                delete Csav[i];
                Csav[i]=NULL;
            }
        }
    }
    fclose(In);
    return True;
}
