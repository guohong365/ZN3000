//////////////////////////////////////////////////////////////////////////////////
// CimageProcess.cpp
// Author : BAla
//////////////////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include <GdiPlus.h>
#include "CimageProcess.h"
#include "MathUtility.h"


double CImageProcessor::m_3DCorrectRatio=1.0;

CImageProcessor::CImageProcessor()
{
	GdiplusStartup(&m_gdiplusToken, &m_gdiplusStartupInput, NULL);	
}


void CImageProcessor::Brightness(Gdiplus::Bitmap *pImage, int nRedVal, int nGreenVal, int nBlueVal)
{
	char *buf;
	Gdiplus::Rect imageRect(0,0, pImage->GetWidth(), pImage->GetHeight());
	Gdiplus::BitmapData bmpData;
	pImage->LockBits(&imageRect, 
		Gdiplus::ImageLockModeRead | Gdiplus::ImageLockModeWrite , 
		PixelFormat32bppRGB, &bmpData);

	// Buffer
	buf = (char*)bmpData.Scan0;
	long nCount=0;
	for (unsigned long i=0; i<bmpData.Height; ++i)
	{
		nCount=0;
		for (unsigned long j=0; j<bmpData.Width; ++j)
		{
			long lVal=0;
			memcpy(&lVal, &buf[nCount], 4);

			// Get the reverse order
			int b = GetRValue(lVal);
			int g = GetGValue(lVal);
			int r = GetBValue(lVal);

			// Red
			r += nRedVal;
			if (r >255)
			{
				r = 255;
			}
			if (r <0)
			{
				r = 0;
			}

			// Green
			g += nGreenVal;
			if (g>255)
			{
				g = 255;
			}
			if (g<0)
			{
				g = 0;
			}

			// Blue
			b += nBlueVal;
			if (b >255)
			{
				b = 255;
			}
			if (b<0)
			{
				b = 0;
			}

			// Store reverse order
			lVal = RGB(b, g, r);
			memcpy(&buf[nCount], &lVal, 4);

			// Increment with 4. RGB color take 4 bytes.The high-order byte must be zero
			// See in MSDN COLORREF
			nCount+=4;
		}
		buf +=bmpData.Stride;
	}

	pImage->UnlockBits(&bmpData);
}



void CImageProcessor::Contrast(Gdiplus::Bitmap *pImage, int nContrastVal)
{
	char *buf;
	Gdiplus::BitmapData bmpData;
	Gdiplus::Rect imageRect(0,0, pImage->GetWidth(), pImage->GetHeight());
	pImage->LockBits(&imageRect, Gdiplus::ImageLockModeRead | Gdiplus::ImageLockModeWrite, PixelFormat32bppRGB, &bmpData);

	buf = (char *) bmpData.Scan0;
	long nCount=0;	
	for (unsigned long i=0; i<bmpData.Height; ++i)
	{
		nCount=0;
		for (unsigned long j=0; j<bmpData.Width; ++j)
		{			
			long lVal=0;
			memcpy(&lVal, &buf[nCount], 4);
			// Get from buffer in reverse order
			int b = GetRValue(lVal);
			int g = GetGValue(lVal);
			int r = GetBValue(lVal);

			r = ((r-128)*nContrastVal)/100 +128;
			g = ((g-128)*nContrastVal)/100 +128;
			b = ((b-128)*nContrastVal)/100 +128;			

			// Red
			if (r >255)
			{
				r = 255;
			}
			if (r <0)
			{
				r = 0;
			}

			// Green
			if (g>255)
			{
				g = 255;
			}
			if (g<0)
			{
				g = 0;
			}

			// Blue			
			if (b >255)
			{
				b = 255;
			}
			if (b<0)
			{
				b = 0;
			}

			// Store in reverse order			
			lVal = RGB((int)b, (int)g, (int)r);

			memcpy(&buf[nCount], &lVal, 4);

			nCount+=4;
		}
		buf +=bmpData.Stride;

	}

	pImage->UnlockBits(&bmpData);
}


void CImageProcessor::BlackAndWhite(Gdiplus::Bitmap *pImage)
{
	char *buf;
	Gdiplus::BitmapData bmpData;
	Gdiplus::Rect imageRect(0,0,pImage->GetWidth(), pImage->GetHeight());

	pImage->LockBits(&imageRect, Gdiplus::ImageLockModeRead|Gdiplus::ImageLockModeWrite, PixelFormat32bppRGB, &bmpData);

	buf = (char *) bmpData.Scan0;

	long nCount=0;	
	for (unsigned long i=0; i<bmpData.Height; ++i)
	{
		nCount=0;
		for (unsigned long j=0; j<bmpData.Width; ++j)
		{
			long lVal=0;
			memcpy(&lVal, &buf[nCount], 4);
			// Get the color value from buffer
			int b = GetRValue(lVal);
			int g = GetGValue(lVal);
			int r = GetBValue(lVal);

			// get the average color value
			lVal = (r+g+b)/3;

			// assign to RGB color			
			lVal = RGB(lVal, lVal, lVal);
			memcpy(&buf[nCount], &lVal, 4);

			nCount+=4;
		}
		buf+=bmpData.Stride;
	}

	pImage->UnlockBits(&bmpData);
}



