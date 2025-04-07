#pragma once
#include "../../Utility/Serial/Serial.h"
class AutoInspDlg;
class COcControl
{
public:
	COcControl();
	~COcControl();

private:
	//AutoInspDlg* m_pMainDlg;
public:
	//void SetMainDlgPtr(AutoInspDlg* pDlg) { m_pMainDlg = pDlg; }
	void init();
public:
	//백두 조명 컨트롤러
	CSerial LightSerial;
	bool OpenLightSerial(unsigned int nPort, unsigned int nBaudRate);
	bool OpenLightClose();
	bool SendLightONOFF(bool USE);
	bool SendLightChange(int USE);
	bool SendLightValue(int data);
	bool SendLightLxStep(int data);
	bool LxSetDataSave();
	//
	//----------------------------------------------------------------------------------------

	//DMS-50V5-2_DM 1채널 이물광원 - YOUTH TECH
	bool DMS_50V5_2_OnOff(int ch, bool OnOff);
	bool DMS_50V5_2_Value(int ch, int data);


};

