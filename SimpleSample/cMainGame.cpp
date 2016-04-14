#include "DXUT.h"
#include "cMainGame.h"
#include "cSystemMgr.h"
#include "cResourceMgr.h"
#include "cObjectMgr.h"
#include "cSceneMgr.h"

cMainGame::cMainGame()
{
}


cMainGame::~cMainGame()
{
	Release();
}


void	cMainGame::Initialize()
{
	srand(GetTickCount());
	DXUTGetD3D9Device()->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
	DXUTGetD3D9Device()->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);
	DXUTGetD3D9Device()->SetRenderState(D3DRS_LIGHTING, false);
	DXUTGetD3D9Device()->SetRenderState(D3DRS_ALPHABLENDENABLE, true);
	DXUTGetD3D9Device()->SetRenderState(D3DRS_FOGENABLE, true);
	DXUTGetD3D9Device()->SetRenderState(D3DRS_FOGVERTEXMODE, D3DFOG_NONE);
	DXUTGetD3D9Device()->SetRenderState(D3DRS_FOGCOLOR, D3DCOLOR_XRGB( 0, 0, 0));
	DXUTGetD3D9Device()->SetRenderState(D3DRS_FOGTABLEMODE, D3DFOG_LINEAR);

	float	fStart = 15.0f;
	float	fEnd = 350.0f;
	float	fDesnity = 0.001f;
	DXUTGetD3D9Device()->SetRenderState(D3DRS_FOGSTART, *((LPDWORD)(&fStart)));
	DXUTGetD3D9Device()->SetRenderState(D3DRS_FOGEND, *((LPDWORD)(&fEnd)));
	DXUTGetD3D9Device()->SetRenderState(D3DRS_FOGDENSITY, *((LPDWORD)(&fDesnity)));

	
	_GETSINGLE(cResourceMgr)->AddAseFile("woman", "Data/woman.ase");
	_GETSINGLE(cResourceMgr)->AddAseFile("player", "Data/Player/player.ase");
