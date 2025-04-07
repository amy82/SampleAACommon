

#include "stdafx.h"
#include "DioControl.h"

CDioControl::CDioControl()
{	
}

//! Lens 흡착 센서
// 흡착만 확인 가능..
bool CDioControl::LensOnCheck(bool flag, bool waitFlag/*=true*/)
{//- B접점
//	if (!flag)
//	{
//		if (curInDio[1] & DIO_IN_LENS_SENSOR)
//			return true;
//	}
//	else
//	{
//		if (!(curInDio[1] & DIO_IN_LENS_SENSOR))
//			return true;
//	}
//
//#ifndef		ON_LINE_DIO
//	return true;
//#endif
//
//	if(!waitFlag)
//		return false;
//
//	double ep = myTimer(true);
//
//	while (waitFlag)
//	{
//		if (!flag)
//		{
//			if (curInDio[1] & DIO_IN_LENS_SENSOR)
//			{
//				break;
//			}
//		}
//
//		if(myTimer(true)-ep>IO_DELAY_TIME)
//		{
//			CString tmpStr;
//
//			sLangChange.LoadStringA(IDS_STRING711);
//			if(flag)			tmpStr.Format(sLangChange);	//LENS 감지 시간 초과
//
//			errMsg2(Task.AutoFlag,tmpStr);
//
//			return false;
//		}
//
//		checkMessage();
//		Sleep(1);
//		checkMessage();
//		Sleep(1);
//		checkMessage();
//	}
	return true;
}


//! PCB 흡착/탈착
bool CDioControl::PCBvaccumOn(int flag, bool waitFlag)		// 0:OFF, 1:흡착, 2:탈착
{
	/*int moduleNo = 2;
	int channelNo = 3;
	int blowTime = 1000;

	g_CriOutDio.Lock();

	int outVal = curOutDioByte[moduleNo][channelNo];

	if(flag==VACCUM_ON)
	{
		outVal =  outVal | DIO_OUT_PCB_VACUUM;
		outVal =  outVal & ~DIO_OUT_PCB_VACUUM_OFF;
	}
	else if(flag == VACCUM_BLOW || (flag == VACCUM_BLOW_ON))
	{
		outVal =  outVal & ~DIO_OUT_PCB_VACUUM;
		outVal =  outVal | DIO_OUT_PCB_VACUUM_OFF;
	}
	else if(flag == VACCUM_OFF)
	{
		outVal =  outVal | DIO_OUT_PCB_VACUUM_OFF;
		outVal =  outVal & ~DIO_OUT_PCB_VACUUM;
	}

	motor.OutDIO(moduleNo, channelNo, outVal);

	
	if(flag == VACCUM_BLOW)
	{
		Sleep(blowTime);

		outVal =  outVal & ~DIO_OUT_PCB_VACUUM;
		outVal =  outVal & ~DIO_OUT_PCB_VACUUM_OFF;

		motor.OutDIO(moduleNo, channelNo, outVal);
	}

	g_CriOutDio.Unlock();


#ifndef		ON_LINE_DIO
	return true;
#endif

	if ( waitFlag )
	{
		bool checkFlag = false;

		if(flag == VACCUM_ON)
			checkFlag = true;

		if ( !PCBvaccumOnCheck(checkFlag, waitFlag) )
		{
			return false;
		}
	}
*/
	return true;
}

//! PCB 흡착 센서
// 흡착만 확인 가능..
bool CDioControl::PCBvaccumOnCheck(bool flag, bool waitFlag)			
{
//	if (!flag)
//	{
//		if (curInDio[1] & DIO_IN_PCB_VACUUM)
//			return true;
//	}
//	else
//	{
//		if (!(curInDio[1] & DIO_IN_PCB_VACUUM))
//			return true;
//	}
//
//#ifndef		ON_LINE_DIO
//	return true;
//#endif
//
//	if(!waitFlag)
//		return false;
//
//	double ep = myTimer(true);
//
//	while (waitFlag)
//	{
//		if (flag)
//		{
//			if (curInDio[1] & DIO_IN_PCB_VACUUM)
//				break;
//		}
//		else
//		{
//			if (!(curInDio[1] & DIO_IN_PCB_VACUUM))
//				break;
//		}
//
//		if(myTimer(true)-ep>IO_DELAY_TIME)
//		{
//			CString tmpStr;
//			CString sTemp;
//			sLangChange.LoadStringA(IDS_STRING1011);
//			sTemp.LoadString(IDS_STRING1009);
//			if(flag)			tmpStr.Format(sLangChange);	//PCB 흡착 동작 시간 초과
//			else				tmpStr.Format(sTemp);	//PCB 탈착 동작 시간 초과
//
//			errMsg2(Task.AutoFlag,tmpStr);
//
//			return false;
//		}
//
//		checkMessage();
//		Sleep(5);
//	}
	return true;
}


