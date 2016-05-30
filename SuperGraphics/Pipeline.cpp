#include "Pipeline.h"
#include "global_data.h"

using namespace DiyMath3D;
using namespace npWin32Draw;

void SPipeline::ZClipTriangle(CClipCircleQueue & ccqPts, float zNear)
{
	ccqPts.push(*ccqPts.front());
	CVertexf *p0 = nullptr;
	CVertexf *p1 = nullptr;
	p0 = ccqPts.front();
	int size = ccqPts.size();
	for (int i = 0; i < size - 1; ++i)
	{
		p1 = ccqPts.second();
		ClipAgainstNearPlane(ccqPts, p0, p1, zNear);
		ccqPts.pop();
		p0 = p1;
	}
	ccqPts.pop();

	if (ccqPts.size() == 0)
		return;
	if (ccqPts.size() == 1)
		return;
	if (ccqPts.size() == 2)
		return;

	ConstructTriangleInZClip(ccqPts);
}

void SPipeline::ClipAgainstNearPlane(CClipCircleQueue &ccqPts, const CVertexf *p0, const CVertexf *p1, float zNear)
{
	bool e0 = p0->m_Position._z < zNear;
	bool e1 = p1->m_Position._z < zNear;
	float u = 0.0f;

	if (e0 && e1)	//both outside
		return;
	else if (!e0 && !e1)	//both inside
	{
		ccqPts.push(*p1);
		return;
	}

	u = (zNear - p0->m_Position._z) / (p1->m_Position._z - p0->m_Position._z);
	CVertexf vt;
	vt.m_Position = p0->m_Position + (p1->m_Position - p0->m_Position)*u;
	vt.m_TexCoords = p0->m_TexCoords + (p1->m_TexCoords - p0->m_TexCoords)*u;
	vt.m_Color = p0->m_Color + (p1->m_Color - p0->m_Color)*u;
	ccqPts.push(vt);

	if (e0)	//outside->inside
		ccqPts.push(*p1);
}


void SPipeline::ConstructTriangleInZClip(CClipCircleQueue &ccq)
{
	//construct triangle as fan
	CVertexf v0;
	v0 = *ccq.front();
	v0.m_Position = PerspectiveDevide(v0.m_Position);
	ccq.pop();

	CVertexf *pv1, *pv2;
	pv1 = ccq.front(); 
	pv1->m_Position = PerspectiveDevide(pv1->m_Position);
	
	//store new construct triangle
	CClipCircleQueue ccqNewTriangle;

	int size = ccq.size();
	for (int n = 0; n < size - 1; ++n, ccq.pop(), pv1 = pv2)
	{
		pv2 = ccq.second();
		pv2->m_Position = PerspectiveDevide(pv2->m_Position);

		//cull back
		if (CullBack(v0.m_Position, pv1->m_Position, pv2->m_Position))
			continue;

		ccqNewTriangle.clear();
		ccqNewTriangle.push(v0);
		ccqNewTriangle.push(*pv1);
		ccqNewTriangle.push(*pv2);

		//clipping
		ClipTriangleInNDC(ccqNewTriangle);

		if (ccqNewTriangle.size() == 0)
			return;

		//construct triangle and draw them
		ConstructTriangleInNDC(ccqNewTriangle);
	}
	ccq.clear();
}

//in ndc
bool SPipeline::CullBack(const CVector4f &pos0, const CVector4f &pos1, const CVector4f &pos2)
{
	CVector3f vec1(pos1 - pos0);
	CVector3f vec2(pos2 - pos1);
	CVector3f normal = CrossProduct(vec1, vec2);
	return normal._z >= 0;
}


void SPipeline::ClipTriangleInNDC(CClipCircleQueue &cq)
{
	for (int type = ECWET_LEFT; type < ECWET_COUNT; ++type)
	{
		cq.push(*cq.front());
		CVertexf *p0 = nullptr;
		CVertexf *p1 = nullptr;
		p0 = cq.front();
		int size = cq.size();
		for (int i = 0; i < size-1; ++i)
		{
			p1 = cq.second();
			
			ClipEdgeAgainstClipWindow(cq, *p0, *p1, static_cast<eClipWindowEdgeType>(type));

			cq.pop();
			p0 = p1;
		}
		cq.pop();
		if (cq.size() == 0)
			return;
	}
}

