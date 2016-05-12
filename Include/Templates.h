#ifndef __TEMPLATES_H
#define __TEMPLATES_H

template<typename ptr>
ptr *safe_realloc(ptr *p, size_t size)
{
	if (size == 0)
	{
		if (p) free(p);
		return (ptr *)malloc(0);
	}

	ptr *n = (ptr *)realloc(p, size);
	if (n) return n;
	free(p);
	return NULL;
}

#endif
