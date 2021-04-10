// Example of library source file
// Library Source file must have
// 1. Includes
// 2. Namespace use or not
// 3. Constructor and Destructor definition
// 4. Class member definition

// Includes
#include "MyLibrary.hpp"                                            // Header in local library
// It is good practice to include all libraries in header file that are used in your library source file and only include this header 
// library file in your library soure file

// Regarding namespaces one option is to use identifier "using namespace <NamespaceName>" and another is not to use and thereby use
// full names

using namespace MyNameSpaceFirst;                                   // Namespace definition (This is optional, classes can be accessed with full name)
using namespace MyNameSpaceFirst::MyNameSpaceSecond;                // Namespace definition (This is optional, classes can be accessed with full name)

MyClassOne::MyStructOne InstanceStructOneInLibrary;                 // Create instance of typedef struct
MyClassOne::MyEnumOne InstanceEnumOneInLibrary;                     // Create instance of enum
//MyControlClass::ControlStruct controlstruct;                      // Create instance of typedef struct THIS IS NOT NEEDED SINCE ITS PASSED FROM HEADER

//****************************************************************************************************************************************************
// MyClassOne definitions
MyClassOne::MyClassOne()                                            // Class constructor definition
{
    std::cout << "MyClassOne constructor called" << std::endl;
}

MyClassOne::~MyClassOne()                                           // Class destructor definition
{                                                                   
    std::cout << "MyClassOne destructor called" << std::endl;              
}       

void MyClassOne::PrintFromFirstClass(void)                          // Method(function) definition
{                                                                   
    std::cout << "This is print from first class" << std::endl;               
}

//****************************************************************************************************************************************************
// MyClassTwo definitions
MyClassTwo::~MyClassTwo()                                           // Class constructor definition
{
    std::cout << "MyClassTwo constructor called" << std::endl;
}

MyClassTwo::MyClassTwo()                                            // Class destructor definition
{
    std::cout << "MyClassTwo destructor called" << std::endl;
}

void MyClassTwo::PrintVectorCalculations(std::vector<int> MyVectorToBePassed)         // Method(function) defintion, passing vector to function
{ 
    std::cout << "PrintVectorCalculations called, size of vector " <<MyVectorToBePassed.size()<< std::endl;
    for (int i = 10; i <= 16; i++)
    {
        MyVectorToBePassed.push_back(i);
    }
    std::cout << "Vector size now " <<MyVectorToBePassed.size()<< std::endl;
    std::cout << "Vector first element " <<MyVectorToBePassed.front()<< std::endl;
    std::cout << "Vector third element " <<MyVectorToBePassed.at(3)<< std::endl;
}
void MyClassTwo::PrintArrayCalculations(char MyArrayToBePassed[5])                      // Method(function) declaration which takes array as parameter
{
    std::cout << "PrintArrayCalculations called" << std::endl;
    MyArrayToBePassed[0] = {'D'};                                                       // Accessing to array member and overwriting it
    MyArrayToBePassed[1] = {'R'};                                                       // Accessing to array member and overwriting it
    MyArrayToBePassed[2] = {'O'};                                                       // Accessing to array member and overwriting it
    MyArrayToBePassed[3] = {'N'};                                                       // Accessing to array member and overwriting it
    MyArrayToBePassed[4] = {'E'};                                                       // Accessing to array member and overwriting it
    //MyArrayToBePassed = {'Z','Z'};
    for(int i = 0; i <= 5; i++)
    {
        std::cout << "Array member " <<i<< " value " <<MyArrayToBePassed[i] << std::endl;
    }
    std::cout << "Size of array " <<sizeof(MyArrayToBePassed)<< std::endl;
}

//****************************************************************************************************************************************************
// MyControlClass definitions
MyControlClass::MyControlClass()                                    // Class constructor definition
{
    std::cout << "MyControlClass constructor called" << std::endl;
}

MyControlClass::~MyControlClass()                                   // Class destructor definiton
{
    std::cout << "MyControlClass destructor called" << std::endl;
}

void MyControlClass::FunctionThatTakesPointerVariableAndStructAndEnumAsParameter(int *ControlPointer, ControlStruct controlstruct, ControlEnum controlenum)   // Method(Function definition)
{
    std::cout << "FunctionThatTakesPointerVariableAndStructAndEnumAsParameter has been called" << std::endl;    // Printing
    std::cout << "ControlPointer = " <<*ControlPointer<< std::endl;                                             // Printing
    std::cout << "ControlPointer address = " <<ControlPointer<< std::endl;                                      // Printing
    std::cout << "StringFromControlStruct = " <<controlstruct.StringFromControlStruct<< std::endl;              // Printing
    std::cout << "ControlInteger = " <<controlstruct.ControlInteger<< std::endl;                                // Printing
    std::cout << "ControlEnumOne = " <<ControlEnum::ControlEnumOne<< std::endl;                                 // Printing
    std::cout << "ControlEnumTwo = " <<ControlEnum::ControlEnumTwo<< std::endl;                                 // Printing
}

MyControlClass::ClassInMyControlClass::ClassInMyControlClass()                      // Class constructor definition
{
    std::cout << "ClassInMyControlClass constructor called" << std::endl;
}

MyControlClass::ClassInMyControlClass::~ClassInMyControlClass()                     // Class destructor definition
{
    std::cout << "ClassInMyControlClass destructor called" << std::endl;
}

void MyControlClass::ClassInMyControlClass::FunctionInClassInMyControlClass()       // Class method(function) definition
{
    std::cout << "FunctionInClassInMyControlClass called" << std::endl;
}