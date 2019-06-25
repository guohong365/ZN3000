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
    //TODO��ͼ���ڽ�������ǰ��תΪ͸��ͼ�����ø�ֵĿǰ��Ϊʶ���޸�Symbolʱ�Ƿ��޸���͸��ɫ��
    //  ����ӹ��������Կ��ǣ������豣��ԭʼͼ��͸��Ч��ͨ����������ģ��ͼ��ʵ�֣���ģ��������͸��ɫʱ����������ͼ��ͬʱ���档
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
