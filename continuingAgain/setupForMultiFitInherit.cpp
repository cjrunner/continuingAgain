//
//  setupForMultiFitInherit.cpp
//  energyUsageWithoutThe_PQXX_Bullshit
//
//  Created by Clifford Campo on 5/20/18 and then made to support object inheritance on 2018-07-02.
//  Copyright © 2018 CliffordCampo. All rights reserved.
//
#include <iostream>
#include <cstdlib>
#include <ctime>
#include <cstring>
#include <string>
#include <stdexcept>
#include <iterator>
#include <sstream>

#include <stdio.h>

#include <chrono>
//#include "setupForMultiFit.hpp"
#include <gsl/gsl_blas.h>

#include <gsl/gsl_matrix.h>
#include <gsl/gsl_matrix_float.h>

#include <gsl/gsl_multifit.h>


#include <gsl/gsl_statistics.h>
#include <gsl/gsl_statistics_float.h>

#include <gsl/gsl_vector.h>
#include <gsl/gsl_vector_float.h>

#include "TblPolyFitWithCovInherit.hpp"
#include "myPrototypes.hpp"
#include "GetTimeOfDay.hpp"

//std::ostringstream *current_Date_Time(BaseClassInherit *);  //Latest c++ version replaces the C version currentDateTime
SetupForMultiFitInherit::~SetupForMultiFitInherit() {  //Destructor
    try {
        gsl_matrix_free(this->independentVariable);
        } catch ( ... ) {
            std::cout << "Line " << __LINE__ << " in file " << __FILE__ << " caught abnormal termination with gsl_matrix_free(this->independentVariable)" << std::endl;
        }
    
    try {
        gsl_matrix_set_zero(this->covarienceMatrix);
        gsl_matrix_free(this->covarienceMatrix);
    } catch ( ... ) {
            std::cout << "Line " << __LINE__ << " in file " << __FILE__ << " caught abnormal termination with gsl_matrix_free(this->covarienceMatrix);" << std::endl;
    }
    
    try {
        gsl_vector_free(this->dependentVariable);
    } catch ( ... ) {
        std::cout << "Line " << __LINE__ << " in file " << __FILE__ << " caught abnormal termination with  gsl_vector_free(this->dependentVariable);" << std::endl;
    }

    try {
           gsl_vector_free(this->weights);
    } catch ( ... ) {
        std::cout << "Line " << __LINE__ << " in file " << __FILE__ << " caught abnormal termination with  gsl_vector_free(this->weights);" << std::endl;
    }

    try {
        gsl_vector_free(this->coefficients);
    } catch ( ... ) {
        std::cout << "Line " << __LINE__ << " in file " << __FILE__ << " caught abnormal termination with  gsl_vector_free(this->coefficients);" << std::endl;
    }
    
    try {
        gsl_multifit_linear_free(this->work);
    } catch ( ... ) {
        std::cout << "Line " << __LINE__ << " in file " << __FILE__ << " caught abnormal termination with  gsl_multifit_linear_free(this->work);" << std::endl;
    }

    try {
        gsl_vector_free(this->computedDependetVariable);
    } catch ( ... ) {
        std::cout << "Line " << __LINE__ << " in file " << __FILE__ << " caught abnormal termination with  gsl_vector_free(this->computedDependetVariable);" << std::endl;
    }
    
    
}
SetupForMultiFitInherit::SetupForMultiFitInherit( char ourCase ) : BaseClassInherit( ourCase ),  AllocateWorkingVectors( ), Align( ) {
    this->polynomialDegree = POLYNOMIALDEGREE;
    this->onePlusPolynomialDegree = 1 + polynomialDegree;
    ptrCovarianceMatrixArray = ArrayCovarianceMatrix;


}
SetupForMultiFitInherit::SetupForMultiFitInherit(char ourCase,  size_t count) : BaseClassInherit( ourCase ), AllocateWorkingVectors( ), Align( )  { //Constructor
    self = this;
    ptrCovarianceMatrixArray = ArrayCovarianceMatrix;
    this->ptrChiSquared = &chiSquared;
//    this->arrayOfValues = aOV; <===== DO THIS LATER. NOT NEEDED DURING INITIAL CONSTRUCTION.
    this->polynomialDegree = POLYNOMIALDEGREE;
    this->onePlusPolynomialDegree = 1+polynomialDegree;
    this->numberOfEntries = count; //Number of rows inserted into table \
    tbl_bucketized_by_tempf_ranges[0 or 1 or 2] which is usually 38
    this->independentVariable = gsl_matrix_alloc(this->numberOfEntries, this->onePlusPolynomialDegree );
    gsl_matrix_set_zero(this->independentVariable);
    this->covarienceMatrix = gsl_matrix_alloc(this->onePlusPolynomialDegree, this->onePlusPolynomialDegree);
    //    gsl_matrix_set_zero(this->covarienceMatrix);
    this->dependentVariable = gsl_vector_alloc(this->numberOfEntries);
    //   gsl_vector_set_zero(this->dependentVariable);
    this->computedDependetVariable = gsl_vector_alloc(this->numberOfEntries);
    this->weights = gsl_vector_alloc(this->numberOfEntries);
    //    gsl_vector_set_zero(this->weights);
    this->coefficients = gsl_vector_alloc(this->onePlusPolynomialDegree);
    //    gsl_vector_set_zero(this->coefficients);
    this->work = gsl_multifit_linear_alloc(this->numberOfEntries, this->onePlusPolynomialDegree);
}
int    SetupForMultiFitInherit::doMultiFit(void) {
    this->rc = gsl_multifit_wlinear(this->independentVariable, \
                                    this->weights, \
                                    this->dependentVariable, \
                                    this->coefficients, \
                                    this->covarienceMatrix, \
                                    this->ptrChiSquared, \
                                    this->work);
    return this->rc;
}
void  SetupForMultiFitInherit::setIntoMatrix(gsl_matrix *whichMatrix, int row, int column, double value) {
    gsl_matrix_set(whichMatrix, row, column, value);
}
void  SetupForMultiFitInherit::setIntoVector(gsl_vector *whichVector, int row, double value) {
    gsl_vector_set(whichVector, row, value);
}
double  SetupForMultiFitInherit::getFromMatrix(gsl_matrix *whichMatrix, int row, int column) {
    return (gsl_matrix_get(whichMatrix, row, column));
}
double  SetupForMultiFitInherit::getFromVector(gsl_vector *whichVector, int row) {
    return ( gsl_vector_get(whichVector, row) );
}
/*
int  SetupForMultiFitInherit::outputPolynomial(TblPolyFitWithCovInherit *ptrPF, const char *leadOffString, const char *trailingString) { 

    this->stringStreamForOutput << current_Date_Time(this->BaseClassInherit::self) << "\t" << leadOffString;
    for (this->row = 0; this->row < this->coefficients->size ; this->row++) {
        if (this->row > 0) {
            this->stringStreamForOutput << " + " << gsl_vector_get(this->coefficients, this->row) << "*T^" << this->row;
        } else {
            this->stringStreamForOutput << gsl_vector_get(this->coefficients, this->row) ;
        }
    }
    this->stringStreamForOutput << trailingString << "\n";
    return 0;
}
 */
