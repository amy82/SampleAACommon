#pragma once
#ifndef __GLOBAL_DEFINE
#define __GLOBAL_DEFINE


//#define		NORINDA_MODE

#define		NO_CIRCLE_MODEL			//TENGERING_M5
//=======================================================================
#define	PCB_TILT_AA				0
#define LENS_TILT_AA			1

#define ____AA_WAY					LENS_TILT_AA	//PCB_TILT_AA, LENS_TILT_AA


#define	MACHINE_1ST				110
#define MACHINE______2ST		304
//
#define	__MACHINE_MODEL			MACHINE_1ST



const int M2_OX01H1B = 200900;		//윤경목 선임 SFR, RI, 이물
const int M2_VB56G4A = 201000;		//윤경목 선임 SFR, RI, 이물

const int M1_TANGERINE_5M = 200800;		//김동훈 책임,
const int M2_MBOX081D_8M = 200700;		// 최종철 선임, OC, SFR, 이물, Rotation, RI, Fov, Distortion


const int M1_LSI_5M = 200600;			//241126 최승필 책임, 원형도포 기존 썬더 제품에서 개별 콜리메터 에서 평차트로 변경제품
const int M2_VB1940_5M = 200400;		//241017 LG 신용진선임신용진선임 ,  (Sensor : VB1940)  5개 패턴 , 십자가 원형마크 , 원형도포 도포 무게 0.032 , 0.004 , 0.01 로 uv offset테스트
const int M2_OV9284_5M = 200300;		//240826 LG 강찬구 책임, 신용진선임 ,  (Sensor : OV9284)  11개 패턴 , 십자가 원형마크
const int M2_FF_MODULE = 200200;		//240726 박동찬, 김세영 선임, 사각도포 얇은 제품 사격형 도포 차트 9개 LGIT_GRABBER_20240715_01_v2.2.3  epoxy무게 = 0.035
const int M2_RFPCB_5M = 200100;			//240721 최승필 책임 얇은 제품 원형 도포  차트 9개 LGIT_GRABBER_20240715_01_v2.2.3
const int M1_HEATING_GEN2 = 10050;		//240721 렌즈에 꼬리 ,강민우선임 ,차트 9개 0F, 05F, 07F
//



static int LGIT_MODEL_INDEX = M1_TANGERINE_5M;
//-----------------------------------------------------------------------------------------------------------------------------------
#define					VER_STR		"250404_3"				// DMS Value 안에 DPS OnOff 함수로 값 조절.
//#define					VER_STR		"250404_2"				//g_motorStop;
//#define					VER_STR		"250404_1"				//
//#define					VER_STR		"250403_2"				//M1_TANGERINE_5M 임시 PCB TXTY 본딩위치 티칭값으로 가게
//#define					VER_STR		"250403_1"				//sfrValue offset *1.37 
//#define					VER_STR		"250402_2"				//도포 마지막 2.5 보간 M1_TANGERINE_5M 으로 구분해서 없앰 공기 포인트 
//#define					VER_STR		"250402_1"				//
//#define					VER_STR		"250327_1"				//1호기 유스테크 이물광원 1채널 추가 OcLight_Dms50v52
//#define					VER_STR		"250327_1"			//레이져 변위 완제품, 렌즈 구동 방향 다름 PCB AA 일때만 렌즈, 완제품 if 문
//#define					VER_STR		"250326_2"			//완제품 align XY 방향 부호 반대
//#define					VER_STR		"250326_1"			// _checkMaxSfrPos ->  m_iCnt_Step_AA_Total 임시 값 -10
//#define					VER_STR		"250325_3"			//PCB AA
//#define					VER_STR		"250325_2"			//
//#define					VER_STR		"250324_3"			//Line Pulse 0.125
//#define					VER_STR		"250324_2"			//IR CHART, CCD OPEN
//#define					VER_STR		"250324_1"			//1호기 원형마크 없는 텐저링 모델 수정중 lee
//#define					VER_STR		"250320_1"			// win7, Mil9 재설치 Okey? Okey Okey
//#define					VER_STR		"250318_1"			// CCD OPEN
//#define					VER_STR		"250317_1"			// 1호기 Tengring
//#define					VER_STR		"250314_1"			// AA,EOL
//#define					VER_STR		"250313_2"			// SetFOVOffset(HFOVOffset, VFOVOffset, DFOVOffset) -> 오류
//#define					VER_STR		"250313_1"			// SetFOVOffset(HFOVOffset, VFOVOffset, DFOVOffset) UI SfrSpec 그리드로
//#define					VER_STR		"250312_4"			//_findCirclePos 원 찾음 Fov 검사 =  실패
//#define					VER_STR		"250312_2"			//원점, 영상 확인
//#define					VER_STR		"250311_1"			//DLL 변경
//#define					VER_STR		"250307_1"			//DLL 변경
//#define					VER_STR		"250304_1"			//SFR Algorithm 그리드 -> 콤보 박스로 변경
//#define					VER_STR		"250227_1"			//Tangering AA - 해상력, 이물, OC

//#define					VER_STR		"241128_1"			//1호기 miu close 수정, 차트 좌표 5,6,7,8  로 수정 9~ , 1호기 Sensor Align pass
//#define					VER_STR		"241126_1"			//1호기 M1_LSI_5M 추가
//#define					VER_STR		"241114_1"
//차트 밝기 green 150정도 노출값으로 조정 , 도포무게 = 0.035 ~ 0.039정도
//0x300B 0x05   //; exposure H
//0x300C 0x78   //; exposure L
//uv전 , uv후 , 완제품 mtf측정시 평균측정 이미지도 남도록 수정
//#define					VER_STR		"241112_1"			//M2_FF_MODULE 모델 김세영 선임 sfr , 여러번 측정 평균낸 값으로 aa 진행 요청, 수정중

