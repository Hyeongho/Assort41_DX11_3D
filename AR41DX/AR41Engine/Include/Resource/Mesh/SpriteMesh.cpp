
#include "SpriteMesh.h"

CSpriteMesh::CSpriteMesh()
{
	m_MeshType = MeshType::Sprite;

	SetTypeID<CSpriteMesh>();
}

CSpriteMesh::~CSpriteMesh()
{
}
