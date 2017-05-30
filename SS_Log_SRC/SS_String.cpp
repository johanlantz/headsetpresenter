// ----------------------------------------------------------------------- //
///
/// @file       SS_String.cpp
/// @author     Steve Schaneville
/// @date       27 Nov 2001, 09:44
/// @brief      Implementation file for the SS_String class.
///
///  
///
/// Copyright (c) 2001 - 2003\n
/// z_web_master@yahoo.com\n
/// http://schaneville.webhop.net/SS_Utils
///
// ----------------------------------------------------------------------- //

// ------------------[       Pre-Include Defines       ]------------------ //
// ------------------[          Include Files          ]------------------ //
#include "SS_String.h"

// ------------------[      Macros/Constants/Types     ]------------------ //

/// A simple verify macro used in several functions, just to save typing.
#define VERIFY_POINTER(p)           \
{                                   \
    if( !p )                        \
    {                               \
        ASSERT(!_T("bad pointer")); \
        return NULL;                \
    }                               \
}

// ------------------[         Global Variables        ]------------------ //
// ------------------[         Global Functions        ]------------------ //
// ------------------[    Class Function Definitions   ]------------------ //

// ----------------------------------------------------------------------- //
/// The default constructor.
///
/// @fn	        SS_String::SS_String()
/// @return     none
/// @author     Steve Schaneville
// ----------------------------------------------------------------------- //
SS_String::SS_String()
{
    InitObject();
}

// ----------------------------------------------------------------------- //
/// This is the destructor.  We will ASSERT if any outstanding references
/// remain on this instance of the SS_String.  In release builds, we will
/// simply ignore outstanding references.
///
/// @fn	        SS_String::~SS_String()
/// @return     none
/// @author     Steve Schaneville
// ----------------------------------------------------------------------- //
SS_String::~SS_String()
{
    delete [] m_szString;
    m_szString = NULL;
    BufferSize(0);
    FreeTokens();
    if( References() )
        ASSERT(!_T("There should be no references outstanding!!  You should ")
                _T("call Release() before allowing this instance to be ")
                _T("destructed"));
}

// ----------------------------------------------------------------------- //
/// @fn	        SS_String::SS_String(const SS_String& ssString)
/// @param		ssString [in] the SS_String instance to create a copy of.
/// @return     The instance of the newly created string.
/// @author     Steve Schaneville
// ----------------------------------------------------------------------- //
SS_String::SS_String( const SS_String& ssString )
{
    InitObject();
    *this = ssString;
}

// ----------------------------------------------------------------------- //
/// @fn	        SS_String::SS_String(LPCTSTR szString)
/// @param		szString [in] a null-terminated string used to initialize 
///                           our new SS_String instance.
/// @return     none
/// @author     Steve Schaneville
// ----------------------------------------------------------------------- //
SS_String::SS_String( LPCTSTR szString )
{
    InitObject();
    *this = szString;
}

// ----------------------------------------------------------------------- //
/// If we are in an ANSI application, the BSTR will be converted to an ANSI 
/// string before initialization.
///
/// @fn	        SS_String::SS_String(BSTR bstrString)
/// @param		bstrString [in] a null-terminated BSTR string used to 
///                             initialize our new SS_String instance.
/// @return     none
/// @author     Steve Schaneville
// ----------------------------------------------------------------------- //
SS_String::SS_String( BSTR bstrString )
{
    InitObject();
    *this = bstrString;
}

// ----------------------------------------------------------------------- //
/// The string will be initialized to a text representation of the integer 
/// passed into the constructor.
///
/// @fn	        SS_String::SS_String(INT nValue)
/// @param		nValue [in] an integer value used to initialize our new 
///                         SS_String instance.  
/// @return     none
/// @author     Steve Schaneville
// ----------------------------------------------------------------------- //
SS_String::SS_String(INT nValue)
{
    InitObject();
    TCHAR szTmp[SS_STR_UNIT_LEN];
    *this = _itot(nValue, szTmp, 10);
}

// ----------------------------------------------------------------------- //
/// @fn	        SS_String::SS_String(HINSTANCE hInstance, UINT nResourceID)
/// @param		hInstance [in] the instance of the windows process, passed
///                            into the WinMain (or main, or DllMain) 
///                              function.
/// @param		nResourceID [in] a string resource ID from which we will 
///                              initialize our string.
/// @return     none
/// @author     Steve Schaneville
// ----------------------------------------------------------------------- //
SS_String::SS_String(HINSTANCE hInstance, UINT nResourceID)
{
    InitObject();
    LoadString(hInstance, nResourceID);
}

#if defined(UNICODE) || defined(_UNICODE)
// ----------------------------------------------------------------------- //
/// A constructor that takes a const char* in UNICODE applications.
///
/// @fn	        SS_String::SS_String(const char* szString)
/// @param		szString [in] a null-terminated BSTR string used to 
///                           initialize our new SS_String instance.
/// @return     none
///
/// @note This constructor will only be defined if we are in a UNICODE 
/// application.  The ANSI string passed into this constructor will be 
/// converted into a UNICODE string before initialization.
/// @author     Steve Schaneville
// ----------------------------------------------------------------------- //
SS_String::SS_String(const char* szString)
{
    InitObject();
    *this = szString;
}
#endif


// ----------------------------------------------------------------------- //
/// The default assignment operator.
///
/// @fn	        SS_String::operator = ( const SS_String& rhs )
/// @param		rhs [in] a null-terminated BSTR string used to 
///                      initialize our new SS_String instance.
/// @return     none
/// @author     Steve Schaneville
// ----------------------------------------------------------------------- //
SS_String& SS_String::operator = ( const SS_String& rhs )
{
    if( References() )
        // we do this only to check and see if the string will
        // be re-initialized.  If it will be (and we have outstanding
        // references), the function below will cause an ASSERT.  Note
        // that the user should call ReleaseBuffer() if this situation
        // arises.
        String(rhs.String());

    delete [] m_szString;
    m_szString = NULL;
    FreeTokens();
    InitObject();
    String(rhs.String());

    return *this;
}

// ----------------------------------------------------------------------- //
///	This function should be called from all constructors BEFORE initializing
/// any variables in the constructor.
///
/// @fn	        SS_String::InitObject()
/// @return     none
/// @author     Steve Schaneville
// ----------------------------------------------------------------------- //
VOID SS_String::InitObject()
{
    m_szString = NULL;
    m_b_ss_tcstok_trailingToken = FALSE;
    m_sz_ss_tcstok_pos = 0;
    BufferSize(0);
    References(0);
    Tokens(NULL);
    TokenCount(0);
    CurrentToken(-1);
    String(_T(""));
    TokensAreValid(FALSE);
}

// ----------------------------------------------------------------------- //
///	This is the function that most assignment operators map to.  It will
/// take a string and copy it into out local buffer.
///
/// @fn	        SS_String::String(LPCTSTR szString)
/// @param		szString [in] null-terminated string to copy into the buffer
///							  of our SS_String instance.
/// @return     none
/// @author     Steve Schaneville
// ----------------------------------------------------------------------- //
VOID SS_String::String(LPCTSTR szString)
{
    if( szString )
    {
        if( InitString(_tcslen(szString)) )
            _tcscpy(m_szString, szString);
    }
    else
    {
        String(_T(""));
    }
}

// ----------------------------------------------------------------------- //
///	This function will take the contents of the BSTR and copy them into our
/// local buffer.  If we are in a UNICODE application, this function will
/// convert the BSTR into an ANSI string before copying it into the local
/// buffer.
///
/// @fn	        SS_String::String(const BSTR bstrString)
/// @param		bstrString [in] BSTR string to copy into the buffer
///							    of our SS_String instance.
/// @return     none
/// @author     Steve Schaneville
// ----------------------------------------------------------------------- //
VOID SS_String::String(const BSTR bstrString)
{
    INT nLength = 0;

    if( bstrString )
        nLength = InitString(wcslen(bstrString));
    else
        nLength = InitString(0);

    if( bstrString )
    {
#if defined( UNICODE ) || defined( _UNICODE )
        _tcscpy(m_szString, bstrString);
#else
        wcstombs(m_szString, bstrString, nLength);
#endif
    }
}

#if defined(UNICODE) || defined(_UNICODE)
// ----------------------------------------------------------------------- //
///	This function will convert the contents of the szString from ANSI to
/// UNICODE and then copy the contents into the local buffer.  
///
/// @fn	        SS_String::String(const char* szString)
/// @param		szString [in] null-terminated string to copy into the buffer
///							  of our SS_String instance.
/// @return     none
///
/// @note This function is only defined in UNICODE applications.
/// @author     Steve Schaneville
// ----------------------------------------------------------------------- //
VOID SS_String::String(const char* szString)
{
    INT nLength = InitString(strlen(szString));
    if( nLength )
        mbstowcs(m_szString, szString, nLength);
}
#endif

