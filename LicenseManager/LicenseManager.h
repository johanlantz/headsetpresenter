

/* this ALWAYS GENERATED file contains the definitions for the interfaces */


 /* File created by MIDL compiler version 6.00.0361 */
/* at Wed Jan 24 09:59:42 2007
 */
/* Compiler settings for .\LicenseManager.idl:
    Oicf, W1, Zp8, env=Win32 (32b run)
    protocol : dce , ms_ext, c_ext, robust
    error checks: allocation ref bounds_check enum stub_data 
    VC __declspec() decoration level: 
         __declspec(uuid()), __declspec(selectany), __declspec(novtable)
         DECLSPEC_UUID(), MIDL_INTERFACE()
*/
//@@MIDL_FILE_HEADING(  )

#pragma warning( disable: 4049 )  /* more than 64k source lines */


/* verify that the <rpcndr.h> version is high enough to compile this file*/
#ifndef __REQUIRED_RPCNDR_H_VERSION__
#define __REQUIRED_RPCNDR_H_VERSION__ 475
#endif

#include "rpc.h"
#include "rpcndr.h"

#ifndef __RPCNDR_H_VERSION__
#error this stub requires an updated version of <rpcndr.h>
#endif // __RPCNDR_H_VERSION__

#ifndef COM_NO_WINDOWS_H
#include "windows.h"
#include "ole2.h"
#endif /*COM_NO_WINDOWS_H*/

#ifndef __LicenseManager_h__
#define __LicenseManager_h__

#if defined(_MSC_VER) && (_MSC_VER >= 1020)
#pragma once
#endif

/* Forward Declarations */ 

#ifndef __ILicenseHandler_FWD_DEFINED__
#define __ILicenseHandler_FWD_DEFINED__
typedef interface ILicenseHandler ILicenseHandler;
#endif 	/* __ILicenseHandler_FWD_DEFINED__ */


#ifndef __LicenseHandler_FWD_DEFINED__
#define __LicenseHandler_FWD_DEFINED__

#ifdef __cplusplus
typedef class LicenseHandler LicenseHandler;
#else
typedef struct LicenseHandler LicenseHandler;
#endif /* __cplusplus */

#endif 	/* __LicenseHandler_FWD_DEFINED__ */


/* header files for imported files */
#include "oaidl.h"
#include "ocidl.h"

