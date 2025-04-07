#ifndef _ATOM_SFOTISP_EXPORT_H__
#define _ATOM_SFOTISP_EXPORT_H__

#ifdef LIBATOMSOFTISP_EXPORTS
#define ATOMSOFTISP_API	__declspec(dllexport)
#else
#define ATOMSOFTISP_API	__declspec(dllimport)
#endif


#endif /*_ATOM_SFOTISP_EXPORT_H__*/