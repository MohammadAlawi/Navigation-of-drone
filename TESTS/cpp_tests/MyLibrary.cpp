// Example of library source file
// Library Source file must have
// 1. Includes
// 2. Namespace use or not
// 3. Definition of class member

// Includes
#include "MyLibrary.hpp"                                // Header in local library
// It is good practice to include all libraries in header file that are used in your library source file and only include this header 
// library file in your library soure file

// Regarding namespaces one option is to use identifier "using namespace <NamespaceName>" and another is not to use and thereby use
// full names
// This example shows both options
// Option 1

using namespace MyNameSpaceFirst;                                   // Namespace definition
using namespace MyNameSpaceFirst::MyNameSpaceSecond;                // Namespace definition

void MyClassOne::PrintFromFirstClass(void)                          // Method(function) definition
{                                                                   // Method(function) definition
    std::cout << "This is print from first class" << std::endl;     // Method(function) definition
}                                                                   // Method(function) definition

MyClassOne::MyStructOne::MyIntInStructOne = 1;                      // Attribute(variable) definition
MyClassOne::MyStructOne::MyIntInStructTwo = 2;                      // Attribute(variable) definition

MyClassOne::MyEnumOne::MyEnumMemberOne = 10;                        // Attribute(variable) definition
MyClassOne::MyEnumOne::MyEnumMemberTwo = 20;                        // Attribute(variable) definition


