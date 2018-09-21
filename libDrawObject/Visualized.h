#pragma once

#define GET_PIC_FLAG_CLIP    0x00000001    //ԭʼ��С������
#define GET_PIC_FLAG_STRETCH 0x00000002    //����������x��y���Ų�һ��
#define GET_PIC_FLAG_ASPECT  0x00000004    //x��y���������ţ�һ�������������
#define GET_PIC_FLAG_MASK    0x000000FF


#define GET_PIC_POS_TOP     0x00000100
#define GET_PIC_POS_BOTTOM  0x00000200
#define GET_PIC_POS_LEFT    0x00000400
#define GET_PIC_POS_RIGHT   0x00000800
#define GET_PIC_POS_HCENTER 0x00001000
#define GET_PIC_POS_VCENTER 0x00002000
#define GET_PIC_POS_MASK    0x00FFFF00

class Visualized
{
public:
    virtual ~Visualized(){}
    virtual Gdiplus::Image* GetPicture(int width, int height, Gdiplus::Color colorBackground, DWORD flag){ return nullptr; }
};
