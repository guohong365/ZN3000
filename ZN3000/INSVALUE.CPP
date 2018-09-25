#include <math.h>
#include <stdlib.h>
#include <string.H>
#include "hospital.hpp"
extern unsigned short Scale;
short MarkNum;
extern double Interval;
extern double iInterval;
extern unsigned short BodyPart;
int Sign(float d)
{
    if(d>0)return 1;
    if(d<0)return -1;
    return 0;
};
short Mark[10];
void Smooth(float *source,unsigned short count);
void VtoO(float *buffer,unsigned short count);
void AutoMark(float *,float *,short,short);
void InsValue(short *source,unsigned short sCount,float *target,unsigned short tCount)
{
    register i;
    unsigned short Count=tCount-1;
    unsigned short scount=sCount-1;
    for(i=0;i<tCount;i++)
    {
        target[i]=source[i*scount/Count]+(source[i*scount/Count+1]-source[i*scount/Count])*
                          ((float)i*(float)scount/(float)Count-i*scount/Count);
    }

//    Smooth(target,tCount);
};

void MarkHeart(float *databuff,float *heartbuff,short count);
void MarkLung(float *databuff,short count,short);
void MarkBrain(float *databuff,short count,short);
void MarkGan(float *databuff,short count,short);
void MarkShen(float *databuff,short count,short);
void MarkBody(float *databuff,short count,short);

void AutoMark(float *databuff,float *heartbuff,short count,short deltaN)
{
    switch(BodyPart)
    {
        case LEFTBRAINE:// heart
        case RIGHTBRAINE:
        case LEFTBRAINZ:
        case RIGHTBRAINZ:
             //MarkBrain(databuff,count,deltaN);
             //MarkNum=4;
             break;
        case HEART://heart
             //MarkHeart(databuff,heartbuff,count);
             //MarkNum=9;
             break;
        case LUNG:
             //MarkLung(databuff,count,deltaN);
             //MarkNum=5;
             break;
        case GAN://gan
             //MarkGan(databuff,count,deltaN);
             //MarkNum=6;
             break;
        case SHEN://shen
             //MarkShen(databuff,count,deltaN);
             //MarkNum=5;
             break;
        case LEFTLEG://body
        case RIGHTLEG:
        case ARM:
             //MarkBody(databuff,count,deltaN);
             //MarkNum=1;
             break;
        default:break;
    }

}
    
void MarkBrain(float *databuff,short count,short deltaN)
{
    float *dData=new float[count/deltaN+1];
//    float *dHeart=new float[count];
    int i,k;
    for(i=0;i<=count/deltaN;i++)
    {
        k=i/deltaN;
        dData[i]=(databuff[(k+1)*deltaN]-databuff[k*deltaN])/deltaN;
//        dHeart[i]=(heartbuff[(k+1)*deltaN]-heartbuff[k*deltaN])/deltaN;        
    }
    dData[count/deltaN+1]=(databuff[count]-databuff[(count/deltaN)*deltaN])/
         (count-(count/deltaN)*count);    
    int sign=0;
    int num=0;
    //mark the character point
    sign=Sign(dData[0]);
    for(i=0;i<count;i++)
    {
        switch(num)
        {
            case 0:
                 if(Sign(dData[i]!=-1)&&sign==-1)
                   if(dData[i]>1.0&&databuff[i]<0.5)
                   {
                     Mark[0]=i;
                     num++;
                   }
                 break;
            case 1:
                if(sign==1&&dData[i]-dData[i+1]>0)
                {
                    Mark[1]=i;
                    num++;
                }
                break;
            case 2:
                 if(sign==1&&Sign(dData[i]!=1)&&databuff[i]>2.5)
                 {
                     Mark[2]=i;
                     num++;
                 }
                 break;
            case 3:
                 if(databuff[i]<1.0&&sign==-1&&Sign(dData[i])!=-1&&dData[i]>1.0)
                 {
                     Mark[3]=i;
                     num++;
                 }
            default: break;
        }
        sign=Sign(dData[i]);
    }
    MarkNum=num;
    delete dData;
//    delete dHeart;
};