//
//#define					VER_STR		"241017_1"					///// M2_VB1940_5M모델 LensVacuumOn 사용 , aps 직접 제작 , _findCirclePos 로 다시 변경

//#define					VER_STR		"240910_1"			/// M2_OV9284_5M모델 LensVacuumOn 사용
//#define					VER_STR		"240902_1"			//M2_FF_MODULE 모델 이물검사추가
//#define					VER_STR		"240819_1"				//2호기 start버튼 접점 반대로 변경
//#define					VER_STR		"240731_1"			//원형마크 찾는부분 원복
//#define					VER_STR		"240730_1"			//수동 본딩위치 자동구분
//M2_FF_MODULE 모델 센서 4군데 레이저 보정후 , 외곽 4군데 측정만 로그 저장

//#define					VER_STR		"240729_1"			//oc측정 수정 , LASER 보정량 추가
//#define					VER_STR		"240725_2"				//laser 4포인트 로그 추가 , g_GetIllumination광축 oc 추가
//#define					VER_STR		"240725_1"			//LOG THREAD 추가
//그래프 부분 Z축도 올리고 그래프 화면도 초기화해야된다.
//#define					VER_STR		"2400724_2"
//#define					VER_STR		"2400724_1"			//1,2,호기 합침
//#define					VER_STR		"2400723_6"			//heating DEMOSAICMETHOD_GRADIENT_G_ONLY
//#define					VER_STR		"2400723_5"			//pcb x,y 위치 확인 offset -로 변경
//#define					VER_STR		"2400723_4"			//th보정추가
//#define					VER_STR		"2400723_1"			//본딩 하강전 꼬리 언그립
//#define					VER_STR		"2400722_1"				//M1_RFPCB_5M 모델로 LGIT_GRABBER_20240715_01_v2.2.3 dll변경 , bmpImageSaveFn 추가
//_OpencvFindCirclePos 추가
//#define					VER_STR		"2400719_1"				//1호기 이물조명은 src 장비 프론티어 옆으로 얇은 컨트롤러 추가 , 1호기 tail 그립 동작 io 추가
//
//
//
//
//
//
//210712
#define  M_TRINITY						900					//LENS AA , X자형 차트에 패턴 9개/20포인트
//220414
#define  M_AF_CM						1000					//PCB AA , IR차트, X자형 차트에 패턴 9개/20포인트
//220511
#define  M_GEN2							2000					//모비스 MODERAT랑 동일 ROI MTF 측정 기준은 Ny/2 이므로0.25cycles/pixel 기준 입니다.
//220915
#define  M_COB_8M						3000				//M_GEN2 와 완전동일 기적으로만 달라짐

#define  M_TRINITY_WIDE					3200					//LENS AA , X자형 차트에 패턴 개별 CL 렌즈 X자형 5개 패턴  83도 , 180도
#define  M_MPC							3300					//LENS AA , X자형 차트에 패턴 개별 CL 렌즈 X자형 5개 패턴  83도 , 180도


#define ____MODEL_NAME					-999999		//M2_RFPCB_5M
//모델명 선택후 빌드하세요~

//#define		FURONTEER_OC				//1호기 이물조명 없어서 백두에서 퓨론티어로 컨트롤러오 변경
//======================================================================= Lens_Motor_Move
//


//opencv dll 추가
//PATH=$(ProjectDir)\Library\lib64;C:\Opencv346\install\x64\vc14\bin;%PATH%
//=======================================================================



//=======================================================================
//=======================================================================
#define		EVMS
#define		ON_LINE_MODE
#ifdef NORINDA_MODE
//#define		ON_LINE_VISION
//#define		ON_LINE_MOTOR
//#define		ON_LINE_DIO
//#define		ON_LINE_KEYENCE
//#define		ON_LINE_MONITOR

#else
#ifdef		ON_LINE_MODE
#define		ON_LINE_MOTOR
////#define		ON_LINE_LAN
#define		ON_LINE_DIO
#define		ON_LINE_VISION
#define		ON_LINE_KEYENCE
#define		ON_LINE_MONITOR
#endif
#endif


//=======================================================================
#define  XPENG_REAR				125		//협각
#define  XPENG_SIDE				578		//광각 = FRONT , BACK 구분 모델 클릭해서 변경

#define  VEONEER				138				//PCB AA , SFR = 50
//
#define  ZOOX_NARROW_A					200				//협각 , PCB AA , 다각형 0.234
#define  ZOOX_NARROW_B					201				//협각 , PCB AA , 다각형 0.234
#define  ZOOX_NARROW_C					202				//협각 , PCB AA , 다각형 0.234
#define  ZOOX_WIDE_D					203				//광각 , PCB AA , 다각형 0.234
#define  ZOOX_WIDE_E					204				//광각 , PCB AA , 다각형 0.234
//
#define  S_MINI_NARROW							300				//협각 , LENS AA , cellsize = 3.75 , pulse 65  , 60도
#define  S_MINI_WIDE							301				//협각 , LENS AA , cellsize = 3.75 , pulse 65 , 190도
#define  NEW_S_MINI_WIDE						400
#define  FCM30_W_PCB							500						//PCB AA 방식으로 진행 ,차트수 12개
//
#define  GCM2							600						//FCM30_W_PCB랑 같은데 사각도포

