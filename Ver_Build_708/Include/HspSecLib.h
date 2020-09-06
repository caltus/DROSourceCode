/**
 @File	HspSecLib.h
 @brief	HspSecLib 헤더파일.

 (C) Copyright 2005. Ahn Lab, Inc.\n
 Any part of this source code can not be copied with\n
 any method without prior written permission from\n
 the author or authorized person.\n
*/

#ifndef _HSPSECLIB_H
#define _HSPSECLIB_H

// HShield Pro SecLib Version Info
#define	HSPRO_VERSION(w,x,y,z)	        (w) <<24|(x&0xFF)<<16|(y&0xFF)<<8|(z&0xFF)
#define HSHIELDPRO_SECLIB_VER		    HSPRO_VERSION(1,0,0,2)

// 에러코드 정의.
#define ERROR_HSPSECLIB_BASECODE							    0x0004A000
#define ERROR_HSPSECLIB_MAKESESSIONKEY_INVALIDPARAM				ERROR_HSPSECLIB_BASECODE + 0x01
#define ERROR_HSPSECLIB_INITCRYPT_INVALIDPARAM					ERROR_HSPSECLIB_BASECODE + 0x02
#define ERROR_HSPSECLIB_GETENCMSG_INVALIDPARAM					ERROR_HSPSECLIB_BASECODE + 0x03
#define ERROR_HSPSECLIB_GETDECMSG_INVALIDPARAM					ERROR_HSPSECLIB_BASECODE + 0x04

#define ERROR_HSPSECLIB_ENCFILE_INVALIDPARAM					ERROR_HSPSECLIB_BASECODE + 0x05
#define ERROR_HSPSECLIB_ENCFILE_FOPEN							ERROR_HSPSECLIB_BASECODE + 0x06
#define ERROR_HSPSECLIB_ENCFILE_GETFILELEN						ERROR_HSPSECLIB_BASECODE + 0x07
#define ERROR_HSPSECLIB_ENCFILE_SIZEZERO						ERROR_HSPSECLIB_BASECODE + 0x08
#define ERROR_HSPSECLIB_ENCFILE_INITCRYPT						ERROR_HSPSECLIB_BASECODE + 0x09
#define ERROR_HSPSECLIB_ENCFILE_COPYFILE						ERROR_HSPSECLIB_BASECODE + 0x0A
#define ERROR_HSPSECLIB_ENCFILE_ENCFILE							ERROR_HSPSECLIB_BASECODE + 0x0B
#define ERROR_HSPSECLIB_ENCFILE_FWRITE							ERROR_HSPSECLIB_BASECODE + 0x0C

#define ERROR_HSPSECLIB_FILEENC_INVALIDPARAM					ERROR_HSPSECLIB_BASECODE + 0x10
#define ERROR_HSPSECLIB_FILEENC_FREAD							ERROR_HSPSECLIB_BASECODE + 0x11
#define ERROR_HSPSECLIB_FILEENC_GETENCMSG						ERROR_HSPSECLIB_BASECODE + 0x12

#define ERROR_HSPSECLIB_FREAD_INVALIDPARAM						ERROR_HSPSECLIB_BASECODE + 0x13
#define ERROR_HSPSECLIB_FREAD_GETFILELEN						ERROR_HSPSECLIB_BASECODE + 0x14
#define ERROR_HSPSECLIB_FREAD_SIZEZERO							ERROR_HSPSECLIB_BASECODE + 0x15
#define ERROR_HSPSECLIB_FREAD_GETPOSITION						ERROR_HSPSECLIB_BASECODE + 0x16
#define ERROR_HSPSECLIB_FREAD_RANGE								ERROR_HSPSECLIB_BASECODE + 0x17
#define ERROR_HSPSECLIB_FREAD_FREAD								ERROR_HSPSECLIB_BASECODE + 0x18
#define ERROR_HSPSECLIB_FREAD_FSEEK								ERROR_HSPSECLIB_BASECODE + 0x19
#define ERROR_HSPSECLIB_FREAD_GETDECMSG							ERROR_HSPSECLIB_BASECODE + 0x1A