// ----------------------------------------------------------------------- //
///	Given a minimum desired buffer size, this function allocates a new buffer 
/// for our string that is a power of 2 of the SS_STR_UNIT_LEN.  An additional
/// byte is allocated for the null terminator.
///
/// @fn	        SS_String::InitString(INT nLength)
/// @param		nLength [in] the minimum length of the new buffer size
///                          desired for the string, not including
///                          the null terminator (which will be
///                          accounted for automatically).
/// @return     Size of the new buffer created, or 0 if failed.
///
/// @note The function checks for outstanding references before allowing the 
/// re-allocation and returns no new allocation if any remain outstanding.
/// @author     Steve Schaneville
// ----------------------------------------------------------------------- //
INT SS_String::InitString(INT nLength)
{
	ASSERT(nLength >= 0);
    nLength++; // add 1 for our null terminator to be safe
	INT nNewLength = SS_STR_UNIT_LEN;
	while( nNewLength < nLength )
		nNewLength *= 2;
    if( nNewLength > BufferSize() )
    {
        if( References() )
        {
            // if we get here, we are about to delete the string while there
            // are still references to the string somewhere (presumably
            // aquired with the GetBuffer() functions).  You must first call
            // ReleaseBuffer() before re-initializing this string.
            ASSERT(!_T("References remain open on this SS_String instance.  Call ")
                    _T("ReleaseBuffer() before allowing a string re-initialization."));
            return 0;
        }

        delete [] m_szString;
        BufferSize( nNewLength );
        m_szString = new TCHAR[BufferSize()];
        VERIFY_POINTER(m_szString);
    }
    _tcscpy(m_szString, _T(""));
    TokensAreValid(FALSE);
    return BufferSize();
}

// ----------------------------------------------------------------------- //
///	Case-sensitive comparison function for a BSTR.  For ANSI applications, 
/// this function will first convert the BSTR to an ANSI string before the 
/// comparison.
///
/// @fn	        SS_String::operator == (const BSTR rhs) const
/// @param		rhs [in] BSTR string to be compared to our local buffer
/// @return     TRUE if the local buffer and the BSTR are identical.
///	@return		FALSE if the local buffer and the BSTR differ.
/// @author     Steve Schaneville
// ----------------------------------------------------------------------- //
BOOL SS_String::operator == (const BSTR rhs) const
{
#if defined(UNICODE) || defined(_UNICODE)
    return _tcscmp(m_szString, rhs) == 0 ? TRUE : FALSE;
#else
    char* szRhs = new char[wcslen(rhs) + 1];
    VERIFY_POINTER(szRhs);
    wcstombs(szRhs, rhs, wcslen(rhs)+1);
    BOOL bResult = _tcscmp(m_szString, szRhs) == 0 ? TRUE : FALSE;
    delete [] szRhs;
    return bResult;
#endif
}

// ----------------------------------------------------------------------- //
///	Case-sensitive "not equals" function for a BSTR.  For ANSI applications, 
/// this function will first convert the BSTR to an ANSI string before the 
/// comparison.
///
/// @fn	        SS_String::operator != (const BSTR rhs) const
/// @param		rhs [in] BSTR string to be compared to our local buffer
/// @return     TRUE if the local buffer and the BSTR differ.
///	@return		FALSE if the local buffer and the BSTR are identical.
/// @author     Steve Schaneville
// ----------------------------------------------------------------------- //
BOOL SS_String::operator != (const BSTR rhs) const
{
#if defined(UNICODE) || defined(_UNICODE)
    return _tcscmp(m_szString, rhs) != 0 ? TRUE : FALSE;
#else
    char* szRhs = new char[wcslen(rhs) + 1];
    VERIFY_POINTER(szRhs);
    wcstombs(szRhs, rhs, wcslen(rhs)+1);
    BOOL bResult = _tcscmp(m_szString, szRhs) != 0 ? TRUE : FALSE;
    delete [] szRhs;
    return bResult;
#endif
}

// ----------------------------------------------------------------------- //
///	Case-sensitive "greater than or equals" function for a BSTR.  For ANSI 
/// applications, this function will first convert the BSTR to an ANSI string 
/// before the comparison.
///
/// @fn	        SS_String::operator >= (const BSTR rhs) const
/// @param		rhs [in] BSTR string to be compared to our local buffer
/// @return     TRUE if the local buffer is greater than or equal to
///				the passed in BSTR.
///	@return		FALSE if the local buffer is less than the BSTR.
/// @author     Steve Schaneville
// ----------------------------------------------------------------------- //
BOOL SS_String::operator >= (const BSTR rhs) const
{
#if defined(UNICODE) || defined(_UNICODE)
    return _tcscmp(m_szString, rhs) >= 0 ? TRUE : FALSE;
#else
    char* szRhs = new char[wcslen(rhs) + 1];
    VERIFY_POINTER(szRhs);
    wcstombs(szRhs, rhs, wcslen(rhs)+1);
    BOOL bResult = _tcscmp(m_szString, szRhs) >= 0 ? TRUE : FALSE;
    delete [] szRhs;
    return bResult;
#endif
}

// ----------------------------------------------------------------------- //
///	Case-sensitive "less than or equals" function for a BSTR.  For ANSI 
/// applications, this function will first convert the BSTR to an ANSI string 
/// before the comparison.
///
/// @fn	        SS_String::operator <= (const BSTR rhs) const
/// @param		rhs [in] BSTR string to be compared to our local buffer
/// @return     TRUE if the local buffer is less than or equal to
///				the passed in BSTR.
///	@return		FALSE if the local buffer is greater than the BSTR.
/// @author     Steve Schaneville
// ----------------------------------------------------------------------- //
BOOL SS_String::operator <= (const BSTR rhs) const
{
#if defined(UNICODE) || defined(_UNICODE)
    return _tcscmp(m_szString, rhs) <= 0 ? TRUE : FALSE;
#else
    char* szRhs = new char[wcslen(rhs) + 1];
    VERIFY_POINTER(szRhs);
    wcstombs(szRhs, rhs, wcslen(rhs)+1);
    BOOL bResult = _tcscmp(m_szString, szRhs) <= 0 ? TRUE : FALSE;
    delete [] szRhs;
    return bResult;
#endif
}

// ----------------------------------------------------------------------- //
///	Case-sensitive "greater than" function for a BSTR.  For ANSI 
/// applications, this function will first convert the BSTR to an ANSI string 
/// before the comparison.
///
/// @fn	        SS_String::operator > (const BSTR rhs) const
/// @param		rhs [in] BSTR string to be compared to our local buffer
/// @return     TRUE if the local buffer is greater than the passed in BSTR.
///	@return		FALSE if the local buffer is not greater than the BSTR.
/// @author     Steve Schaneville
// ----------------------------------------------------------------------- //
BOOL SS_String::operator > (const BSTR rhs) const
{
#if defined(UNICODE) || defined(_UNICODE)
    return _tcscmp(m_szString, rhs) > 0 ? TRUE : FALSE;
#else
    char* szRhs = new char[wcslen(rhs) + 1];
    VERIFY_POINTER(szRhs);
    wcstombs(szRhs, rhs, wcslen(rhs)+1);
    BOOL bResult = _tcscmp(m_szString, szRhs) > 0 ? TRUE : FALSE;
    delete [] szRhs;
    return bResult;
#endif
}

// ----------------------------------------------------------------------- //
///	Case-sensitive "less than" function for a BSTR.  For ANSI 
/// applications, this function will first convert the BSTR to an ANSI string 
/// before the comparison.
///
/// @fn	        SS_String::operator < (const BSTR rhs) const
/// @param		rhs [in] BSTR string to be compared to our local buffer
/// @return     TRUE if the local buffer is less than the passed in BSTR.
///	@return		FALSE if the local buffer is not less than the BSTR.
/// @author     Steve Schaneville
// ----------------------------------------------------------------------- //
BOOL SS_String::operator < (const BSTR rhs) const
{
#if defined(UNICODE) || defined(_UNICODE)
    return _tcscmp(m_szString, rhs) < 0 ? TRUE : FALSE;
#else
    char* szRhs = new char[wcslen(rhs) + 1];
    VERIFY_POINTER(szRhs);
    wcstombs(szRhs, rhs, wcslen(rhs)+1);
    BOOL bResult = _tcscmp(m_szString, szRhs) < 0 ? TRUE : FALSE;
    delete [] szRhs;
    return bResult;
#endif
}

#if !defined(UNICODE) || !defined(_UNICODE)
// ----------------------------------------------------------------------- //
///	BSTR cast function.  This function will convert the local buffer into 
/// a BSTR and return it.  
///
/// @fn	        SS_String::operator BSTR ()
/// @return     none
///
/// @note The returned BSTR is allocated with SysAllocString.  The user
/// is expected to free this memory with the SysFreeString function or
/// a memory leak will result.
///
/// @note This function is only defined in non-UNICODE applications.
/// @author     Steve Schaneville
// ----------------------------------------------------------------------- //
SS_String::operator BSTR ()
{
    WCHAR* wszTmp = new WCHAR[BufferSize()];
    VERIFY_POINTER(wszTmp);
    mbstowcs(wszTmp, String(), _tcslen(String())+1);
    BSTR bstrString = SysAllocString(wszTmp);
    delete [] wszTmp;
    return bstrString;
}
#endif

// ----------------------------------------------------------------------- //
///	DOUBLE cast function.  This function will use the atof c-runtime function
/// to convert our string to a DOUBLE.  See the atof documentation for more
/// details.
///
/// @fn	        SS_String::operator DOUBLE ()
/// @return     none
/// @author     Steve Schaneville
// ----------------------------------------------------------------------- //
SS_String::operator DOUBLE ()
{
#if defined(UNICODE) || defined(_UNICODE)
    double dResult = 0;
    char* szNum = new char[BufferSize()];
    VERIFY_POINTER(szNum);
    wcstombs(szNum, String(), wcslen(String())+1);
    dResult = atof(szNum);
    delete [] szNum;
    return dResult;
#else
    return atof(String());
#endif
}

