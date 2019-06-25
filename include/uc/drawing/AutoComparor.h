#pragma once
#include "polygonobject.h"

		class CAutoComparor:public CPolygonObject
		{
		protected:
			DECLARE_SERIAL(CAutoComparor) CAutoComparor(void);
		public:
			virtual ~ CAutoComparor(void);

			CAutoComparor(const Gdiplus::Point pt);
			virtual void DrawMark(Gdiplus::Graphics & graph);
			virtual void OnDraw(Gdiplus::Graphics & graph);
		};
