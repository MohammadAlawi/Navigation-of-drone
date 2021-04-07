/*
#include <stdio.h>
#include "python3.6/Python.h"

int main()
{
	char filename[] = "testfile.py";
	FILE* fp;

	Py_Initialize();

	fp = _Py_fopen(filename, "r");
	PyRun_SimpleFile(fp, filename);

	Py_FinalizeEx();
	return 0;
}
*/

/*
#include <stdio.h>
#include "./include/pyhelper.hpp"

int main()
{
    CPyInstance pyInstance;

	PyRun_SimpleString("print('Hello World from Embedded Python!!!')");
	

	printf("\nPress any key to exit...\n");
	return 0;
}*/

#include <stdio.h>
#include "python3.6/Python.h"
#include "./include/pyhelper.hpp"

int main()
{
	CPyInstance hInstance;

	CPyObject pName = PyUnicode_FromString("testfile");
	CPyObject pModule = PyImport_Import(pName);

	if(pModule)
	{
		CPyObject pFunc = PyObject_GetAttrString(pModule, "getInteger");
		if(pFunc && PyCallable_Check(pFunc))
		{
			CPyObject pValue = PyObject_CallObject(pFunc, NULL);

			printf("C: getInteger() = %ld\n", PyLong_AsLong(pValue));
		}
		else
		{
			printf("ERROR: function getInteger()\n");
		}

	}
	else
	{
		printf("ERROR: Module not imported\n");
	}

	return 0;
}