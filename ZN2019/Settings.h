#pragma once

#define SETTING_FILE_NAME _T("zn2000.ini")

class Settings
{
public:
	int load();
	int save();

	float Evaluate;  //º£°Î
	float Z0;        //»ù´¡µ¼ÄÉ(Ä·Å·)

};