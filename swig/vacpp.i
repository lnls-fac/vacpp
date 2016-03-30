%module vacpp

%{
    #include "driver.h"
%}

%include "std_string.i"
%include "std_vector.i"


%include "driver.h"

namespace std {
      %template(CppStringVector) vector<string>;
      %template(CppDoubleVector) vector<double>;
//    %template(CppPVValuePairVector) vector<PVValuePair>;
}
