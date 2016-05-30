#ifndef _GAME_H_
#define _GAME_H_

#include "Light.h"
#include "FpsCamera.h"
#include "Pyramid.h"

class CGame
{
public:
	enum eMouseType
	{
		EMT_NONE = 0,
		EMT_LBUTTONDOWN,
		EMT_RBUTTONDOWN,
		EMT_MBUTTONDOWN,
		EMT_COUNT
	};


	CGame();
	~CGame();
	void init(int width, int height);
	void release();
	void keyDown(int key);
	void MouseMove(int offx, int offy, eMouseType button);
	void MouseWheel(int delta);
	void update(float delta_time);
	void render(HDC hdc);


	float getYaw(){ return m_FpsCamera.getYaw(); }
	float getPitch(){ return m_FpsCamera.getPitch(); }
private:
	void BuildModel();

private:
	SLight					m_Light;
	CPyramid				m_Pyramid;
	CFpsCamera				m_FpsCamera;
	bool					m_bLight;
	float					m_Fov;
	float					m_yRotate;
	float					m_xRotate;
	float					m_zRotate;
};

#endif //_GAME_H_