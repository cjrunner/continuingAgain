//
//  selectFromTable.cpp
//  c++Postgres
//
//  Created by Clifford Campo on 5/1/18.
//  Copyright © 2018 CliffordCampo. All rights reserved.
//
/*
 SELECT Operation

*/
/*
int     polynomialFit(int polynomialdegree, int reccntr, double *XVALin, double *YVALin, double *FPXVAL, double *IPXVAL, double *returnJD, double *returnZD, char *returnISO8601, double *TSS, double *goodnessOfFit);   // <==== New Way of Calling what used to be a test program
 
 id  |    date    |   time   | microsofttime | m1kwh | m2kwh | avtempf | avtempk | m1m2kwh |       jd       | siteid
 ------+------------+----------+---------------+-------+-------+---------+---------+---------+----------------+--------
 2874 | 2018-05-04 | 05:01:00 |   43224.20903 | 42940 |  7734 |         |         |   11.51 |  2458243.20903 |
 2873 | 2018-05-03 | 06:05:00 |  43223.253472 | 42930 |  7733 | 62.2122 | 289.935 |    7.18 | 2458242.253472 |      9
 2872 | 2018-05-02 | 06:41:00 |  43222.278472 | 42924 |  7732 | 53.9762 | 285.359 |   12.32 | 2458241.278472 |      9
 2871 | 2018-05-01 | 07:18:00 |  43221.304167 | 42913 |  7731 | 52.4003 | 284.484 |   25.16 | 2458240.304167 |      9
 2870 | 2018-04-30 | 06:30:00 |  43220.270833 | 42889 |  7729 | 47.0272 | 281.498 |   23.81 | 2458239.270833 |      9
 2868 | 2018-04-29 | 07:19:00 |  43219.304861 | 42867 |  7728 | 57.8237 | 287.496 |   14.12 | 2458238.304861 |      9
 2867 | 2018-04-28 | 10:55:00 |  43218.454861 | 42856 |  7727 |  51.817 | 284.159 |   21.83 | 2458237.454861 |      9
 2865 | 2018-04-27 | 06:20:00 |  43217.263889 | 42833 |  7724 | 57.8552 | 287.514 |   13.57 | 2458236.263889 |      9
 2866 | 2018-04-26 | 07:20:00 |  43216.305556 | 42821 |  7723 |   58.89 |  288.09 |   17.11 | 2458235.305556 |      9
 2864 | 2018-04-25 | 06:05:00 |  43215.253472 | 42804 |  7722 | 57.0455 | 287.064 |   15.92 | 2458234.253472 |      9
 2863 | 2018-04-24 | 07:28:00 |  43214.311111 | 42790 |  7721 | 55.8987 | 286.427 |   15.42 | 2458233.311111 |      9
 2862 | 2018-04-23 | 06:34:00 |  43213.273611 | 42775 |  7720 | 55.3796 | 286.139 |   17.86 | 2458232.273611 |      9
 */
#include <stdio.h>
#include <iostream>
#include <fstream> 
#include <cstring>
#include <string>
#include <sys/select.h>
#include <sys/time.h>
//#include <unistd.h>`
#include <sstream>
//#include "effectConnection.hpp"
//#include "/usr/local/pgsql/pgsql101/include/libpq-fe.h"
//#include "/usr/local/pgsql/pgsql101/include/libpq-fe.h"
#include "/usr/local/pgsql95/pgsql090513/include/libpq-fe.h"
#include "myPrototypes.hpp"
#include "AllocateWorkingVectors.hpp"
#include "baseClassInherit.hpp"
//#include "insertIBucketizedEnergyValuesIntoTable.hpp"
#include "BitFlags.hpp"
#include "setupForMultiFitInherit.hpp"
#include "tblPolyFit.hpp"
#include "TblPolyFitWithCovInherit.hpp"
// include "/Users/cjc/c++/energyUsage/multiThreadEnergyUsageWithPostgreSQLTableOutput/multiThreadEnergyUsageWithPostgreSQLTableOutput/setupForMultiFit.hpp"
#include "alignField.hpp"
#include "varyingType.hpp"
#include "GetTimeOfDay.hpp"

// #include "tblPolyFit.hpp"
//std::ostringstream *current_Date_Time(BaseClassInherit *);
int     polynomialFit(int, int, double *, double *,  double *, double *,  double *, double *);
using namespace std;

#ifdef METER //Provided by the compiler -D switch
#define WHICHMETER METER
#else
#define WHICHMETER m1m2kwh   //Set the Default
#endif
extern const char *insertIntoTable[];
extern const char *createTable[];     //From message.cpp
extern const char *readBackTable[];  //SQL SELECT for reading back the data that we previously inserted into \
table tbl_bucketized_by_tempf_ranges
extern const char *begin_Transaction;
extern const char *end_Transaction;
extern const char *fetchALLTransaction;
extern const char *close_Transaction;
extern const char *cases[]; //Array of strings specifying cases
extern const char *myTempFiles[]; //Array of the names of Temp Files for the m1m2kwh case, the m1kwh case, and \
the m2kwh case.
extern const char *dependentVariableColumn[];
extern const char *theCaseLabels[];
extern const char *copySQL;
extern const char *theMotherOfAllUnionSelectStatements[];
extern PGconn *mycon;
//extern const char *ptr_char_conn_string;
extern const char *motherOfAllSelectStatements[];
extern const char *newMotherOfAllSelectStatements[];
extern const char *copy_file[];
//extern const char *dropThenCreateTempTable;
extern const char *drop_tbl_temp_eu[];
extern const char *dropTable[];
extern const char *create_tbl_temp_eu[];
extern const char * staticArrayOfConnectKeyWords; //Defined as a global in main
extern const char * staticArrayOfValues;          //Defined as a global in main
extern const char *sqlStat1;  //Added 2018-07-09T08:36:31
//std::string currentDateTime(BaseClassInherit *);
void exit_nicely(TblPolyFitWithCovInherit *,  int, int) ;
void exit_nicely(TblPolyFitWithCovInherit *tpfwci, int rc, int lineNumber) {
    PQfinish(tpfwci->BaseClassInherit::conn); //Close this task's connection.
    cout << tpfwci->BaseClassInherit::ptrGTOD->doTimePoint() << "\t" << __FILE__ << "Line " << lineNumber << "\nexit_nicely is terminating this program with exit code: " << rc << cases[tpfwci->BaseClassInherit::debugFlags.intMyCase] << endl;
    tpfwci->BaseClassInherit::selectFromTable_RC = rc; //Duplicate the exit code as selectFromTable_RC this was the mother task will know why the child exited
    exit (rc);
}

