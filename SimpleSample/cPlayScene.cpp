#include "DXUT.h"
#include "cPlayScene.h"
#include "cObjectMgr.h"
#include "shared.h"

cPlayScene::cPlayScene()
{
}


cPlayScene::~cPlayScene()
{
}

void	cPlayScene::Init()
{
	_GETS(cObjectMgr)->GameStart();
}