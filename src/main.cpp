#include "main.h"


int main(int argc, char const *argv[]) {
    {
        VaDriver driver;
        driver.set_value(std::string("MyPV"), 5.645);
        std::cin.get();
        std::this_thread::sleep_for(std::chrono::seconds(1));
    }

    return 0;
}
