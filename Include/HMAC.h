//
// Computing HMAC based on MD5
// Author: Pavel Kostromitinov <kostrom@mailru.com>
//

#ifndef __HMAC_H
#define __HMAC_H

#include <MD5.h>

void HMAC_MD5(BYTE *InputBuffer,UINT InputLength,BYTE *Key,UINT KeyLength,BYTE Result[16]);

#endif
