#include "stdafx.h"
#include "zn2000.h"
#include "InfoPane.h"
#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CInfoPane �Ի���

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
	, _pPatient(nullptr)
	, _id(_T("��ţ�"))
	, _name(_T("������"))
	, _gender(_T("�Ա�"))
	, _height(_T("��ߣ�"))
	, _weight(_T("���أ�"))
	, _bloodPressure(_T("Ѫѹ��"))
	, _heartRate(_T("���ʣ�"))
{
}

CInfoPane::~CInfoPane()
{
}

void CInfoPane::SetPatient(IPatient* pPatient)
{
	_pPatient = pPatient;
}

void CInfoPane::RefreshInfo()
{
	if (_pPatient)
	{
		_id.Format(_T("��ţ�%s"), _pPatient->getId());
		_name.Format(_T("������%s"), _pPatient->getName());
		_gender.Format(_T("�Ա�%s"), _pPatient->getGender() == 0 ? _T("��") : _T("Ů"));
		_height.Format(_T("��ߣ�%3.1f (cm)"), _pPatient->getHeight());
		_weight.Format(_T("���أ�%3.1f (Kg)"), _pPatient->getWeight());
		_bloodPressure.Format(_T("Ѫѹ��%3.1f / %3.1f (mmHg)"), _pPatient->getDiastolicBloodPressure(),
		                      _pPatient->getSystolicBloodPressure());
		if (_pPatient->getHeartRate() != 0)
		{
			_heartRate.Format(_T("���ʣ�%d"), int(_pPatient->getHeartRate()));
		}
		else
		{
			_heartRate = _T("���ʣ�");
		}
	}
	else
	{
		_id = _T("��ţ�");
		_name = _T("������");
		_gender = _T("�Ա�");
		_height = _T("��ߣ�");
		_weight = _T("���أ�");
		_bloodPressure = _T("Ѫѹ��");
		_heartRate = _T("���ʣ�");
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
