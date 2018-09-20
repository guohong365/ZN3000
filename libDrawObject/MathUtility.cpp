#include "StdAfx.h"

#include <math.h>

#include "MathUtility.h"
#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif
#define MAX_LOOP  200000
using namespace Gdiplus;

static int rgauss(int n, double *a, double *b);

double CMathUtility::CalculateLineAngle(double x1, double y1, double x2, double y2)
{
    if (y1 == y2)
    {
        if (x1 <= x2)
        {
            return 0;
        }
        return 180;
    }
    if (x1 == x2)
    {
        if (y1 < y2)
        {
            return 90;
        }
        return -90;
    }
    double tanValue;
	tanValue = (y2-y1)/(x2-x1);

	//tanValue=tan( 30.0 / 180.0 * 3.14159265 ); //(y2 - y1) / (x2 - x1);

    double angle;
	//angle = atan(tanValue) * 180 / 3.14159265;
	angle= atan(tanValue);
	tanValue = tan(angle);
	angle=angle * 180 / 3.14159265;
    if (y2 > y1 && x2 < x1)
    {
        angle += 180;
    }
    if (y2 < y1 && x2 < x1)
    {
        angle -= 180;
    }
    return angle;
}

bool CMathUtility::CalculateCorrectParameter(const double *x0, const double *y0, const double *x1, const double *y1, double *k)
{
    bool quit = false;

    double a[1000] = { 0 };
/*
	b[0]=x0[0];
	b[1]=x0[1];
	b[2]=x0[2];
	b[3]=x0[3];
	b[4]=y0[0];
	b[5]=y0[1];
	b[6]=y0[2];
	b[7]=y0[3];

	matrix[ 0 * 8 + 0 ]=x1[0];
	matrix[ 0 * 8 + 1 ]=y1[0]; 
	matrix[ 0 * 8 + 2 ]=1;
	matrix[ 0 * 8 + 6 ]=-x0[0]*x1[0];	
	matrix[ 0 * 8 + 7 ]=-x0[0]*y1[0];
	matrix[ 1 * 8 + 0 ]=x1[1];
	matrix[ 1 * 8 + 1 ]=y1[1];	
	matrix[ 1 * 8 + 2 ]=1; 
	matrix[ 1 * 8 + 6 ]=-x0[1]*x1[1]; 
	matrix[ 1 * 8 + 7 ]=-x0[1]*y1[1];
	matrix[ 2 * 8 + 0 ]=x1[2]; 
	matrix[ 2 * 8 + 1 ]=y1[2];	
	matrix[ 2 * 8 + 2 ]=1; 	
	matrix[ 2 * 8 + 6 ]=-x0[2]*x1[2];
	matrix[ 2 * 8 + 7 ]=-x0[2]*y1[2];
	matrix[ 3 * 8 + 0 ]=x1[3]; 
	matrix[ 3 * 8 + 1 ]=y1[3]; 
	matrix[ 3 * 8 + 2 ]=1; 
	matrix[ 3 * 8 + 6 ]=-x0[3]*x1[3];	
	matrix[ 3 * 8 + 7 ]=-x0[3]*y1[3];

	matrix[ 4 * 8 + 3 ]=x1[0]; 	
	matrix[ 4 * 8 + 4 ]=y1[0]; 
	matrix[ 4 * 8 + 5 ]=1; 
	matrix[ 4 * 8 + 6 ]=-x1[0]*y0[0];
	matrix[ 4 * 8 + 7 ]=-y0[0]*y1[0];
	matrix[ 5 * 8 + 3 ]=x1[1]; 
	matrix[ 5 * 8 + 4 ]=y1[1];	
	matrix[ 5 * 8 + 5 ]=1; 
	matrix[ 5 * 8 + 6 ]=-x1[1]*y0[1];
	matrix[ 5 * 8 + 7 ]=-y0[1]*y1[1];
	matrix[ 6 * 8 + 3 ]=x1[2]; 
	matrix[ 6 * 8 + 4 ]=y1[2]; 
	matrix[ 6 * 8 + 5 ]=1; 
	matrix[ 6 * 8 + 6 ]=-x1[2]*y0[2];
	matrix[ 6 * 8 + 7 ]=-y0[2]*y1[2];
	matrix[ 7 * 8 + 3 ]=x1[3]; 
	matrix[ 7 * 8 + 4 ]=y1[3]; 	
	matrix[ 7 * 8 + 5 ]=1;
	matrix[ 7 * 8 + 6 ]=-x1[3]*y0[3]; 
	matrix[ 7 * 8 + 7 ]=-y0[3]*y1[3];
*/
    k[0] = x0[0];
    k[1] = x0[1];
    k[2] = x0[2];
    k[3] = x0[3];
    k[4] = y0[0];
    k[5] = y0[1];
    k[6] = y0[2];
    k[7] = y0[3];

    a[0 * 8 + 0] = x1[0];
    a[0 * 8 + 1] = y1[0];
    a[0 * 8 + 2] = 1;
    a[0 * 8 + 6] = -x0[0] * x1[0];
    a[0 * 8 + 7] = -x0[0] * y1[0];
    a[1 * 8 + 0] = x1[1];
    a[1 * 8 + 1] = y1[1];
    a[1 * 8 + 2] = 1;
    a[1 * 8 + 6] = -x0[1] * x1[1];
    a[1 * 8 + 7] = -x0[1] * y1[1];
    a[2 * 8 + 0] = x1[2];
    a[2 * 8 + 1] = y1[2];
    a[2 * 8 + 2] = 1;
    a[2 * 8 + 6] = -x0[2] * x1[2];
    a[2 * 8 + 7] = -x0[2] * y1[2];
    a[3 * 8 + 0] = x1[3];
    a[3 * 8 + 1] = y1[3];
    a[3 * 8 + 2] = 1;
    a[3 * 8 + 6] = -x0[3] * x1[3];
    a[3 * 8 + 7] = -x0[3] * y1[3];

    a[4 * 8 + 3] = x1[0];
    a[4 * 8 + 4] = y1[0];
    a[4 * 8 + 5] = 1;
    a[4 * 8 + 6] = -x1[0] * y0[0];
    a[4 * 8 + 7] = -y0[0] * y1[0];
    a[5 * 8 + 3] = x1[1];
    a[5 * 8 + 4] = y1[1];
    a[5 * 8 + 5] = 1;
    a[5 * 8 + 6] = -x1[1] * y0[1];
    a[5 * 8 + 7] = -y0[1] * y1[1];
    a[6 * 8 + 3] = x1[2];
    a[6 * 8 + 4] = y1[2];
    a[6 * 8 + 5] = 1;
    a[6 * 8 + 6] = -x1[2] * y0[2];
    a[6 * 8 + 7] = -y0[2] * y1[2];
    a[7 * 8 + 3] = x1[3];
    a[7 * 8 + 4] = y1[3];
    a[7 * 8 + 5] = 1;
    a[7 * 8 + 6] = -x1[3] * y0[3];
    a[7 * 8 + 7] = -y0[3] * y1[3];

    if (rgauss(8, a, k))
    {
        return true;
    }
    return false;
}

