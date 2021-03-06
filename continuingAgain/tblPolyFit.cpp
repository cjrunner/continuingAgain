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
#include "TblPolyFitWithCovInherit.hpp"
#include "tblPolyFit.hpp"
#include "varyingType.hpp"

TblPolyFit::TblPolyFit(float *ptrCovMx, short pd, int nv, int rsltFmt, bool withCov) {
//      R U N    M O D E       R U N    M O D E       R U N    M O D E       R U N    M O D E

    this->ptrToCovarianceMatrix = ptrCovMx; //Make our own copy of a pointer to the covariance matrix. \
    This class, TblPolyFit, deptends upon selectFromTable.cpp to have filled in the cavariance matrix \
    with values of type float.

    ptrCovNbo = new float[pd * pd]{};  //Get another array, that mirrors the original array of covariance data, but this array will have the covariance data in NetworkByteOrder.

    
    //At this point all covariance data should be in Network Byte Order.
    
    //connString = cs;
    nParams = nv;
    resultFormat = rsltFmt;
    paramValues = new const char [nv]{};
    paramTypes = new const Oid[nv]{INT2OID,FLOAT8OID, FLOAT8OID, FLOAT8OID, FLOAT8OID, FLOAT4ARRAYOID, INT2OID, FLOAT8OID, FLOAT8OID};
    paramLengths = new const  int[nv]{2,8,8,8,8, 16*4, 2,8,8};
    paramFormats = new const int [nv]{1,1,1,1,1,   1,  1,1,1};
    /**/
}

TblPolyFit::~TblPolyFit() {
#ifdef DOTABLETblPolyFitWithCovInherit
//    if(ptrSUMF == NULL) delete [] ptrCov;  //Only in test mode do we allocate ptrCov, so now we must delete it because ptrSUMF == NULL => test mode.
    delete [] ptrCovNbo;
#endif
 //   if (paramValues != nullptr || paramValues != (const char *)'\0') delete [] paramValues; 
    if (paramTypes != nullptr) delete [] paramTypes;
    if (paramLengths != nullptr)delete [] paramLengths;
    if (paramFormats != nullptr) delete [] paramFormats;
}
#ifdef DOTABLEPOLYFITWITHCOV
//χ-squared value ---------------------------------------------------------------------------------------------------+
//Correlation Coefficient --------------------------------------------------------------------------------+          |
 //Meter Descriptor ----------------------------------------------------------------------------+         |          |
 //coefficient C3 -------------------------------------------------------------------+          |         |          |
 //coefficient C2 --------------------------------------------------------+          |          |         |          |
 //coefficient C1 ---------------------------------------------+          |          |          |         |          |
 //coefficient C0 -----------------------------------+         |          |          |          |         |          |
 //short int showing polynomial degree ---+          |         |          |          |          |         |          |
 //    SetupForMultiFit            ----+             |          |         |          |          |          |         |          |
