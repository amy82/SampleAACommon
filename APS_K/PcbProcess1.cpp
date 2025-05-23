#include "StdAfx.h"
#include "PcbProcess1.h"

CWinThread* pThread_TaskCCD1 = NULL;
bool bThreadCCDRun1 = false;
typedef struct THREADV
{
	int t_Unit;
}_threadV;
//
UINT Thread_TaskCCD1(LPVOID parm)
{
	CAABonderDlg* pFrame = (CAABonderDlg*)AfxGetApp()->m_pMainWnd;
	CString logStr = "";


	bThreadCCDRun1 = true;
	if (!pFrame->MIUCheck_process())// || gMIUDevice.CurrentState != 4)
	{
		logStr.Format("CCD 모듈 영상 초기화 실패.\n 제품 안착 상태 및 제품 불량 확인 하세요.");
		errMsg2(Task.AutoFlag, logStr);
		bThreadCCDRun1 = false;
		return false;
	}
	Sleep(300);
	bThreadCCDRun1 = false;


	return true;
}

void CPcbProcess1::putListLog(CString strLog)
{
	theApp.MainDlg->putListLog(strLog);
}


CPcbProcess1::CPcbProcess1(void)
{


}


CPcbProcess1::~CPcbProcess1(void)
{
}

int CPcbProcess1::Ready_process(int iStep)
{
	CString sLog = "";
	int iRtnFunction = iStep;

	switch (iStep)
	{
	case 10000:
		Task.PCBTaskTime = myTimer(true);
		vision.clearOverlay(CCD);

		sLog.Format("		Ready Step [%d]", iRtnFunction);
		putListLog(sLog);



		if (LGIT_MODEL_INDEX == M1_HEATING_GEN2)		//Ready_process
		{
			Dio.LensTailGrip(false, false);
		}
		iRtnFunction = 10150;
		break;


	case  10150:
		if (Dio.LensMotorGripCheck(true, false))
		{
			if (Task.m_bOKLensPass == 1)   //Lens 넘김 완료상태
			{
				iRtnFunction = 10160;
				sLog.Format("		Ready Step [%d]", iRtnFunction);
				putListLog(sLog);
			}
			else
			{
				sLog.Format("[운전 준비] Lens Grip 전진 상태 입니다[%d]\n렌즈넘김상태 확인바랍니다", iStep);
				errMsg2(Task.AutoFlag, sLog);
				iRtnFunction = -10150;
				break;
			}
		}
		else
		{
			if (!Dio.LensMotorGripCheck(false, false))
			{
				sLog.Format("[운전 준비]Lens Grip 전진 확인 실패 [%d]", iStep);
				errMsg2(Task.AutoFlag, sLog);
				iRtnFunction = -10150;
			}
			else
			{
				if (Task.m_bOKLensPass == 1)   //Lens 넘김 완료상태
				{
					sLog.Format("[운전 준비]Lens Grip 후진 상태 [%d]]\n렌즈넘김상태 확인바랍니다", iStep);
					errMsg2(Task.AutoFlag, sLog);
					iRtnFunction = -10150;
				}
				else
				{
					iRtnFunction = 10160;
					sLog.Format("		Ready Step [%d]", iRtnFunction);
					putListLog(sLog);
				}

			}
		}
		break;

		//Laser 실린더 초기화
	case 10160:
		if (Dio.LaserCylinderUPDOWN(true, false))
		{
			sLog.Format("LASER 실린더 상승 완료[%d]", iRtnFunction);
			theApp.MainDlg->putListLog(sLog);
			Task.PCBTaskTime = myTimer(true);
			iRtnFunction = 10170;
		}
		else
		{
			sLog.Format("LASER 실린더 상승 실패 [%d]", iRtnFunction);
			errMsg2(Task.AutoFlag, sLog);
			iRtnFunction = -10160;
		}
		break;

	case 10170:
		if (Dio.LaserCylinderCheck(true, false))
		{
			sLog.Format("LASER 실린더 상승 확인 완료[%d]", iRtnFunction);
			theApp.MainDlg->putListLog(sLog);
			iRtnFunction = 10200;

			Sleep(300);
		}
		else if ((myTimer(true) - Task.PCBTaskTime) > IO_DELAY_TIME)
		{
			sLog.Format("LASER 실린더 상승 시간 초과..[%d]", iRtnFunction);
			errMsg2(Task.AutoFlag, sLog);
			iRtnFunction = -10170;
		}
		break;
		//Laser 실린더 초기화 END

	case 10200:
		if (motor.PCB_Z_Motor_Move(Wait_Pos))		//PCB Z축 대기 위치 이동
		{
			iRtnFunction = 10300;
			sLog.Format("		Ready Step [%d]", iRtnFunction);
			putListLog(sLog);
		}
		else
		{
			sLangChange.LoadStringA(IDS_STRING305);
			sLog.Format(sLangChange, iStep);	//"[운전 준비] PCB Z축 대기위치 이동 실패[%d]"
			putListLog(sLog);
			iRtnFunction = -10200;
		}

		break;

	case 10300:
		if (motor.LENS_Z_Motor_Move(Wait_Pos))		//Lens Z축 대기 위치
		{
			iRtnFunction = 10400;
			sLog.Format("		Ready Step [%d]", iRtnFunction);
			putListLog(sLog);
		}
		else
		{
			sLangChange.LoadStringA(IDS_STRING303);
			sLog.Format(sLangChange, iStep);	//"[운전 준비] Lens Z축 대기위치 이동 실패[%d]"
			putListLog(sLog);
			iRtnFunction = -10300;
		}
		break;
	case 10400:

		iRtnFunction = 10600;
		sLog.Format("		Ready Step [%d]", iRtnFunction);
		putListLog(sLog);
		break;

	case 10600:
		if (motor.Pcb_Motor_Move(Wait_Pos))			//PCB부 모터 대기위치 이동
		{
			iRtnFunction = 10700;
			sLog.Format("		Ready Step [%d]", iRtnFunction);
			putListLog(sLog);
		}
		else
		{
			sLangChange.LoadStringA(IDS_STRING306);
			sLog.Format(sLangChange, iStep);	//"[운전 준비] PCB 대기위치 이동 실패[%d]"
			putListLog(sLog);
			iRtnFunction = -10600;
		}
		break;
	case 10700:
		if (motor.Lens_Motor_MoveXY(0, Wait_Pos))			//Lens부 모터 대기 위치 이동(Lens X/Y/Z축)
		{
			iRtnFunction = 10750;
			sLog.Format("		Ready Step [%d]", iRtnFunction);
			putListLog(sLog);
		}
		else
		{
			sLangChange.LoadStringA(IDS_STRING304);
			sLog.Format(sLangChange, iStep);	//"[운전 준비] Lens 대기위치 이동 실패[%d]"
			putListLog(sLog);
			iRtnFunction = -10700;
		}
		break;
	case 10750:
		if (motor.Pcb_Tilt_Motor_Move(Wait_Pos))
		{
			iRtnFunction = 10800;
			sLog.Format("		Ready Step [%d]", iRtnFunction);
			putListLog(sLog);
		}
		else
		{
			sLangChange.LoadStringA(IDS_STRING1017);	//PCB부 Tx, Ty축 대기 위치 이동 실패
			sLog.Format(sLangChange);
			putListLog(sLog);
			iRtnFunction = -10750;
		}
		break;
	case 10800:
		if (motor.Lens_Tilt_Motor_Move(Wait_Pos))
		{
			iRtnFunction = 10900;
			sLog.Format("		Ready Step [%d]", iRtnFunction);
			putListLog(sLog);
		}
		else
		{
			sLangChange.LoadStringA(IDS_STRING745);	//LENS부 Tx, Ty축 대기 위치 이동 실패
			sLog.Format(sLangChange);
			putListLog(sLog);
			iRtnFunction = -10800;
		}
		break;

	case  10900:
		///////////////////////
		//if( !Dio.LensGrip(true, true) )
		//{		//-- Lens 감지 상태에서 Grip 전진 안될 경우 NG
		//	sLangChange.LoadStringA(IDS_STRING300);
		//	sLog.Format(sLangChange, iStep);	//"[운전 준비] Lens Grip 전진 실패 [%d]"
		//	putListLog(sLog);
		//	iRtnFunction = -10900;
		//	break;
		//}			
		//else if( Dio.LensGripCheck(false, false) )			//Lens 감지가 안되었을 경우  
		//{//-- Lens Grip 후진 동작
		//if(! Dio.HolderGrip(false,false) )    //-- Lens 감지 안되고/ Grip 후진 안될 경우 NG
		//{  
		//	sLangChange.LoadStringA(IDS_STRING301);
		//	sLog.Format(sLangChange, iStep);	//"[운전 준비] Lens Grip 후진 실패 [%d]"
		//	putListLog(sLog);
		//	iRtnFunction = -10900;
		//	break;
		//}
		iRtnFunction = 10950;
		sLog.Format("		Ready Step [%d]", iRtnFunction);
		putListLog(sLog);
		break;
	case 10950:
		iRtnFunction = 10970;
		sLog.Format("		Ready Step [%d]", iRtnFunction);
		putListLog(sLog);
		break;
	case 10970:
		if (!MIU.Stop())					// 95 ~ 100 msec
		{
			sLog.Format(_T("CCD CLOSE FAIL [%d]"), iStep);
			putListLog(sLog);
			iRtnFunction = -10970;
			break;
		}
		iRtnFunction = 15000;	//While문 종료
		sLog.Format("		Ready Step [%d]", iRtnFunction);
		putListLog(sLog);
		break;
	case 15000:
		UVCommand.UV_Shutter_PowerSet(model.UV_Power);// 95);//
		sLog.Format("	#1 UV POWER SET: %d", model.UV_Power);
		putListLog(sLog);
		Sleep(500);
		UVCommand2.UV_Shutter_PowerSet(model.UV_Power);// 95);//
		sLog.Format("	#2 UV POWER SET: %d", model.UV_Power);
		putListLog(sLog);
		iRtnFunction = 19000;
		break;
	case 19000:
		Task.AutoReday = 1;
		//		AutoRunView(3);
		iRtnFunction = 19900;
		sLog.Format("		Ready Step [%d]", iRtnFunction);
		putListLog(sLog);
		break;
	}
	return iRtnFunction;
}
int CPcbProcess1::RunProc_ProductLoading(int iUseStep)
{//! 작업자 제품 투입 자동 Step		(10000 ~ 10999)
	int iRtnFunction = iUseStep;
	CString logStr = "", sBarCode = "";
	bool bChk = true;

	switch (iUseStep)
	{
	case 10000://! Door커튼 Lift Open.
		Task.interlockLens  = 0;
		Task.interlockPcb = 0;

		iRtnFunction = 10010;
		break;
	case 10010:
		iRtnFunction = 10030;
		break;
	case 10030:
		iRtnFunction = 10050;
		break;
	case 10050:
		//LightControl.ctrlLedVolume(LIGHT_OC_6500K, 0);		// Align만 조명 ON
		Dio.LaserCylinderUPDOWN(true, false);

		logStr.Format("LASER 실린더 상승 완료[%d]", iRtnFunction);
		theApp.MainDlg->putListLog(logStr);
		Task.PCBTaskTime = myTimer(true);

		iRtnFunction = 10060;
		break;

	case 10060:
		if (Dio.LaserCylinderCheck(true, false))
		{
			logStr.Format("LASER 실린더 상승 확인 완료[%d]", iRtnFunction);
			theApp.MainDlg->putListLog(logStr);
			iRtnFunction = 10100;

			Sleep(300);
		}
		else if ((myTimer(true) - Task.PCBTaskTime) > IO_DELAY_TIME)
		{
			logStr.Format("LASER 실린더 상승 시간 초과..[%d]", iRtnFunction);
			errMsg2(Task.AutoFlag, logStr);
			iRtnFunction = -10060;
		}
		break;

	case 10100://! Chip 정보 Draw
		theApp.MainDlg->func_ChipID_Draw();

		iRtnFunction = 10110;

		logStr.Format("		Start 버튼을 눌러주세요[%d]", iRtnFunction);
		putListLog(logStr);
		theApp.MainDlg->m_btnStart.m_iStateBtn = 4;
		theApp.MainDlg->m_btnStart.Invalidate();
		break;

	case 10110://! Start P/B Button Push 대기. 

		if (Dio.StartPBOnCheck(true, false))// || Start_Btn_On)	//프로그램에 start 버튼 추가
		{
			iRtnFunction = 10165;
			MbufClear(vision.MilOptImage, 0);
			theApp.MainDlg->m_btnStart.m_iStateBtn = 0;
			theApp.MainDlg->m_btnStart.Invalidate();
			theApp.MainDlg->Start_Btn_On = false;
			if (Task.m_bOKLensPass == 1)   //Lens 넘김 완료상태
			{
				if (!askMsg("렌즈 넘김상태입니다. \n계속 진행하시겠습니까?") == IDOK)
				{
					iRtnFunction = 10000;
					logStr.Format("		loading Step [%d]", iRtnFunction);
					putListLog(logStr);
					break;
				}
			}
			logStr.Format("		loading Step [%d]", iRtnFunction);
			putListLog(logStr);
		}
		break;
	case 10165://! Start P/B Button Push 대기. 
			   //Dio.StartPBLampOn(false);
		sBarCode.Format("%s", Task.ChipID);
		if (sysData.m_iProductComp == 1)
		{
			logStr.Format("====%s 완제품 검사 start====", Task.ChipID);
		}
		else
		{
			logStr.Format("====%s AA start====", Task.ChipID);
		}
		vision.clearOverlay(CCD);
		vision.drawOverlay(CCD);
		putListLog("	");
		putListLog(logStr);
		putListLog("	");
		iRtnFunction = 10170;
		logStr.Format("		loading Step [%d]", iRtnFunction);
		putListLog(logStr);
		break;
	case 10170://! Start P/B Button Push 대기.
	{
		bool bflag = true;

		Task.d_Align_offset_x[LENS_Align_MARK] = 0;
		Task.d_Align_offset_y[LENS_Align_MARK] = 0;
		Task.d_Align_offset_th[LENS_Align_MARK] = 0;

		/*bflag = EpoxyTimeCheck(bEpoxyTimeChk);
		if(!bflag)
		{
		iRtnFunction = -10170;
		break;
		}*/
		MandoInspLog.func_InitData();
		Task.func_TactTimeInit();
		theApp.MainDlg->dispGrid();
		Task.m_timeChecker.Measure_Time(1);	//Start 버튼 클릭 시간
		theApp.MainDlg->func_ChipID_Draw();
		MandoInspLog.bInspRes = true;
		vision.clearOverlay(CCD);
		int iSocketCount = 0;
		//logStr.Format("%d", sysData.m_Socket_Count);
		iSocketCount = atoi(logStr);
		iSocketCount++;
		sysData.m_Socket_Count = iSocketCount;
		sysData.Save();

		iRtnFunction = 10180;
		logStr.Format("		loading Step [%d]", iRtnFunction);
		putListLog(logStr);
	}
	break;
	case 10180:
		if (motor.PCB_Z_Motor_Move(Wait_Pos))
		{
			logStr.Format("PCB Z Wait 위치 이동 완료 [%d]", iRtnFunction);
			putListLog(logStr);
			iRtnFunction = 10200;
			logStr.Format("		loading Step [%d]", iRtnFunction);
			putListLog(logStr);
		}
		else
		{
			logStr.Format("PCB Z Wait 위치 이동 완료 실패 [%d]", iRtnFunction);
			errMsg2(Task.AutoFlag, logStr);
			iRtnFunction = -10180;
		}
		break;
	case 10200:
		if (motor.Lens_Tilt_Motor_Move(Wait_Pos))
		{
			iRtnFunction = 10202;
			logStr.Format("		Ready Step [%d]", iRtnFunction);
			putListLog(logStr);
		}
		else
		{
			sLangChange.LoadStringA(IDS_STRING745);	//LENS부 Tx, Ty축 대기 위치 이동 실패
			logStr.Format(sLangChange);
			putListLog(logStr);
			iRtnFunction = -10200;
		}
		break;

	case 10202:
		if (Dio.LaserCylinderCheck(true, false) == false)
		{
			logStr.Format("LASER 실린더 상승 확인 실패[%d]", iRtnFunction);
			errMsg2(Task.AutoFlag, logStr);
			theApp.MainDlg->putListLog(logStr);
			iRtnFunction = -10202;
			break;
		}
		iRtnFunction = 10205;
		logStr.Format("		loading Step [%d]", iRtnFunction);
		putListLog(logStr);
		break;

	case 10205:
		if (sysData.m_iProductComp == 1)
		{
			iRtnFunction = 10250;
			logStr.Format("		loading Step [%d]", iRtnFunction);
			putListLog(logStr);
		}
		else
		{
			iRtnFunction = 10210;
			logStr.Format("		loading Step [%d]", iRtnFunction);
			putListLog(logStr);
		}
		break;

	case 10210:
		if (Task.m_bOKLensPass == 1)   //Lens 넘김 완료상태
		{
			iRtnFunction = 10250;
			break;
		}

		iRtnFunction = 10250;
		logStr.Format("		loading Step [%d]", iRtnFunction);
		putListLog(logStr);
		break;

	case 10250:
		iRtnFunction = 10400;
		logStr.Format("		loading Step [%d]", iRtnFunction);
		putListLog(logStr);
		break;
	case 10400:	//! 바코드 정보 Check

				////////////////////////////////
		MandoInspLog.func_InitData();	//-- Log 초기화
		sBarCode.Format("%s", Task.ChipID);

		if (sBarCode == "EMPTY")//바코드정보가 없을 경우 확인 Message
		{
			logStr.Format("바코드 정보가 없습니다. \n [ %s ]명으로 진행 하시겠습니까?", Task.ChipID);
			if (askMsg(logStr) == IDOK)
			{
				sBarCode.Format("%s", Task.ChipID);
			}
			else
			{
				iRtnFunction = 10100;
				logStr.Format("		loading Step [%d]", iRtnFunction);
				putListLog(logStr);
				break;
			}
		}
		else
		{
			if (!g_ADOData.func_AATaskToRecordID(modelList.curModelName, Task.ChipID))
			{
				logStr.Format("		loading Step [%d]", iRtnFunction);
				putListLog(logStr);
#if (____AA_WAY == PCB_TILT_AA)
				/*if(sysData.m_iProductComp==1)
				{
				logStr.Format("일치하는 아이디가 없습니다.[%d]", iUseStep);
				}else
				{
				logStr.Format("DataBase 생성 실패.[%d]\n MS Office를 닫아주세요.", iUseStep);
				}
				errMsg2(Task.AutoFlag,logStr);
				iRtnFunction = -10400;
				break;*/
#endif		

			}
		}


		iRtnFunction = 10440;

		break;
	case 10440:
		if (sysData.m_iProductComp != 1)
		{
			pThread_TaskCCD1 = ::AfxBeginThread(Thread_TaskCCD1, this);

		}
		iRtnFunction = 10500;
		logStr.Format("		loading Step [%d]", iRtnFunction);
		putListLog(logStr);
		break;
	case 10500:
		iRtnFunction = 10900;
		break;
	//case 10550:
	//	//그립확인
	//	if (Dio.PcbGripCheck(true, false) == true)
	//	{
	//		logStr.Format(_T("PCB GRIP 확인 완료 [%d]"), iRtnFunction);
	//		iRtnFunction = 10600;
	//	}
	//	else if ((myTimer(true) - Task.PCBTaskTime) > IO_DELAY_TIME)
	//	{
	//		logStr.Format(_T("PCB GRIP 실패 [%d]"), iRtnFunction);
	//		iRtnFunction = -10550;
	//		errMsg2(Task.AutoFlag, logStr);
	//	}
	//	break;
	//case 10600:
	//	//턴
	//	Sleep(1000);
	//	if (Dio.PcbTurn(false, false) == true)
	//	{
	//		logStr.Format(_T("PCB RETURN 완료 [%d]"), iRtnFunction);
	//		iRtnFunction = 10650;
	//	}
	//	else
	//	{
	//		logStr.Format(_T("PCB RETURN 실패 [%d]"), iRtnFunction);
	//		iRtnFunction = -10600;
	//		errMsg2(Task.AutoFlag, logStr);
	//	}
	//	putListLog(logStr);
	//	Task.PCBTaskTime = myTimer(true);

	//	break;
	//case 10650:
	//	//턴확인
	//	if (Dio.PcbTurnCheck(false, false) == true)
	//	{
	//		logStr.Format(_T("PCB RETURN 확인 완료 [%d]"), iRtnFunction);
	//		iRtnFunction = 10700;
	//	}
	//	else if ((myTimer(true) - Task.PCBTaskTime) > IO_DELAY_TIME)
	//	{
	//		logStr.Format(_T("PCB RETURN 확인 실패 [%d]"), iRtnFunction);
	//		iRtnFunction = -10650;
	//		errMsg2(Task.AutoFlag, logStr);
	//	}

	//	break;
	//case 10700:
	//	//PCB 로딩 후 확인 눌러주세요
	//	if (askMsg("PCB 로딩 후 확인 눌러주세요") == IDOK)
	//	{
	//		iRtnFunction = 10900;
	//	}

	//	break;
	case 10900:
		Task.PcbOnStage = 100;
		Task.m_b_AA_Retry_Flag = false;

		logStr.Format("%d", Task.m_iPcbPickupNo);
		theApp.MainDlg->m_labelPickupNoPcb.SetText(logStr);
		theApp.MainDlg->m_labelPickupNoPcb.Invalidate();

		MandoInspLog.sBarcodeID.Format("%s", Task.ChipID);	//만도 차량용Camera 검사 Log 저장
		iLaser_Pos = 0;


		if (sysData.m_iIrChartUse == 1)
		{
			IrvAlignLed.Irv_Light_SetValue(model.m_iLedValue[LEDDATA_TOP1_CHART], IR_CHART);
		}
		else
		{
			LightControl.ctrlLedVolume(LIGHT_TOP_CHART1, model.m_iLedValue[LEDDATA_TOP1_CHART]);
			LightControl.ctrlLedVolume(LIGHT_TOP_CHART2, model.m_iLedValue[LEDDATA_TOP2_CHART]);
			LightControl.ctrlLedVolume(LIGHT_TOP_CHART3, model.m_iLedValue[LEDDATA_TOP3_CHART]);
			LightControl.ctrlLedVolume(LIGHT_TOP_CHART4, model.m_iLedValue[LEDDATA_TOP4_CHART]);
			LightControl.ctrlLedVolume(LIGHT_LEFT_CHART, model.m_iLedValue[LEDDATA_CHART_L]);
			LightControl.ctrlLedVolume(LIGHT_RIGHT_CHART, model.m_iLedValue[LEDDATA_CHART_R]);
		}




#if (____AA_WAY == PCB_TILT_AA)
		if (Task.m_bOKLensPass == 1)   //Lens 넘김 완료상태
		{
			logStr.Format("		Lens 그립 진행 [%d]", iRtnFunction);
			putListLog(logStr);
			iRtnFunction = 15500;// 10250;
			break;
		}
#endif
		iRtnFunction = 11000;
		logStr.Format("		loading Step [%d]", iRtnFunction);
		putListLog(logStr);
		break;

	default:
		sLangChange.LoadStringA(IDS_STRING1007);	//PCB 제품 투입 Thread Step 번호 비정상 . 
		logStr.Format("PCB 제품 투입 Thread Step 번호 비정상 [%d]", iUseStep);
		errMsg2(Task.AutoFlag, logStr);
		iRtnFunction = -1;
		break;
	}

	return iRtnFunction;
}

int CPcbProcess1::RunProc_PCBOutsideAlign(int iUseStep)
{
	//! PCB 외부 Align Step(11000 ~ 14999)					//!! MIU_Initialize -> PCB Holder Align 위치
	int iRtnFunction = iUseStep;
	int iCamDelay = model.strInfo_Cam[1].m_iDelayTime;		//PCB 영상 획득 Delay
	int iAlignRetry = model.strInfo_Cam[1].m_iRetryCnt;
	int iRtn = 0;
	CString logStr = "";
	bool bChk = true;
	double offsetX = 0.0, offsetY = 0.0, offsetTh = 0.0;

	//ctrlSubDlg(MAIN_DLG);		//20161212  ccd start 수정
	theApp.MainDlg->m_iCurCamNo = 0;
	theApp.MainDlg->setCamDisplay(1, 0);
	theApp.MainDlg->changeMainBtnColor(MAIN_DLG);

	switch (iUseStep)
	{
	case 11000:
		/////////////////////////
		Task.PCBTaskTime = myTimer(true);
		iRtnFunction = 11500;
		break;

	case 11500:
		iRtnFunction = 12000;
		break;
	case 12000://! 바코드 DB Data 저장
			   /*if( !g_ADOData.func_AATaskToRecordID(modelList.curModelName, Task.ChipID) )
			   {
			   logStr.Format("DataBase 생성 실패.[%d]\n MS Office를 닫아주세요.", iUseStep);
			   errMsg2(Task.AutoFlag,logStr);
			   iRtnFunction = -12000;
			   break;
			   }*/
		iRtnFunction = 13000;
		break;

	case 13000:
		if (Task.m_bOKLensPass == 1)   //Lens 넘김 완료상태
		{
			iRtnFunction = 14100;
			break;
		}

		Task.m_iRetry_Opt = 0;
		LightControlthird.ctrlLedVolume(LIGHT_PCB, model.m_iLedValue[LEDDATA_HOLDER]);		// Align만 조명 ON
		Task.d_Align_offset_x[PCB_Holder_MARK] = 0;
		Task.d_Align_offset_y[PCB_Holder_MARK] = 0;
		Task.d_Align_offset_th[PCB_Holder_MARK] = 0;
		if (motor.Pcb_Motor_Move(Holder_Pos))
		{
			Task.PCBTaskTime = myTimer(true);
			logStr.Format("Pcb Holder 위치 이동 완료[%d]", iUseStep);
			putListLog(logStr);
			iRtnFunction = 13300;
		}
		else
		{
			logStr.Format("Pcb Holder 위치 이동 실패[%d]", iUseStep);
			errMsg2(Task.AutoFlag, logStr);
			iRtnFunction = -13000;
		}
		break;

	case 13300:
		if (motor.PCB_Z_Motor_Move(Holder_Pos))
		{
			Task.PCBTaskTime = myTimer(true);
			logStr.Format("Pcb Holder Z위치 이동 완료[%d]", iUseStep);
			putListLog(logStr);
			//151109 영진 임시 딜레이
			Sleep(1000);
			iRtnFunction = 13500;
		}
		else
		{
			logStr.Format("Pcb Holder Z위치 이동 실패[%d]", iUseStep);
			errMsg2(Task.AutoFlag, logStr);
			iRtnFunction = -13300;
		}
		break;
	case 13500:
		if ((myTimer(true) - Task.PCBTaskTime) > iCamDelay && (motor.IsStopAxis(Motor_PCB_X) && motor.IsStopAxis(Motor_PCB_Y)))
		{
			Task.PCBTaskTime = myTimer(true);
			iRtnFunction = 13700;
		}
		break;
	case 13700: // lens Align

		if (Task.m_iRetry_Opt > iAlignRetry)
		{
			logStr.Format("PCB Holder 재검사 %d 회 실패[%d]\n 보정 미적용으로 진행하시겠습니까?", Task.m_iRetry_Opt, iUseStep);
			if (askMsg(logStr) == IDOK)
			{
				Task.d_Align_offset_x[PCB_Holder_MARK] = 0;
				Task.d_Align_offset_y[PCB_Holder_MARK] = 0;
				Task.d_Align_offset_th[PCB_Holder_MARK] = 0;
				iRtnFunction = 14100;
			}
			else
			{
				Task.m_iRetry_Opt = 0;
				iRtnFunction = 13700;
				logStr.Format("PCB Holder 재검사 시작[%d]", iUseStep);
				putListLog(logStr);
				//logStr.Format("PCB Holder 재검사 %d 회 실패[%d]", Task.m_iRetry_Opt, iUseStep);
				//errMsg2(Task.AutoFlag,logStr);
			}

			break;
		}

		offsetX = offsetY = offsetTh = 0.0;
		iRtn = theApp.MainDlg->procCamAlign(CAM2 - 1, PCB_Holder_MARK, false, offsetX, offsetY, offsetTh);
		offsetTh = 0.0;
		saveInspImage(PCB_IMAGE_SAVE, Task.m_iRetry_Opt);

		Task.m_iRetry_Opt++;
		offsetTh = 0;//holder x,y축만
		if (iRtn == 0)
		{
			Task.d_Align_offset_x[PCB_Holder_MARK] -= offsetX;
			Task.d_Align_offset_y[PCB_Holder_MARK] -= offsetY;
			Task.d_Align_offset_th[PCB_Holder_MARK] += offsetTh;

			logStr.Format("Pcb Holder : %.3lf %.3lf %.3lf", Task.d_Align_offset_x[PCB_Holder_MARK], Task.d_Align_offset_y[PCB_Holder_MARK], Task.d_Align_offset_th[PCB_Holder_MARK]);//0.000
																																													 //			Sleep(500);
																																													 //pcbDlg->m_labelAlignResult.SetText(logStr);
																																													 //pcbDlg->m_labelAlignResult.Invalidate();

			putListLog(logStr);

			int iRtnVal = theApp.MainDlg->AlignLimitCheck(CAM2 - 1, offsetX, offsetY, offsetTh);
			if (iRtnVal == 1)
			{
				iRtnFunction = 14000;				// 보정 이동
			}
			else if (iRtnVal == 2)
			{
				sLangChange.LoadStringA(IDS_STRING654);	//Lens Align [%d] 완료 [%d]
				logStr.Format("		Pcb Holder [%d] 완료 [%d]", Task.m_iRetry_Opt, iUseStep);
				putListLog(logStr);

				theApp.MainDlg->InstantMarkDelete(PCB_Holder_MARK);			//임시 등록 마크 삭제

				logStr.Format("		Pcb Holder Complete : %d, %.03f, %.03f, %.03f", Task.m_iPcbPickupNo, Task.d_Align_offset_x[PCB_Holder_MARK], Task.d_Align_offset_y[PCB_Holder_MARK], Task.d_Align_offset_th[PCB_Holder_MARK]);//0.000);
				AlignResultSave(logStr);
				Task.m_iRetry_Opt = 0;
				iRtnFunction = 14100;				// 완료
			}
			else
			{
				logStr.Format("Pcb Holder [%d] 보정 범위 초과[%d]", Task.m_iRetry_Opt, iUseStep);
				errMsg2(Task.AutoFlag, logStr);
				AlignLogSave(logStr);
				theApp.MainDlg->InstantMarkDelete(PCB_Holder_MARK);
				iRtnFunction = -13700;				// NG
			}
		}
		else
		{
			logStr.Format("Pcb Holder [%d] 마크 인식 실패[%d]", Task.m_iRetry_Opt, iUseStep);
			putListLog(logStr);
			Sleep(300);
			iRtnFunction = 13700;					//  재검사 
		}
		break;

	case 14000://!! 보정량 이동.- Retry
		if (motor.Pcb_Holder_Align_Move(Holder_Pos, Task.d_Align_offset_x[PCB_Holder_MARK], Task.d_Align_offset_y[PCB_Holder_MARK], Task.d_Align_offset_th[PCB_Holder_MARK]))
		{
			Task.PCBTaskTime = myTimer(true);
			iRtnFunction = 13500;
		}
		else
		{
			logStr.Format("Pcb Holder 보정 위치 이동 실패[%d]", iUseStep);
			errMsg2(Task.AutoFlag, logStr);
			iRtnFunction = -14000;
		}
		break;
	case 14100:
		if (!motor.PCB_Z_Motor_Move(Wait_Pos))
		{
			logStr.Format(" PCB Z축 대기위치 이동 실패[%d]", iUseStep);
			delayMsg(logStr.GetBuffer(99), 3000, M_COLOR_RED);
			iRtnFunction = -14100;
			break;
		}
#if (____AA_WAY == PCB_TILT_AA)
		iRtnFunction = 15000;
#elif (____AA_WAY == LENS_TILT_AA)
		iRtnFunction = 26000;

#endif

		break;
	default:
		logStr.Format("PCB 제품 투입 Thread Step 번호 비정상 . [%d]", iUseStep);
		errMsg2(Task.AutoFlag, logStr);
		iRtnFunction = -1;
		break;
	}
	return iRtnFunction;
}