void CImageProcessor::InvertColor(Gdiplus::Bitmap *pImage)
{
	unsigned char *buf;
	Gdiplus::BitmapData bmpData;
	Gdiplus::Rect imageRect(0,0,pImage->GetWidth(), pImage->GetHeight());
	pImage->LockBits(&imageRect, Gdiplus::ImageLockModeRead|Gdiplus::ImageLockModeWrite, PixelFormat32bppRGB, &bmpData);

	buf = (unsigned char *) bmpData.Scan0;
	long nCount=0;	
	for (unsigned long i=0; i<bmpData.Height; ++i)
	{
		nCount=0;
		for (unsigned long j=0; j<bmpData.Width; ++j)
		{
			buf[nCount]=255-buf[nCount];
			buf[nCount+1]=255-buf[nCount+1];
			buf[nCount+2]=255-buf[nCount+2];
			nCount+=4;
		}
		buf+=bmpData.Stride;
	}

	pImage->UnlockBits(&bmpData);
}

void CImageProcessor::Blur(Gdiplus::Bitmap *pImage)
{
	unsigned char *pOriBuf; // pointer to a original buffer

	Gdiplus::BitmapData bmpData1;
	Gdiplus::Rect imageRect(0,0,pImage->GetWidth(), pImage->GetHeight());
	pImage->LockBits(&imageRect, Gdiplus::ImageLockModeRead|Gdiplus::ImageLockModeWrite, PixelFormat32bppRGB, &bmpData1);

	pOriBuf = (unsigned char *) bmpData1.Scan0;

	Gdiplus::BitmapData bmpData2;
	Gdiplus::Bitmap *bmp2=pImage->Clone(imageRect, pImage->GetPixelFormat());
	bmp2->LockBits(&imageRect, Gdiplus::ImageLockModeRead | Gdiplus::ImageLockModeWrite, PixelFormat32bppRGB, &bmpData2);

	unsigned char *tmpBuf = (unsigned char *) bmpData2.Scan0;

	long nCount=0;
	unsigned long c1;

	// Retrive from original buffer
	// Caluculate the value and store new value into tmpBuf
	for (unsigned long i=0; i<bmpData1.Height; ++i)
	{
		nCount=0;
		for (unsigned long j=0; j<bmpData1.Width; ++j)
		{
			// Red
			if ( i> 0  && i < bmpData1.Height-1 && j > 0 && j < bmpData1.Width -1)
			{
				for(int k=0; k <3 ; k ++)
				{
					c1= *(pOriBuf - bmpData1.Stride + nCount - 4 + k) + *(pOriBuf - bmpData1.Stride + nCount + k)  + *(pOriBuf - bmpData1.Stride + nCount + 4 + k) +
						*(pOriBuf + nCount - 4 + k) + *(pOriBuf + nCount + k) + *(pOriBuf + nCount + 4 + k) +
						*(pOriBuf + bmpData1.Stride + nCount - 4 + k) + *(pOriBuf + bmpData1.Stride + nCount + k) + *(pOriBuf + bmpData1.Stride + nCount + 4 + k) ;
					c1/=9;
					tmpBuf[nCount + k]=(unsigned char)c1;
				}
			}
			else
			{
				tmpBuf[nCount    ]=pOriBuf[nCount    ];
				tmpBuf[nCount + 1]=pOriBuf[nCount + 1];
				tmpBuf[nCount + 2]=pOriBuf[nCount + 2];
			}

			nCount+=4;
		}
		pOriBuf+=bmpData1.Stride;
		tmpBuf+=bmpData2.Stride;
	}

	pImage->UnlockBits(&bmpData1);
	bmp2->UnlockBits(&bmpData2);
	Gdiplus::Graphics graph(pImage);
	graph.DrawImage(bmp2, imageRect);
	delete bmp2;
}

