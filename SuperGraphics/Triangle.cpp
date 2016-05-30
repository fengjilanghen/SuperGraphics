#include "Triangle.h"
using namespace DiyMath3D;

CTrianglef::CTrianglef()
{
	m_Vertices[0] = CVertexf();
	m_Vertices[1] = CVertexf();
	m_Vertices[2] = CVertexf();
}
CTrianglef::CTrianglef(const CVertexf& pt1, const CVertexf& pt2, const CVertexf& pt3)
{
	m_Vertices[0] = pt1;
	m_Vertices[1] = pt2;
	m_Vertices[2] = pt3;
}

CTrianglef::CTrianglef(const CTrianglef& ref)
{
	*this = ref;
}


CTrianglef& CTrianglef::operator=(const CTrianglef& ref)
{
	int size = sizeof(m_Vertices);
	for (int i = 0; i < size; ++i)
		m_Vertices[i] = ref.m_Vertices[i];
	return *this;
}

CVertexf& CTrianglef::operator[](int index)
{
	assert(index >= 0 && index < 3);
	return m_Vertices[index];
}

CVertexf CTrianglef::operator[](int index) const
{
	assert(index >= 0 && index < 3);
	return m_Vertices[index];
}