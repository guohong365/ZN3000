#pragma once

#define ZND_ID "ZND"
#define ZND_MARK_MAX  10
#define ZND_PARAM_MAX 30
#define ZND_FILE_LENGTH 30100
#define MAX_WAVE_LENGTH  4000
#define MAX_WAVE_BUFFER_LENGTH  1024000

const static TCHAR PART_LABEL[][30]={
	_T("HEART"),
	_T("LUNG"),
	_T("LIVER"),
	_T("KIDNEY"),
	_T("LEFT_INTERNAL_CAROTID_ARTERY"),
	_T("RIGHT_INTERNAL_CAROTID_ARTERY"),
	_T("LEFT_VERTEBRAL_ARTERY"),
	_T("RIGHT_VERTEBRAL_ARTERY"),
	_T("ARM"),
	_T("LEFT_LEG"),
	_T("RIGHT_LEG"),
	_T("HEART_ECG")
};
const TCHAR BODY_STRING[][20]=
{
	_T("��"),
	_T("��"),
	_T("��"),
	_T("��"),
	_T("���ڶ���"),
	_T("�Ҿ��ڶ���"),
	_T("��׵����"),
	_T("��׵����"),
	_T("��"),
	_T("����"),
	_T("����"),
	_T("�ĵ�")
};

enum PartId
{
	PART_CALIBRATION =-2,
	PART_NONE=-1,
	PART_HEART=0,
	PART_LUNG,
	PART_LIVER,
	PART_KIDNEY,
	PART_LEFT_INTERNAL_CAROTID_ARTERY,
	PART_RIGHT_INTERNAL_CAROTID_ARTERY,
	PART_LEFT_VERTEBRAL_ARTERY,
	PART_RIGHT_VERTEBRAL_ARTERY,
	PART_ARM,
	PART_LEFT_LEG,
	PART_RIGHT_LEG,
	PART_HEART_ECG,
	PART_MAX
};

#define OS_IDLE        0x00000000
#define OS_PARAM_INPUT 0x00000001
#define OS_SELECT_PART 0x00000002
#define OS_BEGIN       0x00000004
#define OS_PAUSE       0x00000008
#define OS_CALC        0x00000010
#define OS_SEARCH      0x00000020
