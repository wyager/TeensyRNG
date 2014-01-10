#ifndef _types_h_included__
#define _types_h_included__
#include <inttypes.h>

struct RandomData{
    union{
        uint8_t bytes[16];
        uint32_t dwords[4];
    };
};

#endif