int	CPcbProcess1::RunProc_SensorAlign(int iUseStep)
{
	int iRtnFunction = iUseStep;
	int iRtn = 0;
	CString logStr = _T("");
	bool bChk = true;
	double offsetX = 0.0, offsetY = 0.0, offsetTh = 0.0;
	int iCamDelay = model.strInfo_Cam[1].m_iDelayTime;	//PCB 영상 획득 Delay
	int iAlignRetry = model.strInfo_Cam[1].m_iRetryCnt;
	int iLaserDelay = model.strInfo_Cam[0].m_iDelayTime;	//Laser 측정전 Delay
	switch (iUseStep)
	{
	case 26000:
		offsetX = offsetY = offsetTh = 0.0;
		if (!motor.PCB_Z_Motor_Move(Wait_Pos))
		{
			logStr.Format("		PCB Z축 대기위치 이동 실패[%d]", iUseStep);
			delayMsg(logStr.GetBuffer(99), 3000, M_COLOR_RED);
			iRtnFunction = -26000;
			break;
		}
		//Sensor Align Pass 241128
		Task.d_Align_offset_x[PCB_Chip_MARK] = 0;
		Task.d_Align_offset_y[PCB_Chip_MARK] = 0;
		Task.d_Align_offset_th[PCB_Chip_MARK] = 0;
		iRtnFunction = 26629;

		break;

		iRtnFunction = 26050;
		break;
	case 26050:
		Task.m_iRetry_Opt = 0;
		iRtnFunction = 26600;
		break;

	case 26600:
		//Task.LensTaskTime = myTimer(true);
		iRtnFunction = 26610;
		break;

	case 26610:
		//ctrlSubDlg(MAIN_DLG);
		theApp.MainDlg->m_iCurCamNo = 0;
		//SetDigReference(PCB_Chip_MARK);
		LightControlthird.ctrlLedVolume(LIGHT_PCB, model.m_iLedValue[LEDDATA_SENSOR]);		// Align만 조명 ON

		if (motor.Pcb_Motor_Move(Align_Pos))
		{
			if (motor.PCB_Z_Motor_Move(Align_Pos))
			{
				logStr.Format("PCB Sensor Align 위치 이동 완료[%d]", iUseStep);
				putListLog(logStr);
				Task.PCBTaskTime = myTimer(true);
				Task.d_Align_offset_x[PCB_Chip_MARK] = 0;
				Task.d_Align_offset_y[PCB_Chip_MARK] = 0;
				Task.d_Align_offset_th[PCB_Chip_MARK] = 0;
				iRtnFunction = 26615;
			}
		}
		else
		{
			logStr.Format("PCB Sensor Align 위치 이동 실패 [%d]", iUseStep);
			errMsg2(Task.AutoFlag, logStr);
			iRtnFunction = -26610;
		}
		break;
	case 26615:
		if ((myTimer(true) - Task.PCBTaskTime) > iCamDelay && (motor.IsStopAxis(Motor_PCB_X) && motor.IsStopAxis(Motor_PCB_Y)))
		{
			iRtnFunction = 26620;
		}
		break;
	case 26620:
		if (Task.m_iRetry_Opt>iAlignRetry)
		{
			logStr.Format("PCB Align(*SensorAlign*) 재검사 %d 회 실패[%d]\n 보정 미적용으로 진행하시겠습니까?", Task.m_iRetry_Opt, iUseStep);
			Task.d_Align_offset_x[PCB_Chip_MARK] = 0.0;
			Task.d_Align_offset_y[PCB_Chip_MARK] = 0.0;
			Task.d_Align_offset_th[PCB_Chip_MARK] = 0.0;
			Task.m_iRetry_Opt = 0;
			if (askMsg(logStr) == IDOK)
			{
				iRtnFunction = 26629;
			}
			else
			{
				iRtnFunction = 26620;
				logStr.Format("PCB Align 재검사 시작[%d]", iUseStep);
				putListLog(logStr);
			}
			break;

			break;
		}

		offsetX = offsetY = offsetTh = 0.0;

		iRtn = theApp.MainDlg->procCamAlign(CAM2 - 1, PCB_Chip_MARK, false, offsetX, offsetY, offsetTh);
		saveInspImage(CHIP_IMAGE_SAVE, Task.m_iRetry_Opt);
		Task.m_iRetry_Opt++;

		//offsetTh=0;
		if (iRtn == 0)
		{
			Task.d_Align_offset_x[PCB_Chip_MARK] -= offsetX;
			Task.d_Align_offset_y[PCB_Chip_MARK] -= offsetY;
			Task.d_Align_offset_th[PCB_Chip_MARK] -= offsetTh;


			Task.dPcbAlignTh = Task.d_Align_offset_th[PCB_Chip_MARK];

			logStr.Format("PCB Sensor Align : %.3lf %.3lf %.3lf", Task.d_Align_offset_x[PCB_Chip_MARK], Task.d_Align_offset_y[PCB_Chip_MARK], Task.d_Align_offset_th[PCB_Chip_MARK]);
			//pcbDlg->m_labelAlignResult.SetText(logStr);
			//pcbDlg->m_labelAlignResult.Invalidate();

			putListLog(logStr);

			int iRtnVal = theApp.MainDlg->AlignLimitCheck(CAM2 - 1, offsetX, offsetY, offsetTh);

			if (iRtnVal == 1)
			{
				iRtnFunction = 26625;				// 보정 이동
			}
			else if (iRtnVal == 2)
			{
				logStr.Format("		PCB Sensor Align [%d] 완료 [%d]", Task.m_iRetry_Opt, iUseStep);
				putListLog(logStr);
				//InstantMarkDelete(PCB_Chip_MARK);			//임시 등록 마크 삭제

				logStr.Format("		PCB Sensor Align Complete : %d, %.03f, %.03f, %.03f", Task.m_iPcbPickupNo, Task.d_Align_offset_x[PCB_Chip_MARK], Task.d_Align_offset_y[PCB_Chip_MARK], Task.d_Align_offset_th[PCB_Chip_MARK]);
				AlignResultSave(logStr);
				Task.m_iRetry_Opt = 0;

				Task.m_timeChecker.Measure_Time(4);	//Laser 변위 측정 시작 시간

				iRtnFunction = 26629;		// 완료//iRtnFunction = 16630;				// 완료
				LightControlthird.ctrlLedVolume(LIGHT_PCB, 0);		// Align만 조명 ON
			}
			else
			{
				logStr.Format("PCB Sensor Align [%d] 보정 범위 초과", Task.m_iRetry_Opt, iUseStep);
				errMsg2(Task.AutoFlag, logStr);
				AlignLogSave(logStr);
				//InstantMarkDelete(PCB_Chip_MARK);
				iRtnFunction = -26620;				// NG
			}
		}
		else
		{
			logStr.Format("PCB Sensor Align [%d] 마크 인식 실패[%d]", Task.m_iRetry_Opt, iUseStep);
			putListLog(logStr);
			Sleep(300);
			iRtnFunction = 26620;				//  재검사 
		}
		break;
	case 26625://- Sensor Align Retry
		if (motor.Pcb_Holder_Align_Move(Align_Pos, Task.d_Align_offset_x[PCB_Chip_MARK], Task.d_Align_offset_y[PCB_Chip_MARK], Task.d_Align_offset_th[PCB_Chip_MARK]))
		{
			Task.PCBTaskTime = myTimer(true);
			iRtnFunction = 26615;
		}
		else
		{
			logStr.Format("PCB Sensor Align 보정 위치 이동 실패[%d]", iUseStep);
			errMsg2(Task.AutoFlag, logStr);
			iRtnFunction = -26625;
		}
		break;
	case 26629:
		if (!motor.PCB_Z_Motor_Move(Wait_Pos))
		{
			sLangChange.LoadStringA(IDS_STRING305);	//PCB부 z축 모터 위치 이동 실패[%d]
			logStr.Format(sLangChange, iUseStep);
			delayMsg(logStr.GetBuffer(99), 3000, M_COLOR_RED);
			iRtnFunction = -26629;
			break;
		}
		iRtnFunction = 26990;
		break;
	case 26990://! Lens-Z축 대기 위치 이동
#if (____AA_WAY == PCB_TILT_AA)
		if (sysData.m_iEpoxyLaserInspPass == 1)
		{
			iRtnFunction = 30000;	//도포이동
		}
		else
		{
			iRtnFunction = 27000;	//Laser측정 ->Align -> 도포
		}
#elif (____AA_WAY == LENS_TILT_AA)
		if (sysData.m_iEpoxyLaserInspPass == 1)
		{
			iRtnFunction = 30000;//Laser 측정 Pass
		}
		else
		{
			iRtnFunction = 27000;
		}
#endif
		break;
	default:
		logStr.Format("Lens 외부 Align Thread Step 번호 비정상 [%d]", iUseStep);
		errMsg2(Task.AutoFlag, logStr);
		iRtnFunction = -1;
		break;
	}
	logStr.Empty();
	return iRtnFunction;
}

int	CPcbProcess1::RunProc_LaserMeasure(int iUseStep)
{
	int iRtnFunction = iUseStep;
	int iRtn = 0;
	CString logStr = "";
	bool bChk = true;
	double offsetX = 0.0, offsetY = 0.0, offsetTh = 0.0;
	int iCamDelay = model.strInfo_Cam[1].m_iDelayTime;	//PCB 영상 획득 Delay
	int iAlignRetry = model.strInfo_Cam[1].m_iRetryCnt;
	int iLaserDelay = model.strInfo_Cam[0].m_iDelayTime;	//Laser 측정전 Delay
	switch (iUseStep)
	{
	case 27000:
		Task.oldLaserTx = 0.1;
		Task.oldLaserTy = 0.1;
		iLaser_Pos = 0;// 4;//4번부터가 laser out point
		LightControlthird.ctrlLedVolume(LIGHT_PCB, 0);		// Align만 조명 ON
		if (!motor.PCB_Z_Motor_Move(Wait_Pos))
		{
			logStr.Format("[운전 준비] PCB Z축 대기위치 이동 실패[%d]", iUseStep);
			delayMsg(logStr.GetBuffer(99), 3000, M_COLOR_RED);
			iRtnFunction = -27000;
			break;
		}

		//if ((sysData.m_iDispensePass == 1) || (Task.m_bOkDispense == 1))
		//{
		//	iRtnFunction = 30000;	//-- 도포 Pass Mode 이거나, 도포 완료 일 경우
		//	break;
		//}

		iRtnFunction = 27100;// 27350;//<-임시 패스   Original-> 27100; 
		break;

	case 27100:
		if (motor.Pcb_Motor_Move_Laser(iLaser_Pos, Task.d_Align_offset_x[PCB_Chip_MARK], Task.d_Align_offset_y[PCB_Chip_MARK], Task.d_Align_offset_th[PCB_Chip_MARK]))
		{
			Task.PCBTaskTime = myTimer(true);
			logStr.Format("Laser 변위 측정 외부 위치 이동 완료[%d]", iUseStep);
			putListLog(logStr);
			iRtnFunction = 27150;
		}
		else
		{
			logStr.Format("Laser 변위 측정 외부 위치 이동 실패[%d]", iUseStep);
			errMsg2(Task.AutoFlag, logStr);
			iRtnFunction = -27100;
		}
		break;

	case 27150:
		if (iLaser_Pos == 0)
		{
			if (!motor.PCB_Z_Motor_Move(Laser_Pcb_Pos))
			{
				logStr.Format("PCB Laser_Pcb_Pos Z 이동 실패[%d]", iUseStep);
				putListLog(logStr);
				errMsg2(Task.AutoFlag, logStr);
				iRtnFunction = -27150;
				break;
			}
			else
			{
				logStr.Format("PCB Laser_Pcb_Pos Z 이동 완료[%d]", iUseStep);
				putListLog(logStr);
			}
		}

		if (iLaser_Pos == 0) { iRtnFunction = 27160; }//처음에만 실린더 하강
		else { iRtnFunction = 27200; }//처음아니면 바로 검사로.
		Task.PCBTaskTime = myTimer(true);
		break;
		//laser 실린더 하강
	case 27160:
		if (Dio.LaserCylinderUPDOWN(false, false))
		{
			logStr.Format("LASER 실린더 하강 완료[%d]", iRtnFunction);
			theApp.MainDlg->putListLog(logStr);
			Task.PCBTaskTime = myTimer(true);
			iRtnFunction = 27170;
		}
		else
		{
			logStr.Format("LASER 실린더 하강 실패 [%d]", iRtnFunction);
			errMsg2(Task.AutoFlag, logStr);
			iRtnFunction = -27160;
		}
		break;

	case 27170:
		if (Dio.LaserCylinderCheck(false, false))
		{
			logStr.Format("LASER 실린더 하강 확인 완료[%d]", iRtnFunction);
			theApp.MainDlg->putListLog(logStr);
			iRtnFunction = 27200;

			Sleep(300);
		}
		else if ((myTimer(true) - Task.PCBTaskTime) > IO_DELAY_TIME)
		{
			logStr.Format("LASER 실린더 하강 시간 초과..[%d]", iRtnFunction);
			errMsg2(Task.AutoFlag, logStr);
			iRtnFunction = -27170;
		}
		break;
		//laser실린더 하강 END
	case 27200:
		if ((myTimer(true) - Task.PCBTaskTime) > iLaserDelay && (motor.IsStopAxis(Motor_PCB_X) && motor.IsStopAxis(Motor_PCB_Y)))
		{
			Sleep(200);
			iRtnFunction = 27250;
		}
		break;
	case 27250:

		Keyence.func_CL3000_Scan(Task.m_Laser_Point[iLaser_Pos]);
		Sleep(100);
		logStr.Format("변위센서 %lf - %d 위치[%d]", Task.m_Laser_Point[iLaser_Pos], iLaser_Pos + 1, iUseStep);
		putListLog(logStr);
		LaserPos[iLaser_Pos].x = motor.GetEncoderPos(Motor_PCB_X);
		LaserPos[iLaser_Pos].y = motor.GetEncoderPos(Motor_PCB_Y);
		LaserValue[iLaser_Pos] = Task.m_Laser_Point[iLaser_Pos];

		if (Task.m_Laser_Point[iLaser_Pos] != 0)
		{
			if (iLaser_Pos == 3)
			{
				if (theApp.MainDlg->func_Check_LaserValueErr(LaserValue) == false)
				{
					logStr.Format("Laser 측정값이상: %.04lf, %.04lf, %.04lf, %.04lf [%d]", LaserValue[0], LaserValue[1], LaserValue[2], LaserValue[3], iUseStep);
					errMsg2(Task.AutoFlag, logStr);
					iRtnFunction = -27250;
					break;
				}

				if (theApp.MainDlg->_calcLaserTilt(LaserPos, LaserValue, Task.d_Align_offset_xt[PCB_Chip_MARK], Task.d_Align_offset_yt[PCB_Chip_MARK]))
				{
					Task.m_dataOffset_x[0] = Task.d_Align_offset_xt[PCB_Chip_MARK];
					Task.m_dataOffset_y[0] = Task.d_Align_offset_yt[PCB_Chip_MARK];
					logStr.Format("	보정량 Tx: %.04lf, Ty: %.04lf", Task.d_Align_offset_xt[PCB_Chip_MARK], Task.d_Align_offset_yt[PCB_Chip_MARK]);
					putListLog(logStr);

					int Rnd = theApp.MainDlg->TiltAlignLimitCheck(Task.d_Align_offset_xt[PCB_Chip_MARK], Task.d_Align_offset_yt[PCB_Chip_MARK]);
					if (Rnd == 2)
					{
						if (Task.m_dataOffset_x[0] == 0 || Task.m_dataOffset_y[0] == 0)
						{
							logStr.Format("Laser 변위 차 값 이상 발생..Xt : %lf, Yt : %lf", Task.m_dataOffset_x[0], Task.m_dataOffset_y[0]);
							//errMsg2(Task.AutoFlag, logStr);
							//iRtnFunction = -27250;
							//break;
						}
						if (!g_ADOData.func_AATaskToRecordLaser(Task.ChipID, Task.m_dataOffset_x[0], Task.m_dataOffset_y[0], Task.m_Laser_Point))
						{
							logStr.Format("DataBase Laser 변위 측정 Data 기록 실패.[%d]\n MS Office를 닫아주세요.", iUseStep);
							errMsg2(Task.AutoFlag, logStr);
							iRtnFunction = -27250;
							break;
						}
						Task.m_PcbLaserAfterTx = motor.GetCommandPos(Motor_PCB_Xt);
						Task.m_PcbLaserAfterTy = motor.GetCommandPos(Motor_PCB_Yt);

						logStr.Format("	PCB Laser tx:%lf , ty:%lf [%d]", model.axis[Motor_PCB_Xt].pos[Laser_Pcb_Pos], model.axis[Motor_PCB_Yt].pos[Laser_Pcb_Pos], iUseStep);
						putListLog(logStr);
						logStr.Format("	PCB Laser After tx:%lf , ty:%lf [%d]", Task.m_PcbLaserAfterTx, Task.m_PcbLaserAfterTy, iUseStep);
						putListLog(logStr);
						iRtnFunction = 27310;//->Laser실린더상승, 27350;->PCB WaitPos
						iLaser_Pos++;
					}
					else if (Rnd == 1)
					{
						iRtnFunction = 27300;
					}
					else
					{
						logStr.Format("Laser Tilt  보정값 Limit를 초과 하였습니다.[%d]", iUseStep);
						errMsg2(Task.AutoFlag, logStr);
						iRtnFunction = -27250;
					}
				}
			}
			else
			{
				iRtnFunction = 27100;
				iLaser_Pos++;
			}
		}
		else
		{
			iRtnFunction = 27250;
		}
		break;
	case 27300:
		if (motor.Pcb_Move_Tilt(Task.d_Align_offset_xt[PCB_Chip_MARK], Task.d_Align_offset_yt[PCB_Chip_MARK]))
		{
			iRtnFunction = 27100;
			logStr.Format("PCB Xt : %lf, Yt : %lf [%d]", Task.d_Align_offset_xt[PCB_Chip_MARK], Task.d_Align_offset_yt[PCB_Chip_MARK], iUseStep);
			putListLog(logStr);
			iLaser_Pos = 0;
		}
		else
		{
			logStr.Format("PCB tx,ty축  위치 이동 실패[%d]", iUseStep);
			errMsg2(Task.AutoFlag, logStr);
			iRtnFunction = -27300;
		}
		break;

		//laser 실린더 상승
	case 27310:
		if (Dio.LaserCylinderUPDOWN(true, false))
		{
			logStr.Format("LASER 실린더 상승 완료[%d]", iRtnFunction);
			theApp.MainDlg->putListLog(logStr);
			Task.PCBTaskTime = myTimer(true);
			iRtnFunction = 27320;
		}
		else
		{
			logStr.Format("LASER 실린더 상승 실패 [%d]", iRtnFunction);
			errMsg2(Task.AutoFlag, logStr);
			iRtnFunction = -27310;
		}
		break;
	case 27320:
		if (Dio.LaserCylinderCheck(true, false))
		{
			logStr.Format("LASER 실린더 상승 확인 완료[%d]", iRtnFunction);
			theApp.MainDlg->putListLog(logStr);
			iRtnFunction = 27350;
			Sleep(300);
		}
		else if ((myTimer(true) - Task.PCBTaskTime) > IO_DELAY_TIME)
		{
			logStr.Format("LASER 실린더 상승 시간 초과..[%d]", iRtnFunction);
			errMsg2(Task.AutoFlag, logStr);
			iRtnFunction = -27320;
		}
		break;
		//laser실린더 상승 END

	case 27350:
		if (motor.PCB_Z_Motor_Move(Wait_Pos))
		{
			logStr.Format("PCB Z축 대기 위치 이동 완료[%d]", iUseStep);
			putListLog(logStr);
			iRtnFunction = 27600;
		}
		else
		{
			logStr.Format("PCB Z축 대기 위치 이동 실패[%d]", iUseStep);
			errMsg2(Task.AutoFlag, logStr);
			iRtnFunction = -27350;
		}

		break;
	case 27600:

		iRtnFunction = 27650;
		break;
	case 27650:
		//Sensor Align 추가
		//
		offsetX = offsetY = offsetTh = 0.0;
		iRtnFunction = 27750;
		break;
	case 27750:
		Task.m_iRetry_Opt = 0;
		iRtnFunction = 27800;
		break;
	case 27800:
		theApp.MainDlg->ctrlSubDlg(MAIN_DLG);
		theApp.MainDlg->m_iCurCamNo = 0;
		LightControlthird.ctrlLedVolume(LIGHT_PCB, model.m_iLedValue[LEDDATA_SENSOR]);		// Align만 조명 ON

		if (motor.Pcb_Motor_Move(Align_Pos))
		{
			if (motor.PCB_Z_Motor_Move(Align_Pos))
			{
				logStr.Format("PCB Sensor Align 위치 이동 완료[%d]", iUseStep);
				putListLog(logStr);
				Task.d_Align_offset_x[PCB_Chip_MARK] = 0;
				Task.d_Align_offset_y[PCB_Chip_MARK] = 0;
				Task.d_Align_offset_th[PCB_Chip_MARK] = 0;
				iRtnFunction = 27850;
				Task.PCBTaskTime = myTimer(true);
			}
		}
		else
		{
			logStr.Format("PCB Sensor Align 위치 이동 실패 [%d]", iUseStep);
			errMsg2(Task.AutoFlag, logStr);
			iRtnFunction = -27960;
		}
		break;
	case 27850:
		if ((myTimer(true) - Task.PCBTaskTime) > iCamDelay && (motor.IsStopAxis(Motor_PCB_X) && motor.IsStopAxis(Motor_PCB_Y)))
		{
			iRtnFunction = 27900;
		}
		break;
	case 27900:
		if (Task.m_iRetry_Opt>iAlignRetry)
		{
			logStr.Format("PCB Align(*Laser*) 재검사 %d 회 실패[%d]\n 보정 미적용으로 진행하시겠습니까?", Task.m_iRetry_Opt, iUseStep);
			Task.m_iRetry_Opt = 0;
			Task.d_Align_offset_x[PCB_Chip_MARK] = 0.0;
			Task.d_Align_offset_y[PCB_Chip_MARK] = 0.0;
			Task.d_Align_offset_th[PCB_Chip_MARK] = 0.0;
			if (askMsg(logStr) == IDOK)
			{
				iRtnFunction = 27960;
				logStr.Format("PCB Align 미보정 진행[%d]", iUseStep);
				putListLog(logStr);
			}
			else
			{
				iRtnFunction = 27900;
				logStr.Format("PCB Align 재검사 시작[%d]", iUseStep);
				putListLog(logStr);
				Sleep(500);
			}
			break;
		}

		offsetX = offsetY = offsetTh = 0.0;

		iRtn = theApp.MainDlg->procCamAlign(CAM2 - 1, PCB_Chip_MARK, false, offsetX, offsetY, offsetTh);

		saveInspImage(CHIP_IMAGE_SAVE, Task.m_iRetry_Opt);
		Task.m_iRetry_Opt++;
		//offsetTh=0;
		if (iRtn == 0)
		{
			Task.d_Align_offset_x[PCB_Chip_MARK] -= offsetX;
			Task.d_Align_offset_y[PCB_Chip_MARK] -= offsetY;
			Task.d_Align_offset_th[PCB_Chip_MARK] -= offsetTh;

			Task.dPcbAlignTh = Task.d_Align_offset_th[PCB_Chip_MARK];
			logStr.Format("PCB Sensor Align : %.3lf %.3lf %.3lf", Task.d_Align_offset_x[PCB_Chip_MARK], Task.d_Align_offset_y[PCB_Chip_MARK], Task.d_Align_offset_th[PCB_Chip_MARK]);
			putListLog(logStr);

			int iRtnVal = theApp.MainDlg->AlignLimitCheck(CAM2 - 1, offsetX, offsetY, offsetTh);

			if (iRtnVal == 1)
			{
				iRtnFunction = 27950;				// 보정 이동
			}
			else if (iRtnVal == 2)
			{
				logStr.Format("		PCB Sensor Align [%d] 완료 [%d]", Task.m_iRetry_Opt, iUseStep);
				putListLog(logStr);
				logStr.Format("		PCB Sensor Align Complete : %d, %.03f, %.03f, %.03f", Task.m_iPcbPickupNo, Task.d_Align_offset_x[PCB_Chip_MARK], Task.d_Align_offset_y[PCB_Chip_MARK], Task.d_Align_offset_th[PCB_Chip_MARK]);
				AlignResultSave(logStr);
				Task.m_iRetry_Opt = 0;

				Task.m_timeChecker.Measure_Time(4);	//Laser 변위 측정 시작 시간


				iRtnFunction = 27960;		// 완료
				LightControlthird.ctrlLedVolume(LIGHT_PCB, 0);		// Align만 조명 ON 
				LightControlthird.ctrlLedVolume(LIGHT_OC_6500K, 0);		// Align만 조명 ON
			}
			else
			{
				logStr.Format("PCB Sensor Align [%d] 보정 범위 초과", Task.m_iRetry_Opt, iUseStep);
				errMsg2(Task.AutoFlag, logStr);
				AlignLogSave(logStr);
				iRtnFunction = -27900;				// NG
			}
		}
		else
		{
			logStr.Format("PCB Sensor Align(*Laser*) [%d] 마크 인식 실패[%d]", Task.m_iRetry_Opt, iUseStep);
			putListLog(logStr);
			Sleep(500);
			iRtnFunction = 27900;				//  재검사 
		}
		break;
	case 27950:
		if (motor.Pcb_Holder_Align_Move(Align_Pos, Task.d_Align_offset_x[PCB_Chip_MARK], Task.d_Align_offset_y[PCB_Chip_MARK], Task.d_Align_offset_th[PCB_Chip_MARK]))
		{
			Task.PCBTaskTime = myTimer(true);
			iRtnFunction = 27850;
		}
		else
		{
			logStr.Format("PCB Sensor Align 보정 위치 이동 실패[%d]", iUseStep);
			errMsg2(Task.AutoFlag, logStr);
			iRtnFunction = -27950;
		}
		break;
	case 27960:
		if (!motor.PCB_Z_Motor_Move(Wait_Pos))
		{
			logStr.Format("PCB부 Z축 대기 위치 이동 실패[%d]", iUseStep);
			errMsg2(Task.AutoFlag, logStr);
			iRtnFunction = -27960;
			break;
		}
		else
		{
			logStr.Format("PCB부 Z축 대기 위치 이동 완료[%d]", iUseStep);
			putListLog(logStr);
			iRtnFunction = 27970;
		}

		break;
	case 27970:
		iRtnFunction = 27990;
	case 27990:
		iRtnFunction = 30000;
		break;
	default:
		logStr.Format("Pcb Thread Step 번호 비정상 .[%d]", iUseStep);
		errMsg2(Task.AutoFlag, logStr);
		iRtnFunction = -1;
		break;
	}
	return iRtnFunction;
}

int CPcbProcess1::RunProc_EpoxyNewResing(int iUseStep)
{//! Epoxy 동작 및 도포 검사 Step(17000 ~ 17999)
	int iRtnFunction = iUseStep;
	int iCamDelay = model.strInfo_Cam[1].m_iDelayTime;		//PCB 영상 획득 Delay
	int iRtn = 0;
	CString logStr = "";
	bool bChk = true;
	double offsetX = 0.0, offsetY = 0.0, offsetTh = 0.0;
	int iLaserDelay = model.strInfo_Cam[0].m_iDelayTime;	//Laser 측정전 Delay
	int iAlignRetry = model.strInfo_Cam[1].m_iRetryCnt;

	switch (iUseStep)
	{
	case 30000:
		if (!motor.PCB_Z_Motor_Move(Wait_Pos))
		{
			sLangChange.LoadStringA(IDS_STRING1012);	//PCB_Z축 대기위치 이동 실패.
			logStr.Format(sLangChange, iUseStep);
			putListLog(logStr);
			iRtnFunction = -30000;
			break;
		}

		if ((sysData.m_iDispensePass == 1) || (Task.m_bOkDispense == 1))
		{
			iRtnFunction = 34000;// 31000;	//-- 도포 Pass Mode 이거나, 도포 완료 일 경우
		}
		else
		{
			iRtnFunction = 30020;
		}
		break;
	case 30020: //Dispenser -Z축 하강
		if (motor.PCB_Z_Motor_Move(Wait_Pos))
		{
			iRtnFunction = 30030;
		}
		else
		{
			logStr.Format("PCB Z Wait 위치 이동 완료 실패 [%d]", iUseStep);
			errMsg2(Task.AutoFlag, logStr);
			iRtnFunction = -30020;
		}
		break;
	case 30030:  //도포 위치 이동
		LightControlthird.ctrlLedVolume(LIGHT_PCB, 0);		// Align 조명 OFF
		if (motor.Pcb_Motor_Move(Dispense_Pos, Task.d_Align_offset_x[PCB_Chip_MARK], Task.d_Align_offset_y[PCB_Chip_MARK], Task.d_Align_offset_th[PCB_Chip_MARK]))
		{
			sLangChange.LoadStringA(IDS_STRING935);	//PCB Dispense 보정 위치 이동 완료-보정량 X:%.03lf,Y:%.03lf,T:%.03lf
			logStr.Format(sLangChange, Task.d_Align_offset_x[PCB_Chip_MARK], Task.d_Align_offset_y[PCB_Chip_MARK], Task.d_Align_offset_th[PCB_Chip_MARK]);
			putListLog(logStr);
			iRtnFunction = 30040;
		}
		else
		{
			sLangChange.LoadStringA(IDS_STRING938);	//PCB Dispense 위치 이동 실패[%
			logStr.Format(sLangChange + _T("d]"), iUseStep);
			errMsg2(Task.AutoFlag, logStr);
			iRtnFunction = -30030;
		}
		break;
	case 30040: //Dispenser-z 축 상승
		if (motor.PCB_Z_Motor_Move(Dispense_Pos))
		{
			iRtnFunction = 30050;
		}
		else
		{
			sLangChange.LoadStringA(IDS_STRING977);	//PCB Z Dispense 위치 이동 실패
			logStr.Format(sLangChange + _T("[%d]"), iUseStep);
			errMsg2(Task.AutoFlag, logStr);
			iRtnFunction = -30040;
		}
		break;

	case 30050: // 디스펜스 동작

		if (sysData.nEpoxyIndex == CRICLE_EPOXY)
		{
			bChk = motor.func_Epoxy_CircleDraw();
		}
		else if (sysData.nEpoxyIndex == RECT_EPOXY)
		{
			bChk = motor.func_Epoxy_Draw();
		}
		else if (sysData.nEpoxyIndex == POLYGON_EPOXY)
		{
			motor.func_Epoxy_Rect_Circle_Draw();
		}
		else if (sysData.nEpoxyIndex == POINT_EPOXY)
		{
			//point 형
		}

		if (bChk)
		{
			Task.m_bOkDispense = 1;

			logStr.Format("PCB Dispense 완료[%d]", iUseStep);
			putListLog(logStr);

			iRtnFunction = 30070;
		}
		else
		{
			logStr.Format("PCB Dispense 실패 [%d]", iUseStep);
			errMsg2(Task.AutoFlag, logStr);
			iRtnFunction = -30050;
		}
		break;
	case 30070:
		if (motor.IsStopAxis(Motor_PCB_X) && motor.IsStopAxis(Motor_PCB_Y))
		{
			LightControlthird.ctrlLedVolume(LIGHT_PCB, model.m_iLedValue[LEDDATA_SENSOR]);//도포 검사전 얼라인마크용 라이트
			iRtnFunction = 30100;
		}
		break;
	case 30100:
		iRtnFunction = 31000;
		break;
	case 31000:
		if (!motor.PCB_Z_Motor_Move(Wait_Pos))
		{
			sLangChange.LoadStringA(IDS_STRING305);	//PCB부 z축 모터 위치 이동 실패[%d]
			logStr.Format(sLangChange, iUseStep);
			delayMsg(logStr.GetBuffer(99), 3000, M_COLOR_RED);
			iRtnFunction = -31000;
			break;
		}
		else
		{
			logStr.Format("PCB Z 대기 위치 이동 완료[%d]", iUseStep);
			putListLog(logStr);
		}
		iRtnFunction = 34000;
		/*if( sysData.m_iDispenseInspPass == 0 )
		{
		iRtnFunction = 32000;

		}else
		{
		iRtnFunction = 34000;
		}*/
		break;

	case 32000: // 도포검사 이동
		LightControlthird.ctrlLedVolume(LIGHT_PCB, model.m_iLedValue[LEDDATA_HOLDER]);		// Align만 조명 ON
		if (motor.Pcb_Motor_Move(Holder_Pos))
		{
			iRtnFunction = 32500;
		}
		else
		{
			sLangChange.LoadStringA(IDS_STRING989);	//PCB 도포검사 위치 이동 실패
			logStr.Format(sLangChange + _T("[%d]"), iUseStep);
			errMsg2(Task.AutoFlag, logStr);
			iRtnFunction = -32000;
		}
		break;

	case 32500:// 이동후, 일정 시간 지연 및 마크 확인
		if (motor.PCB_Z_Motor_Move(Holder_Pos))

		{
			Task.PCBTaskTime = myTimer(true);
			sLangChange.LoadStringA(IDS_STRING990);	//PCB 도포검사 위치 이동 완료[%d]
			logStr.Format(sLangChange, iUseStep);
			putListLog(logStr);
			iRtnFunction = 33000;
		}
		else
		{
			sLangChange.LoadStringA(IDS_STRING989);	//PCB 도포검사 위치 이동 실패
			logStr.Format(sLangChange + _T("[%d]"), iUseStep);
			errMsg2(Task.AutoFlag, logStr);
			iRtnFunction = -32500;
		}
		break;
	case 33000: //도포검사 z축 이동
		if ((myTimer(true) - Task.PCBTaskTime) > iCamDelay && (motor.IsStopAxis(Motor_PCB_X) && motor.IsStopAxis(Motor_PCB_Y)))
		{
			Sleep(300);
			int iMarkNo = 0;
			CString sTemp = "";

			vision.clearOverlay(CAM1);

			if (vision.getLiveMode())
			{
				vision.getSnapImage(CAM1);
				vision.setLiveMode(true);
			}

			double ep1 = 0.0;
			MappTimer(M_TIMER_RESET, &ep1);
			iMarkNo = vision.findMark(CAM1, PCB_Holder_MARK);

			double ep2 = 0.0;
			MappTimer(M_TIMER_READ, &ep2);
			sTemp.Format(" %.3lf sec", ep2 - ep1);
			vision.textlist[CAM1].addList((CAM_SIZE_X - 140), (CAM_SIZE_Y - 60), sTemp, M_COLOR_GREEN, 24, 10, "Arial");

			vision.drawOverlay(CAM1);

			if (iMarkNo != -1)
			{
#ifdef USE_GEOMETRIC
				MgraColor(M_DEFAULT, M_COLOR_GREEN);
				MmodDraw(M_DEFAULT, vision.ModResult[PCB_Holder_MARK][iMarkNo], vision.MilOverlayImage[CAM1], M_DRAW_BOX + M_DRAW_POSITION + M_DRAW_EDGES + M_DRAW_AXIS, M_DEFAULT, M_DEFAULT);

#endif
			}
			CAABonderDlg* pFrame = (CAABonderDlg*)AfxGetApp()->m_pMainWnd;
			pFrame->m_rBox.left = 0;
			pFrame->m_rBox.top = 0;
			pFrame->m_rBox.right = 0;
			pFrame->m_rBox.bottom = 0;
			iRtnFunction = 33500;
		}
		LightControlthird.ctrlLedVolume(LIGHT_PCB, model.m_iLedValue[LEDDATA_EPOXY_INSP]);
		break;
	case 33500: //도포검사                                추가해야함
		Sleep(100);
		vision.clearOverlay(CAM1);
		bChk = theApp.MainDlg->_EpoxyFinddispense(CAM1);
		vision.drawOverlay(CAM1);
		if (!bChk)
		{
			putListLog("Epoxy 도포 검사 실패");
			//logStr.Format("Epoxy Table 좌표 (X:%.03f, Y:%.03f, Θ:%.03f)", motor.GetEncoderPos(Motor_PCB_X), motor.GetEncoderPos(Motor_PCB_Y), motor.GetEncoderPos(Motor_PCB_TH));
			//putListLog(logStr);
			logStr.Format("Epoxy 도포 검사 실패. \n자동 운전 진행 하시겠습니까?");
			//_stprintf_s(motor.m_szLogMsg, "Epoxy 도포 검사 실패. \n자동 운전 진행 하시겠습니까?");

			Dio.setAlarm(ALARM_ON);
			if (askMsg(logStr))
			{
				Task.PCBTaskTime = myTimer(true);
				Dio.setAlarm(ALARM_OFF);
				Dio.setAlarm(AUTO_RUN);
			}
			else
			{
				Dio.setAlarm(AUTO_STOP);
				logStr.Format("Epoxy 도포 검사 실패[%d]", iUseStep);
				iRtnFunction = -33500;
				break;
			}
		}
		Task.m_timeChecker.Measure_Time(3);	//도포 완료 시간
		Task.m_dTime_Epoxy = Task.m_timeChecker.m_adTime[3] - Task.m_timeChecker.m_adTime[2];
		theApp.MainDlg->dispGrid();

		logStr.Format("PCB 도포검사 완료[%d]", iUseStep);
		putListLog(logStr);
		iRtnFunction = 34000;
		break;
	case 34000:
		if (!motor.PCB_Z_Motor_Move(Wait_Pos))
		{
			sLangChange.LoadStringA(IDS_STRING1012);	//PCB_Z축 대기위치 이동 실패.
			logStr.Format(sLangChange, iUseStep);
			putListLog(logStr);
			iRtnFunction = -34000;
			break;
		}
#if (____AA_WAY == PCB_TILT_AA)
		iRtnFunction = 39000;
#elif (____AA_WAY == LENS_TILT_AA)
		iRtnFunction = 35000;
#endif
		break;
	default:
		sLangChange.LoadStringA(IDS_STRING939);	//PCB Dispenser Thread Step 번호 비정상 .
		logStr.Format(sLangChange + _T(" [%d]"), iUseStep);
		errMsg2(Task.AutoFlag, logStr);
		iRtnFunction = -1;
		break;
	}

	return iRtnFunction;
}

