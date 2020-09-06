#include <WinSock2.h>
#include <Windows.h>
#include <stdio.h>
#include <tchar.h>

#include "Minia_Server_Op.h"
#include "NetSvMngr.h"
#include "AntiCpSvrFunc.h"

DWORD m_dwCheckInterval;
BOOL m_bUseAntiCpX;
int m_nFirstCheckCrcOpt;
int m_nNextCheckCrcOpt;
AHNHS_SERVER_HANDLE	m_hServer;

//
/*******************************************************************************
*	Receive한 데이터의 헤더정보를 이용하여 어떤 작업을 원하는지 확인한 후
*	적절한 작업을 수행한다.

* Parameters: 
*   pIoData : Client로부터 전달받은 데이터
*	pHandleData : Socket Handle과 관련된 데이터
* 
* Return: 
*	NONE
*******************************************************************************/

DWORD WINAPI JobProcessing (IN PIO_DATA pIoData, 	IN PHANDLE_DATA pHandleData)
{
	DWORD dwRet = ERROR_SUCCESS;

	switch ( pIoData->msgHeader.opCode )
	{
	// 클라이언트 로그인 
	case OP_LOGON:
		dwRet = Op_ClientLogon ( pIoData, pHandleData);
		break;
	// 클라이언트 로그오프
	case OP_LOGOFF:
		Op_ClientLogoff ( pIoData, pHandleData );
		break;	
	// 클라이언트 ACK-GUID 메세지
	case OP_ACKGUID:
		{
			// [확장 서버연동] 확장 서버연동은, GuidAck 와 CrcAck 의 구별이 없음.
			if ( m_bUseAntiCpX )
				dwRet = Op_CrcAck ( pIoData, pHandleData);
			// [구 서버연동]
			else
				dwRet = Op_GuidAck ( pIoData, pHandleData );			
		}		
		break;
	// 클라이언트 ACK-CRC 메세지 
	case OP_ACKCRC:
		dwRet = Op_CrcAck ( pIoData, pHandleData);
		break;

	// ※주의: 클라이언트로부 일정시간안에 응답메세지를 수신하지 못하는 경우,
	//		   해당 클라이언트 접속 세션을 종료해야한다.

	default:
		break;
	}

	return dwRet;
}

