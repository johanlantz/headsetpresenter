// ----------------------------------------------------------------------- //
//
//  FILENAME:	SS_List.h
//  AUTHOR:		Steve Schaneville
//  CREATED:	28 Jan 2002, 09:04
//
//  PURPOSE:	
//
//  Copyright (c) 2002
//
// ----------------------------------------------------------------------- //
#ifndef __SS_List_h__
#define __SS_List_h__

// ------------------[       Pre-Include Defines       ]------------------ //
// ------------------[          Include Files          ]------------------ //
#include <wtypes.h>
#include <assert.h>

// ------------------[          Macros/Defines         ]------------------ //
#ifndef ASSERT
#define ASSERT assert
#endif

// ------------------[      Constants/Enumerations     ]------------------ //
// ------------------[       Forward Declarations      ]------------------ //
// ------------------[         Global Variables        ]------------------ //
// ------------------[         Global Functions        ]------------------ //
// ------------------[             Classes             ]------------------ //

// ----------------------------------------------------------------------- //
//  Class:			SS_List
//  Author:			Steve Schaneville
//  Notes:			
// ----------------------------------------------------------------------- //
template <class T>
class SS_List
{
public:

    // construction, destruction
    SS_List();
    virtual ~SS_List();

    // copy and assignment
    SS_List(SS_List& rhs) { InitObject(); *this = rhs; }
    SS_List& operator = (SS_List& rhs)
    {
        RemoveAll();
        InitObject(); 
        Iterator it = rhs.Beginning();
        while( it.IsValid() )
        {
            InsertEnd(it.Current());
            ++it;
        }        
        return *this;
    }

    // Helper sub-class
    class Object
    {
    public:
        Object() { m_pNext = NULL; m_pPrev = NULL; }
        ~Object() {}

        T        m_Value;

        Object*  m_pNext;
        Object*  m_pPrev;
    };
    
    class Iterator
    {
    public:

        Iterator() { InitObject(&Object()); }
        Iterator(Object* pObject) { InitObject(pObject); }
        
        VOID InitObject(Object* pObject) { m_pCurrent = pObject; }        
        VOID operator = (Object* pObject) { InitObject(pObject); }

        T& operator ++ () 
        { 
            if( m_pCurrent->m_pNext )
                InitObject(m_pCurrent->m_pNext);
            else
                InitObject(NULL);

            return Current();
        }

        T& operator -- ()
        { 
            if( m_pCurrent->m_pPrev )
                InitObject(m_pCurrent->m_pPrev);
            else
                InitObject(NULL);
            
            return Current();
        }

        BOOL operator != (Object* rhs) { return m_pCurrent != rhs ? TRUE : FALSE; }
        BOOL operator == (Object* rhs) { return m_pCurrent == rhs ? TRUE : FALSE; }
        
        T& operator -> () { return Current(); }
		operator T () { return Current(); }
        
        T& Current() { return m_pCurrent->m_Value; }
        T& Next() { ++(*this); return Current(); }
        T& Prev() { --(*this); return Current(); }

        BOOL IsLast() { return (m_pCurrent->m_pNext == NULL) ? TRUE : FALSE; }
        BOOL IsFirst() { return (m_pCurrent->m_pPrev == NULL) ? TRUE : FALSE; }
        BOOL IsValid() { return (m_pCurrent != NULL) ? TRUE : FALSE; }
        
    private:

        Object* m_pCurrent;

    };
    
protected:

    // initialization
    virtual VOID	            InitObject		        ();

public:

	// accessor functions
    Object*                     Beginning               ();
    Object*                     End                     ();
    
    // utilities
    VOID                        InsertBeginning         (T object);
    VOID                        InsertEnd               (T object);
    VOID                        Insert                  (T object, INT nIndex);
    VOID                        Remove                  (INT nIndex);
    VOID                        RemoveAll               ();
    BOOL                        RemoveCurrent           (Iterator& it);
    
    INT                         Count                   ();
    INT                         IncrementCount          ();
    INT                         DecrementCount          ();

    T&                          GetAt                   (INT nIndex);
    T&                          operator []             (INT nIndex);
    
protected:

    VOID                        Beginning               (Object* pObject);
    VOID                        End                     (Object* pObject);
    VOID                        Count                   (INT nCount);
    Object*                     GetObjectAt             (INT nIndex);
    
private:

    Object*                     m_pBegin;
    Object*                     m_pEnd;
    INT                         m_nCount;

};


// ----------------------------------------------------------------------- //
//  SS_List Accessor Functions
// ----------------------------------------------------------------------- //

template <class T>
inline SS_List<T>::Object* SS_List<T>::Beginning()
{ return m_pBegin; }

template <class T>
inline SS_List<T>::Object* SS_List<T>::End()
{ return m_pEnd; }

template <class T>
inline VOID SS_List<T>::Beginning(Object* pObject)
{ m_pBegin = pObject; }

template <class T>
inline VOID SS_List<T>::End(Object* pObject)
{ m_pEnd = pObject; }

template <class T>
inline INT SS_List<T>::Count()
{ return m_nCount; }

template <class T>
inline VOID SS_List<T>::Count(INT nCount)
{ m_nCount = nCount; }

template <class T>
inline INT SS_List<T>::IncrementCount()
{ return ++m_nCount; }