int CPcbProcess1::RunProc_InspAAPos(int iUseStep)
{
	//! Lens Pickup에 제품 넘김 Step	(18000 ~ 19999)

	int iRtnFunction = iUseStep;
	int iRtn = 0;
	CString logStr = "";
	double	dFinalDelay = (double)(model.strInfo_Cam[3].m_iDelayTime);
	bool bChk = true;
	double offsetX = 0.0, offsetY = 0.0, offsetTh = 0.0;

	switch (iUseStep)
	{
	case 39000:
#if (____AA_WAY == PCB_TILT_AA)
		iRtnFunction = 41000;
#elif (____AA_WAY == LENS_TILT_AA)
		iRtnFunction = 40000;
#endif		
		break;
		vision.clearOverlay(CAM1);
		vision.drawOverlay(CAM1);
		//if (sysData.m_iDefectInspPass == 1)	// DEFECT 검사 PASS
		//{
		//    iRtnFunction = 39999;
		//}
		//else
		//{
		//    iRtnFunction = 39100;
		//}
		//if(model.m_IsUseBrightDefectTest == 1)	// 광원 Defect Picxel 검사 유무

		iRtnFunction = 39700;//aa때 defect Pass
		break;
	case 39100:
		LightControlthird.ctrlLedVolume(LIGHT_OC_6500K, model.m_iLedValue[LEDDATA_6500K]);
		if (!motor.Pcb_Motor_Move(OC_6500K_Pos, Task.d_Align_offset_x[PCB_Chip_MARK], Task.d_Align_offset_y[PCB_Chip_MARK], Task.d_Align_offset_th[PCB_Chip_MARK]))
		{
			logStr.Format("PCB부 모터 0C 6500K 검사 위치 이동 실패[%d]", iUseStep);
			putListLog(logStr);
			iRtnFunction = -39100;
		}
		else
		{
			iRtnFunction = 39300;
		}
		break;
	case 39300:
		if (!motor.PCB_Z_Motor_Move(OC_6500K_Pos))
		{
			logStr.Format(_T("PCB Z axis 0C 6500K Pos Move Failed [%d]"), iUseStep);
			errMsg2(Task.AutoFlag, logStr);
			iRtnFunction = -39300;
			break;
		}
		else
		{
			logStr.Format(_T("PCB Z axis 0C 6500K Pos Move Complete [%d]"), iUseStep);
			//errMsg2(Task.AutoFlag,logStr);
			iRtnFunction = 39350;
			break;
		}


	case 39350: //UV전 OC Defect 검사 
		Sleep(100);
		MIU.func_Set_InspImageCopy(MID_6500K_RAW, MIU.m_pFrameRawBuffer);
		logStr.Format("MID_6500K_RAW SAVE OK [%d]", iUseStep);
		putListLog(logStr);
		Sleep(500);

		LightControlthird.ctrlLedVolume(LIGHT_OC_6500K, 0);
		iRtnFunction = 39400;
		break;

	case 39400:
		if (!motor.PCB_Z_Motor_Move(Wait_Pos))
		{
			logStr.Format(_T("PCB Z axis Defect Pos Move Failed [%d]"), iUseStep);
			errMsg2(Task.AutoFlag, logStr);
			iRtnFunction = -39400;
			break;
		}
		else
		{
			logStr.Format(_T("PCB Z axis Defect Pos Move Complete [%d]"), iUseStep);
			//errMsg2(Task.AutoFlag,logStr);
			iRtnFunction = 39450;
			break;
		}

	case 39450:	//Dark 검사 위치 이동
		iRtnFunction = 39500;
		//if(model.m_IsUseDarkDefectTest == 1)	// 암막 Defect Picxel 검사 유무
		//{
		//	iRtnFunction = 18102;				//암막 검사 위치 이동
		//}
		//else
		//{
		//	iRtnFunction = 18115;				
		//}
		break;

	case 39500:	//Dark 검사 위치 이동
		if (!motor.Pcb_Motor_Move(Dark_Pos, Task.d_Align_offset_x[PCB_Chip_MARK], Task.d_Align_offset_y[PCB_Chip_MARK], Task.d_Align_offset_th[PCB_Chip_MARK]))
		{
			sLangChange.LoadStringA(IDS_STRING1022);	//PCB부 모터 Dark 검사 위치 이동 실패[%d]
			logStr.Format(sLangChange, iUseStep);
			putListLog(logStr);
			Task.PCBTaskTime = myTimer(true);
			iRtnFunction = -39500;
		}
		else
		{
			LightControlthird.ctrlLedVolume(LIGHT_OC_6500K, 0);
			LightControlthird.ctrlLedVolume(LIGHT_PCB, 0);
			iRtnFunction = 39600;
		}
		break;

	case 39600:
		if (!motor.PCB_Z_Motor_Move(Dark_Pos))
		{
			logStr.Format(_T("PCB Z axis Dark Pos Move Failed [%d]"), iUseStep);
			errMsg2(Task.AutoFlag, logStr);
			iRtnFunction = -39600;
		}
		else
		{
			logStr.Format(_T("PCB Z axis Dark Pos Move Complete [%d]"), iUseStep);
			//errMsg2(Task.AutoFlag,logStr);
			iRtnFunction = 39620;
		}
		break;

	case 39620:
		Sleep(500);
		MIU.func_Set_InspImageCopy(LOW_LEVEL_RAW, MIU.m_pFrameRawBuffer);
		logStr.Format("LOW_LEVEL_RAW SAVE OK [%d]", iUseStep);
		putListLog(logStr);
		iRtnFunction = 39650;
		break;
	case 39650:
		if (g_clPriInsp.func_Insp_Defect(MIU.vDefectMidBuffer_6500K, MIU.vDefectLowBuffer, false) == false)
		{
			logStr.Format("[AUTO] Defect 검사 실패 [%d]", iUseStep);
			MandoInspLog.bInspRes = false;
		}

		logStr.Format("[AUTO] Defect 검사 완료 [%d]", iUseStep);
		putListLog(logStr);
		iRtnFunction = 39700;
		break;

	case 39700:
		if (!motor.PCB_Z_Motor_Move(Wait_Pos))
		{
			logStr.Format(_T("PCB Z axis Wait Pos Move Failed [%d]"), iUseStep);
			errMsg2(Task.AutoFlag, logStr);
			iRtnFunction = -39700;
		}
		else
		{
			iRtnFunction = 39850;
		}
		break;
	case 39850:
		iRtnFunction = 39900;
		Task.PCBTaskTime = myTimer(true);
		break;
	case 39900: // 본딩 위치 이동
		iRtnFunction = 39999;
		break;
	case 39999:			//여기까지옴 180706_1
						//if( func_MIU_ConnectLiveCheck() == false)		//MIU 영상 Check
						//{
						//	iRtnFunction = -39999;
						//	break;
						//}
#if (____AA_WAY == PCB_TILT_AA)
		iRtnFunction = 41000;
#elif (____AA_WAY == LENS_TILT_AA)
		iRtnFunction = 40000;
#endif		
		break;
	default:
		logStr.Format("Pcb Align 동작 Thread Step 번호 비정상 . [%d]", iUseStep);
		errMsg2(Task.AutoFlag, logStr);
		iRtnFunction = -1;
		break;
	}
	logStr.Empty();
	return iRtnFunction;
}

int CPcbProcess1::RunProc_Bonding_Pos_GO(int iUseStep)
{
	int iRtnFunction = iUseStep;
	int iRtn = 0;
	CString logStr = "";
	double	dFinalDelay = (double)(model.strInfo_Cam[3].m_iDelayTime);
	bool bChk = true;
	double offsetX = 0.0, offsetY = 0.0, offsetTh = 0.0;

	switch (iUseStep)
	{
	case 41000:
		if (sysData.m_iIrChartUse == 1)
		{
			IrvAlignLed.Irv_Light_SetValue(model.m_iLedValue[LEDDATA_TOP1_CHART], IR_CHART);
		}
		else
		{

		}
		

		iRtnFunction = 41050;
		break;
	case 41050:
		if (!motor.PCB_Z_Motor_Move(Wait_Pos))
		{
			logStr.Format(_T("PCB Z axis Wait Pos Move Failed [%d]"), iUseStep);
			errMsg2(Task.AutoFlag, logStr);
			iRtnFunction = -41050;
		}
		else
		{
			iRtnFunction = 41100;
		}
		break;
	case 41100:
		
		iRtnFunction = 41150;
		break;
	case 41150: // 본딩 위치 이동
		Task.PCBTaskTime = myTimer(true);
		iRtnFunction = 41200;
		break;
	case 41200:
		if (motor.Lens_Motor_MoveXY(0, Bonding_Pos))
		{
#if (____AA_WAY == LENS_TILT_AA)
			if (!motor.Lens_Tilt_Motor_Move(Bonding_Pos))
			{
				logStr.Format("Lens %s Tx,Ty이동 실패", MotorPosName[Bonding_Pos]);
				delayMsg(logStr.GetBuffer(99), 3000, M_COLOR_RED);
				iRtnFunction = -41200;
				break;
			}

#endif
			logStr.Format("Lens Bonding 위치 이동 완료[%d]", iRtnFunction);
			putListLog(logStr);
			Task.interlockPcb = 1;
			iRtnFunction = 41250;
		}
		else
		{
			logStr.Format("Lens Bonding 위치 이동 실패[%d]", iRtnFunction);
			errMsg2(Task.AutoFlag, logStr);
			iRtnFunction = -41200;
			break;
		}
		break;
	case 41250:
#if (____AA_WAY == PCB_TILT_AA)
		if (motor.Pcb_Motor_Move(Bonding_Pos, Task.d_Align_offset_x[PCB_Chip_MARK], Task.d_Align_offset_y[PCB_Chip_MARK], Task.d_Align_offset_th[PCB_Chip_MARK]))
		{
			sLangChange.LoadStringA(IDS_STRING913);	//"PCB Bonding 위치 이동 완료[%d]"
			logStr.Format(sLangChange, iUseStep);
			putListLog(logStr);
			iRtnFunction = 41300;
			if (!motor.Pcb_Tilt_Motor_Move(Bonding_Pos))
			{
				logStr.Format("PCB tx,ty Bonding_Pos 이동 실패");
				delayMsg(logStr.GetBuffer(99), 3000, M_COLOR_RED);
				iRtnFunction = -41250;
				break;
			}
			break;
		}
		else
		{
			sLangChange.LoadStringA(IDS_STRING912);	//PCB Bonding 위치 이동 실패
			logStr.Format(sLangChange + _T("[%d]"), iUseStep);
			errMsg2(Task.AutoFlag, logStr);
			iRtnFunction = -41250;
			break;
		}
#elif	(____AA_WAY == LENS_TILT_AA)

		
		if (LGIT_MODEL_INDEX == M1_HEATING_GEN2)
		{
			iRtnFunction = 41300;
			break;
		}
		else
		{
			if (motor.Pcb_Motor_Move(Bonding_Pos, Task.d_Align_offset_x[PCB_Chip_MARK], Task.d_Align_offset_y[PCB_Chip_MARK], Task.d_Align_offset_th[PCB_Chip_MARK]))
			{
				sLangChange.LoadStringA(IDS_STRING913);	//"PCB Bonding 위치 이동 완료[%d]"
				logStr.Format(sLangChange, iUseStep);
				putListLog(logStr);
				iRtnFunction = 41300;	//jump
				break;
			}
			else
			{
				sLangChange.LoadStringA(IDS_STRING912);	//PCB Bonding 위치 이동 실패
				logStr.Format(sLangChange + _T("[%d]"), iUseStep);
				errMsg2(Task.AutoFlag, logStr);
				iRtnFunction = -41250;
				break;
			}
		}
		
#endif
		break;
	case 41300:
		if (sysData.m_iEpoxyLaserInspPass == 1)
		{
			//Laser 측정 Pass
			logStr.Format("Laser 보정 pass[%d]", iRtnFunction);
			putListLog(logStr);
			if (!motor.Pcb_Tilt_Motor_Move(Bonding_Pos))
			{
				logStr.Format("PCB %s 이동 실패", MotorPosName[Bonding_Pos]);
				delayMsg(logStr.GetBuffer(99), 3000, M_COLOR_RED);
				iRtnFunction = -41300;
				break;
			}
			iRtnFunction = 41310;
			break;
		}
		else
		{
			if (motor.Pcb_Tilt_Trinity_Move(Task.m_PcbLaserAfterTx, Task.m_PcbLaserAfterTy))	//Laser 보정한 값
			{
				iRtnFunction = 41310;
			}
			else
			{
				sLangChange.LoadStringA(IDS_STRING912);	//PCB Bonding 위치 이동 실패
				logStr.Format(sLangChange + _T("[%d]"), iUseStep);
				errMsg2(Task.AutoFlag, logStr);
				iRtnFunction = -41250;
			}
		}
		


		break;
	case 41310:
		if (Task.interlockPcb == 1)		//pcb틸트일 경우  렌즈 레이저,얼라인 작업 끝난후 본딩위치 올때까지 기다려야돼서
		{
			iRtnFunction = 41350;
		}
		else if ((myTimer(true) - Task.PCBTaskTime) > 50000)
		{
			logStr.Format("Lens Bonding 위치 이동 시간 초과[%d]", iUseStep);
			errMsg2(Task.AutoFlag, logStr);
			iRtnFunction = -41310;
		}
		break;
	case 41350:
		theApp.MainDlg->setCamDisplay(3, 1);
		iRtnFunction = 41400;

		break;
	case 41400:
		if (LGIT_MODEL_INDEX != M1_HEATING_GEN2)
		{
			iRtnFunction = 48000;
			break;
		}
		if (motor.Pcb_Motor_Move(Dark_Pos, Task.d_Align_offset_x[PCB_Chip_MARK], Task.d_Align_offset_y[PCB_Chip_MARK], Task.d_Align_offset_th[PCB_Chip_MARK]))
		{
			logStr.Format("PCB Tail X,Y 위치 이동 완료[%d]", iUseStep);
			putListLog(logStr);
			iRtnFunction = 41500;
		}
		else
		{
			sLangChange.LoadStringA(IDS_STRING912);	//PCB Bonding 위치 이동 실패
			logStr.Format(sLangChange + _T("[%d]"), iUseStep);
			errMsg2(Task.AutoFlag, logStr);
			iRtnFunction = -41400;
		}

		break;
	case 41500:
		if (!askMsg("진행 #1?"))
		{
			errMsg2(Task.AutoFlag, "일시정지");
			iRtnFunction = -41500;
			break;
		}
		iRtnFunction = 41600;
		break;
	case 41600:
		if (!motor.PCB_Z_Motor_Move(Dark_Pos))
		{
			logStr.Format("PCB Z축 Tail 위치 이동 실패[%d]", iRtnFunction);
			errMsg2(Task.AutoFlag, logStr);
			iRtnFunction = -41600;
			break;
		}
		else
		{
			logStr.Format("PCB Z축 Tail 위치 이동 완료[%d]", iRtnFunction);
			putListLog(logStr);
			iRtnFunction = 41700;
		}

		break;
	case 41700:
		Dio.LensTailGrip(false, false);

		Task.PCBTaskTime = myTimer(true);
		iRtnFunction = 41800;
		break;
	case 41800:
		if (Dio.LensTailGripCheck(false, false) == true)
		{
			logStr.Format("Lens Tail UnGrip 확인 완료");
			putListLog(logStr);
			iRtnFunction = 41900;
			break;
		}
		else if ((myTimer(true) - Task.PCBTaskTime) > IO_DELAY_TIME)
		{
			logStr.Format("Lens Tail UnGrip 확인 시간 초과");
			errMsg2(Task.AutoFlag, logStr);
			iRtnFunction = -41700;
			break;
		}

		break;
	case 41900:
		if (!askMsg("진행 #2?"))
		{
			errMsg2(Task.AutoFlag, "일시정지");
			iRtnFunction = -41500;
			break;
		}
		iRtnFunction = 42000;
		break;
	case 42000:
		if (motor.Pcb_Motor_Move(Bonding_Pos, Task.d_Align_offset_x[PCB_Chip_MARK], Task.d_Align_offset_y[PCB_Chip_MARK], Task.d_Align_offset_th[PCB_Chip_MARK]))
		{
			sLangChange.LoadStringA(IDS_STRING913);	//"PCB Bonding 위치 이동 완료[%d]"
			logStr.Format(sLangChange, iUseStep);
			putListLog(logStr);
			iRtnFunction = 42100;
		}
		else
		{
			sLangChange.LoadStringA(IDS_STRING912);	//PCB Bonding 위치 이동 실패
			logStr.Format(sLangChange + _T("[%d]"), iUseStep);
			errMsg2(Task.AutoFlag, logStr);
			iRtnFunction = -42000;
		}

		break;
	case 42100:
		if (!askMsg("진행 #3?"))
		{
			errMsg2(Task.AutoFlag, "일시정지");
			iRtnFunction = -41500;
			break;
		}
		iRtnFunction = 48000;
		break;
	case 48000:
		LightControlthird.ctrlLedVolume(LIGHT_PCB, 0);		// Align만 조명 ON

		logStr.Format("Loading End[%d]", iUseStep);
		putListLog(logStr);
		iRtnFunction = 49000;
		break;
		//
	default:
		logStr.Format("Pcb Align 동작 Thread Step 번호 비정상 . [%d]", iUseStep);
		errMsg2(Task.AutoFlag, logStr);
		iRtnFunction = -1;
		break;
	}
	logStr.Empty();
	return iRtnFunction;
}

