// ----------------------------------------------------------------------- //
///
/// \file       SS_String.h
/// \author     Steve Schaneville
/// \date       27 Nov 2001, 09:43
/// \brief      Header file for the SS_String class.
///
///  
///
/// Copyright (c) 2001 - 2003\n
/// z_web_master@yahoo.com\n
/// http://schaneville.webhop.net/SS_Utils
///
// ----------------------------------------------------------------------- //
#ifndef __SS_String_h__
#define __SS_String_h__

// ------------------[       Pre-Include Defines       ]------------------ //
// ------------------[          Include Files          ]------------------ //
#include <stdio.h>
#include <wtypes.h>
#include <tchar.h>
#include <assert.h>

// ------------------[          Macros/Defines         ]------------------ //

/// "SS_STR_UNIT_LEN" defines the unit length of the string.  Memory is reserved
/// for all strings as a multiple of this number.  The reason for doing this
/// is to prevent memory fragmentation.  To illustrate memory fragmentation,
/// here is an example: if a user asks for 6 characters for a string (with "new"),
/// then asks for more memory for some unrelated reason, then "delete"s the
/// first string, a memory hole of 6 characters is created.  Because the computer
/// will always reserve memory in contiguous blocks, it will re-use that 6
/// character hole only if the user asks for more memory that is 6 characters
/// or less.  When an application remains open for an extended period and
/// creates many of these little holes, the computer behaves much like the
/// program had caused memory leaks, because the computer will have trouble
/// finding large enough memory locations to reserve new memory when asked for
/// it.  To prevent this, we always reserve memory in multiples of some number.
/// This way, when we free the memory and ask for more, it is very likely that
/// our next request will be for the same amount of memory that we freed earlier
/// and the computer will re-use that same memory area.
#define SS_STR_UNIT_LEN 64 // the unit length of the string

/// If the project including the SS_String class has not yet #declared the
/// ASSERT macro, do it for them now.
#ifndef ASSERT
#define ASSERT assert
#endif

// ------------------[      Constants/Enumerations     ]------------------ //
// ------------------[       Forward Declarations      ]------------------ //
// ------------------[         Global Variables        ]------------------ //
// ------------------[         Global Functions        ]------------------ //
// ------------------[             Classes             ]------------------ //

// ----------------------------------------------------------------------- //
/// @class		SS_String
/// @author		Steve Schaneville
/// @brief		Drop-in replacement for MFC's CString with added 
///             functionality.
///
/// This string class mimics MFC's CString class, including virtually all 
/// functions found in that class.  The behavior is similar to CString's 
/// functions, with select few functions left out.  This class, however, 
/// offers a large amount of functionality more than the CString class.
// ----------------------------------------------------------------------- //
class SS_String
{
public:

    /// @{ @name Constructors
	SS_String();

	SS_String					    		            (const SS_String& ssString);
    SS_String					    		            (LPCTSTR szString);
    SS_String					    		            (BSTR bstrString);
    SS_String                                           (INT nValue);
    SS_String					    		            (HINSTANCE hInstance,
                                                         UINT nResourceID);
#if defined(UNICODE) || defined(_UNICODE)
    SS_String					    		            (const char* szString);
#endif
    /// @}

    /// @{ @name Destructor
    virtual ~SS_String();
    /// @}

protected:

	/// @ingroup Constructors
    virtual VOID	            InitObject		        ();

public:

	/// @{ @name Buffer Accessor Functions
    LPCTSTR                     String                  () const;
    VOID                        String                  (LPCTSTR szString);
    VOID                        String                  (const BSTR bstrString);
#if defined(UNICODE) || defined(_UNICODE)
    VOID                        String                  (const char* szString);
#endif
    INT                         Length                  () const;
    LPTSTR                      DetachBuffer            ();    
    LPTSTR                      GetBuffer               (INT nMinLength = -1);
    LPCTSTR                     GetBufferConst          ();
    VOID                        ReleaseBuffer           ();
    TCHAR                       GetAt                   (INT nPos);
    VOID                        SetAt                   (INT nPos, TCHAR nCharacter);
    SS_String                   Left                    (INT nCount) const;
    SS_String                   Right                   (INT nCount) const;
    SS_String                   Mid                     (INT nFirst, INT nCount) const;
	/// @}


    /// @{ @name Comparison Operators
    BOOL                        operator ==             (const SS_String& rhs) const;
    BOOL                        operator !=             (const SS_String& rhs) const;
    BOOL                        operator >=             (const SS_String& rhs) const;
    BOOL                        operator <=             (const SS_String& rhs) const;
    BOOL                        operator >              (const SS_String& rhs) const;
    BOOL                        operator <              (const SS_String& rhs) const;

