//
//  MakeConnectionString.cpp
//  energyUsageWithoutThe_PQXX_Bullshit
//
//  Created by Clifford Campo on 5/14/18.
//  Copyright © 2018 CliffordCampo. All rights reserved.
//
#include <iostream>
#include <string>
#include <stdio.h>
#include <time.h>
//#include "setupForMultiFit.hpp"
#include "setupForMultiFitInherit.hpp"
#include "cmdLineArgs.hpp"
#include "baseClassInherit.hpp"
#include "myPrototypes.hpp"
#include "GetTimeOfDay.hpp"
extern CmdLineArgs *ptrCLA;
extern const char *motherOfAllSelectStatements[];

// const std::string currentDateTime(BaseClassInherit *);
// M E M B E R    F U N C T I O N       D E F I N I T I O N    F O R    C L A S S    M A K E C O N N E C T I O N S T R I N G

BaseClassInherit::BaseClassInherit(const char ourCase, const char *myUserID, const char *myDatabaseName, const char *myPortID, const char *myHostID, const size_t maxNumberOfAvgTemps)  {
///Users/cjc/c++/continuingOn/continuingOn/baseClassInherit.cpp:23:19: Constructor for 'BaseClassInherit' must explicitly initialize the reference member 'gtod'

    //If true we are running multi-thread. Obviously, multiThread ***MUST*** be the same value for all instances of BaseClassInherit. Default value is FALSE. Command line switch `-t` sets this to TRUE.
    //    memset((void *)this->debugFlags.startHere, 0, THISSIZE); //Experiment on initializing memory in one fell swoop. (DOESN'T WORK)
    /*
     this->end = &this->e;
     this->start = &this->s;
     memset((char *)this->len, 0, (size_t)(&this->e - &this->s) );
     */
    CLA = ptrCLA;   //Point to the class named `CmdLineArgs` which we are using for this run. \
    There is only one instance of CmdLineArgs, although many instances of BaseClassInherit may point to it; \
    a case of one parent (CmdLineArgs, or CLA) with multiple children.
    //First let's initiate certain of BaseClassInherit's values with the default values as found in class CmdLineArgs:
    clUserID = CLA->defaultUserID;
    clHostID = CLA->defaultHostID;
    clPortID = CLA->defaultPortID;
    clDatabaseName = CLA->defaultDatabaseName;
    results_File = CLA->defaultResultsFileName;
    connectionResponsibilityCount = 0;
    //Now let's initiate the rest of BaseClassInherit's member data that needs initiating.
    switch (ourCase) {
        case M1M2KWH: // ourCase is '0'
            this->debugFlags.intMyCase = M1M2KWHCASE;
            this->debugFlags.mycase = M1M2KWH;
            this->BFS = *(motherOfAllSelectStatements + M1M2KWHCASE );  //m1m2kwh is the default value
            this->debugFlags.dependentVariableColumn = "m1m2kwh"; //Here we have the Constructor allowed, during execution time, to define what value gets assigned to the `const char *`
            break;
        case M1KWH:   // ourCase is '1'
            this->debugFlags.intMyCase = M1KWHCASE;
            this->debugFlags.mycase = M1KWH;
            this->BFS = *(motherOfAllSelectStatements + M1KWHCASE );  //m1m2kwh is the default value
            this->debugFlags.dependentVariableColumn = "m1kwh"; //Here we have the Constructor allowed, during execution time, to define what value gets assigned to the `const char *`
            this->debugFlags.includeM2InAverages = false; //If M1KWH, don't include M2KWH in averages.
            break;
        case M2KWH:   // ourCase is '2'
            this->debugFlags.intMyCase = M2KWHCASE;
            this->debugFlags.mycase = M2KWH;
            this->BFS = *(motherOfAllSelectStatements + M2KWHCASE );  //m1m2kwh is the default value
            this->debugFlags.dependentVariableColumn = "m2kwh"; //Here we have the Constructor allowed, during execution time, to define what value gets assigned to the `const char *`
            this->debugFlags.includeM1InAverages = false; //If M2KWH, don't include M1KWH in averages.
            break;
        case MAIN:    // ourCase is  '3'
            this->debugFlags.intMyCase = MAINCASE;
            this->debugFlags.mycase = M2KWH;
//            this->BFS = *(motherOfAllSelectStatements + M2KWHCASE );  //m1m2kwh is the default value  NOT NEEDED FOR main.cpp because main.cpp does not issue any postgreSQL commands.
            this->debugFlags.dependentVariableColumn = "main"; //Here we have the Constructor allowed, during execution time, to define what value gets assigned to the `const char *`
            this->debugFlags.includeM1InAverages = false; //If M2KWH, don't include M1KWH in averages.
            break;
        default:
            std::cerr <<  " File " << __FILE__ << ", Line " << __LINE__ << " Unknown ourCase value: " << ourCase << " passed to BaseClassInherit Constructor. These are the only legal values: " << M1M2KWH << " (for m1m2kwh), " << M1KWH << " (for m1kwh), and " << M2KWH << " (for m2kwh). " << __FILE__ << " is exiting at " <<  " with exit code of -1." << std::endl;
            exit(-1);
    }

    clDatabaseName =  ptrCLA->_clDatabaseName;
    results_File =  ptrCLA->_results_File;
    debugFlags.useKelvin = ptrCLA->_useKelvin;
    clUserID = ptrCLA->_clUserID;
    clPW = &pwBuffer[0];
    minBucketTemp = MINTEMPERATURE;
    buckettemp = minBucketTemp + BUCKETSIZE;
    maxBucketTemp = MAXTEMPERATURE;
    thisCount=INITIALVALUE;
    worthyOfPolynomialFitProcessing = INITIALVALUE;
    for (k=0; k < NUMBEROFVALUES; k++) values[k] = INITIALVALUE;
    clPW = ptrCLA->_clPW;
    clHostID = ptrCLA->_clHostID;
    clPortID = ptrCLA->_clPortID;
    clSQL = ptrCLA->_clSQL;
    debugFlags.debug1 = ptrCLA->_debug1;
    debugFlags.debug2 = ptrCLA->_debug2;
    debugFlags.debug3 = ptrCLA->_debug3;
    debugFlags.debug4 = ptrCLA->_debug4;
    lookAtMyConnectionString = connectionString; //Point to our connection string
    this->debugFlags.tableCreated = false;
    this->debugFlags.tempTableCreated = false;

    this->debugFlags.includeM1InAverages = true;
    this->debugFlags.includeM2InAverages = true;
    
    this->clUserID = myUserID;
    this->clDatabaseName = myDatabaseName;
    this->clPortID = myPortID;
    this->clHostID = myHostID;
    this->clSQL = NULL;
    this->debugFlags.countOfGoodEntries = 0;
    this->debugFlags.countOfBadEntries = 0;
    //    this->debugFlags.dependentVariableColumn = defaultDependentVariableColumn;
    this->debugFlags.includeM2InAverages = 1;
    this->ptrMaxNumberOfAvgTemps =  &maxNumberOfAvgTemps;
    this->ptrExpDepVarArray = NULL;
    this->ptrObsDepVarArray = NULL;
    this->ptrExpMinusObsDepVarArray = NULL;
    this->ptrIndVariableArray = NULL;
    bufferOffset = 0; // Initialize this counter.

    this->trc_t = 0;
    
    minBucketTemp = 0;
    countOfGoodEntries = 0;
    loopCounter = 0;
    rowBeingProcessed = 0;
    numberOfReturnedRows = 0;
    numberOfReturnedColumns = 0;
    grc = 0;
    brc = 0;
    k = 0;
    
    thisCount = 0;
    
    memset(this->pwBuffer,0, MAXLENGTHPASSWORD); //Zero-out the password buffer so we don't get fooled by stepping in crap!

    k=0;
    errmsg = NULL;
    ptrValues = values;
    
    
}

