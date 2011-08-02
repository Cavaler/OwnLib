#pragma once

#include "StringEx.h"

class CDebugTimer {
public:
	inline CDebugTimer(const TCHAR *szMessage) : m_strMessage(szMessage), m_dwStart(GetTickCount()) {}

	inline void Stop() {
		OutputDebugString(FormatStr(m_strMessage.c_str(), GetTickCount() - m_dwStart).c_str());
		OutputDebugString(_T("\r\n"));
		m_dwStart = 0;
	}

	inline ~CDebugTimer() {
		if (m_dwStart) Stop();
	}

protected:
	tstring m_strMessage;
	DWORD m_dwStart;
};