void SPipeline::ClipEdgeAgainstClipWindow(CClipCircleQueue &cq, const CVertexf &v0, const CVertexf &v1, eClipWindowEdgeType type)
{
	float x0 = v0.m_Position._x;
	float y0 = v0.m_Position._y;
	float z0 = v0.m_Position._z;
	float oneoverpz0 = g_Pipeline.getOneOverZ(z0), oneoverz1 = g_Pipeline.getOneOverZ(v1.m_Position._z);
	float soverz0 = v0.m_TexCoords._x*oneoverpz0;
	float toverz0 = v0.m_TexCoords._y*oneoverpz0;
	float roverz0 = v0.m_Color._x*oneoverpz0;
	float goverz0 = v0.m_Color._y*oneoverpz0;
	float boverz0 = v0.m_Color._z*oneoverpz0;

	float dx = v1.m_Position._x - v0.m_Position._x;
	float dy = v1.m_Position._y - v0.m_Position._y;
	float dz = v1.m_Position._z - v0.m_Position._z;
	float dsovez = (v1.m_TexCoords._x*oneoverz1 - v0.m_TexCoords._x*oneoverpz0);
	float dtovez = (v1.m_TexCoords._y*oneoverz1 - v0.m_TexCoords._y*oneoverpz0);
	float drovez = (v1.m_Color._x*oneoverz1 - v0.m_Color._x*oneoverpz0);
	float dgovez = (v1.m_Color._y*oneoverz1 - v0.m_Color._y*oneoverpz0);
	float dbovez = (v1.m_Color._z*oneoverz1 - v0.m_Color._z*oneoverpz0);
	float u =0.0f;
	bool e0 = true, e1 = true;

	switch (type)
	{
	case ECWET_LEFT:
		{
			float xmin = -1.0f;
			e0 = v0.m_Position._x < xmin;
			e1 = v1.m_Position._x < xmin;
			u = (xmin - v0.m_Position._x) / dx;
		}
		break;
	case ECWET_RIGHT:
		{
			float xmax = 1.0f;
			e0 = v0.m_Position._x > xmax;
			e1 = v1.m_Position._x > xmax;
			u = (xmax - v0.m_Position._x) / dx;
		}
		break;
	case ECWET_UP:
		{
			float ymax = 1.0f;
			e0 = v0.m_Position._y > ymax;
			e1 = v1.m_Position._y > ymax;
			u = (ymax - v0.m_Position._y) / dy;
		}
		break;
	case ECWET_DOWN:
		{
			float ymin = -1.0f;
			e0 = v0.m_Position._y < ymin;
			e1 = v1.m_Position._y < ymin;
			u = (ymin - v0.m_Position._y) / dy;
		}
		break;
	}

	if (e0 && e1)	//both outside
		return;
	else if (!e0 && !e1)	//both inside
	{
		cq.push(v1);
		return;
	}

	//query intersection
	CVertexf vt;
	vt.m_Position._x = x0 + u*dx;
	vt.m_Position._y = y0 + u*dy;
	vt.m_Position._z = z0 + u*dz;
	float pz = g_Pipeline.getZ(vt.m_Position._z);
	vt.m_TexCoords._x = (soverz0 + u*dsovez) * pz;
	vt.m_TexCoords._y = (toverz0 + u*dtovez) * pz;
	vt.m_Color._x = (roverz0 + u*drovez) * pz;
	vt.m_Color._y = (goverz0 + u*dgovez) * pz;
	vt.m_Color._z = (boverz0 + u*dbovez) * pz;
	cq.push(vt);

	if (e0)	//outside->inner
		cq.push(v1);
}