float Zero=0;    
void MarkHeart(float *databuff,float *heartbuff,short count)
{
    float *dData=new float[count];
    float *dHeart=new float[count];
    float *dd=new float[count];
    float *hh=new float[count];
    float Hmax=0,Dmax=0;
    float Dmin=50,Hmin=50;
    float dH=0,dD=0;
    float tmp;
    float sumD=0;
    float sumH=0;
    short p=0;
    memcpy(dd,databuff,count*sizeof(float));
    memcpy(hh,heartbuff,count*sizeof(float));    
    Smooth(dd,count);
    Smooth(hh,count);
    int i;
    for(i=0;i<count;i++)
    {
       if(dd[i]<Dmin)Dmin=dd[i];
       if(hh[i]<Hmin)Hmin=hh[i];
       if(dd[i]>Dmax)Dmax=dd[i];
       if(hh[i]>Hmax)Hmax=hh[i];
       sumD+=dd[i];
       sumH+=hh[i];
       if(i>=2)
       {
           dData[i]=(dd[i+1]-dd[i-1])/iInterval;//200;
           dHeart[i]=(hh[i+1]-hh[i-1])/iInterval;//200;
       }
       else
       {
           dData[i]=(dd[i+1]-dd[i])/iInterval;//100;
           dHeart[i]=(hh[i+1]-hh[i])/iInterval;//100;
       }
       if(dHeart[i]>0)
       {
           p++;
           dH+=dHeart[i];
       }

    }
    sumD/=count;
    sumH/=count;
    if(p) dH/=p;
    else dH=0;
    int num=0;
    //mark the character point
    //Q2
    for(i=Mark[0]+(short)(0.6/iInterval);i<count;i++)
    {
      if(dHeart[i]>dH*3/2)
      {
          Mark[9]=i;
          break;
      }
    }
    //R1
    tmp=0;
    for(i=Mark[0]+1;i<0.130/iInterval;i++)
    {
       if(heartbuff[i]>tmp)
       {
           tmp=hh[i];
           Mark[1]=i;
       }
    }
    
    //Z
    tmp=0;
    for(i=Mark[1]+1;i<0.5/iInterval;i++)
    {
        if(databuff[i]>tmp)
        {
            tmp=dd[i];
            Mark[3]=i;
        }
    }
    //B 
    for(i=Mark[1]+1;i<Mark[3];i++)
    {
        if(dData[i]>dData[i-1])
        {
            Mark[2]=i;
            Zero=dd[i];
        }
    }
    if((Mark[2]-Mark[0])*iInterval>0.120)
    {
        Mark[2]=Mark[0]+0.120/iInterval;
    }
    
    tmp=0;
    for(i=Mark[3];i<Mark[9];i++)
    {
        if(dHeart[i-1]<0&&dHeart[i]>0)
        {
            p=i;
            break;
        }
    }

    //C
    for(i=p;i<Mark[0]+0.410/iInterval;i++)
    {
        if(dData[i]<=0&&dData[i-1]>0)
        {
            Mark[4]=i;
            break;
        }
    }
    //X
    tmp=Dmax; 
    for(i=Mark[4];i<Mark[0]+0.5/iInterval;i++)
    {
          if(dd[i]<tmp)
          {
              tmp=dd[i];
              Mark[5]=i;
          }
    }

    if((Mark[5]-Mark[2])*iInterval>0.34)
    {
        Mark[5]=Mark[2]+0.34/iInterval;
    }
    
    //O
    tmp=0;
    for(i=Mark[5];i<Mark[9];i++)
    {
        if(dd[i]>tmp)
        {
            tmp=dd[i];
            Mark[7]=i;
        }
    }

    
    //Y
/*    
    for(i=Mark[5];i<Mark[7];i++)
    {
       if(dData[i]>0&&dData[i-1]>dData[i])
       {
           Mark[6]=i;
           break;
       }
    }
*/    
//    if((Mark[6]-Mark[5])*iInterval-0.02>0.002)
    {
        Mark[6]=(short)(Mark[5]+0.02/iInterval);
    }
    
    MarkNum=9;
    delete dData;
    delete dHeart;
    delete dd;
    delete hh;
};
    
