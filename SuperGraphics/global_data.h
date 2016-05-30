#ifndef _GLOBAL_H_
#define _GLOBAL_H_

#include <Windows.h>

#define max(a,b)            (((a) > (b)) ? (a) : (b))
#define min(a,b)            (((a) < (b)) ? (a) : (b))

#define g_Pipeline (*SPipeline::getInstance())

inline void mySetPixel(int x, int y, float z, tagRGBQUAD color); 
void testAndSetPixel(int x, int y, float z, tagRGBQUAD color);

float getDepth(int x, int y);

#endif //_GLOBAL_H_