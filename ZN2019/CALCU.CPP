#include "stdafx.h"
#include "hospital.hpp"
#include <string.h>
#include <math.h>
double A[10];
double HeartB[29];
double HeartLB[29];
double HeartHB[29];

double LungB[10];
double LungLB[10];
double LungHB[10];

double GanB[10];
double GanLB[10];
double GanHB[10];

double ShenB[7];
double ShenLB[7];
double ShenHB[7];

double LeftBrainEB[8];
double LeftBrainELB[8];
double LeftBrainEHB[8];

double LeftBrainZB[8];
double LeftBrainZLB[8];
double LeftBrainZHB[8];

double RightBrainEB[8];
double RightBrainELB[8];
double RightBrainEHB[8];

double RightBrainZB[8];
double RightBrainZLB[8];
double RightBrainZHB[8];

double ArmB[2];
double ArmLB[2];
double ArmHB[2];

double LegB[2];
double LegLB[2];
double LegHB[2];

double C[84];
double SA,DA;
char   *Csav[11];
double *Ctmp;
short  *Dtmp;
extern short Qp,Rp1,Rp2,Bp,Bp2,Zp,Cp,Dp,Xp,Yp,Op;
double Cs,Xs,Al,Sc;
double HD,HS,HA,L,Oy,ST,DT,H,H1,H2,H3,Hb; 
double Ay,By,Cy,Zy,Z0,HBZTZ,LBZTZ,BZTZ,TA,Alpha,Beta,Y0;
double C68A;
double C69A,C69B;
extern unsigned short BodyPart;
extern double Interval;
extern double iInterval;
extern float HeartAbnormal[][6];
extern float LungAbnormal[][6];
extern float GanAbnormal[][6];
extern float ShenAbnormal[][6];
extern float LeftBrainEAbnormal[][6];
extern float LeftBrainZAbnormal[][6];
extern float RightBrainEAbnormal[][6];
extern float RightBrainZAbnormal[][6];
extern float LegAbnormal[][6];
extern float ArmAbnormal[][6];
extern Person person;
void CompareD();
void CalculateC(void);
void CalculateB(void);
void CalculateA(void);
void Save(void);
void Calculate(void)
{
    CalculateC();
    CompareD();
    Save();
};

void CalculateA(void)
{
    A[0]=person.BeatRate;
    A[3]=person.Sex;
    A[4]=person.Age;
    A[5]=person.Height;
    A[6]=person.Weight;
    A[8]=person.HaiBa;
    DA=person.LowBloodPresure;
    SA=person.HiBloodPresure;
    L=person.DjInterval;
    
    if(A[4]<=14)
    {
        BZTZ=2*A[4]+8;
        LBZTZ=1.8*A[4]+8;
        HBZTZ=2.2*A[4]+8;
    }
    else
    {
        if(A[3]==1)
        {
            BZTZ=A[5]-105;
            LBZTZ=0.9*A[5]-96.5;
            HBZTZ=1.1*A[5]-115.5;
        }
        else
        {
            BZTZ=A[5]-110;
            LBZTZ=0.9*A[5]-99.0;
            HBZTZ=1.1*A[5]-121.0;
        }
    }
    if(BZTZ>HBZTZ)BZTZ=HBZTZ;
    if(BZTZ<LBZTZ)BZTZ=LBZTZ;
};

void CalculateHeartB()
{
    //A10(B1)  heart rate
    HeartB[0]=C[0];

    if(A[4]<=14)
    {
        HeartHB[0]=114.47-2.62*A[4]+26;
        HeartLB[0]=114.47-2.62*A[4]-26;
    }
    else
    {
        HeartHB[0]=100;
        HeartLB[0]=60;
    }
    if(HeartB[0]<HeartLB[0]) HeartB[0]=HeartLB[0];
    if(HeartB[0]>HeartHB[0]) HeartB[0]=HeartHB[0];

    //B2 LVET
    if(A[3]==1) HeartB[1]=0.466-0.00244*HeartB[0];
    else HeartB[1]=0.495-0.00261*HeartB[0];
    HeartLB[1]=0.95*HeartB[1];
    HeartHB[1]=1.05*HeartB[1];

    //B3 PEP
    HeartB[2]=0.289*HeartB[1];
    HeartLB[2]=0.2371*HeartB[1];
    HeartHB[2]=0.3409*HeartB[1];

    //B4
    HeartB[3]=HeartB[1]+HeartB[2];
    HeartLB[3]=HeartLB[1]+HeartLB[2];
    HeartHB[3]=HeartHB[1]+HeartHB[2];

    //B5
    HeartB[4]=0.0061*A[5]+0.0128*A[6]-0.1529;

    //B6 EF
    HeartB[5]=0.71;
    HeartLB[5]=0.68;
    HeartHB[5]=0.74;

    //B7 BV
    if((A[4]<14&&A[3]==0)||A[3]==1)
    {
        HeartB[6]=0.205*A[5]*A[5]*A[5]/1000000+0.06253*A[6]-0.662;
    }
    if(A[4]>=14&&A[3]==0)
    {
        HeartB[6]=0.168*A[5]*A[5]*A[5]/1000000+0.05107*A[6]+0.443;
    }

    //B8 SV
    HeartB[7]=41.07*HeartB[5]*HeartB[6]/HeartB[4];
    if(A[4]<30)
    {
        HeartB[7]=(1+0.0050*(30-A[4]))*HeartB[7];
    }
    else if(A[4]>40)
    {
        HeartB[7]=(1-0.007*(A[4]-40>15?15:A[4]-40))*HeartB[7];
        if(A[4]>55)
          HeartB[7]=(1-0.0085*(A[4]-55))*HeartB[7];
    }
    HeartLB[7]=27.46*HeartB[6]/HeartB[4];
    HeartHB[7]=33.39*HeartB[6]/HeartB[4];

    //B9 CO
    HeartB[8]=HeartB[0]*HeartB[7]/1000;
    HeartLB[8]=HeartB[0]*HeartLB[7]/1000;
    HeartHB[8]=HeartB[0]*HeartHB[7]/1000;

    //B10 SI
    HeartB[9]=HeartB[7]/HeartB[4];
    HeartLB[9]=HeartLB[7]/HeartB[4];
    HeartHB[9]=HeartHB[7]/HeartB[4];

    //B11 CI
    HeartB[10]=HeartB[8]/HeartB[4];
    HeartLB[10]=HeartLB[8]/HeartB[4];
    HeartHB[10]=HeartHB[8]/HeartB[4];

    //B12 CO/Wt
    HeartB[11]=HeartB[8]/A[6];
    HeartLB[11]=HeartLB[8]/A[6];
    HeartHB[11]=HeartHB[8]/A[6];

    //B13 SwI
    HeartB[12]=(SA+2*DA)*HeartB[9]/3000000;
    HeartLB[12]=HeartLB[9]*(SA+2*DA)/3000000;
    HeartHB[12]=HeartHB[9]*(SA+2*DA)/3000000;

    //B14 CwI
    HeartB[13]=HeartB[10]*(SA+2*DA)/3000;
    HeartLB[13]=HeartLB[10]*(SA+2*DA)/3000;
    HeartHB[13]=HeartHB[10]*(SA+2*DA)/3000;

    //B15 TPR
    HeartB[14]=80*(SA+2*DA)/(3*HeartB[8]);
    HeartLB[14]=80*(SA+2*DA)/(3*HeartHB[8]);
    HeartHB[14]=80*(SA+2*DA)/(3*HeartLB[8]);

    //B16 PCWP
    HeartB[15]=188.*HeartB[2]+1.8;
    HeartLB[15]=178.6*HeartB[2]+1.8;
    HeartHB[15]=197.4*HeartB[2]+1.8;

    //B17 DI
    HeartB[16]=HeartB[7]/HeartB[15];
    HeartLB[16]=HeartLB[7]/HeartHB[15];
    HeartHB[16]=HeartHB[7]/HeartLB[15];

    //B18
    HeartB[17]=(225.445-0.94175*HeartB[0])/1000;
    HeartLB[17]=(214.17275-0.89466*HeartB[0])/1000;
    HeartHB[17]=(236.71725-0.98884*HeartB[0])/1000;

    //B19
    HeartB[18]=(131-0.9*HeartB[0])/1000;
    HeartLB[18]=(124.45-0.855*HeartB[0])/1000;
    HeartHB[18]=(137.55-0.945*HeartB[0])/1000;

    //B20 PEP/LVET
//    B[19]=B[2]/B[1];
//    B[19]=0.2371;
//    B[19]=0.3409;
    HeartB[19]=0.0814+0.002595*HeartB[0];
    HeartLB[19]=0.07733+0.002465*HeartB[0];
    HeartHB[19]=0.08547+0.002725*HeartB[0];

    //B21 PEPI
    HeartB[20]=0.44*HeartB[0]+HeartB[2]*1000;
    HeartLB[20]=0.44*HeartB[0]+1000*HeartLB[2];
    HeartHB[20]=0.44*HeartB[0]+1000*HeartHB[2];

    //B22 LVETI
    HeartB[21]=1.42*HeartB[0]+1000*HeartB[1];
    HeartHB[21]=1.05*HeartB[21];
    HeartLB[21]=0.95*HeartB[21];

    //B23 Q-S2I
    HeartB[22]=1.85*HeartB[0]+1000*HeartB[3];
    HeartHB[22]=1.05*HeartB[22];
    HeartLB[22]=0.95*HeartB[22];

    //B24 Q-ZI
    HeartB[23]=0.76*HeartB[0]+HeartB[17];
    HeartHB[23]=0.76*HeartB[0]+HeartHB[17];
    HeartLB[23]=0.76*HeartB[0]+HeartLB[17];

    //B25  
    HeartB[24]=HeartB[20]/HeartB[21];
    HeartHB[24]=HeartHB[20]/HeartLB[21];
    HeartLB[24]=HeartLB[20]/HeartHB[21];

    //B26
    HeartB[25]=DA-HeartB[15];
    HeartHB[25]=DA-HeartLB[15];
    HeartLB[25]=DA-HeartHB[15];

    //B27 
    HeartB[26]=45*HeartB[8];
    HeartHB[26]=50*HeartB[8];
    HeartLB[26]=40*HeartB[8];

    //B28
    HeartB[27]=HeartB[26]/HeartB[0];
    HeartHB[27]=HeartHB[26]/HeartB[0];
    HeartLB[27]=HeartLB[26]/HeartB[0];

    //B29
    HeartB[28]=7.5*HeartB[27]*HeartB[25]/((60/HeartB[0]-HeartB[1])*SA);
    HeartHB[28]=7.5*HeartB[27]*HeartHB[25]/((60/HeartB[0]-HeartB[1])*SA);
    HeartLB[28]=7.5*HeartB[27]*HeartLB[25]/((60/HeartB[0]-HeartB[1])*SA);
};

