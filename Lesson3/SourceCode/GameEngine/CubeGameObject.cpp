#include "CubeGameObject.h"
#include "../RenderEngine/CubeRenderProxy.h"

CubeGameObject::CubeGameObject()
{
	m_pRenderProxy = new CubeRenderProxy();
	type = rand() % 3;
}

int CubeGameObject::GetType()
{
	return type;
}