#ifdef __cplusplus
extern "C"{
#endif 

void * __RPC_USER MIDL_user_allocate(size_t);
void __RPC_USER MIDL_user_free( void * ); 

#ifndef __ILicenseHandler_INTERFACE_DEFINED__
#define __ILicenseHandler_INTERFACE_DEFINED__

/* interface ILicenseHandler */
/* [unique][helpstring][nonextensible][dual][uuid][object] */ 


EXTERN_C const IID IID_ILicenseHandler;

#if defined(__cplusplus) && !defined(CINTERFACE)
    
    MIDL_INTERFACE("0246D106-5CB9-4e75-8333-05FED161D9B1")
    ILicenseHandler : public IDispatch
    {
    public:
        virtual /* [helpstring][id][propget] */ HRESULT STDMETHODCALLTYPE get_status( 
            /* [retval][out] */ LONG *pVal) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE CheckLicense( 
            /* [in] */ BSTR productName,
            /* [in] */ BSTR productVersion,
            /* [retval][out] */ BSTR *resultAsString) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE CreateLicFile( 
            /* [in] */ BSTR productName,
            /* [in] */ BSTR productVersion,
            /* [retval][out] */ BSTR *statusAsString) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE AllowAnyValidLicenseVersion( void) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE QueryForEmail( void) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE SetRegistrationComment( 
            /* [in] */ BSTR iregistrationComment) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE CRCCheckSelf( 
            /* [in] */ BSTR fullPathToApplication,
            /* [in] */ BSTR fullPathToNonAppendedCRC,
            /* [retval][out] */ BSTR *resultAsString) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE SetMinVersionAllowed( 
            /* [in] */ DOUBLE minVersionAllowed,
            /* [retval][out] */ BSTR *resultAsString) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE Authenticate( 
            /* [in] */ INT challenge,
            /* [retval][out] */ INT *response) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE Activate( 
            /* [in] */ INT secretKey) = 0;
        
    };
    
#else 	/* C style interface */

    typedef struct ILicenseHandlerVtbl
    {
        BEGIN_INTERFACE
        
        HRESULT ( STDMETHODCALLTYPE *QueryInterface )( 
            ILicenseHandler * This,
            /* [in] */ REFIID riid,
            /* [iid_is][out] */ void **ppvObject);
        
        ULONG ( STDMETHODCALLTYPE *AddRef )( 
            ILicenseHandler * This);
        
        ULONG ( STDMETHODCALLTYPE *Release )( 
            ILicenseHandler * This);
        
        HRESULT ( STDMETHODCALLTYPE *GetTypeInfoCount )( 
            ILicenseHandler * This,
            /* [out] */ UINT *pctinfo);
        
        HRESULT ( STDMETHODCALLTYPE *GetTypeInfo )( 
            ILicenseHandler * This,
            /* [in] */ UINT iTInfo,
            /* [in] */ LCID lcid,
            /* [out] */ ITypeInfo **ppTInfo);
        
        HRESULT ( STDMETHODCALLTYPE *GetIDsOfNames )( 
            ILicenseHandler * This,
            /* [in] */ REFIID riid,
            /* [size_is][in] */ LPOLESTR *rgszNames,
            /* [in] */ UINT cNames,
            /* [in] */ LCID lcid,
            /* [size_is][out] */ DISPID *rgDispId);
        
        /* [local] */ HRESULT ( STDMETHODCALLTYPE *Invoke )( 
            ILicenseHandler * This,
            /* [in] */ DISPID dispIdMember,
            /* [in] */ REFIID riid,
            /* [in] */ LCID lcid,
            /* [in] */ WORD wFlags,
            /* [out][in] */ DISPPARAMS *pDispParams,
            /* [out] */ VARIANT *pVarResult,
            /* [out] */ EXCEPINFO *pExcepInfo,
            /* [out] */ UINT *puArgErr);
        
        /* [helpstring][id][propget] */ HRESULT ( STDMETHODCALLTYPE *get_status )( 
            ILicenseHandler * This,
            /* [retval][out] */ LONG *pVal);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *CheckLicense )( 
            ILicenseHandler * This,
            /* [in] */ BSTR productName,
            /* [in] */ BSTR productVersion,
            /* [retval][out] */ BSTR *resultAsString);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *CreateLicFile )( 
            ILicenseHandler * This,
            /* [in] */ BSTR productName,
            /* [in] */ BSTR productVersion,
            /* [retval][out] */ BSTR *statusAsString);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *AllowAnyValidLicenseVersion )( 
            ILicenseHandler * This);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *QueryForEmail )( 
            ILicenseHandler * This);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *SetRegistrationComment )( 
            ILicenseHandler * This,
            /* [in] */ BSTR iregistrationComment);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *CRCCheckSelf )( 
            ILicenseHandler * This,
            /* [in] */ BSTR fullPathToApplication,
            /* [in] */ BSTR fullPathToNonAppendedCRC,
            /* [retval][out] */ BSTR *resultAsString);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *SetMinVersionAllowed )( 
            ILicenseHandler * This,
            /* [in] */ DOUBLE minVersionAllowed,
            /* [retval][out] */ BSTR *resultAsString);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *Authenticate )( 
            ILicenseHandler * This,
            /* [in] */ INT challenge,
            /* [retval][out] */ INT *response);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *Activate )( 
            ILicenseHandler * This,
            /* [in] */ INT secretKey);
        
        END_INTERFACE
    } ILicenseHandlerVtbl;

    interface ILicenseHandler
    {
        CONST_VTBL struct ILicenseHandlerVtbl *lpVtbl;
    };

    

#ifdef COBJMACROS


#define ILicenseHandler_QueryInterface(This,riid,ppvObject)	\
    (This)->lpVtbl -> QueryInterface(This,riid,ppvObject)

#define ILicenseHandler_AddRef(This)	\
    (This)->lpVtbl -> AddRef(This)

#define ILicenseHandler_Release(This)	\
    (This)->lpVtbl -> Release(This)


#define ILicenseHandler_GetTypeInfoCount(This,pctinfo)	\
    (This)->lpVtbl -> GetTypeInfoCount(This,pctinfo)

#define ILicenseHandler_GetTypeInfo(This,iTInfo,lcid,ppTInfo)	\
    (This)->lpVtbl -> GetTypeInfo(This,iTInfo,lcid,ppTInfo)

#define ILicenseHandler_GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId)	\
    (This)->lpVtbl -> GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId)

#define ILicenseHandler_Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr)	\
    (This)->lpVtbl -> Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr)


#define ILicenseHandler_get_status(This,pVal)	\
    (This)->lpVtbl -> get_status(This,pVal)

#define ILicenseHandler_CheckLicense(This,productName,productVersion,resultAsString)	\
    (This)->lpVtbl -> CheckLicense(This,productName,productVersion,resultAsString)

#define ILicenseHandler_CreateLicFile(This,productName,productVersion,statusAsString)	\
    (This)->lpVtbl -> CreateLicFile(This,productName,productVersion,statusAsString)

#define ILicenseHandler_AllowAnyValidLicenseVersion(This)	\
    (This)->lpVtbl -> AllowAnyValidLicenseVersion(This)

#define ILicenseHandler_QueryForEmail(This)	\
    (This)->lpVtbl -> QueryForEmail(This)

#define ILicenseHandler_SetRegistrationComment(This,iregistrationComment)	\
    (This)->lpVtbl -> SetRegistrationComment(This,iregistrationComment)

#define ILicenseHandler_CRCCheckSelf(This,fullPathToApplication,fullPathToNonAppendedCRC,resultAsString)	\
    (This)->lpVtbl -> CRCCheckSelf(This,fullPathToApplication,fullPathToNonAppendedCRC,resultAsString)

