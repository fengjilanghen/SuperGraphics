#ifndef _TEXTURE_H_
#define _TEXTURE_H_

#include <Windows.h>

class CTexture
{
public:
	typedef tagRGBTRIPLE color_type;

	CTexture();
	CTexture(LPCTSTR fileName);
	~CTexture();
	bool loadTexture(LPCTSTR fileName);
	const color_type& getColor(float s, float t) const;
	const color_type& getColor(int s, int t) const;
	void release();
private:
	BYTE* LoadBMP(int* width, int* height, int* planes, LPCTSTR bmpfile);

private:
	BYTE	*m_pData;
	int		m_Width;
	int		m_Height;
	int		m_BytesPerRow;
	int		m_BytesPerPixel;
};

#endif //_TEXTURE_H_