// ----------------------------------------------------------------------- //
///	FLOAT cast function.  This function will use the atof c-runtime function
/// to convert our string to a FLOAT.  See the atof documentation for more
/// details.
///
/// @fn	        SS_String::operator FLOAT ()
/// @return     none
/// @author     Steve Schaneville
// ----------------------------------------------------------------------- //
SS_String::operator FLOAT ()
{
    return (FLOAT)((DOUBLE)*this);
}

// ----------------------------------------------------------------------- //
///	Addition operator.  This function concatenates the string on the right
/// side of the + operator with the string on the left and returns the result.  
/// Note that this function does not change either of the original strings.
///
/// @fn	        SS_String::operator + (LPCTSTR rhs)
/// @param      rhs [in] a null-terminated string buffer to concatenate with 
///                      the string on the left side of the + operator.
/// @return     An SS_String containing the concatenated result.
/// @author     Steve Schaneville
// ----------------------------------------------------------------------- //
SS_String SS_String::operator + (LPCTSTR rhs)
{
    ASSERT(rhs);
    SS_String ssResult = *this;
    return ssResult += rhs;
}

// ----------------------------------------------------------------------- //
///	BSTR Addition operator.  This function concatenates the contents of the 
/// BSTR on the right side of the + operator with the string on the left and 
/// returns the result.  This function does not change either of 
/// the original strings.  In ANSI applications, the BSTR will be converted
/// to an ANSI string before the concatenation.
///
/// @fn	        SS_String::operator + (const BSTR rhs)
/// @param      rhs [in] a BSTR to concatenate with the string on the
///                      left side of the + operator.
/// @return     An SS_String containing the concatenated result.
/// @author     Steve Schaneville
// ----------------------------------------------------------------------- //
SS_String SS_String::operator + (const BSTR rhs)
{
    ASSERT(rhs);
    SS_String ssResult = *this;
    return ssResult += rhs;
}

// ----------------------------------------------------------------------- //
///	Addition operator.  This function concatenates the contents of the 
/// SS_String on the right side of the + operator with the string on the left
/// and returns the result.  This function does not change either of 
/// the original strings.
///
/// @fn	        SS_String::operator + (const SS_String& rhs)
/// @param      rhs [in] an SS_String to concatenate with the string on the
///                      left side of the + operator.
/// @return     An SS_String containing the concatenated result.
/// @author     Steve Schaneville
// ----------------------------------------------------------------------- //
SS_String SS_String::operator + (const SS_String& rhs)
{
    SS_String ssResult = *this;
    return ssResult += rhs.String();
}

// ----------------------------------------------------------------------- //
///	Addition assignment operator.  This function concatenates the contents of 
/// the null-terminated string pointer with the string on the left and 
/// returns the left SS_String instance.  This function permanently 
/// changes the string on the left of the += operator to contain the 
/// concatenated result.
///
/// @fn	        SS_String::operator += (LPCTSTR rhs)
/// @param      rhs [in] a null-terminated string to concatenate with the 
///                      string on the left side of the + operator.
/// @return     The original instance of the SS_String, which now contains 
///             the concatenated result.
/// @author     Steve Schaneville
// ----------------------------------------------------------------------- //
SS_String& SS_String::operator += (LPCTSTR rhs)
{
    TCHAR* szTmp = new TCHAR[BufferSize()];
    if(!szTmp)
    {
        ASSERT(!_T("memory allocation failed"));
        return *this;
    }
    _tcscpy(szTmp, String());
    InitString(_tcslen(szTmp)+_tcslen(rhs));
    _tcscpy(m_szString, szTmp);
    _tcscat(m_szString, rhs);
    delete [] szTmp;
    return *this;
}

// ----------------------------------------------------------------------- //
///	BSTR Addition assignment operator.  This function concatenates the 
/// contents of the BSTR on the right side of the += operator with the 
/// string on the left and returns the original SS_String instance.  This 
/// function permanently changes the original SS_String instance to 
/// contain the concatenated result.  Note that in ANSI applications,
/// this function will convert the BSTR to an ANSI string before the 
/// concatenation.
///
/// @fn	        SS_String::operator += (const BSTR rhs)
/// @param      rhs [in] a BSTR string to concatenate with the string on the
///                      left side of the + operator.
/// @return     The left instance of the SS_String, which now contains 
///             the concatenated result.
/// @author     Steve Schaneville
// ----------------------------------------------------------------------- //
SS_String& SS_String::operator += (const BSTR rhs)
{
#if defined(UNICODE) || defined(_UNICODE)
    return *this += (LPCTSTR)rhs;
#else
    TCHAR* szRhs = new TCHAR[wcslen(rhs)+1];
    if(!szRhs)
    {
        ASSERT(!_T("memory allocation failed"));
        return *this;
    }
    wcstombs(szRhs, rhs, wcslen(rhs)+1);
    *this += szRhs;
    delete [] szRhs;
    return *this;
#endif
}

// ----------------------------------------------------------------------- //
///	Addition assignment operator.  This function concatenates the contents of 
/// the SS_String on the right side of the += operator with the string on the
/// left and returns the left SS_String instance.  This function permanently 
/// changes the string on the left of the += operator to contain the 
/// concatenated result.
///
/// @fn	        SS_String::operator += (const SS_String& rhs)
/// @param      rhs [in] an SS_String to concatenate with the string on the
///                      left side of the + operator.
/// @return     The left instance of the SS_String, which now contains 
///             the concatenated result.
/// @author     Steve Schaneville
// ----------------------------------------------------------------------- //
SS_String& SS_String::operator += (const SS_String& rhs)
{
    *this += rhs.String();
    return *this;
}


// utilities
//
VOID SS_String::FreeTokens()
{
    delete [] Tokens();
    Tokens(NULL);
    TokenCount(0);
    CurrentToken(-1);
    TokensAreValid(FALSE);
}

// ----------------------------------------------------------------------- //
///	This function will return the token stored at the index passed in the
/// nTokenIndex parameter.  Each tokens consists of the text between 
/// any of the delimiters passed into the szDelimiters parameter.  Delimiters
/// must be passed into the first call to this function, but future calls
/// can leave this parameter null.  
///
/// If the local string buffer changes after a call to one of the tokens
/// functions is made, all stored tokens will become invalid, and further
/// calls to this function will fail unless a new set of delimiters are
/// passed in.
///
/// @fn	        SS_String::Token(INT nTokenIndex, LPCTSTR szDelimiters)
/// @param      nTokenIndex [in] zero-based index of the token desired in 
///                              the return value.
/// @param      szDelimiters [in] null-terminated string containing a list
///                               of delimiting characters.  Each of these
///                               characters will be used to break up the 
///                               local string buffer into subgroups.\n  
///								  \n
///                               If this parameter is null, the function
///                               will use delimiters previously passed
///                               into this function or other token functions
///                               that accept delimiters.\n
///								  \n
///                               If this parameter is not null, the function
///                               will parse the local buffer using these
///                               delimiters, and save the token results
///                               for future calls to token functions.
///                               
/// @return     The token at the index requested, or NULL if no token
///             is found at the index.
/// @author     Steve Schaneville
// ----------------------------------------------------------------------- //
LPTSTR SS_String::Token(INT nTokenIndex, LPCTSTR szDelimiters /*=NULL*/)
{
    if( !TokensAreValid() || szDelimiters )
    {
        if( szDelimiters )
            InitTokens(szDelimiters);
        else
        {
            // MUST set new delimiters since string has changed
            // since last time we searched for the tokens.
            ASSERT(!_T("Must set new delimiters because local buffer changed"));
            return NULL;
        }
    }

    if( nTokenIndex < TokenCount() && nTokenIndex >= 0 )
    {
        CurrentToken(nTokenIndex);
        return Tokens()[nTokenIndex];
    }
    else
        return NULL;
}

// ----------------------------------------------------------------------- //
///	This function will return the first token stored found in the local 
/// buffer.  Each tokens consists of the text between any of the
/// delimiters passed into the szDelimiters parameter.  Delimiters
/// must be passed into the first call to this function, but future calls
/// can leave this parameter null.  
///
/// If the local string buffer changes after a call to one of the tokens
/// functions is made, all stored tokens will become invalid, and further
/// calls to this function will fail unless a new set of delimiters are
/// passed in.
///
/// @fn	        SS_String::FirstToken(LPCTSTR szDelimiters)
/// @param      szDelimiters [in] null-terminated string containing a list
///                               of delimiting characters.  Each of these
///                               characters will be used to break up the 
///                               local string buffer into subgroups.\n  
///								  \n
///                               If this parameter is null, the function
///                               will use delimiters previously passed
///                               into this function or other token functions
///                               that accept delimiters.\n
///								  \n
///                               If this parameter is not null, the function
///                               will parse the local buffer using these
///                               delimiters, and save the token results
///                               for future calls to token functions.
///                               
/// @return     The first token in the string, or NULL if no tokens
//              are found.
/// @author     Steve Schaneville
// ----------------------------------------------------------------------- //
LPTSTR SS_String::FirstToken(LPCTSTR szDelimiters /*=NULL*/)
{
    if( !TokensAreValid() || szDelimiters )
    {
        if( szDelimiters )
            InitTokens(szDelimiters);
        else
        {
            // MUST set new delimiters since string has changed
            // since last time we searched for the tokens.
            ASSERT(!_T("Must set new delimiters because local buffer changed"));
            return 0;
        }
    }

    if( TokenCount() )
    {
        CurrentToken(0);
        return m_ppTokens[CurrentToken()];
    }
    else
    {
        CurrentToken(-1);
        return NULL;
    }
}

