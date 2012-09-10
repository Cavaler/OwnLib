#ifndef __HANDLES_H
#define __HANDLES_H

template<class HandleType, int Null = (int)INVALID_HANDLE_VALUE, void *CloseFn = CloseHandle> class CHandleTempl {
public:
	CHandleTempl() : m_hHandle((HandleType)Null) {}
	CHandleTempl(HandleType handle) : m_hHandle(handle) {}
	void operator =(HandleType handle) {
		if (m_hHandle != handle) {
			Close();
			m_hHandle = handle;
		}
	}
	void Close() {
		if (m_hHandle != (HandleType)Null) {
			((CloseFunc)CloseFn)(m_hHandle);
			m_hHandle = (HandleType)Null;
		}
	}
	~CHandleTempl() {Close();}

	HandleType *operator &() {return &m_hHandle;}
	operator HandleType &() {return m_hHandle;}

	bool Valid() const     {return m_hHandle != (HandleType)Null;}
	operator bool() const  {return m_hHandle != (HandleType)Null;}
	bool operator!() const {return m_hHandle == (HandleType)Null;}
protected:
	typedef void (WINAPI * CloseFunc)(HandleType _h);
	HandleType m_hHandle;
};

typedef CHandleTempl<HANDLE> CHandle;
typedef CHandleTempl<HANDLE, (int)INVALID_HANDLE_VALUE, FindClose> CHFind;
typedef CHandleTempl<HKEY, NULL, RegCloseKey> CHKey;

#endif
