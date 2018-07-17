//
//  AllocateWorkingVectors.cpp
//  continuingOn
//
//  Created by Clifford Campo on 7/8/18.
//  Copyright © 2018 CliffordCampo. All rights reserved.
//

#include <iostream>
#include "AllocateWorkingVectors.hpp" 
AllocateWorkingVectors::AllocateWorkingVectors( ) { //First Phase, Initial, Constructor
    std::cout << "In the φ1 constructor of Allocate Working Vectors. We do nothing but make sure the data members have a clean, known,  slate" << std::endl;
    temperatureVector = NULL;
    energyUsageVector = NULL;
    weightVector = NULL;
}
void AllocateWorkingVectors::setupAWV(size_t nor)  { //Second Phase, subsequent, Constructor.
    std::cout << "In the φ2 constructor of Allocate Working Vectors. In φ2, set the numberOfRows datamember \
    (=" << nor <<"), and then allocate space for the temperature, energyUsage, and weight vectors." << std::endl;
    this->numberOfRows = nor;
    if (nor > 1) {
        this->temperatureVector = new double[nor];  // << ===================================
        this->energyUsageVector = new double[nor];  // << ===================================
        this->weightVector = new double[nor];        // << ===================================
    } else if (nor == 0){ //Why waste a dynamic memory when you have just one value;
        this->temperatureVector = &this->temperature;
        this->energyUsageVector = &this->energyUsage;
        this->weightVector = &this->weight;
    } else {
        std::cout << "Exiting, can't have a nor value less than 1. In this case nor = " << nor << std::endl;
        exit (2); //Abend, can't have a nor value less than 1!
    }
}
AllocateWorkingVectors::~AllocateWorkingVectors () {
    std::cout << "1. Deleting temperatureVector which is " << sizeof(this->temperatureVector) << " bytes long." << std::endl;
    if (this->temperatureVector != nullptr) delete this->temperatureVector;
    std::cout << "2. Deleting energyUsageVector which is " << sizeof(this->energyUsageVector) << " bytes long." << std::endl;
    if (this->energyUsageVector != nullptr) delete this->energyUsageVector;
    std::cout << "3. Deleting weightVector which is " << sizeof(this->weightVector) << " bytes long." << std::endl;
    if (this->weightVector != nullptr)  delete this->weightVector;
    
}
