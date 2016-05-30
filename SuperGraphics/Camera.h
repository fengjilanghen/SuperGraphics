#ifndef _CAMERA_H_
#define _CAMERA_H_

#include "DiyMath3D.h"

class CCamera
{
	//friend class CCamera;
public:
	CCamera(const DiyMath3D::CVector3f& pos = DiyMath3D::CVector3f(0, 0, 0),
		const DiyMath3D::CVector3f& lookat = DiyMath3D::CVector3f(0, 0, 1),
		const DiyMath3D::CVector3f& up = DiyMath3D::CVector3f(0, 1, 0))
		:m_Position(pos), m_LookAt(lookat), m_Up(DiyMath3D::Normalize(up)), m_View()
	{
		lookAt(lookat, up);
	}

	CCamera(const CCamera& ref)
	{
		*this = ref;
	}

	CCamera& operator=(const CCamera& ref)
	{
		m_Position = ref.m_Position;
		m_LookAt = ref.m_LookAt;
		m_Up = ref.m_Up;
		return *this;
	}

	inline void setPosition(const DiyMath3D::CVector3f& pos)
	{
		m_Position = pos;
	}

	DiyMath3D::CMatrix44f lookAt(const DiyMath3D::CVector3f& lookat,
		const DiyMath3D::CVector3f& up = DiyMath3D::CVector3f(0, 1, 0))
	{
		m_LookAt = lookat;
		m_Up = DiyMath3D::Normalize(up);
		return DiyMath3D::LookAt(m_Position, lookat, m_Up);
	}	

	inline DiyMath3D::CMatrix44f getView() const
	{
		return m_View;
	}


private:
	DiyMath3D::CVector3f		m_Position;
	DiyMath3D::CVector3f		m_LookAt;
	DiyMath3D::CVector3f		m_Up;
	DiyMath3D::CMatrix44f		m_View;
};

#endif //_CAMERA_H_