double  SetupForMultiFitInherit::outputCovarianceMatrixAndReturnChi2( const char *leadOffString, const char *trailingString, bool makeCopy) {
    // unused    double firstC = *this->ptrChiSquared;
    //2018-06-27T05:57:25 Added `BaseClassInherit to Calling Sequence, so I can get to bc->outstring more easily.
 //                *ptrbc->outstringOS << smf->outputCovarianceMatrixAndReturnChi2(ptrbc, "cov = [", cases[ptrbc->debugFlags.intMyCase] ) << "☜ χ-squared" << cases[ptrbc->debugFlags.intMyCase] << "\n";
    ptrFloatCovarianceMatrix = ArrayCovarianceMatrix; //Establish addressability to our own copy of the covariance matrix.
    for (this->row = 0; this->row < this->covarienceMatrix->size1; this->row++) {

        this->stringStreamForOutput << this->BaseClassInherit::ptrGTOD->doTimePoint() << "\t" << leadOffString;
        for (this->col = 0; this->col < this->covarienceMatrix->size2; this->col++) {
            stringStreamForOutput  << *(this->covarienceMatrix->data + this->covarienceMatrix->size1*this->row + this->col);
            //Here we will insert the value of covariance matrix into our array of data reserved for it;
            this->oneFloatValue = (float)*(this->covarienceMatrix->data + this->covarienceMatrix->size1*this->row + this->col);
            *(this->SetupForMultiFitInherit::ptrCovarianceMatrixArray + this->covarienceMatrix->size1 * this->row + this->col) = this->oneFloatValue;
            if (makeCopy) { //If makeCopy is true [the default] then we will make a copy of the covariance matrix for object BaseClassInherit.
                *(this->BaseClassInherit::ptrFloatCovarianceMatrix +  this->covarienceMatrix->size1 * this->row + this->col) = this->oneFloatValue; //Added 2018-07-15T13:53:06
            }
            if (col < this->covarienceMatrix->size1 - 1) {
               stringStreamForOutput << ", "; //Output a comma to separate covarient values
            } else {
                //Output closed brackets if we've just output the rightmost covarient value for this row.
                stringStreamForOutput << "]\t " << trailingString << "\n";
            }
        }  //End of inner for loop (col)
    } // End of outter for loop (row)
    return *this->ptrChiSquared;
}
double SetupForMultiFitInherit::returnChiSquared( ) {
    return *this->ptrChiSquared;
}
double  SetupForMultiFitInherit::computeCorrelationBetweenIndependentAndDependentVariables ( ) {
   
    this->trialComputedDependentVariable=0;
    this->A = gsl_vector_get(this->coefficients, 0);
    this->B = gsl_vector_get(this->coefficients, 1);
    this->C = gsl_vector_get(this->coefficients, 2);
    this->D = gsl_vector_get(this->coefficients, 3);
    for (this->row=0; this->row < this->numberOfEntries; this->row++) {
//       this->T = this->bc->indVarArray[this->row];
        this->T = this->BaseClassInherit::indVarArray[ this->row ];
        this->trialComputedDependentVariable = this->A + (this->B  +  (this->C  + this->D *this->T )* this->T) * this->T; 
//        this->bc->depVarArray[this->row]= trialComputedDependentVariable;
        this->BaseClassInherit::depVarArray[this->row] = trialComputedDependentVariable;
    }
    this->meanOfIndependentVariable = gsl_stats_mean(this->BaseClassInherit::indVarArray, 1, this->numberOfEntries);
    this->meanOfDependentVariables = gsl_stats_mean(this->BaseClassInherit::depVarArray, 1, this->numberOfEntries);
    this->correlationCoefficient = gsl_stats_correlation(this->BaseClassInherit::indVarArray, 1, this->BaseClassInherit::depVarArray, 1, this->numberOfEntries );

    return this->correlationCoefficient ;
}