int	CPcbProcess1::procAutoFocus(int iStep)
{
	int iRtn = 0;
	int iRtnFunction = 0;
	CString logStr = _T("");
	int iDelayTime = 0;

	bool limitCheckFlag = false;

	double dMovePos = 0.0;

	double dAutoZ = 0.0;

	if (iStep >= 60300 && iStep != 69999)
	{
		if (gMIUDevice.bMIUOpen == 0 || gMIUDevice.CurrentState != 4)
		{
			logStr.Format("CCD 동영상 정지 상태입니다.");
			errMsg2(Task.AutoFlag, logStr);
			return iStep;
		}
	}

	if (Task.m_bFlag_Fine_Move == false)
	{
		iDelayTime = model.strInfo_AF1.m_iDelayTime;
	}
	else
	{
		iDelayTime = model.strInfo_AF2.m_iDelayTime;
	}

	iRtnFunction = iStep;

	switch (iStep)
	{
	case 60000:
	
		MbufClear(vision.MilOptImage, 0x00);
		MbufClear(vision.MilDefectImage, 0x00);

		Task.initSFRElem();

		Task.PCBTaskTime = myTimer(true);

		theApp.MainDlg->autodispDlg->dispCurTaskOnLabel(1, "-");

		theApp.MainDlg->autodispDlg->DrawGraph(0);
		theApp.MainDlg->autodispDlg->DrawGraph(1);
		MandoInspLog.bInspRes = true;	//60000 초기화
		Task.m_timeChecker.Measure_Time(6);	//AA 시작 시간
		iRtnFunction = 60010;
		break;
	case  60010:
		if (bThreadCCDRun1 == false)		//	CCD 영상 쓰레드 멈추면 진행... step = 12000 참고
		{
			iRtnFunction = 60015;
		}
		else
		{
			logStr.Format("CCD 영상 Open 진행중입니다[%d]", iRtnFunction);
			errMsg2(Task.AutoFlag, logStr);
			iRtnFunction = -60010;
		}
		break;
	case  60015:
		if (motor.LENS_Z_Motor_Move(Bonding_Pos))
		{
			logStr.Format("Lens Z Bonding 위치 이동 완료[%d]", iStep);
			putListLog(logStr);
			iRtnFunction = 60020;

		}
		else
		{
			logStr.Format("Lens Z Bonding 위치 이동 실패 [%d]", iStep);
			putListLog(logStr);
			iRtnFunction = -60015;
		}
		break;
	case 60020:
		if (!motor.PCB_Z_Motor_Move(Bonding_Pos))
		{
			logStr.Format("PCB Z축 Bonding 위치 이동 실패[%d]", iRtnFunction);
			errMsg2(Task.AutoFlag, logStr);
			iRtnFunction = -60020;
			break;
		}
		else
		{
			iRtnFunction = 60030;
			logStr.Format("PCB Z축 Bonding 위치 이동 완료[%d]", iRtnFunction);
			putListLog(logStr);
		}

		break;
	case 60030:
	{
		double MotorPos[2];

		MotorPos[0] = fabs(motor.GetEncoderPos(theApp.MainDlg->TITLE_MOTOR_X) - model.axis[theApp.MainDlg->TITLE_MOTOR_X].pos[Bonding_Pos]);
		MotorPos[1] = fabs(motor.GetEncoderPos(theApp.MainDlg->TITLE_MOTOR_Y) - model.axis[theApp.MainDlg->TITLE_MOTOR_Y].pos[Bonding_Pos]);

		if ((MotorPos[0] < model.axis[theApp.MainDlg->TITLE_MOTOR_X].m_dLimit_Err && MotorPos[1] < model.axis[theApp.MainDlg->TITLE_MOTOR_Y].m_dLimit_Err))
		{
			logStr.Format("PCB부 x,y 본딩 위치에 도달 완료[%d]", iStep);
			putListLog(logStr);
			iRtnFunction = 60040;
			logStr.Format("	AA Step [%d]", iRtnFunction);
			putListLog(logStr);
			break;
		}
		else
		{
			logStr.Format("Bonding 위치 이동 실패[%d]", iRtnFunction);
			errMsg2(Task.AutoFlag, logStr);
			iRtnFunction = -60030;
			break;
		}
	}
	break;
	case 60040:		/* PCB Z모터 본딩위치 이동 */
	{
		iRtn = theApp.MainDlg->_checkPcbMotor();
		if (iRtn > 0)
		{
			logStr.Format("Z 모터 본딩 위치 이동");
			theApp.MainDlg->autodispDlg->dispCurTaskOnLabel(1, logStr);

			theApp.MainDlg->setCamDisplay(3, 1);
			double dCurPos = 0.0;
			double dMovePos = model.axis[Motor_Lens_Z].pos[Bonding_Pos];
			motor.MoveAxis(Motor_Lens_Z, ABS, dMovePos, sysData.fMotorSpeed[Motor_Lens_Z], sysData.fMotorAccTime[Motor_Lens_Z]);
			double ep = myTimer(true);

			while (1)
			{
				if (myTimer(true) - ep < 5000)
				{
					if (motor.IsStopAxis(Motor_Lens_Z))// && motor.GetInposition(Motor_Lens_Z) )
					{
						dCurPos = motor.GetCommandPos(Motor_Lens_Z);

						if (fabs(dCurPos - dMovePos) <= 0.003)
						{
							Task.PCBTaskTime = myTimer(true);

							logStr.Format("Auto Z Pass");
							putListLog(logStr);
							iRtnFunction = 60900;
							logStr.Format("	AA Step [%d]", iRtnFunction);
							putListLog(logStr);
							break;
						}
					}
				}
				else
				{
					putListLog("Auto Focusing 中 LENS Z Motor 이동 시간 초과.");
					iRtnFunction = -60040;
					break;
				}
			}
		}
		else
		{
			putListLog("PCB 가 본딩위치에 도달하지 못했습니다.");
			iRtnFunction = -60040;
		}
	}
	break;
	case 60070:		/* SFR 값이 연속으로 하락할 때, Z모터 이동 */
	{
		double dCurPos = 0.0;
		double dMovePos = model.axis[theApp.MainDlg->TITLE_MOTOR_Z].pos[Bonding_Pos] + model.m_dMove_Offset_Z;
		//연속으로 하락시 model.m_dMove_Offset_Z만큼 z축을 움직인다 
		if (fabs(model.m_dMove_Offset_Z) > 0.2)
		{
			logStr.Format("z축 이동치 Limit 초과 : Z : %.3lf", dMovePos);
			errMsg2(Task.AutoFlag, logStr);
			iRtnFunction = -60070;
			break;
		}
		motor.MoveAxis(theApp.MainDlg->TITLE_MOTOR_Z, ABS, dMovePos, sysData.fMotorSpeed[theApp.MainDlg->TITLE_MOTOR_Z], sysData.fMotorAccTime[theApp.MainDlg->TITLE_MOTOR_Z]);
		double ep = myTimer(true);
		Task.m_bFlag_Fine_Move = false;
		while (1)
		{
			if (myTimer(true) - ep< 5000)
			{
				if (motor.IsStopAxis(theApp.MainDlg->TITLE_MOTOR_Z))// && motor.GetInposition(TITLE_MOTOR_Z))
				{
					dCurPos = motor.GetCommandPos(theApp.MainDlg->TITLE_MOTOR_Z);
					if (fabs(dCurPos - dMovePos) <= 0.003)
					{
						Task.PCBTaskTime = myTimer(true);
						iRtnFunction = 60900;
						logStr.Format("	AA Step [%d]", iRtnFunction);
						putListLog(logStr);
						break;
					}
				}
			}
			else
			{
				putListLog("Auto Focusing 中 Z Motor 이동 시간 초과.");
				iRtnFunction = -60070;
				break;
			}
		}
	}
	break;

	case 60100:
	{
		double dMoveDist = 0.0;
		if (!Task.m_bFlag_Fine_Move)
		{
			dMoveDist = model.strInfo_AF1.m_dMoveDistance;
			logStr.Format("Through Focusing");
			theApp.MainDlg->autodispDlg->dispCurTaskOnLabel(1, logStr);
		}
		else
		{
			dMoveDist = model.strInfo_AF2.m_dMoveDistance;
			logStr.Format("Fine Focusing");
			theApp.MainDlg->autodispDlg->dispCurTaskOnLabel(1, logStr);
		}

		iRtn = theApp.MainDlg->_moveZMotor(dMoveDist, model.strInfo_AF1.m_dMoveVel);		/* Z모터 Rough 이동 */
		if (iRtn > 0) 
		{
			iRtnFunction = 61000;
			logStr.Format("	AA Step [%d]", iRtnFunction);
			putListLog(logStr);
		}
		else {
			iRtnFunction = -60100;
		}
		Task.PCBTaskTime = myTimer(true);
	}
	break;
	case 60200:
	{
		if (Task.bFirstAA)
		{
			if (sysData.m_i1stAaEbable)	//1차 AA후 종료시
			{
				iRtn = theApp.MainDlg->_getMTF(SFR_FINAL);

				if (iRtn > 0)
				{
					Task.m_iCnt_Second_AA_Start = Task.m_iCnt_Step_AA_Total;
					Task.m_iDrawBarStep = Task.m_iCnt_Step_AA_Total;
					Task.m_iCnt_Step_AA = 0;

					theApp.MainDlg->autodispDlg->DrawBarGraph();
					//MIU.func_Set_InspImageCopy(CHART_RAW);			//1차 AA후 
					Task.sfrResult = theApp.MainDlg->func_MTF(MIU.vChartBuffet);		//1차 AA후 종료
					if (Task.sfrResult)
					{
						logStr.Format("1차 AA 결과 Spec OK.");
						putListLog(logStr);
						iRtnFunction = 69999;
						logStr.Format("	AA Step [%d]", iRtnFunction);
						putListLog(logStr);
						break;
						////int boxIndex[5] = {SFR_CENTER_UP, MTF_INSP_CNT-8, MTF_INSP_CNT-6, MTF_INSP_CNT-4, MTF_INSP_CNT-2};

						//double maxCornor = Task.SFR._64_fSfrN4[boxIndex[1]];//SFR_LEFT_UP_INDEX
						//double minCornor = Task.SFR._64_fSfrN4[boxIndex[1]];//SFR_LEFT_UP_INDEX

						//for (int i=2; i<5; i++)
						//{
						//	if (maxCornor < Task.SFR._64_fSfrN4[boxIndex[i]])
						//		maxCornor = Task.SFR._64_fSfrN4[boxIndex[i]];
						//	if (minCornor > Task.SFR._64_fSfrN4[boxIndex[i]])
						//		minCornor = Task.SFR._64_fSfrN4[boxIndex[i]];
						//}

						//if( maxCornor-minCornor < sysData.dLimit_SFR_Balance)
						//{
						//	//saveSfrSumLog();//최종 Data log 저장
						//	logStr.Format("1차 AA 결과 Spec OK.");
						//	putListLog(logStr);
						//	iRtnFunction = 69999;
						//	logStr.Format("	AA Step [%d]" , iRtnFunction);
						//	putListLog(logStr);
						//	break;
						//}
					}
				}
			}
			else
			{
				Task.m_iCnt_Second_AA_Start = Task.m_iCnt_Step_AA_Total;
				Task.m_iCnt_Step_AA = 0;
			}
		}
		double dMovePos = 0.0;
#if (____AA_WAY == PCB_TILT_AA)
		dMovePos = 1 * sysData.m_dMove_MaxPos_Fine;
#elif (____AA_WAY == LENS_TILT_AA)
		dMovePos = -1 * sysData.m_dMove_MaxPos_Fine;
#endif	

		iRtn = theApp.MainDlg->_moveZMotor(dMovePos, model.strInfo_AF1.m_dMoveVel);		/* Z모터 Rough 이동 */

		if (iRtn > 0)
		{
			iRtnFunction = 60900;
			Task.PCBTaskTime = myTimer(true);
		}
		else
		{
			iRtnFunction = -60200;
		}
	}

	break;

	case 60900:			/* Z축 본딩 위치 도착 후 */
	{
		if ((myTimer(true) - Task.PCBTaskTime) > model.strInfo_Cam[CCD].m_iDelayTime)
		{
			iRtnFunction = 60910;
		}
		else
		{
			iRtnFunction = 60900;
		}
	}
	break;

	case 60910:	// AA전 Lens X/Y, PCB Theta 보정

		iRtn = theApp.MainDlg->_getMTF(SFR_FIRST/*SFR_FINAL*/);
		if (iRtn > 0)
		{
			if (!theApp.MainDlg->_calcImageAlignment())
			{
				logStr.Format("NG.  X : %.3lf Y : %.3lf", Task.m_dShift_IMG_X, Task.m_dShift_IMG_Y);
				putListLog(logStr);
				iRtnFunction = -60910;
			}
			else
			{
				if (fabs(Task.m_dShift_IMG_X) <= sysData.m_dOcSpec.x &&fabs(Task.m_dShift_IMG_Y) <= sysData.m_dOcSpec.y)
				{
					logStr.Format("		 영상 Shift. Spec(%.03lf, %.03lf) In : X %.3lf, Y %.3lf",
						sysData.m_dOcSpec.x, sysData.m_dOcSpec.y,
						Task.m_dShift_IMG_X, Task.m_dShift_IMG_Y);
					putListLog(logStr);
					iRtnFunction = 62000;
					//Task.ocCheckIng = false; 
					break;
				}
				else
				{
					logStr.Format("		 영상 Shift.  X %.3lf, Y %.3lf ,Spec(%.03lf, %.03lf)", Task.m_dShift_IMG_X, Task.m_dShift_IMG_Y, sysData.m_dOcSpec.x, sysData.m_dOcSpec.y);
					putListLog(logStr);
					iRtnFunction = 60920;
					//Task.ocCheckIng = true;
				}
			}
		}
		else
		{
			logStr.Format("SFR 영상 획득 실패.");	//SFR 영상 획득 실패.
			errMsg2(Task.AutoFlag, logStr);
			iRtnFunction = -60910;
		}
		break;

	case 60920:
		if (!theApp.MainDlg->_MotorMove_IMG_Align())
		{
			iRtnFunction = -60920;
		}
		else
		{
			iRtnFunction = 60930;
		}
		break;

	case 60930:
		if (!theApp.MainDlg->_MotorMove_IMG_AlignTheta())
		{
			iRtnFunction = -60930;
		}
		else
		{
			Sleep(200);
			iRtnFunction = 60900;
		}
		break;

	case 61000:
	{
		if ((myTimer(true) - Task.PCBTaskTime) > iDelayTime)
		{
			iRtnFunction = 62000;
			logStr.Format("	AA Step [%d]", iRtnFunction);
			putListLog(logStr);
		}
	}

	break;

	case 62000:
	{
		iRtn = theApp.MainDlg->_getMTF((Task.bFirstAA == true) ? SFR_FINAL : SFR_FIRST);
		logStr.Format("Step :[%d] SFR [%.03f] [Z %.03f]", Task.m_iCnt_Step_AA, Task.SFR.fSfrN4[Task.m_iCnt_Step_AA][0], motor.GetEncoderPos((theApp.MainDlg->TITLE_MOTOR_Z)));
		putListLog(logStr);
		if (iRtn>0)
		{
			Task.m_iCnt_Step_AA++;		/* Auto Focus rough 측정 횟수 증가 */

			if (Task.bFirstAA)
			{
				theApp.MainDlg->autodispDlg->DrawGraph(1);		/* Z 모터 위치별 SFR 데이터 그래프 그리기 */
			}
			else
			{
				theApp.MainDlg->autodispDlg->DrawGraph(0);		/* Z 모터 위치별 SFR 데이터 그래프 그리기 */
			}

			if (!Task.bFirstAA == true)
			{
				if (Task.m_iCnt_Step_AA == model.m_iCnt_Check_SFR)
				{
					iRtnFunction = 63000;
					logStr.Format("	AA Step [%d]", iRtnFunction);
					putListLog(logStr);
				}
				else if (Task.m_bFlag_Decrease_SFR == true && Task.m_iCnt_Step_AA == model.m_iCnt_Check_SFR * 2)
				{
					iRtnFunction = 63000;
					logStr.Format("	AA Step [%d]", iRtnFunction);
					putListLog(logStr);
				}
				else {
					iRtnFunction = 64000;
					logStr.Format("	AA Step [%d]", iRtnFunction);
					putListLog(logStr);
				}
			}
			else {
				iRtnFunction = 64000;
				logStr.Format("	AA Step [%d]", iRtnFunction);
				putListLog(logStr);
			}
			//////////////////////////////////////////////////////////////////////////
		}
		else {
			iRtnFunction = -63000;
		}
	}

	break;

	case 63000:		/* 초기 3개 데이터 확인 후 감소 중이면 Z축 모터 이동 */
	{
		iRtn = theApp.MainDlg->_checkDecreaseSFR();
		if (iRtn > 0) {
			iRtnFunction = 64000;
			logStr.Format("	AA Step [%d]", iRtnFunction);
			putListLog(logStr);
		}
		else
		{
			Task.m_bFlag_Decrease_SFR = true;
			theApp.MainDlg->autodispDlg->m_iCur_Index_Through = Task.m_iCnt_Step_AA_Total;

			//autodispDlg->DrawGraph(0);			//------>20180607_1
			//autodispDlg->DrawGraph(1);
			iRtnFunction = 60070;
			logStr.Format("	AA Step [%d]", iRtnFunction);
			putListLog(logStr);
		}
	}
	break;

	case 64000:
	{
		iRtn = 1;
		int iRtn2 = 1;

		iRtn = theApp.MainDlg->_checkMaxSfrPos(0);		/* MAX 값 찾기 */

		theApp.MainDlg->ccdDlg->SetAlignData_Tilt(Task.SFR.dTilt_X, Task.SFR.dTilt_Y);

		if (iRtn>0)			// && iRtn2>0)
		{
			//if (!Task.bFirstAA)
			//{  
			//	Task.SFR.dTilt_Y = _calcTiltX();//*model.Tilt_Weight_Pre;//*modelSign;//modelWeight_Pre;
			//	Task.SFR.dTilt_X = _calcTiltY();//*model.Tilt_Weight_Pre;//*modelSign;//modelWeight_Pre;
			//}else
			//{
			//	Task.SFR.dTilt_Y = _calcTiltX();//*model.Tilt_Weight_Post;//*modelSign;//modelWeight_Post;
			//	Task.SFR.dTilt_X = _calcTiltY();//*model.Tilt_Weight_Post;//*modelSign;//modelWeight_Post;
			//} 
			//
#if 0
			if (Task.bFirstAA == false)
			{
				double dCurPos = motor.GetCommandPos(TITLE_MOTOR_Z);
				double setZ = sysData.m_dOffset_Conti_Down_Z;
				if (true)//setZ < 0)
				{
					double dMovePos = setZ + dCurPos;
					logStr.Format("Z CurPos : (%.3f) , Z down : (%.3f)", dMovePos, setZ);
					putListLog(logStr);
					////
					if (fabs(setZ) > 0.5)
					{
						logStr.Format("Z Down Limit :%.3f [0.5]", fabs(setZ));
						putListLog(logStr);
						errMsg2(Task.AutoFlag, logStr);
						iRtnFunction = -64000;
						break;
					}
					motor.MoveAxis(TITLE_MOTOR_Z, ABS, dMovePos, sysData.fMotorSpeed[TITLE_MOTOR_Z], sysData.fMotorAccTime[TITLE_MOTOR_Z]);
					double ep = myTimer(true);
					while (1)
					{
						if (myTimer(true) - ep < 5000)
						{
							if (motor.IsStopAxis(TITLE_MOTOR_Z))// && motor.GetInposition(TITLE_MOTOR_Z))
							{
								dCurPos = motor.GetEncoderPos(TITLE_MOTOR_Z);
								if (fabs(dCurPos - dMovePos) <= 0.003)
								{
									Task.PCBTaskTime = myTimer(true);
									logStr.Format("Z Down 위치 이동 완료");
									putListLog(logStr);
									break;
								}
							}
						}
						else
						{
							logStr.Format("Z Down 위치 이동 시간 초과");
							putListLog(logStr);
							iRtnFunction = -64000;
							break;
						}
					}
					Sleep(600);
					//------------------------------------------------------------------------
					dCurPos = motor.GetCommandPos(TITLE_MOTOR_Z);
					setZ = sysData.m_dOffset_Conti_Down_Z * -1;
					dMovePos = setZ + dCurPos;
					logStr.Format("Z CurPos 2: (%.3f) , Z down : (%.3f)", dMovePos, setZ);
					putListLog(logStr);
					//end 1
					motor.MoveAxis(TITLE_MOTOR_Z, ABS, dMovePos, sysData.fMotorSpeed[TITLE_MOTOR_Z], sysData.fMotorAccTime[TITLE_MOTOR_Z]);
					ep = myTimer(true);
					while (1)
					{
						if (myTimer(true) - ep < 5000)
						{
							if (motor.IsStopAxis(TITLE_MOTOR_Z))// && motor.GetInposition(TITLE_MOTOR_Z))
							{
								dCurPos = motor.GetEncoderPos(TITLE_MOTOR_Z);
								if (fabs(dCurPos - dMovePos) <= 0.003)
								{
									Task.PCBTaskTime = myTimer(true);
									logStr.Format("Z Down 정위치 이동 완료");
									putListLog(logStr);
									break;
								}
							}
						}
						else
						{
							logStr.Format("Z Down 정위치 이동 시간 초과");
							putListLog(logStr);
							iRtnFunction = -64000;
							break;
						}
					}
					//end 2
				}
			}
#endif

			int mDixTx = 1;
			int mDixTy = 1;
			if (model.Tilt_Diretion[2] < 0)
			{
				mDixTx = -1;
			}
			if (model.Tilt_Diretion[3] < 0)
			{
				mDixTy = -1;
			}

			if (sysData.m_iDicChange == 0)
			{
				Task.SFR.dTilt_X = theApp.MainDlg->_calcTiltX() *  mDixTx;
				Task.SFR.dTilt_Y = theApp.MainDlg->_calcTiltY() *  mDixTy;
			}
			else
			{
				Task.SFR.dTilt_X = theApp.MainDlg->_calcTiltY() *  mDixTy;
				Task.SFR.dTilt_Y = theApp.MainDlg->_calcTiltX() *  mDixTx;
			}
			logStr.Format("TiltX : %.4f TiltY : %.4f", Task.SFR.dTilt_X, Task.SFR.dTilt_Y);
			putListLog(logStr);

			if (saveSfrLog(SFR_SECOND) == false)
			{
				iRtnFunction = -64000;
			}
			else if (fabs(Task.SFR.dTilt_X) > model.axis[theApp.MainDlg->TITLE_MOTOR_TX].m_dLimit_Err ||
				fabs(Task.SFR.dTilt_Y) > model.axis[theApp.MainDlg->TITLE_MOTOR_TY].m_dLimit_Err)
			{
				logStr.Format("Tilt 보정값 Limit 초과 : Tx : %.4f Ty : %.4f", Task.SFR.dTilt_X, Task.SFR.dTilt_Y);
				errMsg2(Task.AutoFlag, logStr);
				iRtnFunction = -64000;
			}
			else
			{
				if (Task.bFirstAA == true)
				{
					//2차
					logStr.Format("	AA Step 2차[%d]", iRtnFunction);
					putListLog(logStr);
				}
				else
				{
					//1차
					logStr.Format("	AA Step 1차[%d]", iRtnFunction);
					putListLog(logStr);
				}
				iRtnFunction = 64200;		/* Focusing 완료 */
				logStr.Format("	AA Step [%d]", iRtnFunction);
				putListLog(logStr);
			}
		}
		else {
			iRtnFunction = 60100;		/* 다시 측정 */
			logStr.Format("	AA Step [%d]", iRtnFunction);
			putListLog(logStr);
		}

		if (Task.m_iCnt_Step_AA>model.strInfo_AF1.m_iStepCnt + 5)
		{
			logStr.Format("SFR 측정 최대 횟수 초과.");
			errMsg2(Task.AutoFlag, logStr);
			iRtnFunction = -64000;	/* 카운트 초과시 에러 */
		}

		double z_move_dist = fabs(model.axis[theApp.MainDlg->TITLE_MOTOR_Z].pos[Bonding_Pos] - motor.GetCommandPos(theApp.MainDlg->TITLE_MOTOR_Z));

		if (z_move_dist > 1.0)//0.5
		{
			logStr.Format("[SFR 측정] Z 모터 하강 거리 초과 (1.0mm 이상 내릴 수 없습니다.)");
			errMsg2(Task.AutoFlag, logStr);

			iRtnFunction = -64000;
		}
	}
	break;

	case 64200:		/* Trough Focus 후 Center SFR 값 MAX 위치로 Z모터 이동 */
	{
		double dCurPos = 0.0;
		double dMovePos = Task.SFR.dMaxPos[0];//-3.970

											  //////////////////////////////////////////////////////////////////////////////////////////
											  //////////////////////////////////////////////////////////////////////////////////////////

		double z_move_dist = 0.0;
		//PCB Z춖이 움찎이므로 Tilt보정시 PCB-Z춖이 하깡 햬야함.
		//
		//2차 넘어가면서 Z축 띄우는 부분
		//Lens는 Z축 상승
		//Pcb는 Z축 하강
#if (____AA_WAY == PCB_TILT_AA)
		z_move_dist = fabs(model.axis[theApp.MainDlg->TITLE_MOTOR_Z].pos[Bonding_Pos] - dMovePos) * -1;	//1차 ->2 차이동
#elif (____AA_WAY == LENS_TILT_AA)
		z_move_dist = fabs(model.axis[theApp.MainDlg->TITLE_MOTOR_Z].pos[Bonding_Pos] - dMovePos) * 1;	//1차 ->2 차이동
#endif
		if (z_move_dist > 1.0)
		{
			logStr.Format("[SFR 측정] Z 모터 하강 거리 초과 (1.0mm 이상 내릴 수 없습니다.)");
			errMsg2(Task.AutoFlag, logStr);

			iRtnFunction = -64200;
			break;
		}

		motor.MoveAxis(theApp.MainDlg->TITLE_MOTOR_Z, ABS, dMovePos, /*model.strInfo_AF1.m_dMoveVel*/10.000, sysData.fMotorAccTime[theApp.MainDlg->TITLE_MOTOR_Z]);

		double ep = myTimer(true);

		while (1)
		{
			if (myTimer(true) - ep < 5000)
			{
				if (motor.IsStopAxis(theApp.MainDlg->TITLE_MOTOR_Z))// && motor.GetInposition(TITLE_MOTOR_Z) )
				{
					dCurPos = motor.GetCommandPos(theApp.MainDlg->TITLE_MOTOR_Z);

					if (fabs(dCurPos - dMovePos) <= 0.003)
					{
						iRtnFunction = 64400;
						logStr.Format("	AA Step [%d]", iRtnFunction);
						putListLog(logStr);
						Task.PCBTaskTime = myTimer(true);

						break;
					}
				}
			}
			else
			{
				putListLog("Auto Focusing 中 LENS Z Motor 이동 시간 초과.");
				errMsg2(Task.AutoFlag, sLangChange);
				iRtnFunction = -64200;
				break;
			}
		}
	}

	break;


	case 64400:
	{
		iRtn = theApp.MainDlg->_procTiltX();

		if (iRtn>0) {
			iRtnFunction = 64600;
			logStr.Format("	AA Step [%d]", iRtnFunction);
			putListLog(logStr);
		}
		else {
			iRtnFunction = -64400;
		}
	}

	break;

	case 64600:
	{
		iRtn = theApp.MainDlg->_procTiltY();

		if (iRtn>0) {
			iRtnFunction = 64700;
		}
		else {
			iRtnFunction = -64600;
		}
	}

	break;

	case 64700:			/* Tilt 보정 후 Delay */
		if (Task.bFirstAA == true)
		{
			if ((myTimer(true) - Task.PCBTaskTime) > model.strInfo_Cam[CCD].m_iDelayTime)
			{
				iRtnFunction = 65000;
			}
			else
			{
				iRtnFunction = 64700;
			}
		}
		else
		{
			iRtnFunction = 65000;
		}
		break;

	case 65000:			/* Tilt 후 MTF 측정 */
	{
		//Task.bFirstAA=true;
		if (Task.bFirstAA == true)//! Active Alignment 2번째 수행 
		{
			Sleep(200);
			iRtn = theApp.MainDlg->_getMTF(SFR_FINAL);

			Task.m_iDrawBarStep = Task.m_iCnt_Step_AA_Total;
			theApp.MainDlg->autodispDlg->DrawBarGraph();

			if (iRtn > 0)
			{
				if (!theApp.MainDlg->_calcImageAlignment())
				{//-- Align 보정 Limit(임의의 값 사용함...1.0)
					sLangChange.LoadStringA(IDS_STRING605);	//ImageCenter NG.  X : %.3lf Y : %.3lf
					logStr.Format(sLangChange, Task.m_dShift_IMG_X, Task.m_dShift_IMG_Y);
					putListLog(logStr);
					iRtnFunction = -65000;
					break;
				}
				else
				{
					if (Task.iMTFCnt < 6)
					{
						if (fabs(Task.m_dShift_IMG_X) > sysData.m_dOcSpec.x &&
							fabs(Task.m_dShift_IMG_Y) > sysData.m_dOcSpec.y)
						{
							logStr.Format("ImageCenter 영상 Shift.  X %.3lf, Y %.3lf", Task.m_dShift_IMG_X, Task.m_dShift_IMG_Y);
							putListLog(logStr);
							Sleep(200);
							iRtnFunction = 65500;
							logStr.Format("	AA Step [%d]", iRtnFunction);
							putListLog(logStr);
							break;
						}
						else
						{
							logStr.Format("ImageCenter 영상 Shift. Spec(%.03lf, %.03lf) In : X %.3lf, Y %.3lf", sysData.m_dOcSpec.x, sysData.m_dOcSpec.y, Task.m_dShift_IMG_X, Task.m_dShift_IMG_Y);
							putListLog(logStr);
						}
					}
				}

				bool bMtfCheck2 = true;
				vision.MilBufferUpdate();
				Sleep(500);
				MIU.func_Set_InspImageCopy(UV_BEFORE_CHART, MIU.m_pFrameRawBuffer);			//AA UV BEFORE
				Task.sfrResult = theApp.MainDlg->func_MTF(MIU.vChartBuffet);				// [AA UV BEFORE] #1 MIU.m_pFrameRawBuffer);// 

				MandoInspLog.func_LogSave_UVBefore();	// UV전 Log Data 저장

				theApp.MainDlg->autodispDlg->DrawBarGraph();
				if (!Task.sfrResult)// && bMtfCheck2)
				{
					saveInspImage(AA_NG_IMAGE_SAVE, Task.m_iCnt_Step_AA_Total);
					logStr.Format("UV전 SFR 검사 NG.  \nAA RETRY 하시겠습니까?");//logStr.Format("SFR 최종 검사 NG.  \nAA RETRY 하시겠습니까?");
					if (askMsg(logStr))
					{
						iRtnFunction = 60000;
						logStr.Format("	AA Step [%d]", iRtnFunction);
						putListLog(logStr);
						break;
					}
				}

				//saveSfrSumLog();//최종 Data log 저장

				double dCurPosX, dCurPosY, dCurPosTH, dCurPosTX, dCurPosTY, dCurPosZ;
				dCurPosX = motor.GetEncoderPos(Motor_PCB_X);
				dCurPosY = motor.GetEncoderPos(Motor_PCB_Y);
				dCurPosTH = motor.GetCommandPos(Motor_PCB_TH);
				dCurPosTX = motor.GetCommandPos(Motor_PCB_Xt);
				dCurPosTY = motor.GetCommandPos(Motor_PCB_Yt);
				dCurPosZ = motor.GetCommandPos(Motor_PCB_Z);
				if (!g_ADOData.func_AATaskToAAPcbMoter(Task.ChipID, dCurPosX, dCurPosY, dCurPosTH, dCurPosTX, dCurPosTY, dCurPosZ))
				{
					logStr.Format("DataBase 생성 실패.[%d]\n MS Office를 닫아주세요.", iStep);
					errMsg2(Task.AutoFlag, logStr);
					iRtnFunction = -69999;
					break;
				}
				iRtnFunction = 69999;
				logStr.Format("	AA Step [%d]", iRtnFunction);
				putListLog(logStr);
			}
			else
			{
				logStr.Format("SFR 최종 검사 실패.");
				errMsg2(Task.AutoFlag, logStr);
				iRtnFunction = -65000;
			}
		}
		else
		{//! Active Alignment 1번째 수행
			iRtn = theApp.MainDlg->_getMTF(SFR_FINAL);
			if (iRtn > 0)
			{
				if (!theApp.MainDlg->_calcImageAlignment())	//여기다 //수동AA
				{//-- Align 보정 Limit(임의의 값 사용함...1.0)
					sLangChange.LoadStringA(IDS_STRING848);	//NG.  X : %.3lf Y : %.3lf
					logStr.Format(sLangChange, Task.m_dShift_IMG_X, Task.m_dShift_IMG_Y);
					putListLog(logStr);

					iRtnFunction = -65000;

					break;
				}
				else
				{
					if (fabs(Task.m_dShift_IMG_X) <= sysData.m_dOcSpec.x &&
						fabs(Task.m_dShift_IMG_Y) <= sysData.m_dOcSpec.y)
					{//X/Y 보정값이 Spec In일 경우 다음 Step
						sLangChange.LoadStringA(IDS_STRING1302);	//영상 Shift. Spec(%.03lf, %.03lf) In : X %.3lf, Y %.3lf
						logStr.Format(_T("		 ") + sLangChange, sysData.m_dOcSpec.x, sysData.m_dOcSpec.y, Task.m_dShift_IMG_X, Task.m_dShift_IMG_Y);
						putListLog(logStr);
						iRtnFunction = 69999;
						logStr.Format("	AA Step [%d]", iRtnFunction);
						putListLog(logStr);
						break;
					}
					else
					{
						sLangChange.LoadStringA(IDS_STRING1301);	//영상 Shift.  X %.3lf, Y %.3lf
						logStr.Format(_T("		 ") + sLangChange, Task.m_dShift_IMG_X, Task.m_dShift_IMG_Y);
						putListLog(logStr);
						iRtnFunction = 65500;
						logStr.Format("	AA Step [%d]", iRtnFunction);
						putListLog(logStr);
					}
				}
			}
			else
			{
				sLangChange.LoadStringA(IDS_STRING1074);	//SFR 영상 획득 실패.
				errMsg2(Task.AutoFlag, sLangChange);
				iRtnFunction = -65000;
			}
		}
	}

	//iRtnFunction = -65000;
	break;

	case 65500:
		if (!theApp.MainDlg->_MotorMove_IMG_Align()) {
			iRtnFunction = -65500;
		}
		else
		{
			if (Task.iMTFCnt >= 5)
			{
				if (Task.bFirstAA == true) {
					iRtnFunction = 65520;// 65000;
					logStr.Format("	AA Step [%d]", iRtnFunction);
					putListLog(logStr);
					Task.PCBTaskTime = myTimer(true);
				}
				else
				{
					iRtnFunction = 69999;
					logStr.Format("	AA Step [%d]", iRtnFunction);
					putListLog(logStr);
				}
			}
			else
			{
				Task.iMTFCnt++;
				iRtnFunction = 65520;// 65000;
				logStr.Format("	AA Step [%d]", iRtnFunction);
				putListLog(logStr);
				Task.PCBTaskTime = myTimer(true);
			}
		}
		break;

	case 65520:
		if ((myTimer(true) - Task.PCBTaskTime) > iDelayTime)
		{
			iRtnFunction = 65000;
			logStr.Format("	AA Step [%d]", iRtnFunction);
			putListLog(logStr);
		}

		break;

	case 69000:								// 최종 SFR 검사 결과 저장..
	{									// SFR 검사 NG 발생 시 1회 재검사 후 양불 판정.
		if (myTimer(true) - Task.PCBTaskTime > model.strInfo_Cam[CCD].m_iDelayTime * 2 || (myTimer(true)<Task.PCBTaskTime))
		{
			iRtn = theApp.MainDlg->_getMTF(SFR_FINAL);		/* MTF값 획득 */

			Task.m_iDrawBarStep = Task.m_iCnt_Step_AA_Total;
			theApp.MainDlg->autodispDlg->DrawBarGraph();

			if (iRtn>0)
			{
				if (Task.m_b_AA_Retry_Flag == false)
				{
					Task.m_b_AA_Retry_Flag = true;		// TRUE인 경우 다음번에는 검사 결과 NG 시 알람 발생..
					iRtnFunction = 60000;				// 60000에서 1, 2차 AA 검사 결과 초기화..
					logStr.Format("	AA Step [%d]", iRtnFunction);
					putListLog(logStr);
				}
				else
				{
					saveInspImage(AA_NG_IMAGE_SAVE, Task.m_iCnt_Step_AA_Total);
					Dio.setAlarm(ALARM_ON);

					logStr.Format("SFR 최종 검사 NG. 자동 진행 하시겠습니까?");
					if (askMsg(sLangChange))
					{
						Dio.setAlarm(ALARM_OFF);
						logStr.Format("NG 배출 하시겠습니까. (예:NG, 아니오:OK)");
						if (askMsg(logStr)) {
							Task.m_bOkFlag = 0;
						}
						else {
							Task.m_bOkFlag = 1;
						}
						iRtnFunction = 69999;
						logStr.Format("	AA Step [%d]", iRtnFunction);
						putListLog(logStr);
					}
					else
					{
						Dio.setAlarm(ALARM_OFF);

						//Task.PauseLensStep = abs(Task.LensTask);
						Task.PausePCBStep = abs(Task.PCBTask);
						Task.AutoFlag = 2;		// 일시정지
						theApp.MainDlg->AutoRunView(Task.AutoFlag);
						iRtnFunction = -65000;
					}
				}
			}
			else
			{
				logStr.Format("SFR 최종 검사 (2차) 실패.");
				errMsg2(Task.AutoFlag, sLangChange);
				iRtnFunction = -65000;
			}
		}
	}
	break;
	case 69999:
	{
		double dCurPosX, dCurPosY, dCurPosTH, dCurPosTX, dCurPosTY, dCurPosZ;
		dCurPosX = motor.GetEncoderPos(Motor_PCB_X);
		dCurPosY = motor.GetEncoderPos(Motor_PCB_Y);
		dCurPosTH = motor.GetCommandPos(Motor_PCB_TH);
		dCurPosTX = motor.GetCommandPos(Motor_PCB_Xt);
		dCurPosTY = motor.GetCommandPos(Motor_PCB_Yt);
		dCurPosZ = motor.GetCommandPos(Motor_PCB_Z);
		//
		theApp.MainDlg->putListLog("	");
		logStr.Format("database teaching data save [%s]", Task.ChipID);
		theApp.MainDlg->putListLog("dbData/teachingData");
		theApp.MainDlg->putListLog(logStr);
		logStr.Format("AA x: %lf/%lf", dCurPosX, model.axis[Motor_PCB_X].pos[Bonding_Pos]);
		theApp.MainDlg->putListLog(logStr);
		logStr.Format("AA y: %lf/%lf", dCurPosX, model.axis[Motor_PCB_Y].pos[Bonding_Pos]);
		theApp.MainDlg->putListLog(logStr);
		logStr.Format("AA th: %lf/%lf", dCurPosX, model.axis[Motor_PCB_TH].pos[Bonding_Pos]);
		theApp.MainDlg->putListLog(logStr);
		logStr.Format("AA tx: %lf/%lf", dCurPosX, model.axis[Motor_PCB_Xt].pos[Bonding_Pos]);
		theApp.MainDlg->putListLog(logStr);
		logStr.Format("AA ty: %lf/%lf", dCurPosX, model.axis[Motor_PCB_Yt].pos[Bonding_Pos]);
		theApp.MainDlg->putListLog(logStr);
		logStr.Format("AA z: %lf/%lf", dCurPosX, model.axis[Motor_PCB_Z].pos[Bonding_Pos]);
		theApp.MainDlg->putListLog(logStr);
		//
		if (!g_ADOData.func_AATaskToAAPcbMoter(Task.ChipID, dCurPosX, dCurPosY, dCurPosTH, dCurPosTX, dCurPosTY, dCurPosZ))
		{
			logStr.Format("DataBase 생성 실패.[%d]\n MS Office를 닫아주세요.", iStep);
			errMsg2(Task.AutoFlag, logStr);
			iRtnFunction = -69999;
			break;
		}
		if (Task.bFirstAA == true)
		{
			Task.m_timeChecker.Measure_Time(7);	//AA 완료 시간
			Task.m_dTime_FineAA = Task.m_timeChecker.m_adTime[5] - Task.m_timeChecker.m_adTime[4];
			theApp.MainDlg->dispGrid();

			iRtnFunction = 110000;
			logStr.Format("	AA Step [%d]", iRtnFunction);
			putListLog(logStr);
		}
		else
		{
			//procOptical Aling으로...
			//Task.m_timeChecker.Measure_Time(4);
			//Task.m_dTime_ThroughAA = Task.m_timeChecker.m_adTime[4] - Task.m_timeChecker.m_adTime[3];

			iRtnFunction = 80000;
			logStr.Format("	AA Step [%d]", iRtnFunction);
			putListLog(logStr);
		}
	}

	break;
	}
	logStr.Empty();
	return iRtnFunction;
}

