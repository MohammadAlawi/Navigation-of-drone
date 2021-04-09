/*
#include <stdio.h>
#include "python3.6/Python.h"

struct World
{
	char filename[] = "testfile.py";
	FILE* fp;

/// Staticly linking a Python extension for embedded Python.
BOOST_PYTHON_MODULE(hello)
{
  namespace python = boost::python;
  python::class_<World>("World")
    .def("greet", &World::greet)
    .def("set", &World::set)
    ;
}

int main(int argc, char *argv[])
{
  if (argc < 3)
  {
    std::cerr << "Usage: call pythonfile funcname [args]" << std::endl;
    return 1;
  }
  char* module_name   = argv[1];
  char* function_name = argv[2];

  // Explicitly add initializers for staticly linked modules.
  PyImport_AppendInittab("hello", &inithello);

  // Initialize Python.
  setenv("PYTHONPATH", ".", 1);
  Py_Initialize();

  namespace python = boost::python;
  try
  {
    // Convert remaining args into a Python list of integers.
    python::list args;
    for (int i=3; i < argc; ++i)
    {
      args.append(std::atoi(argv[i]));
    }

    // Import the user requested module.
    // >>> import module
    python::object module = python::import(module_name);

    // Invoke the user requested function with the provided arguments.
    // >>> result = module.fn(*args)
    python::object result = module.attr(function_name)(*python::tuple(args));

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
  PyRun_SimpleString("sys.path.append(os.getcwd())");
	CPyObject pName = PyUnicode_FromString("multitag_positioning");
	CPyObject pModule = PyImport_Import(pName);

	if(pModule)
	{
		CPyObject pFunc = PyObject_GetAttrString(pModule, "getPosition");
		if(pFunc && PyCallable_Check(pFunc))
		{
			CPyObject pValue = PyObject_CallObject(pFunc, NULL);

			printf("C: POZYX multiposition = %ld\n", PyLong_AsLong(pValue));
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