// ----------------------------------------------------------------------- //
///	This function will return the last token stored found in the local 
/// buffer.  Each tokens consists of the text between any of the
/// delimiters passed into the szDelimiters parameter.  Delimiters
/// must be passed into the first call to this function, but future calls
/// can leave this parameter null.  
///
/// If the local string buffer changes after a call to one of the tokens
/// functions is made, all stored tokens will become invalid, and further
/// calls to this function will fail unless a new set of delimiters are
/// passed in.
///
/// @fn	        SS_String::LastToken(LPCTSTR szDelimiters)
/// @param      szDelimiters [in] null-terminated string containing a list
///                               of delimiting characters.  Each of these
///                               characters will be used to break up the 
///                               local string buffer into subgroups.\n  
///								  \n
///                               If this parameter is null, the function
///                               will use delimiters previously passed
///                               into this function or other token functions
///                               that accept delimiters.\n
///								  \n
///                               If this parameter is not null, the function
///                               will parse the local buffer using these
///                               delimiters, and save the token results
///                               for future calls to token functions.
///                               
/// @return     The last token in the string, or NULL if no tokens
//              are found.
/// @author     Steve Schaneville
// ----------------------------------------------------------------------- //
LPTSTR SS_String::LastToken(LPCTSTR szDelimiters /*=NULL*/)
{
    if( !TokensAreValid() || szDelimiters )
    {
        if( szDelimiters )
            InitTokens(szDelimiters);
        else
        {
            // MUST set new delimiters since string has changed
            // since last time we searched for the tokens.
            ASSERT(!_T("Must set new delimiters because local buffer changed"));
            return 0;
        }
    }

    if( TokenCount() )
    {
        CurrentToken(TokenCount()-1);
        return m_ppTokens[CurrentToken()];
    }
    else
    {
        CurrentToken(-1);
        return NULL;
    }
}

// ----------------------------------------------------------------------- //
///	This function parses this SS_String for any characters
/// that are in szDelimiters... if any are found, we store all
/// sections of the string between these found characters for
/// future use by the user (see strtok() in the msdn for
/// more information about the use of tokens).
///                               
/// @fn	        SS_String::InitTokens(LPCTSTR szDelimiters)
/// @param      szDelimiters [in] null-terminated string containing a list
///                               of delimiting characters.  Each of these
///                               characters will be used to break up the 
///                               local string buffer into subgroups.  
///
///                               If this parameter is null, the function
///                               will use delimiters previously passed
///                               into this function or other token functions
///                               that accept delimiters.
///
///                               If this parameter is not null, the function
///                               will parse the local buffer using these
///                               delimiters, and save the token results
///                               for future calls to token functions.
/// @return     none
/// @author     Steve Schaneville
// ----------------------------------------------------------------------- //
VOID SS_String::InitTokens(LPCTSTR szDelimiters)
{
    ASSERT(szDelimiters);

    // delete old stuff
    FreeTokens();

    // Create new temporary buffer
    LPTSTR szToken = NULL;
    LPTSTR szTokens = new TCHAR[BufferSize()];
    if(!szTokens)
    {
        ASSERT(!_T("memory allocation failed"));
        return;
    }
    _tcscpy(szTokens, String());

    // count tokens
    INT nCount = 0;
    szToken = _ss_tcstok(szTokens, szDelimiters);
    while(szToken)
    {
        nCount++;
        szToken = _ss_tcstok(NULL, szDelimiters);
    }

    // now create the space for the tokens
    Tokens( new SS_String[nCount] );
    TokenCount(nCount);

    // fill the tokens into the array
    _tcscpy(szTokens, String());
    szToken = _ss_tcstok(szTokens, szDelimiters);
    nCount = 0;
    while(szToken)
    {
        Tokens()[nCount] = szToken;
        szToken = _ss_tcstok(NULL, szDelimiters);
        nCount++;
    }

    delete [] szTokens;

    if( TokenCount() )
        TokensAreValid(TRUE);
}

// ----------------------------------------------------------------------- //
///	This function will return the next token found in the local 
/// buffer.  Each tokens consists of the text between any of the
/// delimiters passed into the szDelimiters parameter.  Delimiters
/// must be passed into the Token, FirstToken, or LastToken calls before this
/// function can be called.
///
/// If the local string buffer changes after a call to one of the tokens
/// functions is made, all stored tokens will become invalid, and further
/// calls to this function will fail unless a new set of delimiters are
/// passed in.
///
/// @fn	        SS_String::NextToken()
/// @return     The next token in the string, or NULL if no more tokens
///             are found.
/// @author     Steve Schaneville
// ----------------------------------------------------------------------- //
LPTSTR SS_String::NextToken()
{
    ASSERT(TokensAreValid()); // you must first set the delimiters by
                              // calling FirstToken(), LastToken(), or
                              // Token().  You must do this every time
                              // this SS_String changes.

    if( CurrentToken() == TokenCount() )
        return NULL;

    if( TokenCount() )
        CurrentToken(CurrentToken()+1); // m_nCurrentToken++;

    if( CurrentToken() == TokenCount() )
        return NULL;

    return m_ppTokens[CurrentToken()];
}

// ----------------------------------------------------------------------- //
///	This function will return the previous token found in the local 
/// buffer.  Each tokens consists of the text between any of the
/// delimiters passed into the szDelimiters parameter.  Delimiters
/// must be passed into the Token, FirstToken, or LastToken calls before this
/// function can be called.
///
/// If the local string buffer changes after a call to one of the tokens
/// functions is made, all stored tokens will become invalid, and further
/// calls to this function will fail unless a new set of delimiters are
/// passed in.
///
/// @fn	        SS_String::NextToken()
/// @return     The previous token in the string, or NULL if no more tokens
///             are found.
/// @author     Steve Schaneville
// ----------------------------------------------------------------------- //
LPTSTR SS_String::PreviousToken()
{
    ASSERT(TokensAreValid()); // you must first set the delimiters by
                              // calling FirstToken(), LastToken(), or
                              // Token().  You must do this every time
                              // this SS_String changes.

    if( -1 == CurrentToken() )
        return NULL;

    if( TokenCount() )
        CurrentToken(CurrentToken()-1); // m_nCurrentToken--;

    if( -1 == CurrentToken() )
        return NULL;

    return m_ppTokens[CurrentToken()];
}

// ----------------------------------------------------------------------- //
/// This function loads a string resource ID into the local buffer.
///
/// @fn	        SS_String::LoadString(HINSTANCE hInstance, UINT nResourceID)
/// @param      hInstance [in] the instance handle of this process, provided
///                            to the user in the WinMain functions.
/// @param      nResourceID [in] a string resource ID from which to load the
///                              string into the current buffer.
/// @return     This instance of the SS_String with the string resource ID
///             loaded.
/// @author     Steve Schaneville
// ----------------------------------------------------------------------- //
SS_String& SS_String::LoadString(HINSTANCE hInstance, UINT nResourceID)
{
    TCHAR szTmp[SS_STR_UNIT_LEN];
    INT nLen = ::LoadString(hInstance, nResourceID, szTmp, SS_STR_UNIT_LEN);
    if( 1 < SS_STR_UNIT_LEN - nLen )
    {
        *this = szTmp;
        return *this;
    }

    INT nSize = SS_STR_UNIT_LEN - 1;
    do
    {
        nSize += SS_STR_UNIT_LEN;
        nLen = ::LoadString(hInstance, nResourceID, GetBuffer(nSize), nSize);
        ReleaseBuffer();
    } while( 1 >= nSize - nLen);

    return *this;
}

// ----------------------------------------------------------------------- //
/// This function displays a message box with the current contents of our
/// SS_String object as the message.
///
/// @fn	        SS_String::MessageBox(HWND hWnd, LPCTSTR szTitle, UINT uType) const
/// @param      hWnd [in] parent window for the message box
/// @param      szTitle [in] a string to put into the title of the dialog.
///                          The default value is "Dialog".
/// @param      uType [in] one or more of the messagebox types (MB_OK, MB_YESNO, etc.)
///                        specified in the Windows API MessageBox function.  
///                        The default is MB_OK|MB_ICONINFORMATION.
/// @return     The user's response when closing the message box. 
/// @author     Steve Schaneville
///
// ----------------------------------------------------------------------- //
INT SS_String::MessageBox(HWND hWnd, LPCTSTR szTitle, UINT uType) const
{
    return ::MessageBox(hWnd, String(), szTitle, uType);
}

