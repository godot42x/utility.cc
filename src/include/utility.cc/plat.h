#pragma once


#if _WIN32
    #ifdef UTILITY_CC_BUILD_SHARED
        #define UTILITY_CC_API __declspec(dllexport)
    #else
        #define UTILITY_CC_API __declspec(dllimport)
    #endif
#else
    #define UTILITY_CC_API
#endif