// 클라이언트의 GUID를 얻어와야 한다.
DWORD Op_ClientLogon (IN PIO_DATA pIoData, IN PHANDLE_DATA pHandleData)
{
	BYTE byGuidReqMsg[SIZEOF_GUIDREQMSG] = { 0, };	// [구 서버연동] 클라이언트에 전달하게 될 변수
	AHNHS_TRANS_BUFFER tbRequest;					// [확장 서버연동] 클라이언트에 전달하게 될 변수
	DWORD dwRet = ERROR_SUCCESS;
	MSGHEADER header = { 0, 0 };
	
	// [확장 서버연동]
	if ( m_bUseAntiCpX )
	{
		// ------------------------------------------------------------------------
		// [_AhnHS_CreateClientObject ]
		// ------------------------------------------------------------------------
		//  - DESCRIPTION:
		//    서버 핸들을 입력 받아 클라이언트 핸들을 생성합니다. 클라이언트 핸들은 
		//    클라이언트가 접속할 때 마다 생성하며, 세션이 유지되는 동안 핸들을 닫지 않고 재사용합니다.
		//
		//  - SYNTAX:
		//    AHNHS_CLIENT_HANDLE _AhnHS_CreateClientObject (IN AHNHS_SERVER_HANDLE hServer);
		//
		//  - PARAMETERS:
		//    hServer	  :AHNHS_SERVER_HANDLE	서버 핸들(Server Handle) 
		//
		//  - RETURN VALUE
		//    클라이언트 핸들(Client Handle)

		pHandleData->hClient = _AhnHS_CreateClientObject ( m_hServer );

		if ( pHandleData->hClient == ANTICPX_INVALID_HANDLE_VALUE )
		{
			dwRet = ERROR_INVALID_HANDLE;
		}
		else 
		{
			// -----------------------------------------------------------
			// [_AhnHS_MakeRequest ]
			// -----------------------------------------------------------
			// - DESCRIPTION:
			//    현재 세션에 맞는 클라이언트 핸들을 입력하여 요청 메시지를 생성합니다. 
			//    요청 메시지는 AHNHS_TRANS_BUFFER 구조체 형태로 출력되며, 
			//    멤버 변수 값은 다음과 같습니다.
			//    typedef struct _AHNHS_TRANS_BUFFER
			//    {
			//      	unsigned short nLength;			// 요청 메시지 생성에 사용된 버퍼 길이
			//          unsigned char byBuffer[ANTICPX_TRANS_BUFFER_MAX]; // 요청 메시지 생성에 사용될 수 있는 최대 바이트 버퍼
			//    } AHNHS_TRANS_BUFFER, *PAHNHS_TRANS_BUFFER;
			//
			//  ※주의:
			//    byBuffer는 요청 메시지 생성에 사용될 수 있는 최대 버퍼 크기이므로 네트워크로 전송할 때 nLength 
			//    만큼만 전송해야 합니다.
			//
			// - SYNTAX:
			//   unsigned long __stdcall _AhnHS_MakeRequest (IN AHNHS_CLIENT_HANDLE hClient, 
			//												OUT PAHNHS_TRANS_BUFFER pRequestBuffer);
			// - PARAMETERS:
			//   .hClient			:AHNHS_CLIENT_HANDLE	클라이언트 핸들
			//   .pRequestBuffer	:PAHNHS_TRANS_BUFFER	보낼 데이터 버퍼/길이
			// 
			// - RETURN VALUE:
			//   . ERROR_SUCCESS:
			//	   함수 호출을 성공했을 때 리턴하는 값입니다. (Value = 0x00000000)
			//   . 기타				: 기타 리턴값에 대해서는 핵쉴드 프로그래밍 가이드 참고바람.


			dwRet = _AhnHS_MakeRequest( pHandleData->hClient, &tbRequest );

			TCHAR szMsg[256]={0,};
			_stprintf(szMsg, "##_AhnHS_MakeRequest #1 result:%x\r\n", dwRet);
			OutputDebugString(szMsg);
		}
	}
	// [구 서버연동]
	else
	{
		// ----------------------------------------------------
		// [_AntiCpSvr_MakeGuidReqMsg]
		// ----------------------------------------------------
		// - DESCRIPTION:
		//   클라이언트로 전달할 암호화된 버전 요구 메시지를 생성합니다.
		//
		// - SYNTAX:
		//	unsigned long __stdcall _AntiCpSvr_MakeGuidReqMsg (	OUT unsigned char *pbyGuidReqMsg,
		//														OUT unsigned char *pbyGuidReqInfo)
		// - PARAMETERS:
		//   . pbyGuidReqMsg	unsigned char *				: 클라이언트로 전송할 암호화된 버전 요구 메시지로 버퍼의 크기는 
		//													  AntiCpSvrFunc.h에 정의된  SIZEOF_GUIDREQMSG이어야 합니다. 
		//   . pbyGuidReqInfo	unsigned char *				: _AntiCpSvr_AnalyzeGuidAckMsg 함수에서 분석할 때 필요한 버전 
		//												      요구 메시지 정보로 버퍼의 크기는 AntiCpSvrFunc.h에 정의된
		//												      SIZEOF_GUIDREQINFO이어야 합니다.
		// - RETURN VALUE:
		//   . ERROR_SUCCESS								: 함수 호출을 성공했을 때 리턴하는 값입니다. (Value = 0x00000000)
		//   . 기타											: 기타 리턴값에 대해서는 핵쉴드 프로그래밍 가이드 참고바람.

		dwRet = _AntiCpSvr_MakeGuidReqMsg ( byGuidReqMsg, pHandleData->byClientInfo );
	}	
	
	if ( dwRet != ERROR_SUCCESS )
	{
		TCHAR szMsg[MAX_PATH] = { 0, };

		_stprintf ( szMsg, TEXT ( "Minia_Server : _AntiCpSvr_MakeGuidReqMsg Fail ( 0x%X ) IP : %d.%d.%d.%d\t%d\n" ),
					dwRet,
					pHandleData->sockaddr.sin_addr.S_un.S_un_b.s_b1,
					pHandleData->sockaddr.sin_addr.S_un.S_un_b.s_b2,
					pHandleData->sockaddr.sin_addr.S_un.S_un_b.s_b3,
					pHandleData->sockaddr.sin_addr.S_un.S_un_b.s_b4,
					ntohs ( pHandleData->sockaddr.sin_port ) );

		OutputDebugString ( szMsg );

		header.opCode = OP_ERROR;
		header.length = htonl ( dwRet );

		// 
		IoWrite ( pHandleData, &header, NULL, 0 );
	}
	else
	{
		// [확장 서버연동]	tbRequest를 클라이언트에 송신함.
		if ( m_bUseAntiCpX )
		{
			header.opCode = OP_REQGUID;
			header.length = htonl ( tbRequest.nLength );

			IoWrite ( pHandleData, &header, tbRequest.byBuffer, tbRequest.nLength );
		}
		// [구 서버연동] byGuidReqMsg를 클라이언트 송신함.
		else
		{
			header.opCode = OP_REQGUID;
			header.length = htonl ( SIZEOF_GUIDREQMSG );

			IoWrite ( pHandleData, &header, byGuidReqMsg, SIZEOF_GUIDREQMSG );
		}
	}

	return dwRet;
}


