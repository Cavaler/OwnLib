//
// Computing HMAC based on MD5
// Author: Pavel Kostromitinov <kostrom@mailru.com>
//

#include <HMAC.h>

void HMAC_MD5(BYTE *InputBuffer,UINT InputLength,BYTE *Key,UINT KeyLength,BYTE Result[16]) {
	BYTE HashedKey[16];
	if (KeyLength>64) {
		CMD5 KeyMD5;
		KeyMD5.Update(Key,KeyLength);
		KeyMD5.Final(HashedKey);
		Key=HashedKey;KeyLength=16;
	}

	CMD5 SubMD5;
	BYTE SubHash[16];
	UINT I;

	for (I=0;I<KeyLength;I++) {
		BYTE B=BYTE(Key[I]^0x36);
		SubMD5.Update(&B,1);
	}
	for (I=KeyLength;I<64;I++) {
		BYTE B=0x36;
		SubMD5.Update(&B,1);
	}
	SubMD5.Update(InputBuffer,InputLength);
	SubMD5.Final(SubHash);

	CMD5 HMAC;
	for (I=0;I<KeyLength;I++) {
		BYTE B=BYTE(Key[I]^0x5C);
		HMAC.Update(&B,1);
	}
	for (I=KeyLength;I<64;I++) {
		BYTE B=0x5C;
		HMAC.Update(&B,1);
	}
	HMAC.Update(SubHash,16);
	HMAC.Final(Result);
}