void SPipeline::ConstructTriangleInNDC(CClipCircleQueue &ccq)
{
	//construct triangle as fan
	CVertexf v0;
	CVertexf *pv1, *pv2;
	v0 = *ccq.front();
	v0.m_Position = CVector4f(v0.m_Position, 1.0f)*g_Pipeline.viewport;
	ccq.pop();
	
	pv1 = ccq.front();
	pv1->m_Position = CVector4f(pv1->m_Position, 1.0f)*g_Pipeline.viewport;

	int num = ccq.size();
	for (int n = 0; n < num - 1; ++n)
	{
		pv2 = ccq.second();

		//viewport transform
		pv2->m_Position = CVector4f(pv2->m_Position, 1.0f)*g_Pipeline.viewport;

		//draw
		DrawTriangle(&v0, pv1, pv2);

		pv1 = pv2;
		ccq.pop();
	}
}


void SPipeline::DrawTriangle(const CVertexf *p0, const CVertexf *p1, const CVertexf *p2)
{
	const CVector3f &vec1 = p0->m_Position;
	const CVector3f &vec2 = p1->m_Position;
	const CVector3f &vec3 = p2->m_Position;
	const CVector3f &clr1 = p0->m_Color;
	const CVector3f &clr2 = p1->m_Color;
	const CVector3f &clr3 = p2->m_Color;
	const CVector2f tex1 = p0->m_TexCoords;
	const CVector2f tex2 = p1->m_TexCoords;
	const CVector2f tex3 = p2->m_TexCoords;

	if (drawType == EDT_FRAME)
	{
		SPosition pt1(static_cast<int>(vec1._x), static_cast<int>(vec1._y));
		SPosition pt2(static_cast<int>(vec2._x), static_cast<int>(vec2._y));
		SPosition pt3(static_cast<int>(vec3._x), static_cast<int>(vec3._y));

		COLORREF color1 = RGB(static_cast<BYTE>(clr1._x * 255), static_cast<BYTE>(clr1._y * 255), static_cast<BYTE>(clr1._z * 255));
		COLORREF color2 = RGB(static_cast<BYTE>(clr2._x * 255), static_cast<BYTE>(clr2._y * 255), static_cast<BYTE>(clr2._z * 255));
		COLORREF color3 = RGB(static_cast<BYTE>(clr3._x * 255), static_cast<BYTE>(clr3._y * 255), static_cast<BYTE>(clr3._z * 255));

		DrawLineFunction2 func = DdaLine2;
		switch (lineMode)
		{
		case ELM_DDA:
			func = DdaLine2;
			break;
		case ELM_BRESENHAM:
			func = BresenhamLine2;
			break;
		case ELM_AA:
			func = DdaLineAA;
			break;
		}
		Triangle2(pt1, pt2, pt3, color1, color2, color3, func);
	}
	else
	{
		SVertex3D v1(static_cast<int>(vec1._x),
			static_cast<int>(vec1._y),
			vec1._z,
			tex1._x, tex1._y,
			{ static_cast<BYTE>(clr1._z * 255), static_cast<BYTE>(clr1._y * 255), static_cast<BYTE>(clr1._x * 255) });

		SVertex3D v2(static_cast<int>(vec2._x),
			static_cast<int>(vec2._y),
			vec2._z,
			tex2._x, tex2._y,
			{ static_cast<BYTE>(clr2._z * 255), static_cast<BYTE>(clr2._y * 255), static_cast<BYTE>(clr2._x * 255) });

		SVertex3D v3(static_cast<int>(vec3._x),
			static_cast<int>(vec3._y),
			vec3._z,
			tex3._x, tex3._y,
			{ static_cast<BYTE>(clr3._z * 255), static_cast<BYTE>(clr3._y * 255), static_cast<BYTE>(clr3._x * 255) });


		//triangle plane is Ax+By+Cz+D=0, ac=A/C, bc=B/C 
		if (drawType == EDT_TEXTURE_MAPPING)
			ScanTriangle(v1, v2, v3);
		else if (drawType == EDT_PERSPECTIVE_CORRECTION)
			ScanTriangle_c(v1, v2, v3);
	}
}

