// Example of main source file
// Library Source file must have
// 1. Includes
// 2. Namespace use or not
// 3. Instances(objects) of class
// 4. Calling object methods(functions) and writing or reading attributtes(variables)

// Includes
#include "MyLibrary.hpp"

using namespace MyNameSpaceFirst;                                                                   // Namespace definition
using namespace MyNameSpaceFirst::MyNameSpaceSecond;                                                // Namespace definition

MyClassOne::MyStructOne InstanceStructOne;
MyClassOne::MyEnumOne InstanceEnumOne; 

int main()                                                                                          // Main function with arguments input option 
{                                                                                                   // THIS MEANS when running program ./main argument1 argument2
    MyClassOne MyInstanceClassOne;                                                                  // Create instance of class from the library
    MyInstanceClassOne.PrintFromFirstClass();                                                       // Call function of class form the library
    InstanceStructOne.MyIntInStructOne = 88;
    std::cout << "MyIntInStructOne = " <<InstanceStructOne.MyIntInStructOne<< std::endl;
    std::cout << "MyIntInStructTwo = " <<InstanceStructOne.MyIntInStructTwo<< std::endl;
    std::cout << "MyIntInStructThree = " <<InstanceStructOne.MyIntInStructThree<< std::endl;
    std::cout << "MyIntInStructFour = " <<InstanceStructOne.MyIntInStructFour<< std::endl;
    std::cout << "MyIntInStructFive = " <<InstanceStructOne.MyIntInStructFive<< std::endl;
    std::cout << "MyIntInStructSix = " <<InstanceStructOne.MyIntInStructSix<< std::endl;

    std::cout << "MyIntInStructSix = " <<InstanceEnumOne.MyEnumMemberOne<< std::endl;
    return 0;
}