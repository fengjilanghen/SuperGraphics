#ifndef _DRAW_H_
#define _DRAW_H_

#include "Texture.h"
#include <cmath>
#include <vector>
#include <cassert>
#include <functional>
#include <Windows.h>
#include <fstream>

namespace npWin32Draw
{
	enum eEdgeType
	{
		EET_LEFT=0,
		EET_RIGHT,
		EET_COUNT
	};

	struct SPosition
	{
		int x, y;
		SPosition() :x(0), y(0){}
		SPosition(int x, int y) :x(x), y(y){}
	};

	struct SVertex3D
	{
		int x, y;
		float z;
		tagRGBQUAD color;
		float s, t;
		SVertex3D() :x(0), y(0), z(1.0f), color({255, 255, 255}), s(0), t(0){}
		SVertex3D(int x, int y, float z, float tx, float ty, tagRGBQUAD color) :x(x), y(y), z(z), color(color), s(tx), t(ty){}
	};

	typedef std::function<void(const SPosition&, const SPosition&, COLORREF)>				DrawLineFunction;
	typedef std::function<void(const SPosition&, const SPosition&, COLORREF, COLORREF)>	DrawLineFunction2;

	typedef std::function<void(eEdgeType, const SVertex3D &)> fAddIntersection;

	//color dot
	inline tagRGBTRIPLE Dot(const tagRGBTRIPLE& c1, const tagRGBTRIPLE& c2)
	{
		return{ (c1.rgbtBlue*c2.rgbtBlue) >> 8, (c1.rgbtGreen*c2.rgbtGreen) >> 8, (c1.rgbtRed*c2.rgbtRed) >> 8 };
	}
	inline tagRGBQUAD Dot(const tagRGBQUAD& c1, const tagRGBTRIPLE& c2)
	{
		return{ (c1.rgbBlue *c2.rgbtBlue) >> 8, (c1.rgbGreen*c2.rgbtGreen) >> 8, (c1.rgbRed*c2.rgbtRed) >> 8 };
	}

	//normal draw
	void DdaLine(const SPosition& pt1, const SPosition& pt2, COLORREF color = RGB(0, 0, 0));
	void BresenhamLine(const SPosition& pt1, const SPosition& pt2, COLORREF color = RGB(0, 0, 0)); 
	void Triangle(const SPosition& pt1, const SPosition& pt2, const SPosition& pt3, COLORREF color = RGB(0, 0, 0), DrawLineFunction = DdaLine);
	// draw with gouraud
	void BresenhamLine2(const SPosition& pt1, const SPosition& pt2, COLORREF color1, COLORREF color2);
	void DdaLine2(const SPosition& pt1, const SPosition& pt2, COLORREF color1, COLORREF color2);
	void Triangle2(const SPosition& pt1, const SPosition& pt2, const SPosition &pt3, COLORREF color1, COLORREF color2, COLORREF color3, DrawLineFunction2 = DdaLine2);
	//AA
	void DdaLineAA(const SPosition& pt1, const SPosition& pt2, COLORREF color1, COLORREF color2);
}

#endif //_DRAW_H_