#ifndef _TRIANGLE_H_
#define _TRIANGLE_H_
#include "Vertex.h"

namespace DiyMath3D
{
	class CTrianglef;
}

class DiyMath3D::CTrianglef
{
public:
	CTrianglef();
	CTrianglef(const CVertexf& pt1, const CVertexf& pt2, const CVertexf& pt3);

	CTrianglef(const CTrianglef& ref);

	CTrianglef& operator=(const CTrianglef& ref);

	CVertexf& operator[](int index);

	CVertexf operator[](int index) const;

private:
	CVertexf m_Vertices[3];
};


#endif //_TRIANGLE_H_