    BOOL                        operator ==             (LPCTSTR rhs) const;
    BOOL                        operator !=             (LPCTSTR rhs) const;
    BOOL                        operator >=             (LPCTSTR rhs) const;
    BOOL                        operator <=             (LPCTSTR rhs) const;
    BOOL                        operator >              (LPCTSTR rhs) const;
    BOOL                        operator <              (LPCTSTR rhs) const;

    BOOL                        operator ==             (const BSTR rhs) const;
    BOOL                        operator !=             (const BSTR rhs) const;
    BOOL                        operator >=             (const BSTR rhs) const;
    BOOL                        operator <=             (const BSTR rhs) const;
    BOOL                        operator >              (const BSTR rhs) const;
    BOOL                        operator <              (const BSTR rhs) const;
    /// @}

    /// @{ @name Casts
    operator LPCTSTR                                    () const;
    operator LPTSTR                                     ();
#if !defined(_UNICODE) || !defined(UNICODE)
    operator BSTR                                       ();
#endif
    operator DOUBLE                                     ();
    operator FLOAT                                      ();
    operator LONG                                       ();
    operator INT                                        ();
    operator DWORD                                      ();
    operator WORD                                       ();
    /// @}

    /// @{ @name Assignment Operators
    SS_String&                  operator =              (LPCTSTR szString);
    SS_String&                  operator =              (const BSTR bstrString);
    SS_String&			        operator =		        (const SS_String& rhs);
#if defined(UNICODE) || defined(_UNICODE)
    SS_String&                  operator =              (const char* szString);
#endif

    SS_String&                  operator +=             (const SS_String& rhs);
    SS_String&                  operator +=             (LPCTSTR rhs);
    SS_String&                  operator +=             (const BSTR rhs);

    SS_String&                  operator ^=             (const SS_String& rhs);
    SS_String&                  operator ^=             (LPCTSTR rhs);
    /// @}

    /// @{ @name Additive Operators
    SS_String                   operator +              (const SS_String& rhs);
    SS_String                   operator +              (LPCTSTR rhs);
    SS_String                   operator +              (const BSTR rhs);
    
    SS_String                   operator ^              (const SS_String& rhs);
    SS_String                   operator ^              (LPCTSTR rhs);
    /// @}

    /// @{ @name Other Operators
    TCHAR                       operator []             (INT nPos);
    /// @}

	/// @{ @name Token Functions
    LPTSTR                      Token                   (INT nTokenIndex, LPCTSTR szDelimiters=NULL);
    LPTSTR                      FirstToken              (LPCTSTR szDelimiters=NULL);
    LPTSTR                      LastToken               (LPCTSTR szDelimiters=NULL);
    LPTSTR                      NextToken               ();
    LPTSTR                      PreviousToken           ();
	/// @}

	/// @{ @name Reporting Functions
    INT                         MessageBox              (HWND hWnd = NULL,
                                                         LPCTSTR szTitle = _T("Dialog"),
                                                         UINT uType = MB_OK|MB_ICONINFORMATION) const;
    static INT                  MessageBox              (HINSTANCE hInstance,
                                                         UINT nResourceID,
                                                         HWND hWnd = NULL,
                                                         LPCTSTR szTitle = _T("Dialog"),
                                                         UINT uType = MB_OK|MB_ICONINFORMATION);
    static SS_String            GetLastError            (BOOL bShowMessageBox=TRUE,
														 HWND pParent=NULL,
														 BOOL bShowMessageBoxIfNoError=FALSE);
	/// @}
    
	
	/// @{ @name Search and Replace Functions
    SS_String                   Find                    (LPCTSTR szFindString,
                                                         INT nStartPos = 0,
                                                         INT* pnFoundAtPos = NULL,
                                                         BOOL bIgnoreCase = FALSE);
    SS_String                   FindReverse             (LPCTSTR szFindString,
                                                         INT nStartPos = -1,
                                                         INT* pnFoundAtPos = NULL,
                                                         BOOL bIgnoreCase = FALSE);
    INT                         Replace                 (TCHAR chOld, TCHAR chNew);
    INT                         Replace                 (LPCTSTR szOld, 
														 LPCTSTR szNew, 
														 BOOL bIgnoreCase = FALSE);
    INT                         ReplaceNoCase           (LPCTSTR szOld, 
														 LPCTSTR szNew);
    INT                         Insert                  (INT nPos, TCHAR ch);
    INT                         Insert                  (INT nPos, LPCTSTR szString);
    INT                         Delete                  (INT nPos, INT nCount);
    INT                         Delete                  (LPCTSTR szString,
														 BOOL bIgnoreCase = FALSE);
    INT                         DeleteNoCase            (LPCTSTR szString);
    INT                         Remove                  (LPCTSTR szString, 
														 BOOL bIgnoreCase = FALSE);
    INT                         Remove                  (TCHAR ch);
    INT                         RemoveNoCase            (LPCTSTR szString);

