#include "DXUT.h"
#include "cHomeScene.h"
#include "cButton.h"
#include "cBackUI.h"
#include "shared.h"

cHomeScene::cHomeScene()
{
}


cHomeScene::~cHomeScene()
{
}

void	cHomeScene::Init()
{
	CreateObject<cBackUI>(g_vMiddle, "ui_main_background");
	CreateObject<cButton>(D3DXVECTOR3(1428, 411, 0), "ui_main_start_button");
	CreateObject<cButton>(D3DXVECTOR3(1428, 583, 0), "ui_main_howto_button");
	CreateObject<cButton>(D3DXVECTOR3(1428, 757, 0), "ui_main_about_button");
	CreateObject<cButton>(D3DXVECTOR3(1428, 927, 0), "ui_main_rank_button");
}