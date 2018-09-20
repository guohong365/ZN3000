#pragma once

#define ZND_ID "ZND"
#define ZND_DATA_SIZE 570
#define ZND_MARK_MAX  10
#define ZND_PARAM_MAX 30
#define ZND_FILE_LENGTH 30100

#define MAX_WAVE_LENGTH  4000
#define MAX_WAVE_BUFFER_LENGTH  1024000

const TCHAR BODY_STRING[][20]=
{
	_T("ÐÄ"),
	_T("·Î"),
	_T("¸Î"),
	_T("Éö"),
	_T("×ó¾±ÄÚ¶¯Âö"),
	_T("ÓÒ¾±ÄÚ¶¯Âö"),
	_T("×ó×µ¶¯Âö"),
	_T("ÓÒ×µ¶¯Âö"),
	_T("±Û"),
	_T("×óÍÈ"),
	_T("ÓÒÍÈ")
};

enum PartId
{
	HEART=0,
	LUNG,
	LIVER,
	KIDNEY,
	LEFT_INTERNAL_CAROTID_ARTERY,
	RIGHT_INTERNAL_CAROTID_ARTERY,
	LEFT_VERTEBRAL_ARTERY,
	RIGHT_VERTEBRAL_ARTERY,
	ARM,
	LEFT_LEG,
	RIGHT_LEG,
	PART_MAX
};

#define OS_IDLE        0x00000000
#define OS_PARAM_INPUT 0x00000001
#define OS_SELECT_PART 0x00000002
#define OS_BEGIN       0x00000004
#define OS_PAUSE       0x00000008
#define OS_CALC        0x00000010
#define OS_SEARCH      0x00000020