//                                     V             V          V         V          V          V          V         V          V
int TblPolyFit::doInsertInto(float *ptrCovMx, short *p1, double *p2, double *p3, double *p4, double *p5, float *p9, short *p6, double *p7, double *p8) {
    /*Used for tbl_poly_fit_with_cov, which looks like:
     LocalWeather=# \d tbl_poly_fit_with_cov
     Table "public.tbl_poly_fit_with_cov"
     Column |           Type           |                             Modifiers
     --------+--------------------------+--------------------------------------------------------------------
     id     | integer                  | not null default nextval('tbl_poly_fit_with_cov_id_seq'::regclass)
     lt     | timestamp with time zone |
     pd     | smallint                 | not null
     coeff  | double precision[]       |
     cov    | real[]                   |
     kind   | smallint                 |
     correl | double precision         |
     chi2   | double precision         |
     Indexes:
     "tbl_poly_fit_with_cov_pkey" PRIMARY KEY, btree (id)
    Note that fields named coeff and cov are arrays. */
    VaryingType<short> *tshort =new VaryingType<short>();
    tshort->in64.d64 = *p1;
    tshort->VaryingType<short>::toNetworkByteOrder();
    this->nbop1 = tshort->out64.d64;
    this->paramValues[0] = (char *)&this->nbop1; //Points to the polynomial degree, as type short, in NBO format.
// --------------------------------------------
    tshort->in64.d64 = *p6; //Reuse the short object that we created above
    tshort->VaryingType<short>::toNetworkByteOrder();
    this->nbop6 = tshort->out64.d64; //Points to the C0 coefficient in NBO format
    this->paramValues[21] = (char *)&this->nbop6; //Points to the meter id, as type short, in NBO format.
// --------------------------------------------
    VaryingType<double> *tp2_8 =new VaryingType<double>();  //This tp2_8 object, for type double, should serve us for making input parameters p2, p3, p4, p5, p7, and p8 in network byte order.
    tp2_8->in64.d64 = *p2;
    tp2_8->VaryingType<double>::toNetworkByteOrder();
    this->nbop2 = tp2_8->out64.d64;
 //   *(this->ptrCoeffNbo+0) = *p2;
    *(this->ptrCoeffNbo+0) = this->nbop2 ; //Points to the C0 coefficient, as type double, in NBO format
    this->paramValues[1] =  (char *)&this->nbop2 ; //Points to the C0 coefficient, as type double, in NBO format
// --------------------------------------------
    tp2_8->in64.d64 = *p3;
    tp2_8->VaryingType<double>::toNetworkByteOrder();
    this->nbop3 = tp2_8->out64.d64;
    *(this->ptrCoeffNbo+1) = *p3;
//    *(this->ptrCoeffNbo+1) = this->nbop3 ; //Points to the C1 coefficient, as type double, in NBO format
    this->paramValues[2] =  (char *)&this->nbop3; //Points to the C1 coefficient, as type double, in NBO format
// --------------------------------------------
    tp2_8->in64.d64 = *p4;
    tp2_8->VaryingType<double>::toNetworkByteOrder();
    this->nbop4 = tp2_8->out64.d64;
    *(this->ptrCoeffNbo+2) = *p4;
//    *(this->ptrCoeffNbo+2) = this->nbop4 ; //Points to the C2 coefficient, as type double, in NBO format
    this->paramValues[3] =  (char *)&this->nbop4; //Points to the C2 coefficient, as type double, in NBO format
// --------------------------------------------
    tp2_8->in64.d64 = *p5;
    tp2_8->VaryingType<double>::toNetworkByteOrder();
    this->nbop5 = tp2_8->out64.d64;
//    *(this->ptrCoeffNbo+3) = *p5;
    *(this->ptrCoeffNbo+3) = this->nbop5 ; //Points to the C3 coefficient, as type double, in NBO format
    this->paramValues[4] =  (char *)&this->nbop5;  //Points to the C3 coefficient, as type double, in NBO format
// --------------------------------------------
    
    for (this->intLoopCounter = 0; this->intLoopCounter < NUMBEROFCOVMX; this->intLoopCounter++) {
        //*(this->ptrCovNbo + i)
        //Note: The constructor should have converted the test data (the test data is defined in the constructor) to network byte order.
        *(this->intermediateValue + this->intLoopCounter) = this->ptrCovNbo[this->intLoopCounter];
        this->paramValues[5+this->intLoopCounter] = (char *)((this->intermediateValue + this->intLoopCounter));
    }
    // --------------------------------------------
    tp2_8->in64.d64 = *p7;
    tp2_8->VaryingType<double>::toNetworkByteOrder();
    this->nbop7 = tp2_8->out64.d64;
    this->paramValues[22] =  (char *)&this->nbop7;  //Points to the correlation coefficient, as type double, in NBO format
// --------------------------------------------
    tp2_8->in64.d64 = *p8;
    tp2_8->VaryingType<double>::toNetworkByteOrder();
    this->nbop8 = tp2_8->out64.d64;
    this->paramValues[23] =  (char *)&this->nbop8;  //Points to the chi-squared valye, as type double, in NBO format
// --------------------------------------------
//    this->paramValues[1] = (char *)this->ptrCoeffNbo;
//    this->paramValues[2] = (char *)this->ptrCovNbo; //Points to the array, each element of type float, comprising the covariance matrix
    // Note how c++ initializes an array ***without the use of an equal sign*** between the closing `]` and the opening `{`.
    //Oid *arrayOfOid = new Oid[8]{INT2OID, FLOAT8OID, FLOAT8OID, FLOAT8OID, FLOAT8OID, INT2OID, FLOAT8OID, FLOAT8OID};
    //this->paramTypes = arrayOfOid;
    //What follows is the crazyiest type casting I've ever seen. See The PostgreSQL 9.0 Reference Manual, Volume 2: Programming Guide, bottom of page 82. I highlighted this in yellow.
    //const char * const *arrayOfparamValues = new const char *[8]{ (char *)&this->nbop1, (char *)&this->nbop2, (char *)&this->nbop3, (char *)&this->nbop4, (char *)&this->nbop5, (char *)&this->nbop6, (char *)&this->nbop7, (char *)&this->nbop8};
    //this->paramValues = arrayOfparamValues;
    //int *arrayOfParamLengths = new  int[8]{sizeof(short), sizeof(double), sizeof(double), sizeof(double), sizeof(double), sizeof(double), sizeof(double), sizeof(double)};
    //this->paramLengths = arrayOfParamLengths;
    //const int *arrayOfParamFormats = new const int[8]{BINARYRESULTS, BINARYRESULTS, BINARYRESULTS, BINARYRESULTS, BINARYRESULTS, BINARYRESULTS, BINARYRESULTS, BINARYRESULTS};
    //this->paramFormats = arrayOfParamFormats;
// --------------------------------------------
    this->conn  = PQconnectdb(this->connString);
    if(PQstatus(this->conn) != CONNECTION_OK) {
        std::cout << "Connection to database failed: " << PQerrorMessage(this->conn) << std::endl;
        PQfinish(this->conn);
        exit(1);
    } else {
        std::cout << "Looks like we connected to database ok." << std::endl;
    }
// --------------------------------------------
//                           √                √                 √               √              √                  √                √               √
    this->res = PQexecParams(this->conn, this->insertIntoCov, this->nParams, this->paramTypes, this->paramValues, this->paramLengths, this->paramFormats, 1 );
    if (PQresultStatus(this->res) != PGRES_COMMAND_OK) { //INSERTS usually return PGRES_COMMAND_OK when they do a successful insert.\
        PGRES_COMMAND_OK is the successful indicator status for commands, like INSERT INTO, that reurn no data.
        std::cout << "Insert Operation failed: " << PQerrorMessage(this->conn);
        this->rc=1;
    } else {
        std::cout << "Successful insert!" << std::endl;
    }
    PQclear(this->res);
    PQfinish(this->conn);
    delete tp2_8;
    delete tshort;
    return this->rc;
}
#else
//int doInsertInto(float *ptrCovMx, short *p1, double *p2, double *p3, double *p4, double *p5, float *p9, short *p6, double *p7, double *p8);
/*  Used for tbl_poly_fit */
//χ-squared value ---------------------------------------------------------------------------------------------------------------+
//Correlation Coefficient --------------------------------------------------------------------------------+          |
//Meter Descriptor ----------------------------------------------------------------------------+         |          |
//coefficient C3 -------------------------------------------------------------------+          |         |          |
//coefficient C2 --------------------------------------------------------+          |          |         |          |
//coefficient C1 ---------------------------------------------+          |          |          |         |          |
//coefficient C0 -----------------------------------+         |          |          |          |         |          |
//short int showing polynomial degree ------+          |         |          |          |          |         |          |
//                ----------+                |          |         |          |          |          |         |          |
//                          V               V          V         V          V          V          V         V          V
int TblPolyFit::doInsertInto(short *p1, double *p2, double *p3, double *p4, double *p5, float *p9, short *p6, double *p7, double *p8) {

    
    VaryingType<short> *tshort =new VaryingType<short>();
    tshort->in64.d64 = *p1;
    tshort->VaryingType<short>::toNetworkByteOrder();
    this->nbop1 = tshort->out64.d64;
    // --------------------------------------------
    tshort->in64.d64 = *p6; //Reuse the short object that we created above
    tshort->VaryingType<short>::toNetworkByteOrder();
    this->nbop6 = tshort->out64.d64;
    // --------------------------------------------
    VaryingType<double> *tdouble =new VaryingType<double>();  //This tdouble object, for type double, should serve us for making input parameters p2, p3, p4, p5, p7, and p8 in network byte order.
    tdouble->in64.d64 = *p2;
    tdouble->VaryingType<double>::toNetworkByteOrder();
    this->nbop2 = tdouble->out64.d64;
    // --------------------------------------------
    tdouble->in64.d64 = *p3;
    tdouble->VaryingType<double>::toNetworkByteOrder();
    this->nbop3 = tdouble->out64.d64;
    // --------------------------------------------
    tdouble->in64.d64 = *p4;
    tdouble->VaryingType<double>::toNetworkByteOrder();
    this->nbop4 = tdouble->out64.d64;
    // --------------------------------------------
    tdouble->in64.d64 = *p5;
    tdouble->VaryingType<double>::toNetworkByteOrder();
    this->nbop5 = tdouble->out64.d64;
    // --------------------------------------------
    tdouble->in64.d64 = *p7;
    tdouble->VaryingType<double>::toNetworkByteOrder();
    this->nbop7 = tdouble->out64.d64;
    // --------------------------------------------
    tdouble->in64.d64 = *p8;
    tdouble->VaryingType<double>::toNetworkByteOrder();
    this->nbop8 = tdouble->out64.d64;
    // --------------------------------------------
    // Note how c++ initializes an array ***without the use of an equal sign*** between the closing `]` and the opening `{`.
    Oid *arrayOfOid = new Oid[8]{INT2OID, FLOAT8OID, FLOAT8OID, FLOAT8OID, FLOAT8OID, INT2OID, FLOAT8OID, FLOAT8OID};
    this->paramTypes = arrayOfOid;
    //What follows is the crazyiest type casting I've ever seen. See The PostgreSQL 9.0 Reference Manual, Volume 2: Programming Guide, bottom of page 82. I highlighted this in yellow.
    const char **arrayOfparamValues = new const char *[8]{ (char *)&this->nbop1, (char *)&this->nbop2, (char *)&this->nbop3, (char *)&this->nbop4, (char *)&this->nbop5, (char *)&this->nbop6, (char *)&this->nbop7, (char *)&this->nbop8};
    this->paramValues = *arrayOfparamValues;
    int *arrayOfParamLengths = new  int[8]{sizeof(short), sizeof(double), sizeof(double), sizeof(double), sizeof(double), sizeof(short), sizeof(double), sizeof(double)};
    this->paramLengths = arrayOfParamLengths;
    const int *arrayOfParamFormats = new const int[8]{BINARYRESULTS, BINARYRESULTS, BINARYRESULTS, BINARYRESULTS, BINARYRESULTS, BINARYRESULTS, BINARYRESULTS, BINARYRESULTS};
    this->paramFormats = arrayOfParamFormats;
    // --------------------------------------------
    this->conn  = PQconnectdb(this->connString);
    if(PQstatus(this->conn) != CONNECTION_OK) {
        std::cout << "Connection to database failed: " << PQerrorMessage(this->conn) << std::endl;
        PQfinish(this->conn);
        exit(1);
    } else {
        std::cout << "Looks like we connected to database ok." << std::endl;
    }
    this->res = PQexecParams(this->conn, this->insertINTO, 8, arrayOfOid, arrayOfparamValues, arrayOfParamLengths, arrayOfParamFormats, 1 );
    if (PQresultStatus(this->res) != PGRES_COMMAND_OK) { //INSERTS usually return PGRES_COMMAND_OK when they do a successful insert.\
        PGRES_COMMAND_OK is the successful indicator status for commands, like INSERT INTO, that reurn no data.
        std::cout << "Insert Operation failed: " << PQerrorMessage(this->conn);
        this->rc=1;
    } else {
        std::cout << "Successful insert!" << std::endl;
    }
    PQclear(this->res);
    PQfinish(this->conn);
    delete tdouble;
    delete tshort;
    return this->rc;
}
#endif