void CalculateLungB()
{
    double B[10];
    //B5
    B[4]=0.0061*A[5]+0.0128*A[6]-0.1529;
    //B7 BV
    if((A[4]<14&&A[3]==0)||A[3]==1)
    {
        B[6]=0.205*A[5]*A[5]*A[5]/1000000+0.06253*A[6]-0.662;
    }
    if(A[4]>=14&&A[3]==0)
    {
        B[6]=0.168*A[5]*A[5]*A[5]/1000000+0.05107*A[6]+0.443;
    }
    
    //B30
    LungB[0]=0.221-0.0012*C[0];
    LungHB[0]=1.05*LungB[0];
    LungLB[0]=0.95*LungB[0];

    //B31
    LungB[1]=60/C[0]-LungB[0];
    LungHB[1]=60/C[0]-LungLB[0];
    LungLB[1]=60/C[0]-LungHB[0];

    //B32
    LungB[2]= 3.2877*B[6]/C[0];
    LungHB[2]=4.4828*B[6]/C[0];
    LungLB[2]=2.0926*B[6]/C[0];

    //B33
    LungB[3]=2.58*B[6]/C[0];
    LungHB[3]=3.49*B[6]/C[0];
    LungLB[3]=1.58*B[6]/C[0];

    //B34
    LungB[4]=0.53*B[6]/C[0];
    LungHB[4]=1.064*B[6]/C[0];
    LungLB[4]=0.532*B[6]/C[0];

    //B35
    LungB[5]=0.347032*B[6];
    LungHB[5]=1.366*LungB[5];
    LungLB[5]=0.634*LungB[5];

    //B36
    LungB[6]=0.119112*B[6];
    LungHB[6]=1.366*LungB[6];
    LungLB[6]=0.634*LungB[6];

    //B37
    LungB[7]=LungB[4]/LungB[2];
    LungLB[7]=0;
    LungHB[7]=0.02;

    //B38
    LungB[8]=LungB[3]/LungB[2];
    LungLB[8]=0.40;
    LungHB[8]=0.80;

    //B39
    LungB[9]=0.116*B[6]/B[4];
    LungHB[9]=1.25*LungB[9];
    LungLB[9]=0.75*LungB[9];
};

