//
//  cmdLineArgs.hpp
//  multiThreadEnergyUsage
//
//  Created by Clifford Campo on 6/5/18.
//  Copyright © 2018 CliffordCampo. All rights reserved.
//

#ifndef cmdLineArgs_hpp
#define cmdLineArgs_hpp

#include <iostream>
#include "myPrototypes.hpp"

class CmdLineArgs {
private:
    const char *defaultDatabaseName = "LocalWeather";
    const char *defaultUserID = "cjc";
    const char *defaultHostID = "localhost";
    const char *defaultPortID = "5436";
    const char *defaultResultsFileName = "/Users/cjc/energyUsage/eu";
    bool multiThread = false;               //If true we are running as a multi-thread job. Set by constructor.
    unsigned short numberOfPossibleCommandLineArguments;
public:
    const char *_clDatabaseName;
    const char *_results_File;
    const char *_clUserID;
    const char *_clPW;
    const char *_clHostID;
    const char *_clPortID;
    const char *_clSQL;
    short d4;
    bool _useKelvin;
    bool _debug1;
    bool _debug2;
    bool _debug3;
    bool _debug4; /*Added on 2018-07-16T13:36:17. Controls recording of performance related data via ostringstream, a suspected source of memory leaks. */

    void threadSet(bool);
    bool threadGet(void);
    CmdLineArgs(bool, const unsigned short);
    ~CmdLineArgs();
    friend class BaseClassInherit; //Allow the class named BaseClassInherit to access this class's private member date
    
    
};
#endif /* cmdLineArgs_hpp */
