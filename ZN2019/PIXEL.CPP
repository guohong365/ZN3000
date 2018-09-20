 #include <conio.h>
extern char *Video;
extern unsigned long LineTable[];
unsigned short OldMode;
void SetWRMode(short m)
{
    outp(0x3ce,0x05);
    OldMode=inp(0x3cf);
    outp(0x3cf,(OldMode&0xfc)|m);
    outpw(0x3c4,0xf<<8|0x02);
};
void ResetMode(void)
{
    outp(0x3ce,0x05);
    outp(0x3cf,OldMode);
};    

void SetPixel(short x,short y,short color)
{
    char *sr=Video+LineTable[y]+x/8;
    unsigned short old,bit;
    // Set bitmask
//    SetWRMode(2);
    outp(0x3ce,0x05);
    OldMode=inp(0x3cf);
    outp(0x3cf,(OldMode&0xfc)|2);
    outpw(0x3c4,0xf<<8|0x02);
    
    old=0x80>>(x&7);
    outpw(0x3ce,old<<8|0x08);
    old=*sr;
    *sr=(char )color;
};

