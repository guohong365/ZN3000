// TypeTest.cpp : 定义控制台应用程序的入口点。
//

#include "stdafx.h"
#include <iostream>
template <typename T> void printTypeName( T value){
    std::cout << typeid(T).name() << " " << value << std::endl;
    if(typeid(T) == typeid(char))
    {
        std::cout << "is char" << std::endl;
    }
}

int _tmain(int argc, _TCHAR* argv[])
{
    char v1= 'a';
    unsigned char v2 = 0x60;
    short v3= 1234;
    unsigned short v4= 1234;
    long v5=12345;
    unsigned long v6=12345;
    float v7= 12.345f;
    double v8=12.345;
    printTypeName(v1);
    printTypeName(v2);
    printTypeName(v3);
    printTypeName(v4);
    printTypeName(v5);
    printTypeName(v6);
    printTypeName(v7);
    printTypeName(v8);
	return 0;
}

