#include <iostream>
#include <vector>
#include <queue>
#include <algorithm>
#include <iomanip>
using namespace std;

struct Process {
    string pid;
    int arrival;
    int burst;
    int completion;
    int turnaround;
    int waiting;
    int remaining;
    int finished;
    int originalOrder;
};

struct GanttEntry {
    string pid;
    int start;
    int end;
};

bool compareArrival(Process a, Process b) {
    if (a.arrival == b.arrival)
        return a.originalOrder < b.originalOrder;
    return a.arrival < b.arrival;
}

void mergeGantt(vector<GanttEntry> &gantt) {
    if (gantt.empty()) return;

    vector<GanttEntry> merged;
    merged.push_back(gantt[0]);

    for (int i = 1; i < gantt.size(); i++) {
        if (gantt[i].pid == merged.back().pid && gantt[i].start == merged.back().end) {
            merged.back().end = gantt[i].end;
        } else {
            merged.push_back(gantt[i]);
        }
    }

    gantt = merged;
}

void printGantt(vector<GanttEntry> gantt) {
    mergeGantt(gantt);

    cout << "\nGantt Chart:\n";
    for (auto g : gantt) {
        cout << "| " << g.pid << " ";
    }
    cout << "|\n";

    if (!gantt.empty()) {
        cout << gantt[0].start;
        for (auto g : gantt) {
            cout << "\t" << g.end;
        }
        cout << "\n";
    }
}

void printTable(vector<Process> processes) {
    sort(processes.begin(), processes.end(), compareArrival);

    double totalWT = 0, totalTAT = 0;

    cout << "\nPID\tAT\tBT\tCT\tTAT\tWT\n";
    for (auto p : processes) {
        cout << p.pid << "\t" << p.arrival << "\t" << p.burst << "\t"
             << p.completion << "\t" << p.turnaround << "\t" << p.waiting << "\n";
        totalWT += p.waiting;
        totalTAT += p.turnaround;
    }

    cout << fixed << setprecision(2);
    cout << "\nAverage Waiting Time = " << totalWT / processes.size() << endl;
    cout << "Average Turnaround Time = " << totalTAT / processes.size() << endl;
}

void fcfs(vector<Process> processes) {
    cout << "\n================ FCFS Scheduling ================\n";

    sort(processes.begin(), processes.end(), compareArrival);
    vector<GanttEntry> gantt;
    int currentTime = 0;

    for (int i = 0; i < processes.size(); i++) {
        if (currentTime < processes[i].arrival) {
            gantt.push_back({"Idle", currentTime, processes[i].arrival});
            currentTime = processes[i].arrival;
        }

        int startTime = currentTime;
        currentTime += processes[i].burst;

        processes[i].completion = currentTime;
        processes[i].turnaround = processes[i].completion - processes[i].arrival;
        processes[i].waiting = processes[i].turnaround - processes[i].burst;

        gantt.push_back({processes[i].pid, startTime, currentTime});
    }

    printGantt(gantt);
    printTable(processes);
}

void sjf(vector<Process> processes) {
    cout << "\n================ SJF Scheduling ================\n";

    int n = processes.size();
    vector<GanttEntry> gantt;
    int completed = 0, currentTime = 0;

    for (int i = 0; i < n; i++) {
        processes[i].finished = 0;
    }

    while (completed < n) {
        int idx = -1;
        int minBurst = 1e9;

        for (int i = 0; i < n; i++) {
            if (!processes[i].finished && processes[i].arrival <= currentTime) {
                if (processes[i].burst < minBurst) {
                    minBurst = processes[i].burst;
                    idx = i;
                } else if (processes[i].burst == minBurst) {
                    if (processes[i].arrival < processes[idx].arrival)
                        idx = i;
                    else if (processes[i].arrival == processes[idx].arrival &&
                             processes[i].originalOrder < processes[idx].originalOrder)
                        idx = i;
                }
            }
        }

        if (idx == -1) {
            int nextArrival = 1e9;
            for (int i = 0; i < n; i++) {
                if (!processes[i].finished) {
                    nextArrival = min(nextArrival, processes[i].arrival);
                }
            }
            gantt.push_back({"Idle", currentTime, nextArrival});
            currentTime = nextArrival;
            continue;
        }

        int startTime = currentTime;
        currentTime += processes[idx].burst;

        processes[idx].completion = currentTime;
        processes[idx].turnaround = processes[idx].completion - processes[idx].arrival;
        processes[idx].waiting = processes[idx].turnaround - processes[idx].burst;
        processes[idx].finished = 1;
        completed++;

        gantt.push_back({processes[idx].pid, startTime, currentTime});
    }

    printGantt(gantt);
    printTable(processes);
}