void CalculateGanB()
{
    double B[10];
    //B2 LVET
    B[1]=0.372-0.0011*C[0];
    //B3 PEP
    B[2]=0.289*B[1];
    //B4
    B[3]=B[1]+B[2];
    //B5
    B[4]=0.0061*A[5]+0.0128*A[6]-0.1529;

    //B6 EF
    B[5]=0.71;
    //B7 BV
    if((A[4]<14&&A[3]==0)||A[3]==1)
    {
        B[6]=0.205*A[5]*A[5]*A[5]/1000000+0.06253*A[6]-0.662;
    }
    if(A[4]>=14&&A[3]==0)
    {
        B[6]=0.168*A[5]*A[5]*A[5]/1000000+0.05107*A[6]+0.443;
    }

    //B8 SV
    B[7]=41.07*B[5]*B[6]/B[4];
    if(A[4]<30)
    {
        B[7]=(1+0.0050*(30-A[4]))*B[7];
    }
    else if(A[4]>40)
    {
        B[7]=(1-0.007*(A[4]-40>15?15:A[4]-40))*B[7];
        if(A[4]>55)
          B[7]=(1-0.0085*(A[4]-55))*B[7];
    }

    //B9 CO
    B[8]=C[0]*B[7]/1000;
    
    //B40
    GanB[0]=0.2743*B[8]/C[0];
    GanLB[0]=0;
    GanHB[0]=0.4888*B[8]/C[0];

    //B41
    GanB[1]=1.4678*B[8]/C[0];
    GanLB[1]=0.7908*B[8]/C[0];
    GanHB[1]=2.1448*B[8]/C[0];

    //B42
    GanB[2]=0.8657*B[8]/C[0];
    GanLB[2]=0.4469*B[8]/C[0];
    GanHB[2]=1.2548*B[8]/C[0];

    //B43
    GanB[3]=28.764/C[0];
    GanLB[3]=27.326/C[0];
    GanHB[3]=30.202/C[0];

    //B44
    GanB[4]=31.236/C[0];
    GanLB[4]=29.674/C[0];
    GanHB[4]=32.798/C[0];

    //B45
    if(A[4]<=20)
    {
        GanB[5]=12.246/C[0];
        GanLB[5]=11.6337/C[0];
        GanHB[5]=12.8583/C[0];
    }
    else
    {
        GanB[5]=12.246/C[0]+(A[4]-20)/1750;
        GanLB[5]=11.6337/C[0]+(A[4]-20)/1842.11;
        GanHB[5]=12.8583/C[0]+(A[4]-20)/1666.67;
    }
///*------------------------------------------
    //B46
    GanB[6]=0.20;
//-----------------------------------

    //B47
    GanB[7]=GanB[2]/GanB[1];
    GanLB[7]=0.54-(A[4]-30)/288.60;
    GanHB[7]=0.66-(A[4]-30)/236.13;

    //B48
    GanB[8]=0.365+(A[4]-20)/440;
    GanLB[8]=0.347+(A[4]-20)/463.16;
    GanHB[8]=0.383+(A[4]-20)/419.05;

    //B49
    GanB[9]=220*B[8];
    GanLB[9]=176*B[8];
    GanHB[9]=264*B[8];
}