int CPcbProcess1::UV_process(int iStep)
{
	CString sLog = _T("");
	CString sImgPath = _T("");
	//	int iRtn;
	int reCnt = 0;
	int iRtnFunction = iStep;
	double curMotorDist[3];

	switch (iStep)
	{
	case 110000:

		iRtnFunction = 110500;
		break;

	case 110500:
		if (theApp.MainDlg->Bonding_Pos_Ckeck())
		{
			sLog.Format("AA after PCB Z : %f , set Offset Z : %f", motor.GetCommandPos(theApp.MainDlg->TITLE_MOTOR_Z), sysData.m_dOffset_Prev_UV_Z);	//AA후 Lens Z : %f , 설정 Offset Z : %f
			putListLog(sLog);

			theApp.MainDlg->MoveOffset_Prev_UV();	/* PCB X, Y, Z, Tx, Ty, PCB Th UV 전 offset 이동 */

			Sleep(200);
			if (motor.IsStopAxis(theApp.MainDlg->TITLE_MOTOR_Z))// && motor.GetInposition(TITLE_MOTOR_Z))
			{
				iRtnFunction = 110600;
				Task.PCBTaskTime = myTimer(true);
			}
			else
			{
				iRtnFunction = -110500;
			}
		}
		break;

	case 110600:									// 20140612 윈텍 추가.. UV 경화 전 SFR 측정.
	{

		//	sLangChange.LoadStringA(IDS_STRING864);	//Offset Z 적용 후 Lens Z : %f
		//sLog.Format(sLangChange, motor.GetEncoderPos(Motor_Lens_Z));
		//putListLog(sLog);
		Sleep(200);
		saveSfrLog(SFR_OFFSET_DATA);
		theApp.MainDlg->_getMTF(SFR_AFTER_OFFSET_MOVE);
		//saveSfrLog(SFR_OFFSET_DATA);
		iRtnFunction = 110650;
		Task.PCBTaskTime = myTimer(true);
	}
	break;

	case 110650:					//UV전 MTF 검사 진행
	{
		Task.bInsCenter = true;

		theApp.MainDlg->_calcImageAlignment();
		Task.bInsCenter = false;

		iRtnFunction = 110700;

	}
	break;


	case 110700:	// Lens Z  UV 전 offset 이동 
					// 20150730 HJH Offset 중복 적용으로 제거
					//		MoveOffset_Z();	/* Lens Z  UV 전 offset 이동 */

		iRtnFunction = 111000;
		Task.PCBTaskTime = myTimer(true);
		break;

	case 111000:	// UV 경화
	{
		sLangChange.LoadStringA(IDS_STRING1122);
		theApp.MainDlg->autodispDlg->dispCurTaskOnLabel(1, sLangChange);

		Task.PCBTaskTime = myTimer(true);
		//Task.m_timeChecker.Measure_Time(9);

		if (sysData.m_iUVPass == 1)
		{
			sLog.Format("====UV Pass [%d]====", iStep);
			putListLog(sLog);
			iRtnFunction = 113500;
			break;
		}
		UVCommand.UV_Shutter_Open();
		if (sysData.nUvIndex == 1)
		{
			Sleep(100);
			UVCommand2.UV_Shutter_Open();
		}
		Sleep(10);

		sLog.Format("===> UV ON");
		putListLog(sLog);
		Sleep(model.UV_Time);

		UVCommand.UV_Shutter_Close();
		if (sysData.nUvIndex == 1)
		{
			Sleep(100);
			UVCommand2.UV_Shutter_Close();
		}
		sLog.Format("===> UV OFF [%d]", iStep);

		putListLog(sLog);
		iRtnFunction = 113500;
	}
	break;

	case 113500:
		sLangChange.LoadStringA(IDS_STRING1124);
		theApp.MainDlg->autodispDlg->dispCurTaskOnLabel(1, sLangChange);//"UV 경화 완료"

		Task.m_timeChecker.Measure_Time(8);	//UV 완료 시간
		Task.m_dTime_UV = Task.m_timeChecker.m_adTime[8] - Task.m_timeChecker.m_adTime[7];


		if (myTimer(true) - Task.PCBTaskTime> 100)  // Delay 체크
		{
			Task.PCBTaskTime = myTimer(true);
			iRtnFunction = 115000;
		}
		break;

	case 115000:
		curMotorDist[0] = fabs(motor.GetEncoderPos(Motor_Lens_X) - model.axis[Motor_Lens_X].pos[Bonding_Pos]);
		curMotorDist[1] = fabs(motor.GetEncoderPos(Motor_Lens_Y) - model.axis[Motor_Lens_Y].pos[Bonding_Pos]);
		curMotorDist[2] = fabs(motor.GetCommandPos(Motor_Lens_Z) - model.axis[Motor_Lens_Z].pos[Bonding_Pos]);

		if ((curMotorDist[0]<model.axis[Motor_Lens_X].m_dLimit_Err && curMotorDist[1]<model.axis[Motor_Lens_Y].m_dLimit_Err && curMotorDist[2]<model.axis[Motor_Lens_Z].m_dLimit_Err))
		{
			Task.PcbOnStage = 200;			// 검사 완료.

			iRtnFunction = 115300;

			//if(!Dio.HolderGrip(false, true))				//Lens Gripper open
			//{
			//	sLog.Format("Lens Grip Open 실패 [%d]", iStep);
			//	errMsg2(Task.AutoFlag, sLog);
			//	iRtnFunction = -115000;
			//}
			//else
			//{
			//	Task.m_bOKLensPass = -1;//메인 넘김 완료 Lamp 취소
			//	Task.PCBTaskTime = myTimer(true);
			//	iRtnFunction = 115300;
			//}

		}

		break;

	case 115300:
		if (!Dio.LensMotorGrip(false, true))
		{

			sLog.Format("Lens Unit Gripper 해제 실패[%d]", iStep);
			putListLog(sLog);
			iRtnFunction = -115300;
			break;
		}
		Task.m_bOKLensPass = -1;//메인 넘김 완료 Lamp 취소
		Task.m_iFineThCnt = 0;
#if (____AA_WAY == PCB_TILT_AA)
		iRtnFunction = 115400;
#elif (____AA_WAY == LENS_TILT_AA)
		iRtnFunction = 115500;
#endif
		break;
	case 115400:
		iRtnFunction = 115450;
		/*if(Dio.CamLaserSlinderMove(false, true))
		{
		iRtnFunction = 115450;
		}
		else
		{
		sLog.Format("Laser/Cam 실린더 후진 실패[%d]", iStep);
		errMsg2(Task.AutoFlag,sLog);
		iRtnFunction = -115400;
		}*/
		break;
	case 115450:
		iRtnFunction = 115500;
		//if(Dio.CamLaserSlinderCheck(false, false))
		//{
		//	sLog.Format("Laser/Cam 실린더 후진 완료[%d]", iStep);
		//	putListLog(sLog);
		//	iRtnFunction = 115500;
		//}
		//else if((myTimer(true) - Task.PCBTaskTime) > 10000)//IO_DELAY_TIME)
		//{
		//	sLog.Format("Laser/Cam 실린더 전진 시간 초과[%d]", iStep);
		//	errMsg2(Task.AutoFlag,sLog);
		//	iRtnFunction = -115450;
		//}
		break;
	case 115500: //UV완료후 최종검사
		Task.m_bOkFlag = 0;
		work.m_iCnt_Output++;
		work.Save();
		iRtnFunction = 120000;
		theApp.MainDlg->_getMTF(SFR_AFTER_UV);
		break;


	default:
		sLangChange.LoadStringA(IDS_STRING1120);	//UV Process Step 번호가 비정상입니다.
		sLog.Format(sLangChange + _T(" [%d]"), iStep);
		errMsg2(Task.AutoFlag, sLog);
		break;
	}
	sLog.Empty();
	sImgPath.Empty();
	return iRtnFunction;
}

int	CPcbProcess1::func_MandoFinalSFR(int iStep)
{
	CString sLog = _T("");

	double	dFinalDelay = (double)(model.strInfo_Cam[3].m_iDelayTime);
	double offsetX = 0.0;
	double offsetY = 0.0;
	double offsetTh = 0.0;
	int iRtn = 0;
	int iRtnFunction = iStep;
	bool bUseDispense = true;	//도포 Pass 여부

	if ((sysData.m_iDispensePass == 1))
	{
		bUseDispense = false;//도포 Pass Mode 이거나, 도포 완료 일 경우
	}
	else
	{
		bUseDispense = true;
	}

	switch (iStep)
	{
	case 120000:

		iRtnFunction = 121000;
		break;

	case 121000:

		iRtnFunction = 122000;
		break;
	case 122000:	//완제품 검사만 해당 (Final 검사전 X/Y/T 보정)
		Sleep(dFinalDelay);
		iRtn = theApp.MainDlg->_getMTF(SFR_FINAL);
		if (iRtn > 0)
		{
			if (!theApp.MainDlg->_calcImageAlignment())	//-- Align 보정 Limit(임의의 값 사용함...1.0)
			{
				sLog.Format("NG.  X : %.3lf Y : %.3lf", Task.m_dShift_IMG_X, Task.m_dShift_IMG_Y);
				putListLog(sLog);
				iRtnFunction = -122000;
				break;
			}
			else
			{
				//틸팅값 안맞을 경우 값이 동일하게 반복됨.. 3회 정도만 하고 안맞으면 Spec IN 시키기..,
				if (Task.m_iFineThCnt > 5)	Task.m_dShift_IMG_TH = 0.0;
				if (fabs(Task.m_dShift_IMG_TH) <= sysData.dSpec_OC_Theta)//X/Y 보정값이 Spec In일 경우 다음 Step
				{
					sLog.Format("	영상 Shift. Spec In(%.03lf) In : T: %.3lf", sysData.dSpec_OC_Theta, Task.m_dShift_IMG_TH);
					putListLog(sLog);
					iRtnFunction = 122500;
				}
				else
				{
					sLog.Format("	영상 Shift(Spec : %.03lf).  T: %.3lf", sysData.dSpec_OC_Theta, Task.m_dShift_IMG_TH);
					putListLog(sLog);
					if (bUseDispense == false) //AAB, 도포 미사용일 경우->영상 Shift안하고 진행
					{
						iRtnFunction = 122500;
					}
					else//영상 Shift 진행
					{
						Task.m_iFineThCnt++;
						iRtnFunction = 122100;
					}
				}
			}
		}
		else
		{
			if (askMsg("진행?") == IDOK)
			{
				iRtnFunction = 122500;
				break;
			}
			else
			{
				errMsg2(Task.AutoFlag, "MTF 측정 전 SFR 영상 획득 실패..");
				iRtnFunction = -122000;
			}
		}
		break;

	case 122100:
		if (!theApp.MainDlg->_MotorMove_IMG_AlignTheta())
		{
			iRtnFunction = -122100;
		}
		else
		{
			Sleep(100);
			Task.PCBTaskTime = myTimer(true);
			iRtnFunction = 122200;	//Retry
		}
		break;
	case 122200:
		if ((myTimer(true) - Task.PCBTaskTime) > 300)
		{
			iRtnFunction = 122000;
			sLog.Format("	AA Step [%d]", iRtnFunction);
			putListLog(sLog);
		}
		break;
	case 122500:	//Final 재 검사 - 확인용
		theApp.MainDlg->setCamDisplay(3, 1);

		Sleep(dFinalDelay);
		MIU.func_Set_InspImageCopy(UV_AFTER_CHART, MIU.m_pFrameRawBuffer);		//AA UV AFTER
		Sleep(300);
		iRtnFunction = 122520;
		break;
	case 122520:

		Task.bInsCenter = true;

		theApp.MainDlg->_calcImageAlignment();

		Task.bInsCenter = false;

		Task.sfrResult = theApp.MainDlg->func_MTF(MIU.vChartBuffet);			// [AA UV AFTER] #1

		if (Task.sfrResult == true)
		{
			putListLog("[검사] MTF 검사 성공.");// Optic_Axis,
		}
		else
		{
			putListLog("[검사] MTF 검사 실패.");// Optic_Axis,
		}
		Task.m_iDrawBarStep = Task.m_iCnt_Step_AA_Total;
		theApp.MainDlg->autodispDlg->DrawBarGraph();
		iRtnFunction = 122550;

		break;
	case 122550:
		if (!motor.PCB_Z_Motor_Move(Wait_Pos))
		{
			sLangChange.LoadStringA(IDS_STRING305);	//PCB부 z축 모터 위치 이동 실패[%d]
			sLog.Format(sLangChange, iStep);
			putListLog(sLog);
			iRtnFunction = -122550;
			break;
		}

		if (!motor.LENS_Z_Motor_Move(Wait_Pos))
		{
			sLangChange.LoadStringA(IDS_STRING747);	//Lens부 z축 모터 위치 이동 실패[%d]
			sLog.Format(sLangChange, iStep);
			putListLog(sLog);
			iRtnFunction = -122550;
		}



		iRtnFunction = 122700;
		break;
	case 122700:
		Task.LensTaskTime = myTimer(true);
		Task.m_iRetry_Opt = 0;
		iRtnFunction = 122800;
		break;
	case 122800:
		if (sysData.m_iDefectInspPass == 1)	// 이물검사 유무
		{
			sLog.Format(_T("[PASS] Defect Insp"), iStep);
			putListLog(sLog);
			iRtnFunction = 123000;
		}
		else
		{
			//iRtnFunction = 122850;
		}
		break;
	case 122850:
		/*if (!motor.Pcb_Motor_Move(Dark_Pos))
		{
			sLog.Format("PCB부 Dark 위치 이동 실패[%d]", iStep);
			putListLog(sLog);
			iRtnFunction = -122850;
		}
		else
		{
			sLog.Format("PCB부 Dark 위치 이동 완료[%d]", iStep);
			putListLog(sLog);
			iRtnFunction = 122900;
		}*/

		break;
	case 122900:
		/*if (!motor.PCB_Z_Motor_Move(Dark_Pos))
		{
			sLog.Format("PCB부 Dark Z 위치 이동 실패[%d]", iStep);
			putListLog(sLog);
			iRtnFunction = -122900;
			break;
		}*/
		Task.PCBTaskTime = myTimer(true);
		iRtnFunction = 122950;
		break;
	case 122950:
		if ((myTimer(true) - Task.PCBTaskTime) > 500)
		{
			Task.PCBTaskTime = myTimer(true);
			iRtnFunction = 122960;
		}
		break;
	case 122960:
		Sleep(100);
		if (motor.IsStopAxis(Motor_PCB_Z))
		{
			MIU.func_Set_InspImageCopy(LOW_LEVEL_RAW, MIU.m_pFrameRawBuffer);
			iRtnFunction = 122970;
		}
		else if ((myTimer(true) - Task.PCBTaskTime) > 10000)
		{
			sLog.Format("PCB Z DAR 정지확인 시간 초과[%d]", iStep);
			errMsg2(Task.AutoFlag, sLog);
			iRtnFunction = -122900;
		}

		break;
	case 122970:
		if (!motor.PCB_Z_Motor_Move(Wait_Pos))
		{
			sLangChange.LoadStringA(IDS_STRING305);	//PCB부 z축 모터 위치 이동 실패[%d]
			sLog.Format(sLangChange, iStep);
			putListLog(sLog);
			iRtnFunction = -122970;
			break;
		}
		iRtnFunction = 123000;
		break;
	case 123000:
		if (sysData.m_iDefectInspPass == 1 && sysData.m_iStaintInspPass == 1)
		{
			iRtnFunction = 124500;
			break;
		}
		if (sysData.m_iIrChartUse == 1)
		{
			IrvAlignLed.Irv_Light_SetValue(model.m_iLedValue[LEDDATA_6500K], IR_OC);
		}
		else
		{
#ifdef FURONTEER_OC
			OCControl.SendLightLxStep(model.m_iLedValue[LEDDATA_6500K]);
#else
			if (LGIT_MODEL_INDEX == M1_TANGERINE_5M)
			{
				OcLight_Dms50v52.DMS_50V5_2_Value(1, model.m_iLedValue[LEDDATA_6500K]);
			}
			else
			{
				LightControlthird.ctrlLedVolume(LIGHT_OC_6500K, model.m_iLedValue[LEDDATA_6500K]);
			}
			
#endif

		}
		iRtnFunction = 123050;

		break;
	case 123050:
		if (!motor.Pcb_Motor_Move(OC_6500K_Pos, Task.d_Align_offset_x[PCB_Chip_MARK], Task.d_Align_offset_y[PCB_Chip_MARK], Task.d_Align_offset_th[PCB_Chip_MARK]))
		{
			sLangChange.LoadStringA(IDS_STRING1023);	//PCB부 모터 Defect 검사 위치 이동 실패[%d]
			sLog.Format(sLangChange, iStep);
			putListLog(sLog);
			iRtnFunction = -123050;
		}
		else
		{
			iRtnFunction = 123100;
		}
		break;
	case 123100:
		if (motor.PCB_Z_Motor_Move(OC_6500K_Pos))
		{
			sLog.Format("PCB Z Axis OC 6500K Pos Move Complete[%d]", iStep);
			putListLog(sLog);
			Task.PCBTaskTime = myTimer(true);
			iRtnFunction = 123200;
		}
		else
		{
			sLog.Format("PCB Z Axis OC 6500K Pos Move Fail[%d]", iStep);
			errMsg2(Task.AutoFlag, sLog);
			iRtnFunction = -123100;
		}
		break;
	case 123200:
		if ((myTimer(true) - Task.PCBTaskTime) > 500)
		{
			Task.PCBTaskTime = myTimer(true);
			iRtnFunction = 123400;
		}
		break;
	case 123400:
		if (motor.IsStopAxis(Motor_PCB_Z))
		{
			Sleep(100);
			MIU.func_Set_InspImageCopy(MID_6500K_RAW, MIU.m_pFrameRawBuffer);
			Task.PCBTaskTime = myTimer(true);
			iRtnFunction = 124000;
		}
		else if ((myTimer(true) - Task.PCBTaskTime) > 10000)
		{
			sLog.Format("PCB Z 6500K 정지확인 시간 초과[%d]", iStep);
			errMsg2(Task.AutoFlag, sLog);
			iRtnFunction = -123100;
		}
		break;
	case 124000:
		Sleep(dFinalDelay);
		if (sysData.m_iDefectInspPass == 1)	// 이물검사 유무
		{
			sLog.Format(_T("[PASS] Defect Insp"), iStep);
			putListLog(sLog);
		}
		else
		{
			if (g_clPriInsp.func_Insp_Defect(MIU.vDefectMidBuffer_6500K, MIU.vDefectLowBuffer, true) == true)	//AA
			{
				putListLog("[검사] Defect 검사 성공.");
			}
			else
			{
				sLog.Format("[검사] Defect 검사  Error[%d]", iStep);
				putListLog(sLog);
				MandoInspLog.bInspRes = false;
			}
		}
		if (sysData.m_iStaintInspPass == 1)	// 이물검사 유무
		{
			sLog.Format(_T("[PASS] Stain Insp"), iStep);
			putListLog(sLog);

		}
		else
		{
			if (g_clPriInsp.func_Insp_Stain(MIU.vDefectMidBuffer_6500K) == true)
			{
				putListLog("[검사] Stain 검사 성공.");
			}
			else
			{
				sLog.Format("[검사] Stain 검사  Error[%d]", iStep);
				putListLog(sLog);
				MandoInspLog.bInspRes = false;
			}
		}

		iRtnFunction = 124500;
		break;
	case 124500:
		MIU.Close();
		iRtnFunction = 125000;

		//if (MIU.Close())//if(MIU.Stop())
		//{
		//	iRtnFunction = 125000;
		//}
		//else
		//{
		//	iRtnFunction = 124500;

		//	if (Task.m_iRetry_Opt>3)
		//	{
		//		sLog.Format("CCD 영상 Close 실패 [%d]", iStep);	//CCD 영상 Close 실패[%d]
		//		errMsg2(Task.AutoFlag, sLog);
		//		iRtnFunction = -124500;
		//	}
		//	else {
		//		Task.m_iRetry_Opt++;
		//	}
		//}
		break;
	case 125000:
		if (motor.PCB_Z_Motor_Move(Wait_Pos))
		{
			sLog.Format("PCB Z축 대기 위치 이동 완료[%d]", iStep);
			putListLog(sLog);
			iRtnFunction = 127000;
		}
		else
		{
			sLog.Format("PCB Z축 대기 위치 이동 실패[%d]", iStep);
			sLog.Format(sLangChange, iStep);
			errMsg2(Task.AutoFlag, sLog);
			iRtnFunction = -125000;
		}
		break;
	case 127000:
		//MIU.m_acsInspImage.Unlock();
		Task.m_iRetry_Opt = 0;
		MandoInspLog.func_LogSave_UVAfter();	//만도 차량용Camera 검사 Log 저장(검사/보정량 저장 -> 최종Log)
												/*inspMES();
												MESCommunication.Messave();*/
												//! 검사 Count 처리
		work.iInspCnt_Output++;
		if (MandoInspLog.bInspRes == false)	work.iInspCnt_NG_Output++;
		theApp.MainDlg->dispInspResGrid();
		work.Save();
		work.Load();

		Task.m_timeChecker.Measure_Time(13);	//Final 검사 완료 시간
		Task.m_dTime_TotalAA = Task.m_timeChecker.m_adTime[13] - Task.m_timeChecker.m_adTime[12];
		theApp.MainDlg->dispGrid();

		iRtnFunction = 127300;
		break;
	case 127300:
		Sleep(100);
		vision.clearOverlay(CCD);
		if (MandoInspLog.bInspRes == true)
		{
			vision.textlist[CCD].addList(50, 100, "PASS", M_COLOR_GREEN, 65, 85, "Arial");
		}
		else
		{
			vision.textlist[CCD].addList(50, 100, "FAIL", M_COLOR_RED, 80, 100, "Arial");
			MandoInspLog.func_DrawNG_Overlay(CCD);	//화면 Overlay에 NG List Draw
			work.m_iCnt_NG_Output++;
		}
		vision.drawOverlay(CCD, true);

		//ccd영상 전환

		theApp.MainDlg->ctrlSubDlg(MAIN_DLG);
		theApp.MainDlg->setCamDisplay(3, 1);
		theApp.MainDlg->changeMainBtnColor(MAIN_DLG);
		iRtnFunction = 127400;
		break;

	case 127400:
		if (!motor.PCB_Z_Motor_Move(Wait_Pos))
		{
			sLog.Format("PCB Z축 대기 위치 이동 실패[%d]", iStep);
			putListLog(sLog);
			errMsg2(Task.AutoFlag, sLog);
			iRtnFunction = -127400;
			break;
		}
		iRtnFunction = 127500;
		break;
	case 127500:
		if (motor.Pcb_Motor_Move(Wait_Pos))
		{
			sLangChange.LoadStringA(IDS_STRING987);	//PCB 대기 위치 이동 완료 [%d]
			sLog.Format(sLangChange, iStep);
			putListLog(sLog);
			iRtnFunction = 127900;
		}
		else
		{
			sLangChange.LoadStringA(IDS_STRING986);	//PCB 대기 위치 이동 실패 [%d]
			sLog.Format(sLangChange, iStep);
			errMsg2(Task.AutoFlag, sLog);
			iRtnFunction = -127500;
		}
		break;
	case 127900:

		//////Dio.PcbGrip(false, false);   //언제사용?

		iRtnFunction = 128000;
		break;
	case 128000:
		if (!motor.Lens_Motor_MoveXY(0, Wait_Pos))
		{
			sLangChange.LoadStringA(IDS_STRING748);	//Lens부 모터 대기 위치 이동 실패[%d]
			sLog.Format(sLangChange, iStep);
			putListLog(sLog);
			errMsg2(Task.AutoFlag, sLog);
			iRtnFunction = -128000;
		}
		else
		{
			iRtnFunction = 128200;
		}
		break;
	case 128200:
		if (motor.Lens_Tilt_Motor_Move(Wait_Pos))
		{
			iRtnFunction = 128500;
		}
		else
		{
			sLangChange.LoadStringA(IDS_STRING745);	//LENS부 Tx, Ty축 대기 위치 이동 실패
			sLog.Format(sLangChange);
			putListLog(sLog);
			errMsg2(Task.AutoFlag, sLog);
			iRtnFunction = -128200;
		}
		break;
	case 128500:
		if (motor.Pcb_Tilt_Motor_Move(Wait_Pos))
		{
			sLog.Format("PCB TX,TY 대기 위치 이동 완료 [%d]", iStep);
			putListLog(sLog);
			iRtnFunction = 129000;
		}
		else
		{
			sLog.Format("PCB TX,TY 대기위치 이동 실패[%d]", iStep);
			errMsg2(Task.AutoFlag, sLog);
			iRtnFunction = -128500;
		}
		break;
	case 129000:
		iRtnFunction = 129500;
		break;
	case 129500:
		Task.m_bOkDispense = -1;	//메인 도포 완료 Lamp 취소
		putListLog("		");
		putListLog("		");
		sLog.Format("			END =======%s", Task.ChipID);
		putListLog(sLog);
		putListLog("		");
		putListLog("		");
		putListLog("		");
		putListLog("		");
		Task.m_bPBStart = 0;	//바코드 정보 초기화
		if (true)
		{
			memset(Task.ChipID, 0x00, 256);
			sprintf_s(Task.ChipID, "EMPTY");
			theApp.MainDlg->func_ChipID_Draw();
		}

		Task.m_timeChecker.Measure_Time(14);	//완제품 배출 시간
		Task.m_dTime_Total = Task.m_timeChecker.m_adTime[14] - Task.m_timeChecker.m_adTime[1];

		theApp.MainDlg->dispGrid();
		theApp.MainDlg->EpoxyTimeCheck(theApp.MainDlg->bEpoxyTimeChk);

		//Dio.PCBvaccumOn(VACCUM_OFF, false);


		//////////////////////////////////////////////////////////////////////////////////////////////////////
		Task.PCBTask = 10000;
		Task.LensTask = 10000;
		iRtnFunction = 10000;
		break;

	default:
		sLog.Format("Mando Test Process Step Number Error. [%d]", iStep);
		errMsg2(Task.AutoFlag, sLog);
		iRtnFunction = -iStep;
		break;
	}
	sLog.Empty();
	return iRtnFunction;
}

int CPcbProcess1::RunProc_LensNewPassPickup(int iUseStep)
{//! Lens Pickup에 제품 넘김 Step	(15000 ~ 16600)
	int iRtnFunction = iUseStep;
	int iRtn = 0;
	CString logStr = "";
	double	dFinalDelay = (double)(model.strInfo_Cam[3].m_iDelayTime);
	bool bChk = true;
	double offsetX = 0.0, offsetY = 0.0, offsetTh = 0.0;

	switch (iUseStep)
	{
	case 15000:
		if (!motor.LENS_Z_Motor_Move(Wait_Pos))
		{
			logStr.Format("Lens_Z축 대위치 이동 실패");
			delayMsg(logStr.GetBuffer(99), 3000, M_COLOR_RED);
			iRtnFunction = -15000;
			break;
		}

		if (!motor.PCB_Z_Motor_Move(Wait_Pos))
		{
			logStr.Format("PCB Z축 대기위치 이동 실패[%d]", iUseStep);
			delayMsg(logStr.GetBuffer(99), 3000, M_COLOR_RED);
			iRtnFunction = -15000;
			break;
		}

		if (Task.m_bOKLensPass != 1)//! Lens 넘김 미완료 일 경우	 
		{
			if (!motor.Pcb_Motor_Move(Loading_Pos, Task.d_Align_offset_x[PCB_Holder_MARK], Task.d_Align_offset_y[PCB_Holder_MARK], 0))
			{
				logStr.Format("PCB %s 이동 실패", MotorPosName[Loading_Pos]);
				delayMsg(logStr.GetBuffer(99), 3000, M_COLOR_RED);
				iRtnFunction = -15000;
				break;
			}
			iRtnFunction = 15100;
		}
		else
		{
			iRtnFunction = 15500;
		}
		break;
	case 15100:
		if (!Dio.LensMotorGrip(false, false))
		{
			logStr.Format("Lens Unit Gripper 해제 실패[%d]", iUseStep);
			putListLog(logStr);
			iRtnFunction = -15100;
			break;
		}
		Task.PCBTaskTime = myTimer(true);
		iRtnFunction = 15150;
		break;

	case 15150:
		if (Dio.LensMotorGripCheck(false, false))
		{
			iRtnFunction = 15200;
		}
		else if ((myTimer(true) - Task.PCBTaskTime) > IO_DELAY_TIME)
		{
			iRtnFunction = -15150;
		}
		break;

	case 15200: // 공급
		if (!motor.Lens_Tilt_Motor_Move(Loading_Pos))
		{
			logStr.Format("Lens %s Tx,Ty이동 실패", MotorPosName[Loading_Pos]);
			delayMsg(logStr.GetBuffer(99), 3000, M_COLOR_RED);
			iRtnFunction = -15200;
			break;
		}
		if (!motor.Lens_Motor_MoveXY(0, Loading_Pos))
		{
			logStr.Format("Lens %s X,Y 이동 실패", MotorPosName[Loading_Pos]);
			delayMsg(logStr.GetBuffer(99), 3000, M_COLOR_RED);
			iRtnFunction = -15200;
			break;
		}
		logStr.Format("Lens 공급 위치 이동 완료[%d]", iUseStep);
		putListLog(logStr);
		iRtnFunction = 15250;
		break;

	case 15250:
		if (!motor.PCB_Z_Motor_Move(Loading_Pos))
		{
			logStr.Format("PCB loading Z 이동 실패");
			errMsg2(Task.AutoFlag, logStr);
			iRtnFunction = -15250;
		}
		else
		{
			iRtnFunction = 15252;
		}
		break;
	case 15252:
		if (motor.LENS_Z_Motor_Move(Loading_Pos))
		{
			iRtnFunction = 15255;
		}
		else
		{
			logStr.Format("Lens Z 위치 이동 실패 [%d]", iUseStep);
			errMsg2(Task.AutoFlag, logStr);
			iRtnFunction = -15252;
		}
		break;

	case 15255:
		iRtnFunction = 15300;
		break;
	case 15300: // 모터 그립
		if (Dio.LensMotorGrip(true, false))
		{
			Task.m_bOKLensPass = 1;
			Task.PCBTaskTime = myTimer(true);
			iRtnFunction = 15350;
			Sleep(500);
			Dio.LensMotorGrip(false, false);
			Sleep(500);
			Dio.LensMotorGrip(true, false);
		}
		else
		{
			Task.m_bOKLensPass = -1;
			logStr.Format("Lens 그립 실패 [%d]", iUseStep);
			errMsg2(Task.AutoFlag, logStr);
			iRtnFunction = -15300;
		}
		break;

	case 15350://모터 그립 확인
		if (Dio.LensMotorGripCheck(true, false))
		{
			logStr.Format("Lens 그립 완료[%d]", iUseStep);
			putListLog(logStr);
			iRtnFunction = 15400;

			Sleep(800);
		}
		else if ((myTimer(true) - Task.PCBTaskTime) > IO_DELAY_TIME)
		{
			logStr.Format("Lens Gripper 전진 센서 시간 초과..[%d]", iUseStep);
			errMsg2(Task.AutoFlag, logStr);
			iRtnFunction = -15350;
		}
		break;
	case 15400:
		iRtnFunction = 15450;
		//Sleep(400);	//실린더 사용으로 PCB Stage UnGrip시 센서 감지 후, Delay
		/*if(Dio.HolderGrip(false,false))
		{
		Task.PCBTaskTime = myTimer(true);
		iRtnFunction = 15450;
		}
		else
		{
		logStr.Format("Lens Gripper 해제 실패..[%d]", iUseStep);
		errMsg2(Task.AutoFlag,logStr);

		iRtnFunction = -15400;
		}*/
		break;

	case 15450:
		iRtnFunction = 15452;
		/*if(Dio.HolderGripCheck(false, false) )
		{
		iRtnFunction = 15452;
		}
		else if((myTimer(true) - Task.PCBTaskTime) > IO_DELAY_TIME)
		{
		logStr.Format("PCB Stage Lens Gripper 해제 실패..[%d]", iUseStep);
		errMsg2(Task.AutoFlag,logStr);

		iRtnFunction = -15450;
		}*/
		//Sleep(600);
		break;
	case 15452:

		iRtnFunction = 15455;
		break;

	case 15455:

		iRtnFunction = 15500;
		break;
	case 15500: // 대기 위치 이동		//렌즈 그립상태에서 시작하면 이리 온다 수정하지마

		if (!motor.LENS_Z_Motor_Move(Wait_Pos))
		{
			logStr.Format("Lens Z 대기 위치 이동 실패 [%d]", iUseStep);
			errMsg2(Task.AutoFlag, logStr);
			iRtnFunction = -15500;
		}
		//
		if (!motor.PCB_Z_Motor_Move(Wait_Pos))
		{
			logStr.Format("PCB waiting Z 이동 실패");
			errMsg2(Task.AutoFlag, logStr);
			iRtnFunction = -15500;
		}
		iRtnFunction = 15550;
		break;
	case 15550:
		if (motor.Pcb_Motor_Move(Wait_Pos))			//PCB부 모터 대기위치 이동
		{
			iRtnFunction = 15800;
			logStr.Format("Pcb x,y 대기 위치 이동 완료 [%d]", iUseStep);
			putListLog(logStr);
		}
		else
		{
			logStr.Format("Pcb x,y 대기 위치 이동 실패 [%d]", iUseStep);
			putListLog(logStr);
			iRtnFunction = -15550;
		}
		break;
	case 15800:

		Task.LensTask = 30000;	//pcb AA일때 lens 는 Align 이동후 laser
		iRtnFunction = 15900;
		logStr.Format("PCB Epoxy Waiting[%d]", iUseStep);
		putListLog(logStr);
		break;
	case 15900:
		//pcb 는 
		//lens가 Align , laser마치고 
		//본딩위치로 빠지기 전까지 대기 위치로가서 대기해야된다.

		if (Task.interlockLens == 1)
		{
			Task.interlockLens = 0;
			iRtnFunction = 26000;
		}
		break;
	default:
		logStr.Format("Lens 공급 동작 Thread Step 번호 비정상 . [%d]", iUseStep);
		errMsg2(Task.AutoFlag, logStr);
		iRtnFunction = -1;
		break;
	}

	return iRtnFunction;
}

