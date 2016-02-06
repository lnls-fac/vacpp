%module vacpp

%{
    #include "common.h"
    #include "driver.h"
%}

%include "std_string.i"
%include "std_vector.i"


%include "common.h"
%include "driver.h"

namespace std {
    %template(CppPVValuePairVector) vector<PVValuePair>;
}
