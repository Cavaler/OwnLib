/* config.h for CMake builds */

/* #undef HAVE_DIRENT_H 1 */
#define HAVE_SYS_STAT_H 1
#define HAVE_SYS_TYPES_H 1
/* #undef HAVE_UNISTD_H 1 */
#define HAVE_WINDOWS_H 1

/* #undef HAVE_TYPE_TRAITS_H 1 */
/* #undef HAVE_BITS_TYPE_TRAITS_H 1 */

/* #undef HAVE_BCOPY 1 */
#define HAVE_MEMMOVE 1
#define HAVE_STRERROR 1
/* #undef HAVE_STRTOLL 1 */
/* #undef HAVE_STRTOQ 1 */
#define HAVE__STRTOI64 1

#define PCRE_STATIC 1

#define SUPPORT_UTF8 1
#define SUPPORT_UCP 1
/* #undef EBCDIC 1 */
/* #undef BSR_ANYCRLF 1 */
/* #undef NO_RECURSE 1 */

#define HAVE_LONG_LONG 1
#define HAVE_UNSIGNED_LONG_LONG 1

/* #undef SUPPORT_LIBBZ2 1 */
/* #undef SUPPORT_LIBZ 1 */
/* #undef SUPPORT_LIBREADLINE 1 */

#define NEWLINE			10
#define POSIX_MALLOC_THRESHOLD	10
#define LINK_SIZE		2
#define MATCH_LIMIT		10000000
#define MATCH_LIMIT_RECURSION	MATCH_LIMIT


#define MAX_NAME_SIZE	32
#define MAX_NAME_COUNT	10000

/* end config.h for CMake builds */
