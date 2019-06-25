#pragma once
#include "symbolobject.h"

class CImageSymbol :
    public CSymbolObject
{
private:
    Gdiplus::Bitmap * _pSymbolImage;
    bool _bUseTranparent;
    Gdiplus::Color _clrTransparent;

protected:
    CImageSymbol(void);
    DECLARE_SERIAL(CImageSymbol)
public:
    CImageSymbol(Gdiplus::Image * pImage, const CString &name, const CString &category,
		const Gdiplus::Size &size=Gdiplus::Size(DEFAULT_MAX_SYMBOL_SIZE, DEFAULT_MAX_SYMBOL_SIZE),
		int type=INTERNAL_SYMBOL);
    virtual void SetImage(Gdiplus::Image * pImage);
    virtual Gdiplus::Bitmap * GetSymbolImage();
    //TODO：图像在建立符号前已转为透明图像，设置该值目前仅为识别修改Symbol时是否修改了透明色。
    //  如果从功能完整性考虑，后续需保存原始图像，透明效果通过保存掩码模版图像实现，该模版在设置透明色时建立。两个图像同时保存。
    //
    virtual void SetTranparentColor(Gdiplus::Color color);
    virtual Gdiplus::Color GetTransparentColor();
    virtual void SetUseTransparentColor(bool bUse);
    virtual bool GetUseTrasparentColor();

    virtual CDrawObject* CopyFrom(CDrawObject * source);
    virtual ~CImageSymbol(void);

    virtual void OnDraw(Gdiplus::Graphics & graph);
    
    virtual void Serialize(CArchive & ar);
    virtual void OnFillColorChanged();
};