void CalculateShenB()
{
    double B[20];
 
    B[0]=C[0];
    //B2 LVET
    B[1]=0.372-0.0011*C[0];
    //B3 PEP
    B[2]=0.289*B[1];
    //B4
    B[3]=B[1]+B[2];
    //B5
    B[4]=0.0061*A[5]+0.0128*A[6]-0.1529;

    //B6 EF
    B[5]=0.71;
    //B7 BV
    if((A[4]<14&&A[3]==0)||A[3]==1)
    {
        B[6]=0.205*A[5]*A[5]*A[5]/1000000+0.06253*A[6]-0.662;
    }
    if(A[4]>=14&&A[3]==0)
    {
        B[6]=0.168*A[5]*A[5]*A[5]/1000000+0.05107*A[6]+0.443;
    }

    //B8 SV
    B[7]=41.07*B[5]*B[6]/B[4];
    if(A[4]<30)
    {
        B[7]=(1+0.0050*(30-A[4]))*B[7];
    }
    else if(A[4]>40)
    {
        B[7]=(1-0.007*(A[4]-40>15?15:A[4]-40))*B[7];
        if(A[4]>55)
          B[7]=(1-0.0085*(A[4]-55))*B[7];
    }

    //B9 CO
    B[8]=C[0]*B[7]/1000;
    
    //B50
    ShenB[0]=27/B[0];
    ShenLB[0]=25.65/B[0];
    ShenHB[0]=28.35/B[0];

    //B51
    ShenB[1]=33./B[0];
    ShenLB[1]=31.35/B[0];
    ShenHB[1]=34.65/B[0];

    //B52
    ShenB[2]=11.418/B[0];
    ShenLB[2]=10.847/B[0];
    ShenHB[2]=11.989/B[0];

    //B53
    ShenB[3]=133.11*B[8]/(C[0]*C[0]);
    ShenLB[3]=112.78*B[8]/(C[0]*C[0]);
    ShenHB[3]=143.43*B[8]/(C[0]*C[0]);

    //B54
    ShenB[4]=102.35*B[8]/(C[0]*C[0]);
    ShenLB[4]=90.11*B[8]/(C[0]*C[0]);
    ShenHB[4]=114.60*B[8]/(C[0]*C[0]);

    //B55
    ShenB[5]=ShenB[3]/ShenB[2];
    ShenHB[5]=0.625+0.1658;
    ShenLB[5]=0.625-0.1658;

    //B56
    ShenB[6]=112.5*B[8];
    ShenLB[6]=95.625*B[8];
    ShenHB[6]=129.375*B[8];
};
//--------------------------------------------------------------
void CalculateLeftBrainEB()
{
    double B[10];
    B[0]=C[0];
    //B2 LVET
    B[1]=0.372-0.0011*C[0];
    //B3 PEP
    B[2]=0.289*B[1];
    //B4
    B[3]=B[1]+B[2];
    //B5
    B[4]=0.0061*A[5]+0.0128*A[6]-0.1529;

    //B6 EF
    B[5]=0.71;
    //B7 BV
    if((A[4]<14&&A[3]==0)||A[3]==1)
    {
        B[6]=0.205*A[5]*A[5]*A[5]/1000000+0.06253*A[6]-0.662;
    }
    if(A[4]>=14&&A[3]==0)
    {
        B[6]=0.168*A[5]*A[5]*A[5]/1000000+0.05107*A[6]+0.443;
    }

    //B8 SV
    B[7]=41.07*B[5]*B[6]/B[4];
    if(A[4]<30)
    {
        B[7]=(1+0.0050*(30-A[4]))*B[7];
    }
    else if(A[4]>40)
    {
        B[7]=(1-0.007*(A[4]-40>15?15:A[4]-40))*B[7];
        if(A[4]>55)
          B[7]=(1-0.0085*(A[4]-55))*B[7];
    }

    //B9 CO
    B[8]=C[0]*B[7]/1000;
    
    //B57
    LeftBrainEB[0]=(1-(A[4]-40)*0.01)*1.3*B[8]/B[0];
    LeftBrainELB[0]=0.80*LeftBrainEB[0];
    LeftBrainEHB[0]=1.20*LeftBrainEB[0];

    //B61
    LeftBrainEB[1]=14./B[0];
    LeftBrainELB[1]=12.95/B[0];
    LeftBrainEHB[1]=15.05/B[0];

    //B65
    LeftBrainEB[2]=LeftBrainEB[0]/LeftBrainEB[1];
    LeftBrainELB[2]=0.85*LeftBrainEB[2];
    LeftBrainEHB[2]=1.15*LeftBrainEB[2];

    //B69
    if(A[4]>14) //adult
    {
        if(A[3]==1) //male
        {
            LeftBrainEB[3]=1.157-0.0142*A[4];
            LeftBrainELB[3]=0.88*LeftBrainEB[3];
            LeftBrainEHB[3]=1.12*LeftBrainEB[3];
        }
        else
        {
            LeftBrainEB[3]=1.026-0.0121*A[4];
            LeftBrainELB[3]=0.89*LeftBrainEB[3];
            LeftBrainEHB[3]=1.11*LeftBrainEB[3];
        }
    }
    else //child
    {
            LeftBrainEB[3]=0.938-0.121*A[4];
            LeftBrainELB[3]=0.90*LeftBrainEB[3];
            LeftBrainEHB[3]=1.10*LeftBrainEB[3];
    }

    //B71
    LeftBrainEB[4]=28.08*B[8];
    LeftBrainELB[4]=22.464*B[8];
    LeftBrainEHB[4]=33.696*B[8];

    if(A[3]==1) //male
    {
        //B75
        LeftBrainEB[5]=116.9819*LeftBrainEB[2]-(A[4]-40);
        LeftBrainELB[5]=0.878*LeftBrainEB[5];
        LeftBrainEHB[5]=1.122*LeftBrainEB[5];
    }
    else
    {
        //B75
        LeftBrainEB[5]=133.8061*LeftBrainEB[2]-0.6*(A[4]-40);
        LeftBrainELB[5]=0.905*LeftBrainEB[5];
        LeftBrainEHB[5]=1.095*LeftBrainEB[5];
    }
}
///----------------------------------------------
void CalculateRightBrainEB()
{
    double B[10];
    B[0]=C[0];
    //B2 LVET
    B[1]=0.372-0.0011*C[0];
    //B3 PEP
    B[2]=0.289*B[1];
    //B4
    B[3]=B[1]+B[2];
    //B5
    B[4]=0.0061*A[5]+0.0128*A[6]-0.1529;

    //B6 EF
    B[5]=0.71;
    //B7 BV
    if((A[4]<14&&A[3]==0)||A[3]==1)
    {
        B[6]=0.205*A[5]*A[5]*A[5]/1000000+0.06253*A[6]-0.662;
    }
    if(A[4]>=14&&A[3]==0)
    {
        B[6]=0.168*A[5]*A[5]*A[5]/1000000+0.05107*A[6]+0.443;
    }

    //B8 SV
    B[7]=41.07*B[5]*B[6]/B[4];
    if(A[4]<30)
    {
        B[7]=(1+0.0050*(30-A[4]))*B[7];
    }
    else if(A[4]>40)
    {
        B[7]=(1-0.007*(A[4]-40>15?15:A[4]-40))*B[7];
        if(A[4]>55)
          B[7]=(1-0.0085*(A[4]-55))*B[7];
    }

    //B9 CO
    B[8]=C[0]*B[7]/1000;
    
    //B58
    RightBrainEB[0]=1.0875*LeftBrainEB[0];
    RightBrainELB[0]=0.80*RightBrainEB[0];
    RightBrainEHB[0]=1.20*RightBrainEB[0];
    
    //B62
    RightBrainEB[1]=1.0198*LeftBrainEB[1];
    RightBrainELB[1]=13.21/C[0];
    RightBrainEHB[1]=15.35/C[0];

    //B66
    RightBrainEB[2]=1.0875*LeftBrainEB[2];
    RightBrainELB[2]=0.85*RightBrainEB[2];
    RightBrainEHB[2]=1.15*RightBrainEB[2];

    //B69
    if(A[4]>14) //adult
    {
        if(A[3]==1) //male
        {
            RightBrainEB[3]=1.157-0.0142*A[4];
            RightBrainELB[3]=0.88*RightBrainEB[3];
            RightBrainEHB[3]=1.12*RightBrainEB[3];
        }
        else
        {
            RightBrainEB[3]=1.026-0.0121*A[4];
            RightBrainELB[3]=0.89*RightBrainEB[3];
            RightBrainEHB[3]=1.11*RightBrainEB[3];
        }
    }
    else //child
    {
            RightBrainEB[3]=0.938-0.121*A[4];
            RightBrainELB[3]=0.90*RightBrainEB[3];
            RightBrainEHB[3]=1.10*RightBrainEB[3];
    }
    
    //B72
    RightBrainEB[4]=30.537*B[8];
    RightBrainELB[4]=24.430*B[8];
    RightBrainEHB[4]=36.664*B[8];
    
    if(A[3]==1) //male
    {
        //B76
        RightBrainEB[5]=127.2178*RightBrainEB[2]-(A[4]-40);
        RightBrainELB[5]=0.878*RightBrainEB[5];
        RightBrainEHB[5]=1.122*RightBrainEB[5];
    }
    else
    {
        //B76
        RightBrainEB[5]=145.5141*RightBrainEB[2]-0.6*(A[4]-40);
        RightBrainELB[5]=0.905*RightBrainEB[5];
        RightBrainEHB[5]=1.095*RightBrainEB[5];
        
    }
    
    //B70
    RightBrainEB[6]=0.30;
}
//------------------------------------------------------------
void CalculateLeftBrainZB()
{
    double B[10];
    B[0]=C[0];
    //B2 LVET
    B[1]=0.372-0.0011*C[0];
    //B3 PEP
    B[2]=0.289*B[1];
    //B4
    B[3]=B[1]+B[2];
    //B5
    B[4]=0.0061*A[5]+0.0128*A[6]-0.1529;

    //B6 EF
    B[5]=0.71;
    //B7 BV
    if((A[4]<14&&A[3]==0)||A[3]==1)
    {
        B[6]=0.205*A[5]*A[5]*A[5]/1000000+0.06253*A[6]-0.662;
    }
    if(A[4]>=14&&A[3]==0)
    {
        B[6]=0.168*A[5]*A[5]*A[5]/1000000+0.05107*A[6]+0.443;
    }

    //B8 SV
    B[7]=41.07*B[5]*B[6]/B[4];
    if(A[4]<30)
    {
        B[7]=(1+0.0050*(30-A[4]))*B[7];
    }
    else if(A[4]>40)
    {
        B[7]=(1-0.007*(A[4]-40>15?15:A[4]-40))*B[7];
        if(A[4]>55)
          B[7]=(1-0.0085*(A[4]-55))*B[7];
    }

    //B9 CO
    B[8]=C[0]*B[7]/1000;
    
    //B59
    LeftBrainZB[0]=0.80*LeftBrainEB[0];
    LeftBrainZLB[0]=0.70*LeftBrainEB[0];
    LeftBrainZHB[0]=0.90*LeftBrainEB[0];

    //B63
    LeftBrainZB[1]=14./B[0];
    LeftBrainZLB[1]=12.95/B[0];
    LeftBrainZHB[1]=15.05/B[0];

    //B67
    LeftBrainZB[2]=LeftBrainZB[0]/LeftBrainZB[1];
    LeftBrainZLB[2]=0.85*LeftBrainZB[2];
    LeftBrainZHB[2]=1.15*LeftBrainZB[2];

    //B69
    if(A[4]>14) //adult
    {
        if(A[3]==1) //male
        {
            LeftBrainZB[3]=1.157-0.0142*A[4];
            LeftBrainZLB[3]=0.88*LeftBrainZB[3];
            LeftBrainZHB[3]=1.12*LeftBrainZB[3];
        }
        else
        {
            LeftBrainZB[3]=1.026-0.0121*A[4];
            LeftBrainZLB[3]=0.89*LeftBrainZB[3];
            LeftBrainZHB[3]=1.11*LeftBrainZB[3];
        }
    }
    else //child
    {
            LeftBrainZB[3]=0.938-0.121*A[4];
            LeftBrainZLB[3]=0.90*LeftBrainZB[3];
            LeftBrainZHB[3]=1.10*LeftBrainZB[3];
    }

    //B73
    LeftBrainZB[4]=7.02*B[8];
    LeftBrainZLB[4]=5.616*B[8];
    LeftBrainZHB[4]=8.424*B[8];
    
    if(A[3]==1) //male
    {
        //B77
        LeftBrainZB[5]=0.6097*LeftBrainEB[5];
        LeftBrainZLB[5]=0.878*LeftBrainZB[5];
        LeftBrainZHB[5]=1.122*LeftBrainZB[5];
    }
    else
    {
        //B77
        LeftBrainZB[5]=81.5816*LeftBrainZB[2]-0.23*(A[4]-40);
        LeftBrainZLB[5]=0.905*LeftBrainZB[5];
        LeftBrainZHB[5]=1.095*LeftBrainZB[5];
    }
}

