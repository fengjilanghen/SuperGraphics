#ifndef _POINT_LIGHT_H
#define _POINT_LIGHT_H

#include "DiyMath3D.h"

enum eLightType
{
	ELT_POINT=0,
	ELT_SPOT,
	ELT_DIRECTIONAL,
	ELT_COUNT
};

struct SLight
{
	eLightType					type;
	DiyMath3D::CVector3f		position;	//direction when type is ELT_DIRECTIONAL
	DiyMath3D::CVector3f		diffuse;
	DiyMath3D::CVector3f		ambient;

	SLight() :position(), diffuse(1.0f, 1.0f, 1.0f), ambient(){}
	SLight(eLightType type, const DiyMath3D::CVector3f &pos, const DiyMath3D::CVector3f &ambient, const DiyMath3D::CVector3f &diffuse)
		:type(type), position(pos), diffuse(diffuse), ambient(ambient){}
};

#endif //_POINT_LIGHT_H