// ----------------------------------------------------------------------- //
/// This function provides the user with a writable pointer to the local
/// string buffer.  If nMinLength is larger than the current allocated buffer,
/// a new buffer with at least nMinLength bytes will be allocated and our
/// current string copied into it before returning.  
///
/// @fn	        SS_String::GetBuffer(INT nMinLength)
/// @param      nMinLength [in] minimum size of the returned buffer.
/// @return     A writable pointer to the local string buffer.
///
/// @note The user must call ReleaseBuffer once for each call to GetBuffer().
/// Any calls to functions that change the local buffer will fail until
/// all references to the local buffer have been released.
/// @author     Steve Schaneville
// ----------------------------------------------------------------------- //
LPTSTR SS_String::GetBuffer(INT nMinLength)
{
    if( BufferSize() <= nMinLength )
    {
        SS_String ssTmp = *this;
        InitString(nMinLength);
        _tcscpy(m_szString, ssTmp);
    }

    AddReference();
    return m_szString;
}

LPCTSTR SS_String::GetBufferConst()
{
    return m_szString;
}

// ----------------------------------------------------------------------- //
/// Releases a previously obtained reference to the local buffer.  Call this 
/// function once for each call to the GetBuffer function.
///
/// @fn	        String::ReleaseBuffer()
/// @return     none
/// @author     Steve Schaneville
// ----------------------------------------------------------------------- //
VOID SS_String::ReleaseBuffer()
{
    RemoveReference();
}

// ----------------------------------------------------------------------- //
/// Returns a string plus extra characters found in the local buffer, and
/// the zero-based index of the found string.  Note that there may 
/// be more characters in the returned string than passed in with the 
/// szFindStringparameter.  This is because the returned value is assigned
/// by simply pointing to the middle of our SS_String main
/// buffer, and all characters beyond the found sub-string
/// will still be present.  This behavior is by design.
///
/// @fn	        SS_String::Find(LPCTSTR szFindString, INT nStartPos, INT* pnFoundAtPos, BOOL bIgnoreCase)
/// @param      szFindString [in] a sub-string to look for in this SS_String.
/// @param      nStartPos [in] the zero-based position from which to start the 
///                            search.  Defaults to 0.
/// @param      pnFoundAtPos [out] returns the zero-based position of the 
///                                found string, or -1 if the string is not 
///                                found.  If this parameter is NULL (the
///                                default), no position is returned.
/// @param      bIgnoreCase [in] set to TRUE to perform a case-insensitive
///                              search.  Defaults to FALSE.
/// @return     The found string *plus* any remaining characters that are
///             in this SS_String.  
/// @author     Steve Schaneville
// ----------------------------------------------------------------------- //
SS_String SS_String::Find(LPCTSTR szFindString,
                          INT nStartPos /*=0*/,
                          INT* pnFoundAtPos /*=NULL*/,
                          BOOL bIgnoreCase /*=FALSE*/)
{
    ASSERT( nStartPos < BufferSize() );

    LPTSTR szCurrentPos = (LPTSTR)(String()+nStartPos);
    INT nLength = _tcslen(szFindString);
    BOOL bFound = FALSE;
    INT nCurrentPos = nStartPos;

    while( *szCurrentPos != '\0' )
    {
        if( bIgnoreCase )
        {
            if( !_tcsnicmp(szCurrentPos, szFindString, nLength) )
            {
                bFound = TRUE;
                break;
            }
        }
        else
        {
            if( !_tcsncmp(szCurrentPos, szFindString, nLength) )
            {
                bFound = TRUE;
                break;
            }
        }

        nCurrentPos++;
        szCurrentPos++;
    }

    if( bFound )
    {
        if( pnFoundAtPos )
            *pnFoundAtPos = nCurrentPos;

        SS_String ssResult = szCurrentPos;
        return ssResult;
    }
    else
    {
        if( pnFoundAtPos )
            *pnFoundAtPos = -1;

        SS_String ssResult;
        return ssResult;
    }
}

// ----------------------------------------------------------------------- //
/// Returns a string plus extra characters found in the local buffer, and
/// the zero-based index of the found string.  Note that there may 
/// be more characters in the returned string than passed in with the 
/// szFindStringparameter.  This is because the returned value is assigned
/// by simply pointing to the middle of our SS_String main
/// buffer, and all characters beyond the found sub-string
/// will still be present.  This behavior is by design.
///
/// @fn	        SS_String::FindReverse(LPCTSTR szFindString, INT nStartPos, INT* pnFoundAtPos, BOOL bIgnoreCase)
/// @param      szFindString [in] a sub-string to look for in this SS_String.
/// @param      nStartPos [in] the zero-based position from which to start the 
///                            reverse search.  Even though this is a reverse
///                            search, the nStartPos parameter is counted
///                            from the front of the string.  Pass in a 
///                            position of -1 (the default) to search from 
///                            the end of the local buffer.
/// @param      pnFoundAtPos [out] returns the zero-based position of the 
///                                found string, or -1 if the string is not 
///                                found.  If this parameter is NULL (the
///                                default), no position is returned.
/// @param      bIgnoreCase [in] set to TRUE to perform a case-insensitive
///                              search.  Defaults to FALSE.
/// @return     The found string *plus* any remaining characters that are
///             in this SS_String.  
/// @author     Steve Schaneville
// ----------------------------------------------------------------------- //
SS_String SS_String::FindReverse(LPCTSTR szFindString,
                                 INT nStartPos /*=-1*/,
                                 INT* pnFoundAtPos /*=NULL*/,
                                 BOOL bIgnoreCase /*=FALSE*/)
{
    ASSERT( nStartPos < BufferSize() );

    LPTSTR szCurrentPos = NULL;
    INT nCurrentPos = 0;
    if( -1 == nStartPos )
    {
        szCurrentPos = (LPTSTR)(String()+Length()-1);
        nCurrentPos = Length()-1;
    }
    else
    {
        szCurrentPos = (LPTSTR)(String()+nStartPos);
        nCurrentPos = nStartPos;
    }

    INT nLength = _tcslen(szFindString);
    BOOL bFound = FALSE;

    while( 0 <= nCurrentPos )
    {
        if( bIgnoreCase )
        {
            if( !_tcsnicmp(szCurrentPos, szFindString, nLength) )
            {
                bFound = TRUE;
                break;
            }
        }
        else
        {
            if( !_tcsncmp(szCurrentPos, szFindString, nLength) )
            {
                bFound = TRUE;
                break;
            }
        }

        nCurrentPos--;
        szCurrentPos--;
        if( 0 > nCurrentPos )
            break;
    }

    if( bFound )
    {
        if( pnFoundAtPos )
            *pnFoundAtPos = nCurrentPos;

        SS_String ssResult = szCurrentPos;
        return ssResult;
    }
    else
    {
        if( pnFoundAtPos )
            *pnFoundAtPos = -1;

        SS_String ssResult;
        return ssResult;
    }
}

// ----------------------------------------------------------------------- //
/// Returns a list of characters from the middle of the local buffer.
///
/// @fn	        SS_String::Mid(INT nFirst, INT nCount) const
/// @param      nFirst [in] the position of the first character to
///                         return from this SS_String.
/// @param      nCount [in] the number of characters to return.
/// @return     An SS_String containing nCount characters from the nFirst
///             position in our local buffer.
/// @author     Steve Schaneville
// ----------------------------------------------------------------------- //
SS_String SS_String::Mid(INT nFirst, INT nCount) const
{
    ASSERT(nFirst < BufferSize());
    ASSERT(nCount < (BufferSize()-nFirst));
    SS_String ssResult = (LPCTSTR)(String()+nFirst);
    ssResult.SetAt(nCount, '\0');
    return ssResult;
}

// ----------------------------------------------------------------------- //
/// Returns a list of characters from the beginning of the local buffer.
///
/// @fn	        SS_String::Left(INT nCount) const
/// @param      nCount [in] the number of characters to return from this 
///                         SS_String, starting from the first character.
/// @return     An SS_String containing nCount characters from the beginning
///             our local buffer.
/// @author     Steve Schaneville
// ----------------------------------------------------------------------- //
SS_String SS_String::Left(INT nCount) const
{
    ASSERT(nCount < BufferSize());
    SS_String ssResult = String();
    ssResult.SetAt(nCount, '\0');
    return ssResult;
}

// ----------------------------------------------------------------------- //
/// Returns a list of characters from the end of the local buffer.
///
/// @fn	        SS_String::Right(INT nCount) const
/// @param      nCount [in] the number of characters to return from this 
///                         SS_String, starting from the last character and
///                         counting in reverse.
/// @return     An SS_String containing nCount characters from the last 
///             character of our local buffer.
/// @author     Steve Schaneville
// ----------------------------------------------------------------------- //
SS_String SS_String::Right(INT nCount) const
{
    ASSERT(nCount < BufferSize());
    SS_String ssResult = (LPCTSTR)(String()+Length()-nCount);
    return ssResult;
}

// ----------------------------------------------------------------------- //
/// Removes any leading white space from our local buffer.
///
/// @fn	        SS_String::TrimLeft()
/// @return     This instance of the SS_String.
/// @author     Steve Schaneville
// ----------------------------------------------------------------------- //
SS_String& SS_String::TrimLeft()
{
    LPTSTR szPos = (LPTSTR)String();
    while( !_tcsncmp(szPos, _T(" "), 1) )
        szPos++;

    if( szPos != String() )
        String((LPCTSTR)SS_String(szPos));

    return *this;
}