void CalculateRightBrainZB()
{
    double B[10];
    B[0]=C[0];
    //B2 LVET
    B[1]=0.372-0.0011*C[0];
    //B3 PEP
    B[2]=0.289*B[1];
    //B4
    B[3]=B[1]+B[2];
    //B5
    B[4]=0.0061*A[5]+0.0128*A[6]-0.1529;

    //B6 EF
    B[5]=0.71;
    //B7 BV
    if((A[4]<14&&A[3]==0)||A[3]==1)
    {
        B[6]=0.205*A[5]*A[5]*A[5]/1000000+0.06253*A[6]-0.662;
    }
    if(A[4]>=14&&A[3]==0)
    {
        B[6]=0.168*A[5]*A[5]*A[5]/1000000+0.05107*A[6]+0.443;
    }

    //B8 SV
    B[7]=41.07*B[5]*B[6]/B[4];
    if(A[4]<30)
    {
        B[7]=(1+0.0050*(30-A[4]))*B[7];
    }
    else if(A[4]>40)
    {
        B[7]=(1-0.007*(A[4]-40>15?15:A[4]-40))*B[7];
        if(A[4]>55)
          B[7]=(1-0.0085*(A[4]-55))*B[7];
    }

    //B9 CO
    B[8]=C[0]*B[7]/1000;
    
    //B60
    RightBrainZB[0]=1.0875*RightBrainEB[0];
    RightBrainZLB[0]=1.1875*RightBrainZLB[0];
    RightBrainZHB[0]=1.1875*RightBrainZHB[0];

    //B64
    RightBrainZB[1]=RightBrainEB[1];
    RightBrainZLB[1]=RightBrainELB[1];
    RightBrainZHB[1]=RightBrainEHB[1];

    //B68
    RightBrainZB[2]=1.0875*LeftBrainZB[2];
    RightBrainZLB[2]=0.85*RightBrainZB[2];
    RightBrainZHB[2]=1.15*RightBrainZB[2];

    //B69
    if(A[4]>14) //adult
    {
        if(A[3]==1) //male
        {
            RightBrainZB[3]=1.157-0.0142*A[4];
            RightBrainZLB[3]=0.88*RightBrainZB[3];
            RightBrainZHB[3]=1.12*RightBrainZB[3];
        }
        else
        {
            RightBrainZB[3]=1.026-0.0121*A[4];
            RightBrainZLB[3]=0.89*RightBrainZB[3];
            RightBrainZHB[3]=1.11*RightBrainZB[3];
        }
    }
    else //child
    {
            RightBrainZB[3]=0.938-0.121*A[4];
            RightBrainZLB[3]=0.90*RightBrainZB[3];
            RightBrainZHB[3]=1.10*RightBrainZB[3];
    }

    //B74
    RightBrainZB[4]=7.634*B[8];
    RightBrainZLB[4]=6.107*B[8];
    RightBrainZHB[4]=9.161*B[8];

    if(A[3]==1) //male
    {
        //B78
        RightBrainZB[5]=77.5647*LeftBrainEB[2]-0.16*(A[4]-40);
        RightBrainZLB[5]=0.878*RightBrainZB[5];
        RightBrainZHB[5]=1.122*RightBrainZB[5];
    }
    else //famale
    {
        //B78
        RightBrainZB[5]=88.7200*LeftBrainEB[2]-0.23*(A[4]-40);
        RightBrainZLB[5]=0.905*RightBrainZB[5];
        RightBrainZHB[5]=1.095*RightBrainZB[5];
    }

    //B70
    RightBrainZB[6]=0.30;
}
//-----------------------------------------------------------
void CalculateLegB()
{
    double B[10];
    B[0]=C[0];
    //B2 LVET
    B[1]=0.372-0.0011*C[0];
    //B3 PEP
    B[2]=0.289*B[1];
    //B4
    B[3]=B[1]+B[2];
    //B5
    B[4]=0.0061*A[5]+0.0128*A[6]-0.1529;

    //B6 EF
    B[5]=0.71;
    //B7 BV
    if((A[4]<14&&A[3]==0)||A[3]==1)
    {
        B[6]=0.205*A[5]*A[5]*A[5]/1000000+0.06253*A[6]-0.662;
    }
    if(A[4]>=14&&A[3]==0)
    {
        B[6]=0.168*A[5]*A[5]*A[5]/1000000+0.05107*A[6]+0.443;
    }

    //B8 SV
    B[7]=41.07*B[5]*B[6]/B[4];
    if(A[4]<30)
    {
        B[7]=(1+0.0050*(30-A[4]))*B[7];
    }
    else if(A[4]>40)
    {
        B[7]=(1-0.007*(A[4]-40>15?15:A[4]-40))*B[7];
        if(A[4]>55)
          B[7]=(1-0.0085*(A[4]-55))*B[7];
    }

    //B9 CO
    B[8]=C[0]*B[7]/1000;
    
    if(A[3]==1)
    {
        //B79
        LegB[0]=0.5302*B[8];
        LegLB[0]=0.3008*B[8];
        LegHB[0]=0.7596*B[8];
        //B80
        LegB[1]=0.5509*B[8];
        LegLB[1]=0.3113*B[8];
        LegHB[1]=0.7904*B[8];
    }
    else
    {
        //B81
        LegB[0]=0.5037*B[8];
        LegLB[0]=0.2858*B[8];
        LegHB[0]=0.7216*B[8];
        //B82
        LegB[1]=0.5280*B[8];
        LegLB[1]=0.2984*B[8];
        LegHB[1]=0.7576*B[8];
    }
}

void CalculateArmB()
{
    double B[10];
    B[0]=C[0];
    //B2 LVET
    B[1]=0.372-0.0011*C[0];
    //B3 PEP
    B[2]=0.289*B[1];
    //B4
    B[3]=B[1]+B[2];
    //B5
    B[4]=0.0061*A[5]+0.0128*BZTZ-0.1529;

    //B6 EF
    B[5]=0.71;
    //B7 BV
    if((A[4]<14&&A[3]==0)||A[3]==1)
    {
        B[6]=0.205*A[5]*A[5]*A[5]/1000000+0.06253*A[6]-0.662;
    }
    if(A[4]>=14&&A[3]==0)
    {
        B[6]=0.168*A[5]*A[5]*A[5]/1000000+0.05107*A[6]+0.443;
    }

    //B8 SV
    B[7]=41.07*B[5]*B[6]/B[4];
    if(A[4]<30)
    {
        B[7]=(1+0.0050*(30-A[4]))*B[7];
    }
    else if(A[4]>40)
    {
        B[7]=(1-0.007*(A[4]-40>15?15:A[4]-40))*B[7];
        if(A[4]>55)
          B[7]=(1-0.0085*(A[4]-55))*B[7];
    }

    //B9 CO
    B[8]=C[0]*B[7]/1000;

    //B84
    ArmB[0]=0.4028*B[8];
    ArmLB[0]=0.3677*B[8];
    ArmHB[0]=0.4387*B[8];

    //83
    if(person.Age<40)
    {
       ArmB[1]=2.5266; 
       ArmLB[1]=1.4896;
       ArmHB[1]=3.5636;
    }
    else
    {
        ArmB[1]=3.8069;
        ArmLB[1]=3.5855;
        ArmHB[1]=4.0283;
    }
};

