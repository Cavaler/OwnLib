#ifdef DEFINE_VARS
#define EXTERN
#define VALUE(n) = n
#define CONSTRUCT(n) n
#else
#define EXTERN extern
#define VALUE(n)
#define CONSTRUCT(n)
#endif

#if defined DECLARE_PERSIST_VARS
	#define PERSIST_BOOL_VARIABLE(Name, Default) EXTERN BOOL Name VALUE(Default);
	#define PERSIST_bool_VARIABLE(Name, Default) EXTERN bool Name VALUE(Default);
	#define PERSIST_STRING_VARIABLE(Name, Default) EXTERN tstring Name;
	#define PERSIST_TYPED_VARIABLE(Type, Name, Default, Min, Max) EXTERN Type Name VALUE(Default);
	#define PERSIST_FILETIME_VARIABLE(Name) EXTERN FILETIME Name;
	
	#define PERSIST_BOOL_VARIABLE_(Name, Key, Default) EXTERN BOOL Name VALUE(Default);
	#define PERSIST_bool_VARIABLE_(Name, Key, Default) EXTERN bool Name VALUE(Default);
	#define PERSIST_STRING_VARIABLE_(Name, Key, Default) EXTERN tstring Name;
	#define PERSIST_TYPED_VARIABLE_(Type, Name, Key, Default, Min, Max) EXTERN Type Name VALUE(Default);
	#define PERSIST_FILETIME_VARIABLE_(Name, Key) EXTERN FILETIME Name;

#elif defined DECLARE_PERSIST_LOAD
	#define PERSIST_BOOL_VARIABLE(Name, Default) QuerySettingsIntValue(DECLARE_PERSIST_LOAD, _T(#Name), &Name, Default, 0, 1);
	#define PERSIST_bool_VARIABLE(Name, Default) QuerySettingsBoolValue(DECLARE_PERSIST_LOAD, _T(#Name), &Name, Default);
	#define PERSIST_STRING_VARIABLE(Name, Default) QuerySettingsStringValue(DECLARE_PERSIST_LOAD, _T(#Name), Name, Default);
	#define PERSIST_TYPED_VARIABLE(Type, Name, Default, Min, Max) QuerySettingsIntValue(DECLARE_PERSIST_LOAD, _T(#Name), (int *)&Name, (int)Default, (int)Min, (int)Max);
	#define PERSIST_FILETIME_VARIABLE(Name) QuerySettingsIntValue<DWORD>(DECLARE_PERSIST_LOAD, _T(#Name) _T("Lo"), &Name.dwLowDateTime, 0xE1D58000); QuerySettingsIntValue<DWORD>(DECLARE_PERSIST_LOAD, _T(#Name) _T("Hi"), &Name.dwHighDateTime, 0x01A8E79F);

	#define PERSIST_BOOL_VARIABLE_(Name, Key, Default) QuerySettingsIntValue(DECLARE_PERSIST_LOAD, Key, &Name, Default, 0, 1);
	#define PERSIST_bool_VARIABLE_(Name, Key, Default) QuerySettingsBoolValue(DECLARE_PERSIST_LOAD, Key, &Name, Default);
	#define PERSIST_STRING_VARIABLE_(Name, Key, Default) QuerySettingsStringValue(DECLARE_PERSIST_LOAD, Key, Name, Default);
	#define PERSIST_TYPED_VARIABLE_(Type, Name, Key, Default, Min, Max) QuerySettingsIntValue(DECLARE_PERSIST_LOAD, Key, (int *)&Name, (int)Default, (int)Min, (int)Max);
	#define PERSIST_FILETIME_VARIABLE_(Name, Key) QuerySettingsIntValue<DWORD>(DECLARE_PERSIST_LOAD, Key _T("Lo"), &Name.dwLowDateTime, 0xE1D58000); QuerySettingsIntValue<DWORD>(DECLARE_PERSIST_LOAD, Key _T("Hi"), &Name.dwHighDateTime, 0x01A8E79F);

#elif defined DECLARE_PERSIST_SAVE
	#define PERSIST_BOOL_VARIABLE(Name, Default) SetSettingsIntValue(DECLARE_PERSIST_SAVE, _T(#Name), Name);
	#define PERSIST_bool_VARIABLE(Name, Default) SetSettingsIntValue(DECLARE_PERSIST_SAVE, _T(#Name), Name);
	#define PERSIST_STRING_VARIABLE(Name, Default) SetSettingsStringValue(DECLARE_PERSIST_SAVE, _T(#Name), Name);
	#define PERSIST_TYPED_VARIABLE(Type, Name, Default, Min, Max) SetSettingsIntValue(DECLARE_PERSIST_SAVE, _T(#Name), Name);
	#define PERSIST_FILETIME_VARIABLE(Name) SetSettingsIntValue(DECLARE_PERSIST_SAVE, _T(#Name) _T("Lo"), Name.dwLowDateTime); SetSettingsIntValue(DECLARE_PERSIST_SAVE, _T(#Name) _T("Hi"), Name.dwHighDateTime);

	#define PERSIST_BOOL_VARIABLE_(Name, Key, Default) SetSettingsIntValue(DECLARE_PERSIST_SAVE, Key, Name);
	#define PERSIST_bool_VARIABLE_(Name, Key, Default) SetSettingsIntValue(DECLARE_PERSIST_SAVE, Key, Name);
	#define PERSIST_STRING_VARIABLE_(Name, Key, Default) SetSettingsStringValue(DECLARE_PERSIST_SAVE, Key, Name);
	#define PERSIST_TYPED_VARIABLE_(Type, Name, Key, Default, Min, Max) SetSettingsIntValue(DECLARE_PERSIST_SAVE, Key, Name);
	#define PERSIST_FILETIME_VARIABLE_(Name, Key) SetSettingsIntValue(DECLARE_PERSIST_SAVE, Key _T("Lo"), Name.dwLowDateTime); SetSettingsIntValue(DECLARE_PERSIST_SAVE, Key _T("Hi"), Name.dwHighDateTime);
#else
	#error Don't know what to do
#endif
