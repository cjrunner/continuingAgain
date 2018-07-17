//
//  AllocateWorkingMemory.hpp
//  energyUsage
//
//  Created by Clifford Campo on 5/3/18.
//  Copyright © 2018 CliffordCampo. All rights reserved.
//

#ifndef AllocateWorkingVectors_hpp
#define AllocateWorkingVectors_hpp

#include <iostream>
//   #include "makeConnectionString.hpp" //Need this include to resolve the inherited MakeConnectionString class.
class AllocateWorkingVectors  { // AllocateWorkingVectors object inherits the MakeConnectionString Object
private:
    size_t numberOfRows=0;
public:
    double temperature;
    double *temperatureVector;
    double energyUsage;
    double *energyUsageVector;
    double weight;
    double *weightVector; //Occasionally we'll find a value other than one.
    AllocateWorkingVectors(void);
    void setupAWV(size_t);
    ~AllocateWorkingVectors();
};



#endif /* AllocateWorkingMemory_h */