    BOOL                        IsEmpty                 () const;
    INT                         Compare                 (LPCTSTR szString) const;
    INT                         CompareNoCase           (LPCTSTR szString) const;
	/// @}

	/// @{ Formatting Functions
    SS_String&                  TrimLeft                ();
    SS_String&                  TrimRight               ();
    SS_String&                  Format                  (LPCTSTR szString, ...);
    SS_String&                  LoadString              (HINSTANCE hInstance,
														 UINT nResourceID);
	/// @}

	/// @{ @name File Access Functions
    DWORD                       ReadFile                (LPCTSTR szFileName,
                                                         DWORD dwShareMode = FILE_SHARE_READ,
                                                         LPSECURITY_ATTRIBUTES psa = NULL,
                                                         DWORD dwCreationDisposition = OPEN_EXISTING,
                                                         DWORD dwFlagsAndAttributes = FILE_ATTRIBUTE_NORMAL,
                                                         HANDLE hTemplateFile = NULL);
    DWORD                       WriteFile               (LPCTSTR szFileName,
                                                         DWORD dwShareMode = NULL,
                                                         LPSECURITY_ATTRIBUTES psa = NULL,
                                                         DWORD dwCreationDisposition = CREATE_NEW,
                                                         DWORD dwFlagsAndAttributes = FILE_ATTRIBUTE_NORMAL,
                                                         HANDLE hTemplateFile = NULL);
	/// @}

protected:

    LPTSTR                      _ss_tcstok              (LPTSTR szTokens, LPCTSTR szDelims);

private:

	/// @name Private Accessor Functions
    INT                         References              () const;
    VOID                        References              (INT nReferences);
    INT                         AddReference            ();
    INT                         RemoveReference         ();

    VOID                        FreeTokens              ();
    VOID                        InitTokens              (LPCTSTR szDelimiters);

    BOOL                        TokensAreValid          () const;
    VOID                        TokensAreValid          (BOOL bTokensAreValid);
    INT                         CurrentToken            () const;
    VOID                        CurrentToken            (INT nCurrentToken);
    INT                         TokenCount              () const;
    VOID                        TokenCount              (INT nCount);
    SS_String*                  Tokens                  () const;
    VOID                        Tokens                  (SS_String* ppString);

    INT                         BufferSize              () const;
    VOID                        BufferSize              (INT nSize);
    INT                         InitString              (INT nLength);
	/// @}

	/// @{ name Member Variables
    LPTSTR      m_szString;         ///< The local string buffer.
    INT         m_nBufferSize;      ///< The allocated size of memory for the local string buffer.
    SS_String*  m_ppTokens;         ///< The array of tokens currently stored.
    INT         m_nTokenCount;      ///< The count of tokens currently stored.
    INT         m_nCurrentToken;    ///< The current token index (used for NextToken() & PreviousToken()).
    BOOL        m_bTokensAreValid;  ///< Has string been changed since we looked at tokens?
                                    ///  If this is true, we must re-initialize the 
                                    ///  token list before returning any tokens
                                    ///  to the user.
    INT         m_nReferences;      ///< number of outstanding references obtained
                                    ///  by the GetBuffer() function.  These must be
                                    ///  released by the ReleaseBuffer() function.
    LPTSTR      m_sz_ss_tcstok_pos; ///< Global position pointer to keep track of 
                                    ///  where we are in the search for more tokens by the 
                                    ///  _ss_tcstok() function.
    BOOL        m_b_ss_tcstok_trailingToken;
                                    ///< Global flag to keep track of whether or not we 
                                    ///  have a token an the end of our search string 
                                    ///  passed into the _ss_tcstok() function
	/// @}

};


// ----------------------------------------------------------------------- //
//  SS_String Inline Functions
// ----------------------------------------------------------------------- //

/// Returns a const pointer to the local string buffer.
inline LPCTSTR SS_String::String() const
{ return m_szString; }