void sjf_preemptive(vector<Process> processes) {
    cout << "\n============ Preemptive SJF (SRTF) ============\n";

    int n = processes.size();
    vector<GanttEntry> gantt;

    for (int i = 0; i < n; i++) {
        processes[i].remaining = processes[i].burst;
        processes[i].finished = 0;
    }

    int completed = 0, currentTime = 0;

    while (completed < n) {
        int idx = -1;
        int minRemain = 1e9;

        for (int i = 0; i < n; i++) {
            if (processes[i].arrival <= currentTime && processes[i].remaining > 0) {
                if (processes[i].remaining < minRemain) {
                    minRemain = processes[i].remaining;
                    idx = i;
                } else if (processes[i].remaining == minRemain) {
                    if (processes[i].arrival < processes[idx].arrival)
                        idx = i;
                    else if (processes[i].arrival == processes[idx].arrival &&
                             processes[i].originalOrder < processes[idx].originalOrder)
                        idx = i;
                }
            }
        }

        if (idx == -1) {
            gantt.push_back({"Idle", currentTime, currentTime + 1});
            currentTime++;
            continue;
        }

        gantt.push_back({processes[idx].pid, currentTime, currentTime + 1});
        processes[idx].remaining--;
        currentTime++;

        if (processes[idx].remaining == 0) {
            processes[idx].completion = currentTime;
            processes[idx].turnaround = processes[idx].completion - processes[idx].arrival;
            processes[idx].waiting = processes[idx].turnaround - processes[idx].burst;
            completed++;
        }
    }

    printGantt(gantt);
    printTable(processes);
}

void roundRobin(vector<Process> processes, int quantum) {
    cout << "\n============ Round Robin Scheduling ============\n";

    int n = processes.size();
    sort(processes.begin(), processes.end(), compareArrival);

    vector<GanttEntry> gantt;
    queue<int> q;
    vector<int> inQueue(n, 0);

    for (int i = 0; i < n; i++) {
        processes[i].remaining = processes[i].burst;
    }

    int currentTime = 0, completed = 0, i = 0;

    while (completed < n) {
        while (i < n && processes[i].arrival <= currentTime) {
            if (!inQueue[i] && processes[i].remaining > 0) {
                q.push(i);
                inQueue[i] = 1;
            }
            i++;
        }

        if (q.empty()) {
            if (i < n) {
                gantt.push_back({"Idle", currentTime, processes[i].arrival});
                currentTime = processes[i].arrival;
                continue;
            }
        } else {
            int idx = q.front();
            q.pop();

            int startTime = currentTime;
            int execTime = min(quantum, processes[idx].remaining);

            currentTime += execTime;
            processes[idx].remaining -= execTime;

            gantt.push_back({processes[idx].pid, startTime, currentTime});

            while (i < n && processes[i].arrival <= currentTime) {
                if (!inQueue[i] && processes[i].remaining > 0) {
                    q.push(i);
                    inQueue[i] = 1;
                }
                i++;
            }

            if (processes[idx].remaining > 0) {
                q.push(idx);
            } else {
                processes[idx].completion = currentTime;
                processes[idx].turnaround = processes[idx].completion - processes[idx].arrival;
                processes[idx].waiting = processes[idx].turnaround - processes[idx].burst;
                completed++;
            }
        }
    }

    printGantt(gantt);
    printTable(processes);
}

int main() {
    int n;
    cout << "Enter number of processes: ";
    cin >> n;

    vector<Process> processes(n);

    cout << "Enter Process ID, Arrival Time, Burst Time:\n";
    for (int i = 0; i < n; i++) {
        cin >> processes[i].pid >> processes[i].arrival >> processes[i].burst;
        processes[i].completion = 0;
        processes[i].turnaround = 0;
        processes[i].waiting = 0;
        processes[i].remaining = processes[i].burst;
        processes[i].finished = 0;
        processes[i].originalOrder = i;
    }

    int quantum;
    cout << "Enter Time Quantum for Round Robin: ";
    cin >> quantum;

    fcfs(processes);
    sjf(processes);
    sjf_preemptive(processes);
    roundRobin(processes, quantum);

    return 0;
}