void CImageProcessor::Sharpness(Gdiplus::Bitmap *pImage)
{
	unsigned char *pOriBuf; // pointer to a original buffer

	Gdiplus::BitmapData bmpData1;
	Gdiplus::Rect imageRect(0,0,pImage->GetWidth(), pImage->GetHeight());
	pImage->LockBits(&imageRect, Gdiplus::ImageLockModeRead|Gdiplus::ImageLockModeWrite, PixelFormat32bppRGB, &bmpData1);

	pOriBuf = (unsigned char *) bmpData1.Scan0;

	Gdiplus::BitmapData bmpData2;
	Gdiplus::Bitmap *bmp2=pImage->Clone(imageRect, pImage->GetPixelFormat());
	bmp2->LockBits(&imageRect, Gdiplus::ImageLockModeRead | Gdiplus::ImageLockModeWrite, PixelFormat32bppRGB, &bmpData2);

	unsigned char *tmpBuf = (unsigned char *) bmpData2.Scan0;

	long nCount=0;
	long c1, c2, c3, c4, c5;

	for (unsigned long i=0; i<bmpData1.Height; ++i)
	{
		nCount=0;
		for (unsigned long j=0; j<bmpData1.Width; ++j)
		{
			// Red
			if ( i> 0  && i < bmpData1.Height-1 && j > 0 && j < bmpData1.Width -1)
			{
				for(int k=0; k <3 ; k ++)
				{
					c1= *(pOriBuf + nCount + k);
					c2= *(pOriBuf - bmpData1.Stride + nCount + k);
					c3= *(pOriBuf + nCount - 4 + k);
					c4= *(pOriBuf + nCount + 4 + k);
					c5= *(pOriBuf + bmpData1.Stride + nCount + k);

					c1=c1 * 5 - ( c2 + c3 + c4 + c5 );

					if(c1 < 0)
					{
						c1=0;
					}
					if(c1 > 255)
					{
						c1=255;
					}

					tmpBuf[nCount + k]=(unsigned char)c1;
				}
			}
			else
			{
				tmpBuf[nCount    ]=pOriBuf[nCount    ];
				tmpBuf[nCount + 1]=pOriBuf[nCount + 1];
				tmpBuf[nCount + 2]=pOriBuf[nCount + 2];
			}
			tmpBuf[nCount + 3]=0;

			nCount+=4;
		}
		pOriBuf+=bmpData1.Stride;
		tmpBuf+=bmpData2.Stride;
	}


	pImage->UnlockBits(&bmpData1);
	bmp2->UnlockBits(&bmpData2);
	Gdiplus::Graphics graph(pImage);
	graph.SetInterpolationMode(Gdiplus::InterpolationModeHighQualityBicubic);
	graph.DrawImage(bmp2, Gdiplus::Rect(0,0,pImage->GetWidth(), pImage->GetHeight()), 0,0,pImage->GetWidth(), pImage->GetHeight(), Gdiplus::UnitPixel);
	delete bmp2;
}

CImageProcessor::~CImageProcessor()
{
	Gdiplus::GdiplusShutdown(m_gdiplusToken);
}


