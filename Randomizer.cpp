//
//  Randomizer.cpp
//  TeensyRNG
//
//  Created by Will Yager on 6/17/13.
//  Copyright (c) 2013 Will Yager. All rights reserved.
//

#include "Randomizer.h"
#include "sha256.h"


void Randomizer::add(const uint32_t data){
    this->data_dwords[0] ^= data;
    sha256(this->data_bytes, 32, this->data_bytes);
}
void Randomizer::add(const RandomData& data){
    for (int i=0; i<16; i++) {
        this->data_bytes[i] ^= data.bytes[i];
    }
    sha256(this->data_bytes, 32, this->data_bytes);
}
void Randomizer::get(RandomData& out_buffer){
    for(int i=0; i<4; i++){
        out_buffer.dwords[i] = this->data_dwords[i];
    }
}

RandomData Randomizer::get(){
    RandomData result;
    this->get(result);
    return result;
}