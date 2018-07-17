//
//  getTimeOfDay.cpp
//  getCurrentDateTime
//
//  Created by Clifford Campo on 7/4/18.
//  Copyright © 2018 CliffordCampo. All rights reserved.
//

#include "GetTimeOfDay.hpp"
void GetTimeOfDay::getTime(void) {
    GetTimeOfDay::now = std::chrono::system_clock::now();
    GetTimeOfDay::tp = GetTimeOfDay::now.time_since_epoch();
}
GetTimeOfDay::GetTimeOfDay() {
    
    //    typedef std::chrono::duration<int, ratio_multiply<hours::period, ratio<24> >::type> days;
    now = std::chrono::system_clock::now();
    tp = now.time_since_epoch();
    /*
     d = std::chrono::duration_cast<days>(tp);
     tp -= d;
     h = std::chrono::duration<long, ratio_multiply<hours::period, ratio<3600> >   >(tp);
     tp -= h;
     
     
     tp -= min;
     */
    GetTimeOfDay::seconds s = std::chrono::duration_cast<seconds>(tp);
    GetTimeOfDay::tp -= s;
    GetTimeOfDay::useCount = 0;
    GetTimeOfDay::ptrSE = &standardEpoch;
    GetTimeOfDay::ptrDec = &decade;
    GetTimeOfDay::ssb = new std::ostringstream;
}
GetTimeOfDay::~GetTimeOfDay() {
    delete GetTimeOfDay::ssb; //Delete string stream buffer that the constructor acquired.
}


//DEFAULT VALUES                   true           true              true              true
string GetTimeOfDay::doTimePoint(bool getTimeNow, bool localTime, bool subseconds, bool rewind) {
    //    time_t rawtime;
    //    struct tm * timeinfo;
    // Input parameters to doTimePoint are:
    // 1. if getTimeNow is true we will get the time-of-day now, otherwise we will use the time-of-day when \
       this class was instantiated. DEFAULT is true
    // 2. if localTime is true we will use local time, otherwise we will use gmt. DEFAULT is true.
    // 3. if subseconds is true we will supply in the output the sub-seconds with microsecond resolution.\
       otherwise we will not supply the sub-seconds in the output. DEFAULT is true.
    // 4. if rewind is true we will for each call after instatiation, rewind the ostringstream's pointer back\
       to the beginning (left-most) part of the ostringstream's buffer. Otherwise we will concatenate current\
       data immediately to the right of the previous values. DEFAULT is true.
    time (&rawtime);
    timeinfo = localtime (&rawtime);
    using namespace std;
    using namespace std::chrono;
    if (rewind && (this->GetTimeOfDay::useCount > 0) ) this->GetTimeOfDay::ssb->seekp(0);  //Go to the beginning (left-most) of the ostringstream buffer;
    GetTimeOfDay::tt = system_clock::to_time_t(now);
    if(getTimeNow) this->GetTimeOfDay::getTime();
    if (localTime) {
        the_tm = *localtime(&this->GetTimeOfDay::tt);
    } else {
        the_tm = *gmtime(&this->GetTimeOfDay::tt);
    }
    *this->GetTimeOfDay::ssb << this->GetTimeOfDay::the_tm.tm_year + *this->GetTimeOfDay::ptrSE << '-';
    if (this->GetTimeOfDay::the_tm.tm_mon+1 < *this->GetTimeOfDay::ptrDec) {
        *this->GetTimeOfDay::ssb << '0' << this->GetTimeOfDay::the_tm.tm_mon + 1 << '-';
    } else {
        *this->GetTimeOfDay::ssb << this->GetTimeOfDay::the_tm.tm_mon + 1 << '-';
    }
    if (this->GetTimeOfDay::the_tm.tm_mday < *this->GetTimeOfDay::ptrDec) {
        *this->GetTimeOfDay::ssb << '0' << this->GetTimeOfDay::the_tm.tm_mday << ' ';
    } else {
        *this->GetTimeOfDay::ssb <<  this->GetTimeOfDay::the_tm.tm_mday << ' ';
    }
    if (this->GetTimeOfDay::the_tm.tm_hour < *this->GetTimeOfDay::ptrDec) {
        *this->GetTimeOfDay::ssb << '0' << this->GetTimeOfDay::the_tm.tm_hour  << ':';
    } else {
        *this->GetTimeOfDay::ssb << this->GetTimeOfDay::the_tm.tm_hour << ':';
    }
    if (this->GetTimeOfDay::the_tm.tm_min < *ptrDec) {
        *this->GetTimeOfDay::ssb << '0' << this->GetTimeOfDay::the_tm.tm_min << ':';
    } else {
        *ssb << this->GetTimeOfDay::the_tm.tm_min << ':';
    }
    if (subseconds ) {
//Execute this if the caller wants subseconds.
    if (this->GetTimeOfDay::the_tm.tm_sec < *ptrDec) {
        *this->GetTimeOfDay::ssb << '0' << this->GetTimeOfDay::the_tm.tm_sec << '.' << (this->GetTimeOfDay::tp.count() % 1000000) ;
    } else {
        *this->GetTimeOfDay::ssb << this->GetTimeOfDay::the_tm.tm_sec << '.' << (this->GetTimeOfDay::tp.count() % 1000000);
    }
    } else {
//Execute this if the caller does not want subseconds.
        if (this->GetTimeOfDay::the_tm.tm_sec < *this->GetTimeOfDay::ptrDec) {
            *this->GetTimeOfDay::ssb << '0' << this->GetTimeOfDay::the_tm.tm_sec ;
        } else {
            *this->GetTimeOfDay::ssb << this->GetTimeOfDay::the_tm.tm_sec ;
        }
    }
    ++this->GetTimeOfDay::useCount; //Keep track of the number of times we've used `this` buffer
    return this->GetTimeOfDay::ssb->str();  //Convert ostringstream to string
}