#define ERROR_HSPSECLIB_DECFILE_INVALIDPARAM					ERROR_HSPSECLIB_BASECODE + 0x20
#define ERROR_HSPSECLIB_DECFILE_FOPEN							ERROR_HSPSECLIB_BASECODE + 0x21
#define ERROR_HSPSECLIB_DECFILE_FILESEEK						ERROR_HSPSECLIB_BASECODE + 0x22
#define ERROR_HSPSECLIB_DECFILE_INITCRYPT						ERROR_HSPSECLIB_BASECODE + 0x23
#define ERROR_HSPSECLIB_DECFILE_FWRITE							ERROR_HSPSECLIB_BASECODE + 0x24
#define ERROR_HSPSECLIB_DECFILE_GETFILELEN						ERROR_HSPSECLIB_BASECODE + 0x25
#define ERROR_HSPSECLIB_DECFILE_SIZEZERO						ERROR_HSPSECLIB_BASECODE + 0x26

#define ERROR_HSPSECLIB_FILEDEC_INVALIDPARAM					ERROR_HSPSECLIB_BASECODE + 0x30
#define ERROR_HSPSECLIB_FILEDEC_FREAD							ERROR_HSPSECLIB_BASECODE + 0x31
#define ERROR_HSPSECLIB_FILEDEC_GETDECMSG						ERROR_HSPSECLIB_BASECODE + 0x32

#define ERROR_HSPSECLIB_GETFILEHASH_INVALIDPARAM				ERROR_HSPSECLIB_BASECODE + 0x40
#define ERROR_HSPSECLIB_GETFILEHASH_CREATEFILE					ERROR_HSPSECLIB_BASECODE + 0x41
#define ERROR_HSPSECLIB_GETFILEHASH_CREATEFILEMAP				ERROR_HSPSECLIB_BASECODE + 0x42
#define ERROR_HSPSECLIB_GETFILEHASH_GETFILESIZE					ERROR_HSPSECLIB_BASECODE + 0x43
#define ERROR_HSPSECLIB_GETFILEHASH_MAPVIEWFILE					ERROR_HSPSECLIB_BASECODE + 0x44


#include "../../Inc/CryptAlgo/AES.h"


