#pragma once


// CPersonInfoInpputDlg �Ի���

class CPersonInfoInpputDlg : public CDialog
{
	DECLARE_DYNAMIC(CPersonInfoInpputDlg)

public:
	CPersonInfoInpputDlg(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CPersonInfoInpputDlg();

// �Ի�������
	enum { IDD = IDD_PERSON_INFO };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

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
