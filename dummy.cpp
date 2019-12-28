#include <iostream>
#include <string>
#include <Windows.h>

using namespace std;

int main()
{
    int varInt = 123456;
    string varString = "DefaultString";
    char arrChar[128] = "Long char array right there ->";

    int* ptr2int = &varInt;
    int** ptr2ptr = &ptr2int;
    int*** ptr2ptr2 = &ptr2ptr;

    while (1)
    {
        cout << "Process ID:\t" << GetCurrentProcessId() << endl;

        cout << "varInt\t\t(" << hex << uppercase << &varInt << ") = " << varInt << endl;
        cout << "varString\t(" << hex << uppercase << &varString << ") = " << varString << endl;
        cout << "arrChar\t\t(" << hex << uppercase << &arrChar << ") = " << arrChar << endl;
        
        cout << "ptr2int\t\t(" << hex << uppercase << &ptr2int << ") = " << ptr2int << endl;
        cout << "ptr2ptr\t\t(" << hex << uppercase << &ptr2ptr << ") = " << ptr2ptr << endl;
        cout << "ptr2ptr2\t(" << hex << uppercase << &ptr2ptr2 << ") = " << ptr2ptr2 << endl;

        cout << "Press ENTER to print again";
        
        getchar();

        cout << endl << "------------------------------------" << endl << endl;
    }
}
