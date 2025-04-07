/// @mainpage LibAtomResolution
/// @section Introduction
/// - Provide SFR, EIAJ algorithm
/// - Provide SFR, EIAJ Chart Processing
/// @section CREATEINFO Create Information
/// - Team: Optics Software Team in LG Innotek
/// - Date: 2014-02-05
/// @section Copyright 
/// - copyright LG Innotek Co., Ltd.

#ifndef _ATOM_RESOLUTION_EXPORT_H__
#define _ATOM_RESOLUTION_EXPORT_H__

#ifdef LIBATOMRESOLUTION_EXPORTS
#define ATOMRESOLUTION_API	__declspec(dllexport)
#else
#define ATOMRESOLUTION_API	__declspec(dllimport)
#endif


#endif /*_ATOM_RESOLUTION_EXPORT_H__*/

