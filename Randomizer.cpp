//
//  Randomizer.cpp
//  TeensyRNG
//
//  Created by Will Yager on 6/17/13.
//  Copyright (c) 2013 Will Yager. All rights reserved.
//

#include "Randomizer.h"
#include "sha256.h"

//MODIFIES THE BUFFER
void sha_buffer_contents(union data_union& buffer){
    union data_union result;
    sha256(buffer.bytes, 32, result.bytes);
    for(int i=0; i<8; i++){
        buffer.dwords[i] = result.dwords[i];
    }
}

void Randomizer::add(const uint32_t data){
    this->data.dwords[0] ^= data;
    sha_buffer_contents(this->data);
}
void Randomizer::add(const union data_union& data){
    for (int i=0; i<8; i++) {
        this->data.dwords[i] ^= data.dwords[i];
    }
    sha_buffer_contents(this->data);
}
union data_union Randomizer::get(){
    union data_union result;
    for (int i=0; i<8; i++) {
        result.dwords[i] = this->data.dwords[i];
    }
    
    //Makes it so that something leaking data won't give up the contents of the entropy pool 
    //(or its hash, which is used the next time some entropy is added).
    //For example (assuming we didn't do the following step): 
    //1.Program on PC gets 256 bits of entropy from device. Now it knows the state of the randomizer
    //  (if it happens to request from the right part of the random data stream)
    //2.Now, if the program on the PC (maybe an attacker) can make predictions about the next few hundred/thousand
    //  bits of environmental data, it can determine the next 256 bits of pseudorandom data! Bad stuff!
    //Instead, this next step should theoretically obfuscate the state of the randomizer (provided the
    //  attacker was not able to predict environmental data from the beginning).
    result.bytes[0] += 1;
    sha_buffer_contents(result);
    
    
    return result;
}