VOID
Op_ClientLogoff (
	IN PIO_DATA pIoData,
	IN PHANDLE_DATA pHandleData
	)
{

	pIoData;

	// [확장 서버연동]
	if ( m_bUseAntiCpX == TRUE )
	{
		// ----------------------------------------------------------
		// [_AhnHS_CloseClientHandle]
		// ----------------------------------------------------------
		// - DESCRIPTION:
		//   생성한 클라이언트 핸들은 클라이언트 세션이 종료될 때 해제되어야 합니다. 
		//   이때 클라이언트 핸들 생성에 사용되었던 메모리나 시스템 자원을 해제하게 됩니다.
		//
		// - SYNTAX:
		//	 void __stdcall_AhnHS_CloseClientHandle (IN AHNHS_CLIENT_HANDLE hClient)
		//
		// - PARAMETERS:
		//   hClient	:AHNHS_CLIENT_HANDLE	클라이언트 핸들(Client Handle)
		//
		// - RETURN VALUE:
		//   없음.
		//
		// - ※주의 : 클라이언트 핸들이 모두 해제된 이후 서버핸들이 해제되어야 한다 

		_AhnHS_CloseClientHandle ( pHandleData->hClient );
		pHandleData->hClient = ANTICPX_INVALID_HANDLE_VALUE;
	}

	ShutDownSocket ( pIoData, pHandleData );
}


