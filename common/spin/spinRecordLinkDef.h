/** @file spinRecordLinkDef.h

    A linkdef file provided to the rootcint dictionary generator to
    support the spinRecord and associated classes.

    - Kevin Lynch 2009
*/

#pragma link off all globals;
#pragma link off all classes;
#pragma link off all functions;


#pragma link C++ class spinRecord+;

#include <vector>
#pragma extra_include "vector";
#pragma link C++ class vector<spinRecord>+;
