//
//  TblPolyFitWithCovInherit.cpp
//  multiThreadEnergyUsageWithPostgreSQLTableOutput
//
//  Created by Clifford Campo on 6/26/18.
//  Copyright © 2018 CliffordCampo. All rights reserved.
//


//
//  tblPolyFit.cpp
//  multiThreadEnergyUsage
//
//  Created by Clifford Campo on 6/15/18.
//  Copyright © 2018 CliffordCampo. All rights reserved.
//


#include <stdio.h>
#include <iostream>
#include <netinet/in.h>
#include "/usr/local/pgsql95/pgsql090513/include/libpq-fe.h" //changed from 101 on 2018-07-10T14:20:53
#include "baseClassInherit.hpp"
//#include "setupForMultiFitInherit.hpp"
//#include "AllocateWorkingVectors.hpp"
#include "TblPolyFitWithCovInherit.hpp"
#include "varyingType.hpp"
#include "GetTimeOfDay.hpp"
#define OK 0
#define THISISSHIT 1
#define LLS1 OK
#define LLS2 OK //Turned into Shinola on 2018-07-06T17:14:25
#define LLS3 OK
#define LLS4 OK
extern const char *cases[];
//extern const char *ptr_char_conn_string;
//std::ostringstream *current_Date_Time(BaseClassInherit *);

    //      R U N    M O D E       R U N    M O D E       R U N    M O D E       R U N    M O D E