void CDioControl::setAlarm(int AlarmNo)			//0: 알람 + Red Lamp On 1: Yellow Lamp 2: Green Lamp
{
	CAABonderDlg* pFrame = (CAABonderDlg*)AfxGetApp()->m_pMainWnd;
	CString sLog="";

	if (AlarmNo == ALARM_ON)
	{
		SetLampOn(ALARM_ON);
		SetBuzzer(true, AlarmNo);
		pFrame->m_labelTitle.SetBkColor(M_COLOR_RED);
	}
	else if(AlarmNo == ALARM_OFF)
	{
		SetLampOn(ALARM_OFF);
		SetBuzzer(false, AlarmNo);
		pFrame->m_labelTitle.SetBkColor(M_COLOR_BLUE);
	}
	else if(AlarmNo == AUTO_STOP)
	{
		SetLampOn(AUTO_STOP);
		SetBuzzer(false, AlarmNo);
		pFrame->m_labelTitle.SetBkColor(M_COLOR_BLUE);
	}
	else if(AlarmNo == AUTO_RUN)
	{
		SetLampOn(AUTO_RUN);
		//SetBuzzer(false, AlarmNo);
		pFrame->m_labelTitle.SetBkColor(M_COLOR_BLUE);
		//SetBuzzer(true, AlarmNo);
		//pFrame->m_labelTitle.SetBkColor(M_COLOR_BLUE);
		//Sleep(100);
		//SetBuzzer(false, AlarmNo);
	}

	pFrame->m_labelTitle.Invalidate();
}

bool CDioControl::ContiTailGrip(bool flag, bool waitFlag)
{
	
	return true;
}
bool CDioControl::ContiTailGripCheck(bool flag, bool waitFlag)
{
	return true;

//
//	if (flag)
//	{
//		if (curInDio[1] & DIO_IN_LASER_CAM_FOR)
//		{
//			if (!(curInDio[1] & DIO_IN_LASER_CAM_DOWN))
//				return true;
//		}
//	}
//	else
//	{
//		if (curInDio[1] & DIO_IN_LASER_CAM_DOWN)
//		{
//			if (!(curInDio[1] & DIO_IN_LASER_CAM_FOR))
//				return true;
//		}
//	}
//
//#ifndef		ON_LINE_DIO
//	return true;
//#endif
//
//	if (!waitFlag)
//		return false;
//
//	double ep = myTimer(true);
//
//	while (waitFlag)
//	{
//		if (flag)
//		{
//			if (curInDio[1] & DIO_IN_LASER_CAM_FOR)
//			{
//				if (!(curInDio[1] & DIO_IN_LASER_CAM_DOWN))
//					break;
//			}
//		}
//		else
//		{
//			if (curInDio[1] & DIO_IN_LASER_CAM_DOWN)
//			{
//				if (!(curInDio[1] & DIO_IN_LASER_CAM_FOR))
//					break;
//			}
//		}
//
//		if (myTimer(true) - ep>IO_DELAY_TIME)
//		{
//			CString tmpStr = "";
//			if (flag)			tmpStr.Format("Laser/Cam 전진 동작 시간 초과");	//Lens Grip 전진 동작 시간 초과
//			else				tmpStr.Format("Laser/Cam 후진 동작 시간 초과");	//Lens Grip 후진 동작 시간 초과
//
//			errMsg2(Task.AutoFlag, tmpStr);
//
//			return false;
//		}
//
//
//		checkMessage();
//		Sleep(1);
//		checkMessage();
//		Sleep(1);
//		checkMessage();
//	}

	return true;
}

bool CDioControl::LensTailGrip(bool flag, bool waitFlag)
{
#if (__MACHINE_MODEL == MACHINE_1ST)
	int moduleNo = 0;
	int channelNo = 3;

	g_CriOutDio.Lock();

	int outVal = curOutDioByte[moduleNo][channelNo];

	if (flag)
	{
		outVal = outVal | DIO_OUT_TAIL_GRIP;
		outVal = outVal & ~DIO_OUT_TAIL_UN_GRIP;
	}
	else
	{
		outVal = outVal & ~DIO_OUT_TAIL_GRIP;
		outVal = outVal | DIO_OUT_TAIL_UN_GRIP;
	}
	motor.OutDIO(moduleNo, channelNo, outVal);

	g_CriOutDio.Unlock();


#ifndef		ON_LINE_DIO
	return true;
#endif

#endif
	return true;
}

