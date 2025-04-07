// LightDlg.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "LightDlg.h"
//#include "LensTeaching.h"







// CLightDlg 대화 상자입니다.

IMPLEMENT_DYNAMIC(CLightDlg, CDialogEx)

	CLightDlg::CLightDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CLightDlg::IDD, pParent)
	, m_iSelLED(0)
	, m_iSelData(0)
{
	m_iSelLED = 0;
	m_Bchart_Sel_Index = LIGHT_TOP_CHART1;
	m_Align_Sel_Index = LIGHT_PCB;
	//
	m_BchartData_Sel_Index = LEDDATA_TOP1_CHART;
	m_AlignData_Sel_Index = LEDDATA_SENSOR;
	//
	m_OcData_Sel_Index = LEDDATA_6500K;//LEDDATA_STAIN
	m_Oc_Sel_Index = LIGHT_OC_6500K;
}

CLightDlg::~CLightDlg()
{
}

void CLightDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_SLIDER_CAM1_LED, m_sliderCam1Led);
	DDX_Control(pDX, IDC_SLIDER_CAM2_LED, m_sliderCam2Led);
	DDX_Control(pDX, IDC_SLIDER_CAM3_LED, m_sliderCam3Led);

	DDX_Control(pDX, IDC_SLIDER_TOPCHART_LED, m_sliderTopChartLed);
	//
	DDX_Control(pDX, IDC_SLIDER_LENS_LED, m_sliderLensLed);

	DDX_Control(pDX, IDC_EDIT_CAM1_LED, m_LabelChartLight);
	DDX_Control(pDX, IDC_EDIT_CAM2_LED, m_LabelOcLight);
	DDX_Control(pDX, IDC_EDIT_CAM3_LED, m_LabelAlignLight);

	DDX_Control(pDX, IDC_EDIT_LENS_LED, m_LabelLensLight);
	DDX_Control(pDX, IDC_EDIT_TOPCHART_LED, m_LabelTopChartLight);
	

	DDX_Control(pDX, IDC_BTN_TOP1_CHARTLED, m_bTopChart1);
	DDX_Control(pDX, IDC_BTN_TOP2_CHARTLED, m_bTopChart2);
	DDX_Control(pDX, IDC_BTN_TOP3_CHARTLED, m_bTopChart3);
	DDX_Control(pDX, IDC_BTN_TOP4_CHARTLED, m_bTopChart4);
	DDX_Control(pDX, IDC_BTN_TOP5_CHARTLED, m_bTopChart5);
	DDX_Control(pDX, IDC_BTN_TOP6_CHARTLED, m_bTopChart6);
	DDX_Control(pDX, IDC_BTN_LEFT_CHARTLED, m_bLeftChart);
	DDX_Control(pDX, IDC_BTN_RIGHT_CHARTLED,m_bRightChart);
	DDX_Control(pDX, IDC_BTN_CHART_CHANGE,	m_bTotalChart);
	//
	DDX_Control(pDX, IDC_BTN_SENSOR_CAM1_LED, m_bSensorCam1);
	DDX_Control(pDX, IDC_BTN_HOLDER_CAM1_LED, m_bHolderCam1);
	DDX_Control(pDX, IDC_BTN_EPOXYINSP_CAM1_LED, m_bEpoxyInspCam1);
	DDX_Control(pDX, IDC_BTN_LENS_CAM1_LED, m_bLensCam1);

	
	DDX_Control(pDX, IDC_BTN_OC_6500K_LED, m_bStainOc);
	DDX_Control(pDX, IDC_BTN_OC_2800K_LED, m_bDefectOc);
}

BEGIN_MESSAGE_MAP(CLightDlg, CDialogEx)
	ON_BN_CLICKED(IDC_BUTTON_SAVE, &CLightDlg::OnClickedButtonSave)

	//ON_BN_CLICKED(IDC_RADIO_RESIN, &CLightDlg::OnBnClickedRadioResin)

	ON_BN_CLICKED(IDOK, &CLightDlg::OnBnClickedOk)
	ON_WM_SHOWWINDOW()
	ON_WM_HSCROLL()
	ON_STN_CLICKED(IDC_EDIT_CAM1_LED, &CLightDlg::OnStnClickedEditLed)
	ON_BN_CLICKED(IDC_BUTTON_PWR_ONOFF, &CLightDlg::OnBnClickedButtonPwrOnoff)
	ON_STN_CLICKED(IDC_EDIT_LENS_LED, &CLightDlg::OnStnClickedEditLensLed)
	ON_STN_CLICKED(IDC_EDIT_CAM2_LED, &CLightDlg::OnStnClickedEditOcLed)
	ON_STN_CLICKED(IDC_EDIT_TOPCHART_LED, &CLightDlg::OnStnClickedEditTopchartLed)
	ON_COMMAND_RANGE( IDC_BTN_TOP1_CHARTLED, IDC_BTN_RIGHT_CHARTLED, &CLightDlg::OnClickedTopChart)
	ON_COMMAND_RANGE( IDC_BTN_SENSOR_CAM1_LED, IDC_BTN_LENS_CAM1_LED, &CLightDlg::OnClickedCam1)
	ON_COMMAND_RANGE(IDC_BTN_OC_6500K_LED, IDC_BTN_OC_2800K_LED, &CLightDlg::OnClickedOc)

	ON_STN_CLICKED(IDC_EDIT_CAM3_LED, &CLightDlg::OnStnClickedEditBchartLed)

	ON_BN_CLICKED(IDC_BTN_CHART_CHANGE, &CLightDlg::OnBnClickedBtnChartChange)
	ON_BN_CLICKED(IDC_BTN_TOP1_CHARTLED, &CLightDlg::OnBnClickedBtnTop1Chartled)
	ON_BN_CLICKED(IDC_BTN_CHART_LED_OFF, &CLightDlg::OnBnClickedBtnChartLedOff)
	ON_BN_CLICKED(IDC_BUTTON_LIGHT_CLOSE, &CLightDlg::OnBnClickedButtonLightClose)
