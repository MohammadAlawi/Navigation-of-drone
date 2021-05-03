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
//using namespace Telemetry;                                        // Namespace definition (This is optional, classes can be accessed with full name)
using namespace driver;                                             // Driver namespace
                                     

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
MyClassTwo::MyClassTwo()                                                                // Class constructor definition
{
    std::cout << "MyClassTwo constructor called" << std::endl;
}

MyClassTwo::~MyClassTwo()                                                               // Class destructor definition
{
    std::cout << "MyClassTwo destructor called" << std::endl;
}

void MyClassTwo::PrintVectorCalculations(std::vector<int> MyVectorToBePassed)           // Method(function) defintion, passing vector to function
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

//****************************************************************************************************************************************************
// ClassUtilizingPointers definitions

ClassUtilizingPointers::ClassUtilizingPointers()
{
    std::cout << "ClassUtilizingPointers constructor called" << std::endl;
}

ClassUtilizingPointers::~ClassUtilizingPointers()
{
    std::cout << "ClassUtilizingPointers destructor called" << std::endl;
}

void ClassUtilizingPointers::FunctionInClassUtilizingPointers()
{
    std::cout << "FunctionInClassUtilizingPointers called" << std::endl;
}

ClassUtilizingPointers::ClassInClassUtilizingPointers::ClassInClassUtilizingPointers()
{
    std::cout << "ClassInClassUtilizingPointers constructor called" << std::endl;
}

ClassUtilizingPointers::ClassInClassUtilizingPointers::~ClassInClassUtilizingPointers()
{
    std::cout << "ClassInClassUtilizingPointers destructor called" << std::endl;
}

void ClassUtilizingPointers::ClassInClassUtilizingPointers::FunctionInClassInClassUtilizingPointers()
{
    std::cout << "FunctionInClassInClassUtilizingPointers called" << std::endl;
}

void ClassUtilizingPointers::FunctionThatAllocatesMemoryInTwoDifferentWays()
{
    std::cout << "FunctionThatAllocatesMemoryInTwoDifferentWays called" << std::endl;
    int* PointerVariable = NULL;                                                        // Declare pointer and define it to point nowhere(NULL pointer)          
    PointerVariable = new int;                                                          // Allocate memory and call for object constructor
    *PointerVariable = 8998;                                                            // Set value to variable where pointer is pointing to
    std::cout << "Value add PointerVariable " <<*PointerVariable<< std::endl;           
    std::cout << "Address of PointerVariable " <<PointerVariable<< std::endl;
    delete PointerVariable;                                                             // Free allocated memory and delete pointer
    std::cout << "PointerVariable deleted, address of PointerVariable now " <<PointerVariable<<" and value "<<*PointerVariable<< std::endl;

    double* MallocPointer = NULL;                                                       // Declare pointer and define it to point nowhere(NULL pointer)
    MallocPointer = (double*) malloc(4*sizeof(int));                                    // Allocate certain size of memory(bytes)  
    for(int i = 0; i < 4; i++)
    {
        std::cout << "Address of Malloc pointer "<<i<<" "<<MallocPointer+i<< std::endl;
    }
    free(MallocPointer);
    int* PointerSize;
    std::cout << "Size of a pointers on this machine "<<sizeof(PointerSize)<<" "<<PointerSize<< std::endl;
}

void* ClassUtilizingPointers::ParameterlessFunction(void*)
{
    std::cout << "ParameterFunction called" << std::endl;
    /*
    int* c = NULL;
    *c = a + b;
    return c;
    //return NULL;
    */
}

void ClassUtilizingPointers::ParameterlessFunction2(void)
{
    std::cout << "ParameterFunction2 called" << std::endl;
}

void ClassUtilizingPointers::FunctionThatCreatesFourThreads()
{
    std::cout << "FunctionThatCreatesFourThreads called" << std::endl;
    ClassUtilizingPointers::ParameterlessFunction2();                               // Calling method(function)

    pthread_t Thread1, Thread2, Thread3, Thread4;                                   // Initializing and declaring threads (thread pointers)
    int Thread1Return, Thread2Return, Thread3Return, Thread4Return;                 // Declaring variables
    pthread_create( &Thread1, NULL, &FunctionOutsideOfAnyMember, NULL);             // Creating thread
    pthread_join(Thread1, NULL);                                                    // Waiting for thread to complete
    std::cout << "Threads are done" << std::endl;                                   // Print
    //pthread_exit(NULL);                                                           // Terminate threading
}

