//
//  types.h
//  TeensyRNG
//
//  Created by Will Yager on 6/21/13.
//  Copyright (c) 2013 Will Yager. All rights reserved.
//

#ifndef TeensyRNG_types_h
#define TeensyRNG_types_h
#include <inttypes.h>

union data_union{
    uint8_t bytes[32];
    uint16_t words[16];
    uint32_t dwords[8];
};

#endif
