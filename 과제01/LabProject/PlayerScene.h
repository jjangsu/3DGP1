#pragma once
#include "Scene.h"

class CPlayerScene : public CScene
{
public:
	CPlayerScene();
	virtual ~CPlayerScene();

	virtual void BuildObjects();
};