Gdiplus::Bitmap *CImageProcessor::CorrectImage( Gdiplus::Bitmap * pBitmap, const Gdiplus::Point *pt0,double ratio, void *param, bool (*callback)( void * , long , long))
{
	CString message;

	if(pBitmap==NULL)
		return NULL;
	Gdiplus::Bitmap *pDestBmp=NULL;
	unsigned char * pDestColor;
	unsigned char * pSourceColor;

	double x0[4];
	double y0[4];
	double x1[4];
	double y1[4];
	double k[8]; 
	int m;
    //int 型坐标倒成double型
	for(m=0; m< 4; m++)
	{
		x0[m]=pt0[m].X;
		y0[m]=pt0[m].Y;
	}

	double angle[4];
    //计算矩形变形后梯形四条边与水平线的角度
	for( m=0; m<4; m++)
	{
		angle[m]=CMathUtility::CalculateLineAngle(x0[m], y0[m], x0[(m+1)%4], y0[(m+1)%4]);
		while(angle[m]<0)
		{
			angle[m] +=360;
		}
	}

    //找与水平线成最小角度的边为基线
     double minAngle=angle[0];  //最小角度
     int baseline=0;            //最小角度边序号
     for(m = 1 ; m < 3; m++)
     {
        if(angle[m+1] < angle [m])
        {
            minAngle=angle[m];
            baseline = m;
        }
     }
    
	double length[4];
	double total=0;
    //计算梯形四条边长及周长
	for(m=0; m<4; m++)
	{
		length[m] = (x0[m] -x0[(m+1)%4])*(x0[m] -x0[(m+1)%4]) + (y0[m] -y0[(m+1)%4])*(y0[m] -y0[(m+1)%4]);
		length[m] = sqrt(length[m]);
		total += length[m];
	}
    //按长宽比计算原始矩形长宽（像素）
    double origion_height=total/2./(1.+ ratio);
    double origion_width=origion_height * ratio;
    //给原始矩形四顶点赋坐标
	x1[0] = x0[0],				    y1[0]=y0[0];                       //left-top
	x1[1] = x0[0] - origion_width,	y1[1]=y0[0];                       //right - top
	x1[2] = x0[0] - origion_width,	y1[2]=y0[0] - origion_height;      //right - bottom
	x1[3] = x0[0],			        y1[3]=y0[0] - origion_height;      //left-bottom


	Gdiplus::PointF offset;
	Gdiplus::Matrix matrix;
    minAngle=angle[0];

    offset.X=(Gdiplus::REAL)x0[0];
    offset.Y=(Gdiplus::REAL)y0[0];
	matrix.Translate(-offset.X, -offset.Y);
	//matrix.RotateAt((REAL)-minAngle, offset);
	Gdiplus::PointF pt[4]={
		Gdiplus::PointF((Gdiplus::REAL)x1[0],(Gdiplus::REAL)y1[0]),
		Gdiplus::PointF((Gdiplus::REAL)x1[1],(Gdiplus::REAL)y1[1]),
		Gdiplus::PointF((Gdiplus::REAL)x1[2],(Gdiplus::REAL)y1[2]),
		Gdiplus::PointF((Gdiplus::REAL)x1[3],(Gdiplus::REAL)y1[3])
	};
	matrix.TransformPoints(pt, 4);

	for(m=0; m<4 ; m++)
	{
		x1[m]=pt[m].X;
		y1[m]=pt[m].Y;
	}


	if(!CMathUtility::CalculateCorrectParameter(x1, y1, x0, y0, k))
		return NULL;


	//double ax0 = (2035 * k[0] + 995 * k[1] + k[2])/(k[6] * 2035 + k[7] * 995 + 1);
	//double ay0 = (2035 * k[3] + 995 * k[4] + k[5])/(k[6] * 2035 + k[7] * 995 + 1);

	double ax1 = k[2];
	double ay1 = k[5];

	double ax2 = (pBitmap->GetHeight() * k[1] + k[2])/( k[7] * pBitmap->GetHeight() + 1);
	double ay2 = (pBitmap->GetHeight() * k[4] + k[5])/( k[7] * pBitmap->GetHeight() + 1);
	double ax3 = (pBitmap->GetWidth() * k[0] + pBitmap->GetHeight() * k[1] + k[2])/(k[6] * pBitmap->GetWidth() + k[7] * pBitmap->GetHeight() + 1);
	double ay3 = (pBitmap->GetWidth() * k[3] + pBitmap->GetHeight() * k[4] + k[5])/(k[6] * pBitmap->GetWidth() + k[7] * pBitmap->GetHeight() + 1);

	double ax4 = (pBitmap->GetWidth() * k[0]  + k[2])/(k[6] * pBitmap->GetWidth()  + 1);
	double ay4 = (pBitmap->GetWidth() * k[3]  + k[5])/(k[6] * pBitmap->GetWidth()  + 1);

	double minX=min(ax1, min(ax2, min(ax3, ax4)));
	double minY=min(ay1, min(ay2, min(ay3, ay4)));
	double maxX=max(ax1, max(ax2, max(ax3, ax4)));
	double maxY=max(ay1, max(ay2, max(ay3, ay4)));


	long width=(int)(maxX-minX);
	long height=(int)(maxY-minY);

	if(callback)
	{
		message=_T("");
		if(!callback(param, 0, height))
		{
			return NULL;
		}
	}
	if(CMathUtility::CalculateCorrectParameter(x0, y0, x1, y1, k))
	{

		double ax;
		double ay;
		double x;
		double y;

		pDestBmp=new Gdiplus::Bitmap(width, height, PixelFormat32bppARGB);
        if(!pDestBmp)
        {
            return NULL;
        }
		Gdiplus::Rect rect;
		Gdiplus::BitmapData souceData;
		Gdiplus::BitmapData destData;
		rect=Gdiplus::Rect(0,0,pBitmap->GetWidth(), pBitmap->GetHeight());
        if(pBitmap->LockBits(&rect,Gdiplus::ImageLockModeRead , PixelFormat32bppARGB, &souceData)!=Gdiplus::Ok)
        {
            return NULL;
        }
		rect=Gdiplus::Rect(0,0,width,height);
        if(pDestBmp->LockBits(&rect,Gdiplus::ImageLockModeRead | Gdiplus::ImageLockModeWrite , PixelFormat32bppARGB, &destData)!=Gdiplus::Ok)
        {
            pBitmap->UnlockBits(&souceData);
            return NULL;
        }
		long row;
		long col;
		double alpha;
		double beta;
		long left;
		long right;
		long top;
		long bottom;
		pSourceColor=(unsigned char *)souceData.Scan0;
		pDestColor=(unsigned char *)destData.Scan0;
		DWORD colorByte[4];

		long destOffset;

		for(row = 0; row< height; row ++)
		{
			destOffset=0;
			for(col = 0; col < width; col ++)
			{
				x = col + minX;
				y = row + minY;
				ax = (x * k[0] + y * k[1] + k[2])/(k[6] * x + k[7] * y + 1);
				ay = (x * k[3] + y * k[4] + k[5])/(k[6] * x + k[7] * y + 1);

				alpha=ax - floor(ax);
				beta =ay - floor(ay);
				//alpha channel

				left   =(long) floor(ax);
				right  = left + 1;
				top    = (long)floor(ay);
				bottom = top + 1; 


				if(left < 0 || left > (pBitmap->GetWidth()-1) || top < 0 || top > (pBitmap->GetHeight()-1))
				{
					colorByte[0]=0;
				}
				else
				{
                    //printf("1 top=[%d] left=[%d]", top, left);
					memcpy(&colorByte[0], pSourceColor + top * souceData.Stride + left * 4, 4);
                    //((char*)&colorByte[0])[0]= pSourceColor[top * souceData.Stride + left * 4];
                    //((char*)&colorByte[0])[1]= pSourceColor[top * souceData.Stride + left * 4 +1];
                    //((char*)&colorByte[0])[2]= pSourceColor[top * souceData.Stride + left * 4 +2];
                    //((char*)&colorByte[0])[3]= pSourceColor[top * souceData.Stride + left * 4 +3];
				}

				if(right < 0 || right > (pBitmap->GetWidth()-1) ||
					top < 0 || top > (pBitmap->GetHeight()-1))  //out off bound
				{
					colorByte[1]=0;
				}
				else
				{
					memcpy(&colorByte[1], pSourceColor + top * souceData.Stride + right * 4, 4);
                    //((char*)&colorByte[1])[0]= pSourceColor[ top * souceData.Stride + right * 4];
                    //((char*)&colorByte[1])[1]= pSourceColor[ top * souceData.Stride + right * 4+1];
                    //((char*)&colorByte[1])[2]= pSourceColor[ top * souceData.Stride + right * 4+2];
                    //((char*)&colorByte[1])[3]= pSourceColor[ top * souceData.Stride + right * 4+3];

				}
				if(right < 0 || right > (pBitmap->GetWidth()-1) || bottom < 0 ||
					bottom > (pBitmap->GetHeight()-1)) ////out off bound
				{
					colorByte[2]=0;
				}
				else
				{
					memcpy(&colorByte[2], pSourceColor + bottom * souceData.Stride + right * 4, 4);
                    //((char*)&colorByte[2])[0]= pSourceColor[ bottom * souceData.Stride + right * 4 ];
                    //((char*)&colorByte[2])[1]= pSourceColor[ bottom * souceData.Stride + right * 4 +1];
                    //((char*)&colorByte[2])[2]= pSourceColor[ bottom * souceData.Stride + right * 4 +2];
                    //((char*)&colorByte[2])[3]= pSourceColor[ bottom * souceData.Stride + right * 4 +3];

				}

				if( left < 0 || left > (pBitmap->GetWidth()-1) || 
					bottom < 0 || bottom > pBitmap->GetHeight()-1 )
				{
					colorByte[3]=0;
				}
				else
				{
					memcpy(&colorByte[3], pSourceColor + bottom * souceData.Stride + left * 4, 4);
                    //((char*)&colorByte[3])[0]= pSourceColor[ bottom * souceData.Stride + left * 4 ];
                    //((char*)&colorByte[3])[1]= pSourceColor[ bottom * souceData.Stride + left * 4 +1];
                    //((char*)&colorByte[3])[2]= pSourceColor[ bottom * souceData.Stride + left * 4 +2];
                    //((char*)&colorByte[3])[3]= pSourceColor[ bottom * souceData.Stride + left * 4 +3];

				}
				Gdiplus::Color color[4];
				color[0]=colorByte[0];
				color[1]=colorByte[1];
				color[2]=colorByte[2];
				color[3]=colorByte[3];
				double a1=alpha*beta;

				double red=  (1-alpha-beta + a1) * color[0].GetR() + 
					(alpha-a1) * color[1].GetR() +
					a1*color[2].GetR() +
					(beta-a1) * color[3].GetR();  
				double green=  (1-alpha-beta + a1) * color[0].GetG() + 
					(alpha-a1) * color[1].GetG() +
					a1*color[2].GetG() +
					(beta-a1) * color[3].GetG();  
				double blue=  (1-alpha-beta + a1) * color[0].GetB() + 
					(alpha-a1) * color[1].GetB() +
					a1*color[2].GetB() +
					(beta-a1) * color[3].GetB();  
				double trasparent=  (1-alpha-beta + a1) * color[0].GetA() + 
					(alpha-a1) * color[1].GetA() +
					a1*color[2].GetA() +
					(beta-a1) * color[3].GetA();  

				BYTE r=(BYTE)red;
				BYTE g=(BYTE)green;
				BYTE b=(BYTE)blue;
				BYTE t=(BYTE)trasparent;

				DWORD argb= RGB(b,g,r) | ( t << Gdiplus::Color::AlphaShift);

				memcpy(pDestColor + destOffset, &argb, 4);
				destOffset +=4;
			}
			pDestColor +=destData.Stride;
			if(callback)
			{
				if(callback(param, 1, row + 1)==false)
				{
					pBitmap->UnlockBits(&souceData);
					pDestBmp->UnlockBits(&destData);
					delete pDestBmp;
					return NULL;
				}
			}
		}
		pBitmap->UnlockBits(&souceData);
		pDestBmp->UnlockBits(&destData);
		return pDestBmp;
	}
	return NULL;
}