END_MESSAGE_MAP()


// CLightDlg 메시지 처리기입니다.



BOOL CLightDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();


	onOffFlag = true;
	setInterface();
	
	

	//((CButton*)GetDlgItem(IDC_RADIO_MTF))->SetCheck(1);
	return TRUE;  // return TRUE unless you set the focus to a control
	// 예외: OCX 속성 페이지는 FALSE를 반환해야 합니다.
}


void CLightDlg::setInterface()
{

	/*CAABonderDlg* pFrame = (CAABonderDlg*)AfxGetApp()->m_pMainWnd;

	WINDOWPLACEMENT wndpl;

	wndpl.rcNormalPosition = pFrame->m_rectCamDispPos2;
	wndpl.rcNormalPosition.right = pFrame->m_rectCamDispPos2.right + 100;
	wndpl.rcNormalPosition.left = pFrame->m_rectCamDispPos2.left - 0; 
	wndpl.rcNormalPosition.top -= SUB_DLG_OFFSET_Y - 10;
	wndpl.rcNormalPosition.bottom = MAIN_DLG_SIZE_Y - 5 - 90;

	this->MoveWindow(&wndpl.rcNormalPosition);
*/

	this->CenterWindow();
	/* 조명 */
	CString strTemp="";
	m_sliderLensLed.SetRange(0, 999);
	//
	
	//
	int value = 0;
	if (sysData.m_iIrChartUse == 1)
	{
		value = 999;
		m_sliderCam1Led.SetRange(0, value);
		m_sliderCam2Led.SetRange(0, value);
	}
	else
	{
#if (__MACHINE_MODEL == MACHINE_1ST)
		value = 255;
		m_sliderCam1Led.SetRange(0, value);

#ifdef FURONTEER_OC
		m_sliderCam2Led.SetRange(0, 5);
#else
		m_sliderCam2Led.SetRange(0, 255);
#endif
#else
		value = 999;
		m_sliderCam1Led.SetRange(0, value);
		m_sliderCam2Led.SetRange(0, 255);
#endif
		
		GetDlgItem(IDC_STATIC_IRV_CHART_GROUP)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_STATIC_IRV_CHART_TITLE)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_SLIDER_TOPCHART_LED)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_EDIT_TOPCHART_LED)->ShowWindow(SW_HIDE);
	}


	m_sliderCam3Led.SetRange(0, 255);

	//m_sliderTopChartLed.SetRange(0, value);

	m_sliderCam1Led.SetTicFreq(20);
	m_sliderCam2Led.SetTicFreq(20);
	m_sliderCam3Led.SetTicFreq(20);
	m_sliderLensLed.SetTicFreq(32);
	//m_sliderTopChartLed.SetTicFreq(20);
	
	//GetDlgItem(IDC_RADIO_MTF2)->SetWindowText("Top1 Chart");


	 m_bTopChart1.m_iStateBtn = 1;
	 m_bTopChart2.m_iStateBtn = 0;
	 m_bTopChart3.m_iStateBtn = 0;
	 m_bTopChart4.m_iStateBtn = 0;
	 m_bTopChart5.m_iStateBtn = 0;
	 m_bTopChart6.m_iStateBtn = 0;
	 m_bLeftChart.m_iStateBtn = 0;
	 m_bRightChart.m_iStateBtn = 0;

	 m_bSensorCam1.m_iStateBtn = 1;
	 m_bHolderCam1.m_iStateBtn = 0;
	 m_bEpoxyInspCam1.m_iStateBtn = 0;
	 m_bLensCam1.m_iStateBtn = 0;

	 m_bStainOc.m_iStateBtn = 1;
	 m_bDefectOc.m_iStateBtn = 0;
	 //
	 //LightControl.ctrlLedVolume(m_Align_Sel_Index, model.m_iLedValue[(m_AlignData_Sel_Index)]);
	// LightControl.ctrlLedVolume(m_Bchart_Sel_Index, model.m_iLedValue[(m_BchartData_Sel_Index)]);
	 //LightControl.ctrlLedVolume(m_Oc_Sel_Index, model.m_iLedValue[(m_OcData_Sel_Index)]);
#if (__MACHINE_MODEL == MACHINE_1ST)
	 GetDlgItem(IDC_BTN_TOP1_CHARTLED)->SetWindowText("TOP1");
	 GetDlgItem(IDC_BTN_TOP2_CHARTLED)->SetWindowText("TOP2");
	 GetDlgItem(IDC_BTN_TOP3_CHARTLED)->SetWindowText("TOP3");
	 GetDlgItem(IDC_BTN_TOP4_CHARTLED)->SetWindowText("TOP4");
	 if (sysData.m_iIrChartUse == 1)
	 {
		 GetDlgItem(IDC_BTN_TOP3_CHARTLED)->ShowWindow(SW_HIDE);
		 GetDlgItem(IDC_BTN_TOP4_CHARTLED)->ShowWindow(SW_HIDE);
		 GetDlgItem(IDC_BTN_TOP5_CHARTLED)->ShowWindow(SW_HIDE);
	 }

	 GetDlgItem(IDC_BTN_LEFT_CHARTLED)->ShowWindow(SW_HIDE);
	 GetDlgItem(IDC_BTN_RIGHT_CHARTLED)->ShowWindow(SW_HIDE);
