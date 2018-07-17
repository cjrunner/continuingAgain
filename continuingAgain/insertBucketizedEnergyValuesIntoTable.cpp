//
//  insertIBucketizeEnergyValuesIntoTable.cpp
//  energyUsage
//
//  Created by Clifford Campo on 5/5/18.
//  Copyright © 2018 CliffordCampo. All rights reserved.
//


//
//  insertIntoTable.cpp
//  c++Postgres
//
//  Created by Clifford Campo on 5/1/18.
//  Copyright © 2018 CliffordCampo. All rights reserved.
//
#include <iostream>
//#include "/usr/local/pgsql/pgsql101/include/libpq-fe.h"
//#include "/usr/local/pgsql/pgsql101/include/libpq-fe.h"
#include "/usr/local/pgsql95/pgsql090513/include/libpq-fe.h" //changed 2018-07-10T14:24:39 from 090224
#include "myPrototypes.hpp"
#include "TblPolyFitWithCovInherit.hpp"
#include "setupForMultiFitInherit.hpp"

using namespace std;
extern const char *insertIntoTable[];
//extern char *ptr_char_conn_string;
int insertBucketizedEnergyValuesIntoTable(double, double, double, double, double, int, TblPolyFitWithCovInherit *);
int insertBucketizedEnergyValuesIntoTable( double avgtemp, \
                                          double avgeu, \
                                          double stddeveu, \
                                          double mineu, \
                                          double maxeu, \
                                          int counteu, \
                                          TblPolyFitWithCovInherit *ptrbc) {
    ptrbc->BaseClassInherit::rc[INSERTBUCKETIZEDVALUESINTOTABLE] = WONDERFUL; //Preset the return code anticipating success
    
    ptrbc->BaseClassInherit::sqlI = new char [400];
    
    ptrbc->BaseClassInherit::indVarArray [ ptrbc->BaseClassInherit::countOfGoodEntries - 1] = avgtemp;
    sprintf (ptrbc->BaseClassInherit::sqlI, insertIntoTable[ptrbc->BaseClassInherit::debugFlags.intMyCase], avgtemp, avgeu, stddeveu, mineu, maxeu, counteu);  //CREATE the insert into SQL statement
    if ( ptrbc->BaseClassInherit::debugFlags.debug3  ) std::cout << "Insert looks like: " << ptrbc->BaseClassInherit::sqlI << std::endl;

//    ptrbc->BaseClassInherit::connI = PQconnectdbParams(static_ArrayOfConnectKeyWords, static_ArrayOfValues, 0);  //Copy over the pointer
    ptrbc->BaseClassInherit::connI = PQconnectdb("dbname=LocalWeather host=Localhost user=cjc port=5436");  //Try to connect to database for the purpose of doing inserts.
    if (PQstatus(ptrbc->BaseClassInherit::connI) != CONNECTION_OK) {
        cerr << "Connection to database, for the purpose of inserting data, failed for this reason:\n" << PQstatus(ptrbc->BaseClassInherit::connI) << endl;
        ptrbc->BaseClassInherit::rc[INSERTBUCKETIZEDVALUESINTOTABLE] = CONNECTTODATABASEFAILED;
    } else {

    ptrbc->BaseClassInherit::resI = PQexec(ptrbc->connI, ptrbc->sqlI);
    
    if (PQresultStatus(ptrbc->BaseClassInherit::resI) != PGRES_COMMAND_OK) {
        cerr << "Line " << __LINE__ << " of file " << __FILE__ << ": SQL statement\n" << ptrbc->BaseClassInherit::sqlI << "\nfailed with error: " << PQerrorMessage(ptrbc->BaseClassInherit::connI) << endl;
        ptrbc->BaseClassInherit::rc[INSERTBUCKETIZEDVALUESINTOTABLE] = INSERTINTOFAILED;
    } else {
        ptrbc->BaseClassInherit::numberOfReturnedRowsI = PQntuples(ptrbc->BaseClassInherit::resI);
        ptrbc->BaseClassInherit::numberOfReturnedColumnsI = PQnfields(ptrbc->BaseClassInherit::resI);
        if (ptrbc->BaseClassInherit::debugFlags.debug3)  std::cout << "Record inserted successfully with return status of: " << PQstatus(ptrbc->BaseClassInherit::connI) << ". Number of returned rows: " << ptrbc->BaseClassInherit::numberOfReturnedRowsI << ", and the numberOfReturnedColumns: " << ptrbc->BaseClassInherit::numberOfReturnedColumnsI << endl;
    }

    PQclear(ptrbc->BaseClassInherit::resI);
    }
    PQfinish(ptrbc->BaseClassInherit::connI);
    if (ptrbc->BaseClassInherit::sqlI != nullptr) {
        delete [] ptrbc->BaseClassInherit::sqlI;
        ptrbc->BaseClassInherit::sqlI = nullptr; //Mark as deleted
    }
    return ptrbc->BaseClassInherit::rc[INSERTBUCKETIZEDVALUESINTOTABLE];
}