// ----------------------------------------------------------------------- //
/// Removes any trailing white space from our local buffer.
///
/// @fn	        SS_String::TrimRight()
/// @return     This instance of the SS_String.
/// @author     Steve Schaneville
// ----------------------------------------------------------------------- //
SS_String& SS_String::TrimRight()
{
    LPTSTR szPos = (LPTSTR)String() + Length() - 1;
    while( !_tcsncmp(szPos, _T(" "), 1) )
        szPos--;

    szPos++;
    *szPos = '\0';

    return *this;
}

// ----------------------------------------------------------------------- //
/// Replaces all instances of the chOld character with the chNew character.
///
/// @fn	        SS_String::Replace(TCHAR chOld, TCHAR chNew)
/// @param      chOld [in] character to search for for replacement
/// @param      chNew [in] - character to insert in place of chNew
/// @return     An interger count of the number of chOld characters 
///             replaced in the local string buffer.
/// @author     Steve Schaneville
// ----------------------------------------------------------------------- //
INT SS_String::Replace(TCHAR chOld, TCHAR chNew)
{
    INT nCountReplaced = 0;
    LPTSTR szPos = (LPTSTR)String();

    while(*szPos != '\0')
    {
        if( *szPos == chOld )
        {
            *szPos = chNew;
            nCountReplaced++;
        }

        szPos++;
    }

    return nCountReplaced;
}

// ----------------------------------------------------------------------- //
/// Replaces all instances of the szOld string with the szNew string.  If 
/// the string grows beyond the allocated size of our local buffer, we
/// will grow the buffer.  The search is case-sensitive by default, but can
/// be case-insensitive if the bIgnoreCase parameter is set to TRUE.
///
/// @fn	        SS_String::Replace(LPCTSTR szOld, LPCTSTR szNew, BOOL bIgnoreCase)
/// @param      szOld [in] string to search for for replacement
/// @param      szNew [in] string to insert in place of szOld
/// @param      bIgnoreCase [in] set to TRUE to do a case-insensitive search
///                              during the place operation
/// @return     An interger count of the number of szOld strings
///             replaced in the local string buffer.
/// @author     Steve Schaneville
// ----------------------------------------------------------------------- //
INT SS_String::Replace(LPCTSTR szOld, LPCTSTR szNew, BOOL bIgnoreCase)
{
    INT nCountReplaced = 0;
    INT nPos = 0;
    LPTSTR szPos = (LPTSTR)String();

    BOOL bReplace = FALSE;
    while(*szPos != '\0')
    {
        if( bIgnoreCase )
            bReplace = !_tcsnicmp(szPos, szOld, _tcslen(szOld));
        else
            bReplace = !_tcsncmp(szPos, szOld, _tcslen(szOld));

        if( bReplace )
        {
            Delete(nPos, _tcslen(szOld));
            Insert(nPos, szNew);

            // we do this in case the string got re-initialized in the Insert() function
            szPos = ((LPTSTR)String()) + nPos;

            nCountReplaced++;
            szPos += _tcslen(szNew);
            nPos += _tcslen(szNew);
        }
        else
        {
            szPos++;
            nPos++;
        }
    }

    return nCountReplaced;
}

// ----------------------------------------------------------------------- //
/// Replaces all instances of the szOld string with the szNew string.  Calling
/// this function is identical to calling the 
/// Replace(LPCTSTR szOld, LPCTSTR szNew, BOOL bIgnoreCase) function and 
/// setting the bIgnoreCase parameter to TRUE;
///
/// @fn	        SS_String::ReplaceNoCase(LPCTSTR szOld, LPCTSTR szNew)
/// @param      szOld [in] string to search for for replacement
/// @param      szNew [in] string to insert in place of szOld
/// @return     An interger count of the number of szOld strings
///             replaced in the local string buffer.
/// @author     Steve Schaneville
// ----------------------------------------------------------------------- //
INT SS_String::ReplaceNoCase(LPCTSTR szOld, LPCTSTR szNew)
{
    return Replace(szOld, szNew, TRUE);
}

// ----------------------------------------------------------------------- //
/// Inserts a string at the position specified in the nPos parameter.  The 
/// local string buffer will grow if necessary.
///
/// @fn	        SS_String::Insert(INT nPos, LPCTSTR szString)
/// @param      nPos [in] zero-based position to insert the string
/// @param      szString [in] null-terminated string to insert
/// @return     The string length of the resulting string.
/// @author     Steve Schaneville
// ----------------------------------------------------------------------- //
INT SS_String::Insert(INT nPos, LPCTSTR szString)
{
    ASSERT(nPos <= Length() && nPos >= 0);

    LPTSTR szThisString = GetBuffer(Length()+_tcslen(szString));
    SS_String ssString = String();

    _tcscpy(szThisString+nPos, szString);
    _tcscat(szThisString, ((LPCTSTR)(ssString))+nPos);

    ReleaseBuffer();
    return Length();
}

// ----------------------------------------------------------------------- //
/// Inserts a character at the position specified in the nPos parameter.  The 
/// local string buffer will grow if necessary.
///
/// @fn	        SS_String::Insert(INT nPos, TCHAR ch)
/// @param      nPos [in] zero-based position to insert the string
/// @param      ch [in] character to insert
/// @return     The string length of the resulting string.
/// @author     Steve Schaneville
// ----------------------------------------------------------------------- //
INT SS_String::Insert(INT nPos, TCHAR ch)
{
    return Insert(nPos, &ch);
}

// ----------------------------------------------------------------------- //
/// Removes nCount characters starting from the index specified in the nPos
/// parameter.
///
/// @fn	        SS_String::Delete(INT nPos, INT nCount)
/// @param      nPos [in] zero-based position to insert the string
/// @param      nCount [in] number of characters to delete
/// @return     The string length of the resulting string.
/// @author     Steve Schaneville
// ----------------------------------------------------------------------- //
INT SS_String::Delete(INT nPos, INT nCount)
{
    ASSERT(nPos <= Length() && nPos >= 0);
    ASSERT(nCount <= Length()-nPos);

    LPTSTR szThisString = GetBuffer(0);
    SS_String ssString = String();

    _tcscpy(szThisString+nPos, ((LPCTSTR)ssString)+nPos+nCount);

    ReleaseBuffer();
    return Length();
}

// ----------------------------------------------------------------------- //
/// Removes the first occurance of szString found, starting from the beginning
/// of our buffer.  To remove multiple occurances, call this function once 
/// for each removal.
///
/// @fn	        SS_String::Delete(LPCTSTR szString, BOOL bIgnoreCase)
/// @param      szString [in] null-terminated string to delete.
/// @param      bIgnoreCase [in] set to TRUE to perform a case-insensitive
///                              search during the delete operation.  Defaults
///                              to FALSE (case-sensitive).
/// @return     The string length of the resulting string.
/// @author     Steve Schaneville
// ----------------------------------------------------------------------- //
INT SS_String::Delete(LPCTSTR szString, BOOL bIgnoreCase /*=FALSE*/)
{
    INT nPos;
    Find(szString, 0, &nPos, bIgnoreCase);
    if( -1 != nPos )
        Delete(nPos, _tcslen(szString));

    return Length();
}

// ----------------------------------------------------------------------- //
/// Removes the first occurance of szString found, starting from the beginning
/// of our buffer.  To remove multiple occurances, call this function once 
/// for each removal.  Calling this function is identical to calling the
/// Delete(LPCTSTR szString, BOOL bIgnoreCase) function and setting the 
/// bIgnoreCase parameter to TRUE.
///
/// @fn	        SS_String::DeleteNoCase(LPCTSTR szString)
/// @param      szString [in] null-terminated string to delete.
/// @return     The string length of the resulting string.
/// @author     Steve Schaneville
// ----------------------------------------------------------------------- //
INT SS_String::DeleteNoCase(LPCTSTR szString)
{
    return Delete(szString, TRUE);
}

// ----------------------------------------------------------------------- //
/// Removes all occurance of szString.  
///
/// @fn	        SS_String::Remove(LPCTSTR szString, BOOL bIgnoreCase)
/// @param      szString [in] null-terminated string to remove.
/// @param      bIgnoreCase [in] set to TRUE to perform a case-insensitive
///                              search during the remove operation.  Defaults
///                              to FALSE (case-sensitive).
/// @return     The string length of the resulting string.
/// @author     Steve Schaneville
// ----------------------------------------------------------------------- //
INT SS_String::Remove(LPCTSTR szString, BOOL bIgnoreCase)
{
    return Replace(szString, _T(""), bIgnoreCase);
}

// ----------------------------------------------------------------------- //
/// Removes all occurance of the character supplied in the ch parameter.  
///
/// @fn	        SS_String::Remove(TCHAR ch)
/// @param      ch [in] character to remove.
/// @return     The string length of the resulting string.
/// @author     Steve Schaneville
// ----------------------------------------------------------------------- //
INT SS_String::Remove(TCHAR ch)
{
    SS_String ssCh;
    ssCh.SetAt(0, ch);
    ssCh.SetAt(1, '\0');
    return Replace(ssCh, _T(""));
}

// ----------------------------------------------------------------------- //
/// Removes all occurance of szString.  Calling this function is identical to 
/// calling the Remove(LPCTSTR szString, BOOL bIgnoreCase) function and 
/// setting the bIgnoreCase parameter to TRUE.
///
/// @fn	        SS_String::RemoveNoCase(LPCTSTR szString)
/// @param      szString [in] null-terminated string to remove.
/// @return     The string length of the resulting string.
/// @author     Steve Schaneville
// ----------------------------------------------------------------------- //
INT SS_String::RemoveNoCase(LPCTSTR szString)
{
    return Remove(szString, TRUE);
}

