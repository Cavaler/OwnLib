#pragma once

class CFarSettingsKey
{
public:
	CFarSettingsKey();
	CFarSettingsKey(const CFarSettingsKey &Key);
	void operator =(const CFarSettingsKey &Key);
	~CFarSettingsKey();

	bool OpenRoot(LPCTSTR szRootKey);
	bool Open(CFarSettingsKey &Key, LPCTSTR szSubKey, bool bCreate = true);
	bool Valid() const;
	CFarSettingsKey Open(LPCTSTR szSubKey, bool bCreate = true);

	void SetStringValue(LPCTSTR pszKeyName, LPCTSTR szValue);
	void SetStringValue(LPCTSTR pszKeyName, const tstring &strValue) { return SetStringValue(pszKeyName, strValue.c_str()); }
	void SetIntValue   (LPCTSTR pszKeyName, __int64 nValue);
	void SetBoolValue  (LPCTSTR pszKeyName, bool bValue) { SetIntValue(pszKeyName, bValue ? 1 : 0); }

	void StartEnumKeys();
	void StartEnumValues();
	tstring GetNextEnum();

	bool DeleteKey(LPCTSTR szSubKey);
	bool DeleteValue(LPCTSTR szSubKey);
	void DeleteAllKeys();
	void DeleteAllValues();

	void Close();
#ifdef FAR3
public:
	size_t	m_Key;
	struct sHandle {
		sHandle(HANDLE hHandle) : m_Handle(hHandle), m_dwRef(1) {}
		HANDLE	m_Handle;
		DWORD	m_dwRef;
	};
	sHandle	*m_pHandle;

	FarSettingsEnum	m_Enum;
	size_t			m_nEnum;
	bool			m_bKeys;
#else
	operator HKEY() { return m_pHandle->m_Key; }
protected:
	struct sHandle {
		sHandle(HKEY hKey) : m_Key(hKey), m_dwRef(1) {}
		CHKey	m_Key;
		DWORD	m_dwRef;
	};
	sHandle	*m_pHandle;
#endif
};

#ifdef FAR3
void QuerySettingsStringValue(CFarSettingsKey &Key, const TCHAR *pszKeyName, std::tstring &strBuffer, const TCHAR *pszDefault);
__int64 QuerySettingsInt64Value   (CFarSettingsKey &Key, const TCHAR *pszKeyName, __int64 nDefault);
void QuerySettingsBoolValue  (CFarSettingsKey &Key, const TCHAR *pszKeyName, bool *bValue, bool bDefault);
void QuerySettingsBinaryValue(CFarSettingsKey &Key, const TCHAR *pszKeyName, std::vector<BYTE> &arrData);

template<typename IntType>
void QuerySettingsIntValue   (CFarSettingsKey &Key, const TCHAR *pszKeyName, IntType *nValue, IntType nDefault)
{
	*nValue = (IntType)QuerySettingsInt64Value(Key, pszKeyName, nDefault);
}

template<typename IntType>
void QuerySettingsIntValue   (CFarSettingsKey &Key, const TCHAR *pszKeyName, IntType *nValue, IntType nDefault, IntType nMin)
{
	*nValue = (IntType)QuerySettingsInt64Value(Key, pszKeyName, nDefault);

	if (*nValue < nMin) {
		*nValue = nDefault;
	}
}

template<typename IntType>
void QuerySettingsIntValue   (CFarSettingsKey &Key, const TCHAR *pszKeyName, IntType *nValue, IntType nDefault, IntType nMin, IntType nMax)
{
	*nValue = (IntType)QuerySettingsInt64Value(Key, pszKeyName, nDefault);

	if ((*nValue < nMin) || (*nValue > nMax)) {
		*nValue = nDefault;
	}
}

void SetSettingsStringValue(CFarSettingsKey &Key, const TCHAR *pszKeyName, const std::tstring &strBuffer);
void SetSettingsIntValue   (CFarSettingsKey &Key, const TCHAR *pszKeyName, __int64 nValue);
void SetSettingsBinaryValue(CFarSettingsKey &Key, const TCHAR *pszKeyName, const void *pData, int nLength);
#endif