bool CDioControl::LensTailGripCheck(bool flag, bool waitFlag)
{
#if (__MACHINE_MODEL == MACHINE_1ST)
	if (flag)
	{
		if (curInDio[0] & DIO_IN_TAIL_GRIP)
		{
			if (!(curInDio[0] & DIO_IN_TAIL_UN_GRIP))
				return true;
		}
	}
	else
	{
		if (curInDio[0] & DIO_IN_TAIL_UN_GRIP)
		{
			if (!(curInDio[0] & DIO_IN_TAIL_GRIP))
				return true;
		}
	}

#ifndef		ON_LINE_DIO
	return true;
#endif
	if (!waitFlag)
		return false;

	
#endif
	return true;
}

	//! Lens Gripper 전진/후진

bool CDioControl::LensMotorGrip(bool flag, bool waitFlag)
{
	int moduleNo = 0;
	int channelNo = 1;
	 
	g_CriOutDio.Lock();

	int outVal = curOutDioByte[moduleNo][channelNo];

	if(flag)
	{
		outVal =  outVal | DIO_OUT_LENS_GRIP_FOR;
		outVal =  outVal & ~DIO_OUT_LENS_GRIP_BACK;		
	}
	else
	{
		outVal =  outVal & ~DIO_OUT_LENS_GRIP_FOR;
		outVal =  outVal | DIO_OUT_LENS_GRIP_BACK;		
	}
	motor.OutDIO(moduleNo, channelNo, outVal);

	g_CriOutDio.Unlock();


#ifndef		ON_LINE_DIO
	return true;
#endif


	if ( waitFlag )
	{
		if ( !LensMotorGripCheck(flag, waitFlag) )
		{
			return false;
		}
	}

	return true;
}

//! Lens Gripper 전진/후진 센서

bool CDioControl::LensMotorGripCheck(bool flag, bool waitFlag)
{
	if (flag)
	{
		if (curInDio[0] & DIO_IN_LENS_GRIP_FOR)
		{
			if (!(curInDio[0] & DIO_IN_LENS_GRIP_BACK))
				return true;
		}
	}
	else
	{
		if (curInDio[0] & DIO_IN_LENS_GRIP_BACK)
		{
			if (!(curInDio[0] & DIO_IN_LENS_GRIP_FOR))
				return true;
		}
	}

#ifndef		ON_LINE_DIO
	return true;
#endif

	if(!waitFlag)
		return false;

	double ep = myTimer(true);

	while (waitFlag)
	{
		if (flag)
		{
			if (curInDio[0] & DIO_IN_LENS_GRIP_FOR)
			{
				if (!(curInDio[0] & DIO_IN_LENS_GRIP_BACK))
					break;
			}
		}
		else
		{
			if (curInDio[1] & DIO_IN_LENS_GRIP_BACK)
			{
				if (!(curInDio[0] & DIO_IN_LENS_GRIP_FOR))
					break;
			}
		}

		if(myTimer(true)-ep>IO_DELAY_TIME)
		{
			CString tmpStr;
			CString sTemp;
			sLangChange.LoadStringA(IDS_STRING669);
			sTemp.LoadString(IDS_STRING672);
			if(flag)			tmpStr.Format(sLangChange);	//Lens Grip 전진 동작 시간 초과
			else				tmpStr.Format(sTemp);	//Lens Grip 후진 동작 시간 초과

			errMsg2(Task.AutoFlag, tmpStr);

			return false;
		}


		checkMessage();
		Sleep(1);
		checkMessage();
		Sleep(1);
		checkMessage();
	}

	return true;
}


//! START P/B 버튼 Check
bool CDioControl::StartPBOnCheck(bool flag, bool waitFlag)
{
#if (__MACHINE_MODEL == MACHINE_1ST)
	if (flag)
	{
		if (curInDio[0] & DIO_IN_START) {
			StartPBLampOn(true);
			return true;
		}
	}
	else
	{
		if (!(curInDio[0] & DIO_IN_START)) {
			StartPBLampOn(false);
			return true;
		}
	}
#else
	if (flag)
	{
		if (curInDio[0] & DIO_IN_START)
		{
			StartPBLampOn(false);
			return true;
		}

		
	}
	else
	{
		if (!(curInDio[0] & DIO_IN_START))
		{
			StartPBLampOn(true);
			return true;
		}
	}
#endif
	return false;
}

bool CDioControl::StartPBLampOn(bool flag)
{
	int moduleNo = 0;
#if (__MACHINE_MODEL == MACHINE_1ST)
	int channelNo = 2;
#else
	int channelNo = 0;
#endif
	g_CriOutDio.Lock();
	int outVal = curOutDioByte[moduleNo][channelNo];

	if(flag == true)	outVal = outVal | DIO_OUT_START_PB;
	else				outVal = outVal & ~DIO_OUT_START_PB;
	
	motor.OutDIO(moduleNo, channelNo, outVal);
	g_CriOutDio.Unlock();
	return true;
}