//	_GETSINGLE(cResourceMgr)->AddAseFile("player_add1", "Data/Player/Add/add 1.ASE");
	_GETSINGLE(cResourceMgr)->AddAseFile("player_add2", "Data/Player/Add/add on 3.ASE");
	_GETSINGLE(cResourceMgr)->AddAseFile("enemy", "Data/Enemy/wjr1.ase");
	
	_GETSINGLE(cResourceMgr)->AddAseFile("enemy2", "Data/Enemy/wjr2.ase");
	_GETSINGLE(cResourceMgr)->AddAseFile("enemy3", "Data/Enemy/3/wjr3.ase");
	
	_GETSINGLE(cResourceMgr)->AddAseFile("miniboss", "Data/Enemy/Boss/wndrks boss 1.ase");
	_GETSINGLE(cResourceMgr)->AddAseFile("boss", "Data/Monster/boss1.ase");
	
	
	_GETSINGLE(cResourceMgr)->AddAseFile("plan", "Data/plan.ase");
	_GETSINGLE(cResourceMgr)->AddAseFile("car1", "Data/Temp/Car1 0.ase");
	_GETSINGLE(cResourceMgr)->AddAseFile("player_car", "Data/Car/Player/player car.ase");
	_GETSINGLE(cResourceMgr)->AddAseFile("player_car_wheel", "Data/Car/Player/qkznl.ase");
	_GETSINGLE(cResourceMgr)->AddAseFile("bullet", "Data/Bullet/chddkf1.ase");
	_GETSINGLE(cResourceMgr)->AddAseFile("bullet_enemy1", "Data/Bullet/EnemyBullet/rlqhs wjr attack.ase");
	_GETSINGLE(cResourceMgr)->AddAseFile("bullet_enemy2", "Data/Bullet/EnemyBullet/attack1.ase");
	
	_GETSINGLE(cResourceMgr)->AddAseFile("player_gun", "Data/Temp/player gun1.ase");
	_GETSINGLE(cResourceMgr)->AddAseFile("effect_boost", "Data/Effect/Boost/fever booster.ase");
	_GETSINGLE(cResourceMgr)->AddAseFile("effect_normal_boost", "Data/Effect/Boost/rlqhs booster.ase");
	_GETSINGLE(cResourceMgr)->AddAseFile("effect_laser", "Data/Effect/Laser/lazer.ase");
	
	_GETSINGLE(cResourceMgr)->AddTexture("effect_bullet", L"Data/Effect/Effect_Bullet%d.png", 4);
	_GETSINGLE(cResourceMgr)->AddTexture("effect_bomb", L"Data/Effect/Bomb/%d.png", 10);
	_GETSINGLE(cResourceMgr)->AddTexture("effect_item", L"Data/Effect/ItemEff/%d.png", 6);
	_GETSINGLE(cResourceMgr)->AddTexture("effect_shield", L"Data/Effect/shield.PNG" );
	
	
	_GETSINGLE(cResourceMgr)->AddSprite("ui_number", L"Data/Num/%d.png", 10);

	_GETSINGLE(cResourceMgr)->AddSprite("ui_background", L"Data/UI/back.png");
	_GETSINGLE(cResourceMgr)->AddSprite("ui_notice_add", L"Data/UI/add on.png");
	_GETSINGLE(cResourceMgr)->AddSprite("ui_notice_heal", L"Data/UI/HP UP.png");
	_GETSINGLE(cResourceMgr)->AddSprite("ui_notice_missle", L"Data/UI/missile.png");
	_GETSINGLE(cResourceMgr)->AddSprite("ui_notice_power", L"Data/UI/power up.png");
	_GETSINGLE(cResourceMgr)->AddSprite("ui_notice_shield", L"Data/UI/shield.png");
	_GETSINGLE(cResourceMgr)->AddSprite("ui_notice_speed", L"Data/UI/speed up.png");
	_GETSINGLE(cResourceMgr)->AddSprite("ui_notice_warning", L"Data/UI/warning.png");
	_GETSINGLE(cResourceMgr)->AddSprite("ui_notice_st1", L"Data/UI/stage1.png");
	_GETSINGLE(cResourceMgr)->AddSprite("ui_notice_st2", L"Data/UI/stage2.png");
	_GETSINGLE(cResourceMgr)->AddSprite("ui_bar_player", L"Data/UI/player hp.png");
	_GETSINGLE(cResourceMgr)->AddSprite("ui_bar_boss", L"Data/UI/boss hp.png");
	_GETSINGLE(cResourceMgr)->AddSprite("ui_back_hit", L"Data/Effect/Back/xkrur.png");
	_GETSINGLE(cResourceMgr)->AddSprite("ui_back_normal", L"Data/Effect/Back/rlqhs.png");

	_GETSINGLE(cResourceMgr)->AddSprite("ui_main_background", L"Data/UI/Main/main.png");
	_GETSINGLE(cResourceMgr)->AddSprite("ui_fade_background", L"Data/UI/fade.png");
	
	_GETSINGLE(cResourceMgr)->AddSprite("ui_main_about_button", L"Data/UI/Main/about%d.png", 3);
	_GETSINGLE(cResourceMgr)->AddSprite("ui_main_howto_button", L"Data/UI/Main/how to%d.png", 3);
	_GETSINGLE(cResourceMgr)->AddSprite("ui_main_rank_button", L"Data/UI/Main/ranking%d.png", 3);
	_GETSINGLE(cResourceMgr)->AddSprite("ui_main_start_button", L"Data/UI/Main/start%d.png", 3);
	_GETSINGLE(cResourceMgr)->AddSprite("ui_main_rank_bar", L"Data/UI/Main/rankbar.png");
	
	
	
	_GETSINGLE(cResourceMgr)->AddAseFile("structure_stone1", "Data/Strcture/ehf1.ase");
	_GETSINGLE(cResourceMgr)->AddAseFile("structure_stone2", "Data/Strcture/ehf2.ase");
	_GETSINGLE(cResourceMgr)->AddAseFile("structure_stone3", "Data/Strcture/ehf3.ase");
	_GETSINGLE(cResourceMgr)->AddAseFile("structure_stone4", "Data/Strcture/ehf4.ase");
	_GETSINGLE(cResourceMgr)->AddAseFile("structure_stone5", "Data/Strcture/ehf5.ase");
	_GETSINGLE(cResourceMgr)->AddAseFile("structure_stone6", "Data/Strcture/ehf6.ase");
	_GETSINGLE(cResourceMgr)->AddAseFile("structure_stone7", "Data/Strcture/ehf7.ase");
	_GETSINGLE(cResourceMgr)->AddAseFile("structure_cylinder1", "Data/Strcture/rlend1.ase");
	_GETSINGLE(cResourceMgr)->AddAseFile("structure_cylinder2", "Data/Strcture/rlend2.ase");
	_GETSINGLE(cResourceMgr)->AddAseFile("structure_side_stone", "Data/Strcture/side ehf.ase");
	_GETSINGLE(cResourceMgr)->AddAseFile("structure_up_stone1", "Data/Strcture/up stone1.ase");
	_GETSINGLE(cResourceMgr)->AddAseFile("structure_up_stone2", "Data/Strcture/up stone2.ase");
	_GETSINGLE(cResourceMgr)->AddAseFile("structure_box1", "Data/Strcture/st1/boxl.ase");
	_GETSINGLE(cResourceMgr)->AddAseFile("structure_box2", "Data/Strcture/st1/box ejal.ase");
	_GETSINGLE(cResourceMgr)->AddAseFile("structure_tree", "Data/Strcture/st1/skan.ase");

	_GETSINGLE(cResourceMgr)->AddAseFile("item_add", "Data/Item/item add on.ase");
	_GETSINGLE(cResourceMgr)->AddAseFile("item_missle", "Data/Item/item dbeh.ase");
	_GETSINGLE(cResourceMgr)->AddAseFile("item_heal", "Data/Item/item hill.ase");
	_GETSINGLE(cResourceMgr)->AddAseFile("item_upgrade", "Data/Item/item power up.ase");
	_GETSINGLE(cResourceMgr)->AddAseFile("item_shield", "Data/Item/item shield.ase");
	_GETSINGLE(cResourceMgr)->AddAseFile("item_speed", "Data/Item/item speed .ase");

	
	
	
	_GETSINGLE(cResourceMgr)->AddAseFile("map", "Data/map/map/map.ase");
	_GETSINGLE(cResourceMgr)->AddAseFile("map_left", "Data/map/map/map_left.ase");
	_GETSINGLE(cResourceMgr)->AddAseFile("map_right", "Data/map/map/map_right.ase");
	_GETSINGLE(cResourceMgr)->AddAseFile("map_under", "Data/map/map/map_under.ase");
	_GETSINGLE(cResourceMgr)->AddAseFile("map_up", "Data/map/map/map_up.ase");

	/*
	
	_GETSINGLE(cResourceMgr)->AddAseFile("map_s_front_floor", "Data/map/SectionMap/map floor.ase");
	_GETSINGLE(cResourceMgr)->AddAseFile("map_s_front", "Data/map/SectionMap/map.ase");
	
	_GETSINGLE(cResourceMgr)->AddAseFile("map_s_left_floor", "Data/map/SectionMap/map left floor.ase");
	_GETSINGLE(cResourceMgr)->AddAseFile("map_s_left", "Data/map/SectionMap/map left.ase");
	_GETSINGLE(cResourceMgr)->AddAseFile("map_s_right_floor", "Data/map/SectionMap/map right floor.ase");
	_GETSINGLE(cResourceMgr)->AddAseFile("map_s_right", "Data/map/SectionMap/map right.ase");
	*/

	_GETSINGLE(cResourceMgr)->AddShader(SI_CUSTOM, "SimpleSample.fx");
	_GETSINGLE(cResourceMgr)->AddShader(SI_CREATE, "CreateShadow.fx");
	_GETSINGLE(cResourceMgr)->AddShader(SI_APPLY, "ApplyShadow.fx");
	
	_GETSINGLE(cResourceMgr)->SetPlan(
		_GETSINGLE(cResourceMgr)->GetResource("plan"));
	

	_GETSINGLE(cSystemMgr)->Initialize();
	_GETSINGLE(cObjectMgr)->Initialize();

	_GETSINGLE(cSceneMgr)->Init();


	_GETS(cSceneMgr)->ChangeScene(SI_HOME);
}
void	cMainGame::Update()
{
	_GETSINGLE(cSystemMgr)->Update();
	_GETSINGLE(cObjectMgr)->Update();
}
void	cMainGame::Render()
{
	_GETSINGLE(cObjectMgr)->Render();
	_GETS(cSceneMgr)->Render();
}
void	cMainGame::Release()
{
	_GETSINGLE(cObjectMgr)->Destroy();
	_GETSINGLE(cSystemMgr)->Destroy();
	_GETSINGLE(cSceneMgr)->Destroy();
	_GETSINGLE(cResourceMgr)->Destroy();
}