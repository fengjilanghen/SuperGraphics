#ifndef _PYRAMID_H_
#define _PYRAMID_H_

#include "DiyMath3D.h"
#include "Light.h"

class CPyramid
{
public:
	CPyramid();
	CPyramid(SLight *pl, float ka, float kd, float ks);
	CPyramid(const CPyramid& pyr);
	CPyramid& operator=(const CPyramid& pyr);
	void transform(const DiyMath3D::CMatrix44f &model, const DiyMath3D::CMatrix44f &View, const DiyMath3D::CMatrix44f &projection, bool bLight);
	
	inline void setPointLight(SLight* pl){ m_pLight = pl; }
	inline SLight* getPointLight() const { return m_pLight; }
	inline void setRatio(float ka, float kd, float ks){ m_ka = ka; m_kd = kd; m_ks = ks; }
	inline const DiyMath3D::CVector4f& position(int index) const { return m_Positions[index]; }
	inline const DiyMath3D::CVector3f& color(int index) const { return m_Colors[index]; }
	inline float getKa() const { return m_ka; }
	inline float getKd() const { return m_kd; }
	inline float getKs() const { return m_ks; }

public:
	static const int						m_VerticesCount = 8;
	static const int						m_TriangleCount = 12;
	static const CVertexf					m_Vertices[m_VerticesCount];
	static const int						m_Elements[m_TriangleCount * 3];


private:
	DiyMath3D::CVector4f	m_Positions[m_VerticesCount];
	DiyMath3D::CVector3f	m_Colors[m_VerticesCount];
	float					m_ka;
	float					m_kd;
	float					m_ks;
	SLight*					m_pLight;
};

#endif //_PYRAMID_H_