#include <iostream>
#include <iomanip>
#include <string>
#include <algorithm>
#include <vector>
#include <random>
using namespace std;

struct Process {
    int processID, arrivalTime, burstTime, priority;
    int remainingTime, completionTime, waitingTime, turnaroundTime;
    bool isCompleted;

    Process(int pid = 0) {
        processID = pid;
        arrivalTime = burstTime = priority = remainingTime = 0;
        completionTime = waitingTime = turnaroundTime = 0;
        isCompleted = false;
    }
};

class SchedulerBase {
protected:
    Process* processes;
    int n;
    vector<int> ganttStart, ganttPid;

public:
    SchedulerBase(Process* p, int count) {
        n = count;
        processes = new Process[n];
        for (int i = 0; i < n; ++i) processes[i] = p[i];
    }

    virtual ~SchedulerBase() { delete[] processes; }

    virtual void run() = 0;

    void printTable() {
        cout << "\nProcess Execution Table:\n";
        cout << left << setw(12) << "Process ID"
             << setw(15) << "Arrival"
             << setw(15) << "Burst"
             << setw(12) << "Priority"
             << setw(18) << "Completion"
             << setw(18) << "Turnaround"
             << setw(15) << "Waiting" << "\n";
        for (int i = 0; i < n; ++i) {
            Process& p = processes[i];
            cout << left << setw(12) << ("P" + to_string(p.processID))
                 << setw(15) << p.arrivalTime
                 << setw(15) << p.burstTime
                 << setw(12) << p.priority
                 << setw(18) << p.completionTime
                 << setw(18) << p.turnaroundTime
                 << setw(15) << p.waitingTime << "\n";
        }
    }

    void printGanttChart() {
        cout << "\nGantt Chart:\n ";
        for (size_t i = 0; i < ganttPid.size(); ++i) cout << "---------";
        cout << "\n|";
        for (int pid : ganttPid) cout << "   P" << pid << "   |";
        cout << "\n ";
        for (size_t i = 0; i < ganttPid.size(); ++i) cout << "---------";

        cout << "\n" << setw(9) << ganttStart[0];
        for (size_t i = 0; i < ganttStart.size(); ++i) {
            int next = (i + 1 < ganttStart.size()) ? ganttStart[i + 1] : getMaxCompletion();
            cout << setw(9) << next;
        }
        cout << "\n";
    }

    int getMaxCompletion() const {
        int maxCT = 0;
        for (int i = 0; i < n; ++i)
            if (processes[i].completionTime > maxCT)
                maxCT = processes[i].completionTime;
        return maxCT;
    }
};

class FCFS : public SchedulerBase {
public:
    FCFS(Process* p, int n) : SchedulerBase(p, n) {}

    void run() override {
        sort(processes, processes + n, [](Process a, Process b) {
            return a.arrivalTime < b.arrivalTime;
        });

        int currentTime = 0;
        for (int i = 0; i < n; ++i) {
            Process& p = processes[i];
            currentTime = max(currentTime, p.arrivalTime);
            ganttStart.push_back(currentTime);
            ganttPid.push_back(p.processID);

            currentTime += p.burstTime;
            p.completionTime = currentTime;
            p.turnaroundTime = p.completionTime - p.arrivalTime;
            p.waitingTime = p.turnaroundTime - p.burstTime;
        }
    }
};

class SJF : public SchedulerBase {
public:
    SJF(Process* p, int n) : SchedulerBase(p, n) {}

    void run() override {
        int currentTime = 0, completed = 0;
        while (completed < n) {
            int idx = -1, minBT = 9999;
            for (int i = 0; i < n; ++i) {
                Process& p = processes[i];
                if (!p.isCompleted && p.arrivalTime <= currentTime && p.burstTime < minBT) {
                    minBT = p.burstTime;
                    idx = i;
                }
            }
            if (idx == -1) { currentTime++; continue; }

            Process& p = processes[idx];
            ganttStart.push_back(currentTime);
            ganttPid.push_back(p.processID);

            currentTime += p.burstTime;
            p.completionTime = currentTime;
            p.turnaroundTime = p.completionTime - p.arrivalTime;
            p.waitingTime = p.turnaroundTime - p.burstTime;
            p.isCompleted = true;
            completed++;
        }
    }
};

class PreemptiveSJF : public SchedulerBase {
public:
    PreemptiveSJF(Process* p, int n) : SchedulerBase(p, n) {}