void SPipeline::ScanTriangle(const SVertex3D &v0, const SVertex3D &v1, const SVertex3D &v2)
{
	const SVertex3D *pv[3] = { &v0, &v1, &v2 };
	// 1.sort vertices with y in ascend
	for (int i = 0; i < 2; ++i)
	{
		for (int j = 0; j < 2 - i; ++j)
		if (pv[j]->y>pv[j + 1]->y)
			std::swap(pv[j], pv[j + 1]);
	}

	//right
	float xr = static_cast<float>(pv[0]->x);
	float oneoverzr = pv[0]->z;
	float br = static_cast<float>(pv[0]->color.rgbBlue);
	float gr = static_cast<float>(pv[0]->color.rgbGreen);
	float rr = static_cast<float>(pv[0]->color.rgbRed);
	float sr = pv[0]->s;
	float tr = pv[0]->t;

	int dyr = pv[2]->y - pv[0]->y;
	float xir = static_cast<float>(pv[2]->x - pv[0]->x) / dyr;
	float oneoverzir = (pv[2]->z - pv[0]->z) / dyr;
	float bir = static_cast<float>(pv[2]->color.rgbBlue - pv[0]->color.rgbBlue) / dyr;
	float gir = static_cast<float>(pv[2]->color.rgbGreen - pv[0]->color.rgbGreen) / dyr;
	float rir = static_cast<float>(pv[2]->color.rgbRed - pv[0]->color.rgbRed) / dyr;
	float sir = static_cast<float>(pv[2]->s - pv[0]->s) / dyr;
	float tir = static_cast<float>(pv[2]->t - pv[0]->t) / dyr;

	int dyl = pv[1]->y - pv[0]->y;
	//2.scan edge p0p1 and p0p2
	if (dyl > 0)
	{
		//left
		float xl = static_cast<float>(pv[0]->x);
		float oneoverzl = pv[0]->z;
		float bl = static_cast<float>(pv[0]->color.rgbBlue);
		float gl = static_cast<float>(pv[0]->color.rgbGreen);
		float rl = static_cast<float>(pv[0]->color.rgbRed);
		float sl = pv[0]->s;
		float tl = pv[0]->t;

		float xil = static_cast<float>(pv[1]->x - pv[0]->x) / dyl;
		float oneoverzil = (pv[1]->z - pv[0]->z) / dyl;
		float bil = static_cast<float>(pv[1]->color.rgbBlue - pv[0]->color.rgbBlue) / dyl;
		float gil = static_cast<float>(pv[1]->color.rgbGreen - pv[0]->color.rgbGreen) / dyl;
		float ril = static_cast<float>(pv[1]->color.rgbRed - pv[0]->color.rgbRed) / dyl;
		float sil = static_cast<float>(pv[1]->s - pv[0]->s) / dyl;
		float til = static_cast<float>(pv[1]->t - pv[0]->t) / dyl;


		for (int y = pv[0]->y; y < pv[1]->y;
			++y, xl += xil, oneoverzl += oneoverzil, bl += bil, gl += gil, rl += ril, sl += sil, tl += til,
			xr += xir, oneoverzr += oneoverzir, br += bir, gr += gir, rr += rir, sr += sir, tr += tir)
		{
			RGBQUAD colorl = { static_cast<BYTE>(bl), static_cast<BYTE>(gl), static_cast<BYTE>(rl) };
			RGBQUAD colorr = { static_cast<BYTE>(br), static_cast<BYTE>(gr), static_cast<BYTE>(rr) };
			int nxl = static_cast<int>(xl);
			int nxr = static_cast<int>(xr);

			testAndSetPixel(nxl, y, oneoverzl, DotColor(colorl, g_Pipeline.texture.getColor(sl, tl)));
			testAndSetPixel(nxr, y, oneoverzr, DotColor(colorr, g_Pipeline.texture.getColor(sr, tr)));
			ScanLine({ nxl, y, oneoverzl, sl, tl, colorl }, { nxr, y, oneoverzr, sr, tr, colorr });
		}
	}

	//3.scan dege p1p2 and p0p2 
	dyl = pv[2]->y - pv[1]->y;
	if (dyl > 0)
	{
		//left
		float xl = static_cast<float>(pv[1]->x);
		float oneoverzl = pv[1]->z;
		float bl = static_cast<float>(pv[1]->color.rgbBlue);
		float gl = static_cast<float>(pv[1]->color.rgbGreen);
		float rl = static_cast<float>(pv[1]->color.rgbRed);
		float sl = pv[1]->s;
		float tl = pv[1]->t;

		float xil = static_cast<float>(pv[2]->x - pv[1]->x) / dyl;
		float oneoverzil = (pv[2]->z - pv[1]->z) / dyl;
		float bil = static_cast<float>(pv[2]->color.rgbBlue - pv[1]->color.rgbBlue) / dyl;
		float gil = static_cast<float>(pv[2]->color.rgbGreen - pv[1]->color.rgbGreen) / dyl;
		float ril = static_cast<float>(pv[2]->color.rgbRed - pv[1]->color.rgbRed) / dyl;
		float sil = static_cast<float>(pv[2]->s - pv[1]->s) / dyl;
		float til = static_cast<float>(pv[2]->t - pv[1]->t) / dyl;


		for (int y = pv[1]->y; y < pv[2]->y;
			++y, xl += xil, oneoverzl += oneoverzil, bl += bil, gl += gil, rl += ril, sl += sil, tl += til,
			xr += xir, oneoverzr += oneoverzir, br += bir, gr += gir, rr += rir, sr += sir, tr += tir)
		{
			RGBQUAD colorl = { static_cast<BYTE>(bl), static_cast<BYTE>(gl), static_cast<BYTE>(rl) };
			RGBQUAD colorr = { static_cast<BYTE>(br), static_cast<BYTE>(gr), static_cast<BYTE>(rr) };
			int nxl = static_cast<int>(xl);
			int nxr = static_cast<int>(xr);

			testAndSetPixel(nxl, y, oneoverzl, DotColor(colorl, g_Pipeline.texture.getColor(sl, tl)));
			testAndSetPixel(nxr, y, oneoverzr, DotColor(colorr, g_Pipeline.texture.getColor(sr, tr)));
			ScanLine({ nxl, y, oneoverzl, sl, tl, colorl }, { nxr, y, oneoverzr, sr, tr, colorr });
		}
	}
	else //p1p2 is horizontal
	{
		testAndSetPixel(pv[1]->x, pv[1]->y, pv[1]->z, DotColor(pv[1]->color, g_Pipeline.texture.getColor(pv[1]->s, pv[1]->t)));
		ScanLine(*pv[1], *pv[2]);
	}
	testAndSetPixel(pv[2]->x, pv[2]->y, pv[2]->z, DotColor(pv[2]->color, g_Pipeline.texture.getColor(pv[2]->s, pv[2]->t)));
}

