// PersonInfoInpputDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "zn2000.h"
#include "PersonInfoInpputDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

// CPersonInfoInpputDlg 对话框

IMPLEMENT_DYNAMIC(CPersonInfoInpputDlg, CDialog)

CPersonInfoInpputDlg::CPersonInfoInpputDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CPersonInfoInpputDlg::IDD, pParent)
	, _Id(_T(""))
	, _Name(_T(""))
	, _gender(0)
	, _height(0)
	, _weight(0)
	, _age(0)
	, _evelation(0)
	, _systolicBloodPresure(0)
	, _diastolicBloodPresure(0)
	, _electrodeDistance(0)
{

}

CPersonInfoInpputDlg::~CPersonInfoInpputDlg()
{
}

void CPersonInfoInpputDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT_ID, _Id);
	DDX_Text(pDX, IDC_EDIT_NAME, _Name);
	DDX_Radio(pDX, IDC_RADIO_GENGER, _gender);
	DDX_Text(pDX, IDC_EDIT_HEIGHT, _height);
	DDX_Text(pDX, IDC_EDIT_WEIGHT, _weight);
	DDX_Text(pDX, IDC_EDIT_AGE, _age);
	DDX_Text(pDX, IDC_EDIT_EVELATION, _evelation);
	DDX_Text(pDX, IDC_EDIT_LOW_PRESURE, _diastolicBloodPresure);
	DDX_Text(pDX, IDC_EDIT_HI_PRESURE,_systolicBloodPresure );
	DDX_Text(pDX, IDC_EDIT_ELECTRODE_DISTANCE, _electrodeDistance);
}


BEGIN_MESSAGE_MAP(CPersonInfoInpputDlg, CDialog)
END_MESSAGE_MAP()


// CPersonInfoInpputDlg 消息处理程序
