// Example of library source file
// Library Source file must have
// 1. Includes
// 2. Namespace use or not
// 3. Definition of class member

// Includes
#include "MyLibrary.hpp"                                            // Header in local library
// It is good practice to include all libraries in header file that are used in your library source file and only include this header 
// library file in your library soure file

// Regarding namespaces one option is to use identifier "using namespace <NamespaceName>" and another is not to use and thereby use
// full names
// This example shows both options
// Option 1

using namespace MyNameSpaceFirst;                                   // Namespace definition (This is optional, classes can be accessed with full name)
using namespace MyNameSpaceFirst::MyNameSpaceSecond;                // Namespace definition (This is optional, classes can be accessed with full name)

MyClassOne::MyStructOne InstanceStructOneInLibrary;                 // Create instance of typedef struct
MyClassOne::MyEnumOne InstanceEnumOneInLibrary;                     // Create instance of enum

void MyClassOne::PrintFromFirstClass(void)                          // Method(function) definition
{                                                                   
    std::cout << "This is print from first class" << std::endl;               
}   

MyClassOne::~MyClassOne()                                           // Object destructor definition
{                                                                   
    std::cout << "MyClassOne object deleted" << std::endl;              
}                                               






