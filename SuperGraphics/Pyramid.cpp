#include "Pyramid.h"

using namespace DiyMath3D;

const CVertexf	CPyramid::m_Vertices[m_VerticesCount] = {
	//front
	CVertexf(CVector4f(0.f, 0.f, 0.f, 1.0f), CVector2f(0.f, 0.f), CVector3f(1.f, 1.f, 1.f), CVector3f(-0.57735027f, -0.57735027f, -0.57735027f)),	//bottom left
	CVertexf(CVector4f(1.f, 0.f, 0.f, 1.0f), CVector2f(1.f, 0.f), CVector3f(1.f, 1.f, 1.f), CVector3f(0.57735027f, -0.57735027f, -0.57735027f)),	//bottom riht
	CVertexf(CVector4f(1.f, 1.f, 0.f, 1.0f), CVector2f(1.f, 1.f), CVector3f(1.f, 1.f, 1.f), CVector3f(0.57735027f, 0.57735027f, -0.57735027f)),	//top right
	CVertexf(CVector4f(0.f, 1.f, 0.f, 1.0f), CVector2f(0.f, 1.f), CVector3f(1.f, 1.f, 1.f), CVector3f(-0.57735027f, 0.57735027f, -0.57735027f)),	//top left
	//back
	CVertexf(CVector4f(0.f, 0.f, 1.f, 1.0f), CVector2f(0.f, 0.f), CVector3f(1.f, 1.f, 1.f), CVector3f(-0.57735027f, -0.57735027f, 0.57735027f)),	//bottom left
	CVertexf(CVector4f(1.f, 0.f, 1.f, 1.0f), CVector2f(1.f, 0.f), CVector3f(1.f, 1.f, 1.f), CVector3f(0.57735027f, -0.57735027f, 0.57735027f)),	//bottom riht
	CVertexf(CVector4f(1.f, 1.f, 1.f, 1.0f), CVector2f(1.f, 1.f), CVector3f(1.f, 1.f, 1.f), CVector3f(0.57735027f, 0.57735027f, 0.57735027f)),	//top right
	CVertexf(CVector4f(0.f, 1.f, 1.f, 1.0f), CVector2f(0.f, 1.f), CVector3f(1.f, 1.f, 1.f), CVector3f(-0.57735027f, 0.57735027f, 0.57735027f)),	//top left
};

const int	CPyramid::m_Elements[m_TriangleCount * 3]={
	//front
	3, 2, 0,
	0, 2, 1,
	//back
	5, 6, 4,
	4, 6, 7,
	//left
	4, 3, 0,
	3, 4, 7,
	//right
	1, 2, 5,
	5, 2, 6,
	//top
	7, 2, 3,
	2, 7, 6,
	//bottom
	0, 1, 4,
	5, 4, 1};

CPyramid::CPyramid()
:m_ka(0), m_kd(0), m_ks(0), m_pLight(nullptr)
{

}

CPyramid::CPyramid(SLight *pl, float ka, float kd, float ks)
: m_pLight(pl), m_ka(ka), m_kd(kd), m_ks(ks)
{

}
CPyramid::CPyramid(const CPyramid& pyr)
{
	*this = pyr;
}

CPyramid& CPyramid::operator=(const CPyramid& pyr)
{
	m_pLight = pyr.getPointLight();
	setRatio(pyr.getKa(), pyr.getKd(), pyr.getKs());
	return *this;
}
void CPyramid::transform(const DiyMath3D::CMatrix44f &model, const DiyMath3D::CMatrix44f &view, const DiyMath3D::CMatrix44f &projection, bool bLight)
{
	for (int i = 0; i < m_VerticesCount; ++i)
	{
		m_Positions[i] = m_Vertices[i].m_Position*model*view*projection;
		if (!bLight || !m_pLight)
		{
			m_Colors[i] = m_Vertices[i].m_Color;
			continue;
		}
		//gouraud light
		float costheta = Normalize(m_Vertices[i].m_Normal*CMatrix33f(model))*Normalize(-m_pLight->position);
		m_Colors[i] = m_pLight->ambient*m_ka + m_pLight->diffuse*m_kd*(costheta>0?costheta:0);
		m_Colors[i] = ColorDot(m_Colors[i], m_Vertices[i].m_Color);
	}
}
