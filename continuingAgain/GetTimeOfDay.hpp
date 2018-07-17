//
//  getTimeOfDay.hpp
//  getCurrentDateTime
//
//  Created by Clifford Campo on 7/4/18.
//  Copyright © 2018 CliffordCampo. All rights reserved.
//

#ifndef GetTimeOfDay_hpp
#define GetTimeOfDay_hpp

#include <stdio.h>
#include <iostream>
#include <chrono>
#include <iomanip>
#include <string>
#include <ctime>
#include <sstream>
#include <time.h>
using namespace std;
using namespace std::chrono;
//#include "clock.hpp"
typedef struct std::tm t;


class GetTimeOfDay {
private:
    const size_t standardEpoch=1900;
    const size_t decade=10;
public:
    using days = duration<long, ratio_multiply<hours::period, ratio<24> >   > ;
    using hours = duration<long, ratio_multiply<hours::period, ratio<3600> >   >;
    using minutes = duration<long, ratio_multiply<hours::period, ratio<60> >   >;
    using seconds = duration<long, ratio_multiply<seconds::period, ratio<1> >  >;
    using micrs = duration< std::micro>;
    const size_t *ptrSE;
    const size_t *ptrDec;
    int useCount;
    time_t rawtime;
    struct tm * timeinfo;
    std::chrono::system_clock::time_point now;
    std::chrono::system_clock::duration tp;
    time_t tt;
    tm the_tm;
    std::ostringstream* ssb;
    double clockPrecision;
    bool isSteady;
    std::ostringstream myKindOfTime;
    GetTimeOfDay(void);
    ~GetTimeOfDay(void);
    void getTime(void);
    string doTimePoint(bool=true, bool=true, bool=true, bool=true);
    template <typename C>
    string clockAudit ( void ) {
        cout << "-precision: ";
        typedef typename C::period P;
        if(ratio_less_equal<P,milli>::value) {
            typedef typename ratio_multiply<P, kilo>::type TT;
            cout << fixed << double(TT::num)/TT::den << " milliseconds" << endl;
            clockPrecision = double(TT::num)/TT::den;
        } else {
            cout << fixed << double(P::num)/P::den << " seconds" << endl;
        }
        cout << "- is_steady: " << boolalpha << C::is_steady << endl;
        isSteady = C::is_steady;
    }


    
};

#endif /* GetTimeOfDay_hpp */
