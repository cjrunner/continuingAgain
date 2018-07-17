//
//  TblPolyFitWithCovInherit.hpp
//  multiThreadEnergyUsageWithPostgreSQLTableOutput
//
//  Created by Clifford Campo on 6/26/18.
//  Copyright © 2018 CliffordCampo. All rights reserved.
//

#ifndef TblPolyFitWithCovInherit_hpp
#define TblPolyFitWithCovInherit_hpp




//
//  tblPolyFit.hpp
//  multiThreadEnergyUsage
//
//  Created by Clifford Campo on 6/15/18.
//  Copyright © 2018 CliffordCampo. All rights reserved.
//



#include <stdio.h>
#include <iostream>
//#include "/usr/local/pgsql/pgsql101/include/libpq-fe.h"
//#include "/usr/local/pgsql/pgsql101/include/libpq-fe.h"
#include "/usr/local/pgsql95/pgsql090513/include/libpq-fe.h" //changed 2018-07-10T14:18:59 from 090224
#include "myPrototypes.hpp"
#include "baseClassInherit.hpp"
#include "setupForMultiFitInherit.hpp" 
//#include "tblPolyFit.hpp"
#define NUMBEROFPQEXECPARAMS 24
#define FLOAT4ARRAYOID 1021
#define FLOAT8ARRAYOID 1022
#define FLOAT4OID 700
#define FLOAT8OID 701
#define TEXTOID 25
#define INT2OID 21
#define NUMBER_OF_COVARIANCE_MATRIX_ENTRIES 16
#define NMBEROFSHORTS 2
#define NUMBEROFCHI2  1
#define NUMBEROFCC    1

#define  TEXTRESULTS 0    //Used to set resultFormat in public space
#define  BINARYRESULTS 1 //Used to set resultFormat in public space
//class SetupForMultiFit;  //doing a forward declaration of SetupForMultifit instead of:\
#include "/Users/cjc/c++/energyUsage/multiThreadEnergyUsageWithPostgreSQLTableOutput/multiThreadEnergyUsageWithPostgreSQLTableOutput/setupForMultiFit.hpp


class TblPolyFitWithCovInherit : public SetupForMultiFitInherit { //TblPolyFitWithCovInherit is at the bottom of the\
    inheritance chain. Note that during Construction SetupForMultiFit gets called before TblPolyFit.
private:
    size_t szTPFWCI = sizeof(TblPolyFitWithCovInherit);
    const char *nameTPFWCI="TblPolyFitWithCovInherit";
 //   const char *connString = "dbname=LocalWeather port=5436 user=cjc";  //Connection String goes here.
    const int szShort=sizeof(short);
    const int szInt = sizeof(int);
    const float szFloat = sizeof(float);
    const double szDouble = sizeof(double);
protected:



    //   enum kinds {_m1m2kwh, _m1kwh, _m2kwh};
    // Should look something like: INSERT INTO tbl_poly_fit (lt,pd,coeff,kind,correl,chi2) VALUES(now(), 3, '{51.0091, 1.40023, -0.0656151, 0.000525268}', 'm1kwh', -0.945637, 4.8905);
    //                                                                  OID = FLOAT8OID (701) -----------------------------------+---+
    //                                                                  OID = INT2OID (21) ---------------------------------+    |   |
    //                                                                  OID = FLOAT8OID (701) --------------+---+---+---+   |    |   |
    //                                                                  OID = INT2OID  (21) ----------+     |   |   |   |   |    |   |
    //                                                                                                V     V   V   V   V   V    V   V
    //  const char *insertINTO = "INSERT INTO tbl_poly_fit (lt,pd,coeff,kind,correl,chi2) VALUES (now(), $1,  {$2, $3, $4, $5},$6,  $7, $8);";
    const char *insertINTO = "INSERT INTO tbl_poly_fit (lt,pd,coeff[1], coeff[2], coeff[3], coeff[4], kind,correl, chi2) VALUES (now(), $1,  $2, $3, $4, $5,  $6, $7, $8);";
    // NB: kind = 0 => m1m2kwh, kind = 1 => m1kwh, kind = 2 => m2kwh
    int  rc=0;  //Set the return code, assuming success;
    /*
     Column |           Type           |                             Modifiers
     --------+--------------------------+--------------------------------------------------------------------
     id     | integer                  | not null default nextval('tbl_poly_fit_with_cov_id_seq'::regclass)
     lt     | timestamp with time zone |
     pd     | smallint                 | not null
     coeff  | double precision[]       |
     cov    | real[]                   |
     kind   | smallint                 | 0 => m1m2kwh, 1 => m1kwh, 2 => m2kwh.
     correl | double precision         |
     chi2   | double precision         |
     Indexes:
     "tbl_poly_fit_with_cov_pkey" PRIMARY KEY, btree (id)
     */
    /*  The following worked from the psql command line.
     INSERT INTO  tbl_poly_fit_with_cov (lt, pd, coeff, cov, kind, correl, chi2) values(now(), 3, ARRAY[50.9847,1.40283,-0.0656843,0.000525766], ARRAY[[-0.000407403,3.8962e-05,-9.02632e-07,6.033e-09],[0.0640727,-0.00602311,0.000137125,-9.02632e-07], [-2.95741,0.270319,-0.00602311,3.8962e-05], [34.567,-2.95741,0.0640727,-0.000407403]], 1, 0.945595, 6.87839);
     
     
     */
    const char *insertIntoCov =  "INSERT INTO tbl_poly_fit_with_cov (lt,pd, coeff, cov,  kind, correl, chi2) VALUES(now(), $1, \
    ARRAY[$2,$3,$4,$5],  \
    ARRAY[[$6,$7,$8,$9], [$10, $11, $12, $13], [$14, $15, $16, $17], [$18, $19, $20, $21]], \
    $22, $23, $24) " ; //Arrays of doubles (aka, float8s) have an object id of 1022 per  /Users/cjc/C/postgres10/postgresql-10.1/postgresql-10.1/
public:
    const char *ptrOurCase;
    union {
    short intOurCase; //The above, dereferenced, character -- which is either a '0', or '1', or '2', or '3' -- \
    turned into an integer.
    short intMyCase;
    } ushort;
    string tod;
    TblPolyFitWithCovInherit *self;
//