//int createTheTable(TblPolyFitWithCovInherit *, const char *, bool = true);
//int dropTheTable(TblPolyFitWithCovInherit *, const char *);
//int closePortal(TblPolyFitWithCovInherit *, const char *, bool = true);
//int endTransaction(TblPolyFitWithCovInherit *, const char *, bool = true);
//int executeSQL(TblPolyFitWithCovInherit *, const char *, bool = true);
//int beginTransaction(TblPolyFitWithCovInherit *, const char *, bool = true);
//int fetchAllTransaction(TblPolyFitWithCovInherit *, const char *, bool = false);
int insertBucketizedEnergyValuesIntoTable(double, double, double, double, double, int, TblPolyFitWithCovInherit *);
template<typename T>
std::string to_string(T t)
{
    std::ostringstream oss;
    oss << t;
    return oss.str();
}
static const char* const static_ArrayOfConnectKeyWords[] = {"dbname", "port", "user", "password", "host" };
static const char* const static_ArrayOfValues[] = {"LocalWeather", "5436", "cjc", NULL, "localhost"};
// E N T R Y    P O I N T         E N T R Y    P O I N T         E N T R Y    P O I N T         E N T R Y    P O I N T
int selectFromTable(TblPolyFitWithCovInherit *pfWC, int myRC, bool inMultiThreadMode) {

    pfWC->BaseClassInherit::inMultiThreadMode = inMultiThreadMode; //Indicate that we're in multiThread mode

    enum bucketColNames  {_avgtemp, _avgeu, _stddeveu, _mineu, _maxeu, _countu};  //This enum did not work when placed in BaseClassInherit
    myRC = 0;
    pfWC->BaseClassInherit::selectFromTable_RC = myRC; //Assume success!

//    pfWC->BaseClassInherit::conn = PQconnectdb("dbname=LocalWeather host=localhost user=cjc port=5436");
    pfWC->BaseClassInherit::conn = PQconnectdbParams(static_ArrayOfConnectKeyWords, static_ArrayOfValues, 0);  //Copy over the pointer ¡¡We die here the second time around with !7
    if(PQstatus(pfWC->BaseClassInherit::conn) != CONNECTION_OK ) {
        cerr << pfWC->BaseClassInherit::ptrGTOD->doTimePoint() << "\tCase: " << cases[pfWC->BaseClassInherit::debugFlags.intMyCase]<< ", Connection to database failed because: " << PQerrorMessage(pfWC->BaseClassInherit::conn) << "\n Connection String looks like: "<< pfWC->BaseClassInherit::connectionString  << ".\n We're exiting with exit code of 1."<< endl;
        ;
        exit(1);
    } else {
        cout << pfWC->BaseClassInherit::ptrGTOD->doTimePoint() <<": It looks like we connected to the database `" << PQdb(pfWC->BaseClassInherit::conn) << "`, on host `" << PQhost(pfWC->BaseClassInherit::conn)  << ",` ok for case " << cases[pfWC->BaseClassInherit::debugFlags.intMyCase] << endl;
        pfWC->BaseClassInherit::connectionResponsibilityCount++;
    }
    if (pfWC->BaseClassInherit::debugFlags.debug2 ) {
        std::cout << "Host Server version: " << PQserverVersion(pfWC->BaseClassInherit::conn) << std::endl;
        std::cout << \
        dependentVariableColumn[pfWC->BaseClassInherit::debugFlags.intMyCase] << \
        pfWC->BaseClassInherit::ptrGTOD->doTimePoint() << \
        "\nThe mother of all select statements looks like:\n" << \
        newMotherOfAllSelectStatements[pfWC->BaseClassInherit::debugFlags.intMyCase] << std::endl;
    }
//============


    pfWC->BaseClassInherit::len = strlen(newMotherOfAllSelectStatements[pfWC->BaseClassInherit::debugFlags.intMyCase]);
    pfWC->BaseClassInherit::sql = new char [ pfWC->BaseClassInherit::len ];
    pfWC->BaseClassInherit::len = sprintf(pfWC->BaseClassInherit::sql, newMotherOfAllSelectStatements[pfWC->BaseClassInherit::debugFlags.intMyCase], pfWC->BaseClassInherit::debugFlags.includeM2InAverages);  //At this point pfWC->BaseClassInherit::debugFlags.includeM2InAverages is either a 1 or a zero, depending upon the presences of the -c command line switch. If -c switch is NOT present then pfWC->BaseClassInherit::debugFlags.includeM2InAverages = 1, else its zero.

    pfWC->BaseClassInherit::len = strlen(sqlStat1);
    pfWC->BaseClassInherit::len += 4 * (11 + strlen( dependentVariableColumn[pfWC->BaseClassInherit::debugFlags.intMyCase]) ); //Add to the length the anticipated length of the case labels and other data. m1m2kwh is the longest it can be
    pfWC->BaseClassInherit::subCharBuffer = new char[  pfWC->BaseClassInherit::len ]; //Allocate space for our buffer
    pfWC->BaseClassInherit::len = sprintf(pfWC->BaseClassInherit::subCharBuffer,sqlStat1, dependentVariableColumn[pfWC->BaseClassInherit::debugFlags.intMyCase], \
                         dependentVariableColumn[pfWC->BaseClassInherit::debugFlags.intMyCase], \
                         dependentVariableColumn[pfWC->BaseClassInherit::debugFlags.intMyCase], \
                         dependentVariableColumn[pfWC->BaseClassInherit::debugFlags.intMyCase]);
//===============

//     MOVED TO myPrototypes   enum colnames {_currentavtempf,  _previousavtempf,  _davtempf,  _currentavtempk,  _previousavtempk,   _previousdate,   _currentdate, _dday,  _previousm1kwh, _currentm1kwh, _m1Usage, _previousm2kwh,  _currentm2kwh, _m2Usage, _AvgDailyUsage};

    pfWC->BaseClassInherit::res = PQexec(pfWC->BaseClassInherit::conn,  createTable[pfWC->BaseClassInherit::debugFlags.intMyCase]);  //Drop the table, if it exists, then recreate the table specific for this case.
    if (PQresultStatus(pfWC->BaseClassInherit::res) != PGRES_COMMAND_OK) {
        std::cerr << "Line " << __LINE__ << ", File " << __FILE__ << ", case "  << cases[pfWC->BaseClassInherit::debugFlags.intMyCase] <<": Failed to execute the SQL statement: " << createTable[pfWC->BaseClassInherit::debugFlags.intMyCase] << ". Error cause:\n" << PQerrorMessage(pfWC->BaseClassInherit::conn) << std::endl;
        exit_nicely(pfWC, BEGINTRANSACTIONFAILED, __LINE__);
    }
    PQclear(pfWC->BaseClassInherit::res);
//Now place our database in the BEGIN transaction state.

    pfWC->BaseClassInherit::res = PQexec(pfWC->BaseClassInherit::conn,  begin_Transaction);
    if (PQresultStatus(pfWC->BaseClassInherit::res) != PGRES_COMMAND_OK) {
        std::cerr << "Line " << __LINE__ << ", File " << __FILE__ << ", case " << cases[pfWC->BaseClassInherit::debugFlags.intMyCase] <<": Failed to execute the SQL statement: " << begin_Transaction << ". Error cause:\n" << PQerrorMessage(pfWC->BaseClassInherit::conn) << std::endl;
        exit_nicely(pfWC, 3, __LINE__);
    }
    //    PQclear(pfWC->BaseClassInherit::res);

//    rc = executeSQL(ptrC, pfWC->BaseClassInherit::res, ptrBigSQL);
//Now execute the NEW mother of all select statements.
    pfWC->BaseClassInherit::res = PQexec(pfWC->BaseClassInherit::conn,  newMotherOfAllSelectStatements[pfWC->BaseClassInherit::debugFlags.intMyCase]);
    if (PQresultStatus(pfWC->BaseClassInherit::res) != PGRES_COMMAND_OK) {
        std::cerr << "Line " << __LINE__ << " of file " << __FILE__ << ": Failed to execute the SQL statement: " << newMotherOfAllSelectStatements[pfWC->BaseClassInherit::debugFlags.intMyCase] << ". Error cause:\n" <<
        PQerrorMessage(pfWC->BaseClassInherit::conn) << cases[pfWC->BaseClassInherit::debugFlags.intMyCase] << std::endl;
        exit_nicely(pfWC, EXECUTESQLFAILED, __LINE__);

    } else {
        PQclear(pfWC->BaseClassInherit::res); //¡¡ Get a !1 here. When we declare a portal/cursor do we need to then do a PQclear?
    }
//    rc = fetchAllTransaction(ptrC,pfWC->BaseClassInherit::res, fetchALLTransaction);
//Now fetch the results of the above mother of all select statements which is in declared cursor named myportal

    pfWC->BaseClassInherit::res = PQexec(pfWC->BaseClassInherit::conn, "FETCH ALL in myportal" );
    if (PQresultStatus(pfWC->BaseClassInherit::res) != PGRES_TUPLES_OK) {
        std::cerr << "Failed to execute the SQL FETCH ALL: " << pfWC->BaseClassInherit::sql << ". Error cause:\n" <<
        PQerrorMessage(pfWC->BaseClassInherit::conn) << cases[pfWC->BaseClassInherit::debugFlags.intMyCase] << std::endl;
        cerr << "Line " << __LINE__ << "Of file " << __FILE__ << "Failed when attempting to execute this sql:\n" << newMotherOfAllSelectStatements[pfWC->BaseClassInherit::debugFlags.intMyCase] << "\nWe got a return status of: " << PQresultStatus(pfWC->BaseClassInherit::res) << cases[pfWC->BaseClassInherit::debugFlags.intMyCase] << endl;
        exit_nicely(pfWC, FETCHALLFAILED, __LINE__);
    }

    pfWC->BaseClassInherit::numberOfReturnedRows = PQntuples(pfWC->BaseClassInherit::res);
    pfWC->BaseClassInherit::numberOfReturnedColumns = PQnfields(pfWC->BaseClassInherit::res);

//    Align *af = new Align(ASSUMEDBUFFERSIZE);
    const char *headerRow[] ={" currentavtempf | prevavtempf |  ∆avtempf  | currentavtempk | prevavtempk |   yesterday    |   today    | ∆day | cm1kwh | pm1kwh | m1Usage | cm2kwh | pm2kwh | m2Usage | AvgDailyUsage",
                                "----------------+-------------+------------+----------------+-------------+----------------+------------+------+------- +------- +---------+------- +- ------+---------+---------------+"}; \
    /*  Example of what's returned by this mother of all select statements:
          col00           col01         col02          col03          col04          col05          col06     col07  col08   col09    col10    col11   col12   col13        col14
          16w              13w          12wide        16wide          13wide         16wide        12wide      6W      8w      7w      9w       7w     7w       9w         15w
     currentavtempf | prevavtempf |  ∆avtempf  | currentavtempk | prevavtempk |   yesterday    |   today    | ∆day | m1kwh | m1kwh | m1Usage | m2kwh | m2kwh | m2Usage | AvgDailyUsage
    ----------------+-------------+------------+----------------+-------------+----------------+------------+------+-------+-------+---------+-------+-------+---------+---------------
         64.4       |       38.86 |     -25.54 |         291.15 |      276.96 |     2014-12-03 | 2014-12-01 |    2 | 12776 | 12720 |      56 |  2670 |  2665 |       5 |          30.5
        46.15       |       38.86 |      -7.29 |         281.01 |      276.96 |     2014-12-03 | 2014-12-02 |    1 | 12776 | 12739 |      37 |  2670 |  2667 |       3 |            40
        38.86       |       39.27 |       0.41 |         276.96 |      277.19 |     2014-12-04 | 2014-12-03 |    1 | 12813 | 12776 |      37 |  2673 |  2670 |       3 |            40
        39.27       |       34.82 |      -4.45 |         277.19 |      274.71 |     2014-12-05 | 2014-12-04 |    1 | 12852 | 12813 |      39 |  2675 |  2673 |       2 |            41
        34.82       |       40.55 |       5.73 |         274.71 |      277.90 |     2014-12-06 | 2014-12-05 |    1 | 12890 | 12852 |      38 |  2678 |  2675 |       3 |            41
             .          .            .                     .          .                .          .           .       .          .      .      .           .               .
             .          .            .                     .          .                .          .           .       .          .      .      .           .               .
             .          .            .                     .          .                .          .           .       .          .      .      .           .               .

*/


enum colwidths {col00 = 16, col01=13, col02=12, col03=16, col04=13, col05=16, col06=12, col07=6,col08=8, col09=8, col10=9, col11=8, col12=8, col13=9, col14=15};
#define NEITHERFIRSTORLAST 2 //This means we will not include the first elemet or the last element because they are misleading and erroneous: \
      a) The first element lacks the previous day's reading;  \
      b) Likely, the last element does not have a full-day's worth of readings.
    
