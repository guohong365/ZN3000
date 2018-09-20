#include "general.hpp"
#include "hospital.hpp"
#include "editbar.hpp"
#include "printer.hpp"
extern Printer PRN;
extern short FirstX;
extern short SecondX;
extern short *DataBuffer;
extern short *HeartBuffer;
extern unsigned long DataPointer;
extern double A[];
extern double B[];
extern double LB[];
extern double HB[];
extern double C[];
extern double D[];
/*
double SA,DA;
//extern double D[84][6];
double HD,HS,HA,X,Q,S,L,O,R,ST,DT,H,H1,H2,H3,Y,Hb; 
double A0,B0,C0,SC,Z,Z0,BZTZ,TA,Alpha,Beta,Y0;
double C68A,C68B,C68C,C68D,C68E,C68F,C68G,C68H;
double C69A,C69B;
*/
void PrePrint(FILE *fp)
{
    register i;
    short *buff1=new short[SecondX-FirstX+1];
    short *buff2=new short[SecondX-FirstX+1];    
    unsigned short dd=SecondX-FirstX+1;
    for(i=0;i<SecondX-FirstX+1;i++)
    {
        buff1[i]=DataBuffer[(DataPointer-SecondX+i)%TOTAL_DATA];
        buff2[i]=HeartBuffer[(DataPointer-SecondX+i)%TOTAL_DATA];        
    }
    fwrite(&dd,1,sizeof(short),fp);
    fwrite(buff1,dd,sizeof(int),fp);
    fwrite(buff2,dd,sizeof(int),fp);
    fclose(fp);
    delete buff1;
    delete buff2;  
}
