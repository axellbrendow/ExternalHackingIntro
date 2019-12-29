#include <iostream>
#include <string>
#include <Windows.h>

using namespace std;

int main()
{
    int processId;
    void *baseAddress;

    cout << "Inform processId: ";
    cin >> processId;

    cout << "Inform base address: ";
    cin >> baseAddress;

    HANDLE otherProcHandle = OpenProcess(PROCESS_ALL_ACCESS, FALSE, processId);
    int exitcode = EXIT_FAILURE;
    int otherProcVarInt;

    if (otherProcHandle == NULL)
    {
        cerr << "OpenProcess failed. GetLastError = " << dec << GetLastError() << endl;
        system("pause");
        exitcode = EXIT_FAILURE;
    }

    else
    {
        if (ReadProcessMemory(otherProcHandle, (LPCVOID) baseAddress,
            &otherProcVarInt, sizeof(int), NULL))
        {
            cout << "otherProcVarInt = " << dec << otherProcVarInt << endl;
        }
    }

    cout << "Press ENTER to quit";
    getchar();

    return exitcode;
}
