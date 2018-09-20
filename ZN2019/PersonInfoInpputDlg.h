#pragma once


// CPersonInfoInpputDlg 对话框

class CPersonInfoInpputDlg : public CDialog
{
	DECLARE_DYNAMIC(CPersonInfoInpputDlg)

public:
	CPersonInfoInpputDlg(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CPersonInfoInpputDlg();

// 对话框数据
	enum { IDD = IDD_PERSON_INFO };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	CString _Id;
	CString _Name;
	int _gender;
	float _height;
	float _weight;
	int _age;
	float _evelation;
	float _systolicBloodPresure;
	float _diastolicBloodPresure;
	float _electrodeDistance;
};