//
#define  M_DANI							700						// Lens AA type/ 화각 60도
//M_DANI = Raw 파일: SFR Chart 영상 연속으로 2장 캡쳐, 이물 광원 ON 영상 1장, 이물 광원 OFF 영상 1장
///
#define  M_DMS3							800							//렌즈 aa , 원형도포 , defect , stain 검사함 옴니비전센서 210424 (acmis dll변경)



//=======================================================================
//=======================================================================
#define  S_ONSEMI					2100		//기존 온세미 센서
#define  S_OMNIVISION				2200		//옴니비전 센서


#define ____SENSOR_TYPE					999999999		//S_OMNIVISION


//=================================================================================================================================================================


static CString sTempLang;
static CString sLangChange;

#define SWAP(a,b)		{int t; t=a; a=b; b=t;}		/* int형 스왑 */
#define SWAP_STR(a,b)	{CString t; t=a; a=b; b=t;}	/* CString형 스왑 */
#define rgb(r,g,b) (r*256*256+g*256+b)


#define PDC_LAVENDER			RGB(230,230,250)
#define PDC_GREEN					RGB(0,128,0)
#define PDC_LIGHTGREEN			RGB(144,238,144)
#define PDC_YELLOW				RGB(255,255,0)
#define PDC_BLACK					RGB(0,0,0)
#define PDC_RED						RGB(255,0,0)
#define PDC_BLUE					RGB(0,0,255)

#define CV_LAVENDER				CV_RGB(230,230,250)
#define CV_GREEN					CV_RGB(0,128,0)
#define CV_LIGHTGREEN			CV_RGB(144,238,144)
#define CV_YELLOW					CV_RGB(255,255,0)
#define CV_BLACK					CV_RGB(0,0,0)
#define CV_RED						CV_RGB(255,0,0)
#define CV_BLUE						CV_RGB(0,0,255)


enum MOTOR_SPEED { SPEED_AUTO_H = 0, SPEED_AUTO_L, SPEED_MANUAL };				//!! 모터 구동 속도
enum MODE_TYPE { MODE_STOP = 0, MODE_READY, MODE_AUTO, MODE_PAUSE, MODE_ERR };	//!! 모드 상태 변수 0=정지상태, 1=운전준비상태, 2=자동운전상태, 3=일시정지상태
enum HOME_READY_TYPE { TYPE_HOME = 0, TYPE_READY, TYPE_HOME_READY };					//!! 원점 상태 변수

#define		MAX_CAM_NO		4
#define		MAX_MARK		3
enum CAMERA { CAM1 = 0, CAM2, CAM_NULL, Cam_Cnt };

enum MARK { PCB_Chip_MARK = 0, PCB_Holder_MARK, LENS_Align_MARK, MARK_CNT };

enum CHART_FIELD { CHART_CENTER = 0, CHART_TOP, CHART_BOTTOM, CHART_LEFT, CHART_RIGHT, CHART_FIELD_CNT };
enum SFR_ROI { CT = 0, CB, CL, CR, TL, TB, BR, BT, LB, LR, RT, RL, SFR_ROI_CNT };

#define SOCKET_MAIN			"192.168.3.3"
#define SOCKET_AA1			"192.168.3.1"
#define SOCKET_AA2			"192.168.3.2"

#define SFR_COMP_MAX		// SFR계산 시 평균값, 최대치 중 선택.  정의시 최대치

#define J_RCCC	0
#define J_CRCC	1
#define J_CCRC	2
#define J_CCCR	3

#define R_RESULT_PASS					0	//양품
#define R_RESULT_FAIL					1	//불량
#define R_FAIL_NOISE					2
#define R_FAIL_BLACK_SPOT				3
#define R_FAIL_STAIN					4

#define IMG_PROC_TYPE	7


#define		USE_GEOMETRIC
#define     USE_MIL_BAYER_CONVERSION

#define _DEF_MAX_BLOBS		200

#define		CHART_CENTER_MARK	1000


enum IR_LIGH_CONTROL {
	IR_OC, IR_CHART, MAX_IR_LIGHT_CONTROL
};
enum LED_LIGHT_CH1_CONTROL {
	LIGHT_PCB, LIGHT_OC_6500K, LIGHT_OC_2800K, LIGHT_LENS, LIGHT_OC, MAX_LIGHT_CONTROL
};
enum LED_LIGHT_CH2_CONTROL {
	LIGHT_TOP_CHART1, LIGHT_TOP_CHART2, LIGHT_TOP_CHART3,
	LIGHT_TOP_CHART4, LIGHT_RIGHT_CHART, LIGHT_LEFT_CHART,
};

enum LED_LIGHT_DATA {
	LEDDATA_SENSOR, LEDDATA_HOLDER, LEDDATA_CHART_L, LEDDATA_CHART_R, LEDDATA_TOP1_CHART, LEDDATA_TOP2_CHART, LEDDATA_TOP3_CHART, LEDDATA_TOP4_CHART,
	LEDDATA_6500K, LEDDATA_EPOXY_INSP, LEDDATA_RESIN_CALC, LEDDATA_LENS, LEDDATA_OC, LEDDATA_2800K, MAX_LIGHT_DATA
};

