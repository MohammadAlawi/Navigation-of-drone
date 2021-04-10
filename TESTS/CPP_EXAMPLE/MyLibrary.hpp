// Example of library header file
// Header file must have
// 1. Guards (ifndef,define,endif)
// 2. Definitions
// 3. Includes
// 4. Namespaces
// 5. Class declarations
// 6. Class constructor and destructor declaration
// 7. Class members = methods(functions and variables with declared datatypes)
// 8. Class members = attributes(variables with declared datatypes)

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
#include <vector>                                   // Header in standard library
#include <utility>                                  // Header in standard library
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
                MyClassOne();                       // Class Constructor declaration
                ~MyClassOne();                      // Class Destructor declaration
                void PrintFromFirstClass(void);     // Method(Function) declaration 
                int PublicIntegerOne;               // Attribute(Variable) declaration
                typedef struct MyStructOne                                  // Attribute declaration (Struct declaration)
                {
                    int MyIntInStructOne;                                   // Member declaration in struct THIS CAN BE READ AND WRITE BY SOURCE FILE
                    int MyIntInStructTwo = 2;                               // Member declaration in struct THIS CAN BE READ AND WRITE BY SOURCE FILE
                    const static int MyIntInStructThree = 3;                // Member declaration in struct THIS CAN ONLY BE READ AND NEED TO BE DEFINED IN HEADER ONLY
                    char MyIntInStructFour = 'C';                           // Member declaration in struct THIS CAN BE READ AND WRITE BY SOURCE FILE
                    float MyIntInStructFive = 3.1414141;                    // Member declaration in struct THIS CAN BE READ AND WRITE BY SOURCE FILE
                    char MyIntInStructSix[10] = "DRONE";                    // Member declaration in struct THIS CAN ONLY BE READ AND NEED TO BE DEFINED IN HEADER ONLY

                }MyStructOne;
                typedef enum MyEnumOne              // Enum declaration
                {
                    MyEnumMemberOne = 1,            // Enum member declaration
                    MyEnumMemberTwo = 2             // Enum member declaration
                }MyEnumOne;
        };
        class MyClassTwo                            // Class declaration
        {
            private:
                float OnlyPrivateFloatTwo;          // Private Attribute(Variable) declaration THIS IS CAN NOT BE ACCESSED OUTSIDE OF CURRENT CLASS
            public:
                MyClassTwo();         // Class Constructor declaration
                ~MyClassTwo();                                                      // Class Destructor declaration
                void PrintFromSecondClass(void);                                    // Method(Function) declaration
                int PublicIntegerTwo;                                               // Attribute(Variable) declaration
                typedef char MyArray[5];                                            // Array declaration
                typedef std::vector<int> MyVector;                                  // Vector declaration as typedef
                int MyIntegerInMyClassTwo = 10;                                     // Integer declaration and definition
                void PrintVectorCalculations(std::vector<int> MyVectorToBePassed);  // Method(function) declaration which takes vector as parameter
                void PrintArrayCalculations(char MyArrayToBePassed[5]);             // Method(function) declaration which takes array as parameter
                /*
                template<class T>
                MyTemplateFunction(T x, T y)
                {
                T z = x + y;
                std::cout << "MyTemplateFunction returns = " <<z<< std::endl;
                return z;
                }
                */
                
        };
        class MyControlClass                                            // Class declaration
        {
            private:
            /* Private data here */
            public:
            MyControlClass();                                           // Class constructor declaration
            ~MyControlClass();                                          // Class destructor declaration
            typedef struct ControlStruct                                // Attribute declaration (Struct declaration)
            {
                char StringFromControlStruct[20] = "ControlString";     // Member declaration in struct
                int ControlInteger = 12345;                             // Member declaration in struct
            }ControlStruct;
            typedef enum ControlEnum                                    // Attribute declaration (Enum declaration)                                      
            {
                ControlEnumOne = 1,                                     // Member declaration in enum
                ControlEnumTwo = 2                                      // Member declaration in enum
            }ControlEnum;
            void FunctionThatTakesPointerVariableAndStructAndEnumAsParameter(int *ControlPointer, ControlStruct controlstruct, ControlEnum controlenum);
            class ClassInMyControlClass
            {
                private:
                /* Private data here */
                public:
                ClassInMyControlClass();                                // Class constructor declaration
                ~ClassInMyControlClass();                               // Class destructor declaration
                void FunctionInClassInMyControlClass();                 // Method(function) declaration
            };
        };
        class TelemetryClass
        {
            private:
            // Private data here
            public:
            TelemetryClass();
            ~TelemetryClass();

            typedef struct Quaternion
            {
                float x;
                float y;
                float z;
            }Quaternion;

            Quaternion GetQuaternion();
        };
        class ClassUtilizingPointers                                    // Class declaration
        {
            private:
            // Private data here
            public:
            ClassUtilizingPointers();                                   // Constructor declaration
            ~ClassUtilizingPointers();                                  // Destructor declaration
            void FunctionInClassUtilizingPointers();                    // Method(function) declaration
            MyClassTwo* myclasstwo;                                     // Pointer declaration from this class to other class (to access other class members using this class)
            MyClassOne* myclassone;                                     // Pointer declaration from this class to other class (to access other class members using this class) 
            TelemetryClass* telemetryclass;             
            
            class ClassInClassUtilizingPointers                         // Class declaration
            {
                private:
                // Private data here
                public:
                ClassInClassUtilizingPointers();                        // Constructor declaration
                ~ClassInClassUtilizingPointers();                       // Destructor declaration
                void FunctionInClassInClassUtilizingPointers();         // Method(function) declaration

            };
        };
    }
}

#endif                                               // Guards