    void run() override {
        int currentTime = 0, completed = 0, lastPid = -1;
        for (int i = 0; i < n; ++i) processes[i].remainingTime = processes[i].burstTime;

        while (completed < n) {
            int idx = -1, minRT = 9999;
            for (int i = 0; i < n; ++i) {
                Process& p = processes[i];
                if (!p.isCompleted && p.arrivalTime <= currentTime && p.remainingTime < minRT && p.remainingTime > 0) {
                    minRT = p.remainingTime;
                    idx = i;
                }
            }
            if (idx == -1) { currentTime++; lastPid = -1; continue; }

            Process& p = processes[idx];
            if (lastPid != p.processID) {
                ganttStart.push_back(currentTime);
                ganttPid.push_back(p.processID);
                lastPid = p.processID;
            }

            p.remainingTime--;
            currentTime++;
            if (p.remainingTime == 0) {
                p.completionTime = currentTime;
                p.turnaroundTime = p.completionTime - p.arrivalTime;
                p.waitingTime = p.turnaroundTime - p.burstTime;
                p.isCompleted = true;
                completed++;
            }
        }
    }
};

class PriorityNonPreemptive : public SchedulerBase {
public:
    PriorityNonPreemptive(Process* p, int n) : SchedulerBase(p, n) {}

    void run() override {
        int currentTime = 0, completed = 0;
        while (completed < n) {
            int idx = -1, highestPriority = 9999;
            for (int i = 0; i < n; ++i) {
                Process& p = processes[i];
                if (!p.isCompleted && p.arrivalTime <= currentTime && p.priority < highestPriority) {
                    highestPriority = p.priority;
                    idx = i;
                }
            }
            if (idx == -1) { currentTime++; continue; }

            Process& p = processes[idx];
            ganttStart.push_back(currentTime);
            ganttPid.push_back(p.processID);

            currentTime += p.burstTime;
            p.completionTime = currentTime;
            p.turnaroundTime = p.completionTime - p.arrivalTime;
            p.waitingTime = p.turnaroundTime - p.burstTime;
            p.isCompleted = true;
            completed++;
        }
    }
};

class PriorityPreemptive : public SchedulerBase {
public:
    PriorityPreemptive(Process* p, int n) : SchedulerBase(p, n) {}

    void run() override {
        int currentTime = 0, completed = 0, lastPid = -1;
        for (int i = 0; i < n; ++i) processes[i].remainingTime = processes[i].burstTime;

        while (completed < n) {
            int idx = -1, highestPriority = 9999;
            for (int i = 0; i < n; ++i) {
                Process& p = processes[i];
                if (!p.isCompleted && p.arrivalTime <= currentTime && p.remainingTime > 0 && p.priority < highestPriority) {
                    highestPriority = p.priority;
                    idx = i;
                }
            }

            if (idx == -1) { currentTime++; lastPid = -1; continue; }

            Process& p = processes[idx];
            if (lastPid != p.processID) {
                ganttStart.push_back(currentTime);
                ganttPid.push_back(p.processID);
                lastPid = p.processID;
            }

            p.remainingTime--;
            currentTime++;

            if (p.remainingTime == 0) {
                p.completionTime = currentTime;
                p.turnaroundTime = p.completionTime - p.arrivalTime;
                p.waitingTime = p.turnaroundTime - p.burstTime;
                p.isCompleted = true;
                completed++;
            }
        }
    }
};

int main() {
    int n, choice;
    cout << "Enter number of processes: ";
    cin >> n;

    cout << "\nSelect Scheduling Algorithm:\n";
    cout << "1. FCFS\n2. SJF (Non-Preemptive)\n3. SJF (Preemptive)\n";
    cout << "4. Priority (Non-Preemptive)\n5. Priority (Preemptive)\n";
    cout << "Enter choice: ";
    cin >> choice;

    // Create processes
    Process* processes = new Process[n];
    for (int i = 0; i < n; ++i) {
        processes[i].processID = i + 1;
        processes[i].arrivalTime = i; // Sequential arrivals
        processes[i].burstTime = rand() % 10 + 1;

        cout << "\nFor P" << processes[i].processID << ":\n";
        cout << "Arrival Time: " << processes[i].arrivalTime << endl;
        cout << "Burst Time: " << processes[i].burstTime << endl;

        // Ask for priority only if needed
        if (choice == 4 || choice == 5) {
            cout << "Priority: ";
            cin >> processes[i].priority;
        } else {
            processes[i].priority = 0; // Set default priority
        }
    }

    // Create appropriate scheduler
    SchedulerBase* scheduler = nullptr;
    switch (choice) {
        case 1: scheduler = new FCFS(processes, n); break;
        case 2: scheduler = new SJF(processes, n); break;
        case 3: scheduler = new PreemptiveSJF(processes, n); break;
        case 4: scheduler = new PriorityNonPreemptive(processes, n); break;
        case 5: scheduler = new PriorityPreemptive(processes, n); break;
        default: cout << "Invalid choice!\n"; delete[] processes; return 0;
    }

    scheduler->run();
    scheduler->printTable();
    scheduler->printGanttChart();

    delete scheduler;
    delete[] processes;
    return 0;
}