enum	RS_232_COMM {
	COMM_LASER, COMM_BARCODE, COMM_UV,
	COMM_LIGHT1, COMM_LIGHT2, COMM_LIGHT3, COMM_UV2, COMM_LIGHT4, COMM_LIGHT5, COMM_LIGHTOC_1ST,
	MAX_RS232_NO
};


#define		SIZE_OF_100BYTE					100
#define		SIZE_OF_1K						1024

#define		MAIN_DLG_SIZE_X		1920//1280						// Main Dialog..
#define		MAIN_DLG_SIZE_Y		1080
#define		SUB_DLG_OFFSET_Y	10

#define		MARK_SIZE_X			183							// 패턴 마크 Display 크기..
#define		MARK_SIZE_Y			167

#define		MARKSET_SIZE_X		377							// 패턴 마크 Display 크기..
#define		MARKSET_SIZE_Y		333
#if (__MACHINE_MODEL == MACHINE_1ST)
#define		CAM_SIZE_X			1280						// CIS 카메라 영상 크기..
#define		CAM_SIZE_Y			960
#else
#define		CAM_SIZE_X			1280						// CIS 카메라 영상 크기..
#define		CAM_SIZE_Y			1024						//
#endif

#if 1
#define		SMALL_CAM_SIZE_X	(CAM_SIZE_X/3+100)			// 영상 화면 Display..
#define		SMALL_CAM_SIZE_Y	(CAM_SIZE_Y/3+100)

#define		SMALL_CCD_SIZE_X	(CAM_SIZE_X/3+600)			//386
#define		SMALL_CCD_SIZE_Y	(CAM_SIZE_Y/3+240)			//100


#else

#define		SMALL_CAM_SIZE_X	(CAM_SIZE_X/3+100)			// 영상 화면 Display..
#define		SMALL_CAM_SIZE_Y	(CAM_SIZE_Y/3+100)
#define		SMALL_CCD_SIZE_X	(CAM_SIZE_X/3+225)
#define		SMALL_CCD_SIZE_Y	(CAM_SIZE_Y/3+150)

#endif


#define		CAM_REDUCE_FACTOR_X		(double)SMALL_CAM_SIZE_X/CAM_SIZE_X
#define		CAM_REDUCE_FACTOR_Y		(double)SMALL_CAM_SIZE_Y/CAM_SIZE_Y
#define		CAM_EXPAND_FACTOR_X		(double)CAM_SIZE_X/SMALL_CAM_SIZE_X
#define		CAM_EXPAND_FACTOR_Y		(double)CAM_SIZE_Y/SMALL_CAM_SIZE_Y


#define		IO_DELAY_TIME			20000

#define		IN_POSITION_VAL			0.2

#define		MAX_DEVICE				2


#define		CCD				Cam_Cnt

#define		OS_PIN_COUNT		55

#define		MAX_ALARM_PAGE_CNT			20			// 알람 히스토리 창의 최대 알람 페이지 수..
#define		MAX_ALARM_LIST_IN_PAGE		24			// 알람 페이지 당 출력되는 알람 수..

#define		MAX_NG_LIST					20			// 불량 배출 시 NG 항목 등록 수..



enum OUTPUT_MODE { ASCII_MODE = 0, FLOAT_MODE, DEC_MODE, HEX_MODE };
enum E_GRAB_METHOD { LAON_CVT_COLOR, MIL_BAYER_CVT_COLOR };
enum IMAGE_SAVE_ID { LENS_IMAGE_SAVE, PCB_IMAGE_SAVE, CHIP_IMAGE_SAVE, AA_FINAL_IMAGE_SAVE, AA_NG_IMAGE_SAVE, AA_CIRCLE_NG_IMAGE_SAVE, OPTIC_IMAGE_SAVE, DEFECT_NG_IMAGE_SAVE, EPOXY_IMAGE_SAVE };
enum AA_MODE { MODE_MARKCENTER = 0, MODE_DISTORTIONCENTER, MODE_OPTICAL };
enum MTF_INSP_MODE { MTF_17P = 0, MTF_24P };

#define		COMMON_LAST_MARK_CNT			20		//버퍼 공간 , 저장 개수로만 사용
#define		COMMON_MTF_INSP_CNT				50		//버퍼 공간 , 저장 개수로만 사용


#define MAX_FOV_COUNT				4//9       //협각
#define MAX_FOV_FIND_COUNT			14

#define MAX_SNR_COUNT				3       //흰,빨,검순서


#define		SFR_ROI_VERTICAL			0//1
#define		SFR_ROI_HORIZONTAL			1//2

// Cneter SFR 검사 Box 번호..
#define		SFR_CENTER_UP					0
#define		SFR_CENTER_DOWN					1
#define		SFR_CENTER_LEFT					2
#define		SFR_CENTER_RIGHT				3
#define		SFR_CENTER_END					3

#define		SFR_05_LEFT_UP_H				4
#define		SFR_05_LEFT_UP_V				5

#define		SFR_05_RIGHT_UP_H				6
#define		SFR_05_RIGHT_UP_V				7

#define		SFR_05_LEFT_DOWN_H				8
#define		SFR_05_LEFT_DOWN_V				9

#define		SFR_05_RIGHT_DOWN_H				10
#define		SFR_05_RIGHT_DOWN_V				11
//--------------------------------------------------------else
#define		SFR_07_LEFT_UP_H				12
#define		SFR_07_LEFT_UP_V				13

#define		SFR_07_RIGHT_UP_H				14
#define		SFR_07_RIGHT_UP_V				15

