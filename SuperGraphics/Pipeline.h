#ifndef _CLIP_TRIANGLE_H_
#define _CLIP_TRIANGLE_H_

#include "Texture.h"
#include "CircleQueue.h"
#include "Vertex.h"
#include "Win32Draw.h"
#include "DiyMath3D.h"

typedef CCircleQueue<CVertexf, 16> CClipCircleQueue;

enum eClipWindowEdgeType
{
	ECWET_LEFT = 0,
	ECWET_UP,
	ECWET_RIGHT,
	ECWET_DOWN,
	ECWET_COUNT
};

enum eDrawType
{
	EDT_FRAME = 0,
	EDT_TEXTURE_MAPPING,
	EDT_PERSPECTIVE_CORRECTION,
	EDT_COUNT
};

enum eLineMode
{
	ELM_DDA = 0,
	ELM_BRESENHAM,
	ELM_AA,
	ELM_COUNT
};

struct SPipeline
{
public:
	HDC						hdc;
	DiyMath3D::CMatrix44f	model;
	DiyMath3D::CMatrix44f	view;
	DiyMath3D::CMatrix44f	projection;
	DiyMath3D::CMatrix44f	viewport;
	CTexture				texture;
	eDrawType				drawType;
	eLineMode				lineMode;

public:
	inline float getZ(float zp){ return projection(3, 2) / (zp - projection(2, 2)); }
	inline float getOneOverZ(float zp){ return 1.0f / getZ(zp); }	
	inline BYTE DotColor(BYTE b1, BYTE b2){ return b1*b2 / 255; }
	inline RGBQUAD DotColor(const RGBQUAD &q, const RGBTRIPLE &t){ return{ DotColor(q.rgbBlue, t.rgbtBlue), DotColor(q.rgbGreen, t.rgbtGreen), DotColor(q.rgbRed, t.rgbtRed) }; }

	//draw entrance after projection transform
	void ZClipTriangle(CClipCircleQueue & ccqPts, float zNear);
	//clip against near plane
	void ClipAgainstNearPlane(CClipCircleQueue &ccqPts, const CVertexf *p0, const CVertexf *p1, float zNear);
	//contrust with new clipped points
	void ConstructTriangleInZClip(CClipCircleQueue &ccq);

	//cull back face in ndc
	bool CullBack(const DiyMath3D::CVector4f &pos0, const DiyMath3D::CVector4f &pos1, const DiyMath3D::CVector4f &pos2);

	//clip in ndc
	void ClipTriangleInNDC(CClipCircleQueue &cq);
	//clip aganist topleft(-1.0f,1.0f) to bottomright(1.0f, -1.0f)
	void ClipEdgeAgainstClipWindow(CClipCircleQueue &cq, const CVertexf &v0, const CVertexf &v1, eClipWindowEdgeType type);
	//contrust with new clipped points
	void ConstructTriangleInNDC(CClipCircleQueue &ccq);

	//draw triangle in viewport
	void DrawTriangle(const CVertexf *p0, const CVertexf *p1, const CVertexf *p2);
	//texture mapping without correction
	void ScanTriangle(const npWin32Draw::SVertex3D &v0, const npWin32Draw::SVertex3D &v1, const npWin32Draw::SVertex3D &v2);
	void ScanLine(const npWin32Draw::SVertex3D &vl, const npWin32Draw::SVertex3D &vr);
	//with perspective correction
	void ScanTriangle_c(const npWin32Draw::SVertex3D &v0, const npWin32Draw::SVertex3D &v1, const npWin32Draw::SVertex3D &v2);
	void ScanLine_c(const npWin32Draw::SVertex3D &vl, const npWin32Draw::SVertex3D &vr);

	static SPipeline* getInstance()
	{
		static SPipeline pipeline;
		return &pipeline;
	}

private :
	SPipeline(){};
	SPipeline(const SPipeline&) = delete;
	SPipeline& operator=(const SPipeline&) = delete;
};


#endif //_CLIP_TRIANGLE_H_