#pragma once
class CDrawObject;
class IActionHandler:public CObject
{
	CDrawObject *_pSite;
	DECLARE_SERIAL(IActionHandler)
public:
	IActionHandler();
	void SetSite(CDrawObject *pSite);
	CDrawObject *GetSite();

	//Mouse Event
	virtual void OnMouseEnter();
	virtual void OnMouseOver(Gdiplus::Point pt, UINT flags);
	virtual void OnMouseLeave();
	virtual void OnMouseLButtonDown(Gdiplus::Point pt, UINT flags);
    virtual void OnMouseLButtonUp(Gdiplus::Point pt, UINT flags);

    virtual void OnMouseLButtonDlbClick(Gdiplus::Point pt, UINT flags);
    virtual void OnMouseRButtonDown(Gdiplus::Point pt, UINT flags);
	virtual void OnMouseRButtonUp(Gdiplus::Point pt, UINT flags);
	virtual void OnMouseRButtonDlbClick(Gdiplus::Point pt, UINT flags);

	//Keybord Event
	virtual void OnKeyDown(int key, UINT flags);
	virtual void OnKeyUp(int key, UINT flags);

	virtual ~IActionHandler(void);
};
