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

template<typename last_value_type>
last_value_type readChain(vector<uintptr_t>& chain, HANDLE processHandle)
{
    last_value_type last;
    int size = chain.size();
    LPCVOID ptr = (LPCVOID) chain[0]; // base address
    BOOL success = ReadProcessMemory(processHandle, ptr, &ptr, sizeof(ptr), NULL);

    for (int i = 1; success && i < size - 1; i++)
    {
        ptr += chain[i];
        success = ReadProcessMemory(processHandle, ptr, &ptr, sizeof(ptr), NULL);
    }

    ptr += chain[size - 1];
    success = ReadProcessMemory(processHandle, ptr, &last, sizeof(last), NULL);

    return last;
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

    HANDLE otherProcHandle = OpenProcess(PROCESS_VM_READ, FALSE, processId);
    int exitcode = EXIT_FAILURE;

    if (otherProcHandle == NULL)
    {
        exitcode = showError("Open Process failed");
    }

    else
    {
        // Walk through the pointers chain and get the int at the final
        cout << readChain<int>(chain, otherProcHandle) << endl << endl;

        // Pointer to the arrChar in the other process
        LPCVOID ptrToArrChar = (LPCVOID) 0xffffcb00;
        char arrCharBuffer[100];
        // Get the characters from the char array of the other process
        ReadProcessMemory(otherProcHandle, ptrToArrChar, arrCharBuffer, sizeof(arrCharBuffer), NULL);
        cout << "arrCharBuffer: '" << arrCharBuffer << "'" << endl;
    }

    CloseHandle(otherProcHandle);

    cout << "Press ENTER to quit";
    getchar();

    return exitcode;
}
