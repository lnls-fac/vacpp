#include "main.h"


int main(int argc, char const *argv[]) {
    std::cout << "This is a test" << std::endl;

    // VaDriver driver;
    //
    // driver.write_pv(std::string("MyPV"), 5.645);
    // driver.process_forever();

    std::string flat_file_name("/home/fac_files/siriusdb/vacpp/si.txt");
    AcceleratorModel m(flat_file_name);

    m.process();

    return 0;
}
