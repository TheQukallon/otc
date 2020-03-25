#include "pch.h"
#include "PatchManager.h"

ExecutionStatus PatchManager::ExtractToMemory () {
    //Allocate memory for segment, and set pointer.
    DWORD allocatePointer = reinterpret_cast<DWORD> (VirtualAlloc (NULL, SEGMENT_ALLOCATION_SIZE, MEM_RESERVE | MEM_COMMIT, PAGE_EXECUTE_READWRITE));
    //Small check for not nullable pointer. (In 99% of situations this is not necessary. But who knows what is on the player side. :d)
    if (!allocatePointer) return ExecutionStatus { NULL, "Can`t allocate memory for segment. Check permissions.", true };
    //This is also unlikely, but if the player is not guilty of this, it will be useful for debugging.
    if (!memcpy (reinterpret_cast<PVOID> (allocatePointer), g_Segment, SEGMENT_SIZE)) return ExecutionStatus { NULL, "Can`t copy segment to pointer. Check permissions.", true };
    return ExecutionStatus { allocatePointer, "Segment successfully moved to allocated memory.", false };
}

ExecutionStatus PatchManager::ReconstructHotPoints (DWORD pointer) {

    //Iterate relocations in vector.
    for (const auto& relocation : m_SegmentHeader.m_Relocations) {
        //Subtract value with old base address in dump from memory.
        *reinterpret_cast<DWORD*> (pointer + relocation) -= SEGMENT_PREVIOUSLY_RUNTIME;
        //Add new base address in dump to offset.
        *reinterpret_cast<DWORD*> (pointer + relocation) += pointer;
    }

    //Iterate value in imports map.
    for (const auto& importsMap : m_SegmentHeader.m_Imports) {
        //Get info about import from value at map.
        for (const auto& importInfo : importsMap.second) {
            //Get function pointer from module. (Support only function with name, maybe later i add offset-based support)
            DWORD functionPointer = Utils::GetFunction (importsMap.first, importInfo.function);
            //Check non-nullptr at function and module.
            if (!functionPointer) {
                //Just error. Message, value, etc.
                std::ostringstream message;
                message << "Can`t find module or function. Module: " << importsMap.first << " <-> " << "Function: " << importInfo.function << ".";
                return ExecutionStatus { NULL, message.str().c_str(), true };
            }
            //Just iterate all offset. (Locations where function call in segment)
            for (const auto& parsedOffset : importInfo.offsetsMap) {
                //More about ImportType: SegmentHeader.h
                switch (parsedOffset.first) {
                    case INTERNAL:
                        //Patch functions (and in them) that call import.
                        *reinterpret_cast<DWORD*> (pointer + parsedOffset.second) = functionPointer - (pointer + parsedOffset.second + 0x4);
                      break;
                    case PUBLIC:
                        //Patch global variables.
                        *reinterpret_cast<DWORD*> (pointer + parsedOffset.second) = functionPointer;
                      break;
                    default:
                        //Just error. Message, value, etc.
                        std::ostringstream message;
                        message << "Can`t find offset type. Offset: " << parsedOffset.second << " <-> " << "Function: " << importInfo.function << ".";
                        return ExecutionStatus { NULL, message.str().c_str(), true };
                      break;
                }
            }
        }
    }

    //All ok.
    return ExecutionStatus { NULL, "Segment hot-points reconstructed.", false };

}

ExecutionStatus PatchManager::InvokeOEP (DWORD pointer) {
    //Set function address for call.
    DUMP_HEADER_OEP_FUNCTION EntryFunctionCallback = reinterpret_cast<DUMP_HEADER_OEP_FUNCTION> (pointer + SEGMENT_OEP_RVA);
    //Check for non-null OEP address.
    if (!EntryFunctionCallback) return ExecutionStatus { NULL, "Can`t find OEP. Check RVA.", true };
    //Call "OEP" func with arguments.
    EntryFunctionCallback(pointer, 1, 0);
    //All ok.
    return ExecutionStatus { NULL, "OEP invoked.", false };
}