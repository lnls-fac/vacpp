#include "main.h"
#include "driver.h"


int main(int argc, char const *argv[]) {
    std::cout << "This is a test" << std::endl;

    VaDriver driver;

    driver.write_pv(std::string("MyPV"), 5.645);
    driver.process_forever();

    return 0;
}