/// Returns the string length, not including the terminating NULL (like strlen())
inline INT SS_String::Length() const
{ return _tcslen(m_szString); }

/// Returns the size of the allocated local buffer.
inline INT SS_String::BufferSize() const
{ return m_nBufferSize; }

/// Sets the value of the member variable that keeps track of the allocated
/// buffer size.  Note this this does NOT re-allocate the buffer size.
inline VOID SS_String::BufferSize(INT nLength)
{ m_nBufferSize = nLength; }

/// Number of tokens found based on delimiters passed into the Token, FirstToken,
/// and LastToken functions.
inline INT SS_String::TokenCount() const
{ return m_nTokenCount; }

/// Sets the value of the member variable that keeps track of the number of
/// tokens found.  This does NOT change the list of tokens.
inline VOID SS_String::TokenCount(INT nCount)
{ m_nTokenCount = nCount; }

/// Returns TRUE if tokens are currently valid, and FALSE if not.  Tokens become
/// invalid when the string changes after the tokens have been calculated.
inline BOOL SS_String::TokensAreValid() const
{ return m_bTokensAreValid; }

/// Sets the value of the member variable that keeps track of whether or 
/// not the strings are valid.
inline VOID SS_String::TokensAreValid(BOOL bTokensAreValid)
{ m_bTokensAreValid = bTokensAreValid; }

/// Gets the index of the current token.
inline INT SS_String::CurrentToken() const
{ return m_nCurrentToken; }

/// Sets the index of the current token.
inline VOID SS_String::CurrentToken(INT nCurrentToken)
{ m_nCurrentToken = nCurrentToken; }

/// Returns a pointer to the array of stored tokens.
inline SS_String* SS_String::Tokens() const
{ return m_ppTokens; }

/// Sets the pointer to the array of stored tokens.  This is done when
/// allocating a new array of tokens.
inline VOID SS_String::Tokens(SS_String* ppTokens)
{ m_ppTokens = ppTokens; }

/// Increments the reference count.  
inline INT SS_String::AddReference()
{ return ++m_nReferences; }

/// Decrements the reference count, unless the reference count is already
/// zero, inc which case we leave it at zero.
inline INT SS_String::RemoveReference()
{ if(-1 == --m_nReferences) m_nReferences = 0; return m_nReferences; }

/// Returns the current reference count.
inline INT SS_String::References() const
{ return m_nReferences; }

/// Sets the current reference count.
inline VOID SS_String::References(INT nReferences)
{ m_nReferences = nReferences; }

/// Returns the character at the index specified in nPos.
inline TCHAR SS_String::GetAt(INT nPos)
{ ASSERT(nPos < BufferSize()); return *(String()+nPos); }

/// Replaces the character at the index specified in nPos with the character
/// specified in nCharacter.
inline VOID SS_String::SetAt(INT nPos, TCHAR nCharacter)
{ ASSERT(nPos < BufferSize()); *(GetBuffer(0)+nPos) = nCharacter; ReleaseBuffer(); }

/// Returns TRUE if our local string == "" or FALSE otherwise.
inline BOOL SS_String::IsEmpty() const
{ return *this == _T("") ? TRUE : FALSE; }

/// Returns a value less than 0 if the local buffer is less than szString, 
/// 0 if the local buffer == szString, and a value greater than zero if
/// the local buffer is greater than szString.  See MSDN strcmp for more 
/// details.
inline INT SS_String::Compare(LPCTSTR szString) const
{ return _tcscmp(String(), szString); }

/// Returns a value less than 0 if the local buffer is less than szString, 
/// 0 if the local buffer == szString, and a value greater than zero if
/// the local buffer is greater than szString, all reguardless of case.
/// See MSDN stricmp for more details.
inline INT SS_String::CompareNoCase(LPCTSTR szString) const
{ return _tcsicmp(String(), szString); }

/// Copy the contents of szString into the local buffer.
inline SS_String& SS_String::operator = (LPCTSTR szString)
{ String(szString); return *this; }

/// Copy the contents of the BSTR string into the local buffer, converting
/// to ANSI if we are in an ANSI build.
inline SS_String& SS_String::operator = (const BSTR bstrString)
{ String(bstrString); return *this; }

#if defined(UNICODE) || defined(_UNICODE)
/// Copy the contents of szString into the local buffer, converting
/// to UNICODE before the copy.
inline SS_String& SS_String::operator = (const char* szString)
{ String(szString); return *this; }
#endif