#else
	 GetDlgItem(IDC_BTN_TOP1_CHARTLED)->SetWindowText("TOP RGB");
	 GetDlgItem(IDC_BTN_TOP2_CHARTLED)->SetWindowText("TOP IR");
	 GetDlgItem(IDC_BTN_TOP3_CHARTLED)->ShowWindow(SW_HIDE);
	 GetDlgItem(IDC_BTN_TOP4_CHARTLED)->ShowWindow(SW_HIDE);
#endif
	 //if (sysData.m_iIrChartUse == 1)
	 //{
		// GetDlgItem(IDC_BTN_TOP3_CHARTLED)->ShowWindow(SW_HIDE);
		// GetDlgItem(IDC_BTN_TOP4_CHARTLED)->ShowWindow(SW_HIDE);
		// GetDlgItem(IDC_BTN_TOP5_CHARTLED)->ShowWindow(SW_HIDE);
	 //}
	 //else
	 //{
		///* GetDlgItem(IDC_BTN_TOP2_CHARTLED)->ShowWindow(SW_HIDE);*/
		// GetDlgItem(IDC_BTN_TOP3_CHARTLED)->ShowWindow(SW_HIDE);
		// GetDlgItem(IDC_BTN_TOP4_CHARTLED)->ShowWindow(SW_HIDE);
		// GetDlgItem(IDC_BTN_TOP5_CHARTLED)->ShowWindow(SW_HIDE);
		///* GetDlgItem(IDC_BTN_LEFT_CHARTLED)->ShowWindow(SW_HIDE);
		// GetDlgItem(IDC_BTN_RIGHT_CHARTLED)->ShowWindow(SW_HIDE);*/
	 //}

	UpdateData(false);
}

