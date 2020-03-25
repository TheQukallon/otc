#include "pch.h"
#include "SegmentUtils.h"

void SegmentUtils::CreateInfoHeader (DWORD pointer) {
    std::vector<DWORD> info;

    //Nickname in watermark.
    info.emplace_back (0x756F7900);
    info.emplace_back (0x656D6167);
    info.emplace_back (0x7A69622E);

    //Just empty space for normal work wrapper offset function.
    for (int emptyIterator = 0; emptyIterator < 5; emptyIterator++) info.emplace_back (0x0);

    //Some value in steam client. idk.
    info.emplace_back(reinterpret_cast<DWORD> (Utils::GetModule ("steamclient.dll") + SEGMENT_LIBRARY_RVA));

    //Set base address modules in game.
    for (const auto& library : libraries) info.emplace_back (reinterpret_cast<DWORD> (Utils::GetModule(library)));

    //9 - it`s client_panorama.dll base address.
    for (const auto& offset : offsets) info.emplace_back (info.at(9) + offset);

    //Move info-header from vector to segment allocated memory.
    std::memmove (reinterpret_cast<DWORD*> (pointer), reinterpret_cast<PVOID> (info.data()), SEGMENT_INFO_HEADER_SIZE);
}

void SegmentUtils::UpdateNetVars (DWORD pointer) {
    for (const auto& netvar : netvars) {
        *reinterpret_cast<DWORD*> (pointer + netvar.rva) = netvar.new_value;
    }
}

void SegmentUtils::CreateHook (DWORD pointer) {
    //Easy hook with HoShiMin lib <3. About lib: www.github.com/HoShiMin/HookLib
    SetHook (reinterpret_cast<PVOID> (pointer + SEGMENT_FUNCTION_HOOK_RVA), &CustomFunctionHook, reinterpret_cast<PVOID*> (&OriginalFunctionHookCaller));
}