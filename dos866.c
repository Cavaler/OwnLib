typedef unsigned char BYTE;
//typedef int BYTE;

#undef isupper
#define isupper(C) _866_isupper(C)
BYTE _866_isupper(BYTE C) {
	return ((C>=(BYTE)'A')&&(C<=(BYTE)'Z'))||((C>=(BYTE)'Ä')&&(C<=(BYTE)'ü'));
}

#undef islower
#define islower(C) _866_islower(C)
BYTE _866_islower(BYTE C) {
	return ((C>=(BYTE)'a')&&(C<=(BYTE)'z'))||((C>=(BYTE)'†')&&(C<=(BYTE)'Ø'))||((C>=(BYTE)'‡')&&(C<=(BYTE)'Ô'));
}

#undef isalpha
#define isalpha(C) _866_isalpha(C)
BYTE _866_isalpha(BYTE  C) {
	return islower(C)||isupper(C);
}

#undef isalnum
#define isalnum(C) _866_isalnum(C)
BYTE _866_isalnum(BYTE  C) {
	return isalpha(C)||((C>=(BYTE)'0')&&(C<=(BYTE)'9'));
}

#undef toupper
#define toupper(C) _866_toupper(C)
BYTE _866_tolower(BYTE  C) {
	if ((C>=(BYTE)'A')&&(C<=(BYTE)'Z')) return C+(BYTE)'a'-(BYTE)'A';
	if ((C>=(BYTE)'Ä')&&(C<=(BYTE)'è')) return C+(BYTE)'†'-(BYTE)'Ä';
	if ((C>=(BYTE)'ê')&&(C<=(BYTE)'ü')) return C+(BYTE)'‡'-(BYTE)'ê';
	return C;
}

#undef tolower
#define tolower(C) _866_tolower(C)
BYTE _866_toupper(BYTE  C) {
	if ((C>=(BYTE)'a')&&(C<=(BYTE)'z')) return C+(BYTE)'A'-(BYTE)'a';
	if ((C>=(BYTE)'†')&&(C<=(BYTE)'Ø')) return C+(BYTE)'Ä'-(BYTE)'†';
	if ((C>=(BYTE)'‡')&&(C<=(BYTE)'Ô')) return C+(BYTE)'ê'-(BYTE)'‡';
	return C;
}