#define		SFR_07_LEFT_DOWN_H				16
#define		SFR_07_LEFT_DOWN_V				17

#define		SFR_07_RIGHT_DOWN_H				18
#define		SFR_07_RIGHT_DOWN_V				19
//--------------------------------------------------------visteon






// 0.5 필드 코너부 SFR 검사 Box 번호..
#define		SFR_50_05						4
#define		SFR_50_06						5
#define		SFR_50_07						6
#define		SFR_50_08						7

// 0.7 필드 코너부 SFR 검사 Box 번호..
#define		SFR_70_09						8
#define		SFR_70_10						9
#define		SFR_70_11						10
#define		SFR_70_12						11

// 0.7 필드 코너부 SFR 검사 Box 번호..
#define		SFR_LEFT_UP_INDEX				16		// 황준혁  최종 검사관련된 내용 연계되어 있음
#define		SFR_LEFT_UP_INDEX_2				17

#define		SFR_RIGHT_UP_INDEX				18		// 황준혁  최종 검사관련된 내용 연계되어 있음
#define		SFR_RIGHT_UP_INDEX_2			19

#define		SFR_RIGHT_DOWN_INDEX			20
#define		SFR_RIGHT_DOWN_INDEX_2			21

#define		SFR_LEFT_DOWN_INDEX				22
#define		SFR_LEFT_DOWN_INDEX_2			23



#define		LED_CHART			1
#define		LED_OC				2
#define		LED_LENS			3
#define		LED_PCB				4


#define		SFR_FIRST							0
#define		SFR_SECOND							1
#define		SFR_FINAL							2
#define		OC_INSP								3
#define		SFR_BEFORE_UV						4
#define		SFR_AFTER_UV						5
#define		SFR_OFFSET_DATA						6
#define		SFR_AFTER_OFFSET_MOVE				7
#define		SFR_AFTER_TILT						8
#define		SFR_MANUAL							9
#define		MTF_INSP							10


#define		UNLOADING_INDEX		0
#define		AA1_INDEX			1
#define		AA2_INDEX			2
#define		OC_INDEX			3
#define		UV_INDEX			4


#define		RECEIVE_DATA		FALSE
#define		SEND_DATA			TRUE

enum SIDE_MODEL { SIDE_FRONT = 0, SIDE_BACK };

enum DIALOG { MAIN_DLG, MODEL_DLG, LENS_DLG, LENS_EDGE_DLG, PCB_DLG, RESIN_DLG, CCD_DLG, MOTOR_DLG, MOTOR_DLG2, MOTOR_DLG3, IO_DLG, ALARM_DLG, MOTOR_INSPDLG, PCB_INSPDLG , MOTOR_DLG4};

enum RAW_CAPTURE_TYPE { MID_6500K_RAW = 0, LOW_LEVEL_RAW, MID_2800K_RAW, UV_BEFORE_CHART, UV_AFTER_CHART, EOL_CHART, CHART_RAW, CHART_SECOND_RAW };
enum JPG_CAPTURE_TYPE { BLACKSPOT_JPG = 0, LCBJPG, YMEAN_JPG, DEFECT_MID_JPG, DEFECT_LOW_JPG, MTF_JPG };
enum FIDUCIAL_MARK_TYPE { DOT_TYPE = 0, GRID_TYPE = 2 };

enum BOX_MOVE_TYPE { MOVE_POS = 0, MOVE_SIZE };
enum SFR_MOVE_TYPE { MOVE_UP = 0, MOVE_DOWN, MOVE_LEFT, MOVE_RIGHT };

/* 패턴매칭 결과 */
enum PAT_RESULT { FIND_OK, FIND_ERR, NOMARK, RATE_ERR, ROTATE_ERR };
enum MOUSE_CURSOR { MOVE_ALL, MOVE_WIDTH_LEFT, MOVE_WIDTH_RIGHT, MOVE_HEIGHT_TOP, MOVE_HEIGHT_BOTTOM, MOVE_NW, MOVE_NE, MOVE_SW, MOVE_SE };
enum LAMP_NO { ALARM_ON, ALARM_OFF, AUTO_STOP, AUTO_RUN };

enum STEP_ID { STEP_LENS_SUPPLY, STEP_PCB_SUPPLY, STEP_OS_CHECK, STEP_RESIN_SUPPLY, STEP_CCD_ALIGN, STEP_CCD_INSP, STEP_UVLAMP, STEP_EMISSION };
enum STEP_RESULT { STEP_NORMAL, STEP_OK, STEP_NG };


enum TIMER_NUM { MAIN_TIMER = 1, MOTOR_TIMER, JOG_TIMER, IO_TIMER, LENS_TIMER, PCB_TIMER, RESIN_TIMER, MSG_TIMER, HOME_TIMER, CCD_DEFECT_TIMER, KILL_TIMER };


enum EPOXY_MODEL { CRICLE_EPOXY, RECT_EPOXY, POLYGON_EPOXY, POINT_EPOXY};

#define	MAX_MODEL_CNT			1000


enum	MOTOR_POS
{
	Wait_Pos, Loading_Pos, Holder_Pos, Dispense_Pos, Lens_Pos,
	Laser_Pcb_Pos, Laser_Lens_Pos, Align_Pos, Bonding_Pos, Dark_Pos, Defect_Pos, OC_6500K_Pos, OC_2800K_Pos,
	Pcb_Loading_Pos, Cover_Loading_Pos, Cover_UnLoading_Pos, PDispense_Pos,
	Contact_Pos, Calc_Pos, Clean_Pos, Insp_Pos, Holder_Uv_Pos, ContiTailDelay_Pos,
	Com_Laser_Pos, Calc2_Pos, CompleteAlign_Pos,
	Dispense_Pos1, Dispense_Pos2, Dispense_Pos3,
	MAX_POS_NO
};//Contact2_Pos