void CalculateC(void)
{
    short i=0;
    switch(BodyPart)
    {
        case HEART:
             person.BeatRate=C[0]=(int)(60./((Rp2-Rp1)*iInterval)+0.5);
             CalculateHeartB();
             C[1]=(Xp-Bp)*iInterval;
             C[2]=(Bp-Qp)*iInterval;
             C[3]=C[1]+C[2];
             C[4]=0.0061*A[5]+0.0128*A[6]-0.1529;
             C[5]=Cs;
             C[6]=Xs;
             C[7]=(135+0.005*A[8])*L*L*Sc;
             C[8]=C[0]*C[7]/1000;
             C[9]=C[7]/C[4];
             C[10]=C[8]/C[4];
             C[11]=C[8]/A[6];
             C[12]=(SA+2*DA)*C[9]/3000000;
             C[13]=(SA+2*DA)*C[10]/3000;
             C[14]=26.6667*(SA+2*DA)/C[8];
             C[15]=18.8*C[2]/((Op-Xp)*iInterval)+1.8;
             C[16]=C[7]/C[15];
             C[17]=(Zp-Qp)*iInterval;
             C[18]=(Bp-Rp1)*iInterval;
             C[19]=C[2]/C[1];
             C[20]=0.44*C[0]+1000*C[2];
             C[21]=1.42*C[0]+1000*C[1];
             C[22]=1.85*C[0]+1000*C[3];
             C[23]=0.76*C[0]+C[17];
             C[24]=C[20]/C[21];
             C[25]=DA-C[15];
             
             if(C[6]/C[5]>0.4&&Oy/Zy>0.25)
                C[26]=0.045*C[0]*C[7]-292*(C[6]/C[5]+Oy/Zy-0.355);                          
             else
             {
                 if(C[6]/C[5]>0.4)
                    C[26]=0.045*C[0]*C[7]-327*(C[6]/C[5]-0.4);
                 else
                 {
                     if(Oy/Zy>0.25)
                        C[26]=0.045*C[0]*C[7]-212*(Oy/Zy-0.25);
                     else
                        C[26]=0.045*C[0]*C[7];
                 }
             }

             C[27]=C[26]/C[0];
             C[28]=7.5*C[25]*C[27]/((60/C[0]-C[1])*SA);
             Ctmp=new double[29];
             Dtmp=new short[29];
             Csav[BodyPart]=new char[(sizeof(double)+sizeof(short))*29];
             for(i=0;i<29;i++)
             {
                 Ctmp[i]=C[i];
             }
             memcpy(Csav[BodyPart],Ctmp,sizeof(double)*29);
             break;
        case LUNG:
             CalculateLungB();
             C[29]=ST;
             C[30]=DT;
             C[31]=HS*25;
             C[32]=HD*25;
             C[33]=HA*25;
             C[34]=C[31]/C[29];
             C[35]=C[31]/C[30];
             C[36]=HA/HS;
             C[37]=HD/HS;
             C[38]=0.0061*A[5]+0.0128*A[6]-0.1529;
             C[38]=2.116*C[31]/(C[38]*(C[29]+C[30]));
             Ctmp=new double[10];
             Dtmp=new short[10];
             Csav[BodyPart]=new char[(sizeof(double)+sizeof(short))*10];
             for(i=29;i<39;i++)
                Ctmp[i-29]=C[i];
             memcpy(Csav[BodyPart],Ctmp,sizeof(double)*10);
             break;   
        case GAN:
             CalculateGanB();
             C[39]=H2*25;
             C[40]=H*25;
             C[41]=H3*25;
             C[42]=Alpha;
             C[43]=Beta;
             C[44]=Al;
             C[45]=C[39]/C[40];
             C[46]=C[41]/C[40];
             C[47]=H1/C[40];
             if(A[6]>HBZTZ||A[6]<LBZTZ)
                C[48]=9404*C[40]*A[6]*A[6]/((C[42]+C[43])*BZTZ*BZTZ);
             else
               C[48]=9404*C[40]/(C[42]+C[43]);
             Ctmp=new double[10];
             Dtmp=new short[10];
             Csav[BodyPart]=new char[(sizeof(double)+sizeof(short))*10];
             for(i=39;i<49;i++)
                Ctmp[i-39]=C[i];
             memcpy(Csav[BodyPart],Ctmp,sizeof(double)*10);   
             break;  
        case SHEN:
             CalculateShenB();
             C[49]=Alpha;
             C[50]=Beta;
             C[51]=Al;
             C[52]=H*25;
             C[53]=H3*25;
             C[54]=C[53]/C[52];
             C[55]=3.9656*C[52]/(C[49]+C[50]);
             Ctmp=new double[7];
             Dtmp=new short[7];
             Csav[BodyPart]=new char[(sizeof(double)+sizeof(short))*7];
             for(i=49;i<56;i++)
                Ctmp[i-49]=C[i];
             memcpy(Csav[BodyPart],Ctmp,sizeof(double)*7);   
             break;
        case LEFTBRAINE:
             CalculateLeftBrainEB();  
               Ctmp=new double[8];
               Dtmp=new short[8];
               Ctmp[0]=C[56]=HS*50;
               Ctmp[1]=C[60]=TA;
               Ctmp[2]=C[64]=C[56]/C[60];
               Ctmp[3]=C[68]=Hb/HS;
               Ctmp[4]=C[70]=21.6*C[56]*C[0];
               if(A[3]==1)
               {
                 Ctmp[5]=C[74]=116.9819*C[64];
               }
               else
               {
                 Ctmp[5]=C[74]=133.8061*C[64];
               }
               Csav[BodyPart]=new char[(sizeof(double)+sizeof(short))*8];
               memcpy(Csav[BodyPart],Ctmp,sizeof(double)*8);
               break;
        case LEFTBRAINZ:
             CalculateLeftBrainZB();
               Ctmp=new double[8];
               Dtmp=new short[8];
               Ctmp[0]=C[58]=HS*50;
               Ctmp[1]=C[62]=TA;
               Ctmp[2]=C[66]=C[58]/C[62];
               Ctmp[3]=C[68]=Hb/HS;
               Ctmp[4]=C[72]=5.4*C[58]*C[0];
               if(A[3]==1)
               {
                 Ctmp[5]=C[76]=71.3239*C[66];
               }
               else
               {
                 Ctmp[5]=C[76]=81.5816*C[66];
               }
               Csav[BodyPart]=new char[(sizeof(double)+sizeof(short))*8];
               memcpy(Csav[BodyPart],Ctmp,sizeof(double)*8);
               break;  
        case RIGHTBRAINE:
             CalculateRightBrainEB();
               Ctmp=(double*)Csav[LEFTBRAINE];
               C[56]=Ctmp[0]; 
               Ctmp=new double[8];
               Dtmp=new short[8];
               Ctmp[0]=C[57]=HS*50;
               Ctmp[1]=C[61]=TA;
               Ctmp[2]=C[65]=C[57]/C[61];
               Ctmp[7]=C68A=Hb;
               Ctmp[3]=C[68]=Hb/HS;
               Ctmp[6]=C69A=fabs((C[56]-C[57]))/C[56];
               Ctmp[4]=C[71]=23.49*C[57]*C[0];
               if(A[3]==1)
               {
                 Ctmp[5]=C[75]=127.2178*C[65];
               }
               else
               {
                 Ctmp[5]=C[75]=145.5141*C[65];
               }
               Csav[BodyPart]=new char[(sizeof(double)+sizeof(short))*8];
               memcpy(Csav[BodyPart],Ctmp,sizeof(double)*8);
               break;
        case RIGHTBRAINZ:
             CalculateRightBrainZB();
               Ctmp=(double*)(Csav[LEFTBRAINZ]);
               C[58]=Ctmp[0];
               Ctmp=new double[8];
               Dtmp=new short[8];
               Ctmp[0]=C[59]=HS*50;
               Ctmp[1]=C[63]=TA;
               Ctmp[2]=C[67]=C[59]/C[63];
               Ctmp[3]=C[68]=Hb/HS;
               Ctmp[6]=C69B=fabs((C[58]-C[59]))/C[58];
               Ctmp[4]=C[73]=5.873*C[59]*C[0];                 
               if(A[3]==1)
               {
                 Ctmp[5]=C[77]=77.5647*C[67];
               }
               else
               {
                 Ctmp[5]=C[77]=88.72*C[67];
               }
               Csav[BodyPart]=new char[(sizeof(double)+sizeof(short))*8];
               memcpy(Csav[BodyPart],Ctmp,sizeof(double)*8);
               break;
               
        case ARM:
             CalculateArmB();
             Ctmp=(double*)Csav[LUNG];
             C[31]=Ctmp[2];
             Ctmp=new double[2];
             Dtmp=new short[2];
             Ctmp[0]=C[80]=42*C[0]*Zy/Z0;
             Ctmp[1]=C[82]=C[31]/Zy/25;
             Csav[BodyPart]=new char[(sizeof(double)+sizeof(short))*2];
             memcpy(Csav[BodyPart],Ctmp,sizeof(double)*2);
             break;        
        case LEFTLEG:
             CalculateLegB();
             Ctmp=new double[1];
             Dtmp=new short[1];
             if(A[3]==1)
             {
                 Ctmp[0]=C[78]=58*C[0]*Zy/Z0;
             }
             else
             {
                 Ctmp[0]=C[80]=52*C[0]*Zy/Z0;
             }
             Csav[BodyPart]=new char[(sizeof(double)+sizeof(short))];
             memcpy(Csav[BodyPart],Ctmp,sizeof(double));
             break;
        case RIGHTLEG:
             CalculateLegB();
             Ctmp=new double[1];
             Dtmp=new short[1];
             if(A[3]==1)
             {
                 Ctmp[0]=C[79]=58*C[0]*Zy/Z0;
             }
             else
             {
                 Ctmp[0]=C[81]=52*C[0]*Zy/Z0;
             }
             Csav[BodyPart]=new char[(sizeof(double)+sizeof(short))];
             memcpy(Csav[BodyPart],Ctmp,sizeof(double));
             break;
    }
    delete Ctmp;
    Ctmp=NULL;
};

