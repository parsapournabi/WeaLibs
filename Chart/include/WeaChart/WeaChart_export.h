#ifndef WEACHART_EXPORT_H
#define WEACHART_EXPORT_H

// Definition of declaration only for MSVC compiler on Windows. (This will import method & classes to .dll)
#if defined(_WIN32) || defined(__CYGWIN__)
#ifdef WeaChart_EXPORT
#define WEACHART_API __declspec(dllexport)
#else
#define WEACHART_API __declspec(dllimport)
#endif
#else
/// @brief MSVC Import/Export API.
#define WEACHART_API
#endif

#endif // WEACHART_EXPORT_H
