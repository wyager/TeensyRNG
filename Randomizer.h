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


class Randomizer {
private:
    union data_union data;
    
public:
    Randomizer(){
        for (int i=0; i<8; i++) {
            this->data.dwords[i]=0;
        }
    }
void add(uint32_t data);
void add(const union data_union& data);
void get(uint8_t out_buffer[32]);
union data_union get();

};

#endif /* defined(__TeensyRNG__Randomizer__) */