//! Door Open/Close
bool CDioControl::DoorPBOnCheck(bool flag, bool waitFlag)
{
	return false;
	//return true;
	//if (flag)
	//{
	//	if (curInDio[0] & DIO_IN_DOOR_PB){
	//		DoorPBLampOn(true);
	//		return true;
	//	}
	//}
	//else
	//{
	//	if (!(curInDio[0] & DIO_IN_DOOR_PB)){
	//		DoorPBLampOn(false);
	//		return true;
	//	}
	//}

	//if(!waitFlag)	return false;

	//double ep = myTimer(true);
	//while (waitFlag)
	//{
	//	if (flag)
	//	{
	//		if (curInDio[0] & DIO_IN_DOOR_PB){
	//			DoorPBLampOn(true);
	//			break;
	//		}
	//	}
	//	else
	//	{
	//		if (!(curInDio[0] & DIO_IN_DOOR_PB)){
	//			DoorPBLampOn(false);
	//			break;
	//		}
	//	}

	//	if(myTimer(true)-ep > IO_DELAY_TIME)
	//	{
	//		//CString tmpStr;
	//		//if(flag)			tmpStr.Format("Start 스위치 ON 시간 초과");
	//		//else				tmpStr.Format("Start 스위치 OFF 시간 초과");
	//		//errMsg2(Task.AutoFlag,tmpStr);
	//		return false;
	//	}

	//	checkMessage();
	//	Sleep(5);
	//}

	/*return true;*/
}

bool CDioControl::DoorPBLampOn(bool flag)
{
	/*int moduleNo = 0;
	int channelNo = 1;

	g_CriOutDio.Lock();
	int outVal = curOutDioByte[moduleNo][channelNo];

	if(flag == true)	outVal = outVal | DIO_OUT_DOOR_PB;
	else				outVal = outVal & ~DIO_OUT_DOOR_PB;
	
	motor.OutDIO(moduleNo, channelNo, outVal);
	g_CriOutDio.Unlock();
*/
	return true;
}
bool CDioControl::DoorLift(bool flag, bool waitflag)
{
	/*int moduleNo = 2;
	int channelNo = 0;

	g_CriOutDio.Lock();

	int outVal = curOutDioByte[moduleNo][channelNo];

	if(flag)
	{
		outVal =  outVal | DIO_OUT_DOOR_OPEN;
		outVal =  outVal & ~DIO_OUT_DOOR_CLOSE;		
	}
	else
	{
		outVal =  outVal & ~DIO_OUT_DOOR_OPEN;
		outVal =  outVal | DIO_OUT_DOOR_CLOSE;		
	}
	motor.OutDIO(moduleNo, channelNo, outVal);
	
	g_CriOutDio.Unlock();

	if ( !waitflag )	return true;

	if ( waitflag )
	{
		if ( !DoorLiftOnCheck(flag, waitflag) )
		{
			return false;
		}
	}*/
	//int moduleNo = 2;
	//int channelNoOpen = 1, channelNoClose = 2;//0;; 

	//g_CriOutDio.Lock();
	//int outValOpen = curOutDioByte[moduleNo][channelNoOpen];
	//int outValClose = curOutDioByte[moduleNo][channelNoClose];

	//if (flag == true)
	//{//-- Open 일 경우
	//	outValOpen = outValOpen | DIO_OUT_DOOR_OPEN;
	//	outValClose = outValClose & ~DIO_OUT_DOOR_CLOSE;

	//	motor.OutDIO(moduleNo, channelNoClose, outValClose);
	//	motor.OutDIO(moduleNo, channelNoOpen, outValOpen);
	//}
	//else
	//{//-- Close 일 경우
	//	outValOpen = outValOpen & ~DIO_OUT_DOOR_OPEN;
	//	outValClose = outValClose | DIO_OUT_DOOR_CLOSE;

	//	motor.OutDIO(moduleNo, channelNoOpen, outValOpen);
	//	motor.OutDIO(moduleNo, channelNoClose, outValClose);
	//}
	//g_CriOutDio.Unlock();

	//if (!waitflag)	return true;

	//if (waitflag)
	//{
	//	if (!DoorLiftOnCheck(flag, waitflag))
	//	{
	//		return false;
	//	}
	//}
	return true;
}

