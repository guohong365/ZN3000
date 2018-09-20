#include "stdafx.h"
#include "zn2000.h"
#include "InfoPane.h"
#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CInfoPane 对话框

IMPLEMENT_DYNAMIC(CInfoPane, CDialog)

CInfoPane::CInfoPane(CWnd* pParent /*=NULL*/)
	: CDialog(IDD, pParent)
	, _pDrawBuffer(nullptr)
	, _pHeartRateBrush(nullptr)
	, _pHeartBeatFont(nullptr)
	, _pBloodPressureBrush(nullptr)
	, _pBloodPressureFont(nullptr)
	, _pInfoBrush(nullptr)
	, _pInfoFont(nullptr)
	, _pRecord(nullptr)
	, _id(_T("编号："))
	, _name(_T("姓名："))
	, _gender(_T("性别："))
	, _height(_T("身高："))
	, _weight(_T("体重："))
	, _bloodPressure(_T("血压："))
	, _heartRate(_T("心率："))
{
}

CInfoPane::~CInfoPane()
{
}

void CInfoPane::SetRecord(RheographyRecord* pRecord)
{
	_pRecord = pRecord;
}

void CInfoPane::RefreshInfo()
{
	if (_pRecord)
	{
		_id.Format(_T("编号：%s"), _pRecord->getPatientId());
		_name.Format(_T("姓名：%s"), _pRecord->getPatientName());
		_gender.Format(_T("性别：%s"), _pRecord->getGender() == _T('M') ? _T("男") : _T("女"));
		_height.Format(_T("身高：%3.1f (cm)"), _pRecord->getHeight());
		_weight.Format(_T("体重：%3.1f (Kg)"), _pRecord->getWeight());
		_bloodPressure.Format(_T("血压：%3.1f / %3.1f (mmHg)"), _pRecord->getDiastolicBloodPressure(),
		                      _pRecord->getSystolicBloodPressure());
		if (_pRecord->getHeartRate() != 0)
		{
			_heartRate.Format(_T("心率：%d"), int(_pRecord->getHeartRate()));
		}
		else
		{
			_heartRate = _T("心率：");
		}
	}
	else
	{
		_id = _T("编号：");
		_name = _T("姓名：");
		_gender = _T("性别：");
		_height = _T("身高：");
		_weight = _T("体重：");
		_bloodPressure = _T("血压：");
		_heartRate = _T("心率：");
	}
	UpdateData(FALSE);
}

void CInfoPane::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_STATIC_ID, _id);
	DDX_Text(pDX, IDC_STATIC_NAME, _name);
	DDX_Text(pDX, IDC_STATIC_GEBDER, _gender);
	DDX_Text(pDX, IDC_STATIC_HEIGHT, _height);
	DDX_Text(pDX, IDC_STATIC_WEIGHT, _weight);
	DDX_Text(pDX, IDC_STATIC_BLOOD_PRESURE, _bloodPressure);
	DDX_Text(pDX, IDC_STATIC_BEAT_RATE, _heartRate);
}


BEGIN_MESSAGE_MAP(CInfoPane, CDialog)
	ON_WM_PAINT()
	ON_WM_CTLCOLOR()
END_MESSAGE_MAP()


void CInfoPane::OnPaint()
{
	CPaintDC dc(this); // device context for painting
}

void CInfoPane::OnOK()
{
}

void CInfoPane::OnCancel()
{
}

HBRUSH CInfoPane::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
{
	CDialog::OnCtlColor(pDC, pWnd, nCtlColor);

	switch(nCtlColor)
	{
	case CTLCOLOR_DLG:
		break;
	case CTLCOLOR_STATIC:
		if(pWnd->GetDlgCtrlID()==IDC_STATIC_BEAT_RATE)
		{
			pDC->SetTextColor(RGB(255, 0, 0));
		}
		else
		{
			pDC->SetTextColor(RGB(0, 255, 0));
		}
		pDC->SetBkMode(TRANSPARENT);
		break;
	default: 
		break;
	}
	return HBRUSH(GetStockObject(BLACK_BRUSH));
	//return hbr;
}