void CompareD(void)
{
    short i,j;
    double tmp;
    double *dp;
    double *test;
    dp=(double*)Csav[BodyPart];    
    switch(BodyPart)
    {
       case HEART:
            for(i=0;i<29;i++)
            {
                if(i==18)
                {
                    Dtmp[i]=0;
                    if(dp[i]>0.38)Dtmp[i]++;
                    if(dp[i]>0.40)Dtmp[i]++;
                    if(dp[i]>0.42)Dtmp[i]++;
                    continue;
                }
                if(dp[i]<HeartLB[i])
                    tmp=(dp[i]-HeartLB[i])/HeartLB[i];
                else if(dp[i]>HeartHB[i])
                    tmp=(dp[i]-HeartHB[i])/HeartHB[i];
                else tmp=0;    
                Dtmp[i]=-3;
                for(j=0;j<6;j++)
                {
                    if(HeartAbnormal[i][j]==0) Dtmp[i]=0;
                    else if(HeartAbnormal[i][j]<tmp)Dtmp[i]++;
                    else break;
                }
            }
            memcpy(Csav[BodyPart]+sizeof(double)*29,Dtmp,sizeof(short)*29);
            break;        
       case LUNG:
            for(i=0;i<10;i++)
            {
                if(i==7||i==8)
                {
                    if(dp[i]>LungAbnormal[i][5])
                    {
                        Dtmp[i]=3;
                    }
                    else if(dp[i]>LungAbnormal[i][4])
                    {
                        Dtmp[i]=2;
                    }
                    else if(dp[i]>LungAbnormal[i][3])
                    {
                        Dtmp[i]=1;
                    }
                    else Dtmp[i]=0;
                    continue;
                }
                if(dp[i]<LungLB[i])
                    tmp=(dp[i]-LungLB[i])/LungLB[i];
                else if(dp[i]>LungHB[i])
                    tmp=(dp[i]-LungHB[i])/LungHB[i];
                else tmp=0;    
                Dtmp[i]=-3;
                for(j=0;j<6;j++)
                {
                    if(LungAbnormal[i][j]==0) Dtmp[i]=0;
                    else if(LungAbnormal[i][j]<tmp)Dtmp[i]++;
                    else break;
                }
            }
            memcpy(Csav[BodyPart]+sizeof(double)*10,Dtmp,sizeof(short)*10);            
            break;        
       case GAN:
            for(i=0;i<10;i++)
            {
                if(i==6||i==7)
                {
                    if(dp[i]>GanAbnormal[i][5])
                    {
                        Dtmp[i]=3;
                    }
                    else if(dp[i]>GanAbnormal[i][4])
                    {
                        Dtmp[i]=2;
                    }
                    else if(dp[i]>GanAbnormal[i][3])
                    {
                        Dtmp[i]=1;
                    }
                    else Dtmp[i]=0;
                    continue;
                }
                
                if(dp[i]<GanLB[i])
                    tmp=(dp[i]-GanLB[i])/GanLB[i];
                else if(dp[i]>GanHB[i])
                    tmp=(dp[i]-GanHB[i])/GanHB[i];
                else tmp=0;
                Dtmp[i]=-3;
                for(j=0;j<6;j++)
                {
                    if(GanAbnormal[i][j]==0) Dtmp[i]=0;
                    else if(GanAbnormal[i][j]<tmp)Dtmp[i]++;
                    else break;
                }
            }
            memcpy(Csav[BodyPart]+sizeof(double)*10,Dtmp,sizeof(short)*10);                        
            break;        
       
       case SHEN:
            for(i=0;i<7;i++)
            {
                if(i==5)
                {
                    if(dp[i]>ShenAbnormal[i][5])
                    {
                        Dtmp[i]=3;
                    }
                    else if(dp[i]>ShenAbnormal[i][4])
                    {
                        Dtmp[i]=2;
                    }
                    else if(dp[i]>ShenAbnormal[i][3])
                    {
                        Dtmp[i]=1;
                    }
                    else Dtmp[i]=0;
                    continue;
                }
                
                if(dp[i]<ShenLB[i])
                    tmp=(dp[i]-ShenLB[i])/ShenLB[i];
                else if(dp[i]>ShenHB[i])
                    tmp=(dp[i]-ShenHB[i])/ShenHB[i];
                else tmp=0;    
                Dtmp[i]=-3;
                for(j=0;j<6;j++)
                {
                    if(ShenAbnormal[i][j]==0) Dtmp[i]=0;
                    else if(ShenAbnormal[i][j]<tmp)Dtmp[i]++;
                    else break;
                }
            }
            memcpy(Csav[BodyPart]+sizeof(double)*7,Dtmp,sizeof(short)*7);                         
            break;        
       
       case LEFTBRAINE:
            for(i=0;i<5;i++)
            {
                if(dp[i]<LeftBrainELB[i])
                   tmp=(dp[i]-LeftBrainELB[i])/LeftBrainELB[i];
                else if(dp[i]>LeftBrainEHB[i])
                   tmp=(dp[i]-LeftBrainEHB[i])/LeftBrainEHB[i];
                else tmp=0;   
                Dtmp[i]=-3;
                for(j=0;j<6;j++)
                {
                    if(LeftBrainEAbnormal[i][j]==0) Dtmp[i]=0;
                    else if(LeftBrainEAbnormal[i][j]<=tmp)Dtmp[i]++;
                    else break;
                }
            }
            test=(double*)Csav[BodyPart];
            memcpy(Csav[BodyPart]+sizeof(double)*8,Dtmp,sizeof(short)*8);                        
            break;        
       
       case RIGHTBRAINE:
            if(dp[6]>0.50) Dtmp[6]=3;
            else if(dp[6]>0.40)Dtmp[6]=2;
            else if(dp[6]>0.30)Dtmp[6]=1;
            else Dtmp[6]=0;
            for(i=0;i<6;i++)
            {
                if(dp[i]<RightBrainELB[i])
                    tmp=(dp[i]-RightBrainELB[i])/RightBrainELB[i];
                else if(dp[i]>RightBrainEHB[i])
                    tmp=(dp[i]-RightBrainEHB[i])/RightBrainEHB[i];
                else tmp=0;    
                Dtmp[i]=-3;
                for(j=0;j<6;j++)
                {
                    if(RightBrainEAbnormal[i][j]==0) Dtmp[i]=0;
                    else if(RightBrainEAbnormal[i][j]<tmp)Dtmp[i]++;
                    else break;
                }
            }
            memcpy(Csav[BodyPart]+sizeof(double)*8,Dtmp,sizeof(short)*8);                        
            break;        
       case LEFTBRAINZ:
            for(i=0;i<5;i++)
            {
                if(dp[i]<LeftBrainZLB[i])
                    tmp=(dp[i]-LeftBrainZLB[i])/LeftBrainZLB[i];
                else if(dp[i]>LeftBrainZHB[i])
                    tmp=(dp[i]-LeftBrainZHB[i])/LeftBrainZHB[i];
                else tmp=0;    
                Dtmp[i]=-3;
                for(j=0;j<6;j++)
                {
                    if(LeftBrainZAbnormal[i][j]==0) Dtmp[i]=0;
                    else if(LeftBrainZAbnormal[i][j]<tmp)Dtmp[i]++;
                    else break;
                }
            }
            memcpy(Csav[BodyPart]+sizeof(double)*8,Dtmp,sizeof(short)*8);                        
            break;        
       
       case RIGHTBRAINZ:
            if(dp[6]>0.50) Dtmp[6]=3;
            else if(dp[6]>0.40)Dtmp[6]=2;
            else if(dp[6]>0.30)Dtmp[6]=1;
            else Dtmp[6]=0;
            for(i=0;i<6;i++)
            {
                if(dp[i]<RightBrainZLB[i])
                    tmp=(dp[i]-RightBrainZLB[i])/RightBrainZLB[i];
                else if(dp[i]>RightBrainZHB[i])
                    tmp=(dp[i]-RightBrainZHB[i])/RightBrainZHB[i];
                else tmp=0;    
                Dtmp[i]=-3;
                for(j=0;j<6;j++)
                {
                    if(RightBrainZAbnormal[i][j]==0) Dtmp[i]=0;
                    else if(RightBrainZAbnormal[i][j]<tmp)Dtmp[i]++;
                    else break;
                }
            }
            memcpy(Csav[BodyPart]+sizeof(double)*8,Dtmp,sizeof(short)*8);                        
            break;        
       
       case ARM:
            if(dp[0]<ArmLB[0])
                    tmp=(dp[0]-ArmLB[0])/ArmLB[0];
                else if(dp[i]>ArmHB[0])
                    tmp=(dp[0]-ArmHB[0])/ArmHB[0];
                else tmp=0;    
                Dtmp[0]=-3;
                for(j=0;j<6;j++)
                {
                    if(ArmAbnormal[0][j]==0) Dtmp[0]=0;
                    else if(ArmAbnormal[0][j]<=tmp)Dtmp[0]++;
                    else break;
                }
            if(A[4]<40)
            {
                if(dp[1]<1.4896)Dtmp[1]=-1;
                else if(dp[1]>3.5636)Dtmp[1]=1;
                else Dtmp=0;
            }
            else
            {
                if(dp[1]<3.5855)Dtmp[1]=-1;
                else if(dp[1]>4.0283)Dtmp[1]=1;
                else Dtmp=0;
            }
            memcpy(Csav[BodyPart]+sizeof(double)*2,Dtmp,sizeof(short)*2);                                  
            break;        
       case LEFTLEG:
            if(dp[0]<LegLB[0])
               tmp=(dp[0]-LegLB[0])/LegLB[0];
            else if(dp[0]>LegHB[0])
               tmp=(dp[0]-LegHB[0])/LegHB[0];
            else tmp=0;   
            Dtmp[0]=-3;
            for(j=0;j<6;j++)
            {
                if(LegAbnormal[(int)A[3]][j]==0) Dtmp[0]=0;
                else if(LegAbnormal[(int)A[3]][j]<tmp)Dtmp[0]++;
                else break;
            }
            memcpy(Csav[BodyPart]+sizeof(double),Dtmp,sizeof(short));
            break;        
       case RIGHTLEG:
            if(dp[0]<LegLB[1])
               tmp=(dp[0]-LegLB[1])/LegLB[1];
            else if(dp[0]>LegHB[1])
               tmp=(dp[0]-LegHB[1])/LegHB[1];
            else tmp=0;   
            Dtmp[0]=-3;
            for(j=0;j<6;j++)
            {
                if(LegAbnormal[(int)A[3]][j]==0) Dtmp[0]=0;
                else if(LegAbnormal[(int)A[3]][j]<tmp)Dtmp[0]++;
                else break;
            }
            memcpy(Csav[BodyPart]+sizeof(double),Dtmp,sizeof(short));
            break;        
    }
    delete Dtmp;
    Dtmp=NULL;
};
/*
short reSort[][29]=
{
    {7,9,8,10,12,13,11,1,14,21,15,19,16,24,2,18,20,25,3,27,22,26,17,28,23},
    {4,0,2, 1, 3, 5, 7,6, 8, 9, 0, 0, 0, 0,0, 0, 0, 0,0, 0, 0, 0, 0, 0, 0},
    {0,6,1, 7, 2, 5, 3,8, 4, 0, 0, 0, 0, 0,0, 0, 0, 0,0, 0, 0, 0, 0, 0, 0},
    {3,1,4, 5, 2, 0, 0,0, 0, 0, 0, 0, 0, 0,0, 0, 0, 0,0, 0, 0, 0, 0, 0, 0}
};
    
*/