Gdiplus::Bitmap *CImageProcessor::ExpandTyreFrontImage( Gdiplus::Bitmap * pBitmap, const Gdiplus::Point *pt0, int area_height, void *param,  bool (*callback)(void *, long , long ))
{
	double r=(pt0[1].Y-pt0[0].Y) / 2.0;
	double a=asin(area_height / 2.0 / r);
	int width =(int)(2 * r * a);
	int height=pt0[1].X - pt0[0].X;

	Gdiplus::Bitmap * pNewBmp=new Gdiplus::Bitmap(width, height, PixelFormat32bppARGB);
	Gdiplus::BitmapData sourceData;
	Gdiplus::BitmapData targetData;
	Gdiplus::Rect rect;
	rect=Gdiplus::Rect(0,0, width, height);
	pNewBmp->LockBits(&rect, Gdiplus::ImageLockModeWrite | Gdiplus::ImageLockModeRead , PixelFormat32bppARGB, &targetData);
	rect=Gdiplus::Rect(0,0, pBitmap->GetWidth(), pBitmap->GetHeight());
	pBitmap->LockBits(&rect, Gdiplus::ImageLockModeRead , PixelFormat32bppARGB, &sourceData);
	int row, col;
	unsigned char *pSourceColor=(unsigned char *)sourceData.Scan0;
	unsigned char *pDestColor=(unsigned char *)targetData.Scan0;
	DWORD colorByte[4];

	double x, y;
	double sina;
	double alpha;
	double beta;
	int colOffset=0;
	int rowOffset=0;
	if(callback)
	{
		if(!callback(param, 0, width))
		{
			return NULL;
		}
	}
	for(col=0; col < width; col ++)
	{
		sina=sin((col - area_height/2.0)/r);
		rowOffset=0;
		for(row=0; row < height; row ++)
		{
			x= pt0[0].X + row;
			y= pt0[0].Y + r + r * sina;

			alpha=x - floor(x);
			beta=y-floor(y);

			int left   =(int) floor(x);
			int right  = left + 1;
			int top    =(int) floor(y);
			int bottom = top + 1; 

			if(left < 0 || left >(int)(pBitmap->GetWidth()-1) ||
				top < 0 || top > (int)(pBitmap->GetHeight()-1))
			{
				colorByte[0]=0;
			}
			else
			{
				memcpy(&colorByte[0], pSourceColor + top * sourceData.Stride + left * 4, 4);
			}

			if(right < 0 || right > (int)(pBitmap->GetWidth()-1) ||
				top < 0 || top > (int)(pBitmap->GetHeight()-1))  //out off bound
			{
				colorByte[1]=0;
			}
			else
			{
				memcpy(&colorByte[1], pSourceColor + top * sourceData.Stride + right * 4, 4);
			}
			if(right < 0 || right > (int)(pBitmap->GetWidth()-1) || 
				bottom < 0 || (int)(bottom > pBitmap->GetHeight()-1)) ////out off bound
			{
				colorByte[2]=0;
			}
			else
			{
				memcpy(&colorByte[2], pSourceColor + bottom * sourceData.Stride + right * 4, 4);
			}

			if( left < 0 || left > (int)(pBitmap->GetWidth()-1) ||
				bottom < 0 || bottom > (int)(pBitmap->GetHeight()-1) )
			{
				colorByte[3]=0;
			}
			else
			{
				memcpy(&colorByte[3], pSourceColor + bottom * sourceData.Stride + left * 4, 4);
			}
			Gdiplus::Color color[4];
			color[0]=colorByte[0];
			color[1]=colorByte[1];
			color[2]=colorByte[2];
			color[3]=colorByte[3];
			double a1=alpha*beta;

			double red=  (1-alpha-beta + a1) * color[0].GetR() + 
				(alpha-a1) * color[1].GetR() +
				a1*color[2].GetR() +
				(beta-a1) * color[3].GetR();  
			double green=  (1-alpha-beta + a1) * color[0].GetG() + 
				(alpha-a1) * color[1].GetG() +
				a1*color[2].GetG() +
				(beta-a1) * color[3].GetG();  
			double blue=  (1-alpha-beta + a1) * color[0].GetB() + 
				(alpha-a1) * color[1].GetB() +
				a1*color[2].GetB() +
				(beta-a1) * color[3].GetB();  
			double trasparent=  (1-alpha-beta + a1) * color[0].GetA() + 
				(alpha-a1) * color[1].GetA() +
				a1*color[2].GetA() +
				(beta-a1) * color[3].GetA();  

			BYTE r=(BYTE)red;
			BYTE g=(BYTE)green;
			BYTE b=(BYTE)blue;
			BYTE t=(BYTE)trasparent;

			DWORD argb= RGB(b,g,r) | ( t << Gdiplus::Color::AlphaShift);
			pDestColor=(unsigned char*)targetData.Scan0 + rowOffset + colOffset;
			memcpy(pDestColor, &argb, 4);
			rowOffset +=targetData.Stride;
		}
		colOffset +=4;
		if(callback)
		{
			if(callback(param, 1, col + 1)==false)
			{
				pBitmap->UnlockBits(&sourceData);
				pNewBmp->UnlockBits(&targetData);
				delete pNewBmp;
				return NULL;
			}
		}
	}
	pBitmap->UnlockBits(&sourceData);
	pNewBmp->UnlockBits(&targetData);
	return pNewBmp;
}