void CMathUtility::CalculatePrespectiveCoordinate(const double * k, double x0, double y0, double &x, double &y)
{
    x = (x0 * k[0] + y0 * k[1] + k[2])/(k[6] * x0 + k[7] * y0 + 1);
    y =	(x0 * k[3] + y0 * k[4] + k[5])/(k[6] * x0 + k[7] * y0 + 1);
}
void CMathUtility::CalculatePrespectiveCoordinate(const double * k, const double *x0, const double *y0, double * x, double *y, long count)
{
    for(long i=0; i< count ; i++)
    {
        x[i] =(x0[i] * k[0] + y0[i] * k[1] + k[2])/(k[6] * x0[i] + k[7] * y0[i] + 1);
        y[i] =(x0[i] * k[3] + y0[i] * k[4] + k[5])/(k[6] * x0[i] + k[7] * y0[i] + 1);
    }
}

int rgauss(int n, double *a, double *b)
{
    int *js, l, k, i, j, is, p, q;
    double d, t;
    js = new int[n];
    l = 1;
    for (k = 0; k <= n - 2; k++)
    {
        d = 0.0;
        for (i = k; i <= n - 1; i++)
        {
            for (j = k; j <= n - 1; j++)
            {
                t = fabs(a[i * n + j]);
                if (t > d)
                {
                    d = t;
                    js[k] = j;
                    is = i;
                }
            }
        }
        if (d + 1.0 == 1.0)
        {
            l = 0;
        }
        else
        {
            if (js[k] != k)
            {
                for (i = 0; i <= n - 1; i++)
                {
                    p = i * n + k;
                    q = i * n + js[k];
                    t = a[p];
                    a[p] = a[q];
                    a[q] = t;
                }
            }
            if (is != k)
            {
                for (j = k; j <= n - 1; j++)
                {
                    p = k * n + j;
                    q = is * n + j;
                    t = a[p];
                    a[p] = a[q];
                    a[q] = t;
                }
                t = b[k];
                b[k] = b[is];
                b[is] = t;
            }
        }
        if (l == 0)
        {
            delete[]js;
            return (0);
        }
        d = a[k * n + k];
        for (j = k + 1; j <= n - 1; j++)
        {
            p = k * n + j;
            a[p] = a[p] / d;
        }
        b[k] = b[k] / d;
        for (i = k + 1; i <= n - 1; i++)
        {
            for (j = k + 1; j <= n - 1; j++)
            {
                p = i * n + j;
                a[p] = a[p] - a[i * n + k] * a[k * n + j];
            }
            b[i] = b[i] - a[i * n + k] * b[k];
        }
    }
    d = a[(n - 1) * n + n - 1];
    if (fabs(d) + 1.0 == 1.0)
    {
        delete[]js;
        return (0);
    }

    b[n - 1] = b[n - 1] / d;
    for (i = n - 2; i >= 0; i--)
    {
        t = 0.0;
        for (j = i + 1; j <= n - 1; j++)
        {
            t = t + a[i * n + j] * b[j];
        }
        b[i] = b[i] - t;
    }
    js[n - 1] = n - 1;
    for (k = n - 1; k >= 0; k--)
    {
        if (js[k] != k)
        {
            t = b[k];
            b[k] = b[js[k]];
            b[js[k]] = t;
        }
    }
    delete[]js;
    return (1);
}


double DistanceP2P(Point from, Point to)
{
	Point pt=from-to;
	return sqrt((double)pt.X*pt.X + pt.Y*pt.Y);
}
double DistanceP2HL(Point from, Point p1, Point p2)
{
	
	if((from.X > p1.X && from.X < p2.X) ||
		(from.X < p1.X && from.X > p2.X))
	{
		return abs(from.Y-p1.Y);
	}
	return min(abs(from.X-p1.X),abs(from.X-p2.X)) + abs(from.Y-p1.Y);
}
double DistanceP2VL(Point from, Point p1, Point p2)
{
	if((from.Y > p1.Y && from.Y < p2.Y) ||
		(from.Y < p1.Y && from.Y > p2.Y))
	{
		return abs(from.X-p1.X);
	}
	return min(abs(from.Y-p1.Y),abs(from.Y-p2.Y)) + abs(from.X-p1.X);
}
float GetRatio(Size source, Size target)
{
	float w=(float)source.Width/target.Width;
	float h=(float)source.Height/target.Height;
	
	return w>h? h:w;
}
float GetRatio(SizeF source, SizeF target)
{
	float w=(float)source.Width/target.Width;
	float h=(float)source.Height/target.Height;

	return w>h? h:w;

}
