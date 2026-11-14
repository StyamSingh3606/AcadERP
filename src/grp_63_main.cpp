// group-63

#include <exception>
#include <iostream>

#include "grp_63_CommandParser.h"
#include "grp_63_Exceptions.h"

int main() {
    try {
        acaderp::CommandParser parser;
        parser.run(std::cin, std::cout, std::cerr);
        return 0;
    } catch (const acaderp::ErpException& error) {
        std::cerr << "FATAL " << error.code() << ": "
                  << error.what() << '\n';
        return 1;
    } catch (const std::exception& error) {
        std::cerr << "FATAL: " << error.what() << '\n';
        return 1;
    }
}
