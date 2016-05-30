#include "global_data.h"
#include "Win32Draw.h"
#include <algorithm>
void npWin32Draw::BresenhamLine(const SPosition& pt1, const SPosition& pt2, COLORREF color)
{
	int dx = pt2.x - pt1.x;
	int dy = pt2.y - pt1.y;
	int absdx = abs(dx);
	int absdy = abs(dy);

	if (dx == 0 || dy == 0 || absdx == absdy)
	{
		int xIncrement = dx == 0 ? 0 : dx / absdx;
		int yIncrement = dy == 0 ? 0 : dy / absdy;
		int x = pt1.x;
		int y = pt1.y;

		while (x != pt2.x || y != pt2.y)
		{
			//SetPixelV (x, y, color);
			testAndSetPixel(x, y, 0.0f, { GetBValue(color), GetGValue(color), GetRValue(color) });
			x += xIncrement;
			y += yIncrement;
		}
		return;
	}

	if (absdx > absdy)
	{
		SPosition point1 = pt1;
		SPosition point2 = pt2;
		if (point1.x > point2.x)
		{
			std::swap(point1, point2);
			dx = -dx;
			dy = -dy;
		}

		int twoabsdy = absdy * 2;
		int twoabsdyminusdx = 2 * (absdy - dx);
		int yIncrement = dy / absdy;
		int pk = twoabsdy - dx;
		int x = point1.x; 
		int y = point1.y;
		testAndSetPixel(x, y, 0.0f, { GetBValue(color), GetGValue(color), GetRValue(color) });
		while (x <= point2.x)
		{
			if (pk >= 0)
			{
				pk += twoabsdyminusdx;
				y += yIncrement;
			}
			else
				pk += twoabsdy;
			testAndSetPixel(++x, y, 0.0f, { GetBValue(color), GetGValue(color), GetRValue(color) });
		}
		return;
	}
	else if (absdx < absdy)
	{
		SPosition point1 = pt1;
		SPosition point2 = pt2;
		if (point1.y > point2.y)
		{
			std::swap(point1, point2);
			dx = -dx;
			dy = -dy;
		}

		int twoabsdx = absdx * 2;
		int twoabsdxminusdy = 2 * (absdx - dy);
		int xIncrement = dx / absdx;
		int pk = twoabsdx - dy;
		int x = point1.x;
		int y = point1.y;
		testAndSetPixel(x, y, 0.0f, { GetBValue(color), GetGValue(color), GetRValue(color) });
		while (y <= point2.y)
		{
			if (pk >= 0)
			{
				pk += twoabsdxminusdy;
				x += xIncrement;
			}
			else
				pk += twoabsdx;
			testAndSetPixel(x, ++y, 0.0f, { GetBValue(color), GetGValue(color), GetRValue(color) });
		}
		return;
	}
}