Gdiplus::Bitmap *CImageProcessor::ExpandTyreSideImage(  Gdiplus::Bitmap * pBitmap, const Gdiplus::Point *pt0, int inner, void *param,  bool (*callback)(void *, long , long ))
{
	int outter=(pt0[1].X-pt0[0].X)/2;

	int height=outter - inner;

	int width =(int)( 2 * outter * ROTATE_PI );
	Gdiplus::Bitmap * pNewBmp=new Gdiplus::Bitmap(width, height, PixelFormat32bppARGB);
    if(pNewBmp==NULL)
    {
        return NULL;
    }
	Gdiplus::BitmapData sourceData;
	Gdiplus::BitmapData targetData;
	Gdiplus::Rect rect;
	rect=Gdiplus::Rect(0,0, width, height);
    if(pNewBmp->LockBits(&rect, Gdiplus::ImageLockModeWrite | Gdiplus::ImageLockModeRead , PixelFormat32bppARGB, &targetData)!=Gdiplus::Ok)
        return NULL;
	rect=Gdiplus::Rect(0,0, pBitmap->GetWidth(), pBitmap->GetHeight());
    if(pBitmap->LockBits(&rect, Gdiplus::ImageLockModeRead , PixelFormat32bppARGB, &sourceData)!=Gdiplus::Ok)
    {
        pNewBmp->UnlockBits(&targetData);
        return NULL;
    }
	int row, col;
	unsigned char *pSourceColor=(unsigned char *)sourceData.Scan0;
	unsigned char *pDestColor=(unsigned char *)targetData.Scan0;

	DWORD colorByte[4];

	double r;
	double x, y;
	double cosa;
	double sina;
	double alpha;
	double beta;
	int colOffset=0;
	int rowOffset=0;
	if(callback)
	{
		if(!callback(param, 0, width))
		{
			return NULL;
		}
	}

	for(col=0; col < width; col ++)
	{
		cosa=cos(col * 1.0 / outter);
		sina=sin(col * 1.0 / outter);
		rowOffset=0;
		for(row=0; row < height; row ++)
		{
			r= inner + row;
			x=pt0[0].X + outter - r*sina;
            y=pt0[0].Y + outter - r*cosa;

			alpha=x - floor(x);
			beta =y - floor(y);
			//alpha channel

			int left   =(int) floor(x);
			int right  = left + 1;
			int top    =(int) floor(y);
			int bottom = top + 1; 


			if(left < 0 || left >(int)(pBitmap->GetWidth()-1) || 
				top < 0 || top > (int)(pBitmap->GetHeight()-1))
			{
				colorByte[0]=0;
			}
			else
			{
				memcpy(&colorByte[0], pSourceColor + top * sourceData.Stride + left * 4, 4);
			}

			if(right < 0 || right > (int)(pBitmap->GetWidth()-1) ||
				top < 0 || top > (int)(pBitmap->GetHeight()-1))  //out off bound
			{
				colorByte[1]=0;
			}
			else
			{
				memcpy(&colorByte[1], pSourceColor + top * sourceData.Stride + right * 4, 4);
			}
			if(right < 0 || right > (int)(pBitmap->GetWidth()-1) || 
				bottom < 0 || bottom > (int)(pBitmap->GetHeight()-1)) ////out off bound
			{
				colorByte[2]=0;
			}
			else
			{
				memcpy(&colorByte[2], pSourceColor + bottom * sourceData.Stride + right * 4, 4);
			}

			if( left < 0 || left > (int)(pBitmap->GetWidth()-1) || 
				bottom < 0 || bottom > (int)(pBitmap->GetHeight()-1) )
			{
				colorByte[3]=0;
			}
			else
			{
				memcpy(&colorByte[3], pSourceColor + bottom * sourceData.Stride + left * 4, 4);
			}
			Gdiplus::Color color[4];
			color[0]=colorByte[0];
			color[1]=colorByte[1];
			color[2]=colorByte[2];
			color[3]=colorByte[3];
			double a1=alpha*beta;

			double red=  (1-alpha-beta + a1) * color[0].GetR() + 
				(alpha-a1) * color[1].GetR() +
				a1*color[2].GetR() +
				(beta-a1) * color[3].GetR();  
			double green=  (1-alpha-beta + a1) * color[0].GetG() + 
				(alpha-a1) * color[1].GetG() +
				a1*color[2].GetG() +
				(beta-a1) * color[3].GetG();  
			double blue=  (1-alpha-beta + a1) * color[0].GetB() + 
				(alpha-a1) * color[1].GetB() +
				a1*color[2].GetB() +
				(beta-a1) * color[3].GetB();  
			double trasparent=  (1-alpha-beta + a1) * color[0].GetA() + 
				(alpha-a1) * color[1].GetA() +
				a1*color[2].GetA() +
				(beta-a1) * color[3].GetA();  

			BYTE r=(BYTE)red;
			BYTE g=(BYTE)green;
			BYTE b=(BYTE)blue;
			BYTE t=(BYTE)trasparent;

			DWORD argb= RGB(b,g,r) | ( t << Gdiplus::Color::AlphaShift);
			pDestColor=(unsigned char*)targetData.Scan0 + rowOffset + colOffset;
			memcpy(pDestColor, &argb, 4);
			rowOffset +=targetData.Stride;
		}
		colOffset +=4;
		if(callback)
		{
			if(callback(param, 1, col + 1)==false)
			{
				pBitmap->UnlockBits(&sourceData);
				pNewBmp->UnlockBits(&targetData);
				delete pNewBmp;
				return NULL;
			}
		}
	}
	pBitmap->UnlockBits(&sourceData);
	pNewBmp->UnlockBits(&targetData);
	return pNewBmp;
}