void CLightDlg::OnClickedOc(UINT nID)
{
	 m_bStainOc.m_iStateBtn = 0;
	 m_bDefectOc.m_iStateBtn = 0;
	 switch(nID)
	 {
		case IDC_BTN_OC_6500K_LED:
			 m_bStainOc.m_iStateBtn = 1;
			 m_Oc_Sel_Index = LIGHT_OC_6500K;
			 m_OcData_Sel_Index = LEDDATA_6500K;// LEDDATA_STAIN;
			break;
		case IDC_BTN_OC_2800K_LED:
			m_bDefectOc.m_iStateBtn = 1;
			m_Oc_Sel_Index = LIGHT_OC_2800K;
			m_OcData_Sel_Index = LEDDATA_2800K;//LEDDATA_DEFECT
			break;
	 } 
	 m_bStainOc.Invalidate();
	 m_bDefectOc.Invalidate();

#if (__MACHINE_MODEL == MACHINE_1ST)
	 //1호기
	 //
	 if (sysData.m_iIrChartUse == 1)
	 {
		 IrvAlignLed.Irv_Light_SetValue(model.m_iLedValue[(m_OcData_Sel_Index)], IR_OC);
	 }
	 else
	 {

#ifdef FURONTEER_OC
		 //OCControl.SendLightChange(0);
		 OCControl.SendLightLxStep(model.m_iLedValue[(m_OcData_Sel_Index)]);
#else
		 if (LGIT_MODEL_INDEX == M1_TANGERINE_5M)
		 {
			 OcLight_Dms50v52.DMS_50V5_2_Value(1, model.m_iLedValue[(m_OcData_Sel_Index)]);
		 }
		 else
		 {
			 LightControlthird.ctrlLedVolume(m_Oc_Sel_Index, model.m_iLedValue[(m_OcData_Sel_Index)]);
		 }
		 
#endif

	 }
#else
	 //2호기
	 //
	 if (sysData.m_iIrChartUse == 1)
	 {
		 LightControlthird.ctrlLedVolume(IR_OC, model.m_iLedValue[(m_OcData_Sel_Index)]);
	 }
	 else
	 {
		 LightControlthird.ctrlLedVolume(LIGHT_OC, model.m_iLedValue[(m_OcData_Sel_Index)]);
	 }
#endif
	 

	
	 dispData_CH2(m_Oc_Sel_Index);

}
void CLightDlg::OnClickedCam1(UINT nID)
{
	 m_bSensorCam1.m_iStateBtn = 0;
	 m_bHolderCam1.m_iStateBtn = 0;
	 m_bEpoxyInspCam1.m_iStateBtn = 0;
	 m_bLensCam1.m_iStateBtn = 0;

	 switch(nID)
	 {

		case IDC_BTN_SENSOR_CAM1_LED:  
			 m_bSensorCam1.m_iStateBtn = 1;
			 m_Align_Sel_Index = LIGHT_PCB;
			 m_AlignData_Sel_Index = LEDDATA_SENSOR;
			break;
		case IDC_BTN_HOLDER_CAM1_LED:  
			 m_bHolderCam1.m_iStateBtn = 1;
			 m_Align_Sel_Index = LIGHT_PCB;
			 m_AlignData_Sel_Index = LEDDATA_HOLDER;
			break;
		case IDC_BTN_EPOXYINSP_CAM1_LED:  
			 m_bEpoxyInspCam1.m_iStateBtn = 1;
			 m_Align_Sel_Index = LIGHT_PCB;
			 m_AlignData_Sel_Index = LEDDATA_EPOXY_INSP;
			break;
		case IDC_BTN_LENS_CAM1_LED:
			 m_bLensCam1.m_iStateBtn = 1;
			 m_Align_Sel_Index = LIGHT_PCB;
			 m_AlignData_Sel_Index = LEDDATA_LENS;
/*#if (____AA_WAY == PCB_TILT_AA)
			 m_Align_Sel_Index = LIGHT_LENS;
#elif (____AA_WAY == LENS_TILT_AA)
			  m_Align_Sel_Index = LIGHT_PCB;
#endif
		*/	
			 
			 break;
	}
	 LightControlthird.ctrlLedVolume(m_Align_Sel_Index, model.m_iLedValue[(m_AlignData_Sel_Index)]);
	m_bSensorCam1.Invalidate();
	m_bHolderCam1.Invalidate();
	m_bEpoxyInspCam1.Invalidate();
	m_bLensCam1.Invalidate();

	dispData_CH3(m_Align_Sel_Index);
}
void CLightDlg::OnClickedTopChart(UINT nID)
{
	m_bTopChart1.m_iStateBtn = 0;
	m_bTopChart2.m_iStateBtn = 0;
	m_bTopChart3.m_iStateBtn = 0;
	m_bTopChart4.m_iStateBtn = 0;
	m_bTopChart5.m_iStateBtn = 0;
	m_bTopChart6.m_iStateBtn = 0;
	m_bLeftChart.m_iStateBtn = 0;
	m_bRightChart.m_iStateBtn = 0;
	switch(nID)
	{
		case IDC_BTN_TOP1_CHARTLED:  
			 m_bTopChart1.m_iStateBtn = 1;
			 m_Bchart_Sel_Index = LIGHT_TOP_CHART1;
			 m_BchartData_Sel_Index = LEDDATA_TOP1_CHART;
			break;
		case IDC_BTN_TOP2_CHARTLED:  
			 m_bTopChart2.m_iStateBtn = 1;
			 m_Bchart_Sel_Index = LIGHT_TOP_CHART2;
			 m_BchartData_Sel_Index = LEDDATA_TOP2_CHART;
			break;
		case IDC_BTN_TOP3_CHARTLED:  
			 m_bTopChart3.m_iStateBtn = 1;
			 m_Bchart_Sel_Index = LIGHT_TOP_CHART3;
			 m_BchartData_Sel_Index = LEDDATA_TOP3_CHART;
			break;
		case IDC_BTN_TOP4_CHARTLED:  
			 m_bTopChart4.m_iStateBtn = 1;
			 m_Bchart_Sel_Index = LIGHT_TOP_CHART4;//, 
			 m_BchartData_Sel_Index = LEDDATA_TOP4_CHART;
			break;
		case IDC_BTN_LEFT_CHARTLED:  
			 m_bLeftChart.m_iStateBtn = 1;
			 m_Bchart_Sel_Index = LIGHT_LEFT_CHART;
			 m_BchartData_Sel_Index = LEDDATA_CHART_L;
			break;
		case IDC_BTN_RIGHT_CHARTLED:  //너가 리어 차트다
			m_bRightChart.m_iStateBtn = 1;
			m_Bchart_Sel_Index = LIGHT_RIGHT_CHART;//8;
			m_BchartData_Sel_Index = LEDDATA_CHART_R;
			break;

	}
#if (__MACHINE_MODEL == MACHINE_1ST)
	if (sysData.m_iIrChartUse == 1)
	{
		IrvAlignLed.Irv_Light_SetValue(model.m_iLedValue[(m_BchartData_Sel_Index)], IR_CHART);
	}
	else
	{
		if (m_Bchart_Sel_Index == LIGHT_LEFT_CHART || m_Bchart_Sel_Index == LIGHT_RIGHT_CHART)
		{
			LightControlSecond.ctrlLedVolume((m_Bchart_Sel_Index - LIGHT_RIGHT_CHART), model.m_iLedValue[(m_BchartData_Sel_Index)]);
		}
		else
		{
			LightControl.ctrlLedVolume((m_Bchart_Sel_Index), model.m_iLedValue[(m_BchartData_Sel_Index)]);
		}
	}
#else
	if (m_Bchart_Sel_Index == LIGHT_RIGHT_CHART)
	{
		LightControlFourth.ctrlLedVolume((m_Bchart_Sel_Index - LIGHT_RIGHT_CHART), model.m_iLedValue[(m_BchartData_Sel_Index)]);
	}
	else if (m_Bchart_Sel_Index == LIGHT_LEFT_CHART)
	{
		LightControlFifth.ctrlLedVolume((m_Bchart_Sel_Index - LIGHT_LEFT_CHART), model.m_iLedValue[(m_BchartData_Sel_Index)]);
	}
	else if (m_Bchart_Sel_Index == LIGHT_TOP_CHART2)			//ir chart
	{
		LightControlSecond.ctrlLedVolume(0, model.m_iLedValue[(m_BchartData_Sel_Index)]);
		LightControl.ctrlLedVolume(0, 0);
	}
	else
	{
		LightControl.ctrlLedVolume((m_Bchart_Sel_Index), model.m_iLedValue[(m_BchartData_Sel_Index)]);
		LightControlSecond.ctrlLedVolume(0, 0);
	}
#endif
		
		

		m_bTopChart1.Invalidate();
		m_bTopChart2.Invalidate();
		m_bTopChart3.Invalidate();
		m_bTopChart4.Invalidate();
		m_bTopChart5.Invalidate();
		m_bTopChart6.Invalidate();
		m_bLeftChart.Invalidate();
		m_bRightChart.Invalidate();
	
	dispData_CH1(m_Bchart_Sel_Index);
}

