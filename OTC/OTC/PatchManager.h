#pragma once

#include "pch.h"
#include "Segment.h"

class PatchManager {

public:

    SegmentHeader m_SegmentHeader;

    //Extract dump to memory.
    ExecutionStatus ExtractToMemory ();

    //Patch relocations and imports.
    ExecutionStatus ReconstructHotPoints (DWORD pointer);

    //Just invoke dump.
    ExecutionStatus InvokeOEP (DWORD pointer);

};