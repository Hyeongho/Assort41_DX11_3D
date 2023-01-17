#include "AnimationUpdateShader.h"

CAnimationUpdateShader::CAnimationUpdateShader()
{
	SetTypeID<CAnimationUpdateShader>();
}

CAnimationUpdateShader::~CAnimationUpdateShader()
{
}

bool CAnimationUpdateShader::Init()
{
	if (!LoadComputeShader("AnimationUpdateCS", TEXT("AnimationUpdate.fx"),
		SHADER_PATH))
		return false;

	return true;
}