int	CPcbProcess1::RunProc_LensAlign(int iUseStep)			//(35000 ~ 39000)
{
	int iRtnFunction = iUseStep;
	int iRtn = 0;
	CString logStr = "";
	bool bChk = true;
	double offsetX = 0.0, offsetY = 0.0, offsetTh = 0.0;
	int iCamDelay = model.strInfo_Cam[1].m_iDelayTime;	//PCB 영상 획득 Delay
	int iAlignRetry = model.strInfo_Cam[1].m_iRetryCnt;
	int iLaserDelay = model.strInfo_Cam[0].m_iDelayTime;	//Laser 측정전 Delay
	switch (iUseStep)
	{
	case 35000:
		checkMessage();
		offsetX = offsetY = offsetTh = 0.0;
		if (!motor.PCB_Z_Motor_Move(Wait_Pos))
		{
			logStr.Format("		PCB Z축 대기위치 이동 실패[%d]", iUseStep);
			delayMsg(logStr.GetBuffer(99), 3000, M_COLOR_RED);
			iRtnFunction = -35000;
			break;
		}
		iRtnFunction = 35050;
		break;
	case 35050:
		Task.m_iRetry_Opt = 0;
		iRtnFunction = 35100;
		break;

	case 35100:
		Task.m_bOKLensPass = -1;//PCB언로드 과정에서 넘김상태로 만들어놓으므로 초기화.
		Task.d_Align_offset_x[LENS_Align_MARK] = 0;
		Task.d_Align_offset_y[LENS_Align_MARK] = 0;
		Task.d_Align_offset_th[LENS_Align_MARK] = 0;


		if (Task.m_bOKLensPass != 1)//! Lens 넘김 미완료 일 경우	 
		{
			iRtnFunction = 35150;
		}
		else
		{
			logStr.Format("Lens 넘김 완료로 Lens Align Pass![%d]", iUseStep);
			putListLog(logStr);
			iRtnFunction = 35400;
		}
		break;

	case 35150:
		theApp.MainDlg->ctrlSubDlg(MAIN_DLG);
		theApp.MainDlg->m_iCurCamNo = 0;
		//SetDigReference(LENS_Align_MARK);
		LightControlthird.ctrlLedVolume(LIGHT_PCB, model.m_iLedValue[LEDDATA_LENS]);		// Align만 조명 ON
		if (motor.Pcb_Motor_Move(Lens_Pos))
		{
			if (motor.PCB_Z_Motor_Move(Lens_Pos))
			{
				logStr.Format("PCB Lens_Pos Align 위치 이동 완료[%d]", iUseStep);
				putListLog(logStr);
				Task.PCBTaskTime = myTimer(true);

				iRtnFunction = 35200;
			}
		}
		else
		{
			logStr.Format("PCB Sensor Align 위치 이동 실패 [%d]", iUseStep);
			errMsg2(Task.AutoFlag, logStr);
			iRtnFunction = -35150;
		}
		break;
	case 35200:
		checkMessage();
		if ((myTimer(true) - Task.PCBTaskTime) > iCamDelay && (motor.IsStopAxis(Motor_PCB_X) && motor.IsStopAxis(Motor_PCB_Y)))
		{
			iRtnFunction = 35250;
		}
		break;

	case 35250:

		if (Task.m_iRetry_Opt>iAlignRetry)
		{
			logStr.Format("PCB Align 재검사 %d 회 실패[%d]\n 보정 미적용으로 진행하시겠습니까?", Task.m_iRetry_Opt, iUseStep);
			if (askMsg(logStr) == IDOK)
			{
				Task.d_Align_offset_x[LENS_Align_MARK] = 0;
				Task.d_Align_offset_y[LENS_Align_MARK] = 0;
				Task.d_Align_offset_th[LENS_Align_MARK] = 0;
				iRtnFunction = 35400;
			}
			else
			{
				Task.m_iRetry_Opt = 0;
				iRtnFunction = 35250;
				logStr.Format("PCB Lens Align 재검사 시작[%d]", iUseStep);
				putListLog(logStr);

				//iRtnFunction = -35250;
				//logStr.Format("PCB Align 재검사 %d 회 실패[%d]", Task.m_iRetry_Opt, iUseStep);
				//errMsg2(Task.AutoFlag,logStr);
			}
			break;
		}

		offsetX = offsetY = offsetTh = 0.0;

		iRtn = theApp.MainDlg->procCamAlign(CAM1, LENS_Align_MARK, false, offsetX, offsetY, offsetTh);
		saveInspImage(LENS_IMAGE_SAVE, Task.m_iRetry_Opt);
		Task.m_iRetry_Opt++;
		offsetTh = 0;
		if (iRtn == 0)
		{
			Task.d_Align_offset_x[LENS_Align_MARK] -= offsetX;
			Task.d_Align_offset_y[LENS_Align_MARK] -= offsetY;
			Task.d_Align_offset_th[LENS_Align_MARK] = 0.0;//offsetTh;

			Task.dPcbAlignTh = Task.d_Align_offset_th[LENS_Align_MARK];

			logStr.Format("PCB Lens Align : %.3lf %.3lf %.3lf", Task.d_Align_offset_x[LENS_Align_MARK], Task.d_Align_offset_y[LENS_Align_MARK], Task.d_Align_offset_th[LENS_Align_MARK]);
			//pcbDlg->m_labelAlignResult.SetText(logStr);
			//pcbDlg->m_labelAlignResult.Invalidate();

			putListLog(logStr);

			int iRtnVal = theApp.MainDlg->AlignLimitCheck(CAM1, offsetX, offsetY, offsetTh);

			if (iRtnVal == 1)
			{
				iRtnFunction = 35300;				// 보정 이동
			}
			else if (iRtnVal == 2)
			{
				logStr.Format("		PCB Lens Align [%d] 완료 [%d]", Task.m_iRetry_Opt, iUseStep);
				putListLog(logStr);
				//InstantMarkDelete(PCB_Chip_MARK);			//임시 등록 마크 삭제

				logStr.Format("		PCB Lens Align Complete : %d, %.03f, %.03f, %.03f", Task.m_iPcbPickupNo, Task.d_Align_offset_x[PCB_Chip_MARK], Task.d_Align_offset_y[PCB_Chip_MARK], Task.d_Align_offset_th[PCB_Chip_MARK]);
				AlignResultSave(logStr);
				Task.m_iRetry_Opt = 0;

				Task.m_timeChecker.Measure_Time(4);	//Laser 변위 측정 시작 시간

				iRtnFunction = 35400;		// 완료
			}
			else
			{
				logStr.Format("PCB Lens Align [%d] 보정 범위 초과", Task.m_iRetry_Opt, iUseStep);
				errMsg2(Task.AutoFlag, logStr);
				AlignLogSave(logStr);
				//InstantMarkDelete(PCB_Chip_MARK);
				iRtnFunction = -35250;				// NG
			}
		}
		else
		{
			logStr.Format("PCB Lens Align [%d] 마크 인식 실패[%d]", Task.m_iRetry_Opt, iUseStep);
			putListLog(logStr);

			iRtnFunction = 35200;				//  재검사 
			Sleep(300);
		}
		break;

	case 35300://- Sensor Align Retry
		if (motor.Pcb_Holder_Align_Move(Lens_Pos, Task.d_Align_offset_x[LENS_Align_MARK], Task.d_Align_offset_y[LENS_Align_MARK], Task.d_Align_offset_th[LENS_Align_MARK]))
		{
			Task.PCBTaskTime = myTimer(true);
			iRtnFunction = 35200;
		}
		else
		{
			logStr.Format("PCB Sensor Align 보정 위치 이동 실패[%d]", iUseStep);
			errMsg2(Task.AutoFlag, logStr);
			iRtnFunction = -35300;
		}
		break;
	case 35400:
		if (!motor.PCB_Z_Motor_Move(Wait_Pos))
		{
			sLangChange.LoadStringA(IDS_STRING305);	//PCB부 z축 모터 위치 이동 실패[%d]
			logStr.Format(sLangChange, iUseStep);
			delayMsg(logStr.GetBuffer(99), 3000, M_COLOR_RED);
			iRtnFunction = -35400;
			break;
		}
		iRtnFunction = 35500;
		break;
	case 35500://! Lens-Z축 대기 위치 이동
		iRtnFunction = 39000;
		break;
	default:
		logStr.Format("Lens 외부 Align Thread Step 번호 비정상 [%d]", iUseStep);
		errMsg2(Task.AutoFlag, logStr);
		iRtnFunction = -1;
		break;
	}
	logStr.Empty();
	return iRtnFunction;
}

int CPcbProcess1::RunProc_LensLoading(int iUseStep)
{//! Lens Pickup에 제품 넘김 Step	(15000 ~ 16600)
	int iRtnFunction = iUseStep;
	int iRtn = 0;
	CString logStr = "";
	double	dFinalDelay = (double)(model.strInfo_Cam[3].m_iDelayTime);
	bool bChk = true;
	double offsetX = 0.0, offsetY = 0.0, offsetTh = 0.0;
	//Task.m_bOKLensPass = -1;//PCB언로드 과정에서 넘김상태로 만들어놓으므로 초기화.

	switch (iUseStep)
	{
	case 40000:
		if (!motor.LENS_Z_Motor_Move(Wait_Pos))
		{
			logStr.Format("Lens_Z축 대위치 이동 실패");
			delayMsg(logStr.GetBuffer(99), 3000, M_COLOR_RED);
			iRtnFunction = -40000;
			break;
		}

		if (!motor.PCB_Z_Motor_Move(Wait_Pos))
		{
			logStr.Format("PCB Z축 대기위치 이동 실패[%d]", iUseStep);
			delayMsg(logStr.GetBuffer(99), 3000, M_COLOR_RED);
			iRtnFunction = -40000;
			break;
		}

		if (Task.m_bOKLensPass != 1)//! Lens 넘김 미완료 일 경우	 
		{

			Task.d_Align_offset_x[LENS_Align_MARK] = 0.0;
			Task.d_Align_offset_y[LENS_Align_MARK] = 0.0;
			if (!motor.Pcb_Motor_Move(Loading_Pos, Task.d_Align_offset_x[LENS_Align_MARK], Task.d_Align_offset_y[LENS_Align_MARK], 0))
			{
				logStr.Format("PCB %s 이동 실패", MotorPosName[Loading_Pos]);
				delayMsg(logStr.GetBuffer(99), 3000, M_COLOR_RED);
				iRtnFunction = -40000;
				break;
			}
			//M_TRINITY 모델은 레이저후 틸트 심해서 본딩위치갈때 다시 레이저 틸트된 tx,ty 값으로 이동 
			if (!motor.Pcb_Tilt_Motor_Move(Loading_Pos))
			{
				logStr.Format("PCB %s 이동 실패", MotorPosName[Loading_Pos]);
				delayMsg(logStr.GetBuffer(99), 3000, M_COLOR_RED);
				iRtnFunction = -40000;
				break;
			}
			iRtnFunction = 40100;
		}
		else
		{
			iRtnFunction = 40500;
		}
		break;
	case 40100:
		if (!Dio.LensMotorGrip(false, false))
		{
			logStr.Format("Lens Unit Gripper 해제 실패[%d]", iUseStep);
			putListLog(logStr);
			iRtnFunction = -40100;
			break;
		}
		if (LGIT_MODEL_INDEX == M1_HEATING_GEN2)
		{
			Dio.LensTailGrip(false, false);
		}
		Task.PCBTaskTime = myTimer(true);
		iRtnFunction = 40150;
		break;

	case 40150:
		if (Dio.LensMotorGripCheck(false, false)) 
		{
			iRtnFunction = 40170;
			Task.PCBTaskTime = myTimer(true);
		}
		else if ((myTimer(true) - Task.PCBTaskTime) > IO_DELAY_TIME)
		{
			logStr.Format("Lens UnGrip 확인 시간 초과");
			errMsg2(Task.AutoFlag, logStr);
			iRtnFunction = -40150;
			break;
		}
		break;
	case 40170:
		if (LGIT_MODEL_INDEX == M1_HEATING_GEN2)
		{
			if (Dio.LensTailGripCheck(false, false))
			{
				iRtnFunction = 40200;
				Task.PCBTaskTime = myTimer(true);
			}
			else if ((myTimer(true) - Task.PCBTaskTime) > IO_DELAY_TIME)
			{
				logStr.Format("Lens Tail UnGrip 확인 시간 초과");
				errMsg2(Task.AutoFlag, logStr);
				iRtnFunction = -40170;
				break;
			}
		}

		else
		{
			iRtnFunction = 40200;
		}
		
		break;

	case 40200: // 공급 
		if (!motor.Lens_Tilt_Motor_Move(Loading_Pos))
		{
			logStr.Format("Lens %s Tx,Ty이동 실패", MotorPosName[Loading_Pos]);
			delayMsg(logStr.GetBuffer(99), 3000, M_COLOR_RED);
			iRtnFunction = -40200;
			break;
		}
		if (!motor.Lens_Motor_MoveXY(0, Loading_Pos))
		{
			logStr.Format("Lens %s X,Y 이동 실패", MotorPosName[Loading_Pos]);
			delayMsg(logStr.GetBuffer(99), 3000, M_COLOR_RED);
			iRtnFunction = -40200;
			break;
		}

		logStr.Format("Lens 공급 위치 X,Y 이동 완료[%d]", iUseStep);
		putListLog(logStr);
		iRtnFunction = 40250;
		break;

	case 40250:
		if (!motor.PCB_Z_Motor_Move(Loading_Pos))
		{
			logStr.Format("PCB loading Z 이동 실패");
			errMsg2(Task.AutoFlag, logStr);
			iRtnFunction = -40250;
			break;
		}
		if (!motor.LENS_Z_Motor_Move(Loading_Pos))
		{
			logStr.Format("Lens Z 위치 이동 실패 [%d]", iUseStep);
			errMsg2(Task.AutoFlag, logStr);
			iRtnFunction = -40250;
			break;
		}
		iRtnFunction = 40252;
		break;
	case 40252:
		Dio.LensMotorGrip(true, false);
		Sleep(500);
		iRtnFunction = 40255;
		break;

	case 40255:
		Dio.LensMotorGrip(false, false);
		Sleep(300);
		iRtnFunction = 40300;
		break;
	case 40300: // 모터 그립
		if (Dio.LensMotorGrip(true, false))
		{
			Task.m_bOKLensPass = 1;
			Task.PCBTaskTime = myTimer(true);
			iRtnFunction = 40350;
		}
		else
		{
			Task.m_bOKLensPass = -1;
			logStr.Format("Lens 그립 실패 [%d]", iUseStep);
			errMsg2(Task.AutoFlag, logStr);
			iRtnFunction = -40300;
		}
		break;

	case 40350://모터 그립 확인
		if (Dio.LensMotorGripCheck(true, false))
		{
			logStr.Format("Lens 그립 완료[%d]", iUseStep);
			putListLog(logStr);
			iRtnFunction = 40400;
		}
		else if ((myTimer(true) - Task.PCBTaskTime) > IO_DELAY_TIME)
		{
			logStr.Format("Lens Gripper 전진 센서 시간 초과..[%d]", iUseStep);
			errMsg2(Task.AutoFlag, logStr);
			iRtnFunction = -40350;
		}
		break;

	case 40400:
		Sleep(500);	//실린더 사용으로 PCB Stage UnGrip시 센서 감지 후, Delay 0.2s
		iRtnFunction = 40455;
		break;

	case 40455:
		iRtnFunction = 40500;
		break;
	case 40500: // 대기 위치 이동
		if (!motor.LENS_Z_Motor_Move(Wait_Pos))
		{
			logStr.Format("Lens Z 대기 위치 이동 실패 [%d]", iUseStep);
			errMsg2(Task.AutoFlag, logStr);
			iRtnFunction = -40500;
			break;
		}
		//
		if (!motor.PCB_Z_Motor_Move(Wait_Pos))
		{
			logStr.Format("PCB waiting Z 이동 실패");
			errMsg2(Task.AutoFlag, logStr);
			iRtnFunction = -40500;
			break;
		}
		iRtnFunction = 40550;
		break;
	case 40550:

		iRtnFunction = 40600;
		break;
	case 40600:
		if (LGIT_MODEL_INDEX == M1_HEATING_GEN2)
		{
			Dio.LensTailGrip(true, false);
		}
		Task.PCBTaskTime = myTimer(true);
		iRtnFunction = 40650;
		break;
	case 40650:
		if (LGIT_MODEL_INDEX == M1_HEATING_GEN2)
		{
			if (Dio.LensTailGripCheck(true, false))
			{
				iRtnFunction = 40700;

			}
			else if ((myTimer(true) - Task.PCBTaskTime) > IO_DELAY_TIME)
			{
				logStr.Format("Lens Tail Grip 확인 시간 초과");
				errMsg2(Task.AutoFlag, logStr);
				iRtnFunction = -40650;
				break;
			}

		}
		else
		{
			iRtnFunction = 40700;
		}
		break;
	case 40700:

		iRtnFunction = 40750;
		break;
	case 40750:

		iRtnFunction = 40900;
		break;
	case 40900:
		iRtnFunction = 41000;
		break;
	default:
		logStr.Format("Lens 공급 동작 Thread Step 번호 비정상 . [%d]", iUseStep);
		errMsg2(Task.AutoFlag, logStr);
		iRtnFunction = -1;
		break;
	}

	return iRtnFunction;
}

int CPcbProcess1::procProductComplete(int iStep)
{
	CString logStr = "";
	CString sSocketMsg = "";
	CString sBarCode = "";
	//
	int iRtnFunction = iStep;
	int iCamDelay = model.strInfo_Cam[1].m_iDelayTime;	//PCB 영상 획득 Delay
	int iLaserDelay = model.strInfo_Cam[0].m_iDelayTime;	//Laser 측정전 Delay
	int iAlignRetry = model.strInfo_Cam[1].m_iRetryCnt;
	double offsetX = 0.0;
	double offsetY = 0.0;
	double offsetTh = 0.0;
	int iTestPatCnt = 0;
	int iRtn = 0;
	//
	vision.clearOverlay(CCD);
	vision.drawOverlay(CCD);
	theApp.MainDlg->ctrlSubDlg(MAIN_DLG);
	theApp.MainDlg->m_iCurCamNo = 0;
	theApp.MainDlg->changeMainBtnColor(MAIN_DLG);
	//
	switch (iStep)
	{
	case 11000:	// PCB MIU 연결	
		Sleep(1000);
		Task.oldLaserTx = 0.1;
		Task.oldLaserTy = 0.1;
		Task.PCBTaskTime = myTimer(true);
		
		bThreadCCDRun1 = true;
		if (!theApp.MainDlg->MIUCheck_process() || gMIUDevice.CurrentState != 4)
		{
			sLangChange.LoadStringA(IDS_STRING452);	//CCD 모듈 영상 초기화 실패.[%d]\n 제품 안착 상태 및 제품 불량 확인 하세요.
			logStr.Format(sLangChange, iStep);
			errMsg2(Task.AutoFlag, logStr);
			iRtnFunction = -11000;
			break;
		}
		bThreadCCDRun1 = false;

		iRtnFunction = 12000;
		break;
	case 12000:
		//확인필요 norinda
		Task.m_bPBStart = 1;
		//if( Task.m_bPBStart ==  1 )
		//{
		//	func_Socket_Barcode();
		//}
		//else
		//{
		//	//! 바코드가 없기 때문에 통신 사용 안함.
		//	Task.iRecvLenACK[0] = Task.iRecvLenACK[1] = 0;
		//	Task.dTiltingManual[0] = Task.dTiltingManual[1] = 0.0;
		//	Task.dAlignManual[0] = Task.dAlignManual[0] = Task.dAlignManual[0] = 0.0;
		//}
		Task.d_Align_offset_x[PCB_Chip_MARK] = 0;
		Task.d_Align_offset_y[PCB_Chip_MARK] = 0;
		Task.d_Align_offset_th[PCB_Chip_MARK] = 0;

		if (sysData.m_iAlignCamInspPass == 1)		//procProductComplete
		{
			logStr.Format("완제품 Align Pass[%d]", iStep);
			putListLog(logStr);
			iRtnFunction = 12800;
			break;
		}
		iRtnFunction = 12100;
		break;

	case 12100:

		Task.m_iRetry_Opt = 0;
		//SetDigReference(PCB_Holder_MARK);
		LightControlthird.ctrlLedVolume(LIGHT_PCB, model.m_iLedValue[LEDDATA_HOLDER]);		// Align만 조명 ON
		
		if (motor.Pcb_Motor_Move(CompleteAlign_Pos))
		{
			Task.PCBTaskTime = myTimer(true);
			logStr.Format("Pcb 완제품 Align 위치 이동 완료[%d]", iStep);
			putListLog(logStr);
			iRtnFunction = 12200;
		}
		else
		{
			logStr.Format("Pcb 완제품 Align 위치 이동 실패[%d]", iStep);
			errMsg2(Task.AutoFlag, logStr);
			iRtnFunction = -12100;
		}
		break;
	case 12200:
		if (motor.PCB_Z_Motor_Move(CompleteAlign_Pos))
		{
			Task.PCBTaskTime = myTimer(true);
			logStr.Format("Pcb 완제품 Align Z위치 이동 완료[%d]", iStep);
			putListLog(logStr);
			iRtnFunction = 12400;
		}
		else
		{
			logStr.Format("Pcb 완제품 Align Z위치 이동 실패[%d]", iStep);
			errMsg2(Task.AutoFlag, logStr);
			iRtnFunction = -12200;
		}
		break;

	case 12400:
		checkMessage();
		if ((myTimer(true) - Task.PCBTaskTime) > iCamDelay && (motor.IsStopAxis(Motor_PCB_X) && motor.IsStopAxis(Motor_PCB_Y)))
		{
			iRtnFunction = 12500;
		}
		break;
	case 12500:

		if (Task.m_iRetry_Opt>iAlignRetry)
		{
			logStr.Format("PCB 완제품 Align 재검사 %d 회 실패[%d]\n 보정 미적용으로 진행하시겠습니까?", Task.m_iRetry_Opt, iStep);
			if (askMsg(logStr) == IDOK)
			{
				Task.d_Align_offset_x[PCB_Chip_MARK] -= 0;
				Task.d_Align_offset_y[PCB_Chip_MARK] -= 0;
				Task.d_Align_offset_th[PCB_Chip_MARK] += 0;
				iRtnFunction = 12800;
			}
			else
			{
				iRtnFunction = -12500;
				logStr.Format("PCB Align 재검사 %d 회 실패[%d]", Task.m_iRetry_Opt, iStep);
				errMsg2(Task.AutoFlag, logStr);
			}
			break;

		}
		//-----------------------------------
		offsetX = offsetY = offsetTh = 0.0;
		iRtn = theApp.MainDlg->procCamComAlign(CAM1, PCB_Chip_MARK, false, offsetX, offsetY, offsetTh);

		offsetTh = 0.0;
		saveInspImage(PCB_IMAGE_SAVE, Task.m_iRetry_Opt);
		Task.m_iRetry_Opt++;
		if (iRtn == 1)
		{
			Task.d_Align_offset_x[PCB_Chip_MARK] -= offsetX;
			Task.d_Align_offset_y[PCB_Chip_MARK] -= offsetY;
			Task.d_Align_offset_th[PCB_Chip_MARK] += offsetTh;

			logStr.Format("PCB 완제품 Align : %.3lf %.3lf %.3lf", Task.d_Align_offset_x[PCB_Chip_MARK], Task.d_Align_offset_y[PCB_Chip_MARK], Task.d_Align_offset_th[PCB_Chip_MARK]);
			//pcbDlg->m_labelAlignResult.SetText(logStr);
			//pcbDlg->m_labelAlignResult.Invalidate();
			putListLog(logStr);

			int iRtnVal = theApp.MainDlg->AlignLimitCheck(CAM1, offsetX, offsetY, offsetTh); // PCB이재기 움직이는 부분이랑 Limit Check 할때 CAM1임

			if (iRtnVal == 1)
			{
				iRtnFunction = 12700;				// 보정 이동
			}
			else if (iRtnVal == 2)
			{
				iLaser_Pos = 0;
				logStr.Format("		PCB 완제품 Align [%d] 완료 [%d]", Task.m_iRetry_Opt, iStep);
				putListLog(logStr);
				//InstantMarkDelete(PCB_Chip_MARK);
				logStr.Format("		PCB 완제품 Align Complete :  %d, %.03f, %.03f, %.03f", Task.m_iPcbPickupNo, Task.d_Align_offset_x[PCB_Chip_MARK], Task.d_Align_offset_y[PCB_Chip_MARK], Task.d_Align_offset_th[PCB_Chip_MARK]);
				AlignResultSave(logStr);
				Task.m_iRetry_Opt = 0;
				Task.dSocketRecvTime = myTimer(true);
				iRtnFunction = 12800;

			}
			else
			{
				logStr.Format("PCB 완제품 Align [%d] 보정 범위 초과[%d]", Task.m_iRetry_Opt, iStep);
				errMsg2(Task.AutoFlag, logStr);
				AlignLogSave(logStr);
				//InstantMarkDelete(PCB_Chip_MARK);
				iRtnFunction = -12500;
			}
		}
		else
		{
			logStr.Format("PCB 완제품 Align [%d] 마크 인식 실패[%d]", Task.m_iRetry_Opt, iStep);
			putListLog(logStr);
			iRtnFunction = 12500;				//  재검사 
		}
		break;

	case 12700://!! 보정량 이동.- Retry
		if (motor.Pcb_Holder_Align_Move(CompleteAlign_Pos, Task.d_Align_offset_x[PCB_Chip_MARK], Task.d_Align_offset_y[PCB_Chip_MARK], Task.d_Align_offset_th[PCB_Chip_MARK]))
		{
			Task.PCBTaskTime = myTimer(true);
			iRtnFunction = 12200;
		}
		else
		{
			logStr.Format("Pcb 완제품 Align 보정 위치 이동 실패[%d]", iStep);
			errMsg2(Task.AutoFlag, logStr);
			iRtnFunction = -12700;
		}
		break;
	case 12800:
		//LightControlthird.ctrlLedVolume(LIGHT_PCB, 0);		//레이저 검사전 off
		if (!motor.PCB_Z_Motor_Move(Wait_Pos))
		{
			sLangChange.LoadStringA(IDS_STRING305);	//PCB부 z축 모터 위치 이동 실패[%d]
			logStr.Format(sLangChange, iStep);
			delayMsg(logStr.GetBuffer(99), 3000, M_COLOR_RED);
			iRtnFunction = -12800;
			break;
		}

		iRtnFunction = 12850;


		break;
	case 12850:
		iLaser_Pos = 0;
		vision.clearOverlay(CAM1);
		//vision.drawOverlay(CAM1);
		//외부 변위측정값 Read
#if (____AA_WAY == LENS_TILT_AA) 
		if (g_ADOData.func_Get_RecordData(Task.ChipID))//DB에 Data가 있을 경우  //250403 TNEGERINGE_M5
		{

			Task.dAAPcbMotor[0] = g_ADOData.adoRegist.dPCBMotor[0];   //PCB X
			Task.dAAPcbMotor[1] = g_ADOData.adoRegist.dPCBMotor[1];   //PCB Y
			Task.dAAPcbMotor[2] = g_ADOData.adoRegist.dPCBMotor[2];   //PCB TH
																	  //TX[3] , TY[4]
			Task.dAAPcbMotor[3] = g_ADOData.adoRegist.dPCBMotor[3];   //PCB TX 
			Task.dAAPcbMotor[4] = g_ADOData.adoRegist.dPCBMotor[4];   //PCB TY 
																	  //
			Task.dAAPcbMotor[5] = g_ADOData.adoRegist.dPCBMotor[5];   //PCB Z
			Task.dAlignManual[0] = g_ADOData.adoRegist.dOffset_Align[0];  //AlignOffsetX
			Task.dAlignManual[1] = g_ADOData.adoRegist.dOffset_Align[1];  //AlignOffsetY
			Task.dAlignManual[2] = g_ADOData.adoRegist.dOffset_Align[2];  //AlignOffsetT
																		  //
			logStr.Format("PCB 화상검사 Chart Pos X(%.3lf), Y(%.3lf)", Task.dAAPcbMotor[0], Task.dAAPcbMotor[1]);
			putListLog(logStr);
			//
			logStr.Format("PCB 화상검사 TX(%.3lf), TY(%.3lf)", Task.dAAPcbMotor[3], Task.dAAPcbMotor[4]);
			putListLog(logStr);

			iRtnFunction = 12900;
			break;
		}
		else
		{
			Task.dAAPcbMotor[0] = model.axis[Motor_PCB_X].pos[Bonding_Pos];   //PCB X
			Task.dAAPcbMotor[1] = model.axis[Motor_PCB_Y].pos[Bonding_Pos];   //PCB Y
			Task.dAAPcbMotor[2] = model.axis[Motor_PCB_TH].pos[Bonding_Pos];   //PCB TH
																			   //TX[3] , TY[4]
			Task.dAAPcbMotor[3] = model.axis[Motor_PCB_Xt].pos[Bonding_Pos];   //PCB TX 	model.m_InspPcbTilt[0];//
			Task.dAAPcbMotor[4] = model.axis[Motor_PCB_Yt].pos[Bonding_Pos];   //PCB TY 	model.m_InspPcbTilt[1];//

																			   //
			Task.dAAPcbMotor[5] = model.axis[Motor_PCB_Z].pos[Bonding_Pos];   //PCB Z


			if (fabs(Task.dAAPcbMotor[0] - model.axis[Motor_PCB_X].pos[Bonding_Pos]) > 5.0)
			{
				logStr.Format("Pcb 화상검사 X spec Over[%d]", iStep);
				errMsg2(Task.AutoFlag, logStr);
				iRtnFunction = -12850;
				break;
			}
			if (fabs(Task.dAAPcbMotor[1] - model.axis[Motor_PCB_Y].pos[Bonding_Pos]) > 5.0)
			{
				logStr.Format("Pcb 화상검사 Y spec Over[%d]", iStep);
				errMsg2(Task.AutoFlag, logStr);
				iRtnFunction = -12850;
				break;
			}

			iRtnFunction = 12900;

			//iRtnFunction = 12900;
			//logStr.Format("일치 모델 없습니다.\ bonding 위치로 진행 하시겠습니까?");	//전체 원점 복귀를 실행 하시겠습니까?
			//if (askMsg(logStr))
			//{

			//	
			//	break;
			//}
			/*logStr.Format("일치 모델 없습니다.[%d]", iStep);
			errMsg2(Task.AutoFlag, logStr);
			iRtnFunction = -12850;*/
			//일치 모델 없을 경우 수동 티칭값 창으로.
			/*Task.m_iRtnTiltingDlg = 2;
			TiltingManualdlg->ShowWindow(SW_SHOW);
			iRtnFunction = 12870;*/
		}


		//Task.dAAPcbMotor[0] = model.axis[Motor_PCB_X].pos[Bonding_Pos];   //PCB X
		//Task.dAAPcbMotor[1] = model.axis[Motor_PCB_Y].pos[Bonding_Pos];   //PCB Y
		//Task.dAAPcbMotor[2] = model.axis[Motor_PCB_TH].pos[Bonding_Pos];   //PCB TH
		//																   //TX[3] , TY[4]
		//Task.dAAPcbMotor[3] = model.axis[Motor_PCB_Xt].pos[Bonding_Pos];   //PCB TX 	model.m_InspPcbTilt[0];//
		//Task.dAAPcbMotor[4] = model.axis[Motor_PCB_Yt].pos[Bonding_Pos];   //PCB TY 	model.m_InspPcbTilt[1];//

		//																   //
		//Task.dAAPcbMotor[5] = model.axis[Motor_PCB_Z].pos[Bonding_Pos];   //PCB Z


		//if (fabs(Task.dAAPcbMotor[0] - model.axis[Motor_PCB_X].pos[Bonding_Pos]) > 5.0)
		//{
		//	logStr.Format("Pcb 화상검사 X spec Over[%d]", iStep);
		//	errMsg2(Task.AutoFlag, logStr);
		//	iRtnFunction = -12850;
		//	break;
		//	}
		//if (fabs(Task.dAAPcbMotor[1] - model.axis[Motor_PCB_Y].pos[Bonding_Pos]) > 5.0)
		//{
		//	logStr.Format("Pcb 화상검사 Y spec Over[%d]", iStep);
		//	errMsg2(Task.AutoFlag, logStr);
		//	iRtnFunction = -12850;
		//	break;
		//}

		//iRtnFunction = 12900;

#else
		Task.dTiltingManual[0] = 0.0;
		Task.dTiltingManual[1] = 0.0;
		iRtnFunction = 12900;
		/*
		Task.dTiltingManual[0] = g_ADOData.adoRegist.dOffset_TiltTx;
		Task.dTiltingManual[1] = g_ADOData.adoRegist.dOffset_TiltTy;

		Task.dAlignManual[0] = g_ADOData.adoRegist.dOffset_Align[0];
		Task.dAlignManual[1] = g_ADOData.adoRegist.dOffset_Align[1];
		Task.dAlignManual[2] = g_ADOData.adoRegist.dOffset_Align[2];
		*/

		break;
#endif
		break;
	case 12870:
		if (Task.m_iRtnTiltingDlg == 0)
		{
			logStr.Format("변위측정 Data 입력 중 자동모드 정지 선택[%d]", iStep);
			errMsg2(Task.AutoFlag, logStr);
			iRtnFunction = -12870;
			break;
		}
		else if (Task.m_iRtnTiltingDlg == 1)
		{
			logStr.Format("외부 변위 Tilting 완료 위치 : Tx=%.03lf, Ty=%.03lf", Task.dTiltingManual[0], Task.dTiltingManual[1]);
			putListLog(logStr);

			Task.m_timeChecker.Measure_Time(4);	//Laser 변위 측정 시작 시간
												//iRtnFunction = 12900;//pcb aa또 레이저 하도록 200708

			iRtnFunction = 12900;

		}
		break;
	case 12900:
		if (sysData.m_iLaserInspPass == 1)		//완제품
		{
			iRtnFunction = 13300;		//완제품시 레이저 패스
			break;
		}


		if (motor.Pcb_Motor_Move_Laser(iLaser_Pos, Task.d_Align_offset_x[PCB_Holder_MARK], Task.d_Align_offset_y[PCB_Holder_MARK], Task.d_Align_offset_th[PCB_Holder_MARK]))
		{
			Task.PCBTaskTime = myTimer(true);
			if (motor.PCB_Z_Motor_Move(Com_Laser_Pos))//Laser_Lens_Pos))
			{
				Task.PCBTaskTime = myTimer(true);
				logStr.Format("Laser 변위 측정 외부 위치 이동 완료[%d]", iStep);
				putListLog(logStr);
				iRtnFunction = 12910;// 12950;
			}
			else {
				logStr.Format("PCB Com_Laser_Pos Z 이동 실패[%d]", iStep);
				putListLog(logStr);
				errMsg2(Task.AutoFlag, logStr);
				iRtnFunction = -12900;
				break;
			}
		}
		else
		{
			logStr.Format("Laser 변위 측정 외부 위치 이동 실패[%d]", iStep);
			errMsg2(Task.AutoFlag, logStr);
			iRtnFunction = -12900;
		}

		break;
	case 12910:
		if (iLaser_Pos > 0)
		{
			iRtnFunction = 12950;
			break;
		}
		if (Dio.LaserCylinderUPDOWN(false, false))
		{
			logStr.Format("LASER 실린더 하강 완료[%d]", iRtnFunction);
			theApp.MainDlg->putListLog(logStr);
			Task.PCBTaskTime = myTimer(true);
			iRtnFunction = 12920;
		}
		else
		{
			logStr.Format("LASER 실린더 하강 실패 [%d]", iRtnFunction);
			errMsg2(Task.AutoFlag, logStr);
			iRtnFunction = -12910;
		}
		break;

	case 12920:
		if (Dio.LaserCylinderCheck(false, false))
		{
			logStr.Format("LASER 실린더 하강 확인 완료[%d]", iRtnFunction);
			theApp.MainDlg->putListLog(logStr);
			iRtnFunction = 12950;

			Sleep(300);
		}
		else if ((myTimer(true) - Task.PCBTaskTime) > IO_DELAY_TIME)
		{
			logStr.Format("LASER 실린더 하강 시간 초과..[%d]", iRtnFunction);
			errMsg2(Task.AutoFlag, logStr);
			iRtnFunction = -12920;
		}
		break;
		//laser실린더 하강 END
	case 12950:
		checkMessage();
		if ((myTimer(true) - Task.PCBTaskTime) > iLaserDelay && (motor.IsStopAxis(Motor_PCB_X) && motor.IsStopAxis(Motor_PCB_Y)))
		{
			iRtnFunction = 13100;
		}
		break;
	case 13100:
		Keyence.func_CL3000_Scan(Task.m_Laser_Point[iLaser_Pos]);
		Sleep(100);
		logStr.Format("변위 센서 %lf[%d] - 외부 - %d 위치", Task.m_Laser_Point[iLaser_Pos], iStep, iLaser_Pos + 1);
		putListLog(logStr);
		LaserPos[iLaser_Pos].x = motor.GetEncoderPos(Motor_PCB_X);
		LaserPos[iLaser_Pos].y = motor.GetEncoderPos(Motor_PCB_Y);
		LaserValue[iLaser_Pos] = Task.m_Laser_Point[iLaser_Pos];

		if (Task.m_Laser_Point[iLaser_Pos] != 0)
		{
			if (iLaser_Pos == 3)
			{
				if (theApp.MainDlg->func_Check_LaserValueErr(LaserValue) == false)
				{
					logStr.Format("Laser 측정값 이상..외부 변위 측정값이 이상..[%d]\n 측정값:%.04lf, %.04lf, %.04lf, %.04lf ", iStep, LaserValue[0], LaserValue[1], LaserValue[2], LaserValue[3]);
					errMsg2(Task.AutoFlag, logStr);
					iRtnFunction = -13100;
					break;
				}

				if (theApp.MainDlg->_calcLaserTilt(LaserPos, LaserValue, Task.d_Align_offset_xt[0], Task.d_Align_offset_yt[0]))
				{
					/*	logStr.Format("		보정량 Tx: %.04lf, Ty: %.04lf", Task.d_Align_offset_xt[0], Task.d_Align_offset_yt[0] );
					putListLog(logStr);
					logStr.Format("		내부 보정량 편차 Tx: %.04lf, Ty: %.04lf", Task.dTiltingManual[0], Task.dTiltingManual[1] );
					putListLog(logStr);
					int Rnd = TiltAlignLimitCheck(Task.d_Align_offset_xt[0] + Task.dTiltingManual[0], Task.d_Align_offset_yt[0] + Task.dTiltingManual[1] );
					*/


					//Task.m_dataOffset_x[0] =  Task.d_Align_offset_xt[PCB_Holder_MARK];
					//Task.m_dataOffset_y[0] =  Task.d_Align_offset_yt[PCB_Holder_MARK];
					logStr.Format("		보정량 Tx: %.04lf, Ty: %.04lf", Task.d_Align_offset_xt[0], Task.d_Align_offset_yt[0]);
					putListLog(logStr);
					logStr.Format("		외부 보정량 편차 Tx: %.04lf, Ty: %.04lf", Task.dTiltingManual[0], Task.dTiltingManual[1]);
					putListLog(logStr);
					//int Rnd = TiltAlignLimitCheck(Task.d_Align_offset_xt[PCB_Holder_MARK],Task.d_Align_offset_yt[PCB_Holder_MARK]);
					//int Rnd = TiltAlignLimitCheck(Task.d_Align_offset_xt[0] + Task.dTiltingManual[0], Task.d_Align_offset_yt[0] + Task.dTiltingManual[1] );
					int Rnd = theApp.MainDlg->TiltAlignLimitCheck(Task.d_Align_offset_xt[0], Task.d_Align_offset_yt[0]);
					if (Rnd == 2)
					{
						Task.m_timeChecker.Measure_Time(5);	//Laser 변위 측정 완료 시간
						Task.m_dTime_LaserDpm = Task.m_timeChecker.m_adTime[5] - Task.m_timeChecker.m_adTime[4];
						//dispGrid();
						iRtnFunction = 13300;

					}
					else if (Rnd == 1)
					{
						iRtnFunction = 13200;	//보정량 이동		
					}
					else
					{
						logStr.Format("Laser Tilt  보정값 Limit를 초과 하였습니다.[%d]", iStep);
						errMsg2(Task.AutoFlag, logStr);
						iRtnFunction = -13100;
					}
				}
			}
			else
			{
				iRtnFunction = 12900;
				iLaser_Pos++;
			}
		}
		else
		{
			iRtnFunction = 13100;
		}
		break;
	case 13200:
		//if(motor.Pcb_Move_Tilt(Task.d_Align_offset_xt[0] + Task.dTiltingManual[0],Task.d_Align_offset_yt[0] + Task.dTiltingManual[1] ))
		if (motor.Pcb_Move_Tilt(Task.d_Align_offset_xt[0], Task.d_Align_offset_yt[0]))
		{
			iLaser_Pos = 0;
			iRtnFunction = 12900;
		}
		break;

	case 13300:
		if (Dio.LaserCylinderUPDOWN(true, false))
		{
			logStr.Format("LASER 실린더 상승 완료[%d]", iRtnFunction);
			theApp.MainDlg->putListLog(logStr);
			Task.PCBTaskTime = myTimer(true);
			iRtnFunction = 13310;
		}
		else
		{
			logStr.Format("LASER 실린더 상승 실패 [%d]", iRtnFunction);
			errMsg2(Task.AutoFlag, logStr);
			iRtnFunction = -13300;
		}
		break;
	case 13310:
		if (Dio.LaserCylinderCheck(true, false))
		{
			logStr.Format("LASER 실린더 상승 확인 완료[%d]", iRtnFunction);
			theApp.MainDlg->putListLog(logStr);
			iRtnFunction = 13350;
			Sleep(300);
		}
		else if ((myTimer(true) - Task.PCBTaskTime) > IO_DELAY_TIME)
		{
			logStr.Format("LASER 실린더 상승 시간 초과..[%d]", iRtnFunction);
			errMsg2(Task.AutoFlag, logStr);
			iRtnFunction = -13310;
		}
		break;
		//laser실린더 상승 END
	case 13350:
		if (motor.PCB_Z_Motor_Move(Wait_Pos))
		{
			logStr.Format("PCB Z 대기 위치 이동 완료[%d]", iRtnFunction);
			putListLog(logStr);
			iRtnFunction = 14000;
		}
		else
		{
			logStr.Format("PCB Z 대기 위치 이동 실패[%d]", iRtnFunction);
			putListLog(logStr);
			iRtnFunction = -13350;
		}
		break;
	case 14000:

		iRtnFunction = 14100;
		break;
	case 14100:

		iRtnFunction = 14200;
		break;

	case 14200:
		if ((myTimer(true) - Task.PCBTaskTime) > iCamDelay && (motor.IsStopAxis(Motor_PCB_X) && motor.IsStopAxis(Motor_PCB_Y)))
		{
			iRtnFunction = 14300;
			Task.m_iRetry_Opt = 0;
		}
		break;
	case 14300:

		iRtnFunction = 14400;				// 완료
		break;
	case 14400:
		iRtnFunction = 14500;
		break;
	case 14500:
		if (motor.PCB_Z_Motor_Move(Wait_Pos))
		{
			logStr.Format("PCB Z 대기 위치 이동 완료[%d]", iStep);
			putListLog(logStr);
			iRtnFunction = 15200;
		}
		else
		{
			logStr.Format("PCB Z 대기 위치 이동 실패[%d]", iStep);
			putListLog(logStr);
			iRtnFunction = -14500;
		}
		break;
	case 15200:
		iRtnFunction = 19600;
		break;

	case 19600: // 본딩 위치 이동 
				//if(motor.Pcb_Motor_Move(Bonding_Pos, Task.d_Align_offset_x[PCB_Holder_MARK] + Task.dAlignManual[0], Task.d_Align_offset_y[PCB_Holder_MARK] + Task.dAlignManual[1], Task.d_Align_offset_th[PCB_Holder_MARK] + Task.dAlignManual[2]) )
		if (motor.Pcb_Motor_Move(Bonding_Pos, Task.d_Align_offset_x[PCB_Chip_MARK], Task.d_Align_offset_y[PCB_Chip_MARK], Task.d_Align_offset_th[PCB_Chip_MARK]))
		{
			Task.PCBTaskTime = myTimer(true);
			logStr.Format("PCB Bonding 보정 위치 이동 완료[%d]", iStep);
			putListLog(logStr);
			iRtnFunction = 19700;
		}
		else
		{
			logStr.Format("PCB Bonding 위치 이동 실패[%d]", iStep);
			errMsg2(Task.AutoFlag, logStr);
			iRtnFunction = -19600;
		}
		break;
	case 19700:
		double dTxMotorBondingOffset;
		double dTyMotorBondingOffset;

#if (____AA_WAY == LENS_TILT_AA)
		if (sysData.m_iAlignCamInspPass == 1)		//procProductComplete
		{
			dTxMotorBondingOffset = Task.dAAPcbMotor[3];
			dTyMotorBondingOffset = Task.dAAPcbMotor[4];
		}
		else
		{
			dTxMotorBondingOffset = Task.dTiltingManual[0];
			dTyMotorBondingOffset = Task.dTiltingManual[1];
		}

		if (motor.Pcb_Move_Tilt(Task.dTiltingManual[0], Task.dTiltingManual[1])) //Task.dAAPcbMotor[3], Task.dAAPcbMotor[4]))   //250403 TNEGERINGE_M5  //Task.dTiltingManual[0], Task.dTiltingManual[1]
		{
			logStr.Format("PCB TX TY Bonding Teaching 위치 이동 완료[%d]", iStep);
			putListLog(logStr);
			iRtnFunction = 19800;
		}
		else
		{
			logStr.Format(_T("PCB Tilt Bonding Pos Move Fail[%d]"), iStep);
			errMsg2(Task.AutoFlag, logStr);
			iRtnFunction = -19700;
		}
#else
		iRtnFunction = 19800;
#endif
		break;
	case 19800: //z축 본딩 위치 이동
		if (!motor.PCB_Z_Motor_Move(Bonding_Pos))
		{
			logStr.Format(_T("PCB Z Bonding Pos Move Fail[%d]"), iStep);
			errMsg2(Task.AutoFlag, logStr);
			iRtnFunction = -19800;
			break;
		}
		Task.PCBTaskTime = myTimer(true);
		iRtnFunction = 19900;
		break;
	case 19900:
		if ((myTimer(true) - Task.PCBTaskTime) > iCamDelay && (motor.IsStopAxis(Motor_PCB_X) && motor.IsStopAxis(Motor_PCB_Y)))
		{
			if (sysData.m_iIrChartUse == 1)
			{
				IrvAlignLed.Irv_Light_SetValue(model.m_iLedValue[LEDDATA_TOP1_CHART], IR_CHART);
			}
			else
			{
				LightControl.ctrlLedVolume(LIGHT_TOP_CHART1, model.m_iLedValue[LEDDATA_TOP1_CHART]);
				LightControl.ctrlLedVolume(LIGHT_TOP_CHART2, model.m_iLedValue[LEDDATA_TOP2_CHART]);
				LightControl.ctrlLedVolume(LIGHT_TOP_CHART3, model.m_iLedValue[LEDDATA_TOP3_CHART]);
				LightControl.ctrlLedVolume(LIGHT_TOP_CHART4, model.m_iLedValue[LEDDATA_TOP4_CHART]);
			}


			LightControl.ctrlLedVolume(LIGHT_TOP_CHART1, model.m_iLedValue[LEDDATA_TOP1_CHART]);
			LightControl.ctrlLedVolume(LIGHT_TOP_CHART2, model.m_iLedValue[LEDDATA_TOP2_CHART]);
			LightControl.ctrlLedVolume(LIGHT_TOP_CHART3, model.m_iLedValue[LEDDATA_TOP3_CHART]);
			LightControl.ctrlLedVolume(LIGHT_TOP_CHART4, model.m_iLedValue[LEDDATA_TOP4_CHART]);
			LightControl.ctrlLedVolume(LIGHT_LEFT_CHART, model.m_iLedValue[LEDDATA_CHART_L]);
			LightControl.ctrlLedVolume(LIGHT_RIGHT_CHART, model.m_iLedValue[LEDDATA_CHART_R]);



			//if( func_MIU_ConnectLiveCheck() == false)		//MIU 영상 Check
			//{
			//	iRtnFunction = -19900;
			//	break;
			//}
			theApp.MainDlg->setCamDisplay(3, 1);

			iRtnFunction = 120000;//완제품 검사
		}
		break;

	default:
		sLangChange.LoadStringA(IDS_STRING1004);	//PCB 완제품 검사 Thread Step 번호 비정상 . [%d]
		logStr.Format(sLangChange, iStep);
		errMsg2(Task.AutoFlag, logStr);
		iRtnFunction = -20000;
		break;
	}

	logStr.Empty();
	sSocketMsg.Empty();
	sBarCode.Empty();
	return iRtnFunction;
}

