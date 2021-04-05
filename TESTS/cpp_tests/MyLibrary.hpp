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

// Definitions (These definitions are different than function definitions)
#define FILE_SIZE 42                                // This defines that FILE_SIZE is same as 42 THIS IS NOT VARIABLE DEFINITION
#define MESSAGE "Hello"                             // This defines that MESSAGE is same as "Hello" THIS IS NOT VARIABLE DEFINITION

// Includes
#include <iostream>                                 // Header in standard library
#include <cstdlib>                                  // Header in standard library
#include <unistd.h>                                 // Header in standard library
// It is good practice to include all libraries in header file that are used in your library source file and only include this header 
// library file in your library source file

namespace MyNameSpaceFirst                          // Namespace declaration
{
    namespace MyNameSpaceSecond                     // Namespace declaration
    {
        class MyClassOne                            // Class declaration
        {
            private:
                int OnlyPrivateIntegerOne;          // Private Attribute(Variable) declaration THIS IS CAN NOT BE ACCESSED OUTSIDE OF CURRENT CLASS
            public:
                ~MyClassOne();                      // Destructor declaration
                void PrintFromFirstClass(void);     // Function declaration 
                int PublicIntegerOne;               // Attribute(Variable) declaration
                typedef struct MyStructOne
                {
                    int MyIntInStructOne;                                   // Declaration of member in struct THIS CAN BE READ AND WRITE BY SOURCE FILE
                    int MyIntInStructTwo = 2;                               // Declaration of member in struct THIS CAN BE READ AND WRITE BY SOURCE FILE
                    const static int MyIntInStructThree = 3;                // Declaration of member in struct THIS CAN ONLY BE READ AND NEED TO BE DEFINED IN HEADER ONLY
                    char MyIntInStructFour = 'C';                           // Declaration of member in struct THIS CAN BE READ AND WRITE BY SOURCE FILE
                    float MyIntInStructFive = 3.1414141;                    // Declaration of member in struct THIS CAN BE READ AND WRITE BY SOURCE FILE
                    char MyIntInStructSix[10] = "DRONE";                    // Declaration of member in struct THIS CAN ONLY BE READ AND NEED TO BE DEFINED IN HEADER ONLY

                }MyStructOne;
                typedef enum MyEnumOne              // Enum declaration
                {
                    MyEnumMemberOne = 1,       // Enum member declaration
                    MyEnumMemberTwo = 2        // Enum member declaration
                }MyEnumOne;
        };
        class MyClassTwo                            // Class declaration
        {
            private:
                float OnlyPrivateFloatTwo;          // Private Attribute(Variable) declaration THIS IS CAN NOT BE ACCESSED OUTSIDE OF CURRENT CLASS
            public:
                ~MyClassTwo();                      // Destructor declaration
                void PrintFromSecondClass(void);    // Method(Function) declaration
                int PublicIntegerTwo;               // Attribute(Variable) declaration
        };
    }
}
#endif                                               // Guards