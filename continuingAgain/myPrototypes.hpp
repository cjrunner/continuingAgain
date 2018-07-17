//
//  insertIntoTable.hpp
//  c++Postgres
//
//  Created by Clifford Campo on 5/1/18.
//  Copyright © 2018 CliffordCampo. All rights reserved.
//
#ifndef __MYPROTOTYPES__
#define __MYPROTOTYPES__

//#include "/usr/local/pgsql/pgsql101/include/libpq-fe.h"
#include <iostream>
#include <string>
#include <stdio.h>
#include <time.h> //Necessary for currentDateTime function
#define POLYNOMIALDEGREE 3
#define NUMBEROFCOEFF 1 + POLYNOMIALDEGREE
#define NUMBEROFCOVARIANCEMATRIXENTRIES   16
#define NUMBEROFPARAMETERS    NUMBEROFCOEFF + NUMBEROFCOVARIANCEMATRIXENTRIES + NMBEROFSHORTS + NUMBEROFCHI2 + NUMBEROFCC
#define TOTALNUMBEROFPARAMETERS    NUMBEROFPARAMETERS
#define NOTONCOMMANDLINE 0
#define ONCOMMANDLINE 1 + NOTONCOMMANDLINE
#define MAXBUFSIZE  80
#define BUCKETSIZE 2
#define DODROPTABLE 1
#define DOCREATETABLE 2*DODROPTABLE
#define DOINSERTINTOTABLE 2*DOCREATETABLE
#define SIZEOFBUFFERCONTAININGMOTHEROFALLSELECTSTATEMNTS 500
#define SIZEOFONEROW 12
#define INITIALVALUE 0
#define NUMBEROFAVGTEMPS 100; //number of elements to be initially allocated to the gsl_vector type named independentVariableAvgTemp
//Beginning of error IDs
#define EXECUTESQLFAILED 1
#define FETCHALLFAILED 2
#define BEGINTRANSACTIONFAILED 3
#define UNKNOWNTRANSACTIONFAILED 4
#define DROPTABLEFAILED 5
#define CREATETABLEFAILED 6
#define CONNECTTODATABASEFAILED 7
#define INSERTINTOFAILED 8
#define FAILEDTOCREATETEMPTABLE 9
#define COPYFAILURE 10
#define COPYENDFAILURE 11
// End of error IDs
#define NUMBEROFHELPMESSAGRSEGMENTS 14
#define M1M2KWHCASE 0
#define M1KWHCASE 1
#define M2KWHCASE 2
#define MAINCASE  3
#define NUMBEROFCASES 1 + MAINCASE
#define MYTEMPFILENAME "/tmp/temporary_temp_energy_usage_file.csv"
#define TBLPOLYFITPARAMETERS VIACMDLINETBLPOLYFITPARAMETERS
enum bucketColNames  {_avgtemp, _avgeu, _stddeveu, _mineu, _maxeu, _countu};  //This enum did not work when placed in BaseClassInherit
enum colnames {_currentavtempf,  _previousavtempf,  _davtempf,  _currentavtempk,  _previousavtempk,   _previousdate,   _currentdate, _dday,  _previousm1kwh, _currentm1kwh, _m1Usage, _previousm2kwh,  _currentm2kwh, _m2Usage, _AvgDailyUsage};
static inline void rtrim(std::string &);

#endif /* __MYPROTOTYPES__ */

