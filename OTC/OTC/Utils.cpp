#include "pch.h"

HMODULE Utils::GetModule (const char* module) {

   HMODULE hModule = GetModuleHandleA (module);

   if (!hModule) {
       hModule = LoadLibraryA (module);
   }

   return hModule;
}

MODULEINFO Utils::GetModuleInfo (const char* module) {

    MODULEINFO modinfo = { 0 };

    GetModuleInformation (GetCurrentProcess(), GetModule(module), &modinfo, sizeof(MODULEINFO));

    return modinfo;
}

DWORD Utils::GetFunction (const char* module, const char* function) {
   return reinterpret_cast<DWORD> (GetProcAddress(GetModule(module), function));
}