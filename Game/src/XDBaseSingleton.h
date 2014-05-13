/*
*  (c) 2013 XediXermawan < edi.ermawan@gmail.com >
*/
#ifndef __BASE_SINGLETON_H__
#define __BASE_SINGLETON_H__

#include <assert.h>

template < class TYPE > class BaseSingleton {
  private:
    BaseSingleton( const BaseSingleton&);
    BaseSingleton& operator = ( const BaseSingleton&);
    static TYPE* m_Instance;
  protected:
    BaseSingleton()	{};
    virtual ~BaseSingleton()	{
        m_Instance = 0;
    }
  public:
    static void Create()	{
        // you should call this func 1 time only
        assert( m_Instance == 0 );
        if ( !m_Instance )	{
            m_Instance = new TYPE();
        }
    }
    static void Destroy()	{
        if( m_Instance )
            delete m_Instance;
    }
    inline static TYPE* Instance()	{
        assert( m_Instance != 0 );
        return m_Instance;
    }
};

template < class TYPE > TYPE* BaseSingleton< TYPE >::m_Instance = 0 ;

#endif // __BASE_SINGLETON_H__