TblPolyFitWithCovInherit::TblPolyFitWithCovInherit(char ourCase, short pd, int nv, int rsltFmt, bool withCov) :  SetupForMultiFitInherit(ourCase, POLYNOMIALDEGREE ) { 

    self = this;
    if(this->BaseClassInherit::debugFlags.debug3) std::cout <<  "b. Within the TblPolyFitWithCovInherit's constructor for the " << ourCase << " case, withCov = "<< withCov << ", and nv = " << nv << "."  << std::endl;
//    ptrCovNbo = new float[pd * pd]{};  //Get another array, that mirrors the original array of covariance data, \
    but this array will have the covariance data in NetworkByteOrder. We can NOT fill this array until the \
    covariance matrix has been filled by the class SetupForMultiFitInherit. If this TblPolyFitWithCovInherit\
    class is instatiated from the main.cpp program, then it might be too early to fill this array because \
    SetupForMultiFitInherit has not yet been called via the selectFromTable.cpp, to compute the covariance \
    matrix.
    this->ptrArrayOfOid = ArrayOfOid;
    ptrCovNbo = copyOfCovarianceMatrixInNbo; //Establish Addressability to this 2D, 4X4, matrix.
    ptrParamLengths = ArrayOfParamLengths;
    ptrParamFormats = ArrayOfParamFormats;
//Start setting up these non-dynamically acquired arrays to contain the constants they need
    for(intLoopCounter=0; intLoopCounter < 24; intLoopCounter++) {
        ArrayOfParamFormats[intLoopCounter] = BINARYRESULTS;
    }
    for(intLoopCounter=5; intLoopCounter < 22; intLoopCounter++) {
        ArrayOfParamLengths[intLoopCounter]=__SIZEOF_FLOAT__;
        ArrayOfOid[intLoopCounter] = FLOAT4OID;
    }
    for(intLoopCounter=1; intLoopCounter < 5; intLoopCounter++) {
        ArrayOfParamLengths[intLoopCounter]=__SIZEOF_DOUBLE__;
        ArrayOfOid[intLoopCounter] = FLOAT8OID;
    }
    for(intLoopCounter=22; intLoopCounter < 24; intLoopCounter++) {
        ArrayOfParamLengths[intLoopCounter]=__SIZEOF_DOUBLE__;
        ArrayOfOid[intLoopCounter] = FLOAT8OID;
    }
    ArrayOfParamLengths[0]=__SIZEOF_SHORT__;
    ArrayOfOid[0] = INT2OID;
    ArrayOfParamLengths[21]=__SIZEOF_SHORT__;
    ArrayOfOid[21] = INT2OID;
    //Finish setting up these non-dynamically acquired arrays to contain the constants they need
    //connString = cs;
    this->ptrOurCase = &ourCase;
    this->ushort.intOurCase = ourCase - '0';
    nParams = nv;
    resultFormat = rsltFmt;
    /* Commented out on 2018-07-15T10:46:57
    paramValues = new const char [nv]{};
    paramTypes = new const Oid[nv]{INT2OID,FLOAT8OID, FLOAT8OID, FLOAT8OID, FLOAT8OID, FLOAT4ARRAYOID, INT2OID, FLOAT8OID, FLOAT8OID};
    paramLengths = new const  int[nv]{2,8,8,8,8, 16*4, 2,8,8};
    paramFormats = new const int [nv]{1,1,1,1,1,   1,  1,1,1};
    */
}
// D E S T R U C T O R      D E S T R U C T O R      D E S T R U C T O R      D E S T R U C T O R      D E S T R U C T O R      D E S T R U C T O R      
TblPolyFitWithCovInherit::~TblPolyFitWithCovInherit()  {
        /* Now, 2018-07-15T11:22:36, statically allocated.
     if (ptrCovNbo != nullptr)
         try {
         delete [] ptrCovNbo;
         } catch ( ... ) {
             std::cout << "Line: " <<  __LINE__ << " of file " << __FILE__ << " caught an abnormal termination trying to delete [] ptrCovNbo " <<  std::endl;
         }
    */
    //   if (paramValues != nullptr || paramValues != (const char *)'\0') delete [] paramValues;
    /* Now, 2018-07-15T11:22:36, statically allocated.
    if (paramTypes != nullptr) {
        try {
            delete [] paramTypes;
        } catch (...) {
            std::cout << "Line: " <<  __LINE__ << " of file " << __FILE__ << " caught an abnormal termination trying to delete [] paramTypes " <<  std::endl;
        }
    }
     */
        /* Now, 2018-07-15T11:22:36, statically allocated.
    if (paramLengths != nullptr) {
        try {
            delete [] paramLengths;
        } catch (...) {
            std::cout << "Line: " <<  __LINE__ << " of file " << __FILE__ << " caught an abnormal termination trying to delete [] paramLengths " <<  std::endl;
        }
    }
    */
        /* Now, 2018-07-15T11:22:36, statically allocated.
    if (paramFormats != nullptr) {
        try {
            delete [] paramFormats;
        } catch (...) {
            std::cout << "Line: " <<  __LINE__ << " of file " << __FILE__ << " caught an abnormal termination trying to delete [] paramFormats " <<  std::endl;
        }
    }
    */

}
int  TblPolyFitWithCovInherit::outputPolynomial(const char *leadOffString, const char *trailingString) {
    this->SetupForMultiFitInherit::stringStreamForOutput << this->BaseClassInherit::ptrGTOD->doTimePoint() << "\t" << leadOffString;
    for (this->BaseClassInherit::row = 0; this->BaseClassInherit::row < this->::TblPolyFitWithCovInherit::coefficients->size ; this->BaseClassInherit::row++) {
        if (this->BaseClassInherit::row > 0) {
            this->SetupForMultiFitInherit::stringStreamForOutput << " + " << gsl_vector_get(this->SetupForMultiFitInherit::coefficients,  this->BaseClassInherit::row) << "*T^" << this->BaseClassInherit::row;
        } else {
            this->SetupForMultiFitInherit::stringStreamForOutput << gsl_vector_get(this->SetupForMultiFitInherit::coefficients, this->BaseClassInherit::row) ;
        }
    }
    this->SetupForMultiFitInherit::stringStreamForOutput << trailingString << "\n";
    return 0;
}

