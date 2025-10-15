/// ----------------------------------------------------------------
/// Include some definitions to disable a bunch of 
/// unused windows stuff.
/// ----------------------------------------------------------------

#ifndef WINCONFIG_H
#define WINCONFIG_H

// target Windows 10 or later
// usually in targetver.h with default project
// 
// Including SDKDDKVer.h defines the highest available Windows platform.
// If you wish to build your application for a previous Windows platform, include WinSDKVer.h and
// set the _WIN32_WINNT macro to the platform you wish to support before including SDKDDKVer.h
#define _WIN32_WINNT 0x0A00
#include <sdkddkver.h>

// some definitions to improve compilation speed and reduce unnecessary features
#define WIN32_LEAN_AND_MEAN // Exclude rarely-used stuff from Windows headers. It speeds up the build process, it reduces the size of the Win32 header files by excluding some of the less used APIs.
#define NOGDICAPMASKS // No GDI Capabilities
#define NOSYSMETRICS // No System Metrics
#define NOMENUS
#define NOICONS
#define NOSYSCOMMANDS
#define NORASTEROPS
#define OEMRESOURCE
#define NOATOM
#define NOCLIPBOARD
#define NOCOLOR
#define NOCTLMGR
#define NODRAWTEXT
#define NOKERNEL
#define NONLS
#define NOMEMMGR
#define NOMETAFILE
#define NOMINMAX
#define NOOPENFILE
#define NOSCROLL
#define NOSERVICE
#define NOSOUND
#define NOTEXTMETRIC
#define NOWH
#define NOCOMM
#define NOKANJI
#define NOHELP
#define NOPROFILER
#define NODEFERWINDOWPOS
#define NOMCX
#define NORPC
#define NOPROXYSTUB
#define NOIMAGE
#define NOTAPE

#define STRICT

#include <Windows.h>

#endif // WINCONFIG_H