void npWin32Draw::BresenhamLine2(const SPosition& pt1, const SPosition& pt2, COLORREF color1, COLORREF color2)
{
	int dx = pt2.x - pt1.x;
	int dy = pt2.y - pt1.y;
	int absdx = abs(dx);
	int absdy = abs(dy);

	float ir = static_cast<float>(GetRValue(color1));
	float ig = static_cast<float>(GetGValue(color1));
	float ib = static_cast<float>(GetBValue(color1));
	float ir2 = static_cast<float>(GetRValue(color2));
	float ig2 = static_cast<float>(GetGValue(color2));
	float ib2 = static_cast<float>(GetBValue(color2));

	if (dx == 0 || dy == 0 || absdx == absdy)
	{
		int xIncrement = dx == 0 ? 0 : dx / absdx;
		int yIncrement = dy == 0 ? 0 : dy / absdy;
		int x = pt1.x;
		int y = pt1.y;

		float absd = xIncrement == 0 ? absdy : absdx;
		float dir = (ir2 - ir) / absd;
		float dig = (ig2 - ig) / absd;
		float dib = (ib2 - ib) / absd;
		
		while (x != pt2.x || y != pt2.y)
		{
			testAndSetPixel(x, y, 0.0f, { static_cast<BYTE>(ib), static_cast<BYTE> (ig), static_cast<BYTE>(ir) });
			ir += dir;
			ig += dig;
			ib += dib;
			x += xIncrement;
			y += yIncrement;
		}
		return;
	}

	if (absdx > absdy)
	{
		SPosition point1 = pt1;
		SPosition point2 = pt2;
		if (point1.x > point2.x)
		{
			std::swap(point1, point2);
			std::swap(ir, ir2);
			std::swap(ig, ig2);
			std::swap(ib, ib2);
			dx = -dx;
			dy = -dy;
		}

		int twoabsdy = absdy * 2;
		int twoabsdyminusdx = 2 * (absdy - dx);
		int yIncrement = dy / absdy;
		int pk = twoabsdy - dx;
		int x = point1.x;
		int y = point1.y;

		float dir = (ir2 - ir) / absdx;
		float dig = (ig2 - ig) / absdx;
		float dib = (ib2 - ib) / absdx;
		testAndSetPixel(x, y, 0.0f, { static_cast<BYTE>(ib), static_cast<BYTE> (ig), static_cast<BYTE>(ir) });
		while (x < point2.x)
		{
			if (pk >= 0)
			{
				pk += twoabsdyminusdx;
				y += yIncrement;
			}
			else
				pk += twoabsdy;

			ir += dir;
			ig += dig;
			ib += dib;
			testAndSetPixel(++x, y, 0.0f, { static_cast<BYTE>(ib), static_cast<BYTE> (ig), static_cast<BYTE>(ir) });
		}
		testAndSetPixel(++x, y, 0.0f, { static_cast<BYTE>(ib2), static_cast<BYTE> (ig2), static_cast<BYTE>(ir2) });
		return;
	}
	else if (absdx < absdy)
	{
		SPosition point1 = pt1;
		SPosition point2 = pt2;
		if (point1.y > point2.y)
		{
			std::swap(point1, point2);
			std::swap(ir, ir2);
			std::swap(ig, ig2);
			std::swap(ib, ib2);
			dx = -dx;
			dy = -dy;
		}

		int twoabsdx = absdx * 2;
		int twoabsdxminusdy = 2 * (absdx - dy);
		int xIncrement = dx / absdx;
		int pk = twoabsdx - dy;
		int x = point1.x;
		int y = point1.y;		
		
		float dir = (ir2 - ir) / absdy;
		float dig = (ig2 - ig) / absdy;
		float dib = (ib2 - ib) / absdy;
		testAndSetPixel(x, y, 0.0f, { static_cast<BYTE>(ib), static_cast<BYTE> (ig), static_cast<BYTE>(ir) });
		while (y < point2.y)
		{
			if (pk >= 0)
			{
				pk += twoabsdxminusdy;
				x += xIncrement;
			}
			else
				pk += twoabsdx;
			
			ir += dir;
			ig += dig;
			ib += dib;
			testAndSetPixel(x, ++y, 0.0f, { static_cast<BYTE>(ib), static_cast<BYTE> (ig), static_cast<BYTE>(ir) });
		}
		testAndSetPixel(x, ++y, 0.0f, { static_cast<BYTE>(ib2), static_cast<BYTE>(ig2), static_cast<BYTE>(ir2) });
		return;
	}
}

void npWin32Draw::DdaLine(const SPosition& pt1, const SPosition& pt2, COLORREF color)
{
	int dx = pt2.x - pt1.x;
	int absdx = abs(dx);
	int dy = pt2.y - pt1.y;
	int absdy = abs(dy);
	float x = static_cast<float>(pt1.x);
	float y = static_cast<float>(pt1.y);
	float xIncrement = 0;
	float yIncrement = 0;
	int steps = 1;

	if (absdx > absdy)
		steps = absdx;
	else
		steps = absdy;

	xIncrement = static_cast<float>(dx) / static_cast<float>(steps);
	yIncrement = static_cast<float>(dy) / float(steps);

	for (int i = 0; i <= steps; ++i)
	{
		testAndSetPixel(static_cast<int>(x + 0.5f), static_cast<int>(y + 0.5f), 0.0f, { GetBValue(color), GetGValue(color), GetRValue(color) });
		x += xIncrement;
		y += yIncrement;
	}
}