void CLightDlg::dispData(int channel)
{
	if (m_iSelLED<0)			m_iSelLED = 0;

	CString tmpStr = "";
	if (Task.AutoFlag != 1)
	{
		
		
		//m_sliderLensLed.SetPos(model.m_iLedValue[LEDDATA_LENS]);//lens 컨트롤러 고정
		//m_sliderTopChartLed.SetPos(model.m_iLedValue[LEDDATA_TOP1_CHART]); //irv 차트
																		   //
	}
	else
	{
		//자동운전중 변경 안됨
	}
	UpdateData(false);
}


void CLightDlg::dispData_second(int channel)
{
	if (m_iSelLED<0)			m_iSelLED = 0;

	CString tmpStr="";
	if(Task.AutoFlag != 1)
	{
		if(channel == LIGHT_OC_6500K || channel == LIGHT_OC_2800K)
		{
			tmpStr.Format("%d",model.m_iLedValue[m_OcData_Sel_Index]);
			GetDlgItem(IDC_EDIT_CAM2_LED)->SetWindowText(tmpStr);

			m_sliderCam2Led.SetPos(model.m_iLedValue[m_OcData_Sel_Index]);
		}
		//
	}else
	{
		//자동운전중 변경 안됨
	}
	UpdateData(false);
}



void CLightDlg::dispData_CH1(int channel)
{
	if (m_iSelLED<0)			m_iSelLED = 0;

	CString tmpStr = "";
	if (Task.AutoFlag != 1)
	{
		if (channel == LIGHT_LEFT_CHART || channel == LIGHT_RIGHT_CHART || channel == LIGHT_TOP_CHART1 || channel == LIGHT_TOP_CHART2 || channel == LIGHT_TOP_CHART3 || channel == LIGHT_TOP_CHART4)
		{
			tmpStr.Format("%d", model.m_iLedValue[(m_BchartData_Sel_Index)]);
			GetDlgItem(IDC_EDIT_CAM1_LED)->SetWindowText(tmpStr);
			m_sliderCam1Led.SetPos(model.m_iLedValue[m_BchartData_Sel_Index]);
		}
	}

	UpdateData(false);
}


void CLightDlg::dispData_CH2(int channel)
{
	if (m_iSelLED<0)			m_iSelLED = 0;

	CString tmpStr = "";
	if (Task.AutoFlag != 1)
	{
		if (channel == LIGHT_OC_6500K || channel == LIGHT_OC_2800K)
		{
			tmpStr.Format("%d", model.m_iLedValue[m_OcData_Sel_Index]);
			GetDlgItem(IDC_EDIT_CAM2_LED)->SetWindowText(tmpStr);
			m_sliderCam2Led.SetPos(model.m_iLedValue[m_OcData_Sel_Index]);
		}

	}
	else
	{
		//자동운전중 변경 안됨
	}
	UpdateData(false);
}

void CLightDlg::dispData_CH3(int channel)
{
	if (m_iSelLED<0)			m_iSelLED = 0;

	CString tmpStr = "";
	if (Task.AutoFlag != 1)
	{
		if (channel == LIGHT_PCB || channel == LIGHT_LENS)
		{
			tmpStr.Format("%d", model.m_iLedValue[m_AlignData_Sel_Index]);
			GetDlgItem(IDC_EDIT_CAM3_LED)->SetWindowText(tmpStr);
			m_sliderCam3Led.SetPos(model.m_iLedValue[m_AlignData_Sel_Index]);
		}

	}
	else
	{
		//자동운전중 변경 안됨
	}
	UpdateData(false);
}


void CLightDlg::OnShowWindow(BOOL bShow, UINT nStatus)
{
	CDialogEx::OnShowWindow(bShow, nStatus);
	if (bShow)
	{
#if (__MACHINE_MODEL == MACHINE_1ST)
		if (sysData.m_iIrChartUse == 1)
		{
			/*GetDlgItem(IDC_BTN_TOP1_CHARTLED)->SetWindowText("TOP IR");*/
			/*GetDlgItem(IDC_BTN_TOP2_CHARTLED)->ShowWindow(SW_HIDE);*/
			GetDlgItem(IDC_BTN_TOP3_CHARTLED)->ShowWindow(SW_HIDE);
			GetDlgItem(IDC_BTN_TOP4_CHARTLED)->ShowWindow(SW_HIDE);
			GetDlgItem(IDC_BTN_TOP5_CHARTLED)->ShowWindow(SW_HIDE);
			/*GetDlgItem(IDC_BTN_LEFT_CHARTLED)->ShowWindow(SW_HIDE);
			GetDlgItem(IDC_BTN_RIGHT_CHARTLED)->ShowWindow(SW_HIDE);*/
		}
		else
		{
			GetDlgItem(IDC_BTN_TOP5_CHARTLED)->ShowWindow(SW_HIDE);
			
		}
#else

#endif

		dispData_CH1(m_Bchart_Sel_Index);
		dispData_CH2(m_Oc_Sel_Index);
		dispData_CH3(m_Align_Sel_Index);
	}
}


//void CLightDlg::OnBnClickedRadioLens()
//{
//	m_iSelLED = LIGHT_PCB;
//	m_iSelData =  LEDDATA_SENSOR;
//	dispData();
//	UpdateData(true);
//}


//void CLightDlg::OnBnClickedRadioPcb()
//{
//	m_iSelLED = LIGHT_PCB;
//	m_iSelData =LEDDATA_HOLDER;
//	dispData();
//	UpdateData(false);
//}