static char MotorPosName[MAX_POS_NO][50] = { "Wait Pos",	"Loading Pos",	"Holder Pos",	"Dispense Pos", "Lens Align",
"pcb Laser",		"Lens Laser",	"Sensor Align", "Bonding Pos", "Dark Pos", "Defect Pos", "OC 6500K Pos" , "OC 2800K Pos",
"Pcb Load Pos" , "Cover Load Pos",	"Cover UnLoad Pos",	"PDispense Pos",
"Contact Pos", "Calc Pos", "Ready Pos", "도포검사 Pos", "OQLaser Pos", "TailDelay_Pos",
"완제품LaserPos", "도포Calc2 위치", "ComAlign Pos","Dispense Pos1", "Dispense Pos2", "Dispense Pos3" };


enum	MTF_POS { MTF_Center, MTF_left_upper, MTF_right_upper, MTF_left_bottom, MTF_right_bottom, MAX_MTF_NO };
//static TCHAR* SFR_ROI_NAME[] =
//{ _T("[SFR] Roi-0"),
//
//
//};




static TCHAR* DEFECT_SPEC_NAME[26] =//[19] =
{ _T("AnBlockSizeHot"),	 _T("AnBlockSizeDark"),			_T("AnBlockSizeBright"),
_T("AnThresholdTypeHot"),		_T("AnThresholdTypeDark"),		_T("AnThresholdTypeBright"),
_T("AdDefectThresholdHot"),	_T("AdDefectThresholdDark"),	_T("AdDefectThresholdBright"),
_T("AnMaxHotDefectNum"),		_T("AnMaxDarkDefectNum"),		_T("AnMaxBrightDefectNum"),
_T("AnMaxHotClusterNum"),		_T("AnMaxDarkClusterNum"),		_T("AnMaxBrightClusterNum"),
_T("AnEnableChannel"),
_T("AnDefectInCluster"),
_T("ADefectInClusterChannel"),	//_T("AspecColumnFPN"),
_T("AEnableCircle"),
_T("APosOffsetX"),		_T("APosOffsetY"),
_T("ARadiusRatioX"),	_T("ARadiusRatioY"),
_T("AThresholdRatio"),	_T("AROIRange"),		_T("AUsediFixedCircle")
};

static TCHAR* pszXtype[5] = { _T("CENTER"), _T("TL"), _T("TR"), _T("BL"), _T("BR") };
static TCHAR* pszPlustype[5] = { _T("CENTER"), _T("T"), _T("B"), _T("L"), _T("R") };
//
static TCHAR* OC_SPEC_NAME[2] =
{
	_T("AnFiducialMarkNum"), _T("AnFiducialMarkType")
};
//
static TCHAR* COLOR_SENS_SPEC_NAME[8] =
{ _T("AdSpecDevMaxRG"),_T("AdSpecDevMinRG"),_T("AdSpecDevMaxBG"),
_T("AdSpecDevMinBG"),_T("AdSpecDevMaxGrGb"),_T("AdSpecDevMinGrGb")
,_T("AnAdjustType"),_T("AnTypicalValueType")
};
//
static TCHAR* SNR_SPEC_NAME[3] =
{ _T("AdSNRThreshold"), _T("AdSNRMIN"), _T("AdSNRMAX")
};
//


static TCHAR* UNIFORMITY_SPEC_NAME[7] =
{ _T("AnGridSizeX"),		_T("AnGridSizeY"),
_T("AnColorSpaceType"),		_T("AnMaxDiffType"),
_T("AspecMaxDiffRG"),		_T("AspecMaxDiffBG"),
_T("AspecMaxDiffRB")
};
//

static TCHAR* R_ILLUMINATION_SPEC_NAME[5] =
{ _T("AdSpecCenterIntensity1"),		_T("AdSpecCenterIntensity2"),		_T("AdSpecCenterIntensity3"),
_T("AdSpecCenterIntensity4"),		_T("AnSpecPixelCntInBlock")
};

static TCHAR* STAIN_BLEMISH_SPEC_NAME[20] =
{
	_T("AnBlockWidth"),			_T("AnBlockHeight"),		_T("AnClusterSize"),
	_T("AnDefectInCluster"),	_T("AdDefectRatio"),
	_T("AbEnableCircle"),		_T("AnPosOffsetX"),			_T("AnPosOffsetY"),
	_T("AdRadiusRatioX"),		_T("AdRadiusRatioY"),		_T("AdThresholdRatio"),
	_T("AdROIRange"),			_T("AnUsedFixedCircle"),	_T("AbEnableMultiCircle"),
	_T("AdZoneSizeRatio1"),		_T("AdZoneSizeRatio2"),		_T("AdZoneSizeRatio3"),
	_T("AdThreshold1"),			_T("AdThreshold2"),			_T("AdThreshold3")
};
const int LcbMaxCount = 35;


