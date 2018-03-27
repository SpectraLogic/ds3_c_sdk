//
// Created by rachelt on 3/27/18.
//

#ifndef DS3_LIBRARY_EXPORTS_H
#define DS3_LIBRARY_EXPORTS_H

// For windows DLL symbol exports.
#ifdef _WIN32
#    ifdef LIBRARY_EXPORTS
#        define LIBRARY_API __declspec(dllexport)
#    else
#        define LIBRARY_API __declspec(dllimport)
#    endif
#else
#    define LIBRARY_API
#endif

#endif
