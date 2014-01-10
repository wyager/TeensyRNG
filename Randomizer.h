//
//  Randomizer.h
//  TeensyRNG
//
//  Created by Will Yager on 6/17/13.
//  Copyright (c) 2013 Will Yager. All rights reserved.
//

#ifndef __TeensyRNG__Randomizer__
#define __TeensyRNG__Randomizer__
#include <inttypes.h>
#include "types.h"


// This class uses a sponge construction.
// The bitrate is 128 bits and the capacity is 128 bits.
// The permutation function is SHA256.

class Randomizer {
private:
    
    union{
        uint8_t data_bytes[32];
        uint32_t data_dwords[8];
    };
    
public:
    Randomizer(){
        for (int i=0; i<8; i++) {
            this->data_dwords[i]=0;
        }
    }
    void add(uint32_t data);
    void add(const RandomData& data);
    void get(RandomData& out_buffer);
    RandomData get();
};

#endif /* defined(__TeensyRNG__Randomizer__) */
