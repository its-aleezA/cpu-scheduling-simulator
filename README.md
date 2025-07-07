# 🧠 CPU Scheduling Simulator (C++)

This project simulates classic CPU scheduling algorithms and prints process execution tables along with Gantt charts for visualization. It’s written in C++ and is useful for OS education, assignment demos, and conceptual reinforcement.

---

## ⚙️ Features

- Implements the following algorithms:
  - 🔹 First-Come, First-Served (FCFS)
  - 🔹 Shortest Job First (SJF) — Preemptive & Non-Preemptive
  - 🔹 Priority Scheduling — Preemptive & Non-Preemptive
- Auto-generates burst times and accepts custom priority inputs
- Prints formatted process tables with:
  - Arrival Time
  - Burst Time
  - Completion Time
  - Turnaround Time
  - Waiting Time
- Gantt chart generation for visualizing process execution timeline

---

## 🚀 How to Run

### 🧱 Build:
        g++ code/cpu_scheduler.cpp -o cpu_scheduler

### ▶️ Execute:
        ./cpu_scheduler

---

## 📝 Output

Displays:
- Execution Table
- Gantt Chart Timeline

---

## 🖼️ Demo

### First Come First Serve
![FCFS](images/fcfs.jpg)

### Priority Non Preemptive
![Priority Non Preemptive](images/priority_nonpreemptive.jpg)

### Priority Preemptive
![Priority Preemptive](images/priority_preemptive.jpg)

### ShortestJob First Non Preemptive
![SJF Non Preemptive](images/sjf_nonpreemptive.jpg)

### ShortestJob First Preemptive
![SJF Preemptive](images/sjf_preemptive.jpg)

---

## 📁 Project Structure

| Path |	Description |
| ----------- | ----------- |
| code/	| Source code of the scheduler |
| images/	| Demonstration screenshots |
| report/	| Expanded documentation |

---

## 👩‍💻 Authors

- [Aleeza Rizwan](https://github.com/its-aleezA)
- [Muhammad Ibrahim Abdullah](https://github.com/Ibrahim5570)
- [Muhammad Shaheer Afzal](https://github.com/ShaheerAfzal)

---

## 🔖 License

This project is licensed under the MIT License.
See the [LICENSE](LICENSE) file for more information.