int	CPcbProcess1::Complete_FinalInsp(int iStep)
{
	CString sLog = _T("");
	//CString sTemp = _T("");
	//CString sNgMsg = _T("");
	//CString sImgPath = _T("");
	//CString logStr = _T("");
	double	dFinalDelay = (double)(model.strInfo_Cam[3].m_iDelayTime);
	double offsetX = 0.0, offsetY = 0.0, offsetTh = 0.0;
	int iRtn = 0;
	int iRtnFunction = iStep;
	bool bMtfCheck4 = true;
	switch (iStep)
	{
	case 120000:
		theApp.MainDlg->setCamDisplay(3, 1);
		Sleep(dFinalDelay);

		/*sLog.Format("Gen2 Current:%.03lf", model.dGen2Current);
		putListLog(sLog);
		g_clPriInsp.func_insp_CDP800_Current();*/

		iRtnFunction = 121000;
		break;
	case 121000:

		iRtnFunction = 122000;
		break;

	case 122000:	//완제품 검사만 해당 (Final 검사전 X/Y/T 보정)
		Sleep(dFinalDelay);
		iRtn = theApp.MainDlg->_getMTF(SFR_FINAL);
		Sleep(500);
		if (iRtn > 0)
		{
			if (!theApp.MainDlg->_calcImageAlignment())	//-- Align 보정 Limit(임의의 값 사용함...1.0)
			{
				sLog.Format("NG.  X : %.3lf Y : %.3lf", Task.m_dShift_IMG_X, Task.m_dShift_IMG_Y);
				putListLog(sLog);
				iRtnFunction = -122000;
				break;
			}
			else
			{

				//틸팅값 안맞을 경우 값이 동일하게 반복됨.. 3회 정도만 하고 안맞으면 Spec IN 시키기.., 
				if (Task.m_iFineThCnt > 3)	Task.m_dShift_IMG_TH = 0.0;
				if (fabs(Task.m_dShift_IMG_TH) <= sysData.dSpec_OC_Theta)//X/Y 보정값이 Spec In일 경우 다음 Step
				{
					sLog.Format("	영상 Shift. Spec In(%.03lf) In : T: %.3lf", sysData.dSpec_OC_Theta, Task.m_dShift_IMG_TH);
					putListLog(sLog);
					iRtnFunction = 122500;
				}
				else
				{
					sLog.Format("	영상 Shift(Spec : %.03lf).  T: %.3lf", sysData.dSpec_OC_Theta, Task.m_dShift_IMG_TH);
					putListLog(sLog);
					Task.m_iFineThCnt++;
					iRtnFunction = 122100;
				}
			}
		}
		else
		{
			errMsg2(Task.AutoFlag, "MTF 측정 전 SFR 영상 획득 실패..");
			iRtnFunction = -122000;
		}
		break;

	case 122100:
		if (!theApp.MainDlg->_MotorMove_IMG_AlignTheta())
		{
			iRtnFunction = -122100;
		}
		else
		{
			Sleep(100);
			Task.PCBTaskTime = myTimer(true);
			iRtnFunction = 122200;	//Retry
		}
		break;
	case 122200:
		if ((myTimer(true) - Task.PCBTaskTime) > 300)
		{
			iRtnFunction = 122000;
			sLog.Format("	AA Step [%d]", iRtnFunction);
			putListLog(sLog);
		}
		break;
	case 122500:	//Final 재 검사 - 확인용
		theApp.MainDlg->setCamDisplay(3, 1);
		Sleep(dFinalDelay);
		MIU.func_Set_InspImageCopy(EOL_CHART, MIU.m_pFrameRawBuffer);	//완제품 모드
		Sleep(300);
		iRtnFunction = 122520;
		break;
	case 122520:
		Task.bInsCenter = true;

		theApp.MainDlg->_calcImageAlignment();

		Task.bInsCenter = false;

		Task.sfrResult = theApp.MainDlg->func_MTF(MIU.vChartBuffet);//EOL 화상 #1

		if (Task.sfrResult == true)
		{
			sLangChange.LoadStringA(IDS_STRING229);
			putListLog("[검사] MTF 검사 성공.");// Optic_Axis,
		}
		else
		{
			sLangChange.LoadStringA(IDS_STRING228);
			sLog.Format(sLangChange, iStep);
			putListLog(sLog);
		}
		Task.m_iDrawBarStep = Task.m_iCnt_Step_AA_Total;
		theApp.MainDlg->autodispDlg->DrawBarGraph();
		iRtnFunction = 122550;
		break;
	case 122550:
		vision.clearOverlay(CCD);
		vision.drawOverlay(CCD, true);

		iRtnFunction = 122600;
		break;
	case 122600:
		if (!motor.PCB_Z_Motor_Move(Wait_Pos))
		{
			sLangChange.LoadStringA(IDS_STRING305);	//PCB부 z축 모터 위치 이동 실패[%d]
			sLog.Format(sLangChange, iStep);
			putListLog(sLog);
			iRtnFunction = -122600;
			break;
		}
		iRtnFunction = 122700;
		break;
	case 122700:
		if (!motor.LENS_Z_Motor_Move(Wait_Pos))
		{
			sLangChange.LoadStringA(IDS_STRING747);	//Lens부 z축 모터 위치 이동 실패[%d]
			sLog.Format(sLangChange, iStep);
			putListLog(sLog);
			errMsg2(Task.AutoFlag, sLog);
			iRtnFunction = -122700;
			break;
		}

		Task.LensTaskTime = myTimer(true);
		//Task.m_iRetry_Opt = 0;
		iRtnFunction = 123000;
		break;
	case 123000:
		if (sysData.m_iDefectInspPass == 1)	// 이물검사 유무
		{
			sLog.Format(_T("[PASS] Defect Insp"), iStep);
			putListLog(sLog);
			iRtnFunction = 123400;
		}
		else
		{
			//iRtnFunction = 123100;
		}
		break;
	case 123100:
		/*if (!motor.Pcb_Motor_Move(Dark_Pos))
		{
			sLog.Format("PCB부 Dark 위치 이동 실패[%d]", iStep);
			putListLog(sLog);
			iRtnFunction = -123100;
		}
		else
		{
			sLog.Format("PCB부 Dark 위치 이동 완료[%d]", iStep);
			putListLog(sLog);
			iRtnFunction = 123200;
		}*/
		break;
	case 123200:
		/*if (!motor.PCB_Z_Motor_Move(Dark_Pos))
		{
			sLog.Format("PCB부 Dark Z 위치 이동 실패[%d]", iStep);
			putListLog(sLog);
			iRtnFunction = -123200;
			break;
		}*/
		Task.PCBTaskTime = myTimer(true);
		iRtnFunction = 123300;
		break;
	case 123300:
		Sleep(100);
		if (motor.IsStopAxis(Motor_PCB_Z))
		{
			Sleep(100);
			MIU.func_Set_InspImageCopy(LOW_LEVEL_RAW, MIU.m_pFrameRawBuffer);
			Task.PCBTaskTime = myTimer(true);
			iRtnFunction = 123400;
		}
		else if ((myTimer(true) - Task.PCBTaskTime) > 10000)
		{
			sLog.Format("PCB Z DAR 정지확인 시간 초과[%d]", iStep);
			errMsg2(Task.AutoFlag, sLog);
			iRtnFunction = -123200;
		}
		break;
	case 123400:
		if (!motor.PCB_Z_Motor_Move(Wait_Pos))
		{
			sLangChange.LoadStringA(IDS_STRING305);	//PCB부 z축 모터 위치 이동 실패[%d]
			sLog.Format(sLangChange, iStep);
			putListLog(sLog);
			iRtnFunction = -123400;
			break;
		}
		iRtnFunction = 123500;
		break;
	case 123500:
		if (sysData.m_iStaintInspPass == 1 && sysData.m_iDefectInspPass == 1)
		{
			iRtnFunction = 127000;
			break;
		}
		if (sysData.m_iIrChartUse == 1)
		{
			IrvAlignLed.Irv_Light_SetValue(model.m_iLedValue[LEDDATA_6500K], IR_OC);
		}
		else
		{
#ifdef FURONTEER_OC
			OCControl.SendLightLxStep(model.m_iLedValue[LEDDATA_6500K]);
#else
			if (LGIT_MODEL_INDEX == M1_TANGERINE_5M)
			{
				OcLight_Dms50v52.DMS_50V5_2_Value(1, model.m_iLedValue[LEDDATA_6500K]);
			}
			else
			{
				LightControlthird.ctrlLedVolume(LIGHT_OC_6500K, model.m_iLedValue[LEDDATA_6500K]);
			}
			
#endif
		}

		iRtnFunction = 123600;
		break;
	case 123600:

		if (!motor.Pcb_Motor_Move(OC_6500K_Pos, Task.d_Align_offset_x[PCB_Chip_MARK], Task.d_Align_offset_y[PCB_Chip_MARK], Task.d_Align_offset_th[PCB_Chip_MARK]))
		{
			sLangChange.LoadStringA(IDS_STRING1023);	//PCB부 모터 Defect 검사 위치 이동 실패[%d]
			sLog.Format(sLangChange, iStep);
			putListLog(sLog);
			iRtnFunction = -123600;
		}
		else
		{
			iRtnFunction = 123700;
		}
		break;
	case 123700:
		if (motor.PCB_Z_Motor_Move(OC_6500K_Pos))
		{
			sLog.Format("PCB Z Axis OC 6500K Pos Move Complete[%d]", iStep);
			putListLog(sLog);
			Task.PCBTaskTime = myTimer(true);
			iRtnFunction = 123800;
		}
		else
		{
			sLog.Format("PCB Z Axis OC 6500K Pos Move Fail[%d]", iStep);
			errMsg2(Task.AutoFlag, sLog);
			iRtnFunction = -123700;
		}
		break;
	case 123800:
		if ((myTimer(true) - Task.PCBTaskTime) > 500)
		{
			iRtnFunction = 123850;
		}
		break;
	case 123850:
		if (motor.IsStopAxis(Motor_PCB_Z))
		{
			Task.PCBTaskTime = myTimer(true);

			//6500k 이미지 저장
			Sleep(300);
			MIU.func_Set_InspImageCopy(MID_6500K_RAW, MIU.m_pFrameRawBuffer);
			//ccdDlg->m_pSFRDlg->Raw_ImageSave(MIU.m_pFrameRawBuffer, MID_6500K_RAW);
			//MID_6500K_RAW 이미지 저장

			iRtnFunction = 123900;
		}
		else
		{
			sLog.Format("PCB Z Axis OC 6500K Pos Move Check Fail[%d]", iStep);
			errMsg2(Task.AutoFlag, sLog);
			iRtnFunction = -123700;
		}
		break;
	case 123900:
		Sleep(dFinalDelay);
		if (sysData.m_iDefectInspPass == 1)	// 이물검사 유무
		{
			sLog.Format(_T("[PASS] Defect Insp"), iStep);
			putListLog(sLog);
		}
		else
		{
			if (g_clPriInsp.func_Insp_Defect(MIU.vDefectMidBuffer_6500K, MIU.vDefectLowBuffer, true) == true)	//EOL
			{
				putListLog("[검사] Defect 검사 성공.");
			}
			else
			{
				sLog.Format("[검사] Defect 검사  Error[%d]", iStep);
				putListLog(sLog);
				MandoInspLog.bInspRes = false;
			}
		}
		if (sysData.m_iStaintInspPass == 1)	// 이물검사 유무
		{
			sLog.Format(_T("[PASS] Stain Insp"), iStep);
			putListLog(sLog);

		}
		else
		{
			if (g_clPriInsp.func_Insp_Stain(MIU.vDefectMidBuffer_6500K) == true)
			{
				putListLog("[검사] Stain 검사 성공.");
			}
			else
			{
				sLog.Format("[검사] Stain 검사  Error[%d]", iStep);
				putListLog(sLog);
				MandoInspLog.bInspRes = false;
			}
		}
		iRtnFunction = 127000;
		break;
	case 127000:
		if (motor.PCB_Z_Motor_Move(Wait_Pos))
		{
			sLog.Format("PCB Z축 대기 위치 이동 완료[%d]", iStep);
			putListLog(sLog);
			iRtnFunction = 127600;
		}
		else
		{
			sLog.Format("PCB Z축 대기 위치 이동 실패[%d]", iStep);
			sLog.Format(sLangChange, iStep);
			errMsg2(Task.AutoFlag, sLog);
			iRtnFunction = -127000;
		}
		break;

	case 127600:
		Task.m_iRetry_Opt = 0;
		iRtnFunction = 127700;
		break;
	case 127700:
		//
		MIU.Close();
		iRtnFunction = 127800;
		//if (MIU.Close())
		//{
		//	iRtnFunction = 127800;
		//}
		//else
		//{
		//	iRtnFunction = 127700;

		//	if (Task.m_iRetry_Opt>3)
		//	{
		//		sLog.Format("CCD 영상 Close 실패 [%d]", iStep);	//CCD 영상 Close 실패[%d]
		//		errMsg2(Task.AutoFlag, sLog);
		//		iRtnFunction = -127700;
		//	}
		//	else {
		//		Task.m_iRetry_Opt++;
		//	}
		//}
		break;
	case 127800:

		iRtnFunction = 127900;
		break;
	case 127900:
		//MIU.m_acsInspImage.Unlock();
		Task.m_iRetry_Opt = 0;
		MandoInspLog.func_LogSave_UVAfter();	//만도 차량용Camera 검사 Log 저장(검사/보정량 저장 -> 최종Log)
												//! 검사 Count 처리
		g_FinalInspLog();
		//inspMES();
		//MESCommunication.Messave();
		work.iInspCnt_Output++;
		if (MandoInspLog.bInspRes == false)	work.iInspCnt_NG_Output++;
		theApp.MainDlg->dispInspResGrid();
		work.Save();
		work.Load();

		Task.m_timeChecker.Measure_Time(13);	//Final 검사 완료 시간
		Task.m_dTime_TotalAA = Task.m_timeChecker.m_adTime[13] - Task.m_timeChecker.m_adTime[12];
		theApp.MainDlg->dispGrid();

		iRtnFunction = 128000;
		break;
	case 128000:
		Sleep(100);
		vision.clearOverlay(CCD);
		if (MandoInspLog.bInspRes == true)
		{
			vision.textlist[CCD].addList(50, 100, "PASS", M_COLOR_GREEN, 65, 85, "Arial");
		}
		else
		{
			vision.textlist[CCD].addList(50, 100, "FAIL", M_COLOR_RED, 80, 100, "Arial");
			MandoInspLog.func_DrawNG_Overlay(CCD);	//화면 Overlay에 NG List Draw
			work.m_iCnt_NG_Output++;
		}
		vision.drawOverlay(CCD, true);

		//ccd영상 전환

		theApp.MainDlg->ctrlSubDlg(MAIN_DLG);
		theApp.MainDlg->setCamDisplay(3, 1);
		theApp.MainDlg->changeMainBtnColor(MAIN_DLG);
		iRtnFunction = 128100;
		break;

	case 128100:
		iRtnFunction = 129200;

		break;
	case 129200:
		if (!motor.Lens_Motor_MoveXY(0, Wait_Pos))
		{
			sLangChange.LoadStringA(IDS_STRING748);	//Lens부 모터 대기 위치 이동 실패[%d]
			sLog.Format(sLangChange, iStep);
			putListLog(sLog);
			iRtnFunction = -129200;
		}
		else
		{
			iRtnFunction = 129220;
		}
		break;

	case 129220:

		iRtnFunction = 129300;
		break;
	case 129300:
		if (motor.Pcb_Motor_Move(Wait_Pos))
		{
			sLangChange.LoadStringA(IDS_STRING987);	//PCB 대기 위치 이동 완료 [%d]
			sLog.Format(sLangChange, iStep);
			putListLog(sLog);
			iRtnFunction = 129350;
		}
		else
		{
			sLangChange.LoadStringA(IDS_STRING986);	//PCB 대기 위치 이동 실패 [%d]
			sLog.Format(sLangChange, iStep);
			errMsg2(Task.AutoFlag, sLog);
			iRtnFunction = -129300;
		}

		break;
	case 129350:
		iRtnFunction = 129400;
		break;
	case 129400:
		if (motor.Pcb_Tilt_Motor_Move(Wait_Pos))
		{
			sLog.Format("PCB TX,TY 대기 위치 이동 완료 [%d]", iStep);
			putListLog(sLog);
			iRtnFunction = 129500;
		}
		else
		{
			sLog.Format("PCB TX,TY 대기위치 이동 실패[%d]", iStep);
			errMsg2(Task.AutoFlag, sLog);
			iRtnFunction = -129400;
		}
		break;

	case 129500:
		Task.m_bOkDispense = -1;	//메인 도포 완료 Lamp 취소
		putListLog("		");
		putListLog("		");
		sLog.Format("			END =======%s", Task.ChipID);
		putListLog(sLog);
		putListLog("		");
		putListLog("		");
		Task.m_bPBStart = 0;	//바코드 정보 초기화
		if (true)
		{
			memset(Task.ChipID, 0x00, 256);
			sprintf_s(Task.ChipID, "EMPTY");
			theApp.MainDlg->func_ChipID_Draw();
		}

		Task.m_timeChecker.Measure_Time(14);	//완제품 배출 시간
		Task.m_dTime_Total = Task.m_timeChecker.m_adTime[14] - Task.m_timeChecker.m_adTime[1];

		theApp.MainDlg->dispGrid();
		theApp.MainDlg->EpoxyTimeCheck(theApp.MainDlg->bEpoxyTimeChk);
		//////////////////////////////////////////////////////////////////////////////////////////////////////
		Task.PCBTask = 10000;
		Task.LensTask = 10000;
		iRtnFunction = 10000;
		break;
	default:
		sLog.Format("Mando Test Process Step Number Error. [%d]", iStep);
		errMsg2(Task.AutoFlag, sLog);
		iRtnFunction = -iStep;
		break;
	}
	return iRtnFunction;
}