bool CDioControl::DoorLiftOnCheck(bool flag, bool waitflag)
{
	//if (flag)
	//{
	//	if (curInDio[1] & DIO_IN_DOOR_OPEN)
	//	{
	//		if (!(curInDio[1] & DIO_IN_DOOR_CLOSE))
	//			return true;
	//	}
	//}
	//else
	//{
	//	if (curInDio[1] & DIO_IN_DOOR_CLOSE)
	//	{
	//		if (!(curInDio[1] & DIO_IN_DOOR_OPEN))
	//			return true;
	//	}
	//}

	//if(!waitflag)	return false;

	//double ep = myTimer(true);
	//while (waitflag)
	//{
	//	if(Task.AutoFlag ==0)
	//	{
	//		break;	//자동운전시작으로 도어오픈도중에 STOP 클릭시 무한루프 방지
	//	}
	//	if (flag)
	//	{
	//		if (curInDio[1] & DIO_IN_DOOR_OPEN)
	//		{
	//			if (!(curInDio[1] & DIO_IN_DOOR_CLOSE))
	//				break;
	//		}
	//	}
	//	else
	//	{
	//		if (curInDio[1] & DIO_IN_DOOR_CLOSE)
	//		{
	//			if (!(curInDio[1] & DIO_IN_DOOR_OPEN))
	//				break;
	//		}
	//	}

	//	if(myTimer(true)-ep > 8000/*IO_DELAY_TIME*2*/)
	//	{
	//		CString tmpStr="";
	//		if(flag){
	//			tmpStr.Format("DoorLift Open 동작 시간 초과");
	//		}else{
	//			tmpStr.Format("DoorLift Close 동작 시간 초과");
	//		}
	//		errMsg2(Task.AutoFlag, tmpStr);

	//		return false;
	//	}

	//	checkMessage();
	//	Sleep(1);
	//	checkMessage();
	//	Sleep(1);
	//	checkMessage();
	//}

	return true;
}

bool CDioControl::LightCurtainOnCheck(bool flag, bool waitFlag)
{// flag = true일 경우 감지 확인, false = 미감지 확인
	return true;
	//if (flag)
	//{//감지확인 시 감지 되었을 경우
	//	if ( !(curInDio[0] & DIO_IN_LIGHT_CURTAIN) ){
	//		return true;
	//	}
	//}
	//else
	//{//미감지 확인 시  미감지 되었을 경우
	//	if ( (curInDio[0] & DIO_IN_LIGHT_CURTAIN) ){
	//		return true;
	//	}
	//}

	//if(!waitFlag)	return false;

	//double ep = myTimer(true);
	//while (waitFlag)
	//{
	//	if (flag)
	//	{
	//		if ( !(curInDio[0] & DIO_IN_LIGHT_CURTAIN) ){
	//			break;
	//		}
	//	}
	//	else
	//	{
	//		if ( curInDio[0] & DIO_IN_LIGHT_CURTAIN ){
	//			break;
	//		}
	//	}

	//	if(myTimer(true)-ep > IO_DELAY_TIME)
	//	{
	//		return false;
	//	}

	//	checkMessage();
	//	Sleep(5);
	//}
	return true;
}

bool CDioControl::UVCheck(bool flag, bool waitFlag)
{// flag = true일 경우 감지 확인, false = 미감지 확인
	return true;

	//if (flag)
	//{//감지확인 시 감지 되었을 경우
	//	if ( (curInDio[0] & DIO_IN_UV_LAMP_ON) ){
	//		return true;
	//	}
	//}
	//else
	//{//미감지 확인 시  미감지 되었을 경우
	//	if ( !(curInDio[0] & DIO_IN_UV_LAMP_ON) ){
	//		return true;
	//	}
	//}

	//if(!waitFlag)	return false;

	//double ep = myTimer(true);
	//while (waitFlag)
	//{
	//	if (flag)
	//	{
	//		if ( (curInDio[0] & DIO_IN_UV_LAMP_ON) ){
	//			break;
	//		}
	//	}
	//	else
	//	{
	//		if ( !curInDio[0] & DIO_IN_UV_LAMP_ON ){
	//			break;
	//		}
	//	}

	//	if(myTimer(true)-ep > IO_DELAY_TIME)
	//	{
	//		return false;
	//	}

	//	checkMessage();
	//	Sleep(5);
	//}
	return true;
}

bool CDioControl::UVTrigger(bool flag, bool waitFlag)
{
	//int moduleNo = 0;
	//int channelNo = 2;

	//g_CriOutDio.Lock();

	//int outVal = curOutDioByte[moduleNo][channelNo];

	//if (flag)
	//{	
	//	//outVal = outVal | DIO_OUT_UV_OFF;
	//	outVal = outVal /*| DIO_OUT_UV_UP*/ | DIO_OUT_UV_ON;
	//}
	//else
	//{
	//	//outVal = outVal & ~DIO_OUT_UV_OFF;
	//	outVal = outVal & ~/*DIO_OUT_UV_UP | */DIO_OUT_UV_ON;
	//}

	//motor.OutDIO(moduleNo, channelNo, outVal);
	//g_CriOutDio.Unlock();

	//if(!waitFlag)		return true;


	return true;
}

