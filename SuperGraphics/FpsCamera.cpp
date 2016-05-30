#include "FpsCamera.h"
#include <cmath>
using namespace DiyMath3D;


extern const int g_Width, g_Height;

CFpsCamera::CFpsCamera()
:m_Position(0, 0, 0), m_Pitch(0.0f), m_Yaw(0.0f), m_PitchSpeed(89.0f * 2 / g_Height), m_YawSpeed(89.0f * 2 / g_Width)
{
	CaculateView();
}

CFpsCamera::CFpsCamera(const DiyMath3D::CVector3f &pos, float pitch, float yaw)
: m_Position(pos), m_Pitch(pitch), m_Yaw(yaw), m_PitchSpeed(89.0f * 2 / g_Height), m_YawSpeed(89.0f * 2 / g_Width)
{
	CaculateView();
}

void CFpsCamera::Move(eMoveOrientationType orientation, float offset)
{
	assert(orientation < EMOT_COUNT);
	switch (orientation)
	{
	case CFpsCamera::EMOT_BACKFORTH:
		m_Position += m_Front*offset;
		break;
	case CFpsCamera::EMOT_LEFTRIGHT:
		m_Position += m_Right*offset;
		break;
	}
	CaculateView();
}

void CFpsCamera::setAngle(float pitchoff, float yawoff)
{
	m_Pitch = ClampAnger(m_Pitch + pitchoff*m_PitchSpeed);
	m_Yaw += yawoff*m_YawSpeed;
	//m_Yaw = ClampAnger(m_Yaw);
	CaculateView();
}


void CFpsCamera::CaculateView()
{
	m_Front = Normalize(CVector3f(cos(Rad(m_Pitch))*sin(Rad(m_Yaw)), sin(Rad(m_Pitch)), cos(Rad(m_Pitch))*cos(Rad(m_Yaw))));
	m_Right = Normalize(CrossProduct(CVector3f(0.0f, 1.0f, 0.0f), m_Front));
	m_View = DiyMath3D::LookAt(m_Position, m_Position + m_Front);
}

DiyMath3D::CMatrix44f CFpsCamera::getView() const
{
	return m_View;
}