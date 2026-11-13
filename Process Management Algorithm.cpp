#include <iostream>
#include <vector>
#include <string>
#include <chrono>
using namespace std;

class processManagement {

public:

    struct Process {
        string name;
        int size;
        bool isRunning;
        chrono::steady_clock::time_point lastActivity;
    };

    vector<Process> processes;

    // memoryManger* memory;

    // processManagement(memoryManger* mem) {
    //  memory = mem;
    // }

    void addProcess() {

        Process p;

        cout << "Enter process name: ";
        cin >> p.name;
        cout << "Enter process size: ";
        cin >> p.size;
        if (p.size <= 0) {
            cout << "Invalid size!" << endl;
            return;
        }

        p.isRunning = true;
        p.lastActivity = chrono::steady_clock::now();
        processes.push_back(p);
        cout << "Process added successfully!" << endl;

    }

    void updateActivity(string processName) {

        for (int i = 0; i < processes.size(); i++) {
            if (processes[i].name == processName && processes[i].isRunning) {
                processes[i].lastActivity = chrono::steady_clock::now();
                cout << "Activity updated!" << endl;
                return;
            }
        }

        cout << "Process not found or not running!" << endl;

    }

    void terminateProcess(string processName) {

        for (int i = 0; i < processes.size(); i++) {
            if (processes[i].name == processName && processes[i].isRunning) {
                processes[i].isRunning = false;
                // memory->freeMemory(processName);
                cout << "Process terminated successfully!" << endl;
                return;
            }
        }

        cout << "Process not found or already terminated!" << endl;

    }

    void inactivityCleanup() {

        auto currentTime = chrono::steady_clock::now();

        for (int i = 0; i < processes.size(); i++) {
            if (processes[i].isRunning) {
                auto idleTime = currentTime - processes[i].lastActivity;

                if (idleTime > chrono::minutes(30)) {
                    string processName = processes[i].name;
                    cout << processName << " terminated due to inactivity!" << endl;

                    terminateProcess(processName);
                }
            }
        }

    }

};


