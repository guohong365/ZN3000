#include <stdio.h>
#include <stdlib.h>
#include <conio.h>
extern int Mark1,Mark2,Mark3;
void InsValue(int *source,unsigned short sCount,int *target,unsigned short tCount,unsigned short deltaN);
void main(void)
{
    int source[]={1,3,4,2,1,3,2,4,4,3,2,1,3,2};
    int target[]=  {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0};

    InsValue(source,14,target,17,2);
    for(int i=0;i<17;i++) printf("%d ",target[i]);
    printf("\n------------------------------\n");
    printf("%d %d %d\n",Mark1+1,Mark2+1,Mark3+1);
    getch();
};
