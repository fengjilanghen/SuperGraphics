#ifndef _VERTEX_H_
#define _VERTEX_H_

#include "Vector.h"
#include <cassert>

class CVertexf
{
public:
	CVertexf();

	CVertexf(const DiyMath3D::CVector4f &pos,
		const DiyMath3D::CVector2f &texCoords,
		const DiyMath3D::CVector3f &color,
		const DiyMath3D::CVector3f &normal);

	CVertexf(const CVertexf& vertex);

	CVertexf& operator=(const CVertexf& vertex);

	void set(const DiyMath3D::CVector4f &pos,
		const DiyMath3D::CVector2f &texCoords,
		const DiyMath3D::CVector3f &color,
		const DiyMath3D::CVector3f &normal = DiyMath3D::CVector3f());

public:
	DiyMath3D::CVector4f		m_Position;
	DiyMath3D::CVector3f		m_Color;
	DiyMath3D::CVector2f		m_TexCoords;
	DiyMath3D::CVector3f		m_Normal;
};

#endif //_VERTEX_H_