#include <iostream>
#include <string>
#include <vector>
#include <Windows.h>

using namespace std;

int showError(const char *msg)
{
    cerr << msg << ". GetLastError = " << dec << GetLastError() << endl;
    system("pause");
    return EXIT_FAILURE;
}

LPCVOID walkChain(vector<uintptr_t>& chain, HANDLE processHandle)
{
    int size = chain.size();
    char* ptr = (char*) chain[0]; // base address
    BOOL success = ReadProcessMemory(processHandle, ptr, &ptr, sizeof(ptr), NULL);

    for (int i = 1; success && i < size - 1; i++)
    {
        ptr += chain[i];
        success = ReadProcessMemory(processHandle, ptr, &ptr, sizeof(ptr), NULL);
    }

    ptr += chain[size - 1];

    return (LPCVOID) ptr;
}

template<typename last_value_type>
last_value_type readChain(vector<uintptr_t>& chain, HANDLE processHandle)
{
    last_value_type last;
    LPCVOID ptr = walkChain(chain, processHandle);

    ReadProcessMemory(processHandle, ptr, &last, sizeof(last), NULL);

    return last;
}

template<typename last_value_type>
void writeChain(vector<uintptr_t>& chain, HANDLE processHandle, last_value_type value)
{
    LPVOID ptr = (LPVOID) walkChain(chain, processHandle);
    
    WriteProcessMemory(processHandle, ptr, (LPVOID) &value, sizeof(value), NULL);
}

int main()
{
    int processId, numOffsets;
    uintptr_t baseAddress;
    uintptr_t offset;
    vector<uintptr_t> chain; // { 0xffffcae8, 0, 0, 0 }
    // vector<uintptr_t> strChain({ 0xffffcb80 });
    // vector<uintptr_t> arrCharChain({ 0xffffcb00 });

    // Read information to walk through pointers chain
    cout << "Inform process id, base address and number of offsets separated by spaces: ";
    cin >> processId >> hex >> baseAddress >> numOffsets;
    chain.push_back(baseAddress);
    
    for (size_t i = 0; i < numOffsets; i++)
    {
        cout << "Offset" << i << ": ";
        cin >> offset;
        chain.push_back(offset);
    }
    
    cout << endl;

    HANDLE otherProcHandle = OpenProcess(
        PROCESS_VM_OPERATION | PROCESS_VM_READ | PROCESS_VM_WRITE, FALSE, processId);

    int exitcode = EXIT_FAILURE;

    if (otherProcHandle == NULL)
    {
        exitcode = showError("Open Process failed");
    }

    else
    {
        // Walk through the pointers chain and get the int at the final
        cout << readChain<int>(chain, otherProcHandle) << endl << endl;

        writeChain(chain, otherProcHandle, 99);

        // Walk through the pointers chain and get the int at the final
        cout << readChain<int>(chain, otherProcHandle) << endl << endl;
    }

    CloseHandle(otherProcHandle);

    cout << "Press ENTER to quit";
    getchar();

    return exitcode;
}
