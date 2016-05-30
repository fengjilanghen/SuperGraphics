#include "Texture.h"

CTexture::CTexture()
	:m_pData(NULL)
{
}

CTexture::CTexture(LPCTSTR fileName)
{
	loadTexture(fileName);
}

CTexture::~CTexture()
{
	release();
}

void CTexture::release()
{
	delete[] m_pData;
	m_pData = NULL;
}

bool CTexture::loadTexture(LPCTSTR fileName)
{
	release();
	int bitCount;
	m_pData = LoadBMP(&m_Width, &m_Height, &bitCount, fileName);
	if (m_pData)
	{
		m_BytesPerRow = ((m_Width*bitCount + 31)/32)*4;
		m_BytesPerPixel = bitCount >> 3;
		return true;
	}
	return false;
}

const CTexture::color_type& CTexture::getColor(float s, float t) const
{
	int x = static_cast<int>(s*m_Width);
	int y = static_cast<int>(t*m_Height);
	return getColor(x, y);
}

const CTexture::color_type& CTexture::getColor(int s, int t) const
{
	s = max(0, min(s, m_Width-1));
	t = max(0, min(t, m_Height-1));
	return *reinterpret_cast<color_type*>(m_pData+t*m_BytesPerRow + s*m_BytesPerPixel);
}

BYTE* CTexture::LoadBMP(int* width, int* height, int* bitCount, LPCTSTR bmpfile)
{
	// declare bitmap structures
	BITMAPFILEHEADER bmpheader;
	BITMAPINFOHEADER bmpinfo;
	// value to be used in ReadFile funcs
	DWORD bytesread;
	// open file to read from
	HANDLE file = CreateFile(bmpfile, GENERIC_READ, FILE_SHARE_READ,
		NULL, OPEN_EXISTING, FILE_FLAG_SEQUENTIAL_SCAN, NULL);
	if (NULL == file)
		return NULL; // coudn't open file


	// read file header
	if (ReadFile(file, &bmpheader, sizeof(BITMAPFILEHEADER), &bytesread, NULL) == false)
	{
		CloseHandle(file);
		return NULL;
	}

	//read bitmap info

	if (ReadFile(file, &bmpinfo, sizeof(BITMAPINFOHEADER), &bytesread, NULL) == false)
	{
		CloseHandle(file);
		return NULL;
	}

	// check if file is actually a bmp
	if (bmpheader.bfType != 'MB')
	{
		CloseHandle(file);
		return NULL;
	}
	
	// check if bmp is uncompressed
	if (bmpinfo.biCompression != BI_RGB)
	{
		CloseHandle(file);
		return NULL;
	}

	// check if we have 24 bit bmp
	if (bmpinfo.biBitCount != 24)
	{
		CloseHandle(file);
		return NULL;
	}

	// get image measurements
	*width = bmpinfo.biWidth;
	*height = abs(bmpinfo.biHeight);
	*bitCount = bmpinfo.biBitCount;

	// create buffer to hold the data
	long size = bmpheader.bfSize - bmpheader.bfOffBits;
	BYTE* Buffer = new BYTE[size];
	// move file pointer to start of bitmap data
	SetFilePointer(file, bmpheader.bfOffBits, NULL, FILE_BEGIN);
	// read bmp data
	if (ReadFile(file, Buffer, size, &bytesread, NULL) == false)
	{
		delete[] Buffer;
		CloseHandle(file);
		return NULL;
	}

	// everything successful here: close file and return buffer

	CloseHandle(file);

	return Buffer;
}