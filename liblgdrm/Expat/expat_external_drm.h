/* Copyright (c) 1998, 1999, 2000 Thai Open Source Software Center Ltd
   See the file COPYING for copying permission.
*/

#ifndef Expat_External_INCLUDED_DRM
#define Expat_External_INCLUDED_DRM 1

/* External API definitions */

#if defined(_MSC_EXTENSIONS) && !defined(__BEOS__) && !defined(__CYGWIN__) && !defined(WINCE)
#define XML_USE_MSC_EXTENSIONS_DRM 1
#endif

/* Expat tries very hard to make the API boundary very specifically
   defined.  There are two macros defined to control this boundary;
   each of these can be defined before including this header to
   achieve some different behavior, but doing so it not recommended or
   tested frequently.

   XMLCALL    - The calling convention to use for all calls across the
                "library boundary."  This will default to cdecl, and
                try really hard to tell the compiler that's what we
                want.

   XMLIMPORT_DRM  - Whatever magic is needed to note that a function is
                to be imported from a dynamically loaded library
                (.dll, .so, or .sl, depending on your platform).

   The XMLCALL macro was added in Expat 1.95.7.  The only one which is
   expected to be directly useful in client code is XMLCALL.

   Note that on at least some Unix versions, the Expat library must be
   compiled with the cdecl calling convention as the default since
   system headers may assume the cdecl convention.
*/
#ifndef XMLCALL
#if defined(_MSC_VER)
#define XMLCALL __cdecl
#elif defined(__GNUC__) && defined(__i386) && !defined(__INTEL_COMPILER)
#define XMLCALL __attribute__((cdecl))
#else
/* For any platform which uses this definition and supports more than
   one calling convention, we need to extend this definition to
   declare the convention used on that platform, if it's possible to
   do so.

   If this is the case for your platform, please file a bug report
   with information on how to identify your platform via the C
   pre-processor and how to specify the same calling convention as the
   platform's malloc() implementation.
*/
#define XMLCALL
#endif
#endif  /* not defined XMLCALL */


#if !defined(XML_STATIC_DRM) && !defined(XMLIMPORT_DRM)
#ifndef XML_BUILDING_EXPAT_DRM
/* using Expat from an application */

#ifdef XML_USE_MSC_EXTENSIONS_DRM
#define XMLIMPORT_DRM __declspec(dllimport)
#endif

#endif
#endif  /* not defined XML_STATIC_DRM */


/* If we didn't define it above, define it away: */
#ifndef XMLIMPORT_DRM
#define XMLIMPORT_DRM
#endif


#define XMLPARSEAPI(type) XMLIMPORT_DRM type XMLCALL

#ifdef __cplusplus
extern "C" {
#endif

#ifdef XML_UNICODE_WCHAR_T_DRM
#define XML_UNICODE_DRM
#endif

#ifdef XML_UNICODE_DRM     /* Information is UTF-16 encoded. */
#ifdef XML_UNICODE_WCHAR_T_DRM
typedef wchar_t XML_Char;
typedef wchar_t XML_LChar;
#else
typedef unsigned short XML_Char;
typedef char XML_LChar;
#endif /* XML_UNICODE_WCHAR_T_DRM */
#else                  /* Information is UTF-8 encoded. */
typedef char XML_Char;
typedef char XML_LChar;
#endif /* XML_UNICODE_DRM */

#ifdef XML_LARGE_SIZE_DRM  /* Use large integers for file/stream positions. */
#if defined(XML_USE_MSC_EXTENSIONS_DRM) && _MSC_VER < 1400
typedef __int64 XML_Index; 
typedef unsigned __int64 XML_Size;
#else
typedef long long XML_Index;
typedef unsigned long long XML_Size;
#endif
#else
typedef long XML_Index;
typedef unsigned long XML_Size;
#endif /* XML_LARGE_SIZE_DRM */

#ifdef __cplusplus
}
#endif

#endif /* not Expat_External_INCLUDED_DRM */
