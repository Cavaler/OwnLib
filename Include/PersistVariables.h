#if defined DECLARE_PERSIST_VARS
	#define PERSIST_BOOL_VARIABLE(Name, Default) EXTERN BOOL Name VALUE(Default);
	#define PERSIST_bool_VARIABLE(Name, Default) EXTERN bool Name VALUE(Default);
	#define PERSIST_STRING_VARIABLE(Name, Default) EXTERN string Name;
	#define PERSIST_TYPED_VARIABLE(Type, Name, Default, Min, Max) EXTERN Type Name VALUE(Default);
	#define PERSIST_FILETIME_VARIABLE(Name) EXTERN FILETIME Name;

#elif defined DECLARE_PERSIST_LOAD
	#define PERSIST_BOOL_VARIABLE(Name, Default) QueryRegBoolValue(DECLARE_PERSIST_LOAD, #Name, &Name, Default);
	#define PERSIST_bool_VARIABLE(Name, Default) QueryRegBoolValue(DECLARE_PERSIST_LOAD, #Name, &Name, Default);
	#define PERSIST_STRING_VARIABLE(Name, Default) QueryRegStringValue(DECLARE_PERSIST_LOAD, #Name, Name, Default);
	#define PERSIST_TYPED_VARIABLE(Type, Name, Default, Min, Max) QueryRegIntValue(DECLARE_PERSIST_LOAD, #Name, (int *)&Name, Default, Min, Max);
	#define PERSIST_FILETIME_VARIABLE(Name) QueryRegIntValue(DECLARE_PERSIST_LOAD, #Name "Lo", &Name.dwLowDateTime, 0xE1D58000); QueryRegIntValue(DECLARE_PERSIST_LOAD, #Name "Hi", &Name.dwHighDateTime, 0x01A8E79F);

#elif defined DECLARE_PERSIST_SAVE
	#define PERSIST_BOOL_VARIABLE(Name, Default) SetRegBoolValue(DECLARE_PERSIST_SAVE, #Name, Name);
	#define PERSIST_bool_VARIABLE(Name, Default) SetRegBoolValue(DECLARE_PERSIST_SAVE, #Name, Name);
	#define PERSIST_STRING_VARIABLE(Name, Default) SetRegStringValue(DECLARE_PERSIST_SAVE, #Name, Name);
	#define PERSIST_TYPED_VARIABLE(Type, Name, Default, Min, Max) SetRegIntValue(DECLARE_PERSIST_SAVE, #Name, Name);
	#define PERSIST_FILETIME_VARIABLE(Name) SetRegIntValue(DECLARE_PERSIST_SAVE, #Name "Lo", Name.dwLowDateTime); SetRegIntValue(DECLARE_PERSIST_SAVE, #Name "Hi", Name.dwHighDateTime);
#else
	#error Don't know what to do
#endif
