#include "main.h"


int main(int argc, char const *argv[]) {
    VaDriver driver;
    driver.start();

    driver.set_value(std::string("MyPV"), 5.645);

    std::cin.get();
    driver.stop();

    return 0;
}
