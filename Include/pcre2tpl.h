#ifndef __PCRE2TEMPLATES_H
#define __PCRE2TEMPLATES_H

#ifndef PCRE2_STATIC
#define PCRE2_STATIC
#endif

#ifdef UNICODE
#define PCRE2_CODE_UNIT_WIDTH 16
#else
#define PCRE2_CODE_UNIT_WIDTH 8
#endif

#include <pcre2\pcre2.h>

#undef PCRE2_SPTR
#undef pcre2_compile_context
#undef pcre2_code
#undef pcre2_compile

#undef pcre2_general_context
#undef pcre2_match_data
#undef pcre2_match_data_create_from_pattern
#undef pcre2_get_ovector_count
#undef pcre2_get_ovector_pointer

#undef pcre2_match_context
#undef pcre2_match

#undef pcre2_code_free
#undef pcre2_match_data_free

template<class CHAR>
struct pcre2
{
	typedef void pcre2_code;
	typedef void pcre2_match_data;
};

template<>
struct pcre2<char>
{
	typedef PCRE2_SPTR8 PCRE2_SPTR;
	typedef pcre2_compile_context_8 pcre2_compile_context;
	typedef pcre2_code_8 pcre2_code;
	typedef pcre2_general_context_8 pcre2_general_context;
	typedef pcre2_match_data_8 pcre2_match_data;
	typedef pcre2_match_context_8 pcre2_match_context;

	static pcre2_code *pcre2_compile(PCRE2_SPTR pattern, PCRE2_SIZE length, uint32_t options, int *errorcode, PCRE2_SIZE *erroroffset, pcre2_compile_context *ccontext = NULL)
	{
		return pcre2_compile_8(pattern, length, options, errorcode, erroroffset, ccontext);
	}

	static pcre2_match_data *pcre2_match_data_create_from_pattern(const pcre2_code *code, pcre2_general_context *gcontext = NULL)
	{
		return pcre2_match_data_create_from_pattern_8(code, gcontext);
	}

	static uint32_t pcre2_get_ovector_count(pcre2_match_data *data)
	{
		return pcre2_get_ovector_count_8(data);
	}

	static PCRE2_SIZE *pcre2_get_ovector_pointer(pcre2_match_data *data)
	{
		return pcre2_get_ovector_pointer_8(data);
	}

	static int pcre2_match(const pcre2_code *code, PCRE2_SPTR subject, PCRE2_SIZE length, PCRE2_SIZE startoffset, uint32_t options, pcre2_match_data *match_data, pcre2_match_context *mcontext = NULL)
	{
		return pcre2_match_8(code, subject, length, startoffset, options, match_data, mcontext);
	}

	static void pcre2_code_free(pcre2_code *code)
	{
		pcre2_code_free_8(code);
	}

	static void pcre2_match_data_free(pcre2_match_data *match_data)
	{
		pcre2_match_data_free_8(match_data);
	}
};

template<>
struct pcre2<wchar_t>
{
	typedef PCRE2_SPTR16 PCRE2_SPTR;
	typedef pcre2_compile_context_16 pcre2_compile_context;
	typedef pcre2_code_16 pcre2_code;
	typedef pcre2_general_context_16 pcre2_general_context;
	typedef pcre2_match_data_16 pcre2_match_data;
	typedef pcre2_match_context_16 pcre2_match_context;

	static pcre2_code *pcre2_compile(PCRE2_SPTR pattern, PCRE2_SIZE length, uint32_t options, int *errorcode, PCRE2_SIZE *erroroffset, pcre2_compile_context *ccontext = NULL)
	{
		return pcre2_compile_16(pattern, length, options, errorcode, erroroffset, ccontext);
	}

	static pcre2_match_data *pcre2_match_data_create_from_pattern(const pcre2_code *code, pcre2_general_context *gcontext = NULL)
	{
		return pcre2_match_data_create_from_pattern_16(code, gcontext);
	}

	static uint32_t pcre2_get_ovector_count(pcre2_match_data *data)
	{
		return pcre2_get_ovector_count_16(data);
	}

	static PCRE2_SIZE *pcre2_get_ovector_pointer(pcre2_match_data *data)
	{
		return pcre2_get_ovector_pointer_16(data);
	}

	static int pcre2_match(const pcre2_code *code, PCRE2_SPTR subject, PCRE2_SIZE length, PCRE2_SIZE startoffset, uint32_t options, pcre2_match_data *match_data, pcre2_match_context *mcontext = NULL)
	{
		return pcre2_match_16(code, subject, length, startoffset, options, match_data, mcontext);
	}

	static void pcre2_code_free(pcre2_code *code)
	{
		pcre2_code_free_16(code);
	}

	static void pcre2_match_data_free(pcre2_match_data *match_data)
	{
		pcre2_match_data_free_16(match_data);
	}
};

#endif // __PCRE2TEMPLATES_H