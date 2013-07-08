//
//  Entropy.h
//  TeensyRNG
//
//  Created by Will Yager on 6/18/13.
//  Copyright (c) 2013 Will Yager. All rights reserved.
//

#ifndef TeensyRNG_Entropy_h
#define TeensyRNG_Entropy_h
#include "types.h"
class Entropy {
public:
    //All that matters is that this returns semi-random data. It should probably sample data from an ADC or something
    //and somehow integrate it into a data_union. More entropy is better (obviously).
    //In my case, I'm making this return Von Neumann debiased ADC data (256 bits worth)
    static union data_union get_entropy();
};

#endif
