#include <Python.h>
#include <iostream>
#include <Windows.h>
#include <cmath>
#include <string>
#include <fstream>

using namespace std;

void printMenu(); // Print menu options
void displayItemsAndFrequency(); // Display how many times each item appears in the list
void getItemFrequency(); // Get user input and display how many times that appear in the list
void displayHistogram(); // Display histogram of all the items in the list

/*
Description: To call this function, simply pass the function name in Python that you wish to call.
Example : callProcedure("printsomething");
Output: Python will print on the screen : Hello from python!
Return : None
*/
void CallProcedure(string pName)
{
	char* procname = new char[pName.length() + 1];
	std::strcpy(procname, pName.c_str());

	Py_Initialize();
	PyObject* my_module = PyImport_ImportModule("python_app");
	PyErr_Print();
	PyObject* my_function = PyObject_GetAttrString(my_module, procname);
	PyObject* my_result = PyObject_CallObject(my_function, NULL);
	Py_Finalize();

	delete[] procname;
}


/*
Description: To call this function, pass the name of the Python functino you wish to call and the string parameter you want to send
Example: int x = callIntFunc("PrintMe","Test");
Output: Python will print on the screen:
		You sent me: Test
Return: 100 is returned to the C++
*/

int callIntFunc(string proc, string param)
{
	char* procname = new char[proc.length() + 1];
	std::strcpy(procname, proc.c_str());

	char* paramval = new char[param.length() + 1];
	std::strcpy(paramval, param.c_str());

	PyObject* pName, * pModule, * pDict, * pFunc, * pValue = nullptr, * presult = nullptr;
	// Initialize the Python Interpreter
	Py_Initialize();
	// Build the name object
	pName = PyUnicode_FromString((char*)"python_app");
	// Load the module object
	pModule = PyImport_Import(pName);
	// pDict is a borrowed reference 
	pDict = PyModule_GetDict(pModule);
	// pFunc is also a borrowed reference 
	pFunc = PyDict_GetItemString(pDict, procname);
	if (PyCallable_Check(pFunc))
	{
		pValue = Py_BuildValue("(z)", paramval);
		PyErr_Print();
		presult = PyObject_CallObject(pFunc, pValue);
		PyErr_Print();
	}
	else
	{
		PyErr_Print();
	}
	//printf("Result is %d\n", _PyLong_AsInt(presult));
	Py_DECREF(pValue);
	// Clean up
	Py_DECREF(pModule);
	Py_DECREF(pName);
	// Finish the Python Interpreter
	Py_Finalize();

	// clean 
	delete[] procname;
	delete[] paramval;

	return _PyLong_AsInt(presult);
}

/*
Description: To call this function, pass the name of the Python functino you wish to call and the string parameter you want to send
Example: int x = callIntFunc("doublevalue",5);
Return: 25 is returned to the C++
*/
int callIntFunc(string proc, int param)
{
	char* procname = new char[proc.length() + 1];
	std::strcpy(procname, proc.c_str());

	PyObject* pName, * pModule, * pDict, * pFunc, * pValue = nullptr, * presult = nullptr;
	// Initialize the Python Interpreter
	Py_Initialize();
	// Build the name object
	pName = PyUnicode_FromString((char*)"python_app");
	// Load the module object
	pModule = PyImport_Import(pName);
	// pDict is a borrowed reference 
	pDict = PyModule_GetDict(pModule);
	// pFunc is also a borrowed reference 
	pFunc = PyDict_GetItemString(pDict, procname);
	if (PyCallable_Check(pFunc))
	{
		pValue = Py_BuildValue("(i)", param);
		PyErr_Print();
		presult = PyObject_CallObject(pFunc, pValue);
		PyErr_Print();
	}
	else
	{
		PyErr_Print();
	}
	//printf("Result is %d\n", _PyLong_AsInt(presult));
	Py_DECREF(pValue);
	// Clean up
	Py_DECREF(pModule);
	Py_DECREF(pName);
	// Finish the Python Interpreter
	Py_Finalize();

	// clean 
	delete[] procname;

	return _PyLong_AsInt(presult);
}


int main()
{
	int userInput;
	do {
		printMenu(); // print menu options
		try {
			cin >> userInput; // get user input
			if (userInput < 1 || userInput > 4) {
				// throw an exception for invalid (out of range) inputs
				throw runtime_error("Invalid selection. Try again.\n");
			}
			// handle the different menu options
			switch (userInput) {
			case 1:
				displayItemsAndFrequency();
				break;
			case 2:
				getItemFrequency();
				break;
			case 3:
				displayHistogram();
				break;
			case 4:
				cout << "\nGoodbye." << endl;
				break;
			default:
				break;
			}
		}
		catch (runtime_error& excpt) {
			cout << "Error: " << excpt.what() << endl;
		}
	} while (userInput != 4); // iterate until user exits program
	return 0;
}

void printMenu() {
	cout << "WELCME TO CORNER GROCER" << endl;
	cout << "1: Display a summary of all purchased items" << endl;
	cout << "2: Look up an item" << endl;
	cout << "3: Display histogram" << endl;
	cout << "4: Exit" << endl;
	cout << "Enter your selection as a number (1, 2, 3, or 4)\n\n";
}

void displayItemsAndFrequency() {
	system("cls");	
	CallProcedure("PrintSummary");
	cout << endl;
}

void getItemFrequency() {
	system("cls");
	string userInput;
	cout << "Enter item name: ";
	cin >> userInput;
	int itemFrequency = callIntFunc("SearchItem", userInput);
	if (itemFrequency == 0) {
		cout << "Item not found!" << endl;
	}
	else {
		cout << "Item found! " << userInput << " was purchased " << itemFrequency << " times." << endl;
	}
	cout << endl;
}

void displayHistogram() {
	system("cls");
	CallProcedure("ProcessInput");

	ifstream inStream;
	string item;
	int frequency;
	const int LABEL_SIZE = 12;
	inStream.open("frequency.dat"); // open file created by ProcessInput
	if (!inStream.is_open()) {
		cout << "Could not open file. Try again" << endl;
	} else {
		cout << "GROCERY LIST HISTOGRAM" << endl;
		while (!inStream.eof()) { // while end of file is not reached
			inStream >> item; // get item and its frequency
			inStream >> frequency;
			if (!inStream.fail()) { // print histogram if succeed
				int whiteSpaceSize = LABEL_SIZE - item.size();
				cout << item << string(whiteSpaceSize, ' ');
				cout << "|";
				cout << string(frequency, '*') << endl;
			}
		}
		cout << string(LABEL_SIZE, ' ') << "|----|----|----|----|----|----|" << endl;
		cout << string(LABEL_SIZE, ' ') << "0    5    10   15   20   25   30" << endl;
	}
	inStream.close();

	cout << endl;
}