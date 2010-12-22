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
	#define PERSIST_BOOL_VARIABLE(Name, Default) QueryRegBoolValue(DECLARE_PERSIST_LOAD, _T(#Name), &Name, Default);
	#define PERSIST_bool_VARIABLE(Name, Default) QueryRegBoolValue(DECLARE_PERSIST_LOAD, _T(#Name), &Name, Default);
	#define PERSIST_STRING_VARIABLE(Name, Default) QueryRegStringValue(DECLARE_PERSIST_LOAD, _T(#Name), Name, Default);
	#define PERSIST_TYPED_VARIABLE(Type, Name, Default, Min, Max) QueryRegIntValue(DECLARE_PERSIST_LOAD, _T(#Name), (int *)&Name, Default, Min, Max);
	#define PERSIST_FILETIME_VARIABLE(Name) QueryRegIntValue(DECLARE_PERSIST_LOAD, _T(#Name) _T("Lo"), &Name.dwLowDateTime, 0xE1D58000); QueryRegIntValue(DECLARE_PERSIST_LOAD, _T(#Name) _T("Hi"), &Name.dwHighDateTime, 0x01A8E79F);

	#define PERSIST_BOOL_VARIABLE_(Name, Key, Default) QueryRegBoolValue(DECLARE_PERSIST_LOAD, Key, &Name, Default);
	#define PERSIST_bool_VARIABLE_(Name, Key, Default) QueryRegBoolValue(DECLARE_PERSIST_LOAD, Key, &Name, Default);
	#define PERSIST_STRING_VARIABLE_(Name, Key, Default) QueryRegStringValue(DECLARE_PERSIST_LOAD, Key, Name, Default);
	#define PERSIST_TYPED_VARIABLE_(Type, Name, Key, Default, Min, Max) QueryRegIntValue(DECLARE_PERSIST_LOAD, Key, (int *)&Name, Default, Min, Max);
	#define PERSIST_FILETIME_VARIABLE_(Name, Key) QueryRegIntValue(DECLARE_PERSIST_LOAD, Key _T("Lo"), &Name.dwLowDateTime, 0xE1D58000); QueryRegIntValue(DECLARE_PERSIST_LOAD, Key _T("Hi"), &Name.dwHighDateTime, 0x01A8E79F);

#elif defined DECLARE_PERSIST_SAVE
	#define PERSIST_BOOL_VARIABLE(Name, Default) SetRegBoolValue(DECLARE_PERSIST_SAVE, _T(#Name), Name);
	#define PERSIST_bool_VARIABLE(Name, Default) SetRegBoolValue(DECLARE_PERSIST_SAVE, _T(#Name), Name);
	#define PERSIST_STRING_VARIABLE(Name, Default) SetRegStringValue(DECLARE_PERSIST_SAVE, _T(#Name), Name);
	#define PERSIST_TYPED_VARIABLE(Type, Name, Default, Min, Max) SetRegIntValue(DECLARE_PERSIST_SAVE, _T(#Name), Name);
	#define PERSIST_FILETIME_VARIABLE(Name) SetRegIntValue(DECLARE_PERSIST_SAVE, _T(#Name) _T("Lo"), Name.dwLowDateTime); SetRegIntValue(DECLARE_PERSIST_SAVE, _T(#Name) _T("Hi"), Name.dwHighDateTime);

	#define PERSIST_BOOL_VARIABLE_(Name, Key, Default) SetRegBoolValue(DECLARE_PERSIST_SAVE, Key, Name);
	#define PERSIST_bool_VARIABLE_(Name, Key, Default) SetRegBoolValue(DECLARE_PERSIST_SAVE, Key, Name);
	#define PERSIST_STRING_VARIABLE_(Name, Key, Default) SetRegStringValue(DECLARE_PERSIST_SAVE, Key, Name);
	#define PERSIST_TYPED_VARIABLE_(Type, Name, Key, Default, Min, Max) SetRegIntValue(DECLARE_PERSIST_SAVE, Key, Name);
	#define PERSIST_FILETIME_VARIABLE_(Name, Key) SetRegIntValue(DECLARE_PERSIST_SAVE, Key _T("Lo"), Name.dwLowDateTime); SetRegIntValue(DECLARE_PERSIST_SAVE, Key _T("Hi"), Name.dwHighDateTime);
#else
	#error Don't know what to do
#endif
