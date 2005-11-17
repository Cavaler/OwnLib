#ifndef __HANDLES_H
#define __HANDLES_H

template<class HandleType, int Null = (int)INVALID_HANDLE_VALUE, void *Close = CloseHandle> class CHandleTempl {
public:
	CHandle() : m_hHandle((HandleType)Null) {}
	CHandle(HandleType handle) : m_hHandle(handle) {}
	void operator = (HandleType handle) {m_hHandle = handle;}
	~CHandle() {if (m_hHandle != (HandleType)Null) ((CloseFunc)Close)(m_hHandle);}

	operator HandleType &() {return m_hHandle;}
protected:
	typedef void (* CloseFunc)(HandleType _h);
	HandleType m_hHandle;
};

typedef CHandleTempl<HANDLE> CHandle;
typedef CHandleTempl<HKEY, NULL, RegCloseKey> CHKey;

#endif
