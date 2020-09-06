#ifndef __HSUPCHK_H__
#define __HSUPCHK_H__


#define HACKSHIELD_ERROR_SUCCESS				( 0x00000000 )
#define HACKSHIELD_ERROR						( 0x30000000 )

#define HSERROR_ENVFILE_NOTREAD					( HACKSHIELD_ERROR + 0x10 )
#define HSERROR_ENVFILE_NOTWRITE				( HACKSHIELD_ERROR + 0x20 )
#define HSERROR_NETWORK_CONNECT_FAIL			( HACKSHIELD_ERROR + 0x30 )
#define HSERROR_INVALID_PRODUCT_NUMBER			( HACKSHIELD_ERROR + 0x40 )
#define HSERROR_LIB_NOTEDIT_REG					( HACKSHIELD_ERROR + 0x50 )
#define HSERROR_NOTFINDFILE						( HACKSHIELD_ERROR + 0x60 )

#define HSERROR_PROTECT_LISTLOAD_FAIL			( HACKSHIELD_ERROR + 0x70 )
#define HSERROR_PROTECT_VERITY_FAIL				( HACKSHIELD_ERROR + 0x80 )
#define HSERROR_HSUPDATE_TIMEOUT				( HACKSHIELD_ERROR + 0x90 )

#define HSERROR_INVALID_PARAMETER				( HACKSHIELD_ERROR + 0xA0 )
#define HSERROR_STRING_CONVERSION_FAILED		( HACKSHIELD_ERROR + 0xB0 )

#define HSERROR_MISMATCH_ENVFILE				( HACKSHIELD_ERROR + 0xC0 )
#define HSERROR_HOSTFILE_MODIFICATION			( HACKSHIELD_ERROR + 0xD0 )

#define HSERROR_AUTOUPDATE_FAIL					( HACKSHIELD_ERROR + 0xE0 )
				
// Option Parameter AhnHSUpdateEx 
#define  AHNHSUPDATE_CHKOPT_HOSTFILE			0x2
#define  AHNHSUPDATE_CHKOPT_GAMECODE			0x4


// Export API
#if defined(__cplusplus)
extern "C"
{
#endif

DWORD
 __stdcall
 _AhnHS_HSUpdateA ( 
	LPCSTR	szUpdateDir, 
	DWORD	dwTimeOut, 
	DWORD	dwTimeOutPerConnection = 0
	);

DWORD
 __stdcall
 _AhnHS_HSUpdateW ( 
	LPCWSTR	szUpdateDir, 
	DWORD	dwTimeOut,
	DWORD	dwTimeOutPerConnection = 0
	);
#if defined(UNICODE) | defined(_UNICODE)
	#define	 _AhnHS_HSUpdate	 _AhnHS_HSUpdateW
#else
	#define  _AhnHS_HSUpdate	 _AhnHS_HSUpdateA
#endif // !UNICODE


DWORD
 __stdcall
 _AhnHS_HSUpdateExA ( 
	IN LPCSTR	szUpdateDirA,
	IN DWORD	dwTimeOut, 
	IN INT64	i64GameCode,
	IN DWORD    dwOption,
	IN DWORD	dwTimeOutPerConnection = 0
	);
	

DWORD
 __stdcall
 _AhnHS_HSUpdateExW ( 
	IN LPCWSTR	szUpdateDirW, 
	IN DWORD	dwTimeOut, 
	IN INT64	i64GameCode,
	IN DWORD    dwOption,
	IN DWORD	dwTimeOutPerConnection = 0
	);
#if defined(UNICODE) | defined(_UNICODE)
	#define	 _AhnHS_HSUpdateEx	 _AhnHS_HSUpdateExW
#else
	#define  _AhnHS_HSUpdateEx	 _AhnHS_HSUpdateExA
#endif // !UNICODE




#if defined(__cplusplus)
}
#endif

#endif __HSUPCHK_H__