bool CDioControl::LensVacuumOn(bool flag)
{//Dispence 토출 On/Off 
	int moduleNo = 0;
	int channelNo = 0;


#if (__MACHINE_MODEL == MACHINE______2ST)
	g_CriOutDio.Lock();

	int outVal = curOutDioByte[moduleNo][channelNo];

	if (flag)
	{
		outVal = outVal | DIO_OUT_VACUUM_ON;
		outVal = outVal & ~DIO_OUT_VACUUM_OFF;
	}
	else
	{
		outVal = outVal | DIO_OUT_VACUUM_OFF;
		outVal = outVal & ~DIO_OUT_VACUUM_ON;
	}

	motor.OutDIO(moduleNo, channelNo, outVal);

	Sleep(300);

	outVal = outVal & ~DIO_OUT_VACUUM_OFF;
	motor.OutDIO(moduleNo, channelNo, outVal);

	g_CriOutDio.Unlock();
#endif

	return true;
}


bool CDioControl::EpoxyDischargeOn(bool flag, bool waitFlag )
{//Dispence 토출 On/Off 
	int moduleNo = 0;
#if (__MACHINE_MODEL == MACHINE_1ST)
	int channelNo = 3;
#else
	int channelNo = 1;
#endif
	g_CriOutDio.Lock();

	int outVal = curOutDioByte[moduleNo][channelNo];

	if (flag)
	{	
		outVal = outVal | DIO_OUT_EPOXY_ON;
	}
	else
	{
		outVal = outVal & ~DIO_OUT_EPOXY_ON;
	}

	motor.OutDIO(moduleNo, channelNo, outVal);

	g_CriOutDio.Unlock();


	return true;
}

bool CDioControl::EpoxyDischargeOnCheck(bool flag, bool waitFlag)
{	//Dispence 토출중 Check
	//if (!flag){
	//	if (curInDio[0] & DIO_IN_EPOXY_ON)
	//		return true;
	//}
	//else
	//{
	//	if (!(curInDio[0] & DIO_IN_EPOXY_ON))
	//		return true;
	//}

	//if(!waitFlag)	return false;

	//double ep = myTimer(true);

	//while (waitFlag)
	//{
	//	if (flag)
	//	{
	//		if (curInDio[0] & DIO_IN_EPOXY_ON)
	//			break;
	//	}
	//	else
	//	{
	//		if (!(curInDio[0] & DIO_IN_EPOXY_ON))
	//			break;
	//	}

	//	if(myTimer(true)-ep > IO_DELAY_TIME)
	//	{
	//		CString tmpStr;
	//		CString sTemp;
	//		sLangChange.LoadStringA(IDS_STRING501);
	//		sTemp.LoadString(IDS_STRING500);	
	//		if(flag)			tmpStr.Format(sLangChange);		//Dispence 토출상태ON 시간 초과
	//		else				tmpStr.Format(sTemp);			//Dispence 토출상태OFF 시간 초과

	//		errMsg2(Task.AutoFlag,tmpStr);

	//		return false;
	//	}

	//	checkMessage();
	//	Sleep(5);
	//}

	return true;
}


bool CDioControl::EpoxyDischargeOn2(bool flag, bool waitFlag )
{//Dispence 토출 On/Off 
	//int moduleNo = 0;
	//int channelNo = 3;	//

	//g_CriOutDio.Lock();

	//int outVal = curOutDioByte[moduleNo][channelNo];

	//if (flag)
	//{	
	//	outVal = outVal | DIO_OUT_EPOXY_ON2;
	//}
	//else
	//{
	//	//outVal = outVal | DIO_OUT_EPOXY_ON2;
	//	outVal = outVal & ~DIO_OUT_EPOXY_ON2;
	//}

	//motor.OutDIO(moduleNo, channelNo, outVal);

	//g_CriOutDio.Unlock();

	/*if(!waitFlag)		return true;

	if ( waitFlag )
	{
		if ( !EpoxyDischargeOnCheck(flag, waitFlag) )
		{
			return false;
		}
	}*/

	return true;
}

// 20150603
bool CDioControl::EpoxyDischargeOnCheck2(bool flag, bool waitFlag)
{	//Dispence 토출중 Check
	return true;

	//if (!flag){
	//	if (curInDio[0] & DIO_IN_EPOXY_ON2)
	//		return true;
	//}
	//else
	//{
	//	if (!(curInDio[0] & DIO_IN_EPOXY_ON2))
	//		return true;
	//}

	//if(!waitFlag)	return false;

	//double ep = myTimer(true);

	//while (waitFlag)
	//{
	//	if (flag)
	//	{
	//		if (curInDio[0] & DIO_IN_EPOXY_ON2)
	//			break;
	//	}
	//	else
	//	{
	//		if (!(curInDio[0] & DIO_IN_EPOXY_ON2))
	//			break;
	//	}

	//	if(myTimer(true)-ep > IO_DELAY_TIME)
	//	{
	//		CString tmpStr;
	//		CString sTemp;
	//		sLangChange.LoadStringA(IDS_STRING501);
	//		sTemp.LoadString(IDS_STRING500);	
	//		if(flag)			tmpStr.Format(sLangChange);		//Dispence 토출상태ON 시간 초과
	//		else				tmpStr.Format(sTemp);			//Dispence 토출상태OFF 시간 초과

	//		errMsg2(Task.AutoFlag,tmpStr);

	//		return false;
	//	}

	//	checkMessage();
	//	Sleep(5);
	//}

	return true;
}