template <class T>
inline INT SS_List<T>::DecrementCount()
{ return --m_nCount; }


// ----------------------------------------------------------------------- //
//  SS_List Non-Accessor Functions
// ----------------------------------------------------------------------- //

// Standard Constructor
template <class T>
inline SS_List<T>::SS_List()
{
    InitObject();
}

// Standard Destructor
template <class T>
inline SS_List<T>::~SS_List()
{
	Object* pObject = Beginning();

	while( pObject )
	{
		Object* pNext = pObject->m_pNext;
		delete pObject;
		pObject = pNext;
		
	}
}

// Object Initialization
template <class T>
inline VOID SS_List<T>::InitObject()
{
    Beginning(NULL);
    End(NULL);
    Count(0);
}

template <class T>
inline VOID SS_List<T>::InsertEnd(T value)
{
    // create the new Object and set pointers
    Object*  pObject  = new Object;
    pObject->m_pPrev = End();
    pObject->m_pNext = NULL;
    pObject->m_Value = value;
    
    // fix the previous item in the list
    if( pObject->m_pPrev )
        pObject->m_pPrev->m_pNext = pObject;

    // update the global end and begin pointers
    End(pObject);
    if( 0 == Count() )
        Beginning(pObject);
    
    // update the count
    IncrementCount();
}

template <class T>
inline VOID SS_List<T>::InsertBeginning(T value)
{
    // create the new Object and set pointers
    Object*  pObject  = new Object;
    pObject->m_pPrev = NULL;
    pObject->m_pNext = Beginning();
    pObject->m_Value = value;
    
    // fix the next item in the list
    if( pObject->m_pNext )
        pObject->m_pNext->m_pPrev = pObject;
    
    // update the global end and begin pointers
    Beginning(pObject);
    if( 0 == Count() )
        End(pObject);
    
    // update the count
    IncrementCount();
}

template <class T>
inline VOID SS_List<T>::Insert(T value, INT nIndex)
{
    ASSERT( nIndex <= Count() );

    // create the new Object and set pointers
    Object* pObject = new Object;
    pObject->m_Value = value;

    // fix list pointers in new object
    Object* pObjectNext = NULL;
    Object* pObjectPrev = NULL;

    pObjectNext = (Object*)GetObjectAt(nIndex);
    pObjectPrev = (Object*)GetObjectAt(nIndex-1);

    pObject->m_pNext = pObjectNext;
    pObject->m_pPrev = pObjectPrev;

    // fix list pointers in bracketing objects
    if( NULL != pObjectNext )
        pObjectNext->m_pPrev = pObject;

    if( NULL != pObjectPrev )
        pObjectPrev->m_pNext = pObject;    

    // update the global end and beginning pointers
    if( 0 == nIndex )
        Beginning(pObject);
    
    if( Count() == nIndex )
        End(pObject);
    
    // update the count
    IncrementCount();
}

template <class T>
BOOL SS_List<T>::RemoveCurrent(Iterator& it)
{
    Iterator itLocal = Beginning();
    int i = 0;
    BOOL br = FALSE;

    while(itLocal.IsValid())
    {
        if( &(itLocal.Current()) == &(it.Current()))
        {
            Remove(i);
            br = TRUE;
            break;
        }
        ++itLocal;
        ++i;
    }
    return br;
}

template <class T>
inline VOID SS_List<T>::Remove(INT nIndex)
{
    ASSERT( nIndex < Count() );
    
    // fix list pointers in new object
    Object* pObjectNext = NULL;
    Object* pObjectPrev = NULL;
    Object* pObject = NULL;
    
    pObject = (Object*)GetObjectAt(nIndex);
    pObjectNext = pObject->m_pNext;
    pObjectPrev = pObject->m_pPrev;

    delete pObject;
    pObject = NULL;
    
    // fix list pointers in bracketing objects
    if( NULL != pObjectNext )
        pObjectNext->m_pPrev = pObjectPrev;
    
    if( NULL != pObjectPrev )
        pObjectPrev->m_pNext = pObjectNext;    
    
    // update the global end and beginning pointers
    if( 0 == nIndex )
        Beginning(pObjectNext);
    
    if( Count()-1 == nIndex )
        End(pObjectPrev);
    
    // update the count
    DecrementCount();
}

template <class T>
inline VOID SS_List<T>::RemoveAll()
{
    while( Count() )
        Remove(0);
}
    
template <class T>
inline T& SS_List<T>::GetAt(INT nIndex)
{
    ASSERT( nIndex < Count() && nIndex >= 0 );

    Object* pObject = Beginning();
    for( INT i=0; i<nIndex; i++ )
        pObject = pObject->m_pNext;

    return pObject->m_Value;
}

template <class T>
inline T& SS_List<T>::operator [] (INT nIndex)
{
    return GetAt(nIndex);
}

template <class T>
inline SS_List<T>::Object* SS_List<T>::GetObjectAt(INT nIndex)
{
    Object* pObject = NULL;

    if( nIndex < Count() && nIndex >= 0 )
    {
        pObject = Beginning();
        for( INT i=0; i<nIndex; i++ )
            pObject = pObject->m_pNext;      
    }

    return pObject;
}


#endif // __SS_List_h__