void ClassUtilizingPointers::StringHandlingAndConvertingFunction(void)
{
    std::cout << "StringHandlingAndConvertingFunction called" << std::endl;
    // Take string(char array) as input
    char MyCharacterPointer[2] = {'A','B'};
    std::cout << "*MyCharacterPointer " <<*MyCharacterPointer<<" Address "<<&MyCharacterPointer<< std::endl;
    std::cout << "*MyCharacterPointer+4 " <<*MyCharacterPointer+1<< std::endl;
    std::cout << "MyCharacterPointer[1] " <<MyCharacterPointer[1]<< std::endl;
    char OverWritingChar[2] = {'S', 'T'};
    MyCharacterPointer[0] = {'O'};
    MyCharacterPointer[1] = OverWritingChar[0];
    std::cout << "After overwrite MyArrayPointer[0] " <<MyCharacterPointer[0]<< std::endl;
    std::cout << "After overwrite MyArrayPointer[1] " <<MyCharacterPointer[1]<< std::endl;
    char* MyArrayPointer[2] = {"FirstInArray", "SecondinArray"};                                                        
    std::cout << "*MyArrayPointer " <<*MyArrayPointer<<" Address "<<MyArrayPointer<< std::endl;
    std::cout << "*MyArrayPointer+4 " <<*MyArrayPointer+4<< std::endl;
    std::cout << "MyArrayPointer[1] " <<MyArrayPointer[1]<< std::endl;
    char* OverWritingArray[2] = {"Y = 6000","3000"};
    MyArrayPointer[0] = {"Z = 1000"};
    MyArrayPointer[1] = OverWritingArray[1];
    std::cout << "After overwrite MyArrayPointer[0] " <<MyArrayPointer[0]<< std::endl;
    std::cout << "After overwrite MyArrayPointer[1] " <<MyArrayPointer[1]<< std::endl;
    double ConvertedDouble = std::stod(MyArrayPointer[1]);
    std::cout << "Converted string from MyArrayPointer[1] " <<ConvertedDouble<< std::endl;

    std::string MyStringToBeSplitted = {"X4444+Y8888+Z9999+"};
    std::size_t start = MyStringToBeSplitted.find("X");
    std::size_t end = MyStringToBeSplitted.find("+");
    std::string SplittedString3 = MyStringToBeSplitted.substr(start+1,end-1);
    std::cout << "Separated manually " <<SplittedString3<< std::endl;
    double SplittedString3Converted = std::stod(SplittedString3);
    std::cout << "Converted string from SplittedString3 " <<SplittedString3Converted<< std::endl;
}

//****************************************************************************************************************************************************
// Telemetry definitions 

TelemetryClass::Quaternion TelemetryClass::GetQuaternion()  // Quaternion is typedef struct and in this source file namespace Telemetry is not used
{
    TelemetryClass::Quaternion data;                        // Create instance of typedef struct
    data.x = 1.11;                                          // Assign values to struct members
    data.y = 2.22;                                          // Assign values to struct members
    data.z = 3.33;                                          // Assign values to struct members
    return data;                                            // Return struct
}

//****************************************************************************************************************************************************
// Thread usage definitions

void* FunctionOutsideOfAnyMember(void*)                     // Define method(function) that is outside of any member THIS IS NEEDED FOR THREADING
{
    std::cout << "FunctionOutsideOfAnyMember called" << std::endl;
}

//****************************************************************************************************************************************************
// Python Embedding
/*
    const char *scriptDirectoryName = "/home/uwb6/DJIOSDK/Onboard-SDK/build/bin";
    Py_Initialize();
    PyObject *sysPath = PySys_GetObject("path");
    PyObject *path = PyUnicode_FromString(scriptDirectoryName);
    int result = PyList_Insert(sysPath, 0, path);
    PyObject *pModule = PyImport_ImportModule("multitag");

    PyObject* myFunction = PyObject_GetAttrString(pModule,(char*)"MainFunction");
    PyObject* args = PyTuple_Pack(1);

    PyObject* myResult = PyObject_CallObject(myFunction, args);
    double getResult = PyFloat_AsDouble(myResult);
    std::cout << "Print my results " << getResult << std::endl;
*/

//****************************************************************************************************************************************************
// Driver code related 

// usingConstAndExplict::usingConstAndExplict() : real(0.0), imag(0.0)    // Constructor initializing private variables
usingConstAndExplict::usingConstAndExplict(float r, float i)
{
    real = r;
    imag = i;
    // Something here
    std::cout << "usingConstAndExplict normal constructor called" << std::endl;
    std::cout << real << " " << imag << std::endl;
}

usingConstAndExplict usingConstAndExplict::operator+ (const usingConstAndExplict& c) const
{
    std::cout << "usingConstAndExplict operator constructor called" << std::endl;
    usingConstAndExplict result;
    result.real = (this->real + c.real);
    result.imag = (this->imag + c.imag);
    return result;
}

usingConstAndExplict::~usingConstAndExplict()    // Destructor
{
    // Something here
    std::cout << "usingConstAndExplict destructor called" << std::endl;
}