//    AllocateWorkingVectors *const ptrAWV = new AllocateWorkingVectors (pfWC->BaseClassInherit::numberOfReturnedRows - NEITHERFIRSTORLAST);
//    pfWC::BaseClassInherit->setupAWV(pfWC->BaseClassInherit::numberOfReturnedRows - NEITHERFIRSTORLAST);
    pfWC->AllocateWorkingVectors::setupAWV(pfWC->BaseClassInherit::numberOfReturnedRows - NEITHERFIRSTORLAST);
    if (pfWC->BaseClassInherit::debugFlags.debug2 ) std::cout  << headerRow[0] << "\t" << theCaseLabels[pfWC->BaseClassInherit::debugFlags.intMyCase] << std::endl;
    if (pfWC->BaseClassInherit::debugFlags.debug2 ) std::cout  << headerRow[1] << "\t" << theCaseLabels[pfWC->BaseClassInherit::debugFlags.intMyCase] << std::endl;

    const size_t bufferSize = SIZEOFONEROW  *  pfWC->BaseClassInherit::numberOfReturnedRows;  //buffersize becomes 15180 after this instruction.
#define THISROWISNULL 1
//    int pfWC->BaseClassInherit::numberOfReturnedRows;

    pfWC->BaseClassInherit::copyBuffer = new char[bufferSize]; //Get sufficient space (15180 bytes in some cases) to store, in one buffer, all\
    temperture and energy usage (eu) data.
    memset(pfWC->BaseClassInherit::copyBuffer, 0, bufferSize); //Start with a clean copyBuffer
    pfWC->BaseClassInherit::bufferOffset = 0;  //The constructor should initialize this.
    pfWC->BaseClassInherit::grc = 0;
    pfWC->BaseClassInherit::brc = 0;