void CLightDlg::OnClickedButtonSave()
{
	//bool passFlag = password.m_bFlag;
	CString strTemp="";
	//if(passFlag == false)
	//{
	//	CKeyPadDlg keyDlg;
	//	keyDlg.m_strKeyPad.Format("Password를 입력하세요.");

	//	if (keyDlg.DoModal()==IDOK)
	//	{
	//		strTemp = keyDlg.GetstrKeypad();
	//		passFlag = strTemp.CompareNoCase(password.m_szPassword) ? false : true;
	//	}
	//	else
	//	{
	//		return;
	//	}
	//}


	//if(passFlag) 
	//{
	//	password.m_bFlag = true;
	//}
	//else
	//{
	//	strTemp.Format("Password를 잘 못 입력했습니다.");
	//	errMsg2(Task.AutoFlag, strTemp);	//"Password를 잘 못 입력했습니다."
	//	return;
	//}
	/*if (sysData.m_iIrChartUse == 1)
	{
		GetDlgItem(IDC_EDIT_CAM1_LED)->GetWindowText(strTemp);
		model.m_iLedValue[(m_BchartData_Sel_Index)] = atoi(strTemp);
	}*/
	
	//chart
	GetDlgItem(IDC_EDIT_CAM1_LED)->GetWindowText(strTemp);
	model.m_iLedValue[(m_BchartData_Sel_Index)] = atoi(strTemp);

	//stain
	GetDlgItem(IDC_EDIT_CAM2_LED)->GetWindowText(strTemp);
	model.m_iLedValue[m_OcData_Sel_Index]=atoi(strTemp);
	
	//align
	GetDlgItem(IDC_EDIT_CAM3_LED)->GetWindowText(strTemp);
	model.m_iLedValue[m_AlignData_Sel_Index] = atoi(strTemp);
	model.Save();
}

void CLightDlg::OnBnClickedOk()
{
	CDialogEx::OnOK();
}



void CLightDlg::OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar)
{
	CString strTemp;
	if(Task.AutoFlag == 1)
	{
		return;
	}
	if (pScrollBar->m_hWnd == GetDlgItem(IDC_SLIDER_CAM1_LED)->m_hWnd)
	{
		if (sysData.m_iIrChartUse == 1)
		{
			//strTemp.Format("%d.%d", (m_sliderCam1Led.GetPos() / 10), (m_sliderCam1Led.GetPos() % 10));
			strTemp.Format("%d", m_sliderCam1Led.GetPos());
			GetDlgItem(IDC_EDIT_CAM1_LED)->SetWindowText(strTemp);
#if (__MACHINE_MODEL == MACHINE_1ST)
			IrvAlignLed.Irv_Light_SetValue(m_sliderCam1Led.GetPos(), IR_CHART);
#else

			LightControlSecond.ctrlLedVolume(0, m_sliderCam1Led.GetPos());
			LightControl.ctrlLedVolume(0, 0);
#endif
		}
		else
		{
			strTemp.Format("%d", m_sliderCam1Led.GetPos());
			GetDlgItem(IDC_EDIT_CAM1_LED)->SetWindowText(strTemp);
#if (__MACHINE_MODEL == MACHINE_1ST)
			if (m_Bchart_Sel_Index == LIGHT_LEFT_CHART || m_Bchart_Sel_Index == LIGHT_RIGHT_CHART)
			{
				LightControlSecond.ctrlLedVolume((m_Bchart_Sel_Index - LIGHT_RIGHT_CHART), m_sliderCam1Led.GetPos());
			}
			else
			{
				LightControl.ctrlLedVolume((m_Bchart_Sel_Index), m_sliderCam1Led.GetPos());
			}
#else


			if (m_Bchart_Sel_Index == LIGHT_RIGHT_CHART)
			{
				LightControlFourth.ctrlLedVolume((m_Bchart_Sel_Index - LIGHT_RIGHT_CHART), m_sliderCam1Led.GetPos());
			}
			else if (m_Bchart_Sel_Index == LIGHT_LEFT_CHART)
			{
				LightControlFifth.ctrlLedVolume((m_Bchart_Sel_Index - LIGHT_LEFT_CHART), m_sliderCam1Led.GetPos());
			}
			else if (m_Bchart_Sel_Index == LIGHT_TOP_CHART2)		//ir chart
			{
				LightControlSecond.ctrlLedVolume(0, m_sliderCam1Led.GetPos());
				LightControl.ctrlLedVolume(0, 0);
			}
			else
			{
				LightControl.ctrlLedVolume((m_Bchart_Sel_Index), m_sliderCam1Led.GetPos());
				LightControlSecond.ctrlLedVolume(0, 0);
			}	
#endif
		}
	}
	if (pScrollBar->m_hWnd == GetDlgItem(IDC_SLIDER_CAM2_LED)->m_hWnd)
	{
		if (sysData.m_iIrChartUse == 1)
		{
			//strTemp.Format("%d.%d", (m_sliderCam2Led.GetPos() / 10), (m_sliderCam2Led.GetPos() % 10));
			strTemp.Format("%d", m_sliderCam2Led.GetPos());
			GetDlgItem(IDC_EDIT_CAM2_LED)->SetWindowText(strTemp);
			IrvAlignLed.Irv_Light_SetValue(m_sliderCam2Led.GetPos(), IR_OC);
		}
		else
		{
			strTemp.Format("%d", m_sliderCam2Led.GetPos());
			GetDlgItem(IDC_EDIT_CAM2_LED)->SetWindowText(strTemp);
#if (__MACHINE_MODEL == MACHINE_1ST)
			//1호기
#ifdef FURONTEER_OC
			int ndata = m_sliderCam2Led.GetPos();
			OCControl.SendLightLxStep(ndata);
#else
			if (LGIT_MODEL_INDEX == M1_TANGERINE_5M)
			{
				OcLight_Dms50v52.DMS_50V5_2_Value(1, m_sliderCam2Led.GetPos());
			}
			else
			{
				LightControlthird.ctrlLedVolume(LIGHT_OC_6500K, m_sliderCam2Led.GetPos());
			}
			
#endif



#else
			//2호기
			LightControlthird.ctrlLedVolume(4 , m_sliderCam2Led.GetPos());
#endif
		}
	}
	if (pScrollBar->m_hWnd == GetDlgItem(IDC_SLIDER_CAM3_LED)->m_hWnd)
	{
		strTemp.Format("%d", m_sliderCam3Led.GetPos());
		GetDlgItem(IDC_EDIT_CAM3_LED)->SetWindowText(strTemp);
		LightControlthird.ctrlLedVolume(m_Align_Sel_Index, m_sliderCam3Led.GetPos());
	}

	CDialogEx::OnHScroll(nSBCode, nPos, pScrollBar);
}

