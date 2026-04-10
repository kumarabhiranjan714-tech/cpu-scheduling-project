# cpu-scheduling-project
CPU Scheduling Simulator implementing FCFS, SJF, Preemptive SJF (SRTF) and Round Robin algorithms with Gantt Chart, Waiting Time and Turnaround Time calculation.
# 🚀 CPU Scheduling Simulator

This project is a **CPU Scheduling Simulator** developed in **C++** as part of an Operating Systems lab assignment.
It demonstrates how different CPU scheduling algorithms work and compares their performance.

---

## 📌 Features

* ✅ First Come First Serve (FCFS)
* ✅ Shortest Job First (SJF - Non Preemptive)
* ✅ Preemptive SJF (Shortest Remaining Time First - SRTF)
* ✅ Round Robin (RR)

---

## 📥 Input

The program takes the following inputs:

* Process ID
* Arrival Time (AT)
* Burst Time (BT)
* Time Quantum (for Round Robin)

### Example Input:

```
4
P1 0 5
P2 1 3
P3 2 1
P4 3 2
2
```

---

## 📤 Output

For each scheduling algorithm, the program displays:

* 📊 Gantt Chart
* ⏱ Completion Time (CT)
* 🔄 Turnaround Time (TAT)
* ⌛ Waiting Time (WT)
* 📈 Average Waiting Time
* 📈 Average Turnaround Time

---

## 🧠 Algorithms Used

### 1. FCFS (First Come First Serve)

* Executes processes in order of arrival
* Simple but may lead to higher waiting time

### 2. SJF (Shortest Job First)

* Selects process with smallest burst time
* Reduces average waiting time

### 3. Preemptive SJF (SRTF)

* Chooses process with shortest remaining time
* More efficient but involves context switching

### 4. Round Robin

* Each process gets fixed time quantum
* Fair scheduling, suitable for time-sharing systems

---

## ⚙️ How to Run

### Step 1: Compile the code

```
g++ cpu_scheduling.cpp -o cpu
```

### Step 2: Run the program

```
./cpu
```

### Step 3: Enter input as shown in example

---

## 📊 Sample Output

*(Add your screenshots here for better presentation)*

---

## 📈 Comparison of Algorithms

| Algorithm   | Average Waiting Time | Average Turnaround Time |
| ----------- | -------------------- | ----------------------- |
| FCFS        | (your output)        | (your output)           |
| SJF         | (your output)        | (your output)           |
| SRTF        | (your output)        | (your output)           |
| Round Robin | (your output)        | (your output)           |

---

## ⚠️ Edge Cases Handled

* Processes arriving at the same time
* CPU idle condition when no process is available
* Proper queue handling in Round Robin

---

## 🎯 Objective

To understand the working of CPU scheduling algorithms and analyze their performance based on waiting time and turnaround time.

---

## 💻 Technologies Used

* Language: **C++**
* Concepts: **Operating Systems, Scheduling Algorithms**

---

## 👨‍💻 Author

**Abhiranjan Kumar**

---

## 📌 Note

This project is developed for academic purposes to demonstrate CPU scheduling concepts in Operating Systems.
