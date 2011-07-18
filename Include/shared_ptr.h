#pragma once

#include <algorithm> // for std::swap

#if _MSC_VER >= 1500 && _MSC_FULL_VER > 150021022

#include <memory>
using std::tr1::shared_ptr;

#else

// Simple analog of boost::shared_ptr

template < class T > class shared_ptr
{
public:
  typedef T value_type;

public:
  shared_ptr()
    : m_pN(0), m_p(0)
  {}
  
  ~shared_ptr()
  {
    release();
  }

  shared_ptr( const shared_ptr & rhs )
    : m_pN(rhs.m_pN), m_p(rhs.m_p)
  {
    add_ref();
  }

  shared_ptr operator= ( const shared_ptr & rhs )
  {
    shared_ptr(rhs).swap(*this);
    return *this;
  }

  shared_ptr( int n )
    : m_pN(0), m_p(0)
  {
    ATLASSERT(n == 0);
  }

  shared_ptr & operator= ( int n )
  {
    ATLASSERT(n == 0);
    release();
    return *this;
  }

  template < class U > shared_ptr( const shared_ptr<U> & rhs )
    : m_pN(rhs.m_pN), m_p(rhs.m_p)
  {
    add_ref();
  }
  
  template < class U > shared_ptr & operator= ( const shared_ptr<U> & rhs )
  {
    shared_ptr(rhs).swap(*this);
    return *this;
  }

  template < class U > shared_ptr( U * p )
    : m_pN(p ? new size_t(1) : 0), m_p(p)
  {}

  template < class U > shared_ptr & operator= ( U * p )
  {
    shared_ptr(p).swap(*this);
    return *this;
  }

  T & operator* () const { return *m_p; }
  T * operator-> () const { return m_p; }
  operator T * () const { return m_p; }

  operator bool () const { return m_p != 0; }
  bool operator! () const { return m_p == 0; }

  void reset() { *this = 0; }
  template < class U > void reset( U * p ) { *this = p; }

  T * get() const { return m_p; }

  void swap( shared_ptr & rhs )
  {
    std::swap(m_pN, rhs.m_pN);
    std::swap(m_p, rhs.m_p);
  }

private:
  void add_ref()
  {
    if ( m_pN )
      ++*m_pN;
  }

  void release()
  {
    if ( (m_p != 0) && (--*m_pN == 0) )
    {
      delete m_pN;
      delete m_p;
    }

    m_pN  = 0;
    m_p   = 0;
  }

private:
  friend class shared_ptr;

private:
  size_t * m_pN;
  T *      m_p;
};

template < class T > inline void swap( shared_ptr<T> & a, shared_ptr<T> & b )
{
  a.swap(b);
}

template < class T, class U > inline bool operator== ( const shared_ptr<T> & a, const shared_ptr<U> & b )
{
  return a.get() == b.get();
}

template < class T, class U  > inline bool operator!= ( const shared_ptr<T> & a, const shared_ptr<U> & b )
{
  return a.get() != b.get();
}

#endif // _MSC_VER >= 1500
