#pragma	once

#include "cObjBase.h"
#include "cObjectMgr.h"
#include "cResourceMgr.h"

class cBackEff;
template <typename T>
cObjBase* CreateObject(D3DXVECTOR3 vPos = g_vZero, char* pKey = NULL)
{
	cObjBase* pObj = new T();


	if (vPos != g_vZero)
	{
		pObj->SetPosition(vPos);
	}
	pObj->Initialize();

	if (pKey != NULL)
	{
		pObj->SetResource(_GETSINGLE(cResourceMgr)->GetResource(pKey));
	}
	if (typeid(T) == typeid(cEffect))
	{
		_GETSINGLE(cObjectMgr)->m_lstEffect.push_back((cEffect*)pObj);
	}
	else
	{
		if (typeid(T) == typeid(cBackEff))
		{
			_GETSINGLE(cObjectMgr)->m_lstUI.push_back(pObj);
		}
		else
		{
			_GETSINGLE(cObjectMgr)->m_lstObject.push_back(pObj);
		}
	}

	return pObj;
}