void SPipeline::ScanLine(const SVertex3D &vl, const SVertex3D &vr)
{
	int dx = vr.x - vl.x;
	if (dx == 0)	//vl is the same as vr
		return;

	//left
	int x = vl.x;
	float oneoverz = vl.z;
	float b = static_cast<float>(vl.color.rgbBlue);
	float g = static_cast<float>(vl.color.rgbGreen);
	float r = static_cast<float>(vl.color.rgbRed);
	float s = vl.s;
	float t = vl.t;

	int xi = (dx > 0 ? 1 : -1);
	int absdx = abs(dx);
	float oneoverzi = (vr.z - vl.z) / absdx;
	float bi = static_cast<float>(vr.color.rgbBlue - vl.color.rgbBlue) / absdx;
	float gi = static_cast<float>(vr.color.rgbGreen - vl.color.rgbGreen) / absdx;
	float ri = static_cast<float>(vr.color.rgbRed - vl.color.rgbRed) / absdx;
	float si = static_cast<float>(vr.s - vl.s) / absdx;
	float ti = static_cast<float>(vr.t - vl.t) / absdx;

	int y = vr.y;
	while (x != vr.x)
	{
		x += xi;
		oneoverz += oneoverzi;
		b += bi;
		g += gi;
		r += ri;
		s += si;
		t += ti;
		RGBQUAD color = { static_cast<BYTE>(b), static_cast<BYTE>(g), static_cast<BYTE>(r) };
		testAndSetPixel(x, y, oneoverz, DotColor(color, g_Pipeline.texture.getColor(s, t)));
	}
}

