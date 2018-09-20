#ifndef _POINTER_HPP_
#define _POINTER_HPP_
#include <stdio.h>
#include "general.hpp"

const char PrinterName[][33]={
    "Epson,Star SR 系列打印机",
    "EPSON九针打印机",
    "",
    "EPSON 九针彩色打印机",
    "EPSON 二十四针或其兼容打印机",
    "IBM 二十四针或其兼容打印机",
    "EPSON 二十四针或其兼容彩色打印机",
    "IBM 二十四针或其兼容彩色打印机",
    "",
    "HP Laser IIP",
    "没有打印机"
};
const char PrinterModeName[10][5][8]={
 { "60x72","120x72","120x144","240x144",""},
 { "60x72","120x72","120x216","240x216",""},
 { "60x72","120x72","120x216","240x216",""},
 { "60x72","120x72","120x216","240x216",""},
 { "60x72","120x72","120x216","240x216",""},
 { "60x180","120x180","180x180","360x180","360x360"},
 { "60x180","120x180","180x180","360x180",""},
 { "60x180","120x180","180x180","360x180","360x360"},
 { "60x180","120x180","180x180","360x180",""},
 { "75x75","100x100","150x150","300x300",""},
};

/*   drivers  */
enum PrinterType
{
    STAR_SR9= 0,     /* Epson ????, Star SR  */
    EPSON9=1,        /* EPSON */
    PANASONIC9=2,   /* Panasonic KX-P1124 - not tested */
    EPSON9c=3,      /* EPSON (color) */
    EPSON24=4,      /* 24 pins EPSON compatible printer */
    IBM24=5,        /* 24 pins IBM compatible printer */
    EPSON24c=6,    /* 24 pins EPSON compatible color printer */
    IBM24c=7,       /* 24 pins IBM compatible color printer */
    HPPaintJet=8,  /* HP Paint Jet - not tested */
    HPLJII =9 , /* HP Laser Jet*/
    NoPrinter=10
};

 /* modes for particular drivers */
enum PrinterMode
{  Lowest=0,Low=1,Middle=2,High=3,Highest=4 };

struct Printer
{
    PrinterType Type;    //kind of printer
    PrinterMode Mode; //resolution mode
    unsigned short Width;
    unsigned short Height; 
    unsigned short LeftMargin;
    unsigned short TopMargin;
    bool Rotate;
    bool Revers;
    Printer(){};
    Printer(PrinterType type,PrinterMode mode,short width,short height,
                 short leftmargin,short topmargin,bool rotate,bool revers)
                 :Type(type),Mode(mode),Width(width),Height(height),LeftMargin(leftmargin),
                 TopMargin(topmargin),Rotate(rotate),Revers(revers){};
};

enum PRN_Type
{
    Point=0,
    Line=1,
    Bar=2,
    Rect=3,
    Circle=4,
    Ellipse=5,
    Text=6,
    Bitmap=7,
    UserType=8
};

//DATA TYPE DESCRIPTION
struct PRN_Object{};

struct PRN_POINT:public PRN_Object
{
         POINT  Pos;
         COLORS Color;
         PRN_POINT(POINT&p,COLORS color):Pos(p),Color(color){};
};       

struct PRN_LINE:public PRN_Object
{
    POINT Start;
    POINT End;
    COLORS Color;
    line_styles LineStyle;
    PRN_LINE(POINT &start,POINT&end,COLORS color,line_styles linestyle):
                       Start(start),End(end),Color(color),LineStyle(linestyle){};
};

struct PRN_BAR:public PRN_Object
{
    POINT Left;
    POINT Top;
    POINT Right;
    POINT Bottom;
    COLORS FillColor;
    fill_patterns FillStyle;
    PRN_BAR(POINT&left,POINT &top, POINT &right, POINT &bottom, COLORS fillcolor, fill_patterns fillstyle)
                    :Left(left),Top(top),Right(right),Bottom(bottom),FillColor(fillcolor),FillStyle(fillstyle){};

};

struct PRN_RECT:public PRN_Object
{
    POINT Left;
    POINT Top;
    POINT Right;
    POINT Bottom;
    COLORS BorderColor;
    line_styles LineStyle;
    PRN_RECT(POINT&left,POINT &top, POINT &right, POINT &bottom, COLORS bordercolor, line_styles linestyle)
            :Left(left),Top(top),Right(right),Bottom(bottom),BorderColor(bordercolor),LineStyle(linestyle){};
};

struct PRN_CIRCLE:public PRN_Object
{
    POINT Center;
    short Length;
    COLORS Color;
    PRN_CIRCLE(POINT&center,short len,COLORS color)
                :Center(center),Length(len),Color(color){};
};

struct  PRN_ELLEPSE:public PRN_Object
{
    POINT Center;
    short X_Radio;
    short Y_Radio;
    COLORS FillColor;
    fill_patterns FillStyle;
    PRN_ELLEPSE(POINT &center,short x,short y,COLORS fillcolor,fill_patterns fillstyle)
            :Center(center),X_Radio(x),Y_Radio(y),FillColor(fillcolor),FillStyle(fillstyle){};
};

struct PRN_TEXT:public PRN_Object
{
    POINT Pos;
    COLORS Color;
    short Height;
    short Width;
    char String[80];
    PRN_TEXT(POINT pos,COLORS color,short height,short width,char *string)
             :Pos(pos),Color(color),Height(height),Width(width)
             {  strcpy(String,string);};
};

struct PRN_BITMAP:public PRN_Object
{
    POINT Pos;
    putimage_ops Operation;
    short Width;
    short Height;
    char *Image;
    PRN_BITMAP(POINT&pos,putimage_ops ops,short width,short height)
          :Pos(pos),Operation(ops),Width(width),Height(height)
          { Image=new char[_imagesize(0,0,Width,Height)]; };
};

//USER TYPE     { APPENDENT USER'S DEFINATION }
void PrintSetup(void);
void Print(void (*fun)(FILE*fp));

#endif
