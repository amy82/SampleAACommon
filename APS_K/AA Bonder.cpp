
// AA Bonder.cpp : 응용 프로그램에 대한 클래스 동작을 정의합니다.
//

#include "stdafx.h"
#include "AA Bonder.h"
#include "AA BonderDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CAABonderApp

BEGIN_MESSAGE_MAP(CAABonderApp, CWinApp)
	ON_COMMAND(ID_HELP, &CWinApp::OnHelp)
	ON_BN_CLICKED(IDSAVE, &CAABonderApp::OnBnClickedSave)
	ON_BN_CLICKED(IDOK, &CAABonderApp::OnBnClickedOk)
END_MESSAGE_MAP()


// CAABonderApp 생성

CAABonderApp::CAABonderApp()
{
	// 다시 시작 관리자 지원
	m_dwRestartManagerSupportFlags = AFX_RESTART_MANAGER_SUPPORT_RESTART;

	// TODO: 여기에 생성 코드를 추가합니다.
	// InitInstance에 모든 중요한 초기화 작업을 배치합니다.
	
	CString cMsg;
	/*if(GetFileAttributes(DATA_DIR) == -1)
	{
		cMsg=DATA_DIR;
		cMsg=cMsg+"폴더가 생성되지 않았습니다.\n"+cMsg+"폴더와 환경설정 파일을 점검하세요.";
		MessageBox(NULL,cMsg,NULL,0);
		exit(1);
	}*/

	if(GetFileAttributes(BASE_DIR) == -1)
	{
		cMsg=BASE_DIR;
		cMsg=cMsg+"폴더가 생성되지 않았습니다.\n"+cMsg+"폴더와 환경설정 파일을 점검하세요.";
		MessageBox(NULL,cMsg,NULL,0);
		exit(1);
	}

	/*if(GetFileAttributes(DATA_DIR) == -1)
	{
		cMsg=DATA_DIR;
		cMsg=cMsg+"폴더가 생성되지 않았습니다.\n"+cMsg+"폴더와 환경설정 파일을 점검하세요.";
		MessageBox(NULL,cMsg,NULL,0);
		exit(1);
	}*/

	if(GetFileAttributes(LOG_DIR) == -1)
	{
		cMsg=LOG_DIR;
		cMsg=cMsg+"폴더가 생성되지 않았습니다.\n"+cMsg+"폴더와 환경설정 파일을 점검하세요.";
		MessageBox(NULL,cMsg,NULL,0);
		exit(1);
	}

}


// 유일한 CAABonderApp 개체입니다.

CAABonderApp theApp;


// CAABonderApp 초기화

BOOL CAABonderApp::InitInstance()
{
	AfxOleInit();
	// 응용 프로그램 매니페스트가 ComCtl32.dll 버전 6 이상을 사용하여 비주얼 스타일을
	// 사용하도록 지정하는 경우, Windows XP 상에서 반드시 InitCommonControlsEx()가 필요합니다.
	// InitCommonControlsEx()를 사용하지 않으면 창을 만들 수 없습니다.
	INITCOMMONCONTROLSEX InitCtrls;
	InitCtrls.dwSize = sizeof(InitCtrls);
	// 응용 프로그램에서 사용할 모든 공용 컨트롤 클래스를 포함하도록
	// 이 항목을 설정하십시오.
	InitCtrls.dwICC = ICC_WIN95_CLASSES;
	InitCommonControlsEx(&InitCtrls);

	CWinApp::InitInstance();

	

	HANDLE hMutex;
	hMutex = CreateMutex(NULL, FALSE, "AA Bonder*.exe");
	
	if(GetLastError()==ERROR_ALREADY_EXISTS)
	{
		CloseHandle(hMutex);
		return false;
	}

	//! Added by LHW (2013/4/23)
	if ( !AfxSocketInit() )
	{
		AfxMessageBox(_T("Windows Socket Init Failed. "));
		return FALSE;
	}


	AfxEnableControlContainer();

	// 대화 상자에 셸 트리 뷰 또는
	// 셸 목록 뷰 컨트롤이 포함되어 있는 경우 셸 관리자를 만듭니다.
	CShellManager *pShellManager = new CShellManager;

	// 표준 초기화
	// 이들 기능을 사용하지 않고 최종 실행 파일의 크기를 줄이려면
	// 아래에서 필요 없는 특정 초기화
	// 루틴을 제거해야 합니다.
	// 해당 설정이 저장된 레지스트리 키를 변경하십시오.
	// TODO: 이 문자열을 회사 또는 조직의 이름과 같은
	// 적절한 내용으로 수정해야 합니다.
	SetRegistryKey(_T("로컬 응용 프로그램 마법사에서 생성된 응용 프로그램"));


	CAABonderDlg dlg;
	m_pMainWnd = &dlg;
	INT_PTR nResponse = dlg.DoModal();
	if (nResponse == IDOK)
	{
		// TODO: 여기에 [확인]을 클릭하여 대화 상자가 없어질 때 처리할
		//  코드를 배치합니다.
	}
	else if (nResponse == IDCANCEL)
	{
		// TODO: 여기에 [취소]를 클릭하여 대화 상자가 없어질 때 처리할
		//  코드를 배치합니다.
	}

	// 위에서 만든 셸 관리자를 삭제합니다.
	if (pShellManager != NULL)
	{
		delete pShellManager;
	}

	
	// 대화 상자가 닫혔으므로 응용 프로그램의 메시지 펌프를 시작하지 않고  응용 프로그램을 끝낼 수 있도록 FALSE를
	// 반환합니다.
	return FALSE;
}



void CAABonderApp::OnBnClickedSave()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
}


void CAABonderApp::OnBnClickedOk()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
}

