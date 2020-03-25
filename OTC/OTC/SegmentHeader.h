#pragma once

#include "pch.h"
#include "Segment.h"

//Simple DllMain function.
typedef int (__stdcall *DUMP_HEADER_OEP_FUNCTION) (int, int, int);

#define SEGMENT_ALLOCATION_SIZE 0x968940
#define SEGMENT_SIZE 0x186A00
#define SEGMENT_PREVIOUSLY_RUNTIME 0x3D600000
#define SEGMENT_OEP_RVA 0x4738C

enum ImportType {

    /**
          What is intrnal ? Let's look at a simple function.
    
             sub01:
                
                 int myImportSuperValue = (0x313131313) (mySuperValue);
 
          This function receives the value from the import (some dll) and writes it to a variable. This is the Internal.
     **/

    INTERNAL,

    //What is public? This is a global variable that points to function import. I don’t even know what to add. That's all :d
    PUBLIC
};

struct ImportInfo {
    //Import function name.
    const char* function;
    //Location in segment where need patch. (RVA only)
    std::multimap<ImportType, DWORD> offsetsMap;
};

class SegmentHeader {

public:
    
    SegmentHeader() {
        FillImports ();
        FillRelocations ();
    }

    //Need for segment can call the necessary functions from libraries. Storage model: Module -> ImportInfo [Function, offsets ^ n].
    std::map<const char*, std::vector<ImportInfo>> m_Imports;
    //Need for reconstruct own variables/methods call.
    std::vector<DWORD> m_Relocations;

    //Simple (no), functions for fill vectors.

    void FillImports ();
    void FillRelocations ();

};