#include "main.h"
#include "driver.h"


int main(int argc, char const *argv[]) {
    std::cout << "This is a test" << std::endl;

    VaDriver driver;
    driver.process_forever();

    return 0;
}
