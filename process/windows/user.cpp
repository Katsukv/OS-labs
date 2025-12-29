#include <windows.h>
#include <iostream>
#include <string>
#include <cstdlib>
#include <tlhelp32.h>

bool IsProcessRunning(const std::string& processName) {
    PROCESSENTRY32 entry;
    entry.dwSize = sizeof(PROCESSENTRY32);

    HANDLE snapshot = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, NULL);
    if (snapshot == INVALID_HANDLE_VALUE) {
        std::cerr << "Failed to create process snapshot. Error: " << GetLastError() << std::endl;
        return false;
    }

    bool found = false;
    if (Process32First(snapshot, &entry)) {
        do {
            if (_stricmp(entry.szExeFile, processName.c_str()) == 0) {
                found = true;
                break;
            }
        } while (Process32Next(snapshot, &entry));
    }

    CloseHandle(snapshot);
    return found;
}

bool IsProcessRunning(DWORD processId) {
    HANDLE hProcess = OpenProcess(PROCESS_QUERY_INFORMATION, FALSE, processId);
    if (hProcess == NULL) {
        return false;
    }
    CloseHandle(hProcess);
    return true;
}

void RunKiller(const std::string& args) {
    STARTUPINFO si = { sizeof(si) };
    PROCESS_INFORMATION pi;

    std::string command = "killer.exe " + args;
    if (!CreateProcess(
        NULL,
        const_cast<char*>(command.c_str()),
        NULL,
        NULL,
        FALSE,
        0,
        NULL,
        NULL,
        &si,
        &pi
    )) {
        std::cerr << "Failed to start Killer. Error: " << GetLastError() << std::endl;
        return;
    }

    WaitForSingleObject(pi.hProcess, INFINITE);
    CloseHandle(pi.hProcess);
    CloseHandle(pi.hThread);
}

int main() {
    // Set PROC_TO_KILL environment variable
    const char* envVar = "PROC_TO_KILL=notepad,calc";
    if (!_putenv(envVar)) {
        std::cout << "Set PROC_TO_KILL=" << envVar << std::endl;
    } else {
        std::cerr << "Failed to set PROC_TO_KILL." << std::endl;
        return 1;
    }

    // Test --id (kill a specific process)
    std::cout << "Testing --id..." << std::endl;
    DWORD testPid = 0;
    {
        STARTUPINFO si = { sizeof(si) };
        PROCESS_INFORMATION pi;
        if (CreateProcess(
            NULL,
            "notepad.exe",
            NULL,
            NULL,
            FALSE,
            0,
            NULL,
            NULL,
            &si,
            &pi
        )) {
            testPid = pi.dwProcessId;
            CloseHandle(pi.hProcess);
            CloseHandle(pi.hThread);
        }
    }

    if (testPid != 0) {
        std::cout << "Started notepad.exe with PID: " << testPid << std::endl;
        if (IsProcessRunning(testPid)) {
            std::cout << "Process is running. Killing it..." << std::endl;
            RunKiller("--id " + std::to_string(testPid));
            if (!IsProcessRunning(testPid)) {
                std::cout << "Process successfully killed." << std::endl;
            } else {
                std::cerr << "Failed to kill process." << std::endl;
            }
        }
    }

    // Test --name (kill all processes by name)
    std::cout << "\nTesting --name..." << std::endl;
    if (IsProcessRunning("calc.exe")) {
        std::cout << "calc.exe is running. Killing it..." << std::endl;
        RunKiller("--name calc.exe");
        if (!IsProcessRunning("calc.exe")) {
            std::cout << "calc.exe successfully killed." << std::endl;
        } else {
            std::cerr << "Failed to kill calc.exe." << std::endl;
        }
    } else {
        std::cout << "calc.exe is not running. Starting it..." << std::endl;
        STARTUPINFO si = { sizeof(si) };
        PROCESS_INFORMATION pi;
        if (CreateProcess(
            NULL,
            "calc.exe",
            NULL,
            NULL,
            FALSE,
            0,
            NULL,
            NULL,
            &si,
            &pi
        )) {
            CloseHandle(pi.hProcess);
            CloseHandle(pi.hThread);
            std::cout << "Started calc.exe. Killing it..." << std::endl;
            RunKiller("--name calc.exe");
            if (!IsProcessRunning("calc.exe")) {
                std::cout << "calc.exe successfully killed." << std::endl;
            } else {
                std::cerr << "Failed to kill calc.exe." << std::endl;
            }
        }
    }

    // Test PROC_TO_KILL (kill processes from env var)
    std::cout << "\nTesting PROC_TO_KILL..." << std::endl;
    if (IsProcessRunning("notepad.exe")) {
        std::cout << "notepad.exe is running. Killer should kill it via PROC_TO_KILL." << std::endl;
    } else {
        std::cout << "notepad.exe is not running. Starting it..." << std::endl;
        STARTUPINFO si = { sizeof(si) };
        PROCESS_INFORMATION pi;
        if (CreateProcess(
            NULL,
            "notepad.exe",
            NULL,
            NULL,
            FALSE,
            0,
            NULL,
            NULL,
            &si,
            &pi
        )) {
            CloseHandle(pi.hProcess);
            CloseHandle(pi.hThread);
        }
    }
    RunKiller(""); // Just run Killer without args to trigger PROC_TO_KILL
    if (!IsProcessRunning("notepad.exe")) {
        std::cout << "notepad.exe successfully killed via PROC_TO_KILL." << std::endl;
    } else {
        std::cerr << "Failed to kill notepad.exe via PROC_TO_KILL." << std::endl;
    }

    // Clean up environment variable
    if (!_putenv("PROC_TO_KILL=")) {
        std::cout << "\nRemoved PROC_TO_KILL environment variable." << std::endl;
    } else {
        std::cerr << "Failed to remove PROC_TO_KILL." << std::endl;
    }

    return 0;
}