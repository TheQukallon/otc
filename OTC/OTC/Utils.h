#pragma once

#include "pch.h"

struct ExecutionStatus {
    DWORD value;
    const char* msg;
    bool isCause;
};

struct CodeSignature {
    const char* pattern;
    const char* mask;
};

class Utils {

public:

    static HMODULE GetModule (const char* module);

    static MODULEINFO GetModuleInfo (const char* module);

    static DWORD GetFunction (const char* module, const char* function);

};