void CLightDlg::OnStnClickedEditLed()
{
	CString celData="";
	int iTemp=0;
	m_LabelChartLight.GetWindowTextA(celData);
	CGetNumDlg dlg(10, (LPTSTR)(LPCTSTR)celData);
	if(dlg.DoModal() == IDOK)
	{
		m_LabelChartLight.SetText(dlg.GetstrNum());
	}	

	iTemp = atoi(dlg.GetstrNum());
	m_sliderCam1Led.SetPos(iTemp);
	CString tmpStr = _T("");
	if(Task.AutoFlag != 1)
	{
		//int led = m_iSelLED;
		
		tmpStr.Format("%d", m_sliderCam1Led.GetPos());
		GetDlgItem(IDC_EDIT_CAM1_LED)->SetWindowText(tmpStr);

		if (sysData.m_iIrChartUse == 1)
		{
#if (__MACHINE_MODEL == MACHINE_1ST)
			IrvAlignLed.Irv_Light_SetValue(0, m_sliderCam1Led.GetPos());
#else

			LightControlSecond.ctrlLedVolume(0, m_sliderCam1Led.GetPos());
#endif
		}
		else
		{
#if (__MACHINE_MODEL == MACHINE_1ST)
			if (m_Bchart_Sel_Index == LIGHT_LEFT_CHART || m_Bchart_Sel_Index == LIGHT_RIGHT_CHART)
			{
				LightControlSecond.ctrlLedVolume((m_Bchart_Sel_Index - LIGHT_RIGHT_CHART), m_sliderCam1Led.GetPos());
			}
			else
			{
				LightControl.ctrlLedVolume((m_Bchart_Sel_Index), m_sliderCam1Led.GetPos());
			}
#else

			if ( m_Bchart_Sel_Index == LIGHT_RIGHT_CHART)
			{
				LightControlFourth.ctrlLedVolume((m_Bchart_Sel_Index - LIGHT_RIGHT_CHART), m_sliderCam1Led.GetPos());
			}
			else if (m_Bchart_Sel_Index == LIGHT_LEFT_CHART)
			{
				LightControlFifth.ctrlLedVolume((m_Bchart_Sel_Index - LIGHT_RIGHT_CHART), m_sliderCam1Led.GetPos());
			}
			else if (m_Bchart_Sel_Index == LIGHT_TOP_CHART2)			//ir chart
			{
				LightControlSecond.ctrlLedVolume(0, m_sliderCam1Led.GetPos());
				LightControl.ctrlLedVolume(0, 0);
			}
			else
			{
				LightControl.ctrlLedVolume((m_Bchart_Sel_Index), m_sliderCam1Led.GetPos());
				LightControlSecond.ctrlLedVolume(0, 0);
			}
#endif
		}
	}
}



void CLightDlg::OnBnClickedButtonPwrOnoff()
{

#if (__MACHINE_MODEL == MACHINE_1ST)
	IrvAlignLed.Irv_Light_SetValue(50, IR_OC);
	IrvAlignLed.Irv_Light_SetValue(50, IR_CHART);
#else

	IrvAlignLed.Irv_Light_SetValue(50, IR_OC);
	LightControlSecond.ctrlLedVolume(50, IR_CHART);
#endif
}

void CLightDlg::OnStnClickedEditLensLed()
{
	CString celData="";
	int iTemp=0;
	CGetNumDlg dlg(10, (LPTSTR)(LPCTSTR)celData);
	if(dlg.DoModal() == IDOK)
	{
		m_LabelLensLight.SetText(dlg.GetstrNum());

	}	

	iTemp = atoi(dlg.GetstrNum());
	m_sliderLensLed.SetPos(iTemp);

	if(Task.AutoFlag != 1)
	{
		//int led = m_iSelLED;
		LightControl.ctrlLedVolume(LIGHT_LENS, m_sliderLensLed.GetPos());
	}
}