#if defined(__cplusplus)
extern "C"
{
#endif

/*!
 * 
 * @remarks	주어진 크기에 해당하는 랜덤시드를 생성한다.
 *
 *
 * @param	nRndSize : [IN]시드 사이즈
 * @param	pbyRndSeed : [OUT]랜덤시드
 *
 * @retval	없음.
 *
 * @attention	
 *
 */
void __stdcall GetRandSeed ( IN int nRndSize,         
				             OUT PBYTE pbyRndSeed );

/*!
 * 
 * @remarks	세션생성시 사용할 키 생성 기능 수행
 *
 *
 * @param	pbyRandomSeed : [IN]세션키 생성을 위한 랜덤한 시드값 					
 * @param	pbyRndSeed : [OUT]세션키
 *
 * @retval	ERROR_SUCCESS : 성공
 * @retval	ERROR_MAKESESSIONKEY_INVALIDPARAM : 잘못된 파라메터 입력
 * @retval	그밖의 Win32 Error Codes : 실패시
 *
 * @attention	
 *
 */
DWORD __stdcall MakeFileKey ( PBYTE pbyRandomSeed,					
						      PBYTE pbySessionKey );	   



/*!
 * 
 * @remarks	세션생성시 사용할 키 생성 기능 수행
 *
 * @param	pbyFirstSeed : [IN]세션키 생성을 위한 첫번째 시드값 					
 * @param	pbySecondSeed : [IN]세션키 생성을 위한 두번째 시드값 					
 * @param	pbyRndSeed : [OUT]세션키
 *
 * @retval	ERROR_SUCCESS : 성공
 * @retval	ERROR_HSPSECLIB_MAKESESSIONKEY_INVALIDPARAM : 잘못된 파라메터 입력
 *
 * @attention	
 *
 * @sa		
 *
 */
ULONG __stdcall MakeSessionKey ( IN PBYTE pbyFirstSeed,					
								 IN PBYTE pbySecondSeed,				
								 OUT PBYTE pbySessionKey );



/*!
 * 
 * @remarks	암/복호화를 위한 초기화 기능 수행\n
	         =>Key를 입력받아 암복호화를 위한 AES_KEY를 생성한다.
 *
 * @param	pbyPwd : [IN]패스워드 					
 * @param	nKeySize : [IN]패스워드 사이즈
 * @param	pAesEncKey : [OUT]암호화키
 * @param	pAesDecKey : [OUT]복호화키
 *
 * @retval	ERROR_SUCCESS : 성공
 * @retval	ERROR_HSPSECLIB_INITCRYPT_INVALIDPARAM : 잘못된 파라메터 입력
 *
 * @attention	
 *
 * @sa		
 *
 */

DWORD __stdcall InitCrypt ( PBYTE pbyPwd,				
							int nKeySize,				
							AES_KEY *pAesEncKey,		
							AES_KEY *pAesDecKey );

/*!
 * 
 * @remarks	AES128을 이용하여 암호화 기능 수행\n
	         =>입력된 버퍼를 암호화하여 버퍼로 출력한다.
 *
 * @param	pbyInput : [IN]암호화할 버퍼 					
 * @param	nInLength : [IN]암호화할 버퍼사이즈
 * @param	*pAesEncKey : [IN]암호화키
 * @param	pbyOutput : [OUT]암호화된 버퍼
 *
 * @retval	ERROR_SUCCESS : 성공
 * @retval	ERROR_HSPSECLIB_GETENCMSG_INVALIDPARAM : 잘못된 파라메터 입력
 *
 * @attention	
 *
 * @sa		
 *
 */
DWORD __stdcall GetEncMsg ( PBYTE pbyInput,		
						    int nInLength,		
						    AES_KEY *pAesEncKey,	
						    PBYTE pbyOutput );

/*!
 * 
 * @remarks	AES128을 이용하여 복호화 기능 수행\n
	         =>입력된 버퍼를 복호화하여 버퍼로 출력한다.
 *
 * @param	pbyInput : [IN]복호화할 버퍼 					
 * @param	nInLength : [IN]복호화할 버퍼사이즈
 * @param	*pAesEncKey : [IN]복호화키
 * @param	pbyOutput : [OUT]복호화된 버퍼
 *
 * @retval	ERROR_SUCCESS : 성공
 * @retval	ERROR_HSPSECLIB_GETDECMSG_INVALIDPARAM : 잘못된 파라메터 입력
 *
 * @attention	
 *
 * @sa		
 *
 */
DWORD __stdcall GetDecMsg ( PBYTE pbyInput,				
						    int nInLength,				
						    AES_KEY *pAesDecKey,			
						    PBYTE pbyOutput );
	
/*!
 * 
 * @remarks	파일 암호화.
 *
 * @param	lpszInputFile : [IN]암호화할 파일경로 					
 * @param	pbyInitKey : [IN] 초기화 암호키
 * @param	lpszOutputFile : [OUT]원본을 백업한 파일경로
 *
 * @retval	ERROR_SUCCESS : 성공
 * @retval	ERROR_HSPSECLIB_ENCFILE_INVALIDPARAM : 잘못된 파라메터 입력
 * @retval	ERROR_HSPSECLIB_ENCFILE_FOPEN : fopen실패.
 * @retval	ERROR_HSPSECLIB_ENCFILE_GETFILELEN : 파일 사이즈를 구하기 실패.
 * @retval	ERROR_HSPSECLIB_ENCFILE_SIZEZERO : 파일 사이즈 0.
 * @retval	ERROR_HSPSECLIB_ENCFILE_INITCRYPT : 암복호화 초기화 실패.
 * @retval	ERROR_HSPSECLIB_ENCFILE_COPYFILE : 파일 복사 실패.
 * @retval	ERROR_HSPSECLIB_ENCFILE_ENCFILE : 암호화 실패.
 * @retval	ERROR_HSPSECLIB_ENCFILE_FWRITE : 파일쓰기 실패.
 * @retval	그밖의 Win32 Error Codes : 실패시
 *
 * @attention	
 *
 */
DWORD __stdcall GetEncFile ( IN LPCSTR lpszInputFile,			
						     IN PBYTE pbyInitKey,
							 OUT LPSTR lpszOutputFile );


/*!
 * 
 * @remarks	파일 복호화.
 *
 * @param	lpszInputFile : [IN]복호화할 파일경로 					
 * @param	pbyInitKey : [IN] 초기화 암호키
 * @param	lpszOutputFile : [OUT]복호화된 파일경로
 *
 * @retval	ERROR_SUCCESS : 성공
 * @retval	ERROR_HSPSECLIB_DECFILE_INVALIDPARAM : 잘못된 파라메터 입력
 * @retval	ERROR_HSPSECLIB_DECFILE_FOPEN : fopen실패.
 * @retval	ERROR_HSPSECLIB_DECFILE_GETFILELEN : 파일 사이즈를 구하기 실패.
 * @retval	ERROR_HSPSECLIB_DECFILE_SIZEZERO : 파일 사이즈 0.
 * @retval	ERROR_HSPSECLIB_DECFILE_INITCRYPT : 복호화 초기화 실패.
 * @retval	ERROR_HSPSECLIB_DECFILE_FWRITE : 파일쓰기 실패.
 * @retval	그밖의 Win32 Error Codes : 실패시
 *
 * @attention	
 *
 */
DWORD __stdcall GetDecFile( IN LPCSTR lpszInputFile,			
						    IN PBYTE pbyInitKey,
							OUT LPSTR lpszOutputFile);

/*!
 * 
 * @remarks	파일구조체 포인터를 이용하여 원하는 블럭만 복호화후 버퍼로 출력.
 *
 * @param	lpOutBuffer : [OUT]복호화된 버퍼 					
 * @param	dwDecryptSize : [IN]읽을 사이즈
 * @param	pInputStream : [IN]읽을 파일포인터
 * @param	pAesDecKey : [IN]복호화 키
 * @param	pdwReadLen : [OUT]복호화된 사이즈
 *
 * @retval	ERROR_SUCCESS : 성공
 * @retval	ERROR_HSPSECLIB_FREAD_INVALIDPARAM : 잘못된 파라메터 입력
 * @retval	ERROR_HSPSECLIB_FREAD_GETFILELEN : 파일크기 구하기 실패.
 * @retval	ERROR_HSPSECLIB_FREAD_SIZEZERO : 파일크기 0.
 * @retval	ERROR_HSPSECLIB_FREAD_GETPOSITION : 현재 파일포인터 구하기 실패.
 * @retval	ERROR_HSPSECLIB_FREAD_RANGE :복호화할 블럭사이즈가 파일보다 크다.
 * @retval	ERROR_HSPSECLIB_FREAD_FSEEK : AhnHs_FSeek 실패.
 * @retval	ERROR_HSPSECLIB_FREAD_FREAD : fread 실패.
 * @retval	ERROR_HSPSECLIB_FREAD_GETDECMSG : 복호화 실패.
 * @retval	그밖의 Win32 Error Codes : 실패시
 *
 * @attention	
 *
 */
DWORD __stdcall AhnHs_FRead ( OUT LPVOID lpOutBuffer,			
								 IN DWORD dwDecryptSize,			
								 IN FILE *pInputStream,				
								 IN AES_KEY *pAesDecKey,			
								 OUT PDWORD pdwReadLen );



/*!
 * 
 * @remarks	파일의 해쉬를 구한다.
 *
 * @param	lpszFilePath : [IN]해쉬를 구할 파일경로 					
 * @param	pbyHash : [OUT]해쉬값
 *
 * @retval	ERROR_SUCCESS : 성공
 * @retval	ERROR_HSPSECLIB_GETFILEHASH_INVALIDPARAM : 잘못된 파라메터 입력
 * @retval	ERROR_HSPSECLIB_GETFILEHASH_CREATEFILE : 파일생성 실패.
 * @retval	ERROR_HSPSECLIB_GETFILEHASH_CREATEFILEMAP : 오픈한 File에 대한 Memory Map을 생성 실패.
 * @retval	ERROR_HSPSECLIB_GETFILEHASH_GETFILESIZE : 전체 파일 사이즈 구하기 실패.
 * @retval	ERROR_HSPSECLIB_GETFILEHASH_MAPVIEWFILE : 메모리맵에대한 뷰생성 실패.
 * @retval	그밖의 Win32 Error Codes : 실패시
 *
 * @attention	
 *
 * @sa		
 *
 */
DWORD GetFileHash( IN LPCSTR lpszFilePath,		
				   OUT PBYTE pbyHash );

#if defined(__cplusplus)
}
#endif //(__cplusplus)

#endif //_HSPSECLIB_H
