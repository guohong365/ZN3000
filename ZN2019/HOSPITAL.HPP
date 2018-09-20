#ifndef _HOSPITAL_HPP_
#define _HOSPITAL_HPP_
#include <stdio.h>
unsigned short const evMesure=0x65ff;
const unsigned short evBegin=0x66ff;
const unsigned short evPause=0x67ff;
const unsigned short evGetData=0x68ff;
const unsigned short evBodyPart=0x69ff;
const unsigned short evParameter=0x70ff;
const unsigned short evNextStep=0x71ff;

#define  WaveWinLeft 60
const unsigned short WaveWinTop=50;
const unsigned short WaveWinRight=600;
const unsigned short WaveWinBottom=360;

const unsigned short WaveLeft=80;
const unsigned short WaveTop=80;
const unsigned short WaveRight=580;
const unsigned short WaveBottom=340;
//const unsigned short WaveZero1=160;
const unsigned short WaveZero2=280;
const unsigned short WinWidth=500;

#define TOTAL_DATA 5000

const char BodyString[][20]=
{
    "ÐÄ",
    "·Î",
    "¸Î",
    "Éö",
    "×ó¾±ÄÚA",
    "ÓÒ¾±ÄÚA",
    "×ó×µA",
    "ÓÒ×µA",
    "±Û",
    "×óÍÈ",
    "ÓÒÍÈ"
};
    
enum PARTID
{
    HEART,
    LUNG,
    GAN,
    SHEN,
    LEFTBRAINE,
    RIGHTBRAINE,    
    LEFTBRAINZ,
    RIGHTBRAINZ,
    ARM,
    LEFTLEG,
    RIGHTLEG
};
    
    
struct Person
{
    char Name[9];
    char No[9];
    char Date[9];
    enum {Famale,Male}Sex;
    unsigned short Age;
    float Height;
    float Weight;
    unsigned short HiBloodPresure;
    unsigned short  LowBloodPresure;
    unsigned short BeatRate;
    float HaiBa;
    float DjInterval;
};
short GetBodyPart(void);
unsigned short GetMesure(void);
unsigned short GetPara(Person &);
unsigned short GetData(void);
unsigned short Mesure(void);
void PrePrint(FILE *);
#endif
