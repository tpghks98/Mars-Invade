#pragma	once

struct ST_POINT
	:public POINT
{
	ST_POINT()
	{
		this->x = 0;
		this->y = 0;
	}

	ST_POINT(int x, int y)
	{
		this->x = x;
		this->y = y;
	}

	ST_POINT(const ST_POINT& pt)
	{
		this->x = pt.x;
		this->y = pt.y;
	}


};


struct ST_MTL_TEX
{
	D3DMATERIAL9 mtl;
	LPDIRECT3DTEXTURE9 pTex;
	LPDIRECT3DTEXTURE9 pBump;
};

struct ST_POS_TRACK
{
	int nKey;
	D3DXVECTOR3 v;
};

struct ST_ROT_TRACK
{
	int nKey;
	D3DXQUATERNION q;
};

struct ST_PNT_VERTEX
{
	D3DXVECTOR3 v;
	D3DXVECTOR3 n;
	D3DXVECTOR2 t;

	enum {
		FVF = D3DFVF_XYZ | D3DFVF_NORMAL | D3DFVF_TEX1
	};
};

class cResourceFile;
struct ST_MAP_DATA
{
	cResourceFile*	pResourceFile;
	cResourceFile*	pTopResourceFile;
	D3DXVECTOR3 vPlusPosition;
	D3DXVECTOR3 vPlusRotation;
};

struct ST_RANK_DATA
{
	std::string strName;
	int nScore;
};