int TblPolyFitWithCovInherit::doInsertInto( void ) {
//    VaryingType<short> *tshort =new VaryingType<short>();
    VaryingType<short>  tshort;
    tshort.in64.d64 = POLYNOMIALDEGREE;
    tshort.VaryingType<short>::toNetworkByteOrder();
    this->TblPolyFitWithCovInherit::nbop1 = tshort.out64.d64;
    // --------------------------------------------
    
    tshort.in64.d64 = this->TblPolyFitWithCovInherit::ushort.intMyCase; //Reuse the short object that we created above
    tshort.in64.d64 = this->TblPolyFitWithCovInherit::ushort.intMyCase;
    tshort.VaryingType<short>::toNetworkByteOrder();
    this->TblPolyFitWithCovInherit::nbop6 = tshort.out64.d64;
    // --------------------------------------------
//    VaryingType<double> *tdouble =new VaryingType<double>();  //This tdouble object, for type double, should serve us for making input parameters p2, p3, p4, p5, p7, and p8 in network byte order.
    VaryingType<double> tdouble;
    tdouble.in64.d64 = this->TblPolyFitWithCovInherit::copyOfCoefficients[0];
    tdouble.VaryingType<double>::toNetworkByteOrder();
    this->TblPolyFitWithCovInherit::nbop2 = tdouble.out64.d64;
    // --------------------------------------------
    tdouble.in64.d64 = this->TblPolyFitWithCovInherit::copyOfCoefficients[1];
    tdouble.VaryingType<double>::toNetworkByteOrder();
    this->TblPolyFitWithCovInherit::nbop3 = tdouble.out64.d64;
    // --------------------------------------------
    tdouble.in64.d64 = this->TblPolyFitWithCovInherit::copyOfCoefficients[2];
    tdouble.VaryingType<double>::toNetworkByteOrder();
    this->TblPolyFitWithCovInherit::nbop4 = tdouble.out64.d64;
    // --------------------------------------------
    tdouble.in64.d64 = this->TblPolyFitWithCovInherit::copyOfCoefficients[3];
    tdouble.VaryingType<double>::toNetworkByteOrder();
    this->TblPolyFitWithCovInherit::nbop5 = tdouble.out64.d64;
    
    // -------------------------------------------- Now, convert all {[(1+polynomialdegree)*(1+polynomialdegree)]=>16 for polynomialdegree = 3} elements comprising covariance matrix.
//    VaryingType<float> *tfloat =new VaryingType<float>();
    VaryingType<float> tfloat;
    std::cout << "The number of covariance matrix entries is: " << NUMBER_OF_COVARIANCE_MATRIX_ENTRIES << std::endl;
//    this->TblPolyFitWithCovInherit::ptrCovNbo = new float[ NUMBER_OF_COVARIANCE_MATRIX_ENTRIES ]; //We \
    no longer need to dynamically acquire memory since we have statically allocated a 4X4 array of type \
    float in object named TblPolyFitWithCovInherit.
    for (this->TblPolyFitWithCovInherit::intLoopCounter = 0; this->TblPolyFitWithCovInherit::intLoopCounter < ((1+POLYNOMIALDEGREE) * (1+POLYNOMIALDEGREE)) ; this->TblPolyFitWithCovInherit::intLoopCounter++) {
        //*(this->ptrCovNbo + i)
        //Note: The constructor should have converted the test data (the test data is defined in the constructor) to network byte order.
        tfloat.in64.d64 = *( this->TblPolyFitWithCovInherit::ufloat.ptrToCovarianceMatrix + this->TblPolyFitWithCovInherit::intLoopCounter );
        tfloat.VaryingType<float>::toNetworkByteOrder();
        *(this->TblPolyFitWithCovInherit::ptrCovNbo + this->TblPolyFitWithCovInherit::intLoopCounter) = tfloat.out64.d64;
    }
    // --------------------------------------------
    // We converted the second short to NetworkByteOrder, above. See this->TblPolyFitWithCovInherit::nbop6
    // --------------------------------------------
    tdouble.in64.d64 = this->TblPolyFitWithCovInherit::correlationCoefficient;  //correlation coefficient.
    tdouble.VaryingType<double>::toNetworkByteOrder();
    this->TblPolyFitWithCovInherit::nbop7 = tdouble.out64.d64;
    // --------------------------------------------
    tdouble.in64.d64 = this->TblPolyFitWithCovInherit::chi2;  //chi-squared
    tdouble.VaryingType<double>::toNetworkByteOrder();
    this->TblPolyFitWithCovInherit::TblPolyFitWithCovInherit::nbop8 = tdouble.out64.d64;
    // --------------------------------------------
    // Note how c++ initializes an array ***without the use of an equal sign*** between the closing `]` and the opening `{`.
    
    
    
    //What follows is the crazyiest type casting I've ever seen. See The PostgreSQL 9.0 Reference Manual, Volume 2: Programming Guide, bottom of page 82. I highlighted this in yellow.
    
    this->TblPolyFitWithCovInherit::ArrayOfPtrTpParamsInNbo[0] = (char *)&this->TblPolyFitWithCovInherit::nbop1;
    this->TblPolyFitWithCovInherit::ArrayOfPtrTpParamsInNbo[1] = (char *)&this->TblPolyFitWithCovInherit::nbop2;
    this->TblPolyFitWithCovInherit::ArrayOfPtrTpParamsInNbo[2] = (char *)&this->TblPolyFitWithCovInherit::nbop3;
    this->TblPolyFitWithCovInherit::ArrayOfPtrTpParamsInNbo[3] = (char *)&this->TblPolyFitWithCovInherit::nbop4;
    this->TblPolyFitWithCovInherit::ArrayOfPtrTpParamsInNbo[4] = (char *)&this->TblPolyFitWithCovInherit::nbop5;
    for (this->TblPolyFitWithCovInherit::intLoopCounter=5; this->TblPolyFitWithCovInherit::intLoopCounter < 21; this->TblPolyFitWithCovInherit::intLoopCounter++) {
        this->TblPolyFitWithCovInherit::ArrayOfPtrTpParamsInNbo[this->TblPolyFitWithCovInherit::intLoopCounter] = (char *)&(*(this->TblPolyFitWithCovInherit::ptrCovNbo + this->TblPolyFitWithCovInherit::intLoopCounter - 5));
    }
    this->TblPolyFitWithCovInherit::ArrayOfPtrTpParamsInNbo[21] = (char *)&this->TblPolyFitWithCovInherit::nbop6;
    this->TblPolyFitWithCovInherit::ArrayOfPtrTpParamsInNbo[22] = (char *)&this->TblPolyFitWithCovInherit::nbop7;
    this->TblPolyFitWithCovInherit::ArrayOfPtrTpParamsInNbo[23] = (char *)&this->TblPolyFitWithCovInherit::nbop8;
    
    // --------------------------------------------VVVVVVVVVVVVVVVVVVVVVVVV----------------------------
    if (this->BaseClassInherit::connectionResponsibilityCount == 0)  { //Have we connected to database, yet?
        this->BaseClassInherit::conn  = PQconnectdb("dbname=LocalWeather host=localhost user=cjc port=5436");
        if(PQstatus(this->TblPolyFitWithCovInherit::conn) != CONNECTION_OK) {
            std::cout << "Connection to database failed: " << PQerrorMessage(this->TblPolyFitWithCovInherit::conn) << std::endl;
            PQfinish(this->TblPolyFitWithCovInherit::conn);
            exit(1);
        }
    }
    if (this->BaseClassInherit::debugFlags.debug3) {
        std::cout << this->BaseClassInherit::ptrGTOD->doTimePoint() <<"\tIn member function TblPolyFitWithCovInherit::doInsertInto>>> It looks like we connected to database ok, so now let's try to insert the data into table `tbl_poly_fit_with_cov` " << std::endl;
        std::cout << this->BaseClassInherit::ptrGTOD->doTimePoint() << "\tWe will run SQL Command that looks like: " << this->TblPolyFitWithCovInherit::insertIntoCov <<  ".\n This SQL command has " << this->TblPolyFitWithCovInherit::nParams << " parameters." << std::endl;  //This Looks Good.
        if(LLS1) {
            std::cout << this->BaseClassInherit::ptrGTOD->doTimePoint() << "\t1. The ArrayOfID array looks like: \n";  //This looks good
            for(this->intLoopCounter=0 ; this->intLoopCounter < this->TblPolyFitWithCovInherit::nParams; this->intLoopCounter++) {
                std::cout << this->intLoopCounter << ". this->TblPolyFitWithCovInherit::ArrayOfOid[" << this->intLoopCounter  << "]: " <<this->TblPolyFitWithCovInherit::ArrayOfOid[this->intLoopCounter] << std::endl;
            } //1. End of for loop
        } else if (LLS2) {
            std::cout << this->BaseClassInherit::ptrGTOD->doTimePoint() << "\t2. The Array of Pointers to ParamValues (&this->TblPolyFitWithCovInherit::paramValues) Looks Like: \n";  //This LOOKS LIKE SHIT
            for(this->intLoopCounter=0 ; this->intLoopCounter < this->TblPolyFitWithCovInherit::nParams; this->intLoopCounter++) {
                std::cout << this->intLoopCounter << ". &this->TblPolyFitWithCovInherit::paramValues[" << this->intLoopCounter  << "]: addr" << this->TblPolyFitWithCovInherit::ArrayOfPtrTpParamsInNbo[this->intLoopCounter] << "\t value"<< *this->TblPolyFitWithCovInherit::ArrayOfPtrTpParamsInNbo[this->intLoopCounter]  <<  std::endl;
            } //2. End of for loop
        } else if (LLS3) {
            std::cout << this->BaseClassInherit::ptrGTOD->doTimePoint() << "\t3. The ArrayOfParamLengths Looks Like: \n";   //This looks good
            for(this->intLoopCounter=0 ; this->intLoopCounter < this->TblPolyFitWithCovInherit::nParams; this->intLoopCounter++) {
                std::cout << this->intLoopCounter << ". this->TblPolyFitWithCovInherit::ArrayOfParamLengths[" << this->intLoopCounter  << "]: " << this->TblPolyFitWithCovInherit::ArrayOfParamLengths[this->intLoopCounter] << std::endl;
            } //2. End of for loop
        } else if (LLS4) {
            std::cout << this->BaseClassInherit::ptrGTOD->doTimePoint() << "\t4. The ArrayOfParamFormats Looks Like: \n";    //This looks good
            for(this->intLoopCounter=0 ; this->intLoopCounter < this->TblPolyFitWithCovInherit::nParams; this->intLoopCounter++) {
                std::cout << this->intLoopCounter << ". this->TblPolyFitWithCovInherit::ArrayOfParamFormats[" << this->intLoopCounter  << "]: " << this->TblPolyFitWithCovInherit::ArrayOfParamFormats[this->intLoopCounter] << std::endl;
            } //2. End of for loop
        } else {
            ;
        }
        
    } //0. End of IF than ELSE
    // -------------------------------------------- Candidate function not viable: no known conversion from 'const char *' to 'const char *const *' for 5th argument; take the address of the argument with &
    //                         √                √                 √               √              √                  √                √               √
    this->TblPolyFitWithCovInherit::res = PQexecParams(this->BaseClassInherit::conn, this->TblPolyFitWithCovInherit::insertIntoCov, this->TblPolyFitWithCovInherit::nParams, this->TblPolyFitWithCovInherit::ArrayOfOid, this->TblPolyFitWithCovInherit::ArrayOfPtrTpParamsInNbo, this->TblPolyFitWithCovInherit::ArrayOfParamLengths, this->TblPolyFitWithCovInherit::ArrayOfParamFormats, 1 );
    //this->TblPolyFitWithCovInherit::res = PQexecParams(this->TblPolyFitWithCovInherit::conn, this->TblPolyFitWithCovInherit::insertInto, 8, this->TblPolyFitWithCovInherit::arrayOfOid, arrayOfparamValues, arrayOfParamLengths, arrayOfParamFormats, 1 );
    if (PQresultStatus(this->TblPolyFitWithCovInherit::res) != PGRES_COMMAND_OK) { //INSERTS usually return PGRES_COMMAND_OK when they do a successful insert.\
        PGRES_COMMAND_OK is the successful indicator status for commands, like INSERT INTO, that reurn no data.
        std::cout << "Insert Operation failed: " << PQerrorMessage(this->TblPolyFitWithCovInherit::conn);
        this->TblPolyFitWithCovInherit::rc=1;
    } else {
        std::cout << this->BaseClassInherit::ptrGTOD->doTimePoint() <<": Successful insert for case " << cases[this->BaseClassInherit::debugFlags.intMyCase] << std::endl;  
    }
    PQclear(this->TblPolyFitWithCovInherit::res);
    if(!this->BaseClassInherit::selectFromTableDoesPQFinish) {
        PQfinish(this->BaseClassInherit::conn); //Only do PQFinish if so directed by calling\
        program, selectFromTable.cpp. Otherwise it is the calling program's resposibility to doPQFinish.\
        So, if boolean data member BaseClassInherit::selectFromTableDoesPQFinish \
        is set to false, then this member function of TblPolyFitWithCovInherit, named doInsertInto,\
        will do the PQFinish.
    }
//    delete tfloat;
//   delete tdouble;
//    delete tshort;
    /* No longer needed since we now statically define the array in each instance of the object named
     TblPolyFitWithCovInherit
    if (this->TblPolyFitWithCovInherit::ptrCovNbo != nullptr) {
        delete [] this->TblPolyFitWithCovInherit::ptrCovNbo;
        this->TblPolyFitWithCovInherit::ptrCovNbo = nullptr; //Indicates we did a delete.
    }

    if (this->TblPolyFitWithCovInherit::ufloat.ptrToCovarianceMatrix) {
        delete [] this->TblPolyFitWithCovInherit::ufloat.ptrToCovarianceMatrix; //The calling program, \
        identified as selectFromTable.cpp, allocated this storage, but, by mutual agreement, \
        we're deleting it right here.
        this->TblPolyFitWithCovInherit::ufloat.ptrToCovarianceMatrix = nullptr; //Mark as deleted.
    }
    */
    /* These are no longer used since we define these as static arrays in each instance of the object named\
     TblPolyFitWithCovInherit.
     //    delete [] this->TblPolyFitWithCovInherit::arrayOfparamValues;
     //    delete [] this->TblPolyFitWithCovInherit::arrayOfParamLengths;
     //    delete [] this->TblPolyFitWithCovInherit::arrayOfParamFormats;
     //    delete [] this->TblPolyFitWithCovInherit::arrayOfOid;
     */
    return this->TblPolyFitWithCovInherit::rc;
}