void npWin32Draw::DdaLine2(const SPosition& pt1, const SPosition& pt2, COLORREF color1, COLORREF color2)
{
	int dx = pt2.x - pt1.x;
	int absdx = abs(dx);
	int dy = pt2.y - pt1.y;
	int absdy = abs(dy);
	float x = static_cast<float>(pt1.x);
	float y = static_cast<float>(pt1.y);
	float xIncrement = 0;
	float yIncrement = 0;
	int steps = 1;
	
	if (absdx > absdy)
		steps = absdx;
	else
		steps = absdy;
	
	xIncrement = static_cast<float>(dx) / static_cast<float>(steps);
	yIncrement = static_cast<float>(dy) / static_cast<float>(steps);
	
	float ir = static_cast<float>(GetRValue(color1));
	float ig = static_cast<float>(GetGValue(color1));
	float ib = static_cast<float>(GetBValue(color1));
	float ir2 = static_cast<float>(GetRValue(color2));
	float ig2 = static_cast<float>(GetGValue(color2));
	float ib2 = static_cast<float>(GetBValue(color2));
	float dir = (ir2 - ir) / steps;
	float dig = (ig2 - ig) / steps;
	float dib = (ib2 - ib) / steps;

	int ypixel = pt1.y;
	
	for (int i = 0; i <= steps; ++i)
	{
		testAndSetPixel(static_cast<int>(x), static_cast<int>(y), 0.0f, { static_cast<BYTE>(ib), static_cast<BYTE> (ig), static_cast<BYTE>(ir) });
		ir += dir;
		ig += dig;
		ib += dib;

		x += xIncrement;
		y += yIncrement;
	}
}

void npWin32Draw::DdaLineAA(const SPosition& pt1, const SPosition& pt2, COLORREF color1, COLORREF color2)
{
	int dx = pt2.x - pt1.x;
	int absdx = abs(dx);
	int dy = pt2.y - pt1.y;
	int absdy = abs(dy);
	float x = static_cast<float>(pt1.x);
	float y = static_cast<float>(pt1.y);
	float xIncrement = 0;
	float yIncrement = 0;
	int steps = 1;

	if (absdx > absdy)
		steps = absdx;
	else
		steps = absdy;

	xIncrement = static_cast<float>(dx) / static_cast<float>(steps);
	yIncrement = static_cast<float>(dy) / static_cast<float>(steps);

	float ir = static_cast<float>(GetRValue(color1));
	float ig = static_cast<float>(GetGValue(color1));
	float ib = static_cast<float>(GetBValue(color1));
	float ir2 = static_cast<float>(GetRValue(color2));
	float ig2 = static_cast<float>(GetGValue(color2));
	float ib2 = static_cast<float>(GetBValue(color2));
	float dir = (ir2 - ir) / steps;
	float dig = (ig2 - ig) / steps;
	float dib = (ib2 - ib) / steps;

	int ypixel = pt1.y;


	if (absdx > absdy)
	{
		for (int i = 0; i <= steps; ++i)
		{
			int y_upper = static_cast<int>(round(y + 0.5f));
			float lower = y_upper - y;
			float upper = 1 - lower;
			
			int nx = static_cast<int>(x);
			testAndSetPixel(nx, y_upper, 0.0f, { static_cast<BYTE>(ib*upper), static_cast<BYTE>(ig*upper), static_cast<BYTE>(ir*upper) });
			if (y_upper>=1) testAndSetPixel(nx, y_upper - 1, 0.0f, { static_cast<BYTE>(ib*lower), static_cast<BYTE>(ig*lower), static_cast<BYTE>(ir*lower) });
			
			ir += dir;
			ig += dig;
			ib += dib;
			x += xIncrement;
			y += yIncrement;
		}
	}
	else
	{
		for (int i = 0; i <= steps; ++i)
		{
			int x_right = static_cast<int>(round(x + 0.5f));
			float left = x_right - x;
			float right = 1-left;
			
			int ny = static_cast<int>(y);
			testAndSetPixel(x_right, ny, 0.0f, { static_cast<BYTE>(ib*right), static_cast<BYTE>(ig*right), static_cast<BYTE>(ir*right) });
			if (x_right >= 1) testAndSetPixel(x_right - 1, ny, 0.0f, { static_cast<BYTE>(ib*left), static_cast<BYTE>(ig*left), static_cast<BYTE>(ir*left) });
			
			ir += dir;
			ig += dig;
			ib += dib;
			x += xIncrement;
			y += yIncrement;
		}
	}
}

void npWin32Draw::Triangle(const SPosition& pt1, const SPosition& pt2, const SPosition &pt3, COLORREF color, DrawLineFunction line)
{
	line(pt1, pt2, color);
	line(pt2, pt3, color);
	line(pt3, pt1, color);
}


void npWin32Draw::Triangle2(const SPosition& pt1, const SPosition& pt2, const SPosition &pt3, COLORREF color1, COLORREF color2, COLORREF color3, DrawLineFunction2 line2)
{
	line2(pt1, pt2, color1, color2);
	line2(pt2, pt3, color2, color3);
	line2(pt3, pt1, color3, color1);
}
