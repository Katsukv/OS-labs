#include <iostream>
#include <string>
#include <cstdlib>
#include <unistd.h>
#include <sys/wait.h>

using namespace std;

void run_killer(const string& args) {
    pid_t pid = fork();
    if (pid == 0) {
        execl("./killer", "./killer", args.c_str(), nullptr);
        cerr << "Failed to execute killer" << endl;
        exit(1);
    }
    else if (pid > 0) {
        waitpid(pid, nullptr, 0);
    }
    else {
        cerr << "Failed to fork" << endl;
    }
}

int main() {
    //cout << "=== User Application ===" << endl;

    string proc_to_kill = "gedit,gnome-calculator";
    setenv("PROC_TO_KILL", proc_to_kill.c_str(), 1);
    cout << "Set PROC_TO_KILL=" << proc_to_kill << endl;

    cout << "Starting test processes..." << endl;
    system("gedit &");
    system("gnome-calculator &");
    sleep(2);

    cout << "\nTest 1: Kill by PID" << endl;
    run_killer("--name gedit");

    cout << "\nTest 2: Kill by name" << endl;
    run_killer("--name gnome-calculator");

    cout << "\nTest 3: Kill via PROC_TO_KILL" << endl;
    system("gedit &");
    system("gnome-calculator &");
    sleep(2);
    run_killer("");

    unsetenv("PROC_TO_KILL");
    cout << "\nRemoved PROC_TO_KILL environment variable" << endl;

    system("pkill gedit 2>/dev/null");
    system("pkill gnome-calculator 2>/dev/null");

    cout << "\nAll tests completed" << endl;
    return 0;
}