//    Align *const ptrAF = new Align(ASSUMEDBUFFERSIZE);
   pfWC->Align::alignSetup(   headerRow[1]  );  //headerRow[1] has the necessary --------+-------- … ----+---+-- etc.\
    for the alignment object to determine cell boundaries. `+` sign designates/delineates a cell boundry.

    for (pfWC->BaseClassInherit::loopCounter = 0; pfWC->BaseClassInherit::loopCounter < pfWC->BaseClassInherit::numberOfReturnedRows; pfWC->BaseClassInherit::loopCounter++ ) {
        if (PQgetisnull(pfWC->BaseClassInherit::res, pfWC->BaseClassInherit::loopCounter, _AvgDailyUsage) == THISROWISNULL) { //PQgetisnull Returns 1 if row IS NULL. Returns 0 if row is NOT NULL
            ++pfWC->BaseClassInherit::brc; //increment the bad row count.
            continue; //Skip NULL values of today's avgtempf and the first record and the last record
        }
        try {
            if (pfWC->BaseClassInherit::debugFlags.debug2 ) {
                cout <<pfWC->Align::alignCenter(0,  PQgetvalue(pfWC->BaseClassInherit::res, pfWC->BaseClassInherit::loopCounter, _currentavtempf) ) << "|";
                cout <<pfWC->Align::alignCenter(1,  PQgetvalue(pfWC->BaseClassInherit::res, pfWC->BaseClassInherit::loopCounter, _previousavtempf) ) << "|";
                cout <<pfWC->Align::alignCenter(2,  PQgetvalue(pfWC->BaseClassInherit::res, pfWC->BaseClassInherit::loopCounter, _davtempf) ) << "|" ;
                cout <<pfWC->Align::alignCenter(3,  PQgetvalue(pfWC->BaseClassInherit::res, pfWC->BaseClassInherit::loopCounter, _currentavtempk) ) << "|" ;
                cout <<pfWC->Align::alignCenter(4,  PQgetvalue(pfWC->BaseClassInherit::res, pfWC->BaseClassInherit::loopCounter, _previousavtempk) ) << "|" ;
                cout <<pfWC->Align::alignCenter(5,  PQgetvalue(pfWC->BaseClassInherit::res, pfWC->BaseClassInherit::loopCounter, _previousdate) ) << "|" ;
                cout <<pfWC->Align::alignCenter(6,  PQgetvalue(pfWC->BaseClassInherit::res, pfWC->BaseClassInherit::loopCounter, _currentdate) ) << "|" ;
                cout <<pfWC->Align::alignCenter(7,  PQgetvalue(pfWC->BaseClassInherit::res, pfWC->BaseClassInherit::loopCounter, _dday) ) << "|" ;
                cout <<pfWC->Align::alignCenter(8,  PQgetvalue(pfWC->BaseClassInherit::res, pfWC->BaseClassInherit::loopCounter, _currentm1kwh) ) << "|" ;
                cout <<pfWC->Align::alignCenter(9,  PQgetvalue(pfWC->BaseClassInherit::res, pfWC->BaseClassInherit::loopCounter, _previousm1kwh) ) << "|" ;
                cout <<pfWC->Align::alignCenter(10, PQgetvalue(pfWC->BaseClassInherit::res, pfWC->BaseClassInherit::loopCounter, _m1Usage) ) << "|" ;
                cout <<pfWC->Align::alignCenter(11, PQgetvalue(pfWC->BaseClassInherit::res, pfWC->BaseClassInherit::loopCounter, _currentm2kwh) ) << "|" ;
                cout <<pfWC->Align::alignCenter(12, PQgetvalue(pfWC->BaseClassInherit::res, pfWC->BaseClassInherit::loopCounter, _previousm2kwh) ) << "|" ;
                cout <<pfWC->Align::alignCenter(13, PQgetvalue(pfWC->BaseClassInherit::res, pfWC->BaseClassInherit::loopCounter, _m2Usage) ) << "|" ;
                cout <<pfWC->Align::alignCenter(14, PQgetvalue(pfWC->BaseClassInherit::res, pfWC->BaseClassInherit::loopCounter, _AvgDailyUsage) ) << "|";
                cout << cases[pfWC->BaseClassInherit::debugFlags.intMyCase] << endl;
            }
            switch (pfWC->BaseClassInherit::debugFlags.mycase) {
                case M1M2KWH:
                    pfWC->BaseClassInherit::eu = PQgetvalue(pfWC->BaseClassInherit::res, pfWC->BaseClassInherit::loopCounter, _AvgDailyUsage);
//                     ptrAWV->energyUsageVector[pfWC->BaseClassInherit::grc] = atof(pfWC->BaseClassInherit::eu);
                    pfWC->AllocateWorkingVectors::energyUsageVector[pfWC->BaseClassInherit::grc]  = atof(pfWC->BaseClassInherit::eu);
                    break;
                case M1KWH:
                    pfWC->BaseClassInherit::eu = PQgetvalue(pfWC->BaseClassInherit::res, pfWC->BaseClassInherit::loopCounter, _m1Usage);
//                     ptrAWV->energyUsageVector[pfWC->BaseClassInherit::grc]  = atof(pfWC->BaseClassInherit::eu);
                    pfWC->AllocateWorkingVectors::energyUsageVector[pfWC->BaseClassInherit::grc]  = atof(pfWC->BaseClassInherit::eu);
                    break;
                case M2KWH:
                    pfWC->BaseClassInherit::eu = PQgetvalue(pfWC->BaseClassInherit::res, pfWC->BaseClassInherit::loopCounter, _m2Usage);
//                    ptrAWV->energyUsageVector[pfWC->BaseClassInherit::grc]  = atof(pfWC->BaseClassInherit::eu);
                    pfWC->AllocateWorkingVectors::energyUsageVector[pfWC->BaseClassInherit::grc]  = atof(pfWC->BaseClassInherit::eu);
                    break;
                default:
                    cerr << "Line " << __LINE__ << "File " << __FILE__ << "It seems like we picked up an undefined value for pfWC->BaseClassInherit::debugFlags.mycase: " << pfWC->BaseClassInherit::debugFlags.mycase << "\t We're terminating." << cases[pfWC->BaseClassInherit::debugFlags.intMyCase] << endl;
                    exit (BADMYCASEVALUE);
            }
            pfWC->BaseClassInherit::temp = PQgetvalue(pfWC->BaseClassInherit::res, pfWC->BaseClassInherit::loopCounter, _currentavtempf);
            if(strlen(pfWC->BaseClassInherit::temp) < 1) {
                continue; // skip this row if nothing returned because COPY doesn't like nothing there
            } else {
//                 ptrAWV->temperatureVector[pfWC->BaseClassInherit::grc]  = atof(pfWC->BaseClassInherit::temp);
                pfWC->AllocateWorkingVectors::temperatureVector[pfWC->BaseClassInherit::grc]  = atof(pfWC->BaseClassInherit::temp);
            }
            strcpy(pfWC->BaseClassInherit::copyBuffer + pfWC->BaseClassInherit::bufferOffset, pfWC->BaseClassInherit::temp );
            pfWC->BaseClassInherit::bufferOffset += strlen(pfWC->BaseClassInherit::temp);
            *(pfWC->BaseClassInherit::copyBuffer+pfWC->BaseClassInherit::bufferOffset) = ','; //insert a comma to \
            separate the fields.
            pfWC->BaseClassInherit::bufferOffset++;     //Accout for the comma we just added
            *(pfWC->BaseClassInherit::copyBuffer + pfWC->BaseClassInherit::bufferOffset) = ' '; //insert a comma to \
            separate the fields.
//
            pfWC->BaseClassInherit::bufferOffset++; //strlen(","); //account for the space character  we added, above.
            strcpy(pfWC->BaseClassInherit::copyBuffer + pfWC->BaseClassInherit::bufferOffset, pfWC->BaseClassInherit::eu);  //pfWC->BaseClassInherit::eu is different for the m1m2kwh case than for the m1kwh and m2kwh cases. Process differently
            pfWC->BaseClassInherit::bufferOffset += strlen(pfWC->BaseClassInherit::eu); //Account for the eu data we just added
            *(pfWC->BaseClassInherit::copyBuffer+pfWC->BaseClassInherit::bufferOffset) = '\n'; //insert a new line \
            character so postgres's COPY will understand that this is the end of the row.
//
            pfWC->BaseClassInherit::bufferOffset++; //strlen("\n"); account for the '\n' character
            *(pfWC->BaseClassInherit::copyBuffer + pfWC->BaseClassInherit::bufferOffset) = '\0'; //Terminate our \
            growing string. Do we really have to do this since we did a massive memset just after allocating\
            this dynamic instruction?\
            But don't update the pfWC->BaseClassInherit::bufferOffset to account for the above \
            NULL character. We'll just over-write it with the temp data if it's there.
//             ptrAWV->weightVector[pfWC->BaseClassInherit::grc] = atof(PQgetvalue(pfWC->BaseClassInherit::res, pfWC->BaseClassInherit::loopCounter, _dday));
            pfWC->AllocateWorkingVectors::weightVector[pfWC->BaseClassInherit::grc]  = atof(PQgetvalue(pfWC->BaseClassInherit::res, pfWC->BaseClassInherit::loopCounter, _dday));
            pfWC->BaseClassInherit::grc++; //increment the good row count
        } catch (const std::exception &e) {
            std::cerr << "On row: " << pfWC->BaseClassInherit::grc << ", we encountered some kind of conversion error, like trying to convert a null to an int or a float. [["<< e.what() << "]] Skipping this row, however processing continues." << cases[pfWC->BaseClassInherit::debugFlags.intMyCase] << std::endl;
            ++pfWC->BaseClassInherit::brc; //increment the bad row count.
        }
        ++pfWC->BaseClassInherit::trc_t; //Increment the total row count, includes valid and invalid rows.
        
    }  //End of for loop.. No additional memory leaks here
    strcat(pfWC->BaseClassInherit::copyBuffer , "\\." );   //The COPY command likes to see `\.` as the penultimate line.
    pfWC->BaseClassInherit::bufferOffset += 2;
    *(pfWC->BaseClassInherit::copyBuffer+pfWC->BaseClassInherit::bufferOffset) = '\n';   //The COPY command likes to see `\n` as the final line.
    pfWC->BaseClassInherit::bufferOffset++;
    *(pfWC->BaseClassInherit::copyBuffer+pfWC->BaseClassInherit::bufferOffset) = '\0';
    if (pfWC->BaseClassInherit::debugFlags.debug2) cout << "copyBuffer Looks Like: \n" << pfWC->BaseClassInherit::copyBuffer << endl;
    if (pfWC->BaseClassInherit::debugFlags.debug2) std::cout << "\nNumber of VALID Returned Rows: " << pfWC->BaseClassInherit::grc << ".\nNumber of INVALID Rows: " << pfWC->BaseClassInherit::brc << ".\nTotal Number of rows processed: " << pfWC->BaseClassInherit::grc + pfWC->BaseClassInherit::brc << ".\nNumber of unprocessed rows: " << pfWC->BaseClassInherit::numberOfReturnedRows-pfWC->BaseClassInherit::trc_t << "\n length of pfWC->BaseClassInherit::copyBuffer " << strlen(pfWC->BaseClassInherit::copyBuffer) << " bytes." << cases[pfWC->BaseClassInherit::debugFlags.intMyCase] << std::endl;
    
    if (pfWC->BaseClassInherit::debugFlags.debug2)  std::cout << "Successfully performed SELECT operation on all data.\nNow let's look at what we've put in the temperatureVector and the energyUsageVector." << cases[pfWC->BaseClassInherit::debugFlags.intMyCase] << std::endl;
    
    
    if (pfWC->BaseClassInherit::debugFlags.debug2 ) {
        for (pfWC->BaseClassInherit::k=0; pfWC->BaseClassInherit::k < pfWC->BaseClassInherit::numberOfReturnedRows - pfWC->BaseClassInherit::brc; pfWC->BaseClassInherit::k++) {
            
            std::cout << pfWC->BaseClassInherit::k << ". " <<  pfWC->AllocateWorkingVectors::temperatureVector[pfWC->BaseClassInherit::k] << ", " <<  pfWC->AllocateWorkingVectors::energyUsageVector[pfWC->BaseClassInherit::k] << cases[pfWC->BaseClassInherit::debugFlags.intMyCase] << std::endl;
        }
    }
    //Take a character --  as in pfWC->BaseClassInherit::debugFlags.mycase -- and turn \
    it into an integer, so arrays can use it as an index, by subtracting the character '0'
    pfWC->BaseClassInherit::k = (int)(pfWC->BaseClassInherit::debugFlags.mycase - '0');
    pfWC->BaseClassInherit::theTempFile = myTempFiles[pfWC->BaseClassInherit::k];

    pfWC->BaseClassInherit::myTempFile.open(pfWC->BaseClassInherit::theTempFile, ios_base::out);   //2018-06-26T05:45:48: Never had to do std::ios_base::out
    pfWC->BaseClassInherit::myTempFile  << pfWC->BaseClassInherit::copyBuffer  << endl;
    pfWC->BaseClassInherit::myTempFile.close();  //Close the output file
    
    pfWC->BaseClassInherit::allocateArrays(pfWC->BaseClassInherit::grc);
    for(pfWC->BaseClassInherit::loopCounter = 0; pfWC->BaseClassInherit::loopCounter<pfWC->BaseClassInherit::grc; pfWC->BaseClassInherit::loopCounter++ )  {
        pfWC->BaseClassInherit::setArray(IV, pfWC->BaseClassInherit::loopCounter,  \
            pfWC->AllocateWorkingVectors::temperatureVector[ pfWC->BaseClassInherit::loopCounter] ); \
        //Place the independent variable, Average Daily Temperature, into its designated array.
        pfWC->BaseClassInherit::setArray(ODVA, pfWC->BaseClassInherit::loopCounter, \
            pfWC->AllocateWorkingVectors::energyUsageVector[ pfWC->BaseClassInherit::loopCounter] ); //Place the \
        observed dependent variable (EnergyUsage as Read (Observed) from the electric meters into its array.

    }

    //WE will  now close the portal and end the transaction because I don't think we can do a create table on the ptrC connection with a transaction in progress.
    PQclear(pfWC->BaseClassInherit::res); // 2018-06-29T19:35:35 Do we want to do this here and now?
    pfWC->BaseClassInherit::res = PQexec(pfWC->BaseClassInherit::conn,  "CLOSE myportal; END;");
    if (PQresultStatus(pfWC->BaseClassInherit::res) != PGRES_COMMAND_OK) {
        std::cerr << "Line " << __LINE__ << " of file " << __FILE__ << ": Failed to execute the SQL statement: " << newMotherOfAllSelectStatements[pfWC->BaseClassInherit::debugFlags.intMyCase] << ". Error cause:\n" <<
        PQerrorMessage(pfWC->BaseClassInherit::conn) << cases[pfWC->BaseClassInherit::debugFlags.intMyCase] << std::endl;
        exit_nicely(pfWC, EXECUTESQLFAILED, __LINE__);
        
    } else {
        PQclear(pfWC->BaseClassInherit::res);
    }
    


    //Create a temporary table named, tbl_temp_eux; it is here where we will mirror onto this temporary table \
    the data that went into the temperature vector and the energy usage vector, above.
    if (!pfWC->BaseClassInherit::debugFlags.tempTableCreated) {  //Have we created the temporary table?
//Decided do the DROP TABLE … IF EXISTS and CREATE TABLE … IF NOT EXISTS with one SQL statement.
        pfWC->BaseClassInherit::res = PQexec(pfWC->BaseClassInherit::conn, drop_tbl_temp_eu[pfWC->BaseClassInherit::debugFlags.intMyCase]);
        if (PQresultStatus(pfWC->BaseClassInherit::res) != PGRES_COMMAND_OK ) {
            cerr << drop_tbl_temp_eu[pfWC->BaseClassInherit::debugFlags.intMyCase] << " command failed " << cases[pfWC->BaseClassInherit::debugFlags.intMyCase] << endl;
            exit_nicely(pfWC, DROPTABLEFAILED, __LINE__);
        }
        PQclear(pfWC->BaseClassInherit::res);
        pfWC->BaseClassInherit::res = PQexec(pfWC->BaseClassInherit::conn, create_tbl_temp_eu[pfWC->BaseClassInherit::debugFlags.intMyCase] );
        if (PQresultStatus(pfWC->BaseClassInherit::res) != PGRES_COMMAND_OK ) {
            cerr << create_tbl_temp_eu[pfWC->BaseClassInherit::debugFlags.intMyCase] << " command failed " << cases[pfWC->BaseClassInherit::debugFlags.intMyCase] << endl;
            exit_nicely(pfWC, CREATETABLEFAILED, __LINE__);
        }
        PQclear(pfWC->BaseClassInherit::res);
        pfWC->BaseClassInherit::debugFlags.tempTableCreated = true;  //Indicate table created.
    }

    pfWC->BaseClassInherit::errmsgCOPY="";
    if (pfWC->BaseClassInherit::debugFlags.debug2 ) cout << "COPY SQL looks like: " << copy_file[pfWC->BaseClassInherit::debugFlags.intMyCase] << endl;
    pfWC->BaseClassInherit::resC = PQexec(pfWC->BaseClassInherit::conn,  copy_file[pfWC->BaseClassInherit::debugFlags.intMyCase] );  //Execute the SQL statement: COPY tbl_temp_eu (temp, eu) STDIN;
//    PQclear(pfWC->BaseClassInherit::resC);
    pfWC->BaseClassInherit::resultsPQputCopyData = PQputCopyData(pfWC->BaseClassInherit::conn, pfWC->BaseClassInherit::copyBuffer, (int)strlen(pfWC->BaseClassInherit::copyBuffer));
    pfWC->BaseClassInherit::resultsPQputCopyEnd = PQputCopyEnd(pfWC->BaseClassInherit::conn, pfWC->BaseClassInherit::errmsgCOPY);
//    int whichIsIt = PQresultStatus(pfWC->BaseClassInherit::res);
    if (pfWC->BaseClassInherit::debugFlags.debug2 )  cout << " resultsPQputCopyData: " << pfWC->BaseClassInherit::resultsPQputCopyData << "; resultsPQputCopyEnd: " << pfWC->BaseClassInherit::resultsPQputCopyEnd << cases[pfWC->BaseClassInherit::debugFlags.intMyCase] << endl;
    
    if (strlen(pfWC->BaseClassInherit::errmsgCOPY)) {
        cerr << __LINE__ << " of file " << __FILE__ << ": COPY operation failed with error message of-->" << pfWC->BaseClassInherit::errmsgCOPY << "<--" <<cases[pfWC->BaseClassInherit::debugFlags.intMyCase] << endl;
    } else {
        if (pfWC->BaseClassInherit::debugFlags.debug2 ) cout << "\tCOPY operation seems to have worked!\t" << cases[pfWC->BaseClassInherit::debugFlags.intMyCase] << endl;

    }
    PQclear(pfWC->BaseClassInherit::resC);
//END OF COPY OPERATION
    pfWC->BaseClassInherit::res = PQexec(pfWC->BaseClassInherit::conn,begin_Transaction);
    if (PQresultStatus(pfWC->BaseClassInherit::res) != PGRES_COMMAND_OK) {
        std::cout << "Failed to execute the SQL statement: " << begin_Transaction << ". Error cause:\n" << PQerrorMessage(pfWC->BaseClassInherit::conn) << std::endl;
        exit_nicely(pfWC, BEGINTRANSACTIONFAILED, __LINE__);
        
    }
    PQclear(pfWC->BaseClassInherit::res);
//    pfWC->BaseClassInherit::rc[SELECTFROMTABLE] = beginTransaction(pfWC, begin_Transaction);  //Start up a new transaction where we will
//    if (pfWC->BaseClassInherit::rc[SELECTFROMTABLE]) exit_nicely(pfWC, pfWC->BaseClassInherit::rc[SELECTFROMTABLE], __LINE__);
    
    pfWC->BaseClassInherit::res = PQexec(pfWC->BaseClassInherit::conn,theMotherOfAllUnionSelectStatements[pfWC->BaseClassInherit::debugFlags.intMyCase]  );
    if (PQresultStatus(pfWC->BaseClassInherit::res) != PGRES_COMMAND_OK) {
        std::cout << "Failed to execute the SQL statement: " << theMotherOfAllUnionSelectStatements[pfWC->BaseClassInherit::debugFlags.intMyCase] << ". Error cause:\n" <<
        PQerrorMessage(pfWC->BaseClassInherit::conn) << std::endl;
        exit_nicely(pfWC, EXECUTESQLFAILED, __LINE__);
    }
    PQclear(pfWC->BaseClassInherit::res);
    
//    pfWC->BaseClassInherit::rc[SELECTFROMTABLE] = executeSQL(pfWC, theMotherOfAllUnionSelectStatements[pfWC->BaseClassInherit::debugFlags.intMyCase] );   //
//    if (pfWC->BaseClassInherit::rc[SELECTFROMTABLE]) exit_nicely(pfWC, pfWC->BaseClassInherit::rc[SELECTFROMTABLE], __LINE__);
    
    pfWC->BaseClassInherit::res = PQexec(pfWC->BaseClassInherit::conn, fetchALLTransaction);
    if (PQresultStatus(pfWC->BaseClassInherit::res) != PGRES_TUPLES_OK) {
        std::cout << "Failed to execute the SQL statement: " << fetchALLTransaction << ". Error cause:\n" <<
        PQerrorMessage(pfWC->BaseClassInherit::conn) << std::endl;
        exit_nicely(pfWC, FETCHALLFAILED, __LINE__);
    } else {
        pfWC->BaseClassInherit::numberOfReturnedRows = PQntuples(pfWC->BaseClassInherit::res);
        pfWC->BaseClassInherit::numberOfReturnedColumns = PQnfields(pfWC->BaseClassInherit::res);
        if (pfWC->BaseClassInherit::debugFlags.debug2 ) cout << "TheMotherOfAllUnionSelectStatements returned "\
            << pfWC->BaseClassInherit::numberOfReturnedRows << " rows and " << \
            pfWC->BaseClassInherit::numberOfReturnedColumns << " columns." \
            << cases[pfWC->BaseClassInherit::debugFlags.intMyCase] <<  endl;
    }


    if (pfWC->BaseClassInherit::debugFlags.debug2 ) cout << "Now bucketize the data. Each bucket has a temperature range of " << BUCKETSIZE << "ºF " << " and we will get the average energy usage, in kwh, for that temperature range buffer along with the energy usage standard deviation. " << cases[pfWC->BaseClassInherit::debugFlags.intMyCase] << endl;
    pfWC->BaseClassInherit::sqlbuffer= new char[SIZEOFBUFFERCONTAININGMOTHEROFALLSELECTSTATEMNTS];   /* << =================================== Dynamically acquire sufficient storage for constructing, piece-by-piece, the Mother Of All Select Statements. */

    
    const char *headerEU[2] = {"  avgtemp | avg eu |      stddev       |  min  |  max  | count\n", \
                                 "----------+--------+-------------------+-------+-------+-------\n"};
    if (pfWC->BaseClassInherit::debugFlags.debug2 ) cout << *(headerEU +0) << *(headerEU + 1) << cases[pfWC->BaseClassInherit::debugFlags.intMyCase] << std::endl; //Output the headers.//
    
    

//    Align *const ptrAFF = new Align(ASSUMEDBUFFERSIZE);
//    ptrAFF->alignSetup( headerEU[1] ); \
    SHOULD we clean-up (delete the internally dynamically acquired storage), or do we simply reuse the\
    the previous align before we start another one. 
   pfWC->Align::alignSetup( headerEU[1] );
    pfWC->BaseClassInherit::loopCounter = 0; //Reset this.
    pfWC->BaseClassInherit::worthyOfPolynomialFitProcessing=0;
    pfWC->BaseClassInherit::countOfGoodEntries = 0;
    pfWC->BaseClassInherit::countOfBadEntries = 0;
    pfWC->BaseClassInherit::buckettemp = MINTEMPERATURE; //Start out at the minimum temperature.
    while ( (pfWC->BaseClassInherit::buckettemp <  MAXTEMPERATURE) && (pfWC->BaseClassInherit::loopCounter < pfWC->BaseClassInherit::numberOfReturnedRows) ) {  //Beginning of while statement.
        // See http://libpqxx.readthedocs.io/en/latest/a00004.html for explanation of how the next two lines work

//        if ( ( PQgetisnull(pfWC->BaseClassInherit::res, pfWC->BaseClassInherit::loopCounter, _countu) ) ) { // ¡¡ went to 2! here !! Weed-out any bad records that contain poisonous fields.
        pfWC->BaseClassInherit::workCharString = PQgetvalue(pfWC->BaseClassInherit::res, pfWC->BaseClassInherit::loopCounter, _countu);
        if ( atoi(pfWC->BaseClassInherit::workCharString) < POLYNOMIALDEGREE || strlen(pfWC->BaseClassInherit::workCharString) == 0 ) {
            ++pfWC->BaseClassInherit::zeroCounter;
            ++pfWC->BaseClassInherit::countOfBadEntries;
            ++pfWC->BaseClassInherit::loopCounter;
            continue; //Skip this record if any critical values come back as NULL or there are fewer than POLYNOMIALDEGREE of them. Go back to top of while loop.
        }
        try {
            if (pfWC->BaseClassInherit::debugFlags.debug2 ) {
//These used to be ptrAFF`s but we'll experiment with repurposing ptrAF to see if we can get away with using just one object rather than an obect per each header.
                std::cout <<pfWC->Align::alignCenter(0,  PQgetvalue(pfWC->BaseClassInherit::res, pfWC->BaseClassInherit::loopCounter, _avgtemp) ) << "|";
                std::cout <<pfWC->Align::alignCenter(1,  PQgetvalue(pfWC->BaseClassInherit::res, pfWC->BaseClassInherit::loopCounter, _avgeu) ) << "|";
                std::cout <<pfWC->Align::alignCenter(2,  PQgetvalue(pfWC->BaseClassInherit::res, pfWC->BaseClassInherit::loopCounter, _stddeveu) ) << "|";
                std::cout <<pfWC->Align::alignCenter(3,  PQgetvalue(pfWC->BaseClassInherit::res, pfWC->BaseClassInherit::loopCounter, _mineu) ) << "|";
                std::cout <<pfWC->Align::alignCenter(4,  PQgetvalue(pfWC->BaseClassInherit::res, pfWC->BaseClassInherit::loopCounter, _maxeu) ) << "|";
                std::cout <<pfWC->Align::alignCenter(5,  PQgetvalue(pfWC->BaseClassInherit::res, pfWC->BaseClassInherit::loopCounter, _countu) ) << "|" <<  cases[pfWC->BaseClassInherit::debugFlags.intMyCase] << endl;
            }
            if( (pfWC->BaseClassInherit::thisCount =atoi( PQgetvalue(pfWC->BaseClassInherit::res, pfWC->BaseClassInherit::loopCounter, _countu) ) ) > POLYNOMIALDEGREE ) {
                pfWC->BaseClassInherit::values[_avgtemp] = atof(PQgetvalue(pfWC->BaseClassInherit::res, pfWC->BaseClassInherit::loopCounter, _avgtemp) );
                pfWC->BaseClassInherit::values[_avgeu]= atof(PQgetvalue(pfWC->BaseClassInherit::res, pfWC->BaseClassInherit::loopCounter, _avgeu));
                pfWC->BaseClassInherit::values[_stddeveu] =  atof( PQgetvalue(pfWC->BaseClassInherit::res, pfWC->BaseClassInherit::loopCounter, _stddeveu) );
                pfWC->BaseClassInherit::values[_mineu] = atof(PQgetvalue(pfWC->BaseClassInherit::res, pfWC->BaseClassInherit::loopCounter, _mineu));
                pfWC->BaseClassInherit::values[_maxeu] = atof(PQgetvalue(pfWC->BaseClassInherit::res, pfWC->BaseClassInherit::loopCounter, _maxeu) );
                pfWC->BaseClassInherit::thisCount = atoi(PQgetvalue(pfWC->BaseClassInherit::res, pfWC->BaseClassInherit::loopCounter, _countu));
                ++pfWC->BaseClassInherit::worthyOfPolynomialFitProcessing;
                ++pfWC->BaseClassInherit::countOfGoodEntries;
//INSERT the values we extracted into table tbl_bucketized_by_tempf_ranges. Consider NOW THAT WE HAVE COPY WORKING, PERHAPS WE CAN USE COPY RATHER THAN INSERT INTO … BLAH, BLAH …
                pfWC->BaseClassInherit::rc[SELECTFROMTABLE] = insertBucketizedEnergyValuesIntoTable( pfWC->BaseClassInherit::values[_avgtemp], pfWC->BaseClassInherit::values[_avgeu], pfWC->BaseClassInherit::values[_stddeveu],  pfWC->BaseClassInherit::values[_mineu], pfWC->BaseClassInherit::values[_maxeu], pfWC->BaseClassInherit::thisCount, pfWC);
                if (pfWC->BaseClassInherit::rc[SELECTFROMTABLE]) {
                    cerr << "Operation to INSERT INTO tbl_bucketized_by_tempf_ranges  F A I L E D!" << cases[pfWC->BaseClassInherit::debugFlags.intMyCase] << std::endl;
                    exit_nicely(pfWC, pfWC->BaseClassInherit::rc[SELECTFROMTABLE], __LINE__);
                }
            } else { //Come here if fewer than POLYNOMIAL DEGREE counts
                ++pfWC->BaseClassInherit::loopCounter;
                continue; //skip this record because it's no good, not enough data for a standard deviation. Go on to do the next record.
            }
        } catch (const std::exception &e) {
            ++pfWC->BaseClassInherit::countOfBadEntries;
            std::cerr << __FILE__ "2. bad values try/catch detected errors for entry number " << pfWC->BaseClassInherit::debugFlags.countOfBadEntries + pfWC->BaseClassInherit::countOfGoodEntries << cases[pfWC->BaseClassInherit::debugFlags.intMyCase] << std::endl;
        }  //end of try/catch block
        
        pfWC->BaseClassInherit::buckettemp += BUCKETSIZE;  //Go for the next bucket.
        ++pfWC->BaseClassInherit::loopCounter;
    }  //End of while loop
//    pfWC->BaseClassInherit::rc[SELECTFROMTABLE] = closePortal(pfWC, close_Transaction);  //Close myportal before ending the transaction
    PQclear(pfWC->BaseClassInherit::res);  //Don't do this until you've extracted all the data obtained by the above fetchAllTransaction
    pfWC->BaseClassInherit::res = PQexec(pfWC->BaseClassInherit::conn, close_Transaction);
    PQclear(pfWC->BaseClassInherit::res);
    pfWC->BaseClassInherit::res = PQexec(pfWC->BaseClassInherit::conn, end_Transaction);
    PQclear(pfWC->BaseClassInherit::res);
    
//    pfWC->BaseClassInherit::rc[SELECTFROMTABLE] = endTransaction(pfWC, end_Transaction);  //Close myportal before ending the transaction
// =================================================================
    
    if (pfWC->BaseClassInherit::worthyOfPolynomialFitProcessing > POLYNOMIALDEGREE) {


        try {  //Attempt to read back the data we just inserted into table tbl_bucketized_by_tempf_ranges
//            pfWC->BaseClassInherit::rc[SELECTFROMTABLE] = beginTransaction(pfWC, begin_Transaction);
//            if (pfWC->BaseClassInherit::rc[SELECTFROMTABLE]) exit_nicely(pfWC, pfWC->BaseClassInherit::rc[SELECTFROMTABLE], __LINE__);
            pfWC->BaseClassInherit::res = PQexec(pfWC->BaseClassInherit::conn, begin_Transaction);
            if (PQresultStatus(pfWC->BaseClassInherit::res) != PGRES_COMMAND_OK) {
                std::cout << "Failed to execute the SQL statement: " << begin_Transaction << ". Error cause:\n" <<
                PQerrorMessage(pfWC->BaseClassInherit::conn) << std::endl;
                exit_nicely(pfWC, BEGINTRANSACTIONFAILED, __LINE__);
                
            }
            PQclear(pfWC->BaseClassInherit::res);
            pfWC->BaseClassInherit::res = PQexec(pfWC->BaseClassInherit::conn, readBackTable[pfWC->BaseClassInherit::debugFlags.intMyCase]);
            if (PQresultStatus(pfWC->BaseClassInherit::res) != PGRES_COMMAND_OK) {
                std::cout << "Failed to execute the SQL statement: " << readBackTable[pfWC->BaseClassInherit::debugFlags.intMyCase] << ". Error cause:\n" <<
                PQerrorMessage(pfWC->BaseClassInherit::conn) << std::endl;
                exit_nicely(pfWC, EXECUTESQLFAILED, __LINE__);
                
            }
            PQclear(pfWC->BaseClassInherit::res);
            
//            pfWC->BaseClassInherit::rc[SELECTFROMTABLE] = executeSQL(pfWC, readBackTable[pfWC->BaseClassInherit::debugFlags.intMyCase]);  //We might want to order by temp.
//            pfWC->BaseClassInherit::res = PQexec(pfWC->BaseClassInherit::conn, readBackTable[pfWC->BaseClassInherit::debugFlags.intMyCase]);
            if (pfWC->BaseClassInherit::rc[SELECTFROMTABLE]) exit_nicely(pfWC, pfWC->BaseClassInherit::rc[SELECTFROMTABLE], __LINE__);
//            pfWC->BaseClassInherit::rc[SELECTFROMTABLE] = fetchAllTransaction(pfWC, fetchALLTransaction);
            pfWC->BaseClassInherit::res = PQexec(pfWC->BaseClassInherit::conn, fetchALLTransaction);
            if (PQresultStatus(pfWC->BaseClassInherit::res) != PGRES_TUPLES_OK) {
                std::cout << "Failed to execute the SQL statement: " << fetchALLTransaction << ". Error cause:\n" <<
                PQerrorMessage(pfWC->BaseClassInherit::conn) << std::endl;
                exit_nicely(pfWC, FETCHALLFAILED, __LINE__);
                
            }

            if (pfWC->BaseClassInherit::rc[SELECTFROMTABLE]) {
                if (pfWC->BaseClassInherit::debugFlags.debug2 ) cout << "FETCH ALL for SELECT statement\n" << readBackTable[pfWC->BaseClassInherit::debugFlags.intMyCase] << "\nfailed with error: " << PQerrorMessage(pfWC->BaseClassInherit::conn) << cases[pfWC->BaseClassInherit::debugFlags.intMyCase] << endl;
            } else {
                pfWC->BaseClassInherit::numberOfReturnedRows = PQntuples(pfWC->BaseClassInherit::res);
                pfWC->BaseClassInherit::numberOfReturnedColumns = PQnfields(pfWC->BaseClassInherit::res);
            }

            if (pfWC->BaseClassInherit::numberOfReturnedRows) {
                if (pfWC->BaseClassInherit::numberOfReturnedRows != pfWC->BaseClassInherit::worthyOfPolynomialFitProcessing) {
                    std::cerr << "For some unknown reason, the numberOfReturnedRows: " << pfWC->BaseClassInherit::numberOfReturnedRows << " AND values worthyOfPolynomialFitProcessing: " << pfWC->BaseClassInherit::worthyOfPolynomialFitProcessing << " have different values. " << cases[pfWC->BaseClassInherit::debugFlags.intMyCase] << std::endl;
                }

  //            SetupForMultiFitInherit *const smf = pfWC->SetupForMultiFitInherit::self;  //Using a constant\
                pointer, Establish unique addressability to the SetupForMultiFitInherit member of the Class \
                hierarchy. YOU PROBABLY DON't NEED TO DO THIS ANY MORE. USE pfWC with the \
                SetupForMultiFitInherit:: (scope resolution operator). VVVVVVVVVVVVVVVVVVVVVVV SomeWhere Here is where the number of memory leaks goes to 7!
                pfWC->SetupForMultiFitInherit::setNumberOfEntries(pfWC->BaseClassInherit::worthyOfPolynomialFitProcessing);  // ==> Right here we need to set the number of worth entries into SetupForMultiFitInherit    <=====
                pfWC->SetupForMultiFitInherit::setPtrToArrayOfValues(pfWC->BaseClassInherit::ptrValues);
                for  (pfWC->BaseClassInherit::loopCounter = 0, pfWC->BaseClassInherit::rowBeingProcessed = 0; pfWC->BaseClassInherit::loopCounter < pfWC->BaseClassInherit::worthyOfPolynomialFitProcessing; ++pfWC->BaseClassInherit::loopCounter, ++pfWC->BaseClassInherit::rowBeingProcessed)
                { //Begin loading the gsl matrices and vectors with values.
                    pfWC->BaseClassInherit::values[_avgtemp] =atof(PQgetvalue(pfWC->BaseClassInherit::res, pfWC->BaseClassInherit::rowBeingProcessed, _avgtemp) ) ;   //Considered the independent Variable
                    pfWC->BaseClassInherit::values[_avgeu]= atof(PQgetvalue(pfWC->BaseClassInherit::res, pfWC->BaseClassInherit::rowBeingProcessed, _avgeu));        //Considered the dependent Variable
                    pfWC->BaseClassInherit::values[_stddeveu] = atof( PQgetvalue(pfWC->BaseClassInherit::res, pfWC->BaseClassInherit::rowBeingProcessed, _stddeveu) ); //Considered the weight.
                    pfWC->BaseClassInherit::values[_mineu] = atof(PQgetvalue(pfWC->BaseClassInherit::res, pfWC->BaseClassInherit::rowBeingProcessed, _mineu));
                    pfWC->BaseClassInherit::values[_maxeu] =atof(PQgetvalue(pfWC->BaseClassInherit::res, pfWC->BaseClassInherit::rowBeingProcessed, _maxeu) );
                    pfWC->BaseClassInherit::thisCount = atoi(PQgetvalue(pfWC->BaseClassInherit::res, pfWC->BaseClassInherit::rowBeingProcessed, _countu));

                    pfWC->SetupForMultiFitInherit::captureIndependentVariableValues( pfWC->BaseClassInherit::rowBeingProcessed, pfWC->BaseClassInherit::values );    //Now, put into the object the data that will go into the independent variable matrix (_avgtemp), the dependent variable vector (_avgeu), and the weighting
                    pfWC->SetupForMultiFitInherit::setIntoVector( pfWC->SetupForMultiFitInherit::dependentVariable, pfWC->BaseClassInherit::rowBeingProcessed, *(pfWC->BaseClassInherit::values + _avgeu) ); //Put the depenedent variable into its vector.
                    pfWC->SetupForMultiFitInherit::setIntoVector( pfWC->SetupForMultiFitInherit::weights, pfWC->BaseClassInherit::rowBeingProcessed, 1/( *(pfWC->BaseClassInherit::values + _stddeveu) * *(pfWC->BaseClassInherit::values + _stddeveu)  ) ); //Put the weight (stddev) into it's vector. See top 0f page 451 of version 1.12 of GNU Scientific Library Reference Manual (3rd Edition) for the `1/( *(values + _stddeveu) * *(values + _stddeveu)  )` piece of the processing

                } //End of for loop
                PQclear(pfWC->BaseClassInherit::res); //Hold-off doing this until after we've extracted all the \
                data the FETCH ALL provided.
                pfWC->BaseClassInherit::res = PQexec(pfWC->BaseClassInherit::conn, "Close myportal");
                PQclear(pfWC->BaseClassInherit::res);
                pfWC->BaseClassInherit::res = PQexec(pfWC->BaseClassInherit::conn, "END");
                PQclear(pfWC->BaseClassInherit::res);
                if(pfWC->BaseClassInherit::debugFlags.debug2) std::cout << "Line " << __LINE__ << "\tJust Finished Closing myportal and then Ending the Transaction, doing `PQclears` after each `PQexec`." << std::endl;
                pfWC->BaseClassInherit::rc[SELECTFROMTABLE] =  pfWC->SetupForMultiFitInherit::doMultiFit( ); //Actually go out and do the gsl_multifit
                pfWC->BaseClassInherit::rc[SELECTFROMTABLE] =  pfWC->TblPolyFitWithCovInherit::outputPolynomial( "EnergyUsage(T) = ",  cases[pfWC->BaseClassInherit::debugFlags.intMyCase]);
                pfWC->BaseClassInherit::ptrFloatCovarianceMatrix = pfWC->TblPolyFitWithCovInherit::copyOfCovarianceMatrix; //Establish addressability to our copy.
                //   UNUSED    VaryingType<float> *tfloat =new VaryingType<float>();  //This tfloat object, for type double, should serve us for making input parameters p2, p3, p4, p5, p7, and p8 in network byte order.

                 pfWC->SetupForMultiFitInherit::outputCovarianceMatrixAndReturnChi2( "cov = [", cases[pfWC->BaseClassInherit::debugFlags.intMyCase] );
                std::string strD = to_string<double>( pfWC->SetupForMultiFitInherit::computeTrace( pfWC->SetupForMultiFitInherit::covarienceMatrix->data, 1+POLYNOMIALDEGREE));

                const std::string tOCM("☜ Trace of Covariance Matrix");
                const std::string newLine("\n");
                pfWC->SetupForMultiFitInherit::stringStreamForOutput <<  pfWC->SetupForMultiFitInherit::computeCorrelationBetweenIndependentAndDependentVariables( )  << "☜ correlation coefficient" << cases[pfWC->BaseClassInherit::debugFlags.intMyCase] << newLine;
                pfWC->SetupForMultiFitInherit::stringStreamForOutput << strD  << tOCM << cases[pfWC->BaseClassInherit::debugFlags.intMyCase]  << newLine;
                pfWC->SetupForMultiFitInherit::stringStreamForOutput <<  pfWC->SetupForMultiFitInherit::computeCorrelationBetweenIndependentAndDependentVariables( ) << "☜ correlation coefficient" << cases[pfWC->BaseClassInherit::debugFlags.intMyCase] << "\n";
                pfWC->BaseClassInherit::squareRootOfSumOfTheSquaresOfTheVarience =  pfWC->SetupForMultiFitInherit::computeGoodnessOfResults( );
                pfWC->BaseClassInherit::averageVariencePerValidReading = pfWC->BaseClassInherit::squareRootOfSumOfTheSquaresOfTheVarience/pfWC->BaseClassInherit::grc;
                
                 pfWC->SetupForMultiFitInherit::stringStreamForOutput << pfWC->BaseClassInherit::squareRootOfSumOfTheSquaresOfTheVarience << "☜ Square Root Of Sum Of TheSquares Of The Varience; \n" << pfWC->BaseClassInherit::averageVariencePerValidReading << "☜ Average Varience Per Valid Reading\n" << pfWC->BaseClassInherit::averageVariencePerValidReading*100 << "☜ \% Average Varience Per Valid Reading\n"<< pfWC->BaseClassInherit::grc << "☜ Number of valid meter readings (# observed dependent variables)" <<  cases[pfWC->BaseClassInherit::debugFlags.intMyCase] << newLine;

            } else {
                std::cerr << "Although the SELECT seems to have worked, we got " << pfWC->BaseClassInherit::numberOfReturnedRows << " rows back from the table which we thought we just inserted data into!" << cases[pfWC->BaseClassInherit::debugFlags.intMyCase] << std::endl;
            }
        } catch (const std::exception &e) {
            std::cerr << "Some how we failed to get any data back from the table which we thought we just inserted data into!" << cases[pfWC->BaseClassInherit::debugFlags.intMyCase] << std::endl;
        }
 
        
        //        rc = polynomialFit(POLYNOMIALDEGREE, grc,  pfWC->BaseClassInherit::ptrAWV->temperatureVector,  pfWC->BaseClassInherit::ptrAWV->energyUsageVector,  NULL, NULL,  &TSS, &goodnessOfFit);
    } else {
        std::cerr << "Insufficient number of VALID datapoints to do MultiFit processing for desired polynomial of degree " << POLYNOMIALDEGREE << cases[pfWC->BaseClassInherit::debugFlags.intMyCase] << std::endl;
    }



    for (pfWC->BaseClassInherit::row=0; pfWC->BaseClassInherit::row < 1+POLYNOMIALDEGREE; pfWC->BaseClassInherit::row++) {  //Get the coefficients
        *(pfWC->TblPolyFitWithCovInherit::copyOfCoefficients +pfWC->BaseClassInherit::row) = pfWC->getCoefficient(pfWC->BaseClassInherit::row); //One coefficient at a time.
    }
//We expect this piece of dynamically acquired storage to be deleted by member function\
    TblPolyFitWithCovInherit::doInsertInto
    pfWC->TblPolyFitWithCovInherit::ufloat.ptrToCovarianceMatrix = pfWC->BaseClassInherit::ptrFloatCovarianceMatrix; // WE USED TO DO THIS:    pfWC->TblPolyFitWithCovInherit::ufloat.ptrToCovarianceMatrix = new float(NUMBEROFCOVARIANCEMATRIXENTRIES );
//    pfWC->BaseClassInherit::row = 0;
//    pfWC->BaseClassInherit::column = 0;
/*
    for (pfWC->BaseClassInherit::row=0; pfWC->BaseClassInherit::row < 1+POLYNOMIALDEGREE; pfWC->BaseClassInherit::row++) {
        for (pfWC->BaseClassInherit::column=0; pfWC->BaseClassInherit::column < 1+POLYNOMIALDEGREE; pfWC->BaseClassInherit::column++) {

//                         A                                                            B                                   C                                          D                                          E                               F
            *(pfWC->TblPolyFitWithCovInherit::ufloat.ptrToCovarianceMatrix+ pfWC->BaseClassInherit::row * sizeof(float) + pfWC->BaseClassInherit::column) = *(pfWC->SetupForMultiFitInherit::ptrCovarianceMatrixArray + pfWC->BaseClassInherit::row * sizeof(float) + pfWC->BaseClassInherit::column); //Move the covariance data over to the TblPolyFitWithCovInherit Class
        }
    }
*/
    pfWC->TblPolyFitWithCovInherit::correlationCoefficient = pfWC->SetupForMultiFitInherit::correlationCoefficient;   //Make our own copy.
    pfWC->TblPolyFitWithCovInherit::chi2 = pfWC->SetupForMultiFitInherit::chiSquared;
    if(pfWC->BaseClassInherit::debugFlags.debug2) std::cout << "Line " << __LINE__ << "About to do TblPolyFitWithCovInherit::doInsertInto" << std::endl;
    pfWC->BaseClassInherit::rc[SELECTFROMTABLE] = pfWC->TblPolyFitWithCovInherit::doInsertInto( ); //Ok, insert the statistics data into tbl_poly_fit_with_cov
    if(pfWC->BaseClassInherit::rc[SELECTFROMTABLE]) {
        std::cout << pfWC->BaseClassInherit::ptrGTOD->doTimePoint()  <<":\tSome kind of problem in TblPolyFit::doInsertInto, got a return code of " << pfWC->BaseClassInherit::rc[SELECTFROMTABLE] << std::endl;
    } else {
        std::cout << pfWC->BaseClassInherit::ptrGTOD->doTimePoint()  <<":\tSuccessful call to TblPolyFit::doInsertInto, got a return code of " << pfWC->BaseClassInherit::rc[SELECTFROMTABLE] << std::endl;
    }

//Now delete all the dynamic storage we acquired in selectFromTable.cpp
    pfWC->BaseClassInherit::deAllocateArrays();
    if (pfWC->BaseClassInherit::sql != nullptr) {
        try {
            delete [] pfWC->BaseClassInherit::sql;
        } catch ( ... ) {
            std::cout << "Itercepted an abnormal termination when trying to execute  delete [] pfWC->BaseClassInherit::sql; on line " << __LINE__-2 << std::endl;
        }
        pfWC->BaseClassInherit::sql = nullptr;
    }
    if (pfWC->BaseClassInherit::copyBuffer != nullptr) {
        try {
            delete [] pfWC->BaseClassInherit::copyBuffer;
        } catch ( ... ) {
            std::cout << "Itercepted an abnormal termination when trying to execute delete [] pfWC->BaseClassInherit::copyBuffer on line " << __LINE__-2 << std::endl;
        }
        pfWC->BaseClassInherit::copyBuffer = nullptr;
    }
    if (pfWC->BaseClassInherit::subCharBuffer != nullptr) {
        try {
            delete [] (pfWC->BaseClassInherit::subCharBuffer);   //c++'s Equivalent to C's free when we allocate storage with a calloc/malloc. Note the use of [] prior to the (subCharacterBuffer);
        } catch ( ... ) {
            std::cout << "Itercepted an abnormal termination when trying to execute delete [] (pfWC->BaseClassInherit::subCharBuffer) on line " << __LINE__-2 << std::endl;
        }

        pfWC->BaseClassInherit::subCharBuffer = nullptr; //Mark as deleted
    }
    if (pfWC->BaseClassInherit::sqlbuffer != nullptr) {
        try {
            delete[](pfWC->BaseClassInherit::sqlbuffer);       //Delete another dynamically acquired character buffer.
        } catch ( ... ) {
            std::cout << "Itercepted an abnormal termination when trying to execute delete[](pfWC->BaseClassInherit::sqlbuffer); on line " << __LINE__-2 << std::endl;
        }
        pfWC->BaseClassInherit::sqlbuffer= nullptr; //Mark as deleted
    }
    /* 2018-07-15T12:04:44 We no longer dynamically allocate this memory
    if (pfWC->BaseClassInherit::ptrFloatCovarianceMatrix != nullptr ) {
        try {
            delete [] (pfWC->BaseClassInherit::ptrFloatCovarianceMatrix);
        } catch ( ... ) {
            std::cout << "Itercepted an abnormal termination when trying to execute delete [] (pfWC->BaseClassInherit::floatCovarianceMatrix) on line " << __LINE__-2 << std::endl;
        }
        pfWC->BaseClassInherit::ptrFloatCovarianceMatrix = nullptr; //Mark as deleted
    }
    */
/*
 Don't delete these here because these objects were instantiated in the main.cpp program.
    if (ptrAF != nullptr) delete ptrAF;
    if (ptrAFF != nullptr) delete ptrAFF;

    if (ptrAWV != nullptr) delete ptrAWV; //Should this class, along with BaseClassInherit, be inherited into\
 
    SetupForMultiFit?
*/
    if (pfWC->BaseClassInherit::selectFromTableDoesPQFinish)
        try {
            PQfinish(pfWC->BaseClassInherit::conn);  //Since this routine did the connection, this  routine must do the PQfinish to disconnect from the database.
 //           pfWC->BaseClassInherit::connectionResponsibilityCount--;
        } catch   (const std::exception &e) {
            std::cout << "Line: " << __LINE__ << ", within file " << __FILE__ << ", Got an exception when attempting to \ndo a PQfinish on database connection. Exception looksl like:\n"<< e.what() << std::endl;
        }
    pfWC->BaseClassInherit::connectionResponsibilityCount--; 
    return myRC;
}