/// Returns TRUE if rhs is equal to our local buffer, and
/// FALSE otherwise.
inline BOOL SS_String::operator == (LPCTSTR rhs) const
{ return _tcscmp(String(), rhs) == 0 ? TRUE : FALSE; }

/// Returns TRUE if rhs is not equal to our local buffer, and
/// FALSE otherwise.
inline BOOL SS_String::operator != (LPCTSTR rhs) const
{ return _tcscmp(String(), rhs) != 0 ? TRUE : FALSE; }

/// Returns TRUE if rhs is greater than or equal to our local buffer, and
/// FALSE otherwise.
inline BOOL SS_String::operator >= (LPCTSTR rhs) const
{ return _tcscmp(String(), rhs) >= 0 ? TRUE : FALSE; }

/// Returns TRUE if rhs is less than or equal to our local buffer, and
/// FALSE otherwise.
inline BOOL SS_String::operator <= (LPCTSTR rhs) const
{ return _tcscmp(String(), rhs) <= 0 ? TRUE : FALSE; }

/// Returns TRUE if rhs is greater than our local buffer, and
/// FALSE otherwise.
inline BOOL SS_String::operator > (LPCTSTR rhs) const
{ return _tcscmp(String(), rhs) > 0 ? TRUE : FALSE; }

/// Returns TRUE if rhs is less than our local buffer, and
/// FALSE otherwise.
inline BOOL SS_String::operator < (LPCTSTR rhs) const
{ return _tcscmp(String(), rhs) < 0 ? TRUE : FALSE; }

/// Returns TRUE if rhs is equal to our local buffer, and
/// FALSE otherwise.
inline BOOL SS_String::operator == (const SS_String& rhs) const
{ return _tcscmp(String(), rhs.String()) == 0 ? TRUE : FALSE; }

/// Returns TRUE if rhs is not equal to our local buffer, and
/// FALSE otherwise.
inline BOOL SS_String::operator != (const SS_String& rhs) const
{ return _tcscmp(String(), rhs.String()) != 0 ? TRUE : FALSE; }

/// Returns TRUE if rhs is greater than or equal to our local buffer, and
/// FALSE otherwise.
inline BOOL SS_String::operator >= (const SS_String& rhs) const
{ return _tcscmp(String(), rhs.String()) >= 0 ? TRUE : FALSE; }

/// Returns TRUE if rhs is less than or equal to our local buffer, and
/// FALSE otherwise.
inline BOOL SS_String::operator <= (const SS_String& rhs) const
{ return _tcscmp(String(), rhs.String()) <= 0 ? TRUE : FALSE; }

/// Returns TRUE if rhs is greater than our local buffer, and
/// FALSE otherwise.
inline BOOL SS_String::operator > (const SS_String& rhs) const
{ return _tcscmp(String(), rhs.String()) > 0 ? TRUE : FALSE; }

/// Returns TRUE if rhs is less than our local buffer, and
/// FALSE otherwise.
inline BOOL SS_String::operator < (const SS_String& rhs) const
{ return _tcscmp(String(), rhs.String()) < 0 ? TRUE : FALSE; }

/// Casts our local buffer to a const char (or wchar_t in UNICODE) pointer
inline SS_String::operator LPCTSTR () const
{ return m_szString; }

/// Casts our local buffer to a char (or wchar_t in UNICODE) pointer
inline SS_String::operator LPTSTR ()
{ return m_szString; }

/// Casts our local buffer to an INT.  This function will parse the local
/// string buffer and turn it into an INT.  See MSDN atoi for more details.
inline SS_String::operator INT ()
{ return _ttoi(String()); }

/// Casts our local buffer to an LONG.  This function will parse the local
/// string buffer and turn it into an LONG.  See MSDN atoi for more details.
inline SS_String::operator LONG ()
{ return _ttol(String()); }

/// Casts our local buffer to an WORD.  This function will parse the local
/// string buffer and turn it into an WORD.  See MSDN strtoul for more details.
inline SS_String::operator WORD ()
{ return (USHORT)_tcstoul(String(), NULL, 10); }

/// Casts our local buffer to an DWORD.  This function will parse the local
/// string buffer and turn it into an DWORD.  See MSDN strtoul for more details.
inline SS_String::operator DWORD ()
{ return _tcstoul(String(), NULL, 10); }

/// Returns the character from our local string buffer located at the index 
/// specified in the brackets.
inline TCHAR SS_String::operator [] (INT nPos)
{ return GetAt(nPos); }

