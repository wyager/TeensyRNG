//
//  Entropy.cpp
//  TeensyRNG
//
//  Created by Will Yager on 6/25/13.
//  Copyright (c) 2013 Will Yager. All rights reserved.
//

#include "Entropy.h"
extern "C"{
#include "analog.h"
}


//Just returns 1 byte. Ideally choose the most entropic byte
uint8_t get_adc_sample(const uint8_t adc_index){
    uint8_t result = adc_read(adc_index) & 0xFF;
    return result;
}


//given 2 bytes and the offset of a bit to compare, does the von neumann debias. Returns the delta (0 if nothing, -1 or 1 otherwise
inline int8_t von_neumann_debias(const uint8_t byte1, const uint8_t byte2, const int bit_index){
    int8_t bit1 = (byte1 >> bit_index) & 0x01;
    int8_t bit2 = (byte2 >> bit_index) & 0x01;
    return bit1 - bit2; //0 if equal, 1 if falling edge, -1 if rising edge
}


//It's important to realize what we've got here. It's not 256 bits of entropy. It's just 256 bits of very grey noise. This algorithm gets rid of bias (which there is plenty of in the ADCs) but it doesn't guarantee randomness. This basically records all edges (flips from 1 to 0 or 0 to 1) in each ADC bit and saves it in a buffer. This is very close to grey noise (average 127.5) but no guarantees beyond that.
#define number_of_adcs 8
union data_union Entropy::get_entropy(){
    union data_union collected_entropy;
    int collected_entropy_bits = 0; //Number of bits we've collected (We're using a Von Neumann Debias)
    int number_of_times_we_have_tried_to_get_entropy = 0; //This is just a record of how many iterations we've done, so we don't try to sample the same ADC a bunch of times
    
    
    //Keep collecting bits until we have enough
    //We're using the Von Neumann Debias trick to compensate for the fact that the ADCs have a large non-random bias
    //This will slow things down, but it *greatly* increases the quality of our randomness
    //This DOES NOT guarantee a certain amount of randomness, though.
    //Without this trick, we'd be much less sure of how much entropy we were getting.
    
    while (collected_entropy_bits < 256) {
        
        int current_sample_index = number_of_times_we_have_tried_to_get_entropy % number_of_adcs;
        uint8_t sample_1 = get_adc_sample(current_sample_index);
        uint8_t sample_2 = get_adc_sample(current_sample_index);
        
        
        for (int bit=0; bit<8; bit++) {
            int8_t delta = von_neumann_debias(sample_1, sample_2, bit);
            if(delta){//if the bit has changed since last time
                uint8_t byte_index = collected_entropy_bits / 8;
                uint8_t bit_index = collected_entropy_bits % 8;
                if (delta > 0) collected_entropy.bytes[byte_index] |= 1 << bit_index;
                if (delta < 0) collected_entropy.bytes[byte_index] &= ~(1 << bit_index);
                collected_entropy_bits++;
                if(collected_entropy_bits == 256){
                    break;
                }
            }
        }
        number_of_times_we_have_tried_to_get_entropy++;
    }

    //If we're here, we've collected 256 bits of debiased entropy from the ADCs. Yay!
    return collected_entropy; //This returns a union data_union (256 bit buffer)
}
