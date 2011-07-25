/***********************************************************
	@doc
	@module MD5.H |

	Header file for MD5.CPP

	Copyright (C) 1991-2, RSA Data Security, Inc. Created 1991.
	All rights reserved.

	License to copy and use this software is granted provided that it
	is identified as the "RSA Data Security, Inc. MD5 Message-Digest
	Algorithm" in all material mentioning or referencing this software
	or this function.

	License is also granted to make and use derivative works provided
	that such works are identified as "derived from the RSA Data
	Security, Inc. MD5 Message-Digest Algorithm" in all material
	mentioning or referencing the derived work.

	RSA Data Security, Inc. makes no representations concerning either
	the merchantability of this software or the suitability of this
	software for any particular purpose. It is provided "as is"
	without express or implied warranty of any kind.

	These notices must be retained in any copies of any part of this
	documentation and/or software.
***********************************************************/

#ifndef __MD5X_H
#define __MD5X_H

#define WIN32_LEAN_AND_MEAN
#define STRICT
#include <windows.h>

#define MD5_MSG_DIGEST_SIZE ( 16 )

class CMD5 {
private:
	DWORD m_State[4];	// state (ABCD)
	DWORD m_Count[2];	// number of bits, modulo 2^64 (lsb first)
	BYTE m_Buffer[64];	// input buffer

	//@cmember Transformation.
	void Transform(const BYTE block[64]);

public:
	CMD5();
	CMD5(const BYTE* InpBuf, UINT InpLen);

	void Update(const BYTE* InpBuf, UINT InpLen);
	void Update(const char *szString);
	void Update(const wchar_t *szString, int nCP = CP_ACP);

	void Final(BYTE Result[16]);
	void FinalHex(char *Buffer);
	void FinalHex(wchar_t *Buffer);
};

#endif