BaseClassInherit::~BaseClassInherit() {
    
    if(this->ptrIndVariableArray != nullptr) {
        try {
            delete [] this->ptrIndVariableArray;
        } catch (...) {
            std::cout << "Line: " <<  __LINE__ << " of file " << __FILE__ << " caught an abnormal termination trying to delete [] this->ptrIndVariableArray " <<  std::endl;
        }
    }
    
    if(this->ptrExpMinusObsDepVarArray != nullptr) {
        try {
            delete [] this->ptrExpMinusObsDepVarArray;
        } catch (...) {
            std::cout << "Line: " <<  __LINE__ << " of file " << __FILE__ << " caught an abnormal termination trying to delete [] this->ptrExpMinusObsDepVarArray " <<  std::endl;
        }
    }
    
    if(this->ptrExpDepVarArray != nullptr) {
        try {
            delete [] this->ptrExpDepVarArray;
        } catch (...) {
            std::cout << "Line: " <<  __LINE__ << " of file " << __FILE__ << " caught an abnormal termination trying to delete [] this->ptrExpDepVarArray " <<  std::endl;
        }
    }
    
    if(this->ptrObsDepVarArray != nullptr) {
        try {
            delete [] this->ptrObsDepVarArray;
        } catch (...) {
            std::cout << "Line: " <<  __LINE__ << " of file " << __FILE__ << " caught an abnormal termination trying to delete [] this->ptrObsDepVarArray " <<  std::endl;
        }
    }
        
}