// [구 서버연동]
DWORD
Op_GuidAck (
	IN PIO_DATA pIoData,
	IN PHANDLE_DATA pHandleData
	)
{
	BYTE byReqMsg[SIZEOF_REQMSG] = { 0, };
	DWORD dwRet = ERROR_SUCCESS;
	MSGHEADER header = { 0, 0 };

	// [구 서버연동]
	// -----------------------------------------------------------
	// [_AntiCpSvr_AnalyzeGuidAckMsg]
	// ------------------------------------------------------------
	// - DESCRIPTION:
	//   클라이언트에서 전달한 암호화된 버전 응답 메시지를 복호화하여 
	//   현재 사용하고 있는 클라이언트 버전이 서버에서 허용하는 버전인지 확인합니다.
	//
	// - SYNTAX:
	//	 unsigned long __stdcall _AntiCpSvr_AnalyzeGuidAckMsg (	IN unsigned char *pbyGuidAckMsg,
	//															IN unsigned char *pbyGuidReqInfo,
	//															OUT unsigned long **ppCrcInfo)
	// - PARAMETERS:
	//	.pbyAckMsg	unsigned char *							:클라이언트에서 보낸 암호화된 버전 응답 메시지
	//	.pbyGuidReqInfo	unsigned char *						:_AntiCpSvr_MakeGuidReqMsg 함수에서 생성한 버전 
	//														 요구 메시지에 대한 정보
	//	.ppCrcInfo	Unsigned long **						:해당 클라이언트가 사용할 CRC 정보에 대한 메모리 
	//														 포인터를 가지고 있을 버퍼
	//
	// - RETURN VALUE:
	//	 .ERROR_SUCCESS:
	//    함수 호출을 성공했을 때 리턴하는 값입니다. (Value = 0x00000000) 사용 가능한 게임 클라이언트임을 나타냅니다.
	//   . 기타												: 기타 리턴값에 대해서는 핵쉴드 프로그래밍 가이드 참고바람.
	//
	//   . 주의             : ERROR_SUCCESS가 아닌 에러 코드가 리턴되었을 경우에는 클라이언트와의 세션을 끊어서 
	//                        게임을 더 이상 진행할 수 없도록 할 것을 권장합니다.

	dwRet = _AntiCpSvr_AnalyzeGuidAckMsg ( pIoData->szBuf, pHandleData->byClientInfo, &pHandleData->Context );

	// ERROR_SUCCESS가 아닐 경우 게임-클라이언트의 접속 세션을 종료를 권장합니다.
	if ( dwRet != ERROR_SUCCESS )
	{
		TCHAR szMsg[MAX_PATH] = { 0, };

		_stprintf ( szMsg, TEXT ( "Minia_Server : _AntiCpSvr_AnalyzeGuidAckMsg Fail ( 0x%X ) IP : %d.%d.%d.%d\t%d\n" ),
					dwRet,
					pHandleData->sockaddr.sin_addr.S_un.S_un_b.s_b1,
					pHandleData->sockaddr.sin_addr.S_un.S_un_b.s_b2,
					pHandleData->sockaddr.sin_addr.S_un.S_un_b.s_b3,
					pHandleData->sockaddr.sin_addr.S_un.S_un_b.s_b4,
					ntohs ( pHandleData->sockaddr.sin_port ) );

		
		OutputDebugString ( szMsg );

		header.opCode = OP_ERROR;
		header.length = htonl ( dwRet );

		IoWrite ( pHandleData, &header, NULL, 0 );
	}
	else
	{
		ZeroMemory ( pHandleData->byClientInfo, SIZEOF_REQINFO );
		// [구 서버연동]
		// ---------------------------------------------------------------
		// [_AntiCpSvr_MakeReqMsg]
		// ---------------------------------------------------------------
		// - DESCRIPTION:
		//   클라이언트로 전달할 암호화된 CRC 요구 메시지를 생성합니다.
		// - SYNTAX:
		//	DWORD __stdcall _AntiCpSvr_MakeReqMsg (	IN unsigned long *pCrcInfo,
		//											OUT unsigned char *pbyReqMsg,
		//											OUT unsigned char *pbyReqInfo,
		//											IN unsigned long ulOption)
		// - PARAMETERS:
		//   . pCrcInfo	unsigned long *		:  _AntiCpSvr_AnalyzeGuidAckMsg 함수에서 리턴된 해당 클라이언트가 
		//										사용할 CRC 정보에 대한 포인터
		//   . pbyReqMsg	unsigned char * :	클라이언트로 전송할 암호화된 CRC 요구 메시지, 버퍼의 크기는 
		//										AntiCpSvrFunc.h에 정의된 SIZEOF_REQMSG이어야 합니다. 
		//   . pbyReqInfo	unsigned char *	:	_AntiCpSvr_AnalyzeAckMsg 함수에서 분석할 때 필요한 원본 CRC 
		//										요구 메시지 정보로 버퍼의 크기는 AntiCpSvrFunc.h에 정의된 
		//										SIZEOF_REQINFO이어야 합니다.
		//   . ulOption	unsigned long		:  _AntiCpSvr_AnalyzeAckMsg 함수에서 분석할 때 어떤 정보들에 대한 
		//										Request Message를 만들지에 대한 Flag로 AntiCpSvrFunc.h에 
		//										ANTICPSVR_CHECK_GAME_MEMORY, ANTICPSVR_CHECK_HACKSHIELD_FILE,
		//										ANTICPSVR_CHECK_GAME_FILE, ANTICPSVR_CHECK_ENGINE_FILE,
		//										ANTICPSVR_CHECK_ALL로 정의되어 있습니다. 단, 안전을 위해 최초 호출 
		//										시는 ANTICPSVR_CHECK_ALL Option을 이용하여 전체에 대한 안전 유무를 
		//										검사하고 그 다음부터는 Performance를 위해 ANTICPSVR_CHECK_GAME_MEMORY 
		//										Option만 사용하길 권장합니다. 참고로, 엔진 파일(.v3d)의 변조유무도 
		//										검사하고 싶다면 ANTICPSVR_CHECK_ALL | ANTICPSVR_CHECK_ENGINE_FILE로 
		//										설정하고 사용하십시오. 단, 엔진 파일 (.v3d)이 교체될 때마다 
		//										HackShield.crc 파일을 다시 생성하여 서버에 로드해야 합니다.
		//
		// - RETURN VALUE:
		//    .ERROR_SUCCESS		: 함수 호출을 성공했을 때 리턴하는 값입니다. (Value = 0x00000000)
		//    . 기타				: 기타 리턴값에 대해서는 핵쉴드 프로그래밍 가이드 참고바람.

		// ※ 주의: 게임 성능에 지장을 주지 않도록 처음 한번만 ANTICPSVR_CHECK_ALL 옵션을 사용하고
		//          그 이후 부터는 ANTICPSVR_CHECK_GAME_MEMORY 옵션을 구분해서 사용해야 한다.
		//          예:) m_nFirstCheckCrcOpt = ANTICPSVR_CHECK_ALL 
		//               m_nNextCheckCrcOpt = ANTICPSVR_CHECK_GAME_MEMORY
		dwRet = _AntiCpSvr_MakeReqMsg ( &pHandleData->Context, byReqMsg, pHandleData->byClientInfo, m_nFirstCheckCrcOpt );

		if ( dwRet != ERROR_SUCCESS )
		{
			TCHAR szMsg[MAX_PATH] = { 0, };

			_stprintf ( szMsg, TEXT ( "Minia_Server : _AntiCpSvr_MakeReqMsg Fail ( 0x%X ) IP : %d.%d.%d.%d\t%d\n" ),
						dwRet,
						pHandleData->sockaddr.sin_addr.S_un.S_un_b.s_b1,
						pHandleData->sockaddr.sin_addr.S_un.S_un_b.s_b2,
						pHandleData->sockaddr.sin_addr.S_un.S_un_b.s_b3,
						pHandleData->sockaddr.sin_addr.S_un.S_un_b.s_b4,
						ntohs ( pHandleData->sockaddr.sin_port ) );

			OutputDebugString ( szMsg );

			header.opCode = OP_ERROR;
			header.length = htonl ( dwRet );

			IoWrite ( pHandleData, &header, NULL, 0 );
		}
		else
		{
			header.opCode = OP_REQCRC;
			header.length = htonl ( SIZEOF_REQMSG );

			IoWrite ( pHandleData, &header, byReqMsg, SIZEOF_REQMSG );
		}
	}

	return dwRet;
}


