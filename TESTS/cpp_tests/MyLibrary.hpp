// Example of library header file
// Header file must have
// 1. Guards (ifndef,define,endif)
// 2. Definitions
// 3. Includes
// 4. Namespaces
// 5. Class declarations
// 6. Class members = methods(functions and variables with declared datatypes)
// 7. Class members = attributes(variables with declared datatypes)

// 1. typedef struct
// 2. enum
// 3. functions

// Guards
#ifndef MYLIBRARY_H                                 // Guards
#define MYLIBRARY_H                                 // Guards

// Definitions (These definitions are different than function deffinitions)
#define FILE_SIZE 42                                // This defines that FILE_SIZE is same as 42 THIS IS NOT VARIABLE DEFINITION
#define MESSAGE "Hello"                             // This defines that MESSAGE is same as "Hello" THIS IS NOT VARIABLE DEFINITION

// Includes
#include <iostream>                                 // Header in standard library
#include <cstdlib>                                  // Header in standard library
#include <unistd.h>                                 // Header in standard library
// It is good practice to include all libraries in header file that are used in your library source file and only include this header 
// library file in your library soure file

namespace MyNameSpaceFirst                          // Namespace declaration
{
    namespace MyNameSpaceSecond                     // Namespace declaration
    {
        class MyClassOne                            // Class declaration
        {
            private:
                int OnlyPrivateIntegerOne;          // Private Attribute(Variable) declaration THIS IS CAN NOT BE ACCESSED OUTSIDE OF CURRENT CLASS
            public:
                void PrintFromFirstClass(void);     // Function declaration 
                void ~PrintFromFirstClass(void);    // Destructor is not needed this time but is here to show
                int PublicIntegerOne;               // Attribute(Variable) declaration
                typedef struct MyStructOne
                {
                    const static int MyIntInStructOne;          // Declaration of member in struct
                    const static int MyIntInStructTwo;          // Declaration of member in struct
                    const static int MyIntInStructThree;        // Declaration of member in struct
                }MyStructOne;
                enum MyEnumOne                      // Enum declaration
                {
                    MyEnumMemberOne,                // Enum member declaration
                    MyEnumMemberTwo                 // Enum member declaration
                }
        };
        class MyClassTwo                            // Class declaration
        {
            private:
                float OnlyPrivateFloatTwo;          // Private Attribute(Variable) declaration THIS IS CAN NOT BE ACCESSED OUTSIDE OF CURRENT CLASS
            public:
                void PrintFromSecondClass(void);    // Method(Function) declaration
                void ~PrintFromSecondClass(void);   // Destructor is not needed this time but is here to show
                int PublicIntegerTwo;               // Attribute(Variable) declaration
        };
    }
}
endif                                               // Guards