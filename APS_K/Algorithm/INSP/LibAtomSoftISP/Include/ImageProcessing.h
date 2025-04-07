#pragma once

#include "export.h"

namespace AtomSoftISP
{
	typedef struct _COMPLEX
	{
		double re;
		double im;
	} COMPLEX;


#define HIGH_CLIP(x,limit) ( ((x)>(limit)) ? (limit):(x) )
#define LOW_CLIP(x,limit)  ( ((x)<(limit)) ? (limit):(x) )
#define CLIP(x,l,h) HIGH_CLIP(LOW_CLIP(x,l),h)

#define CHECK_HIGH_LIMIT(x,limit) ( ((x)>(limit)) ? (false):(true) )
#define CHECK_LOW_LIMIT(x,limit)  ( ((x)<(limit)) ? (false):(true) )
#define CHECK_LIMIT(x,l,h) (CHECK_HIGH_LIMIT(x,h)&&CHECK_LOW_LIMIT(x,l))
#ifndef ABS
#define ABS(x) ((x<0)?(-(x)):(x))
#endif


#define MAX_POLYFIT_N 7
#define LSC_SHIFT_FACTOR 8
#define LSC_FACTOR 256
#define LS_DATA_ROI_SIZE 50
#define LSC_POLY_NUM 6
#define LSC_IMAGE_MAX_X	4270
#define LSC_IMAGE_MAX_Y	3190
#define MAX_LSC_INDEX (LSC_IMAGE_MAX_X*LSC_IMAGE_MAX_Y/LS_DATA_ROI_SIZE/LS_DATA_ROI_SIZE+50)
#define ROUND2EVEN(x) ((int)((((float)(x)/2.0)+0.5)*2.0))
#define MAX_LSC_CHANNEL 4

	struct sLensShadingCorrectionData
	{
		double ocx;
		double ocy;
		int maxIndex;
		double maxData[MAX_LSC_CHANNEL];
		double data[MAX_LSC_CHANNEL][MAX_LSC_INDEX*3];
		//	double error[MAX_LSC_CHANNEL][MAX_LSC_INDEX*3];
		//	double fitCurve[MAX_LSC_CHANNEL][MAX_LSC_INDEX*3];
		//	double rsquare[MAX_LSC_CHANNEL];
		double coefficientLSCcurve[MAX_LSC_CHANNEL][MAX_POLYFIT_N+1];
		int x[MAX_LSC_INDEX*3];
		int y[MAX_LSC_INDEX*3];
		double distance[MAX_LSC_INDEX*3];
		double tableLSCcurve[MAX_LSC_CHANNEL][LSC_IMAGE_MAX_X*2];
		int tableLSCgain[MAX_LSC_CHANNEL][LSC_IMAGE_MAX_X*2];

	};

	ATOMSOFTISP_API int xGaussianNoiseCheck(BYTE* pBMP, int nWidth, int nHeight, float fThreshold, int nChannel );
	ATOMSOFTISP_API BOOL xMedian(double* pData, int nWidth, int nHeight, int nFilterWidth, int nFilterHeight);
	ATOMSOFTISP_API void xHistogramforBMP(BYTE* pBMP, int nWidth, int nHeight, long* pHistogram, int nChannel );
	ATOMSOFTISP_API void xHistogramEqualize(BYTE* pBMP, long* pHistogram, long lNumofPx, int nChannel );
	ATOMSOFTISP_API double xRotationX(double x,double y,double angle);
	ATOMSOFTISP_API double xRotationY(double x,double y,double angle);
	ATOMSOFTISP_API void xFFT(COMPLEX *f, int logN, int numpoints, int dir);
	ATOMSOFTISP_API void xGetBestFit(double *x, double *y, int n, double* slope, double* offset);
	ATOMSOFTISP_API double xRsquare(double* x, double*y, int n, double slope, double offset);
	ATOMSOFTISP_API double xGetDistance(double x1, double x2, double y1, double y2 );
	ATOMSOFTISP_API double xMean(BYTE* pBMP, int nWidth, int nHeight, RECT& rtROI, int nChannel); // Channel B=0, G=1, R=2

	ATOMSOFTISP_API void GetOpticalCenterRawImage(BYTE *pRaw,int width,int height, double* opticalCenterX, double* opticalCenterY);
	ATOMSOFTISP_API void PolyFit(double *x,double *y,int num,double *coff,int polyN);
	ATOMSOFTISP_API void GenerateLSCtableWithPolyFitCurve(BYTE *pRaw,int width,int height,struct sLensShadingCorrectionData *dataLSC);
	ATOMSOFTISP_API void LensShadingCorrectionWithPolyFit(BYTE *pRawLSCoff,BYTE *pRawLSCon,int width,int height,struct sLensShadingCorrectionData *dataLSC);	
};