DWORD
Op_CrcAck (
	IN PIO_DATA pIoData,
	IN PHANDLE_DATA pHandleData
	)
{
	BYTE byReqMsg[SIZEOF_REQMSG] = { 0, };
	DWORD dwRet = ERROR_SUCCESS;
	MSGHEADER header = { 0, 0 };
	TCHAR szMsg[MAX_PATH] = { 0, };
	AHNHS_TRANS_BUFFER ptsRequestBuf;
	BOOL bVerify = FALSE;

	// [확장 서버연동]
	if ( m_bUseAntiCpX )
	{
		// -----------------------------------------------------------------
		// [_AhnHS_VerifyResponse ]
		// -----------------------------------------------------------------
		// - DESCRIPTION:
		//  _AhnHS_MakeRequest 함수를 통한 요청(메시지)에 대한 클라이언트의 
		//   응답이 올바른지 검사하는 함수입니다.
		//
		// - SYNTAX:
		//  unsigned long __stdcall _AhnHS_VerifyResponse (IN AHNHS_CLIENT_HANDLE hClient,
		//													IN unsigned char *pbyResponse,
		//												IN unsigned long nResponseLength)
		// - PARAMETERS:
		//  .hClient		AHNHS_CLIENT_HANDLE 	:클라이언트 핸들
		//	.pbyResponse	char *					:클라이언트로 부터 받은 데이터 버퍼
		//  .nResponseLength	unsigned long		:클라이언트로 부터 받은 데이터 길이
		//
		// - RETURN VALUE:
		//   . ERROR_SUCCESS    :
		//     함수 호출을 성공했을 때 리턴하는 값입니다. (Value = 0x00000000) 게임과 HackShield가 
		//     정상적으로 실행되고 있다는 의미입니다.
		//   . 기타				: 기타 리턴값에 대해서는 핵쉴드 프로그래밍 가이드 참고바람.
		

		dwRet = _AhnHS_VerifyResponse ( pHandleData->hClient, pIoData->szBuf, pIoData->nBufLen );

		//   . 주의             : 하기의 리턴값일 경우(bVerify = FALSE)만 해당 클라이언트 접속을 *종료하며
		//                        성공(ERROR_SUCCESS)이 아닌 그 외의 리턴값인 경우는 접속을 종료하지 않고 
		//                        더이상 AhnHS_MakeRequest 함수도 호출하지 않는것을 권장합니다.
		if (  dwRet == ERROR_ANTICPXSVR_BAD_MESSAGE || 
				dwRet == ERROR_ANTICPXSVR_REPLY_ATTACK ||
				dwRet == ERROR_ANTICPXSVR_HSHIELD_FILE_ATTACK ||
				dwRet == ERROR_ANTICPXSVR_CLIENT_FILE_ATTACK ||
				dwRet == ERROR_ANTICPXSVR_MEMORY_ATTACK || 
				dwRet == ERROR_ANTICPXSVR_NANOENGINE_FILE_ATTACK ||
				dwRet == ERROR_ANTICPXSVR_UNKNOWN_CLIENT || 
				dwRet == ERROR_ANTICPXSVR_INVALID_HACKSHIELD_VERSION ||
				dwRet == ERROR_ANTICPXSVR_INVALID_ENGINE_VERSION )
		{
			bVerify = FALSE;
		}
		else
		{
			bVerify = TRUE;
		}

	}
	// [구 서버연동]
	else
	{
		// -----------------------------------------------------------------
		// [_AntiCpSvr_AnalyzeAckMsg]
		// -----------------------------------------------------------------
		// - DESCRIPTION:
		//    클라이언트에서 전달한 암호화된 CRC 응답 메시지를 복호화하여 
		//    게임 파일 및 패킷의 무결성, 핵쉴드 정상 동작 유무 및 핵쉴드 
		//    모듈의 무결성, 그리고 메모리 무결성을 확인합니다.
		// - SYNTAX:
		//    unsigned long __stdcall _AntiCpSvr_AnalyzeAckMsg (
		//									IN unsigned long *pCrcInfo,
		//									IN unsigned char *pbyAckMsg,
		//										IN unsigned char *pbyReqInfo)
		// -PARAMETERS:
		//	.pCrcInfo	unsigned long *	: _AntiCpSvr_AnalyzeGuidAckMsg 함수에서 리턴된 해당 클라이언트가 사용할 
		//															  CRC 정보에 대한 포인터
		//	.pbyAckMsg	unsigned char *	: 클라이언트에서 보낸 암호화된 CRC 응답 메시지
		//  .pbyReqInfo	unsigned char *	: _AntiCpSvr_MakeReqMsg 함수에서 리턴된 CRC 요구 메시지 정보
		//
		// -RETURN VALUE:
		//	 .ERROR_SUCCESS		: 함수 호출을 성공했을 때 리턴하는 값입니다. (Value = 0x00000000) 게임과 
		//						  HackShield가 정상적으로 실행하고 있다는 의미입니다.
		//   . 기타				: 기타 리턴값에 대해서는 핵쉴드 프로그래밍 가이드 참고바람.
		//
		dwRet = _AntiCpSvr_AnalyzeAckMsg ( &pHandleData->Context, pIoData->szBuf, pHandleData->byClientInfo );

		//   . 주의             : 하기의 리턴값일 경우(bVerify = FALSE)만 해당 클라이언트 접속을 *종료하며
		//                        성공(ERROR_SUCCESS)이 아닌 그 외의 경우는 접속을 종료하지 않는것을 권장합니다.
		if (dwRet == ERROR_ANTICPSVR_ANALACKMSG_HSHIELD_ERROR ||
			dwRet == ERROR_ANTICPSVR_ANALACKMSG_PACKET_ERROR  ||
            dwRet == ERROR_ANTICPSVR_ANALACKMSG_FILECRC_ERROR ||
			dwRet == ERROR_ANTICPSVR_ANALACKMSG_MEMORYCRC_ERROR ||
			dwRet == ERROR_ANTICPSVR_ANALACKMSG_NANOENGINECRC_ERROR	)
		{
			bVerify = FALSE;
		}
		else
		{
			bVerify = TRUE;
		}

	}	

	//
	if ( bVerify != TRUE ) 
	{
		_stprintf ( szMsg, TEXT ( "Minia_Server : _AntiCpSvr_AnalyzeAckMsg Fail ( 0x%X ) IP : %d.%d.%d.%d\t%d\n" ),
			dwRet,
			pHandleData->sockaddr.sin_addr.S_un.S_un_b.s_b1,
			pHandleData->sockaddr.sin_addr.S_un.S_un_b.s_b2,
			pHandleData->sockaddr.sin_addr.S_un.S_un_b.s_b3,
			pHandleData->sockaddr.sin_addr.S_un.S_un_b.s_b4,
			ntohs ( pHandleData->sockaddr.sin_port ) );
		
		
		OutputDebugString ( szMsg );
		
		header.opCode = OP_ERROR;
		header.length = htonl ( dwRet );
	
		// ※주의:
		// 해당 샘플은 클라이언트에서 서버로부터에 어떤 유형에 의해 종료되었는지 확인을 위해
		// 클라이언트에 해당 리턴(dwRet)값을 전달하나
		
		// *게임 서버에서는 해당 클라이언트의 접속을 바로 *종료해야 합니다.
		// Close(..)

		IoWrite ( pHandleData, &header, NULL, 0 );
	}
	else
	{
		_stprintf ( szMsg, TEXT ( "Minia_Server : CRC Check OK, IP : %d.%d.%d.%d\t%d\n" ),
			pHandleData->sockaddr.sin_addr.S_un.S_un_b.s_b1,
			pHandleData->sockaddr.sin_addr.S_un.S_un_b.s_b2,
			pHandleData->sockaddr.sin_addr.S_un.S_un_b.s_b3,
			pHandleData->sockaddr.sin_addr.S_un.S_un_b.s_b4,
			ntohs ( pHandleData->sockaddr.sin_port ) );
		
		Sleep ( m_dwCheckInterval );
		
		ZeroMemory ( pHandleData->byClientInfo, SIZEOF_REQINFO );

		// [확장 서버연동]
		if ( m_bUseAntiCpX )
		{
			// 함수설명 위와 동일 
			dwRet = _AhnHS_MakeRequest ( pHandleData->hClient, &ptsRequestBuf );

			TCHAR szMsg[256]={0,};
			_stprintf(szMsg, "##_AhnHS_MakeRequest #2 result:%x\r\n", dwRet);
			OutputDebugString(szMsg);
		}
		// [구 서버연동]
		else
		{
			// 함수설명 위와 동일

			// ※ 주의: 게임 성능에 지장을 주지 않도록 처음 한번만 ANTICPSVR_CHECK_ALL 옵션을 사용하고
			//          그 이후 부터는 ANTICPSVR_CHECK_GAME_MEMORY 옵션을 구분해서 사용해야 한다.
			//          예:) m_nFirstCheckCrcOpt = ANTICPSVR_CHECK_ALL 
			//               m_nNextCheckCrcOpt = ANTICPSVR_CHECK_GAME_MEMORY
			dwRet = _AntiCpSvr_MakeReqMsg ( &pHandleData->Context, byReqMsg, pHandleData->byClientInfo, m_nNextCheckCrcOpt );
		}
		
		if ( dwRet != ERROR_SUCCESS )
		{

			_stprintf ( szMsg, TEXT ( "Minia_Server : _AntiCpSvr_MakeReqMsg Fail ( 0x%X ) IP : %d.%d.%d.%d\t%d\n" ),
				dwRet,
				pHandleData->sockaddr.sin_addr.S_un.S_un_b.s_b1,
				pHandleData->sockaddr.sin_addr.S_un.S_un_b.s_b2,
				pHandleData->sockaddr.sin_addr.S_un.S_un_b.s_b3,
				pHandleData->sockaddr.sin_addr.S_un.S_un_b.s_b4,
				ntohs ( pHandleData->sockaddr.sin_port ) );
			
			
			OutputDebugString ( szMsg );
			
			header.opCode = OP_ERROR;
			header.length = htonl ( dwRet );
			
			IoWrite ( pHandleData, &header, NULL, 0 );
		}
		else
		{
			
			// [확장 서버연동]
			if ( m_bUseAntiCpX )
			{
				header.opCode = OP_REQCRC;
				header.length = htonl ( ptsRequestBuf.nLength );

				IoWrite ( pHandleData, &header, ptsRequestBuf.byBuffer, ptsRequestBuf.nLength );
			}
			// [구 서버연동]
			else
			{
				header.opCode = OP_REQCRC;
				header.length = htonl ( SIZEOF_REQMSG );

				IoWrite ( pHandleData, &header, byReqMsg, SIZEOF_REQMSG );				
			}
		}
	}

	return dwRet;
}
