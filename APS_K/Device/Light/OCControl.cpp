#include "stdafx.h"
#include "OCControl.h"
//#include "../../../AutoInspDlg.h"
#include "../../AA BonderDlg.h"
#define BCR_STX				0x02
#define BCR_ETX				0x03

#define BCR_ACK				0x06
#define BCR_NAK				0x15

COcControl::COcControl()
{
	//m_pMainDlg = NULL;
}
COcControl::~COcControl()
{
}
void COcControl::init()
{

}
bool COcControl::OpenLightClose()
{
	LightSerial.Close();
	return true;
}
bool COcControl::OpenLightSerial(unsigned int nPort, unsigned int nBaudRate)
{
	if (LightSerial.Open(nPort, nBaudRate) == false)
	{
		//m_pMainDlg->putListLog(_T("SERIAL OPEN FAIL!"));
		return false;
	}
	else
	{
		//m_pMainDlg->putListLog(_T("SERIAL OPEN OK!"));
		return true;
	}
}

bool COcControl::SendLightONOFF(bool Use)
{
	unsigned char sSendBuff[100];
	int nSendSize;
	int nRetVal;

	memset(sSendBuff, 0x00, sizeof(sSendBuff));
	
	sSendBuff[0] = BCR_STX;
	sSendBuff[1] = 0x01;
	sSendBuff[2] = 0x00;
	sSendBuff[3] = 0x00;
	sSendBuff[4] = 0x00;
	sSendBuff[5] = 0x00;
	if (Use == true)
	{
		sSendBuff[6] = 1;
	}
	else
	{
		sSendBuff[6] = 0;
	}
	
	sSendBuff[7] = BCR_ETX;
	nSendSize = 8;// 1;

	nRetVal = LightSerial.SendData(sSendBuff, nSendSize);
	if (nRetVal != nSendSize)
		return false;

	return true;
}


bool COcControl::SendLightChange(int Use)
{
	unsigned char sSendBuff[100];
	int nSendSize;
	int nRetVal;

	memset(sSendBuff, 0x00, sizeof(sSendBuff));
	sSendBuff[0] = BCR_STX;
	sSendBuff[1] = 0x02;
	//if (Use == OC_LIGHT_5000K)
	//{
	//	sSendBuff[2] = 0x00;	//5000k
	//}
	//else
	//{
	//	sSendBuff[2] = 0x01;	//6500k
	//}
	sSendBuff[2] = 0x00;	//5000k
	//
	sSendBuff[3] = 0x00;
	sSendBuff[4] = 0x00;
	sSendBuff[5] = 0x00;
	sSendBuff[6] = 0x00;
	sSendBuff[7] = BCR_ETX;
	nSendSize = 8;// 1;

	nRetVal = LightSerial.SendData(sSendBuff, nSendSize);
	if (nRetVal != nSendSize)
		return false;

	return true;
}


bool COcControl::SendLightValue(int data)
{
	unsigned char sSendBuff[100];
	int nSendSize;
	int nRetVal;

	memset(sSendBuff, 0x00, sizeof(sSendBuff));
	sSendBuff[0] = BCR_STX;
	sSendBuff[1] = 0x03;		//value up/down
	sSendBuff[2] = 0x00;
	sSendBuff[3] = 0x00;
	sSendBuff[4] = 0x00;
	sSendBuff[5] = 0x00;
	sSendBuff[6] = data;
	sSendBuff[7] = BCR_ETX;
	nSendSize = 8;// 1;

	nRetVal = LightSerial.SendData(sSendBuff, nSendSize);
	if (nRetVal != nSendSize)
		return false;

	return true;
}

bool COcControl::SendLightLxStep(int data)
{

	if (data == 0)
	{
		OCControl.SendLightONOFF(false);
		return true;
	}
	OCControl.SendLightONOFF(true);
	data -= 1;
	unsigned char sSendBuff[100];
	int nSendSize;
	int nRetVal;
	if (data > 4)
	{
		data = 4;
	}
	memset(sSendBuff, 0x00, sizeof(sSendBuff));
	sSendBuff[0] = BCR_STX;
	sSendBuff[1] = 0x09;		//Lx Step Set
	sSendBuff[2] = 0x00;			//ID 사용 X
	sSendBuff[3] = data;			//STEP  COMMEND가 9일때만 (0 ~ 4)  1000LX ~ 5000LX
	sSendBuff[4] = 0x00;			//X
	sSendBuff[5] = 0x00;			//X
	sSendBuff[6] = 0x00;			//X
	sSendBuff[7] = BCR_ETX;
	nSendSize = 8;// 1;

	nRetVal = LightSerial.SendData(sSendBuff, nSendSize);
	if (nRetVal != nSendSize)
		return false;

	return true;
}

bool COcControl::LxSetDataSave()
{
	unsigned char sSendBuff[100];
	int nSendSize;
	int nRetVal;

	memset(sSendBuff, 0x00, sizeof(sSendBuff));
	sSendBuff[0] = BCR_STX;
	sSendBuff[1] = 0x05;		//State data Save E2P
	sSendBuff[2] = 0x00;
	sSendBuff[3] = 0x00;
	sSendBuff[4] = 0x00;
	sSendBuff[5] = 0x00;
	sSendBuff[6] = 0x00;
	sSendBuff[7] = BCR_ETX;
	nSendSize = 8;

	nRetVal = LightSerial.SendData(sSendBuff, nSendSize);
	if (nRetVal != nSendSize)
		return false;

	return true;
}

bool COcControl::DMS_50V5_2_OnOff(int ch, bool OnOff)
{

	//unsigned char sSendBuff[100];
	//int nSendSize = 0;
	//char mch = 0x1 + ch;
	//memset(sSendBuff, 0x00, sizeof(sSendBuff));

	//sSendBuff[0] = 0x02;			//STX
	//sSendBuff[1] = 0x1B;			//Add-0
	//sSendBuff[2] = 0x80;			//Add-1
	//sSendBuff[3] = 0x37;			//Command On/Off 일때는 0x37 고정
	//if (OnOff)
	//{
	//	sSendBuff[4] = 0x11;		//Status 1채널 Off
	//}
	//else
	//{
	//	sSendBuff[4] = 0x10;		//Status 1채널 On
	//}
	//sSendBuff[5] = 0x03;			//ETX
	//sSendBuff[6] = 0x00;
	//sSendBuff[7] = '\r';
	//sSendBuff[8] = '\n';

	//for (int i = 0; i < 8; i++)
	//{
	//	sSendBuff[8] ^= sSendBuff[i];		//BCC - Add-0 ~ Status 모든값의 XOR
	//}
	//nSendSize = 9;

	//Sleep(100);

	//int nRetVal = SendLightONOFF(sSendBuff, nSendSize);
	//if (nRetVal != nSendSize)
	//{
	//	return false;
	//}
	return true;
}
