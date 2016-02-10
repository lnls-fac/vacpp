#include "main.h"


int main(int argc, char const *argv[]) {
    VaDriver driver;
    driver.start();

    driver.set_value(std::string("MyPV"), 5.645);

    for (int i=0; i<10; ++i) {
        std::this_thread::sleep_for(std::chrono::milliseconds(100*i));
        auto n = driver.get_number_of_values_available();
        auto values = driver.get_values(n);
        print_pairs(values);
    }

    driver.stop();

    return 0;
}
