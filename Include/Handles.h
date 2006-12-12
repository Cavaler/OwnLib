#ifndef __HANDLES_H
#define __HANDLES_H

template<class HandleType, int Null = (int)INVALID_HANDLE_VALUE, void *Close = CloseHandle> class CHandleTempl {
public:
	CHandleTempl() : m_hHandle((HandleType)Null) {}
	CHandleTempl(HandleType handle) : m_hHandle(handle) {}
	void operator =(HandleType handle) {m_hHandle = handle;}
	~CHandleTempl() {if (m_hHandle != (HandleType)Null) ((CloseFunc)Close)(m_hHandle);}

	HandleType *operator &() {return &m_hHandle;}
	operator HandleType &() {return m_hHandle;}
	operator bool() {return m_hHandle != (HandleType)Null;}
	bool operator!() {return m_hHandle == (HandleType)Null;}
protected:
	typedef void (WINAPI * CloseFunc)(HandleType _h);
	HandleType m_hHandle;
};

typedef CHandleTempl<HANDLE> CHandle;
typedef CHandleTempl<HANDLE, (int)INVALID_HANDLE_VALUE, FindClose> CHFind;
typedef CHandleTempl<HKEY, NULL, RegCloseKey> CHKey;

#endif