void MarkLung(float *databuff,short count,short deltaN)
{
    float *dData=new float[count];
    int i,k;
    for(i=0;i<=count/deltaN;i++)
    {
        k=i/deltaN;
        dData[i]=(databuff[(k+1)*deltaN]-databuff[k*deltaN])/deltaN;
//        dHeart[i]=(heartbuff[(k+1)*deltaN]-heartbuff[k*deltaN])/deltaN;        
    }
    dData[count/deltaN+1]=(databuff[count]-databuff[(count/deltaN)*deltaN])/
         (count-(count/deltaN)*count);    
    
    int num=0;
    int j;
    float tmp=0;
    //mark the character point
    for(i=1;i<count;i++)
    {
        switch(num)
        {
            case 0://A0
                 if(Sign(dData[i-1]==-1)&&Sign(dData[i])!=-1&&
                   databuff[i]<1.0)
                   {
                     Mark[0]=i;
                     num=2;
                   }
                 break;
            case 2://S1
                if(Sign(dData[i-1])==-1&&Sign(dData[i])!=-1&&
                   databuff[i]<1.0)
                {
                    Mark[2]=i;
                    for(j=Mark[0];j<Mark[2];j++)
                    {
                        if(tmp==Mark[j])
                            Mark[1]=j;
                    }
                    num++;
                }
                break;
            case 3://S0
                 if(Sign(dData[i-1])==1&&Sign(dData[i])!=1&&
                    databuff[i]>2.5)
                 {
                     Mark[3]=i;
                     num++;
                 }
                 break;
            case 4://D0
                 if(databuff[i]<databuff[Mark[3]]&&Sign(dData[i-1])==1&&
                    Sign(dData[i])!=1)
                 {
                     Mark[4]=i;
                     num++;
                 }
                 break;
            case 5://A0
                 if(databuff[i]-databuff[Mark[0]]<=0.5&&
                    Sign(dData[i-1])==-1&&Sign(dData[i])!=-1)
                 {
                      Mark[5]=i;
                      for(j=Mark[0];j<=Mark[5];j++)
                      {
                          if(tmp==databuff[j]&&j<Mark[4]&&
                             j>Mark[2]) Mark[3]=j;
                      }
                 }
                 break;
            default: break;
        }
        if(tmp<databuff[i])tmp=databuff[i];
        
    }
    MarkNum=num;
    delete dData;
};

void MarkGan(float *databuff,short count,short deltaN)
{
    float *dData=new float[count];
    int i,k;
    for(i=0;i<=count/deltaN;i++)
    {
        k=i/deltaN;
        dData[i]=(databuff[(k+1)*deltaN]-databuff[k*deltaN])/deltaN;
//        dHeart[i]=(heartbuff[(k+1)*deltaN]-heartbuff[k*deltaN])/deltaN;        
    }
    dData[count/deltaN+1]=(databuff[count]-databuff[(count/deltaN)*deltaN])/
         (count-(count/deltaN)*count);    
    int num=0;
    int j;
    float tmp=0;
    //mark the character point
    for(i=1;i<count;i++)
    {
        switch(num)
        {
            case 0://A0
                 if(Sign(dData[i-1]==-1)&&
                    Sign(dData[i])!=-1&&databuff[i]<1.0)
                   {
                     Mark[0]=i;
                     num=2;
                   }
                 break;
            case 2://B1
                if(Sign(dData[i-1])==-1&&Sign(dData[i])!=-1&&
                   databuff[i]<1.0)
                {
                    Mark[2]=i;
                    for(j=Mark[0];j<Mark[2];j++) //a
                    {
                        if(tmp==Mark[j])
                            Mark[1]=j;
                    }
                    num++;
                }
                break;
            case 3://B 
                 if(Sign(dData[i-1])==1&&Sign(dData[i])!=1&&
                    databuff[i]>2.5)
                 {
                     Mark[3]=i;
                     num++;
                 }
                 break;
            case 4://C0
                 if(databuff[i]<databuff[Mark[3]]&&Sign(dData[i-1])==-1&&
                    Sign(dData[i])!=-1)
                 {
                     Mark[4]=i;
                     for(j=Mark[2];j<=Mark[4];j++) //b
                     {
                         if(tmp==j)Mark[3]=j;
                     }
                     num++;
                 }
                 break;
            case 5://C
                 if(databuff[i]<databuff[Mark[3]]&&Sign(dData[i-1])==1&&
                    Sign(dData[i])!=1)
                 {
                     Mark[5]=i;
                     num++;
                 }
                 break;
            case 6://A0
                 if(databuff[i]-databuff[Mark[0]]<=0.5&&
                    Sign(dData[i-1])==-1&&Sign(dData[i])!=-1)
                 {
                      Mark[6]=i;
                      for(j=Mark[0];j<=Mark[6];j++)
                      {
                          if(tmp==databuff[j]&&j<Mark[4]&&
                             j>Mark[2]) Mark[3]=j;
                      }
                 }
                 break;
            default: break;
        }
        if(tmp<dData[i])tmp=dData[i];
        
    }
    MarkNum=num;
    delete dData;
};
    