/// Returns TRUE if lhs is equal to our local buffer, and
/// FALSE otherwise.
inline BOOL operator == (LPCTSTR lhs, const SS_String& rhs)
{ return _tcscmp(lhs, rhs.String()) == 0 ? TRUE : FALSE; }

/// Returns TRUE if lhs is not equal to our local buffer, and
/// FALSE otherwise.
inline BOOL operator != (LPCTSTR lhs, const SS_String& rhs)
{ return _tcscmp(lhs, rhs.String()) != 0 ? TRUE : FALSE; }

/// Returns TRUE if lhs is greater than or equal to our local buffer, and
/// FALSE otherwise.
inline BOOL operator >= (LPCTSTR lhs, const SS_String& rhs)
{ return _tcscmp(lhs, rhs.String()) >= 0 ? TRUE : FALSE; }

/// Returns TRUE if lhs is less than or equal to our local buffer, and
/// FALSE otherwise.
inline BOOL operator <= (LPCTSTR lhs, const SS_String& rhs)
{ return _tcscmp(lhs, rhs.String()) <= 0 ? TRUE : FALSE; }

/// Returns TRUE if lhs is greater than our local buffer, and
/// FALSE otherwise.
inline BOOL operator > (LPCTSTR lhs, const SS_String& rhs)
{ return _tcscmp(lhs, rhs.String()) > 0 ? TRUE : FALSE; }

/// Returns TRUE if lhs is less than our local buffer, and
/// FALSE otherwise.
inline BOOL operator < (LPCTSTR lhs, const SS_String& rhs)
{ return _tcscmp(lhs, rhs.String()) < 0 ? TRUE : FALSE; }


/// Returns TRUE if lhs is equal to our local buffer, and
/// FALSE otherwise.  Converts the BSTR to ANSI before
/// the concatenation if we are in an ANSI build.
inline BOOL operator == (BSTR lhs, const SS_String& rhs)
{ return _tcscmp(SS_String(lhs), rhs.String()) == 0 ? TRUE : FALSE; }

/// Returns TRUE if lhs is not equal to our local buffer, and
/// FALSE otherwise.  Converts the BSTR to ANSI before
/// the concatenation if we are in an ANSI build.
inline BOOL operator != (BSTR lhs, const SS_String& rhs)
{ return _tcscmp(SS_String(lhs), rhs.String()) != 0 ? TRUE : FALSE; }

/// Returns TRUE if lhs is greater than or equal to our local buffer, and
/// FALSE otherwise.  Converts the BSTR to ANSI before
/// the concatenation if we are in an ANSI build.
inline BOOL operator >= (BSTR lhs, const SS_String& rhs)
{ return _tcscmp(SS_String(lhs), rhs.String()) >= 0 ? TRUE : FALSE; }

/// Returns TRUE if lhs is less than or equal to our local buffer, and
/// FALSE otherwise.  Converts the BSTR to ANSI before
/// the concatenation if we are in an ANSI build.
inline BOOL operator <= (BSTR lhs, const SS_String& rhs)
{ return _tcscmp(SS_String(lhs), rhs.String()) <= 0 ? TRUE : FALSE; }

/// Returns TRUE if lhs is greater than our local buffer, and
/// FALSE otherwise.  Converts the BSTR to ANSI before
/// the concatenation if we are in an ANSI build.
inline BOOL operator > (BSTR lhs, const SS_String& rhs)
{ return _tcscmp(SS_String(lhs), rhs.String()) > 0 ? TRUE : FALSE; }

/// Returns TRUE if lhs is less than our local buffer, and
/// FALSE otherwise.  Converts the BSTR to ANSI before
/// the concatenation if we are in an ANSI build.
inline BOOL operator < (BSTR lhs, const SS_String& rhs)
{ return _tcscmp(SS_String(lhs), rhs.String()) < 0 ? TRUE : FALSE; }

/// Concatenates the lhs and rhs parameters and returns an SS_String
/// with the concatentated results.
inline SS_String operator + (LPCTSTR lhs, const SS_String& rhs)
{
    SS_String ssResult = lhs;
    ssResult += rhs;
    return ssResult;
}

/// Concatenates the lhs and rhs parameters and returns an SS_String
/// with the concatentated results.  Converts the BSTR to ANSI before
/// the concatenation if we are in an ANSI build.
inline SS_String operator + (BSTR lhs, const SS_String& rhs)
{
    SS_String ssResult = lhs;
    ssResult += rhs;
    return ssResult;
}


#endif // __SS_String_h__