static TCHAR* STAIN_LCB_SPEC_NAME[LcbMaxCount] =
{
	_T("AdCenterThreshold"),		_T("AdEdgeThreshold"),		_T("AdCornerThreshold"),
	_T("AnMaxSingleDefectNum"),
	_T("AnMinDefectWidthHeigh"),
	_T("AnCenterMaxR"),				_T("AnCenterMaxGR"),			_T("AnCenterMaxGB"),	_T("AnCenterMaxB"),
	_T("AnEdgeMaxR"),				_T("AnEdgeMaxGR"),				_T("AnEdgeMaxGB"),			_T("AnEdgeMaxB"),
	_T("AnCornerMaxR"),			_T("AnCornerMaxGR"),			_T("AnCornerMaxGB"),		_T("AnCornerMaxB"),
	_T("AbEnableCircle"),		_T("AnPosOffsetX"),				_T("AnPosOffsetY"),
	_T("AdRadiusRatioX"),			_T("AdRadiusRatioY"),		_T("AdThresholdRatio"),			_T("AdROIRange"),
	_T("AnUsedFixedCircle"),		_T("AbEnableMultiCircle"),
	_T("AdZoneSizeRatio1"),		_T("AdZoneSizeRatio2"),		_T("AdZoneSizeRatio3"),
	_T("AdThreshold1"),				_T("AdThreshold2"),			_T("AdThreshold3"),
	_T("AnBlobSize1"),				_T("AnBlobSize2"),			_T("AnBlobSize3")
};
const int YmeanMaxCount = 28;
static TCHAR* STAIN_YMEAN_SPEC_NAME[YmeanMaxCount] =
{
	_T("AdEdgeSize"),				_T("AdCenterThreshold"),		_T("AdEdgeThreshold"),
	_T("AdCornerThreshold"),		_T("AnDefecBlockSize"),			_T("AnLscBlockSize"),
	_T("AnCalcType"),				_T("AnnMaxRecursiveCount"),     _T("AdMaxDefectSize"),
	_T("AdPixelSize"),
	_T("AbEnableCircle"),
	_T("AnPosOffsetX"),				_T("AnPosOffsetY"),
	_T("AdRadiusRatioX"),			_T("AdRadiusRatioY"),
	_T("AdThresholdRatio"),			_T("AdROIRange"),
	_T("AnUsedFixedCircle"),		_T("AbEnableMultiCircle"),
	_T("AdZoneSizeRatio1"),			_T("AdZoneSizeRatio2"),			_T("AdZoneSizeRatio3"),
	_T("AdThreshold1"),				_T("AdThreshold2"),				_T("AdThreshold3"),
	_T("AnBlobSize1"),				_T("AnBlobSize2"),			_T("AnBlobSize3")
};

const int FdfMaxCount = 30;
static TCHAR* STAIN_FDF_SPEC_NAME[FdfMaxCount] =		//30개
{
	_T("AdCenterThreshold"),		_T("AdEdgeThreshold"),		_T("AdCornerThreshold"),
	_T("AnMedianFilterWidth"),		_T("AnMedianFilterHeight"),
	_T("AnWidthScaleRatio"),		_T("AnHeightScaleRatio"),
	_T("AnMinDefectWidthHeight"),	_T("AnWindowSize"),
	_T("AnEdgeSize"),				_T("AbEnableChannel"),
	_T("AnMaxSingleDefectNum"),
	_T("AbEnableCircle"),
	_T("AnPosOffsetX"),				_T("AnPosOffsetY"),
	_T("AdRadiusRatioX"),			_T("AdRadiusRatioY"),
	_T("AdThresholdRatio"),			_T("AdROIRange"),
	_T("AnUsedFixedCircle"),
	_T("AbEnableMultiCircle"),
	_T("AdZoneSizeRatio1"),			_T("AdZoneSizeRatio2"),			_T("AdZoneSizeRatio3"),
	_T("AdThreshold1"),				_T("AdThreshold2"),				_T("AdThreshold3"),
	_T("AnBlobSize1"),				_T("AnBlobSize2"),			_T("AnBlobSize3")
};

static TCHAR* CHART_SPEC_NAME[5] =
{
	_T("AdRealGapX"), _T("AdRealGapY"),
	_T("AnFiducialMarkType"),		_T("AdModuleChartDistance"),
	_T("AnDistortionAlrotithmType")
};
static TCHAR* RI_SPEC_NAME[8] =
{
	_T("AnSpecPixelCntInBlock"),    _T("AnEnableChannel"),
	_T("AdSpecCenterIntensity1"),_T("AdSpecCenterIntensity2"),_T("AdSpecCenterIntensity3"),_T("AdSpecCenterIntensity4")
	,_T("AnCornerVariationMin"),_T("AnCornerVariationMax")
};

static TCHAR* SATURATION_SPEC_NAME[2] =
{
	_T("AnSpecSaturationMin"),    _T("AnSpecSaturationMax")
};



enum	MARK_DIRECTION { LEFT_BLACK = 0, RIGHT_BLACK };			// 정재호 20140905 추가.. 챠트에서 SFR 검사 영역의 BOX가 좌우 중 어느쪽이 검은 쪽인지?
enum	DIRECTION_NO { LEFT_DIR = 0, RIGHT_DIR, TOP_DIR, BOTTOM_DIR };


#define		MOTOR_Lens_Z_OFFSET			1		// 1mm  본딩 위치 이동시 1mm 위로 후 모든 모터 이동 완료후 원래 위치로 이동.

#define		RGB_DLG_BG						RGB(40, 39, 35)
#define		RGB_CTRL_BG						RGB(174,54,27)//RGB(55, 73, 93)


