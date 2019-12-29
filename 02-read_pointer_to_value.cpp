#include <iostream>
#include <string>
#include <Windows.h>

using namespace std;

int showError(const char *msg)
{
    cerr << msg << ". GetLastError = " << dec << GetLastError() << endl;
    system("pause");
    return EXIT_FAILURE;
}

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
    int* otherProcPtr2Int;
    int otherProcVarInt;

    if (otherProcHandle == NULL)
    {
        exitcode = showError("Open Process failed");
    }

    else
    {
        if (!ReadProcessMemory(otherProcHandle, (LPCVOID) baseAddress,
            &otherProcPtr2Int, sizeof(int*), NULL))
        {
            exitcode = showError("ReadProcessMemory failed");
        }
        
        else
        {
            if (!ReadProcessMemory(otherProcHandle, (LPCVOID) otherProcPtr2Int,
                &otherProcVarInt, sizeof(int), NULL))
            {
                exitcode = showError("ReadProcessMemory failed");
            }
            
            else
            {
                cout << "otherProcVarInt = " << dec << otherProcVarInt << endl;
            }
        }
    }

    cout << "Press ENTER to quit";
    getchar();

    return exitcode;
}