  //  const char **arrayOfparamValues;
  //  int *arrayOfParamLengths;
  //  const int *arrayOfParamFormats;
//
    double copyOfCoefficients[POLYNOMIALDEGREE+1];
    float  copyOfCovarianceMatrix[ NUMBER_OF_COVARIANCE_MATRIX_ENTRIES];
    float  copyOfCovarianceMatrixInNbo[ NUMBER_OF_COVARIANCE_MATRIX_ENTRIES];
    float intermediateValue[NUMBER_OF_COVARIANCE_MATRIX_ENTRIES];
    double correlationCoefficient;
    double chi2;
    union {
        float *ptrToCovarianceMatrix;
        float *ptrCov;  //Points to covariance matrix (see copyOfCovarianceMatrix, above)
    } ufloat;
    float *ptrCovNbo; //Same as above but each element of array is in network byte order. Points to array \
    copyOfCovarianceMatrixInNbo, above. Addressability established in constructor.
    double *ptrCoeffNbo; //Points to array of Coefficients in NBO order ($2, $3, $4, $5) Object ID 1022
    short  nbop1; //contains the polynomial order, as a short int, in Network Byte Order
    double nbop2;    //contains the c0 coefficient, as a double, in Network Byte Order
    double nbop3;   //contains the c1 coefficient, as a double, in Network Byte Order
    double nbop4;   //contains the c2 coefficient, as a double, in Network Byte Order
    double nbop5;   //contains the c3 coefficient, as a double, in Network Byte Order
    short  nbop6;  //points to meter definition
    double nbop7;   //contains correlation coefficient, as a double, in Network Byte Order
    double nbop8;   //contains χ-squared value, as a double, in Network Byte Order
    union {
    Oid *ptrArrayOfOid;
    const Oid *ptrParamTypes; //Pointer to array of length = nParams and of type Oid (see /Users/cjc/C/postgres10/postgresql-10.1/postgresql-10.1/tmp_install/usr/local/pgsql/pgsql101/include/server/catalog/pg_type.h)
    };
    const char  *ptrParamValues; //Pointer to array of length = nParams
    const int *ptrParamLengths; //Pointer to array of length = nParams. Specifies the actual data lengths of binary-format parameters. Ignored for NULL parameters and text-format parameters.
    const int *ptrParamFormats; //Pointer to array of integers specifying if the value is text (array entry is 0), or binary (array entry is 1). Values passed in binary format require knowledge of the internal representation expected by the backend. For example, integers must be passed in network byte order (BIG ENDIAN). Passing numeric values requires knowledge of the server storage format, as implemented in `src/backend/utils/adt/:::numeric_send()` and `src/backend/utils/adt/numeric.c::numeric_recv()`
    int ArrayOfParamFormats[VIACMDLINETBLPOLYFITPARAMETERS];  //Looks Good
    int ArrayOfParamLengths[VIACMDLINETBLPOLYFITPARAMETERS];  //Looks Good
    Oid ArrayOfOid[VIACMDLINETBLPOLYFITPARAMETERS];           //Looks Good
    char *ArrayOfPtrTpParamsInNbo[VIACMDLINETBLPOLYFITPARAMETERS];
    PGconn *conn;
    PGresult *res;
    int intLoopCounter = 0;
    int nParams; // Number of Parameters. Also the lengths (number of elements) of the following four arrays. This value used by constructor for allocating the dynamic storage when instantiating the next four pointers.
    //   SetupForMultiFit *smf;
    int resultFormat; //When 0 then will obtain rsults in text format. When 1, then will obtain results in binary format.

    //TblPolyFitWithCovInherit(char, int=NUMBEROFPQEXECPARAMS, int=BINARYRESULTS );  //Eliminated 2018-07-15T11:08:27. NOT USED
    //         +-------- pointer to array containing the covariance matrix. Only valid if bool=true
    //         V
    TblPolyFitWithCovInherit(char, short, int=VIACMDLINETBLPOLYFITPARAMETERS, int=TEXTRESULTS, bool=true); //This \
    constructor takes the third (right-most) parameter and uses it to set the value of nParams and then \
    dynamically allocate storageTblPolyFit(SetupForMultiFit *,  int=TOTALNUMBEROFPARAMETERS, int=TEXTRESULTS);

    //                +-------- pointer to array containing the covariance matrix.
    //                V
    int doInsertInto(float *, short *, double *, double *, double *, double *, float *, short *, double *, double *);   //When doing insert into tbl_poly_fit_with_cov (WITH covariance matrix)
    int doInsertInto(short *, double *, double *, double *, double *, float *, short *, double *, double *);   //When doing insert into tbl_poly_fit (WITHOUT covariance matrix)
    int doInsertInto(const char *);
    int doInsertInto(void);
    virtual ~TblPolyFitWithCovInherit();  //Needs to delete the four dynamic storage objects  allocated by the above constructor.
    int outputPolynomial(const char *leadOffString, const char *trailingString);
    
    int doInsertIntoCov(void);
};
#endif /* TblPolyFitWithCovInherithpp */
