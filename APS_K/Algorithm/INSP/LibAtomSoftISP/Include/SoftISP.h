#pragma once

#include "export.h"


namespace AtomSoftISP
{

	typedef enum _EOUTMODE
	{
		OUTMODE_YUV = 0,
		OUTMODE_BAYER_BGGR,
		OUTMODE_BAYER_RGGB,
		OUTMODE_BAYER_GBRG,
		OUTMODE_BAYER_GRBG,
		OUTMODE_BAYER_BLACKWHITE
	} EOUTMODE;

	typedef enum _EDATAFORMAT
	{
		DATAFORMAT_YUV,
		DATAFORMAT_BAYER_8BIT,
		DATAFORMAT_BAYER_10BIT,
		DATAFORMAT_BAYER_12BIT
	} EDATAFORMAT;

	ATOMSOFTISP_API void xPacked10bitBuffer( short* p10BitBuffer, const BYTE* pOrgBuffer, int nWidth, int nHeight, int nBlackLevel );
	ATOMSOFTISP_API void xMake10to8Buffer( BYTE* p8BitBuffer, const short* p10bitBuffer, int nWidth, int nHeight );
	ATOMSOFTISP_API void xBMPtoY( int* pYUV, const int* pBMP, int nWidth, int nHeight );

	ATOMSOFTISP_API void xInterpolation3x3bilinear(EOUTMODE eFormat, BYTE *pRaw, BYTE *pBMP, int nWidth, int nHeight);
	
	ATOMSOFTISP_API bool xMeanBayerData( const BYTE* pRaw, int nWidth, int nHeight, RECT& rtROI, double* pdMeanData );

	// Y Table
	ATOMSOFTISP_API void xMakeYLookupTable();
	ATOMSOFTISP_API BYTE xGetY(BYTE r, BYTE g, BYTE b );

	// Lens Shading Correction
	ATOMSOFTISP_API void xCreateLensShadingCorrectionGainTable( int nImageWidth, int nImageHeight, int nScaleLevel, double dGain );
	ATOMSOFTISP_API void xLensShadingCorrection( BYTE* pRaw, int nImageWidth, int nImageHeight, int nScaleLevel );

};