Gdiplus::Bitmap *CImageProcessor::ImageCut( Gdiplus::Bitmap * pBitmap, const Gdiplus::Point *pt0,int ptCount, void *param, bool (*callback)( void * , long , long))
{
	Gdiplus::GraphicsPath rgn;
	Gdiplus::Rect rect;
	rgn.AddPolygon(pt0, ptCount);
	
	rgn.GetBounds(&rect);
	Gdiplus::Matrix matrix;
	matrix.Translate((Gdiplus::REAL)(-rect.X), (Gdiplus::REAL)(-rect.Y));
	rgn.Transform(&matrix);
	Gdiplus::Bitmap *pDestBmp=NULL;

	CString message;

	if(pBitmap==NULL)
		return NULL;

	pDestBmp=new Gdiplus::Bitmap(rect.Width, rect.Height);
	Gdiplus::Graphics graph(pDestBmp);

	graph.SetClip(&rgn);
	Gdiplus::SolidBrush brush(Gdiplus::Color::Aqua);
	graph.FillRectangle(&brush, 0,0, rect.Width, rect.Height);
	graph.DrawImage(pBitmap, Gdiplus::Rect(0,0, rect.Width, rect.Height), rect.X, rect.Y, rect.Width, rect.Height, Gdiplus::UnitPixel);
	return pDestBmp;
}

Gdiplus::Bitmap * CImageProcessor::ImageCut( Gdiplus::Bitmap * pBitmap,const Gdiplus::GraphicsPath *pCutRegion )
{
	Gdiplus::GraphicsPath path;
	path.AddPath(pCutRegion,FALSE);
	Gdiplus::Rect rect;
	path.GetBounds(&rect);
	Gdiplus::Matrix matrix;
	matrix.Translate((Gdiplus::REAL)(-rect.X), (Gdiplus::REAL)(-rect.Y));
	path.Transform(&matrix);

	Gdiplus::Bitmap *pDestBmp=NULL;

	CString message;

	if(pBitmap==NULL)
		return NULL;

	pDestBmp=new Gdiplus::Bitmap(rect.Width, rect.Height);
	Gdiplus::Graphics graph(pDestBmp);

	graph.SetClip(&path);
	Gdiplus::SolidBrush brush(Gdiplus::Color::Aqua);
	graph.FillRectangle(&brush, 0,0, rect.Width, rect.Height);
	graph.DrawImage(pBitmap, Gdiplus::Rect(0,0, rect.Width, rect.Height), rect.X, rect.Y, rect.Width, rect.Height, Gdiplus::UnitPixel);
	return pDestBmp;

}