// ----------------------------------------------------------------------- //
/// This function is primarily useful for debugging purposes.  You can quickly
/// display the error text associated with Windows API functions retreived
/// from the Windows GetLastError and FormatMessage commands by calling:
/// 
///		SS_String::GetLastError();
///
/// For end-user message boxes, though,  you should probably create your own 
/// error messages for display.  
///
/// @fn	        SS_String::GetLastError(BOOL bShowMessageBox, HWND pParent, 
///                                     BOOL bShowMessageBoxIfNoError)
/// @param      bShowMessageBox [in] if TRUE, specifies that a message box
///                  should be displayed containing the last error message.
/// @param      pParent [in] parent window of the message box.  This parameter
///                  is ignored if bShowMessageBox is FALSE.
/// @param      bShowMessageBoxIfNoError [in] if FALSE, a message box will 
///                  *not* be displayed if the error is ERROR_SUCCESS (i.e. 0).
///                  This parameter is ignored if bShowMessageBox is FALSE.
/// @return     This instance of the SS_String containing the formatted
///				error message.
/// @author     Steve Schaneville
// ----------------------------------------------------------------------- //
SS_String SS_String::GetLastError(BOOL bShowMessageBox,
                                   HWND pParent,
                                   BOOL bShowMessageBoxIfNoError)
{
    LPTSTR lpMsgBuf;
    DWORD dwResult = ::GetLastError();
    ::FormatMessage( FORMAT_MESSAGE_ALLOCATE_BUFFER |
                     FORMAT_MESSAGE_FROM_SYSTEM |
                     FORMAT_MESSAGE_IGNORE_INSERTS,
                     NULL,
                     dwResult,
                     MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT), // Default language
                     (LPTSTR) &lpMsgBuf,
                     0,
                     NULL );

    SS_String sError = lpMsgBuf;

    if( bShowMessageBox )
    {
        if( ERROR_SUCCESS == dwResult )
        {
            if(bShowMessageBoxIfNoError)
                sError.MessageBox( pParent, _T("::GetLastError()"), MB_OK|MB_ICONINFORMATION );
        }
        else
            sError.MessageBox( pParent, _T("::GetLastError()"), MB_OK|MB_ICONINFORMATION );
    }

    // Free the buffer.
    LocalFree( lpMsgBuf );

    return sError;
}

// ----------------------------------------------------------------------- //
/// This is an sprintf style function, taking a variable number of parameters
/// and formatting them into the local buffer based on the first parameter's
/// format string.
///
/// @fn	        SS_String::Format(LPCTSTR szString, ...)
/// @param      szString [in] null-terminated sprintf style format string.  
///							  This function will take a variable number of
///							  parameters, and all parameters following this 
///							  first one will be formatted into the format
///							  string in sprintf style.
/// @return     This instance of the SS_String containing the formatted
///				string.
/// @author     Steve Schaneville
// ----------------------------------------------------------------------- //
SS_String& SS_String::Format(LPCTSTR szString, ...)
{
    va_list args;
    va_start(args, szString);

    INT i = 1;
    INT nResult = 0;

    LPTSTR szBuffer = GetBuffer(SS_STR_UNIT_LEN);

    nResult = _vsntprintf(szBuffer, BufferSize(), szString, args);

    while( -1 == nResult )
    {
        ReleaseBuffer();
        i++;
        szBuffer = GetBuffer(SS_STR_UNIT_LEN*i);

		nResult = _vsntprintf(szBuffer, BufferSize(), szString, args);
    }

    ReleaseBuffer();

    va_end(args);

    return *this;
}

// ----------------------------------------------------------------------- //
/// Reads in an ASCII text file into the local buffer.  There is currently no
/// provision to avoid loading excessively large files, which would do 
/// numerous allocations and re-allocations of our local buffer.  So be 
/// careful about the files you expect this function to load.  There is
/// also no provision for asynchronous loads.  These features may be supported
/// in the future.
///
/// @fn	        SS_String::ReadFile(LPCTSTR szFileName, DWORD dwShareMode,
///								    LPSECURITY_ATTRIBUTES psa, 
///								    DWORD dwCreationDisposition,
///									DWORD dwFlagsAndAttributes, 
///								    HANDLE hTemplateFile)
/// @param      szFileName [in] filename and path to the ACSII text-based file
///                 that will be loaded into the local buffer.
/// @param		dwShareMode [in] specifies the access that other applicaations
///					can gain on the file while the SS_String has it open.  
///					Valid values are: 0, GENERIC_READ, and GENERIC_WRITE.
/// @param		psa [in] pointer to a SECURITY_ATTRIBUTES structure.  See
///					the MSDN CreateFile function for details of this 
///					structure.
///	@param		dwCreationDisposition [in] specifies instructions on 
///					what to do with existing or non-existing files.  Valid
///					values include CREATE_NEW, CREATE_ALWAYS,
///                 OPEN_EXISTING, OPEN_ALWAYS, and TRUNCATE_EXISTING.  See
///					the MSDN CreateFile function for details.
/// @param		dwFlagsAndAttributes [in] FILE_ATTRIBUTE_NORMAL, etc.  See
///					the MSDN CreateFile function for details.
/// @param		hTemplateFile [in] a handle with GENERIC_READ access to
///                 a template file, or NULL if no template file is to be
///					used.  See the MSDN CreateFile function for details.
/// @return     A DWORD value retrieved from the Windows API GetLastError
///				function after a call to either CreateFile or ReadFile.
/// @author     Steve Schaneville
// ----------------------------------------------------------------------- //
DWORD SS_String::ReadFile(LPCTSTR szFileName, DWORD dwShareMode,
                          LPSECURITY_ATTRIBUTES psa, DWORD dwCreationDisposition,
                          DWORD dwFlagsAndAttributes, HANDLE hTemplateFile)
{
    DWORD dwr = ERROR_SUCCESS;

    // check the parameters
    ASSERT(szFileName);
    if( szFileName == NULL )
        return ERROR_INVALID_PARAMETER;

    if( dwr == ERROR_SUCCESS )
    {
        // open the file
        HANDLE hFile = ::CreateFile( szFileName, GENERIC_READ, dwShareMode,
                                     psa, dwCreationDisposition, dwFlagsAndAttributes,
                                     hTemplateFile );
        dwr = ::GetLastError();
		SS_String sTmp;
		*this = sTmp;

        if( hFile != INVALID_HANDLE_VALUE )
        {
            INT i = 0;
            DWORD dwBytesRead = SS_STR_UNIT_LEN-1;
            while( dwBytesRead == SS_STR_UNIT_LEN-1 && dwr == ERROR_SUCCESS )
            {
                i++;
				sTmp = _T("");
                BOOL br = ::ReadFile( hFile, sTmp.GetBuffer(SS_STR_UNIT_LEN),
									  SS_STR_UNIT_LEN-1, &dwBytesRead, NULL );
				dwr = ::GetLastError();
				sTmp.SetAt(dwBytesRead, '\0');
                sTmp.ReleaseBuffer();
                if( br == FALSE )
                    break;

				if( dwr != ERROR_HANDLE_EOF )
					*this += sTmp;				
            }

            ::CloseHandle(hFile);
        }
    }

    return dwr;
}

// ----------------------------------------------------------------------- //
/// Writes an ASCII text file from the local buffer.  There is currently
/// no provision for asynchronous writes.  These features may be supported
/// in the future.
///
/// @fn	        SS_String::WriteFile(LPCTSTR szFileName, DWORD dwShareMode,
///								     LPSECURITY_ATTRIBUTES psa, 
///								     DWORD dwCreationDisposition,
///									 DWORD dwFlagsAndAttributes, 
///								     HANDLE hTemplateFile)
/// @param      szFileName [in] filename and path to the ACSII text-based file
///                 that will be written from the local buffer.
/// @param		dwShareMode [in] specifies the access that other applicaations
///					can gain on the file while the SS_String has it open.  
///					Valid values are: 0, GENERIC_READ, and GENERIC_WRITE.
/// @param		psa [in] pointer to a SECURITY_ATTRIBUTES structure.  See
///					the MSDN CreateFile function for details of this 
///					structure.
///	@param		dwCreationDisposition [in] specifies instructions on 
///					what to do with existing or non-existing files.  Valid
///					values include CREATE_NEW, CREATE_ALWAYS,
///                 OPEN_EXISTING, OPEN_ALWAYS, and TRUNCATE_EXISTING.  See
///					the MSDN CreateFile function for details.
/// @param		dwFlagsAndAttributes [in] FILE_ATTRIBUTE_NORMAL, etc.  See
///					the MSDN CreateFile function for details.
/// @param		hTemplateFile [in] a handle with GENERIC_READ access to
///                 a template file, or NULL if no template file is to be
///					used.  See the MSDN CreateFile function for details.
/// @return     A DWORD value retrieved from the Windows API GetLastError
///				function after a call to either CreateFile or WriteFile.
/// @author     Steve Schaneville
// ----------------------------------------------------------------------- //
DWORD SS_String::WriteFile(LPCTSTR szFileName, DWORD dwShareMode,
                           LPSECURITY_ATTRIBUTES psa, DWORD dwCreationDisposition,
                           DWORD dwFlagsAndAttributes, HANDLE hTemplateFile)
{
    DWORD dwr = ERROR_SUCCESS;

    // check the parameters
    ASSERT(szFileName);
    if( szFileName == NULL )
        return ERROR_INVALID_PARAMETER;

    if( dwr == ERROR_SUCCESS )
    {
        // open the file
        HANDLE hFile = ::CreateFile( szFileName, GENERIC_WRITE, dwShareMode,
            psa, dwCreationDisposition, dwFlagsAndAttributes,
            hTemplateFile );
        dwr = ::GetLastError();

        if( hFile != INVALID_HANDLE_VALUE )
        {
            DWORD dwBytesWritten = 0;
            ::WriteFile(hFile, String(), Length(), &dwBytesWritten, NULL);
            dwr = ::GetLastError();

            ::CloseHandle(hFile);
        }
    }

    return dwr;
}