void BaseClassInherit::setConString(const char *cs) { 
    this->lookAtMyConnectionString = cs;
}
void BaseClassInherit::allocateArrays(size_t sizeOfEachArray) {
    this->sizeOfEachArray = sizeOfEachArray;
    this->ptrIndVariableArray = new double[sizeOfEachArray];
    this->ptrExpMinusObsDepVarArray = new double[sizeOfEachArray];
    this->ptrExpDepVarArray = new double[sizeOfEachArray];
    this->ptrObsDepVarArray = new double[sizeOfEachArray];
}
void BaseClassInherit::deAllocateArrays() { 
    delete [] this->ptrIndVariableArray;
    this->ptrIndVariableArray = nullptr;
    delete [] this->ptrExpMinusObsDepVarArray;
    this->ptrExpMinusObsDepVarArray = nullptr;
    delete [] this->ptrExpDepVarArray;
    this->ptrExpDepVarArray = nullptr;
    delete [] this->ptrObsDepVarArray;
    this->ptrObsDepVarArray = nullptr;
}
int BaseClassInherit::setArray(int which, int k, double value) {
    switch(which) {
        case IV:
            *(this->ptrIndVariableArray + k) = value;
            break;
        case EMO:
            *(this->ptrExpMinusObsDepVarArray + k) = value;
            break;
        case EDVA:
            *(this->ptrExpDepVarArray + k) = value;
            break;
        case ODVA:
            *(this->ptrObsDepVarArray + k) = value;
            break;
        default:
            return BADWHICH; //Error
            
            
    }
    return 0;
}

const char *BaseClassInherit::getPrivate(CmdLineArgs &cla, const char which) {
    switch (which) {
        case 'U':
            return (cla.defaultUserID);
            break;
        case 'P':
            return (cla.defaultPortID);
            break;
        case 'D':
            return (cla.defaultDatabaseName);
            break;
        case 'R':
            return (cla.defaultResultsFileName);
            break;
        case 'H':
            return (cla.defaultHostID);
            break;
        default:

            std::cerr << "Invalide `which` parameter. The only valid values are: \n`U` for defaultUserID; \n`P` for defaultPortID; \n`D` for defaultDatabaseName; \n`R` for defaultResultsFileName; and \n`H` for defaultHostID. All other values are invalid. We're returning NULL" << std::endl;
            return (NULL);
            
    }
}







