#include <windows.h>
#include <iostream>
#include <string>
#include <sstream>

using namespace std;

void CreateProcessWithPipes(const string& appName, HANDLE hRead, HANDLE hWrite) {
    SECURITY_ATTRIBUTES sa = { sizeof(SECURITY_ATTRIBUTES), NULL, TRUE };

    STARTUPINFO si = { sizeof(STARTUPINFO) };
    PROCESS_INFORMATION pi;

    si.dwFlags = STARTF_USESTDHANDLES;
    si.hStdInput = hRead;
    si.hStdOutput = hWrite;
    si.hStdError = GetStdHandle(STD_ERROR_HANDLE);

    string cmd = appName + ".exe";
    if (!CreateProcess(NULL, const_cast<char*>(cmd.c_str()), NULL, NULL, TRUE,
                      0, NULL, NULL, &si, &pi)) {
        cerr << "CreateProcess failed (" << GetLastError() << ")" << endl;
        exit(1);
                      }

    CloseHandle(pi.hProcess);
    CloseHandle(pi.hThread);
}

int main() {
    SECURITY_ATTRIBUTES sa = { sizeof(SECURITY_ATTRIBUTES), NULL, TRUE };

    HANDLE hPipeMA[2], hPipeAP[2], hPipePS[2];

    if (!CreatePipe(&hPipeMA[0], &hPipeMA[1], &sa, 0) ||
        !CreatePipe(&hPipeAP[0], &hPipeAP[1], &sa, 0) ||
        !CreatePipe(&hPipePS[0], &hPipePS[1], &sa, 0)) {
        cerr << "CreatePipe failed" << endl;
        return 1;
        }

    CreateProcessWithPipes("M", GetStdHandle(STD_INPUT_HANDLE), hPipeMA[1]);
    CreateProcessWithPipes("A", hPipeMA[0], hPipeAP[1]);
    CreateProcessWithPipes("P", hPipeAP[0], hPipePS[1]);
    CreateProcessWithPipes("S", hPipePS[0], GetStdHandle(STD_OUTPUT_HANDLE));

    CloseHandle(hPipeMA[0]);
    CloseHandle(hPipeMA[1]);
    CloseHandle(hPipeAP[0]);
    CloseHandle(hPipeAP[1]);
    CloseHandle(hPipePS[0]);
    CloseHandle(hPipePS[1]);

    Sleep(1000);

    return 0;
}