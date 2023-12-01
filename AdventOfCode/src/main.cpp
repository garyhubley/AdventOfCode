
#include "2021.h"

#include <iostream>

int main( int argc, char **argv ) {

    int       year;
    const int INDEX_CORRECTION = 1;

    std::cout << "Welcome to Gary's Advent of Code" << std::endl;
    std::cout << "Select a year: \n  2021\n  2023" << std::endl;
    std::cout << "** if year = 0, you will exit" << std::endl;

    std::cin >> year;

    switch( year ) {
        case 2021:
            main2021( argc, argv );
            break;
        case 2023:
            // main2023( argc, argv );
            break;
        default:
            std::cout << "Goodbye" << std::endl;
            break;
    }

    return 0;
}