void MarkShen(float *databuff,short count,short deltaN)
{
    float *dData=new float[count];
    int i,k;
    for(i=0;i<=count/deltaN;i++)
    {
        k=i/deltaN;
        dData[i]=(databuff[(k+1)*deltaN]-databuff[k*deltaN])/deltaN;
//        dHeart[i]=(heartbuff[(k+1)*deltaN]-heartbuff[k*deltaN])/deltaN;        
    }
    dData[count/deltaN+1]=(databuff[count]-databuff[(count/deltaN)*deltaN])/
         (count-(count/deltaN)*count);    
    
    int num=0;
    int j;
    float tmp=0;
    //mark the character point
    for(i=0;i<count;i++)
    {
        switch(num)
        {
            case 0://A
                 if(Sign(dData[i-1])==-1&&Sign(dData[i])!=-1&&
                   databuff[i]<0.5)
                   {
                     Mark[0]=i;
                     num++;
                   }
                 break;
            case 1://b
                if(Sign(dData[i-1])==1&&Sign(dData[i])!=1&&
                   databuff[i]>2.5)
                {
                    Mark[1]=i;
                    num++;
                }
                break;
            case 2://c0
                 if(Sign(dData[i-1])==-1&&Sign(dData[i])!=-1&&
                    databuff[i]<databuff[Mark[1]])
                 {
                     Mark[2]=i;
                     num++;
                 }
                 break;
            case 3://c
                 if(databuff[i]<1.0&&Sign(dData[i-1])==-1&&Sign(dData[i])!=-1)
                 {
                     Mark[3]=i;
                     for(j=Mark[0];j<=Mark[3];j++)
                     {
                         if(databuff[j]==tmp&&j>Mark[0]&&j<Mark[2]) Mark[1]=j;
                     }
                     num++;
                 }
            default: break;
        }
        if(tmp<databuff[i])tmp=databuff[i];
    }
    MarkNum=num;
    delete dData;
};
    
void MarkBody(float *databuff,short count,short deltaN)
{
    float *dData=new float[count];
    int i,k;
    for(i=0;i<=count/deltaN;i++)
    {
        k=i/deltaN;
        dData[i]=(databuff[(k+1)*deltaN]-databuff[k*deltaN])/deltaN;
//        dHeart[i]=(heartbuff[(k+1)*deltaN]-heartbuff[k*deltaN])/deltaN;        
    }
    dData[count/deltaN+1]=(databuff[count]-databuff[(count/deltaN)*deltaN])/
         (count-(count/deltaN)*count);    
    int num=0;
    int j;
    float tmp=0;
    //mark the character point
    for(i=0;i<count;i++)
    {
        switch(num)
        {
            case 0://z0
                 if(Sign(dData[i-1])==-1&&Sign(dData[i])!=-1&&
                   databuff[i]<0.5)
                   {
                     Mark[0]=i;
                     num++;
                   }
                 break;
            case 1://dz
                if(Sign(dData[i-1])==1&&Sign(dData[i])!=1&&
                   databuff[i]>2.5)
                {
                    Mark[1]=i;
                    num++;
                }
                break;
            case 2://z0
                 if(Sign(dData[i-1])==-1&&Sign(dData[i])!=-1&&databuff[i]<0.5)
                 {
                     Mark[2]=i;
                     for(j=Mark[0];j<=Mark[2];j++)
                     {
                         if(tmp==databuff[j])Mark[1]=j;
                     }
                     num++;
                 }
                 break;
            default: break;
        }
        if(tmp<databuff[i])tmp=databuff[i];
    }
    MarkNum=num;
    delete dData;
};

void Smooth(float *s,unsigned short count)  //5 spots 
{
    for(register i=4;i<count;i++)
    {
//        s[i]=0.2*(s[i]+s[i-1]+s[i-2]+s[i-3]+s[i-4]);
//        s[i]=0.5*(s[i]+s[i-1]);
        s[i]=0.3333*(s[i]+s[i-1]+s[i-2]);
    }
};

float VtoOLiner[][2]={
    {0.0000,0.000},
    {10.000,2.300},
    {11.109,2.555},
    {12.490,2.874},
    {14.280,3.285},
    {16.660,3.832},
    {18.178,4.181},
    {19.996,4.599},
    {22.210,5.109},
    {24.993,5.748},
    {30.000,6.900},
    {33.320,7.664},
    {39.800,9.196},
    {49.970,11.49}
};
        
void DtoV(float *buffer,unsigned short count)
{
    register i;
    for(i=0;i<count;i++)
    {
        buffer[i]=buffer[i]*10./4095.;
    }
};

    
void VtoO(float *buffer,unsigned short count)
{
    register i,j;

    for(i=0;i<count;i++)
    {
        for(j=0;j<13;j++)
        {
            if(buffer[i]<VtoOLiner[j][1])
            {
                buffer[i]=VtoOLiner[j-1][0]+
                    (buffer[i]-VtoOLiner[j-1][1])*
                    (VtoOLiner[j][0]-VtoOLiner[j-1][0])/
                    (VtoOLiner[j][1]-VtoOLiner[j-1][1]);
            }
        }
        buffer[i]/=1000;
    }
};


            
