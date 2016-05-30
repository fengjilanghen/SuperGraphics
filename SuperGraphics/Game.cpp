#include "Game.h"
#include "Pipeline.h"

using namespace DiyMath3D;
using namespace npWin32Draw;

#define NEAR	100.0f
#define FAR		1000.0f

CGame::CGame() :m_Light(ELT_DIRECTIONAL, CVector3f(0.0f, -1.0f, 0.0f), CVector3f(0.3f, 0.3f, 0.3f), CVector3f(1.0f, 1.0f, 1.0f)),
m_Pyramid(&m_Light, 0.2f, 0.8f, 0.2f), m_Fov(45.0f), m_xRotate(0), m_yRotate(0.0f), m_zRotate(0),
m_FpsCamera(CVector3f(0, 0, -350.f), 0.0f, 0.0f), m_bLight(false)
{
}

CGame::~CGame()
{
}


void CGame::init(int width, int height)
{
	BuildModel();
	g_Pipeline.projection = Perspective(m_Fov, NEAR, FAR);
	g_Pipeline.viewport = ViewportTransform(0, 0, width, height);
	
	
	if (!g_Pipeline.texture.loadTexture(L"../pic/bw.bmp"))
		MessageBox(NULL, L"loadTexture failed...", L"ERROR", MB_OK);
}

void CGame::BuildModel()
{
	g_Pipeline.model.loadIdentity();
	g_Pipeline.model = Scale(g_Pipeline.model, CVector3f(100.0f, 100.0f, 100.0f));
	g_Pipeline.model = Translate(g_Pipeline.model, CVector3f(-50.0f, -50.0f, -50.f));
	g_Pipeline.model = Rotate(g_Pipeline.model, CVector3f(1.0f, 0.0f, 0.0f), 45.0f);
	//m_View = Rotate(m_View, CVector3f(0.0f, 1.0f, 0.0f), m_yRotate);
	g_Pipeline.model = Rotate(g_Pipeline.model, CVector3f(0.0f, 0.0f, 1.0f), 45.0f);
}

void CGame::release()
{

}

void CGame::keyDown(int key)
{
	const int STEPS = 20.0f;
	switch (key)
	{
	case 'w':
	case 'W':
		m_FpsCamera.Move(CFpsCamera::EMOT_BACKFORTH, STEPS);
		break;
	case 's':
	case 'S':
		m_FpsCamera.Move(CFpsCamera::EMOT_BACKFORTH, -STEPS);
		break;
	case 'a':
	case 'A':
		m_FpsCamera.Move(CFpsCamera::EMOT_LEFTRIGHT, STEPS);
		break;
	case 'd':
	case 'D':
		m_FpsCamera.Move(CFpsCamera::EMOT_LEFTRIGHT, -STEPS);
		break;
	case 'q':
	case 'Q':
		//m_zRotate += 2.0f;
		g_Pipeline.model = Rotate(g_Pipeline.model, CVector3f(0.0f, 0.0f, 1.0f), 2.0f);
		break;
	case 'e':
	case 'E':
		//m_zRotate -= 2.0f;
		g_Pipeline.model = Rotate(g_Pipeline.model, CVector3f(0.0f, 0.0f, 1.0f), -2.0f);
		break;
	case 't':
	case 'T':
		g_Pipeline.drawType = static_cast<eDrawType>((g_Pipeline.drawType + 1) % EDT_COUNT);
		break;
	case 'l':
	case 'L':
		m_bLight=!m_bLight;
		break;
	case 'k':
	case 'K':
		g_Pipeline.lineMode = static_cast<eLineMode>((g_Pipeline.lineMode + 1) % ELM_COUNT);
		break;
	}
}

void CGame::MouseWheel(int delta)
{
	m_Fov = max(min(m_Fov+delta, 90.0f), 10.0f);
	g_Pipeline.projection = Perspective(m_Fov, 1.0f, 1000.0f);
}

void CGame::MouseMove(int offx, int offy, eMouseType button)
{
	switch (button)
	{
	case EMT_RBUTTONDOWN:
		m_FpsCamera.setAngle(static_cast<float>(-offy), static_cast<float>(offx));
		break;
	case EMT_LBUTTONDOWN:
		m_yRotate -= offx;
		m_xRotate -= offy;
		g_Pipeline.model = Rotate(g_Pipeline.model, CVector3f(1.0f, 0.0f, 0.0f), -offy);
		g_Pipeline.model = Rotate(g_Pipeline.model, CVector3f(0.0f, 1.0f, 0.0f), -offx);
		break;
	}
}

void CGame::update(float delta_time)
{	
}

void CGame::render(HDC hdc)
{
	g_Pipeline.view = m_FpsCamera.getView();
	m_Pyramid.transform(g_Pipeline.model, g_Pipeline.view, g_Pipeline.projection, m_bLight);

	CClipCircleQueue	ccqPts;
	CVertexf			vt;
	int					size = m_Pyramid.m_TriangleCount * 3;

	for (int i = 0; i < size;)
	{
		for (int j = 0; j < 3; ++j)
		{
			int nindex = m_Pyramid.m_Elements[i++];
			const CVector4f &pos = m_Pyramid.position(nindex);
			const CVector2f &tex = m_Pyramid.m_Vertices[nindex].m_TexCoords;
			const CVector3f &clr = m_Pyramid.color(nindex);

			vt.set(pos, tex, clr);
			ccqPts.push(vt);
		}

		g_Pipeline.ZClipTriangle(ccqPts, NEAR);
		ccqPts.clear();
	}
}