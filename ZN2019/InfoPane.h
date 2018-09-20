#pragma once
#include "../libzn/Record.h"

// CInfoPane 对话框

class CInfoPane : public CDialog
{
	DECLARE_DYNAMIC(CInfoPane)
	Gdiplus::Bitmap *_pDrawBuffer;
	Gdiplus::Brush *_pHeartRateBrush;
	Gdiplus::Font *_pHeartBeatFont;
	Gdiplus::Brush *_pBloodPressureBrush;
	Gdiplus::Font *_pBloodPressureFont;
	Gdiplus::Brush *_pInfoBrush;
	Gdiplus::Font *_pInfoFont;

	RheographyRecord * _pRecord;
public:
	explicit CInfoPane(CWnd* pParent = nullptr);   
	virtual ~CInfoPane();

	void SetRecord(RheographyRecord* pPatient);

	void RefreshInfo();

	enum { IDD = IDD_INFO_PANE };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnPaint();
protected:
	virtual void OnOK();
	virtual void OnCancel();
public:
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
private:
	CString _id;
	CString _name;
	CString _gender;
	CString _height;
	CString _weight;
	CString _bloodPressure;
	CString _heartRate;
};