double  SetupForMultiFitInherit::computeGoodnessOfResults ( ) {
    //computeGoodnessOfResults defines GoodnessOfResults as the square of the accumulated normalized difference between the expected (modeled)
    //and observed dependent variables ( ∑(((modeledResultForEachIndependentVariable - eachObservedDependentVariable)/modeledResultForEachIndependentVariable))^2 ).
    //Return the square root of the sum of the squares of the percent difference between the expected (modeled) and observed dependent variables.
    double trialComputedDependentVariable=0;
    double accumulatedVarience2 = 0;
    double delta=0;
    double deltaNormalized=0;
    const double A = gsl_vector_get(this->coefficients, 0);
    const double B = gsl_vector_get(this->coefficients, 1);
    const double C = gsl_vector_get(this->coefficients, 2);
    const double D = gsl_vector_get(this->coefficients, 3);
    double T;
    for (this->row=0; this->row <  this->BaseClassInherit::grc; this->row++) {
        T = *(this->BaseClassInherit::ptrIndVariableArray + this->row);
        trialComputedDependentVariable = A + (B  +  (C  + D *T )* T) * T;
        *(this->BaseClassInherit::ptrExpDepVarArray + this->row)= trialComputedDependentVariable;
        delta = (*(this->BaseClassInherit::ptrExpDepVarArray + this->row) - *(this->BaseClassInherit::ptrObsDepVarArray + this->row));
        *(this->BaseClassInherit::ptrExpMinusObsDepVarArray + this->row) = delta;
        deltaNormalized =  delta/(*(this->BaseClassInherit::ptrExpDepVarArray + this->row) );
        accumulatedVarience2 += ( deltaNormalized *  deltaNormalized ); //Accumulate the square of normalized varience.
    }
    
    return sqrt(accumulatedVarience2) ; //Return the square root of the sum of the squares of the normalized varience
}
double SetupForMultiFitInherit::computeTrace(double *someMatrix, int onePlusPolynomialDegree) {
    int &matrixSize = onePlusPolynomialDegree;
    this->traceOfCovarientMatrix = 0;
    for (this->loopCounter =0; this->loopCounter < matrixSize; this->loopCounter++  ) {
        this->traceOfCovarientMatrix += *(someMatrix + (matrixSize + 1) * this->loopCounter ); //∑ along the diagonal of the covarient matrix.
    }
    return this->traceOfCovarientMatrix; 
}
void  SetupForMultiFitInherit::captureIndependentVariableValues( int theRow, double  valueArray[]) {
    this->dblwork = *(valueArray +  _stddeveu);
    if (this->BaseClassInherit::debugFlags.debug2) {
        std::cout << theRow << ". Weights, or *(valueArray +  _stddeveu), Looks like: " << *(valueArray +  _stddeveu) << std::endl;
    }
    gsl_vector_set(this->weights, theRow, this->dblwork);
    this->dblwork = *(valueArray +  _avgeu);
    gsl_vector_set(this->dependentVariable, theRow, this->dblwork );
    this->computedValue = this->initialValue;
    for(this->loopCounter = 0; this->loopCounter< this->onePlusPolynomialDegree; this->loopCounter++)  {   // Note that this->loopCounter Serves as the column counter, too.
        gsl_matrix_set(this->independentVariable, theRow, this->loopCounter, this->computedValue);
        this->computedValue *= valueArray[_avgtemp]; //For each column in this current row, place progressively increasing powers of the independent variable
    }
    
}
double SetupForMultiFitInherit::getCoefficient(int coeffID) {
    return gsl_vector_get(this->coefficients, coeffID );
}
double SetupForMultiFitInherit::getCorrelationCoefficient(void) {
    return this->correlationCoefficient;
}
double SetupForMultiFitInherit::getChi2(void) {
    return *this->ptrChiSquared;
}
void SetupForMultiFitInherit::setNumberOfEntries(size_t noe) {
    this->numberOfEntries = noe;
    this->independentVariable = gsl_matrix_alloc(this->numberOfEntries, this->onePlusPolynomialDegree ); //This causes memory leak meater to jump from 2 to 5!
    gsl_matrix_set_zero(this->independentVariable);
    this->covarienceMatrix = gsl_matrix_alloc(this->onePlusPolynomialDegree, this->onePlusPolynomialDegree);
    //    gsl_matrix_set_zero(this->covarienceMatrix);
    this->dependentVariable = gsl_vector_alloc(this->numberOfEntries);  //This gives a memory leak causing memor leak meter to go from 5 to 6
    //   gsl_vector_set_zero(this->dependentVariable);
    this->computedDependetVariable = gsl_vector_alloc(this->numberOfEntries);
    this->weights = gsl_vector_alloc(this->numberOfEntries); //This causes memor leak meter to go from 6 to 7.
    //    gsl_vector_set_zero(this->weights);
    this->coefficients = gsl_vector_alloc(this->onePlusPolynomialDegree);
    //    gsl_vector_set_zero(this->coefficients);
    this->work = gsl_multifit_linear_alloc(this->numberOfEntries, this->onePlusPolynomialDegree);
}
void SetupForMultiFitInherit::setPtrToArrayOfValues(const double *arrayOfValues) {
    this->arrayOfValues = arrayOfValues;
}
