#include <windows.h>
#include <iostream>
#include <string>
#include <vector>
#include <sstream>
#include <tlhelp32.h>

void KillProcessById(DWORD processId) {
    HANDLE hProcess = OpenProcess(PROCESS_TERMINATE, FALSE, processId);
    if (hProcess == NULL) {
        std::cerr << "Failed to open process with ID " << processId << ". Error: " << GetLastError() << std::endl;
        return;
    }

    if (!TerminateProcess(hProcess, 0)) {
        std::cerr << "Failed to terminate process with ID " << processId << ". Error: " << GetLastError() << std::endl;
    } else {
        std::cout << "Successfully terminated process with ID " << processId << std::endl;
    }

    CloseHandle(hProcess);
}

void KillProcessesByName(const std::string& processName) {
    PROCESSENTRY32 entry;
    entry.dwSize = sizeof(PROCESSENTRY32);

    HANDLE snapshot = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, NULL);
    if (snapshot == INVALID_HANDLE_VALUE) {
        std::cerr << "Failed to create process snapshot. Error: " << GetLastError() << std::endl;
        return;
    }

    bool found = false;
    if (Process32First(snapshot, &entry)) {
        do {
            if (_stricmp(entry.szExeFile, processName.c_str()) == 0) {
                KillProcessById(entry.th32ProcessID);
                found = true;
            }
        } while (Process32Next(snapshot, &entry));
    }

    if (!found) {
        std::cout << "No processes found with name: " << processName << std::endl;
    }

    CloseHandle(snapshot);
}

void KillProcessesFromEnvVar() {
    char* envVar = nullptr;
    size_t len = 0;

    envVar = getenv("PROC_TO_KILL");
    if (envVar == nullptr) {
        std::cout << "Environment variable PROC_TO_KILL not found." << std::endl;
        return;
    }

    std::string processes(envVar);
    std::vector<std::string> processNames;
    std::stringstream ss(processes);
    std::string name;

    while (std::getline(ss, name, ',')) {
        if (!name.empty()) {
            processNames.push_back(name);
        }
    }

    for (const auto& procName : processNames) {
        KillProcessesByName(procName);
    }
}

int main(int argc, char* argv[]) {
    if (argc < 2) {
        std::cerr << "Usage: " << argv[0] << " --id <process_id> | --name <process_name>" << std::endl;
        return 1;
    }

    std::string arg = argv[1];
    if (arg == "--id" && argc == 3) {
        DWORD processId = std::stoul(argv[2]);
        KillProcessById(processId);
    } else if (arg == "--name" && argc == 3) {
        KillProcessesByName(argv[2]);
    } else {
        std::cerr << "Invalid arguments." << std::endl;
        return 1;
    }

    KillProcessesFromEnvVar();

    return 0;
}