void SPipeline::ScanTriangle_c(const SVertex3D &v0, const SVertex3D &v1, const SVertex3D &v2)
{
	const SVertex3D *pv[3] = { &v0, &v1, &v2 };
	// 1.sort vertices with y in ascend
	for (int i = 0; i < 2; ++i)
	{
		for (int j = 0; j < 2 - i; ++j)
		if (pv[j]->y>pv[j + 1]->y)
			std::swap(pv[j], pv[j + 1]);
	}

	float oneoverz00 = g_Pipeline.getOneOverZ(pv[0]->z);
	float oneoverz01 = g_Pipeline.getOneOverZ(pv[1]->z);
	float oneoverz02 = g_Pipeline.getOneOverZ(pv[2]->z);
	//right
	float xr = static_cast<float>(pv[0]->x);
	float zpr = pv[0]->z;
	float oneoverzr = oneoverz00;
	float boverzr = static_cast<float>(pv[0]->color.rgbBlue) * oneoverz00;
	float goverzr = static_cast<float>(pv[0]->color.rgbGreen) * oneoverz00;
	float roverzr = static_cast<float>(pv[0]->color.rgbRed) * oneoverz00;
	float soverzr = pv[0]->s * oneoverz00;
	float toverzr = pv[0]->t * oneoverz00;

	int dyr = pv[2]->y - pv[0]->y;
	float xir = static_cast<float>(pv[2]->x - pv[0]->x) / dyr;
	float zpir = (pv[2]->z - pv[0]->z) / dyr;
	float oneoverzir = (oneoverz02 - oneoverz00) / dyr;
	float boverzir = (static_cast<float>(pv[2]->color.rgbBlue) * oneoverz02 - static_cast<float>(pv[0]->color.rgbBlue) * oneoverz00) / dyr;
	float goverzir = (static_cast<float>(pv[2]->color.rgbGreen) * oneoverz02 - static_cast<float>(pv[0]->color.rgbGreen) * oneoverz00) / dyr;
	float roverzir = (static_cast<float>(pv[2]->color.rgbRed) * oneoverz02 - static_cast<float>(pv[0]->color.rgbRed) * oneoverz00) / dyr;
	float soverzir = (pv[2]->s * oneoverz02 - pv[0]->s * oneoverz00) / dyr;
	float toverzir = (pv[2]->t * oneoverz02 - pv[0]->t * oneoverz00) / dyr;

	int dyl = pv[1]->y - pv[0]->y;
	//2.scan edge p0p1 and p0p2
	if (dyl > 0)
	{
		//left
		float xl = static_cast<float>(pv[0]->x);;
		float zpl = pv[0]->z;
		float oneoverzl = oneoverz00;
		float boverzl = static_cast<float>(pv[0]->color.rgbBlue) * oneoverz00;;
		float goverzl = static_cast<float>(pv[0]->color.rgbGreen) * oneoverz00;;
		float roverzl = static_cast<float>(pv[0]->color.rgbRed) * oneoverz00;;
		float soverzl = pv[0]->s * oneoverz00;;
		float toverzl = pv[0]->t * oneoverz00;;

		float xil = static_cast<float>(pv[1]->x - pv[0]->x) / dyl;
		float zpil = (pv[1]->z - pv[0]->z) / dyl;
		float oneoverzil = (oneoverz01 - oneoverz00) / dyl;
		float boverzil = (static_cast<float>(pv[1]->color.rgbBlue) * oneoverz01 - static_cast<float>(pv[0]->color.rgbBlue) * oneoverz00) / dyl;
		float goverzil = (static_cast<float>(pv[1]->color.rgbGreen) * oneoverz01 - static_cast<float>(pv[0]->color.rgbGreen) * oneoverz00) / dyl;
		float roverzil = (static_cast<float>(pv[1]->color.rgbRed) * oneoverz01 - static_cast<float>(pv[0]->color.rgbRed) * oneoverz00) / dyl;
		float soverzil = (pv[1]->s * oneoverz01 - pv[0]->s * oneoverz00) / dyl;
		float toverzil = (pv[1]->t * oneoverz01 - pv[0]->t * oneoverz00) / dyl;


		for (int y = pv[0]->y; y < pv[1]->y;
			++y, xl += xil, zpl += zpil, oneoverzl += oneoverzil, boverzl += boverzil, goverzl += goverzil, roverzl += roverzil, soverzl += soverzil, toverzl += toverzil,
			xr += xir, zpr += zpir, oneoverzr += oneoverzir, boverzr += boverzir, goverzr += goverzir, roverzr += roverzir, soverzr += soverzir, toverzr += toverzir)
		{
			float zl = 1.0f / oneoverzl;
			float zr = 1.0f / oneoverzr;
			RGBQUAD colorl = { static_cast<BYTE>(boverzl * zl), static_cast<BYTE>(goverzl * zl), static_cast<BYTE>(roverzl * zl) };
			RGBQUAD colorr = { static_cast<BYTE>(boverzr * zr), static_cast<BYTE>(goverzr * zr), static_cast<BYTE>(roverzr * zr) };
			int nxl = static_cast<int>(xl);
			int nxr = static_cast<int>(xr);

			testAndSetPixel(nxl, y, zpl, DotColor(colorl, g_Pipeline.texture.getColor(soverzl * zl, toverzl * zl)));
			testAndSetPixel(nxr, y, zpr, DotColor(colorr, g_Pipeline.texture.getColor(soverzr * zr, toverzr * zr)));
			ScanLine_c({ nxl, y, zpl, soverzl, toverzl, colorl }, { nxr, y, zpr, soverzr, toverzr, colorr });
		}
	}

	//3.scan dege p1p2 and p0p2 
	dyl = pv[2]->y - pv[1]->y;
	if (dyl > 0)
	{
		//left
		float xl = static_cast<float>(pv[1]->x);
		float zpl = pv[1]->z;
		float oneoverzl = oneoverz01;
		float boverzl = static_cast<float>(pv[1]->color.rgbBlue) * oneoverzl;
		float goverzl = static_cast<float>(pv[1]->color.rgbGreen) * oneoverzl;
		float roverzl = static_cast<float>(pv[1]->color.rgbRed) * oneoverzl;
		float soverzl = pv[1]->s * oneoverzl;
		float toverzl = pv[1]->t * oneoverzl;

		float xil = static_cast<float>(pv[2]->x - pv[1]->x) / dyl;
		float zpil = (pv[2]->z - pv[1]->z) / dyl;
		float oneoverzil = (oneoverz02 - oneoverz01) / dyl;
		float boverzil = (static_cast<float>(pv[2]->color.rgbBlue) * oneoverz02 - static_cast<float>(pv[1]->color.rgbBlue) * oneoverz01) / dyl;
		float goverzil = (static_cast<float>(pv[2]->color.rgbGreen) * oneoverz02 - static_cast<float>(pv[1]->color.rgbGreen) * oneoverz01) / dyl;
		float roverzil = (static_cast<float>(pv[2]->color.rgbRed) * oneoverz02 - static_cast<float>(pv[1]->color.rgbRed) * oneoverz01) / dyl;
		float soverzil = static_cast<float>(pv[2]->s * oneoverz02 - pv[1]->s * oneoverz01) / dyl;
		float toverzil = static_cast<float>(pv[2]->t * oneoverz02 - pv[1]->t * oneoverz01) / dyl;


		for (int y = pv[1]->y; y < pv[2]->y;
			++y, xl += xil, zpl += zpil, oneoverzl += oneoverzil, boverzl += boverzil, goverzl += goverzil, roverzl += roverzil, soverzl += soverzil, toverzl += toverzil,
			xr += xir, zpr += zpir, oneoverzr += oneoverzir, boverzr += boverzir, goverzr += goverzir, roverzr += roverzir, soverzr += soverzir, toverzr += toverzir)
		{
			float zl = 1.0f / oneoverzl;
			float zr = 1.0f / oneoverzr;
			RGBQUAD colorl = { static_cast<BYTE>(boverzl * zl), static_cast<BYTE>(goverzl * zl), static_cast<BYTE>(roverzl * zl) };
			RGBQUAD colorr = { static_cast<BYTE>(boverzr * zr), static_cast<BYTE>(goverzr * zr), static_cast<BYTE>(roverzr * zr) };
			int nxl = static_cast<int>(xl);
			int nxr = static_cast<int>(xr);

			testAndSetPixel(nxl, y, zpl, DotColor(colorl, g_Pipeline.texture.getColor(soverzl * zl, toverzl * zl)));
			testAndSetPixel(nxr, y, zpr, DotColor(colorr, g_Pipeline.texture.getColor(soverzr * zr, toverzr * zr)));
			ScanLine_c({ nxl, y, zpl, soverzl, toverzl, colorl }, { nxr, y, zpr, soverzr, toverzr, colorr });
		}
	}
	else //p1p2 is horizontal
	{
		testAndSetPixel(pv[1]->x, pv[1]->y, pv[1]->z, DotColor(pv[1]->color, g_Pipeline.texture.getColor(pv[1]->s, pv[1]->t)));
		SVertex3D v1 = *pv[1], v2 = *pv[2];
		v1.s = pv[1]->s * oneoverz01;
		v1.t = pv[1]->t * oneoverz01;
		v2.s = pv[2]->s * oneoverz02;
		v2.t = pv[2]->t * oneoverz02;
		ScanLine_c(v1, v2);
	}
	testAndSetPixel(pv[2]->x, pv[2]->y, pv[2]->z, DotColor(pv[2]->color, g_Pipeline.texture.getColor(pv[2]->s, pv[2]->t)));

}