void CDioControl::SetBuzzer(bool flag, int iType)
{
	return;
	//int moduleNo = 0;
	//int channelNo = 0;	//

	//g_CriOutDio.Lock();

	//int outVal = curOutDioByte[moduleNo][channelNo];
	//
	//outVal &= ~(DIO_OUT_TOWER_LAMP_BUZZER | DIO_OUT_BUZZER1 |DIO_OUT_BUZZER2 | DIO_OUT_BUZZER3 | DIO_OUT_BUZZER4);
	//switch(iType)
	//{
	//	case ALARM_ON:
	//		outVal |= DIO_OUT_BUZZER1;
	//		break;
	//	case AUTO_RUN:
	//		outVal |= DIO_OUT_BUZZER4;
	//		break;
	//	case AUTO_STOP:
	//		outVal |= DIO_OUT_BUZZER2;
	//		break;
	//}
	//motor.OutDIO(moduleNo, channelNo, outVal);

	//if(iType == AUTO_RUN || iType == AUTO_STOP )
	//{
	//	Sleep(500);
	//	outVal = curOutDioByte[moduleNo][channelNo];

	//	outVal &= ~(DIO_OUT_TOWER_LAMP_BUZZER | DIO_OUT_BUZZER1 | DIO_OUT_BUZZER2 | DIO_OUT_BUZZER3 | DIO_OUT_BUZZER4);
	//	motor.OutDIO(moduleNo, channelNo, outVal);
	//}

	//g_CriOutDio.Unlock();
}

int CDioControl::SensorContactCheck()
{
	int moduleNo = 1;
	int SensorData = 0;

	//if (curInDio[moduleNo] & DIO_IN_DIS_SENSOR)
	//	SensorData = 1;
	//else
	//	SensorData = 0;
	return SensorData;
}


void CDioControl::SetLampOn(int iType)
{
	return;
	int moduleNo = 0;
	int channelNo = 0;	

	g_CriOutDio.Lock();
	int outVal = curOutDioByte[moduleNo][channelNo];

	//if ( iType == ALARM_ON )//RED LAMP
	//{
	//	outVal |= DIO_OUT_TOWER_LAMP_R;
	//	outVal &= ~(DIO_OUT_TOWER_LAMP_G | DIO_OUT_TOWER_LAMP_Y);
	//}
	//else if ( iType == AUTO_RUN )//Green LAMP
	//{
	//	outVal |= DIO_OUT_TOWER_LAMP_G;
	//	outVal &= ~(DIO_OUT_TOWER_LAMP_R | DIO_OUT_TOWER_LAMP_Y);
	//}
	//else if ( iType == AUTO_STOP )//Yellow LAMP
	//{
	//	outVal |= DIO_OUT_TOWER_LAMP_Y;
	//	outVal &= ~(DIO_OUT_TOWER_LAMP_R | DIO_OUT_TOWER_LAMP_G);
	//}
	//else if( iType == ALARM_OFF)
	//{
	//	outVal &= ~(DIO_OUT_TOWER_LAMP_R | DIO_OUT_TOWER_LAMP_Y | DIO_OUT_TOWER_LAMP_G);
	//}
	//motor.OutDIO(moduleNo, channelNo, outVal);
	//g_CriOutDio.Unlock();
}


//! Pcb Grip 전진/후진
bool CDioControl::PcbGrip(bool flag, bool waitFlag)
{
	//return true;
	//int moduleNo = 0;
	//int channelNo = 1;
	//

	//g_CriOutDio.Lock();

	//int outVal = curOutDioByte[moduleNo][channelNo];

	//if(flag)
	//{
	//	outVal =  outVal | DIO_OUT_PCB_GRIP_FOR;
	//	outVal =  outVal & ~DIO_OUT_PCB_GRIP_BACK;		
	//}
	//else
	//{
	//	outVal =  outVal & ~DIO_OUT_PCB_GRIP_FOR;
	//	outVal =  outVal | DIO_OUT_PCB_GRIP_BACK;		
	//}
	//motor.OutDIO(moduleNo, channelNo, outVal);

	//g_CriOutDio.Unlock();


#ifndef		ON_LINE_DIO
	return true;
#endif


	//if ( waitFlag )
	//{
	//	if ( !PcbGripCheck(flag, waitFlag) )
	//	{
	//		return false;
	//	}
	//}

	return true;
}

