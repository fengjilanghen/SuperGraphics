#ifndef _FPS_CAMERA_H_
#define _FPS_CAMERA_H_

#include "global_data.h"
#include "DiyMath3D.h"

class CFpsCamera
{
public:
	enum eMoveOrientationType
	{
		EMOT_BACKFORTH=0,
		EMOT_LEFTRIGHT,
		EMOT_COUNT
	};

	CFpsCamera();
	CFpsCamera(const DiyMath3D::CVector3f &pos, float pitch, float yaw);

	void Move(eMoveOrientationType orientation, float offset);
	void setAngle(float pitchoff, float yawoff);
	DiyMath3D::CMatrix44f getView() const;

	float getPitch(){ return m_Pitch; }
	float getYaw(){ return m_Yaw; }

private:
	void CaculateView();
	inline float ClampAnger(float angle) const { return min(max(-60.0f, angle), 60.0f); }
private:
	float						m_Pitch;
	float						m_Yaw;
	float						m_PitchSpeed;
	float						m_YawSpeed;
	DiyMath3D::CVector3f		m_Position;	//camera pos in world Coordinate system
	DiyMath3D::CVector3f		m_Front;	//unit vector of z axis in view Coordinate system
	DiyMath3D::CVector3f		m_Right;	//unit vector of x axis in view  Coordinate system
	DiyMath3D::CMatrix44f		m_View;
};

#endif //_FPS_CAMERA_H_