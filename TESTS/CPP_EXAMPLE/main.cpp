/* 
TODO
+ Struct pointer passing to function
+ Makefile


*/
// Example of main source file
// Library Source file must have
// 1. Includes
// 2. Namespace use or not
// 3. Instances(objects) of classes
// 4. Calling object methods(functions) and writing or reading attributtes(variables)

// Includes
#include "MyLibrary.hpp"

using namespace MyNameSpaceFirst;                                                               // Namespace definition (OPTIONAL)
using namespace MyNameSpaceFirst::MyNameSpaceSecond;                                            // Namespace definition (OPTIONAL)
MyClassOne::MyStructOne InstanceStructOne;                                                      // Create instance of struct (utilizing namespace)
MyClassOne::MyEnumOne myenumone;
MyControlClass::ControlStruct controlstructmain;                                                // Create instance of struct (utilizing namespace)
MyControlClass::ControlEnum controlenummain;                                                    // Create instance of enum (utilizing namespace)

int main()                                                                                      // Main function definition without arguments ARGUMENTS ARE OPTIONAL
{                                                                                               // THIS MEANS when running program ./main argument1 argument2
    MyClassOne MyInstanceClassOne;                                                              // Create instance of class from the library
    MyInstanceClassOne.PrintFromFirstClass();                                                   // Call function of class form the library
    InstanceStructOne.MyIntInStructOne = 88;                                                    // Overwrite value for member of struct in class
    std::cout << "MyIntInStructOne = " <<InstanceStructOne.MyIntInStructOne<< std::endl;        // Access to struct
    std::cout << "MyIntInStructTwo = " <<InstanceStructOne.MyIntInStructTwo<< std::endl;        // Access to struct
    std::cout << "MyIntInStructThree = " <<InstanceStructOne.MyIntInStructThree<< std::endl;    // Access to struct
    std::cout << "MyIntInStructFour = " <<InstanceStructOne.MyIntInStructFour<< std::endl;      // Access to struct
    std::cout << "MyIntInStructFive = " <<InstanceStructOne.MyIntInStructFive<< std::endl;      // Access to struct
    std::cout << "MyIntInStructSix = " <<InstanceStructOne.MyIntInStructSix<< std::endl;        // Access to struct
    std::cout << "MyEnum = " <<MyClassOne::MyEnumOne::MyEnumMemberOne<< std::endl;              // Access to enum (CAN BE DONE DIRECTLY)

    MyControlClass MyInstanceControlClass;                                                                                                          // Create instance of class from library
    int VariableToBePointedAt = 9;                                                                                                                  // Create variable to be passed to function
    MyInstanceControlClass.FunctionThatTakesPointerVariableAndStructAndEnumAsParameter(&VariableToBePointedAt, controlstructmain, controlenummain); // Call function

    return 0;
}