//! PCB Grip 전진/후진 센서
bool CDioControl::PcbGripCheck(bool flag, bool waitFlag)
{
	/*return true;
	if (flag)
	{
		if (curInDio[0] & DIO_IN_PCB_PUSH_FOR)
		{
			if (!(curInDio[0] & DIO_IN_PCB_PUSH_BACK))
				return true;
		}
	}
	else
	{
		if (curInDio[0] & DIO_IN_PCB_PUSH_BACK)
		{
			if (!(curInDio[0] & DIO_IN_PCB_PUSH_FOR))
				return true;
		}
	}*/

#ifndef		ON_LINE_DIO
	return true;
#endif

	//if(!waitFlag)
	//	return false;

	//double ep = myTimer(true);

	//while (waitFlag)
	//{
	//	if (flag)
	//	{
	//		if (curInDio[0] & DIO_IN_PCB_PUSH_FOR)
	//		{
	//			if (!(curInDio[0] & DIO_IN_PCB_PUSH_BACK))
	//				break;
	//		}
	//	}
	//	else
	//	{
	//		if (curInDio[0] & DIO_IN_PCB_PUSH_BACK)
	//		{
	//			if (!(curInDio[0] & DIO_IN_PCB_PUSH_FOR))
	//				break;
	//		}
	//	}

	//	if(myTimer(true)-ep>IO_DELAY_TIME)
	//	{
	//		CString tmpStr;
	//		CString sTemp;
	//		//sLangChange.LoadStringA(IDS_STRING669);
	//		//sTemp.LoadString(IDS_STRING672);
	//		if(flag)			tmpStr.Format(_T("PCB Grip Advance Operation Timeout"));	//PCB Grip 전진 동작 시간 초과
	//		else				tmpStr.Format(_T("PCB Grip Backward Operation Timeout"));		//PCB Grip 후진 동작 시간 초과

	//		errMsg2(Task.AutoFlag, tmpStr);

	//		return false;
	//	}
	//}

	return true;
}

//Laser Cylinder 상승/하강
bool CDioControl::LaserCylinderUPDOWN(bool flag, bool waitFlag)
{
#if (__MACHINE_MODEL == MACHINE_1ST)
	int moduleNo = 0;
	int channelNo = 2;
	g_CriOutDio.Lock();

	int outVal = curOutDioByte[moduleNo][channelNo];

	if (flag)
	{
		outVal = outVal | DIO_OUT_LASER_CYLINDER_UP;
		outVal = outVal & ~DIO_OUT_LASER_CYLINDER_DOWN;
	}
	else
	{
		outVal = outVal & ~DIO_OUT_LASER_CYLINDER_UP;
		outVal = outVal | DIO_OUT_LASER_CYLINDER_DOWN;
	}

	motor.OutDIO(moduleNo, channelNo, outVal);

	g_CriOutDio.Unlock();


#ifndef		ON_LINE_DIO
	return true;
#endif


	if (waitFlag)
	{
		if (!LaserCylinderCheck(flag, waitFlag))
		{
			return false;
		}
	}
#endif
	return true;
}

//! Laser Cylinder 상승/하강 센서
bool CDioControl::LaserCylinderCheck(bool flag, bool waitFlag)
{
#if (__MACHINE_MODEL == MACHINE_1ST)
	if (flag)
	{
		if (curInDio[0] & DIO_IN_LASER_CYLINDER_UP)
		{
			if (!(curInDio[0] & DIO_IN_LASER_CYLINDER_DOWN))
				return true;
		}
	}
	else
	{
		if (curInDio[0] & DIO_IN_LASER_CYLINDER_DOWN)
		{
			if (!(curInDio[0] & DIO_IN_LASER_CYLINDER_UP))
				return true;
		}
	}

#ifndef		ON_LINE_DIO
	return true;
#endif

	if (!waitFlag)
		return false;

	double ep = myTimer(true);

	while (waitFlag)
	{
		if (flag)
		{
			if (curInDio[0] & DIO_IN_LASER_CYLINDER_UP)
			{
				if (!(curInDio[0] & DIO_IN_LASER_CYLINDER_DOWN))
					break;
			}
		}
		else
		{
			if (curInDio[0] & DIO_IN_LASER_CYLINDER_DOWN)
			{
				if (!(curInDio[0] & DIO_IN_LASER_CYLINDER_UP))
					break;
			}
		}

		if (myTimer(true) - ep>IO_DELAY_TIME)
		{
			CString tmpStr;
			CString sTemp;
			//sLangChange.LoadStringA(IDS_STRING669);
			//sTemp.LoadString(IDS_STRING672);
			if (flag)			tmpStr.Format(_T("Laser Cylinder Rising Operation Timeout"));	//PCB Grip 전진 동작 시간 초과
			else				tmpStr.Format(_T("Laser Cylinder Falling Operation Timeout"));		//PCB Grip 후진 동작 시간 초과

			errMsg2(Task.AutoFlag, tmpStr);

			return false;
		}
	}
#endif
	return true;
}

