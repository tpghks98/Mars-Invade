#pragma	once

#define	_SYNTHESIZE_INHER( varType, varName, FunName )	\
	protected:\
	varType	varName;									\
	public:\
	void Set##FunName(varType value) { varName = value; } \
	varType Get##FunName() { return varName; } \
	varType* Get##FunName##Pointer() { return &varName; }


#define	_SYNTHESIZE_REF_INHER( varType, varName, FunName )	\
	protected:\
	varType	varName;									\
	public:\
	void Set##FunName(const varType& value) { varName = value; } \
	const varType& Get##FunName() { return varName; }	\
	varType* Get##FunName##Pointer() { return &varName; }

template <typename T>
class cSingleTon
{
private:
	static T* m_pInst;
public:
	static	T* GetInst()
	{
		if (m_pInst == NULL)
		{
			m_pInst = new T();
		}
		return m_pInst;
	}
	void Destroy()
	{
		if (m_pInst != NULL)
		{
			delete m_pInst;
		}
	}
};

template <typename T> T* cSingleTon<T>::m_pInst = NULL;

#define _GETSINGLE(p) p::GetInst()
#define _GETS(p) p::GetInst()

#define	_IS( str)	\
else if (IsEqual(szToken, str))

#define	_START	\
	int	nLevel = 0;	\
	char* szToken = NULL;	\
	do { \
	szToken = GetToken();	\
	if (IsEqual(szToken, "{")) ++nLevel; \
	else if (IsEqual(szToken, "}")) --nLevel;

#define _END  }while( nLevel > 0 );

#define	_WHILEIT( p ) \
{auto it = p.begin(); \
	auto en_it = p.end(); \
while (it != en_it)	{

#define	_ENDIT(p)	\
if ((*it)->GetDelete()) \
{ \
	SAFE_DELETE(*it); \
	it = p.erase(it); \
	continue; \
} \
(*it)->Update(); \
++it; \
	}	\
}