#define ILicenseHandler_SetMinVersionAllowed(This,minVersionAllowed,resultAsString)	\
    (This)->lpVtbl -> SetMinVersionAllowed(This,minVersionAllowed,resultAsString)

#define ILicenseHandler_Authenticate(This,challenge,response)	\
    (This)->lpVtbl -> Authenticate(This,challenge,response)

#define ILicenseHandler_Activate(This,secretKey)	\
    (This)->lpVtbl -> Activate(This,secretKey)

#endif /* COBJMACROS */


#endif 	/* C style interface */



/* [helpstring][id][propget] */ HRESULT STDMETHODCALLTYPE ILicenseHandler_get_status_Proxy( 
    ILicenseHandler * This,
    /* [retval][out] */ LONG *pVal);


void __RPC_STUB ILicenseHandler_get_status_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id] */ HRESULT STDMETHODCALLTYPE ILicenseHandler_CheckLicense_Proxy( 
    ILicenseHandler * This,
    /* [in] */ BSTR productName,
    /* [in] */ BSTR productVersion,
    /* [retval][out] */ BSTR *resultAsString);


void __RPC_STUB ILicenseHandler_CheckLicense_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id] */ HRESULT STDMETHODCALLTYPE ILicenseHandler_CreateLicFile_Proxy( 
    ILicenseHandler * This,
    /* [in] */ BSTR productName,
    /* [in] */ BSTR productVersion,
    /* [retval][out] */ BSTR *statusAsString);


void __RPC_STUB ILicenseHandler_CreateLicFile_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id] */ HRESULT STDMETHODCALLTYPE ILicenseHandler_AllowAnyValidLicenseVersion_Proxy( 
    ILicenseHandler * This);


void __RPC_STUB ILicenseHandler_AllowAnyValidLicenseVersion_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id] */ HRESULT STDMETHODCALLTYPE ILicenseHandler_QueryForEmail_Proxy( 
    ILicenseHandler * This);


void __RPC_STUB ILicenseHandler_QueryForEmail_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id] */ HRESULT STDMETHODCALLTYPE ILicenseHandler_SetRegistrationComment_Proxy( 
    ILicenseHandler * This,
    /* [in] */ BSTR iregistrationComment);


void __RPC_STUB ILicenseHandler_SetRegistrationComment_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id] */ HRESULT STDMETHODCALLTYPE ILicenseHandler_CRCCheckSelf_Proxy( 
    ILicenseHandler * This,
    /* [in] */ BSTR fullPathToApplication,
    /* [in] */ BSTR fullPathToNonAppendedCRC,
    /* [retval][out] */ BSTR *resultAsString);


void __RPC_STUB ILicenseHandler_CRCCheckSelf_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id] */ HRESULT STDMETHODCALLTYPE ILicenseHandler_SetMinVersionAllowed_Proxy( 
    ILicenseHandler * This,
    /* [in] */ DOUBLE minVersionAllowed,
    /* [retval][out] */ BSTR *resultAsString);


void __RPC_STUB ILicenseHandler_SetMinVersionAllowed_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id] */ HRESULT STDMETHODCALLTYPE ILicenseHandler_Authenticate_Proxy( 
    ILicenseHandler * This,
    /* [in] */ INT challenge,
    /* [retval][out] */ INT *response);


void __RPC_STUB ILicenseHandler_Authenticate_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id] */ HRESULT STDMETHODCALLTYPE ILicenseHandler_Activate_Proxy( 
    ILicenseHandler * This,
    /* [in] */ INT secretKey);


void __RPC_STUB ILicenseHandler_Activate_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);



#endif 	/* __ILicenseHandler_INTERFACE_DEFINED__ */



#ifndef __LicenseManagerLib_LIBRARY_DEFINED__
#define __LicenseManagerLib_LIBRARY_DEFINED__

/* library LicenseManagerLib */
/* [helpstring][version][uuid] */ 


EXTERN_C const IID LIBID_LicenseManagerLib;

EXTERN_C const CLSID CLSID_LicenseHandler;

#ifdef __cplusplus

class DECLSPEC_UUID("08DE6FB7-8068-457f-BD7F-C63F647177D9")
LicenseHandler;
#endif
#endif /* __LicenseManagerLib_LIBRARY_DEFINED__ */

/* Additional Prototypes for ALL interfaces */

unsigned long             __RPC_USER  BSTR_UserSize(     unsigned long *, unsigned long            , BSTR * ); 
unsigned char * __RPC_USER  BSTR_UserMarshal(  unsigned long *, unsigned char *, BSTR * ); 
unsigned char * __RPC_USER  BSTR_UserUnmarshal(unsigned long *, unsigned char *, BSTR * ); 
void                      __RPC_USER  BSTR_UserFree(     unsigned long *, BSTR * ); 

/* end of Additional Prototypes */

#ifdef __cplusplus
}
#endif

#endif


