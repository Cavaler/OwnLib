#if !defined(__PTR_H__)
#define __PTR_H__

#if _MSC_VER > 1000
#pragma once
#endif

/**********************************************************

 Шаблон класса ptr может быть использован для создания
 только простых данных, для которых не требуется
 наличие конструктора - int, char, short и т.д.

 Если Вы хотите работать со сложными типами данных,
 воспользуйтесь шаблоном класса ptrn. В этом случае
 гарантируется вызов конструктора и деструктора
 при создании/удалении объекта с типом "Т".

 Шаблон класса ptrs поможет Вам при работе со строками,
 заканчивающимися нулевым символом.

**********************************************************/

template<class T> class ptr
{
protected:
	typedef T* PT;
	typedef const T* CPT;
	PT m_pData;
	unsigned int m_nLen;

public:
	ptr() : m_pData(NULL), m_nLen(0) {}

	ptr(unsigned int nLen, CPT pData = NULL) : m_pData(NULL), m_nLen(0)
	{
		SetLength(nLen);
		if (NULL != pData)
			memcpy(m_pData, pData, m_nLen * sizeof(T));
	}

	ptr(const ptr& src) : m_pData(NULL), m_nLen(0)
	{
		SetLength(src.m_nLen);
		memcpy(m_pData, src.m_pData, sizeof(T) * m_nLen);
	}

	~ptr()
	{
		m_nLen = 0;
		free(m_pData);
		m_pData = NULL;
	}

	virtual void SetLength(unsigned int nLen)
	{
		m_nLen = nLen;
		m_pData = (PT) realloc(m_pData, m_nLen * sizeof(T));
	}

	virtual unsigned int GetLength() const
	{
		return m_nLen;
	}

	ptr& operator=(CPT pSrc)
	{
		memcpy(m_pData, pSrc, sizeof(T)*m_nLen);
		return *this;
	}

	ptr& operator=(const ptr& src)
	{
		SetLength(src.m_nLen);
		memcpy(m_pData, src.m_pData, sizeof(T)*m_nLen);
		return *this;
	}

	operator CPT() const
	{
		return m_pData;
	}

	operator PT()
	{
		return m_pData;
	}

	PT Detach()
	{
		PT pData = m_pData;
		m_pData = NULL;
		return pData;
	}
};

template<class T> class ptrn : public ptr<T>
{
public:
	ptrn(unsigned int nLen, CPT pData = NULL) : ptr<T>(nLen, pData) {}
	ptrn(CPT pData = NULL) : ptr<T>(pData) {}
	ptrn(const ptrn& src) : ptr<T>(src) {}

	virtual void SetLength(unsigned int nLen)
	{
		PT pData = NULL;
		if (nLen > 0)
			pData = new T[nLen];
		for (unsigned int nPos = min(nLen, m_nLen)-1; nPos >=0; nPos--)
			pData[nPos] = m_pData[nPos];
		delete[] m_pData;
		m_pData = pData;
		m_nLen = nLen;
	}
};

template<class T> class ptrs : public ptr<T>
{
public:
	ptrs(unsigned int nLen, CPT pData = NULL) : ptr<T>(nLen, pData) {}
	ptrs(const ptrs& src) : ptr<T>(src) {}
	ptrs(CPT pData)
	{
		*this = pData;
	}
	ptrs& operator=(CPT pData)
	{
		for (m_nLen = 0; !!pData[m_nLen]; m_nLen++);
		m_nLen++;
		SetLength(m_nLen);
		memcpy(m_pData, pData, m_nLen * sizeof(T));
		return *this;
	}
};

#if defined(_MSC_VER) && !defined (__ICL)

inline template ptrs<char>::ptrs<char>(ptrs::CPT pData)
{
	SetLength(strlen(pData) + 1);
	memcpy(m_pData, pData, m_nLen * sizeof(T));
}

inline template ptrs<wchar_t>::ptrs<wchar_t>(ptrs::CPT pData)
{
	SetLength(wcslen(pData) + 1);
	memcpy(m_pData, pData, m_nLen * sizeof(T));
}

inline template ptrs<char>::ptrs<char>(unsigned int nLen, ptrs::CPT pData)
{
	SetLength(nLen);
	if (NULL != pData)
		strcpy(m_pData, pData);
}

inline template ptrs<wchar_t>::ptrs<wchar_t>(unsigned int nLen, ptrs::CPT pData)
{
	SetLength(nLen);
	if (NULL != pData)
		wcscpy(m_pData, pData);
}

inline template ptrs<char>& ptrs<char>::operator=(ptrs::CPT pSrc)
{
	strncpy(m_pData, pSrc, m_nLen-1);
	m_pData[m_nLen-1] = 0;
	return *this;
}

inline template ptrs<wchar_t>& ptrs<wchar_t>::operator=(ptrs::CPT pSrc)
{
	wcsncpy(m_pData, pSrc, m_nLen-1);
	m_pData[m_nLen-1] = 0;
	return *this;
}

#endif

#endif