void SPipeline::ScanLine_c(const SVertex3D &vl, const SVertex3D &vr)
{
	int dx = vr.x - vl.x;
	int absdx = abs(dx);
	if (dx == 0 || absdx == 1)	//vl is the same as vr
		return;

	float oneoverz_l = g_Pipeline.getOneOverZ(vl.z);
	float oneoverz_r = g_Pipeline.getOneOverZ(vr.z);
	//left
	int x = vl.x;
	float zp = vl.z;
	float oneoverz = oneoverz_l;
	float boverz = static_cast<float>(vl.color.rgbBlue) * oneoverz_l;
	float goverz = static_cast<float>(vl.color.rgbGreen) * oneoverz_l;
	float roverz = static_cast<float>(vl.color.rgbRed) * oneoverz_l;
	float soverz = vl.s;
	float toverz = vl.t;

	int xi = (dx > 0 ? 1 : -1);
	float zpi = (vr.z - vl.z) / absdx;
	float oneoverzi = (oneoverz_r - oneoverz_l) / absdx;
	float boverzi = (static_cast<float>(vr.color.rgbBlue) * oneoverz_r - static_cast<float>(vl.color.rgbBlue) * oneoverz_l) / absdx;
	float goverzi = (static_cast<float>(vr.color.rgbGreen) * oneoverz_r - static_cast<float>(vl.color.rgbGreen) * oneoverz_l) / absdx;
	float roverzi = (static_cast<float>(vr.color.rgbRed) * oneoverz_r - static_cast<float>(vl.color.rgbRed) * oneoverz_l) / absdx;
	float soverzi = (vr.s - vl.s) / absdx;
	float toverzi = (vr.t - vl.t) / absdx;

	int y = vr.y;
	while (x != vr.x)
	{
		x += xi, zp += zpi, oneoverz += oneoverzi;
		boverz += boverzi, goverz += goverzi, roverz += roverzi;
		soverz += soverzi, toverz += toverzi;
		float z = 1.0f / oneoverz;
		RGBQUAD color = { static_cast<BYTE>(boverz*z), static_cast<BYTE>(goverz*z), static_cast<BYTE>(roverz*z) };
		testAndSetPixel(x, y, zp, DotColor(color, g_Pipeline.texture.getColor(soverz*z, toverz*z)));
	}
}