// ----------------------------------------------------------------------- //
///	Provides a quick way to display a string resource ID in a message box.
/// Simply call:
///
///		SS_String::MessageBox(hInstance, nResourceID);
///
/// @fn	        SS_String::MessageBox(HINSTANCE hInstance,
///									  UINT nResourceID,
///									  HWND hWnd,
///									  LPCTSTR szTitle,
///									  UINT uType)
/// @param		hInstance [in] instance handle of the current process.
/// @param		nResourceID [in] a string resource ID containing the 
///					string that will be the message in the message box.
/// @param		hWnd [in] handle to the parent window.  Defaults to NULL.
/// @param		szTitle [in] null-terminated string containing text for
///					the title of the message box.  Defaults to "Dialog".
/// @param		uType [in] messagebox types, such as MB_OK or MB_YESNO. 
///					Defaults to MB_OK|MB_ICONINFORMATION.
/// @return     The user's response when closing the message box. 
/// @author     Steve Schaneville
// ----------------------------------------------------------------------- //
INT SS_String::MessageBox(HINSTANCE hInstance,
						  UINT nResourceID,
					 	  HWND hWnd,
						  LPCTSTR szTitle,
						  UINT uType )
{
	ASSERT(hInstance);
	SS_String sTmp(hInstance, nResourceID);

	return sTmp.MessageBox(hWnd, szTitle, uType);
}

// ----------------------------------------------------------------------- //
///	This function releases our string buffer, leaving it up
//  to the user of the class to delete the buffer.  This
//  class will no longer maintain responsibility for 
//  for the buffer.  We will, however, reinitialize
//  another memory block and copy the string into that
//  memory block so that our string instance keeps
//  its original value.  Be sure to the pointer returned by 
//  this function as follows:
//
//      delete [] pReturned;
///
/// @fn	        SS_String::DetachBuffer()
/// @return     The pointer to the local string buffer.
/// @author     Steve Schaneville
// ----------------------------------------------------------------------- //
LPTSTR SS_String::DetachBuffer()
{
    LPTSTR szReturn = GetBuffer();
    ReleaseBuffer();
    InitObject();
    String(szReturn);
    return szReturn;
}

// ----------------------------------------------------------------------- //
///	This operator is taken from the InstallShield scripting language.  It 
/// will concatenate two directory paths together, making sure that there is 
/// only 1 backslash between them.  Makes for easy assembling of directory
/// strings.  Note that we will add only backslashes.  If forward-slash paths 
/// are desired, call myString.Replace("\\", "/") after this function.  Note
/// that neither of the original two strings are altered during this call.
///
/// @fn	        SS_String::operator ^ (const SS_String& rhs)
/// @param		rhs [in] an SS_String containing a "path" to concatenate.
/// @return     An SS_String instance containing the concatenated result.
/// @author     Steve Schaneville
// ----------------------------------------------------------------------- //
SS_String SS_String::operator ^ (const SS_String& rhs)
{
    SS_String sResult;
    LPCTSTR szRhs = (LPCTSTR)rhs;
    sResult = (*this) ^ szRhs;
    return sResult;
}

// ----------------------------------------------------------------------- //
///	This operator is taken from the InstallShield scripting language.  It 
/// will concatenate two directory paths together, making sure that there is 
/// only 1 backslash between them.  Makes for easy assembling of directory
/// strings.  Note that we will add only backslashes.  If forward-slash paths 
/// are desired, call myString.Replace("\\", "/") after this function.  Note
/// that neither of the original two strings are altered during this call.
///
/// @fn	        SS_String::operator ^ (LPCTSTR rhs)
/// @param		rhs [in] null-terminated "path" to concatenate.
/// @return     An SS_String instance containing the concatenated result.
/// @author     Steve Schaneville
// ----------------------------------------------------------------------- //
SS_String SS_String::operator ^ (LPCTSTR rhs)
{
    SS_String sLhs = *this;
    TCHAR chEnd = sLhs[sLhs.Length()-1];
    if( chEnd != '\\' && chEnd != '/' )
        sLhs += _T("\\");
    
    SS_String sRhs = rhs;
    if( sRhs[0] == '\\' || sRhs[0] == '/' )
        sRhs = (sRhs.GetBuffer()) + sizeof(TCHAR);
    
    SS_String sResult = (sLhs + sRhs);
    chEnd = sResult[sResult.Length()-1];
    if( chEnd == '\\' || chEnd == '/' )
        sResult.SetAt(sResult.Length()-1, '\0');
    
    return sResult;
}

// ----------------------------------------------------------------------- //
///	This operator is taken from the InstallShield scripting language.  It 
/// will concatenate two directory paths together, making sure that there is 
/// only 1 backslash between them.  Makes for easy assembling of directory
/// strings.  Note that we will add only backslashes.  If forward-slash paths 
/// are desired, call myString.Replace("\\", "/") after this function.
///
/// @fn	        SS_String::operator ^= (LPCTSTR rhs)
/// @param		rhs [in] an SS_String containing a "path" to concatenate.
/// @return     This SS_String instance containing the concatenated result.
/// @author     Steve Schaneville
// ----------------------------------------------------------------------- //
SS_String& SS_String::operator ^= (LPCTSTR rhs)
{
	String( (LPCTSTR)((*this) ^ rhs) );
    return *this;
}

// ----------------------------------------------------------------------- //
///	This operator is taken from the InstallShield scripting language.  It 
/// will concatenate two directory paths together, making sure that there is 
/// only 1 backslash between them.  Makes for easy assembling of directory
/// strings.  Note that we will add only backslashes.  If forward-slash paths 
/// are desired, call myString.Replace("\\", "/") after this function.
///
/// @fn	        SS_String::operator ^= (const SS_String& rhs)
/// @param		rhs [in] null-terminated "path" to concatenate.
/// @return     This SS_String instance containing the concatenated result.
/// @author     Steve Schaneville
// ----------------------------------------------------------------------- //
SS_String& SS_String::operator ^= (const SS_String& rhs)
{
    *this ^= rhs.String();
    return *this;
}

/// 
// ----------------------------------------------------------------------- //
///	The _ss_tcstok() function was written to replace the _tcstok function
/// because the _tcstok() function skips leading and trailing tokens, but we
/// think that leading and trailing tokens are important, and are indeed
/// delimiters signifying blank tokens.  See the MSDN strtok function for
/// details, as this function's behavior mimics that function aside from 
/// the above mentioned differences.
///
/// @fn	        SS_String::_ss_tcstok(LPTSTR szTokens, LPCTSTR szDelims)
/// @param		szTokens [in] the string containing the tokens that will
///					be parsed.
///	@param		szDelims [in] a list of delimiters.
/// @return     The next token in the list, as a null-terminated string.
/// @author     Steve Schaneville
// ----------------------------------------------------------------------- //
LPTSTR SS_String::_ss_tcstok(LPTSTR szTokens, LPCTSTR szDelims)
{
    if( !szTokens )
        szTokens = m_sz_ss_tcstok_pos;
	
    ASSERT(szTokens && szDelims);
    LPTSTR szCurrentDelim = (LPTSTR)szDelims - 1;
    LPTSTR szTokensPos = szTokens - 1;
    while( *(++szTokensPos) != '\0' )
    {
        while( *(++szCurrentDelim) != '\0' )
        {
            if( *szTokensPos == *szCurrentDelim )
            {
                // prep for next call...
                // check to make sure that we didn't have a trailing delimiter
                szCurrentDelim = (LPTSTR)szDelims - 1;
                while( *(++szCurrentDelim) != '\0' )
                {
                    TCHAR szSingleDelim[SS_STR_UNIT_LEN];
                    _tcsncpy( szSingleDelim, szCurrentDelim, 1 );
                    _tcscpy( szSingleDelim+1, _T("\0"));
                    if( !_tcscmp(szTokensPos, szSingleDelim) )
                        m_b_ss_tcstok_trailingToken = TRUE;
                }
				
                // now place '\0' and return our token
                *szTokensPos = '\0';
                m_sz_ss_tcstok_pos = szTokensPos+1;
                return szTokens;
            }
        }
        szCurrentDelim = (LPTSTR)szDelims - 1;
    }
	
    // check for trailing tokens
    if( szTokensPos != szTokens )
    {
        m_sz_ss_tcstok_pos = szTokensPos;
        return szTokens;
    }
	
    if( m_b_ss_tcstok_trailingToken )
    {
        m_b_ss_tcstok_trailingToken = FALSE;
        return szTokens;
    }
	
    return NULL;
}

