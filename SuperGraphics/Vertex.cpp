#include "Vertex.h"

using DiyMath3D::CVector4f;
using DiyMath3D::CVector3f;
using DiyMath3D::CVector2f;

CVertexf::CVertexf()
	:m_Position(), m_Color(), m_TexCoords(), m_Normal()
{
}

CVertexf::CVertexf(const CVector4f &pos,
	const CVector2f &texCoords,
	const CVector3f &color,
	const CVector3f &normal)
	:m_Position(pos), m_Color(color), m_TexCoords(texCoords), m_Normal(normal)
{
}

CVertexf::CVertexf(const CVertexf& vertex)
{
	*this = vertex;
}

CVertexf& CVertexf::operator=(const CVertexf& vertex)
{
	m_Position = vertex.m_Position;
	m_Color = vertex.m_Color;
	m_TexCoords = vertex.m_TexCoords;
	m_Normal = vertex.m_Normal;
	return *this;
}


void CVertexf::set(const DiyMath3D::CVector4f &pos,
	const DiyMath3D::CVector2f &texCoords,
	const DiyMath3D::CVector3f &color,
	const DiyMath3D::CVector3f &normal)
{
	m_Position = pos;
	m_Color = color;
	m_TexCoords = texCoords;
	m_Normal = normal;
}