void CLightDlg::OnStnClickedEditOcLed()
{
	CString celData="";
	int iTemp=0;
	CGetNumDlg dlg(10, (LPTSTR)(LPCTSTR)celData);


	if(dlg.DoModal() == IDOK)
	{
		iTemp = atoi(dlg.GetstrNum()); 
#ifdef FURONTEER_OC
		if (iTemp < 0)
		{
			iTemp = 0;
		}
		if (iTemp > 5)
		{
			iTemp = 5;
		}
#endif
		
		celData.Format("%d", iTemp);
		m_LabelOcLight.SetText(celData);
		//m_LabelOcLight.SetText(celData);
	}	
	else
	{
		return;
	}
	CString tmpStr = _T("");
	m_sliderCam2Led.SetPos(iTemp);
	
	if(Task.AutoFlag != 1)
	{
		tmpStr.Format("%d", m_sliderCam2Led.GetPos());
		GetDlgItem(IDC_EDIT_CAM2_LED)->SetWindowText(tmpStr);

		if (sysData.m_iIrChartUse == 1)
		{
			IrvAlignLed.Irv_Light_SetValue(m_sliderCam2Led.GetPos(), IR_OC);
		}
		else
		{
#if (__MACHINE_MODEL == MACHINE_1ST)
			//1호기
#ifdef FURONTEER_OC
			OCControl.SendLightLxStep(m_sliderCam2Led.GetPos());
#else
			if (LGIT_MODEL_INDEX == M1_TANGERINE_5M)
			{
				OcLight_Dms50v52.DMS_50V5_2_Value(1, m_sliderCam2Led.GetPos());
			}
			else
			{
				LightControlthird.ctrlLedVolume(m_Oc_Sel_Index, m_sliderCam2Led.GetPos());
			}
			

#endif




#else
			//2호기
			LightControlthird.ctrlLedVolume(m_Oc_Sel_Index, m_sliderCam2Led.GetPos());
#endif
		}
		
	}
}




void CLightDlg::OnStnClickedEditTopchartLed()
{
	CString celData="";
	int iTemp=0;
	CGetNumDlg dlg(10, (LPTSTR)(LPCTSTR)celData);
	if(dlg.DoModal() == IDOK)
	{
		celData.Format("%d.%d", (_ttoi(dlg.GetstrNum())/10),(_ttoi(dlg.GetstrNum())%10));
		m_LabelTopChartLight.SetText(celData);
	}	

	iTemp = atoi(dlg.GetstrNum());
	m_sliderTopChartLed.SetPos(iTemp);

	if(Task.AutoFlag != 1)
	{
		//int led = m_iSelLED;

		//LightControl.ctrlLedVolume(LIGHT_OUTSIDE, m_sliderTopChartLed.GetPos());
	}
}


void CLightDlg::OnStnClickedEditBchartLed()
{
	CString celData="";
	int iTemp=0;
	GetDlgItem(IDC_EDIT_CAM3_LED)->GetWindowText(celData);
	CGetNumDlg dlg(10, (LPTSTR)(LPCTSTR)celData);
	if(dlg.DoModal() == IDOK)
	{
		m_LabelAlignLight.SetText(dlg.GetstrNum());
	}	

	iTemp = atoi(dlg.GetstrNum());
	m_sliderCam1Led.SetPos(iTemp);

	if(Task.AutoFlag != 1)
	{
		//int led = m_iSelLED;
		LightControlthird.ctrlLedVolume((m_Align_Sel_Index), m_sliderCam1Led.GetPos());
	}
	m_LabelAlignLight.Invalidate();
}


void CLightDlg::OnBnClickedBtnChartChange()
{
	CString strTemp;
	GetDlgItem(IDC_EDIT_CAM3_LED)->GetWindowText(strTemp);
	for(int i = LIGHT_LEFT_CHART; i <= LIGHT_TOP_CHART3; i++)
	{
		LightControl.ctrlLedVolume( i , model.m_iLedValue[(i)]);
		model.m_iLedValue[i] = atoi(strTemp);
	}
	model.Save();
}


void CLightDlg::OnBnClickedBtnTop1Chartled()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
}


void CLightDlg::OnBnClickedBtnChartLedOff()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	if (Task.AutoFlag != 1)
	{
		onOffFlag = !onOffFlag;
		CString str = _T("");
		if (onOffFlag)
		{
			str.Format("CHART LED ON");
		}
		else
		{
			str.Format("CHART LED OFF");
		}
		GetDlgItem(IDC_BTN_CHART_LED_OFF)->SetWindowText(str);
#if (__MACHINE_MODEL == MACHINE_1ST)
		if (sysData.m_iIrChartUse == 1)
		{
			IrvAlignLed.IrvLight_Power(0, IR_CHART);
		}
		else
		{
			LightControl.allControl(m_Bchart_Sel_Index, onOffFlag);// , m_sliderCam1Led.GetPos());
		}
#else
		//LightControl.allControl(m_Bchart_Sel_Index, onOffFlag);// , m_sliderCam1Led.GetPos());
		if (onOffFlag)
		{
			LightControl.ctrlLedVolume(0, model.m_iLedValue[LEDDATA_TOP1_CHART]);
			LightControlFourth.ctrlLedVolume(0, model.m_iLedValue[LEDDATA_CHART_R]);
			LightControlFifth.ctrlLedVolume(0, model.m_iLedValue[LEDDATA_CHART_L]);
		}
		else
		{
			LightControl.ctrlLedVolume(0, 0);
			LightControlFourth.ctrlLedVolume(0, 0);
			LightControlFifth.ctrlLedVolume(0, 0);
		}
		
#endif
//		if (sysData.m_iIrChartUse == 1)
//		{
//#if (__MACHINE_MODEL == MACHINE_1ST)
//			IrvAlignLed.IrvLight_Power(0, IR_CHART);
//#else
//
//			LightControlSecond.allControl(0, 0);
//#endif
//		}
//		else
//		{
//			LightControl.allControl(m_Bchart_Sel_Index, onOffFlag);// , m_sliderCam1Led.GetPos());
//		}
	}
}


void CLightDlg::OnBnClickedButtonLightClose()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	ShowWindow(SW_HIDE);
	//CDialogEx::OnCancel();
}
