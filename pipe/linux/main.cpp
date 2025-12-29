#include <iostream>
#include <unistd.h>
#include <sys/wait.h>
#include <cstdlib>
#include <string>

using namespace std;

void create_process_chain() {
    int pipeMA[2], pipeAP[2], pipePS[2];

    if (pipe(pipeMA) == -1 || pipe(pipeAP) == -1 || pipe(pipePS) == -1) {
        perror("pipe");
        exit(1);
    }

    pid_t pidM = fork();
    if (pidM == 0) {
        dup2(pipeMA[1], STDOUT_FILENO);
        close(pipeMA[0]);
        close(pipeMA[1]);
        close(pipeAP[0]);
        close(pipeAP[1]);
        close(pipePS[0]);
        close(pipePS[1]);
        execl("./M", "./M", NULL);
        perror("execl M");
        exit(1);
    }

    pid_t pidA = fork();
    if (pidA == 0) {
        dup2(pipeMA[0], STDIN_FILENO);
        dup2(pipeAP[1], STDOUT_FILENO);
        close(pipeMA[0]);
        close(pipeMA[1]);
        close(pipeAP[0]);
        close(pipeAP[1]);
        close(pipePS[0]);
        close(pipePS[1]);
        execl("./A", "./A", NULL);
        perror("execl A");
        exit(1);
    }

    pid_t pidP = fork();
    if (pidP == 0) {
        dup2(pipeAP[0], STDIN_FILENO);
        dup2(pipePS[1], STDOUT_FILENO);
        close(pipeMA[0]);
        close(pipeMA[1]);
        close(pipeAP[0]);
        close(pipeAP[1]);
        close(pipePS[0]);
        close(pipePS[1]);
        execl("./P", "./P", NULL);
        perror("execl P");
        exit(1);
    }

    pid_t pidS = fork();
    if (pidS == 0) {
        dup2(pipePS[0], STDIN_FILENO);
        close(pipeMA[0]);
        close(pipeMA[1]);
        close(pipeAP[0]);
        close(pipeAP[1]);
        close(pipePS[0]);
        close(pipePS[1]);
        execl("./S", "./S", NULL);
        perror("execl S");
        exit(1);
    }

    close(pipeMA[0]);
    close(pipeMA[1]);
    close(pipeAP[0]);
    close(pipeAP[1]);
    close(pipePS[0]);
    close(pipePS[1]);

    waitpid(pidM, NULL, 0);
    waitpid(pidA, NULL, 0);
    waitpid(pidP, NULL, 0);
    waitpid(pidS, NULL, 0);
}

int main() {
    cout << "Enter numbers separated by spaces: ";
    create_process_chain();
    return 0;
}