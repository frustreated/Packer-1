#pragma once

#define WIN32_STUB_BASE_ADDRESS 0x00400000
#define WIN32_STUB_MAIN_SECTION_BASE 0x00100000
#define WIN32_STUB_IMP_SECTION_BASE 0x00200000
#define WIN32_STUB_STAGE2_BASE_ADDRESS 0x50000000

#define WIN32_STUB_MAIN_SECTION_NAME ".main"
#define WIN32_STUB_IMP_SECTION_NAME ".imp"