int CPcbProcess1::RunProc_LENS_AlignLaserMeasure(int iLensStep)
{
	int iRtnFunction = iLensStep;
	int iCamDelay = model.strInfo_Cam[1].m_iDelayTime;		//PCB 영상 획득 Delay
	int iAlignRetry = model.strInfo_Cam[1].m_iRetryCnt;
	int iRtn = 0;
	CString logStr = "";
	bool bChk = true;
	double offsetX = 0.0, offsetY = 0.0, offsetTh = 0.0;

	int iLaserDelay = model.strInfo_Cam[0].m_iDelayTime;	//Laser 측정전 Delay

	switch (iLensStep)
	{
	case 30000:
		Task.oldLaserTx = 0.1;
		Task.oldLaserTy = 0.1;
		Task.d_Align_offset_x[LENS_Align_MARK] = 0;
		Task.d_Align_offset_y[LENS_Align_MARK] = 0;
		Task.d_Align_offset_th[LENS_Align_MARK] = 0;
		//
		Task.m_iRetry_Opt = 0;
		if (motor.LENS_Z_Motor_Move(Wait_Pos))
		{
			Task.LensTaskTime = myTimer(true);
			iRtnFunction = 30100;
		}
		else
		{
			logStr.Format("Lens_Z축 대기위치 이동 실패.[%d]", iLensStep);
			errMsg2(Task.AutoFlag, logStr);
			iRtnFunction = -30000;
		}

		break;
	case 30100:
#if (____AA_WAY == PCB_TILT_AA)
		iRtnFunction = 30500;
#elif (____AA_WAY == LENS_TILT_AA)
		if (motor.Lens_Motor_MoveXY(0, Wait_Pos))
		{
			iRtnFunction = 43000;//41900;		//렌즈쪽 카메라 없음
		}
		else
		{
			logStr.Format("Lens 대기위치 이동 실패[%d]", iLensStep);
			putListLog(logStr);
			iRtnFunction = -30050;
		}
		break;

#endif

		break;
	case 30500:
		iRtnFunction = 30600;

		break;
	case 30600:
		iRtnFunction = 30900;

		break;
	case 30900:
#if (____AA_WAY == PCB_TILT_AA)
		if (sysData.m_iLaserInspPass == 1)	//렌즈
		{
			iRtnFunction = 41100;			//Laser 측정 Pass
		}
		else
		{
			iRtnFunction = 31000;		// 31300;//얼라인 임시 패스
		}

#elif (____AA_WAY == LENS_TILT_AA)
		iRtnFunction = 43000;//41900;		//렌즈쪽 카메라 없음
#endif

		break;
	case 31000:
		if (motor.LENS_Z_Motor_Move(Wait_Pos))
		{
			Task.LensTaskTime = myTimer(true);
			iRtnFunction = 31050;
		}
		else
		{
			logStr.Format("Lens_Z축 대기위치 이동 실패.[%d]", iLensStep);
			errMsg2(Task.AutoFlag, logStr);
			iRtnFunction = -31000;
		}



		break;

	case 31050:
		LightControlthird.ctrlLedVolume(LIGHT_PCB, model.m_iLedValue[LEDDATA_LENS]);
		if (motor.Lens_Motor_MoveXY(0, Lens_Pos))
		{
			Task.LensTaskTime = myTimer(true);
			logStr.Format("Lens Align 위치 이동 완료[%d]", iLensStep);
			putListLog(logStr);
			iRtnFunction = 31060;
		}
		else
		{
			logStr.Format("Lens Align 위치 이동 실패[%d]", iLensStep);
			errMsg2(Task.AutoFlag, logStr);
			iRtnFunction = -31050;
		}
		break;
	case 31060:
		if (motor.LENS_Z_Motor_Move(Lens_Pos))
		{
			Task.LensTaskTime = myTimer(true);
			logStr.Format("Lens Align Z 위치 이동 완료[%d]", iLensStep);
			putListLog(logStr);
			iRtnFunction = 31080;
		}
		else
		{
			logStr.Format("Lens Align Z 위치 이동 실패[%d]", iLensStep);
			errMsg2(Task.AutoFlag, logStr);
			iRtnFunction = -31060;
		}
		break;

	case 31080:
		checkMessage();
		if ((myTimer(true) - Task.PCBTaskTime) > iCamDelay && (motor.IsStopAxis(Motor_Lens_X) && motor.IsStopAxis(Motor_Lens_Y)))
		{
			Task.PCBTaskTime = myTimer(true);
			iRtnFunction = 31100;
		}
		break;

	case 31100: // lens Align
		if (Task.m_iRetry_Opt > iAlignRetry)
		{
			logStr.Format("Lens Align 재검사 %d 회 실패[%d]\n 보정 미적용으로 진행하시겠습니까?", Task.m_iRetry_Opt, iLensStep);
			if (askMsg(logStr) == IDOK)
			{
				Task.d_Align_offset_x[LENS_Align_MARK] = 0;
				Task.d_Align_offset_y[LENS_Align_MARK] = 0;
				Task.d_Align_offset_th[LENS_Align_MARK] = 0;
				iRtnFunction = 31300;
			}
			else
			{
				iRtnFunction = -31100;
				logStr.Format("Lens Align 재검사 %d 회 실패[%d]", Task.m_iRetry_Opt, iLensStep);
				errMsg2(Task.AutoFlag, logStr);
			}
			break;
		}

		offsetX = offsetY = offsetTh = 0.0;
		Sleep(200);
		iRtn = theApp.MainDlg->procCamAlign(CAM2 - 1, LENS_Align_MARK, false, offsetX, offsetY, offsetTh);
		saveInspImage(LENS_IMAGE_SAVE, Task.m_iRetry_Opt);

		Task.m_iRetry_Opt++;
		offsetY = 0;
		if (iRtn == 0)
		{
			Task.d_Align_offset_x[LENS_Align_MARK] -= offsetX;
			Task.d_Align_offset_y[LENS_Align_MARK] += offsetY;
			Task.d_Align_offset_th[LENS_Align_MARK] = 0;//+= offsetTh;	//Lens는 보정량 Theta가 없음

			logStr.Format("Lens Align : %.3lf %.3lf %.3lf", Task.d_Align_offset_x[LENS_Align_MARK], Task.d_Align_offset_y[LENS_Align_MARK], Task.d_Align_offset_th[LENS_Align_MARK]);
			//pcbDlg->m_labelAlignResult.SetText(logStr);
			//pcbDlg->m_labelAlignResult.Invalidate();

			putListLog(logStr);

			int iRtnVal = theApp.MainDlg->AlignLimitCheck(CAM2, offsetX, offsetY, 0);
			if (iRtnVal == 1)
			{
				iRtnFunction = 31200;				// 보정 이동
			}
			else if (iRtnVal == 2)
			{
				logStr.Format("		Lens Align [%d] 완료 [%d]", Task.m_iRetry_Opt, iLensStep);
				putListLog(logStr);
				theApp.MainDlg->InstantMarkDelete(LENS_Align_MARK);			//임시 등록 마크 삭제

				logStr.Format("		Lens Align Complete : %d, %.03f, %.03f, %.03f", Task.m_iLensPickupNoCentering, Task.d_Align_offset_x[LENS_Align_MARK], Task.d_Align_offset_y[LENS_Align_MARK], Task.d_Align_offset_th[LENS_Align_MARK]);
				AlignResultSave(logStr);
				Task.m_iRetry_Opt = 0;
				iRtnFunction = 31250;// 31300;				// 완료
			}
			else
			{
				logStr.Format("Lens Align [%d] 보정 범위 초과[%d]", Task.m_iRetry_Opt, iLensStep);
				errMsg2(Task.AutoFlag, logStr);
				AlignLogSave(logStr);
				theApp.MainDlg->InstantMarkDelete(LENS_Align_MARK);
				iRtnFunction = -31100;				// NG
			}
		}
		else
		{
			logStr.Format("Lens Align [%d] 마크 인식 실패[%d]", Task.m_iRetry_Opt, iLensStep);
			putListLog(logStr);
			iRtnFunction = 31100;					//  재검사 
		}
		break;

	case 31200://!! 보정량 이동.- Retry
		if (motor.Lens_Motor_Align_Move(Task.d_Align_offset_x[LENS_Align_MARK], Task.d_Align_offset_y[LENS_Align_MARK], Task.d_Align_offset_th[LENS_Align_MARK]))
		{
			Task.LensTaskTime = myTimer(true);
			iRtnFunction = 31080;
		}
		else
		{
			sLangChange.LoadStringA(IDS_STRING657);	//Lens Align 보정 위치 이동 실패
			logStr.Format(sLangChange + _T("[%d]"), iLensStep);
			errMsg2(Task.AutoFlag, logStr);
			iRtnFunction = -31200;
		}
		break;
	case 31250:
		LightControlthird.ctrlLedVolume(LIGHT_PCB, 0);
		if (motor.LENS_Z_Motor_Move(Wait_Pos))
		{
			Task.LensTaskTime = myTimer(true);
			iRtnFunction = 31300;
		}
		else
		{
			logStr.Format("Lens_Z축 대기위치 이동 실패.[%d]", iLensStep);
			errMsg2(Task.AutoFlag, logStr);
			iRtnFunction = -31250;
		}
		break;
	case 31300://! Lens-Z축 대기 위치 이동
		iLaser_Pos = 0;
		if (motor.Lens_Motor_Move_Laser(iLaser_Pos, Task.d_Align_offset_x[LENS_Align_MARK], Task.d_Align_offset_y[LENS_Align_MARK], Task.d_Align_offset_th[LENS_Align_MARK]))
		{
			Task.LensTaskTime = myTimer(true);
			logStr.Format("Laser 변위 측정 외부 위치 이동 완료[%d]", iLensStep);	//Laser 변위 측정 외부 위치 이동 완료[%d]
			putListLog(logStr);
			iRtnFunction = 40000;
		}
		else
		{
			logStr.Format("Laser 변위 측정 외부 위치 이동 실패 [%d]", iLensStep);
			errMsg2(Task.AutoFlag, logStr);
			iRtnFunction = -31300;
		}
		break;
	case 40000://! Laser-Z축 외부 측정위치 이동
		if (motor.LENS_Z_Motor_Move(Laser_Lens_Pos))
		{
			Task.LensTaskTime = myTimer(true);
			iRtnFunction = 40100;
		}
		else
		{
			logStr.Format("Laser-Z축 변위 측정 외부 위치 이동 실패 [%d]", iLensStep);
			errMsg2(Task.AutoFlag, logStr);
			iRtnFunction = -40000;
		}
		break;

	case 40100:
		iRtnFunction = 40200;
		break;

	case 40200://! Lens Stage 외부 Laser 측정위치 이동 
		if (motor.Lens_Motor_Move_Laser(iLaser_Pos, Task.d_Align_offset_x[LENS_Align_MARK], Task.d_Align_offset_y[LENS_Align_MARK], Task.d_Align_offset_th[LENS_Align_MARK]))
		{
			Task.LensTaskTime = myTimer(true);
			logStr.Format("Laser 변위 측정 외부 위치 이동 완료[%d]", iLensStep);	//Laser 변위 측정 외부 위치 이동 완료[%d]
			putListLog(logStr);
			if (iLaser_Pos == 0) { iRtnFunction = 40210; }//처음만 하강명령.
			else { iRtnFunction = 40300; }
		}
		else
		{
			logStr.Format("Laser 변위 측정 외부 위치 이동 실패 [%d]", iLensStep);
			errMsg2(Task.AutoFlag, logStr);
			iRtnFunction = -40200;
		}
		break;

		//Laser 실린더 하강
	case 40210:
		if (Dio.LaserCylinderUPDOWN(false, false))
		{
			logStr.Format("LASER 실린더 하강 완료[%d]", iRtnFunction);
			theApp.MainDlg->putListLog(logStr);
			Task.PCBTaskTime = myTimer(true);
			iRtnFunction = 40220;
		}
		else
		{
			logStr.Format("LASER 실린더 하강 실패 [%d]", iRtnFunction);
			errMsg2(Task.AutoFlag, logStr);
			iRtnFunction = -40210;
		}
		break;

	case 40220:
		if (Dio.LaserCylinderCheck(false, false))
		{
			logStr.Format("LASER 실린더 하강 확인 완료[%d]", iRtnFunction);
			theApp.MainDlg->putListLog(logStr);
			iRtnFunction = 40300;

			Sleep(300);
		}
		else if ((myTimer(true) - Task.PCBTaskTime) > IO_DELAY_TIME)
		{
			logStr.Format("LASER 실린더 하강 시간 초과..[%d]", iRtnFunction);
			errMsg2(Task.AutoFlag, logStr);
			iRtnFunction = -40220;
		}
		break;
	case 40300://! 측정 전 Delay
		checkMessage();
		if ((myTimer(true) - Task.LensTaskTime) > iLaserDelay && (motor.IsStopAxis(Motor_Lens_X) && motor.IsStopAxis(Motor_Lens_Y)))	//최주임님, Delay 확인
		{
			iRtnFunction = 41000;
		}
		break;
		//Laser 실린더 하강 END

	case 41000: //외부 변위 감지 (저장 따로 하고 Align 이동)
				//Keyence.func_LT9030_Scan(Task.m_Laser_Point[iLaser_Pos]);
		Keyence.func_CL3000_Scan(Task.m_Laser_Point[iLaser_Pos]);
		logStr.Format("변위 센서 %lf[%d] - 외부 - %d 위치", Task.m_Laser_Point[iLaser_Pos], iLensStep, iLaser_Pos + 1);
		putListLog(logStr);
		LaserPos[iLaser_Pos].x = motor.GetEncoderPos(Motor_Lens_X) - sysData.dDataset[Motor_Lens_X];
		LaserPos[iLaser_Pos].y = motor.GetEncoderPos(Motor_Lens_Y) - sysData.dDataset[Motor_Lens_Y];
		LaserValue[iLaser_Pos] = Task.m_Laser_Point[iLaser_Pos];
		if (Task.m_Laser_Point[iLaser_Pos] != 0)
		{
			if (iLaser_Pos == 3)
			{
				if (theApp.MainDlg->func_Check_LaserValueErr(LaserValue) == false)
				{
					sLangChange.LoadStringA(IDS_STRING639);	//Laser 측정값 이상..외부 변위 측정값이 이상..[%d]\n 측정값:%.04lf, %.04lf, %.04lf, %.04lf
					logStr.Format(sLangChange, iLensStep, LaserValue[0], LaserValue[1], LaserValue[2], LaserValue[3]);
					errMsg2(Task.AutoFlag, logStr);
					iRtnFunction = -41000;
					break;
				}

				if (theApp.MainDlg->_calcLaserTilt(LaserPos, LaserValue, Task.d_Align_offset_xt[LENS_Align_MARK], Task.d_Align_offset_yt[LENS_Align_MARK]))
				{
					Task.m_dataOffset_x[LENS_Align_MARK] = Task.d_Align_offset_xt[LENS_Align_MARK];
					Task.m_dataOffset_y[LENS_Align_MARK] = Task.d_Align_offset_yt[LENS_Align_MARK];


					sLangChange.LoadStringA(IDS_STRING1264);		//보정량 Tx: %.04lf, Ty: %.04lf
					logStr.Format(_T("		") + sLangChange, Task.d_Align_offset_xt[LENS_Align_MARK], Task.d_Align_offset_yt[LENS_Align_MARK]);
					putListLog(logStr);

					int Rnd = theApp.MainDlg->TiltAlignLimitCheck(Task.m_dataOffset_x[LENS_Align_MARK], Task.m_dataOffset_y[LENS_Align_MARK]);
					//20160117 이형석 수정

					if (Rnd == 2)
					{

						Task.m_timeChecker.Measure_Time(5);	//Laser 변위 측정 완료 시간
						Task.m_dTime_LaserDpm = Task.m_timeChecker.m_adTime[5] - Task.m_timeChecker.m_adTime[4];
						//dispGrid();
						MandoInspLog.dTilteOffset[0] = Task.m_dataOffset_x[LENS_Align_MARK];	//만도 차량용Camera 검사 Log 저장
						MandoInspLog.dTilteOffset[1] = Task.m_dataOffset_y[LENS_Align_MARK];


						//20141217 LHC - DataBase에 0값이 저장되는 경우가 생겨 0값이 들어갈 경우 저장하지않고 알람치도록.
						//김영호 20150602 FraneeGrabber  관련 임시 Pass
						if (Task.m_dataOffset_x[LENS_Align_MARK] == 0 || Task.m_dataOffset_y[LENS_Align_MARK] == 0)
						{
							sLangChange.LoadStringA(IDS_STRING627);	//Laser 변위 차 값 이상 발생..
							logStr.Format(sLangChange + _T("Xt : %lf, Yt : %lf"), Task.m_dataOffset_x[LENS_Align_MARK], Task.m_dataOffset_y[LENS_Align_MARK]);
							errMsg2(Task.AutoFlag, logStr);
							iRtnFunction = -41000;
							break;
						}
						if (!g_ADOData.func_AATaskToRecordLaser(Task.ChipID, Task.m_dataOffset_x[LENS_Align_MARK], Task.m_dataOffset_y[LENS_Align_MARK], Task.m_Laser_Point))
						{
							sLangChange.LoadStringA(IDS_STRING489);	//DataBase Laser 변위 측정 Data 기록 실패.[%d]\n MS Office를 닫아주세요.
							logStr.Format(sLangChange, iLensStep);
							errMsg2(Task.AutoFlag, logStr);
							iRtnFunction = -41000;
							break;
						}

						LightControlthird.ctrlLedVolume(LIGHT_PCB, model.m_iLedValue[LEDDATA_LENS]);		// Align만 조명 ON
						Task.LensTaskTime = myTimer(true);
						iRtnFunction = 41100;
						iLaser_Pos++;

					}
					else if (Rnd == 1)
					{
						iRtnFunction = 41050;
					}
					else
					{
						logStr.Format("Laser Tilt  보정값 Limit를 초과 하였습니다.[%d]", iLensStep);
						errMsg2(Task.AutoFlag, logStr);
						iRtnFunction = -41000;
					}
				}
			}
			else
			{
				iRtnFunction = 40200;
				iLaser_Pos++;
			}
		}
		else
		{
			iRtnFunction = 41000;
		}
		break;

	case 41050: // 이동  변위값이 0으로 만들기 위해 이동
		if (motor.Lens_Move_Tilt(Task.d_Align_offset_xt[LENS_Align_MARK], Task.d_Align_offset_yt[LENS_Align_MARK]))
		{
			iRtnFunction = 40200;
			iLaser_Pos = 0;//4;

		}
		break;

	case 41100:
		iRtnFunction = 41110;
		break;

		//laser실린더 상승
	case 41110:
		if (Dio.LaserCylinderUPDOWN(true, false))
		{
			logStr.Format("LASER 실린더 상승 완료[%d]", iRtnFunction);
			theApp.MainDlg->putListLog(logStr);
			Task.PCBTaskTime = myTimer(true);
			iRtnFunction = 41120;
		}
		else
		{
			logStr.Format("LASER 실린더 상승 실패 [%d]", iRtnFunction);
			errMsg2(Task.AutoFlag, logStr);
			iRtnFunction = -41110;
		}
		break;

	case 41120:
		if (Dio.LaserCylinderCheck(true, false))
		{
			logStr.Format("LASER 실린더 상승 확인 완료[%d]", iRtnFunction);
			theApp.MainDlg->putListLog(logStr);
			iRtnFunction = 41200;

			Sleep(300);
		}
		else if ((myTimer(true) - Task.PCBTaskTime) > IO_DELAY_TIME)
		{
			logStr.Format("LASER 실린더 상승 시간 초과..[%d]", iRtnFunction);
			errMsg2(Task.AutoFlag, logStr);
			iRtnFunction = -41120;
		}
		break;
		//laser실린더 상승END

	case 41200://! Lens-Z축 대기 위치 이동
		if (motor.LENS_Z_Motor_Move(Wait_Pos))
		{
			Task.LensTaskTime = myTimer(true);
			iRtnFunction = 41850;;//// 41300; //41850;//임시 렌즈 align pass
								  //iRtnFunction = 41300;
		}
		else
		{
			sLangChange.LoadStringA(IDS_STRING739);	//Lens_Z축 대기위치 이동 실패
			logStr.Format(sLangChange + _T("[%d]"), iLensStep);
			errMsg2(Task.AutoFlag, logStr);
			iRtnFunction = -41200;
		}
		break;

	case 41300:
		if (motor.Lens_Motor_MoveXY(0, Lens_Pos))
		{
			Task.LensTaskTime = myTimer(true);
			sLangChange.LoadStringA(IDS_STRING659);	//Lens Align 이동 [%d]
			logStr.Format(sLangChange, iLensStep);
			putListLog(logStr);
			iRtnFunction = 41400;
		}
		else
		{
			sLangChange.LoadStringA(IDS_STRING660);	//Lens Align 이동 실패[%d]
			logStr.Format(sLangChange, iLensStep);
			putListLog(logStr);
			errMsg2(Task.AutoFlag, logStr);
			iRtnFunction = -41300;
		}

		break;
	case 41400:

		if (motor.LENS_Z_Motor_Move(Lens_Pos))
		{
			Task.d_Align_offset_x[LENS_Align_MARK] = 0;
			Task.d_Align_offset_y[LENS_Align_MARK] = 0;
			Task.d_Align_offset_th[LENS_Align_MARK] = 0;
			Task.LensTaskTime = myTimer(true);
			sLangChange.LoadStringA(IDS_STRING661);	//Lens Align 위치 이동 완료[%d]
			logStr.Format(sLangChange, iLensStep);
			putListLog(logStr);
			iRtnFunction = 41500;
		}
		else
		{
			sLangChange.LoadStringA(IDS_STRING660);	//Lens Align 위치 이동 실패[%d]
			logStr.Format(sLangChange, iLensStep);
			errMsg2(Task.AutoFlag, logStr);
			iRtnFunction = -41400;
		}
		break;

	case 41500:
		checkMessage();
		if ((myTimer(true) - Task.LensTaskTime) > iCamDelay && (motor.IsStopAxis(Motor_Lens_X) && motor.IsStopAxis(Motor_Lens_Y)))
		{
			iRtnFunction = 41600;
			Task.m_iRetry_Opt = 0;
		}
		break;

	case 41600://! LENS Align(변위측정 보정후, Shift 계산 용도)
		if (sysData.m_FreeRun == 1)
		{
			for (int i = 0; i<100; i++)
			{
				Sleep(10);
				checkMessage();
			}

			iRtnFunction = 41700;
			Task.d_Align_offset_x[LENS_Align_MARK] = 0;
			Task.d_Align_offset_y[LENS_Align_MARK] = 0;
			Task.d_Align_offset_th[LENS_Align_MARK] = 0;
			break;
		}
		if (Task.m_iRetry_Opt > iAlignRetry)
		{
			logStr.Format("Lens Align 재검사 %d 회 실패[%d]\n 보정 미적용으로 진행하시겠습니까?", Task.m_iRetry_Opt, iLensStep);
			if (askMsg(logStr) == IDOK)
			{
				Task.d_Align_offset_x[LENS_Align_MARK] = 0;
				Task.d_Align_offset_y[LENS_Align_MARK] = 0;
				Task.d_Align_offset_th[LENS_Align_MARK] = 0;
				iRtnFunction = 41800;
			}
			else
			{
				iRtnFunction = -41600;
				logStr.Format("Lens Align 재검사 %d 회 실패[%d]", Task.m_iRetry_Opt, iLensStep);
				errMsg2(Task.AutoFlag, logStr);
			}

			break;
		}
		offsetX = offsetY = offsetTh = 0.0;
		Sleep(200);
		iRtn = theApp.MainDlg->procCamAlign(CAM2 - 1, LENS_Align_MARK, false, offsetX, offsetY, offsetTh);

		saveInspImage(LENS_IMAGE_SAVE, Task.m_iRetry_Opt);

		Task.m_iRetry_Opt++;

		if (iRtn == 0)
		{
			Task.d_Align_offset_x[LENS_Align_MARK] -= offsetX;
			Task.d_Align_offset_y[LENS_Align_MARK] += offsetY;
			Task.d_Align_offset_th[LENS_Align_MARK] = 0;//+= offsetTh;	//Lens는 보정량 Theta가 없음

			sLangChange.LoadStringA(IDS_STRING651);	//Lens Align : %.3lf %.3lf %.3lf
			logStr.Format("Lens Align : %.3lf %.3lf %.3lf", Task.d_Align_offset_x[LENS_Align_MARK], Task.d_Align_offset_y[LENS_Align_MARK], Task.d_Align_offset_th[LENS_Align_MARK]);
			//			Sleep(500);
			//pcbDlg->m_labelAlignResult.SetText(logStr);
			//pcbDlg->m_labelAlignResult.Invalidate();

			putListLog(logStr);

			int iRtnVal = theApp.MainDlg->AlignLimitCheck(CAM2, offsetX, offsetY, 0);
			if (iRtnVal == 1)
			{
				iRtnFunction = 41700;				// 보정 이동
			}
			else if (iRtnVal == 2)
			{
				sLangChange.LoadStringA(IDS_STRING654);	//Lens Align [%d] 완료 [%d]
				logStr.Format(_T("		") + sLangChange, Task.m_iRetry_Opt, iLensStep);
				putListLog(logStr);

				theApp.MainDlg->InstantMarkDelete(LENS_Align_MARK);			//임시 등록 마크 삭제

				sLangChange.LoadStringA(IDS_STRING655);	//Lens Align Complete : %d, %.03f, %.03f, %.03f
				logStr.Format(_T("		") + sLangChange, Task.m_iLensPickupNoCentering, Task.d_Align_offset_x[LENS_Align_MARK], Task.d_Align_offset_y[LENS_Align_MARK], Task.d_Align_offset_th[LENS_Align_MARK]);
				AlignResultSave(logStr);
				Task.m_iRetry_Opt = 0;
				iRtnFunction = 41800;				// 완료
			}
			else
			{
				sLangChange.LoadStringA(IDS_STRING653);	//Lens Align [%d] 보정 범위 초과
				logStr.Format(sLangChange + _T("[%d]"), Task.m_iRetry_Opt, iLensStep);
				errMsg2(Task.AutoFlag, logStr);
				AlignLogSave(logStr);
				theApp.MainDlg->InstantMarkDelete(LENS_Align_MARK);
				iRtnFunction = -41600;				// NG
			}
		}
		else
		{
			sLangChange.LoadStringA(IDS_STRING652);	//Lens Align [%d] 마크 인식 실패[%d]
			logStr.Format(sLangChange, Task.m_iRetry_Opt, iLensStep);
			putListLog(logStr);

			iRtnFunction = 41600;					//  재검사 
		}
		break;

	case 41700://!! 보정량 이동.- Retry
		if (motor.Lens_Motor_Align_Move(Task.d_Align_offset_x[LENS_Align_MARK], Task.d_Align_offset_y[LENS_Align_MARK], Task.d_Align_offset_th[LENS_Align_MARK]))
		{
			Task.LensTaskTime = myTimer(true);
			iRtnFunction = 41500;
		}
		else
		{
			sLangChange.LoadStringA(IDS_STRING657);	//Lens Align 보정 위치 이동 실패
			logStr.Format(sLangChange + _T("[%d]"), iLensStep);
			errMsg2(Task.AutoFlag, logStr);
			iRtnFunction = -41700;
		}
	case 41800: // 값 저장2

				//!!!!! 1차 Align 보정량 편차 저장 공식(PCB<-> Lens Sensor간)
		Task.m_dataOffset_x[1] = Task.d_Align_offset_x[LENS_Align_MARK];
		Task.m_dataOffset_y[1] = Task.d_Align_offset_y[LENS_Align_MARK];
		//		Task.m_dataOffset_th[1] = Task.d_Align_offset_th[LENS_Align_MARK] - Task.d_Align_offset_th[PCB_Chip_MARK];

		//sLangChange.LoadStringA(IDS_STRING962);	//PCB Sensor_Holder간 Align 편차값 : X:%.03f, Y:%.03f, Θ:%.04f [%d]
		//logStr.Format(_T("		") + sLangChange, Task.m_dataOffset_x[1], Task.m_dataOffset_y[1],Task.m_dataOffset_th[1], iLensStep);
		//putListLog(logStr);

		//만도 차량용Camera 검사 Log 저장
		MandoInspLog.dLensOffset[0] = Task.m_dataOffset_x[1];
		MandoInspLog.dLensOffset[1] = Task.m_dataOffset_y[1];
		MandoInspLog.dLensOffset[2] = Task.m_dataOffset_th[1];


		iRtnFunction = 41850;
		//if (Dio.CamLaserSlinderMove(false, true))
		//{
		//	logStr.Format("Laser/Cam 실린더 후진 완료[%d]", iLensStep);
		//	//putListLog(logStr);
		//	Task.LensTaskTime = myTimer(true);
		//	iRtnFunction = 41850;
		//}
		//else
		//{
		//	logStr.Format("Laser/Cam 실린더 후진 실패[%d]", iLensStep);
		//	errMsg2(Task.AutoFlag, logStr);
		//	iRtnFunction = -41800;
		//}

		// 20141217 LHC - sensor_holder간 align 편차 값에 0가 들어가면 정지하게끔
		//김영호 20150602 FraneeGrabber  관련 임시 Pass
		//if(Task.m_dataOffset_x[1] == 0 || Task.m_dataOffset_y[1] == 0 || Task.m_dataOffset_th[1] == 0)
		//{
		//	sLangChange.LoadStringA(IDS_STRING963);	//PCB Sensor_Holder간 Align 편차값 이상 발생 : X:%.03f, Y:%.03f, Θ:%.04f [%d]
		//	logStr.Format(_T("		") + sLangChange, Task.m_dataOffset_x[1], Task.m_dataOffset_y[1],Task.m_dataOffset_th[1], iLensStep);
		//	putListLog(logStr);
		//	errMsg2(Task.AutoFlag,logStr);
		//	iRtnFunction = -41800;
		//	break;
		//}

		//-- ADO DB 저장
		//g_ADOData.func_AATaskToRecordAlign(Task.ChipID, MandoInspLog.dLensOffset[0], MandoInspLog.dLensOffset[1], MandoInspLog.dLensOffset[2]);

		break;
	case 41850:
		iRtnFunction = 41870;
		//if (Dio.CamLaserSlinderMove(false, true))
		//{
		//	logStr.Format("Laser/Cam 실린더 후진 완료[%d]", iLensStep);
		//	//putListLog(logStr);
		//	Task.LensTaskTime = myTimer(true);
		//	iRtnFunction = 41870;
		//}
		//else
		//{
		//	logStr.Format("Laser/Cam 실린더 후진 실패[%d]", iLensStep);
		//	errMsg2(Task.AutoFlag, logStr);
		//	iRtnFunction = -41850;
		//}
		break;
	case 41870:
		iRtnFunction = 41900;
		//if (Dio.CamLaserSlinderCheck(false, false))
		//{
		//	logStr.Format("Laser/Cam 실린더 후진 확인 완료[%d]", iLensStep);
		//	putListLog(logStr);
		//	iRtnFunction = 41900;
		//}
		//else if ((myTimer(true) - Task.LensTaskTime) > 10000)//IO_DELAY_TIME)
		//{
		//	logStr.Format("Laser/Cam 실린더 후진 시간 초과[%d]", iLensStep);
		//	errMsg2(Task.AutoFlag, logStr);
		//	iRtnFunction = -41870;
		//}
		break;
	case 41900://! Lens-Z축 대기 위치 이동
		if (motor.LENS_Z_Motor_Move(Wait_Pos))
		{
			Task.LensTaskTime = myTimer(true);
			iRtnFunction = 41950;
		}
		else
		{
			sLangChange.LoadStringA(IDS_STRING739);	//Lens_Z축 대기위치 이동 실패
			logStr.Format(sLangChange + _T("[%d]"), iLensStep);
			errMsg2(Task.AutoFlag, logStr);
			iRtnFunction = -41900;
		}
		break;
	case 41950:
		iRtnFunction = 42000;


		break;
	case 42000: // 본딩 위치 이동
		if (motor.Lens_Motor_MoveXY(0, Bonding_Pos, Task.d_Align_offset_x[LENS_Align_MARK], Task.d_Align_offset_y[LENS_Align_MARK]))
		{
			sLangChange.LoadStringA(IDS_STRING659);	//Lens Align 이동 [%d]
			logStr.Format(sLangChange, iRtnFunction);
			putListLog(logStr);
			iRtnFunction = 42200;
		}
		else
		{
			sLangChange.LoadStringA(IDS_STRING664);	//Lens Bonding 위치 이동 실패[%d]
			logStr.Format(sLangChange, iRtnFunction);
			errMsg2(Task.AutoFlag, logStr);
			iRtnFunction = -42000;
		}
		break;
	case 42200:
		//죽스 모델  , lens 본딩위치에서 z축 대기위치에서 pcb 본딩위치로 못들어와서 z축 좀 들어올려놔야됨
		iRtnFunction = 43000;
		break;
	case 42400:
		if ((myTimer(true) - Task.LensTaskTime) > iCamDelay && (motor.IsStopAxis(Motor_Lens_Z)))
		{
			iRtnFunction = 43000;
		}
		break;
	case 43000:
		Task.interlockPcb = 1;
		Task.interlockLens = 1;
		iRtnFunction = 50000;
		break;
	default:
		sLangChange.LoadStringA(IDS_STRING628);	//Laser 변위 측정 Thread Step 번호 비정상 .
		logStr.Format(sLangChange + _T("[%d]"), iLensStep);
		errMsg2(Task.AutoFlag, logStr);
		iRtnFunction = -1;
		break;
	}

	return iRtnFunction;
}




