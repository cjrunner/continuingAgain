//
//  SetupForMultiFit.hpp
//  energyUsage
//
//  Created by Clifford Campo on 5/13/18.
//  Copyright © 2018 CliffordCampo. All rights reserved.
//


#ifndef __SETUPFORMULTIFITINHERIT__
#define __SETUPFORMULTIFITINHERIT__

// #include "insertIBucketizedEnergyValuesIntoTable.hpp" //The Class for SetupForMultiFit is defined here.
//
//  2018-05-20T11:45:10 Name chaged to setupForMultifit.hpp from
//  insertIBucketizedEnergyValuesIntoTable.hpp
//  energyUsage
//
//  Created by Clifford Campo on 5/5/18, 2018-05-20
//  Copyright © 2018 CliffordCampo. All rights reserved.
//
//  The class, SetupForMultiFit , inherits from the BaseClassInherit
//

#include <iostream>
#include <string>
#include <sstream>
#include <gsl/gsl_blas.h>
#include <gsl/gsl_multifit.h>
#include <gsl/gsl_statistics.h>
#include "baseClassInherit.hpp" //Need this to resolve the inherited MakeConnectionString class.
#include "AllocateWorkingVectors.hpp"
//#include "TblPolyFitWithCovInherit.hpp"
class SetupForMultiFitInherit : public BaseClassInherit, virtual public AllocateWorkingVectors, virtual public Align  { //SetupForMultiFitInherit inherits from BaseClassInherit. This \
    means that BaseClassInherit's constructor starts constructing before Class SetupForMultiFinInherit, \
    Which starts its constructing activities before Class TblPolyFitWithCovInherit. Need to do a\
    #include "baseClassInherit.hpp above!

private:
    size_t polynomialDegree; //From constructor parameter;
    size_t numberOfEntries;  // From count
    const double *arrayOfValues; //Pointer to array of current values, per enum, above, bucketColNames
    size_t &maxNumberOfRows = numberOfEntries;  // Use a c++-type reference rather than a C-type pointer.
    
public:

//        BaseClassInherit *;

    float ArrayCovarianceMatrix[ 16 ];
    SetupForMultiFitInherit *self;
    size_t onePlusPolynomialDegree;
    const  double initialValue = 1;
    double dblwork;
    double computedValue;
    double chiSquared;
    double *ptrChiSquared;
    double TSS; //total sum of squares
    double goodnessOfFit;
    double correlationCoefficient;
    double meanOfIndependentVariable;
    double meanOfDependentVariables;
    double traceOfCovarientMatrix;
    float  oneFloatValue;
    float *ptrCovarianceMatrixArray;
    size_t loopCounter; 
    gsl_multifit_linear_workspace *work;
    gsl_matrix *independentVariable;
    gsl_matrix *covarienceMatrix;
    gsl_vector *independentVariableTemp;
    gsl_vector *dependentVariable;
    gsl_vector *computedDependetVariable;
    gsl_vector *weights;
    gsl_vector *coefficients;
    int row;
    int col;
    int rc;
    //2018-06-28T08:40:10: the following doubles have been moved here from being local variables \
    in member function SetupForMultiFit::computeCorrelationBetweenIndependentAndDependentVariables
    double  trialComputedDependentVariable;   //2018-06-28T08:40:10
    double T;                                 //2018-06-28T08:40:10
    mutable double A;                          //2018-06-28T08:40:10
    mutable double B;                          //2018-06-28T08:40:10
    mutable double C;                          //2018-06-28T08:40:10
    mutable double D;                          //2018-06-28T08:40:10
    std::ostringstream stringStreamForOutput;         //std::string doesn't work 
    //Declaration of various member functions
    virtual ~SetupForMultiFitInherit();
    SetupForMultiFitInherit(char, size_t)  ;
    SetupForMultiFitInherit(char)  ;  //Used for inheritance operations
    int doMultiFit(void) ;
    
    void  setIntoMatrix(gsl_matrix *, int, int, double);
    
    void  setIntoVector(gsl_vector *, int, double);
    
    double  getFromMatrix(gsl_matrix *, int, int);
    
    double  getFromVector(gsl_vector *, int); 
    
//    int  outputPolynomial(TblPolyFitWithCovInherit *, const char *, const char *);
    
    double  outputCovarianceMatrixAndReturnChi2( const char *, const char *, bool=true);
    
    void captureIndependentVariableValues( int, double []);  //☞Note  that the `[]` is necessary in this declaration
    
    double computeTrace(double *, int);
    
    double  computeCorrelationBetweenIndependentAndDependentVariables(void);
    
    double computeGoodnessOfResults (void);
    
    double getCoefficient(int );
    
    double getChi2(void);
    double returnChiSquared( void );
    double getCorrelationCoefficient(void);
    double *getPointerToCovarianceMatrix(void);
    void setNumberOfEntries(size_t);
    void setPtrToArrayOfValues(const double *);
   
};
#endif /* __SETUPFORMULTIFITINHERIT__ */
