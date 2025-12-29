#include <iostream>
#include <string>
#include <vector>
#include <cstdlib>
#include <unistd.h>
#include <dirent.h>
#include <signal.h>
#include <fstream>
#include <sstream>
#include <algorithm>

using namespace std;

string get_process_name(pid_t pid) {
    string path = "/proc/" + to_string(pid) + "/comm";
    ifstream file(path);
    string name;
    if (file >> name) return name;
    return "";
}

bool kill_process(pid_t pid) {
    if (kill(pid, SIGTERM) == 0) {
        cout << "Process " << pid << " terminated (SIGTERM)" << endl;
        return true;
    }
    if (kill(pid, SIGKILL) == 0) {
        cout << "Process " << pid << " killed (SIGKILL)" << endl;
        return true;
    }
    cerr << "Failed to kill process " << pid << endl;
    return false;
}

void kill_processes_by_name(const string& name) {
    DIR* dir = opendir("/proc");
    if (!dir) {
        cerr << "Failed to open /proc directory" << endl;
        return;
    }

    struct dirent* entry;
    bool found = false;
    pid_t my_pid = getpid();

    while ((entry = readdir(dir)) != nullptr) {
        if (isdigit(entry->d_name[0])) {
            pid_t pid = stoi(entry->d_name);
            if (pid == my_pid) continue; // Не убиваем сами себя

            string proc_name = get_process_name(pid);
            if (proc_name == name) {
                cout << "Found process: " << name << " (PID: " << pid << ")" << endl;
                if (kill_process(pid)) found = true;
            }
        }
    }
    closedir(dir);

    if (!found) {
        cout << "No processes found with name: " << name << endl;
    }
}

vector<string> split_string(const string& s, char delimiter) {
    vector<string> tokens;
    string token;
    istringstream token_stream(s);
    while (getline(token_stream, token, delimiter)) {
        if (!token.empty()) {
            tokens.push_back(token);
        }
    }
    return tokens;
}

int main(int argc, char* argv[]) {
    if (argc == 1) {
        char* env = getenv("PROC_TO_KILL");
        if (!env) {
            cerr << "PROC_TO_KILL environment variable not set" << endl;
            return 1;
        }

        vector<string> names = split_string(env, ',');
        for (const auto& name : names) {
            kill_processes_by_name(name);
        }
    }
    else if (argc == 3) {
        string param = argv[1];
        if (param == "--id") {
            pid_t pid = stoi(argv[2]);
            kill_process(pid);
        }
        else if (param == "--name") {
            kill_processes_by_name(argv[2]);
        }
        else {
            cerr << "Usage: " << argv[0] << " [--id PID | --name NAME]" << endl;
            return 1;
        }
    }
    else {
        cerr << "Usage: " << argv[0] << " [--id PID | --name NAME]" << endl;
        return 1;
    }

    return 0;
}