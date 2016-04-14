#pragma once
#include "cNode.h"
class cTypeInfo :
	public cNode
{
private:
	const	std::type_info* m_pInfo;

public:
	inline cTypeInfo::cTypeInfo(void)
	{
		class Null {};
		m_pInfo = &typeid(Null);
	}

	inline cTypeInfo::cTypeInfo(const std::type_info& pType)
		:m_pInfo( &pType )
	{
	}

	const std::type_info& cTypeInfo::GetType() const { return *m_pInfo; }
public:
	~cTypeInfo();
};

