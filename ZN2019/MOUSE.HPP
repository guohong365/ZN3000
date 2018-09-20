#ifndef _MOUSE_H_
#define _MOUSE_H_
#include "general.hpp"

class MOUSE
 {
 POINT Position;
 short valid;
 short visiable;
public: 
 MOUSE(void);
 short  Init(void);
 void On(void);
 void Off(void);
 short  WhereClick(void);
 void Where(POINT &Poshort);
 void Where(void);
 void SetPosition(short x,short y);
 void GetPosition(POINT &p);
 short  ButtonStatus(short button);
 short  ReleasStatus(short button);
 void SetLimitArea(short left,short top,short right,short bottom);
 void SetCursor(POINT HotPoshort,unsigned short mask[]);
 };
#endif