#define		GRID_COLOR_TITLE			RGB(0,155,155)
//
#define		GRID_COLOR_WHITE            RGB(255,255,255)
#define		GRID_COLOR_GREEN			RGB(128, 255, 128)
#define		GRID_COLOR_RED				RGB(255, 94, 164)

#define		GRID_COLOR_SPARE_MOTOR		RGB(128, 128, 128)
#define		GRID_COLOR_SELECT			RGB(244, 216, 234)

#define		GRID_COLOR_DIO_OUT			RGB(128, 255, 128)
#define		GRID_COLOR_DIO_IN			RGB(255, 94, 164)

#define		RGB_COLOR_WHITE					RGB(255, 255, 255)
#define		RGB_COLOR_BLACK					RGB(0, 0, 0)
#define		RGB_COLOR_RED					RGB(255, 0, 0)
#define		RGB_COLOR_GREEN					RGB(0, 255, 0)
#define		RGB_COLOR_BLUE					RGB(0, 0, 255)
#define		RGB_COLOR_GRAY					RGB(100, 100, 100)

#define		A_COLOR_BLACK                                 M_RGB888(0,0,0)
#define		A_COLOR_RED                                   M_RGB888(255,0,0)
#define		A_COLOR_GREEN                                 M_RGB888(0,255,0)
#define		A_COLOR_BLUE                                  M_RGB888(0,0,255)
#define		A_COLOR_YELLOW                                M_RGB888(255,255,0)
#define		A_COLOR_MAGENTA                               M_RGB888(255,0,255)
#define		A_COLOR_CYAN                                  M_RGB888(0,255,255)
#define		A_COLOR_WHITE                                 M_RGB888(255,255,255)
#define		A_COLOR_GRAY                                  M_RGB888(128,128,128)
#define		A_COLOR_BRIGHT_GRAY                           M_RGB888(192,192,192)
#define		A_COLOR_LIGHT_GRAY                            M_RGB888(160,160,164)
#define		A_COLOR_LIGHT_GREEN                           M_RGB888(192,220,192)
#define		A_COLOR_LIGHT_BLUE                            M_RGB888(166,202,240)
#define		A_COLOR_LIGHT_WHITE                           M_RGB888(255,251,240)
#define		A_COLOR_DARK_RED                              M_RGB888(128,0,0)
#define		A_COLOR_DARK_GREEN                            M_RGB888(0,128,0)
#define		A_COLOR_DARK_BLUE                             M_RGB888(0,0,128)
#define		A_COLOR_DARK_YELLOW                           M_RGB888(128,128,0)
#define		A_COLOR_DARK_MAGENTA                          M_RGB888(128,0,128)
#define		A_COLOR_DARK_CYAN                             M_RGB888(0,128,128)

#define CV_LAVENDER					CV_RGB(230,230,250)
#define CV_GREEN					CV_RGB(0,128,0)
#define CV_LIGHTGREEN				CV_RGB(144,238,144)
#define CV_YELLOW					CV_RGB(255,255,0)
#define CV_BLACK					CV_RGB(0,0,0)
#define CV_RED						CV_RGB(255,0,0)
#define CV_BLUE						CV_RGB(0,0,255)

#define		MOTOR_GAP				0.1				// Encoder  오차 허용 범위

#define		MOTOR_ORIGIN_TIME	    320000			// msec..// test중
#define		MOTOR_MOVE_TIME			50000			// msec..//test중
#define		MOTOR_STOP_TIME			1000			// msec..
#define		IO_CHECK_TIME			3000			// msec..

#define		MOTOR_JOG_LOW			0.1				//JOG_SPEED_LOW
#define		MOTOR_JOG_MID			0.5				//JOG_SPEED_MID
#define		MOTOR_JOG_HIGH			1.0				//JOG_SPEED_HIGH

#ifndef M_PI
#define M_PI       	3.14159265358979323846f
#endif

#define M_PI_2     	1.57079632679489661923f
#define M_PI_4     	0.785398163397448309616f
#define M2_PI      	(M_PI+M_PI)
//! <--------------------------------------------------------------------------
//! 아진엑스텍 Motion Controller 전역 객체
#if (__MACHINE_MODEL == MACHINE_1ST)
#else
#endif
#include "AxlMotion/AXTDefine.h"
#include "AxlMotion/AxlMotorSet.h"


#include "AxlMotion/FlowProcess.h"
typedef struct _RawImgInfo
{
	int nSensorWidth;
	int nSensorHeight;
	int nFrameBufSize; // sensor_width * sensor_height * 2(=unpacked 16bit)

	int nDisplayWidth;
	int nDisplayHeight;
	int nDisplayStartPosY;
	int nDisplayStartPosX;
	int nDisplaySizeY;
	int nDisplaySizeX;
} RawImgInfo;


class mCDRect
{
public:
	double left, right, top, bottom;
	mCDRect()
	{
		left = right = top = bottom = 0.0;
	}
};

class CRectPos
{
public:
	CPoint Pos[50];
	CRectPos()
	{
		for (int i = 0; i < 50; i++)
		{
			Pos[i] = 0;
		}
		//Pos[0] = Pos[1] = Pos[2] = Pos[3] = Pos[4] = Pos[5] = 0;
	}
};

class CFPoint
{
public:
	float x, y;
	CFPoint()
	{
		x = y = 0.0f;
	}
};

class mCDPoint
{
public:
	double x, y;
	mCDPoint()
	{
		x = y = 0.0;
	}
};



#endif