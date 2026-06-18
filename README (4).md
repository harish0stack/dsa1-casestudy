# 🗳️ Smart Election Management & Voter Analytics System

A terminal-based, modular C++ application that modernizes election administration using classical Data Structures and Algorithms — built from scratch without STL for core structures.

---

## 📌 Problem Statement

Democratic processes handle millions of voter records and require immense logistical coordination. Managing voter registrations, preventing fraudulent voting, tracking polling station capacities, and establishing efficient transport routes for election materials are critical for conducting fair elections.

This system is designed to streamline election operations through optimized data handling and logistical route planning.

---

## 🎯 Objectives

- Process voter registration requests using **Queue** data structures
- Provide secure, instant voter verification using **Hash Tables**
- Manage election data recovery and undo operations using a **Stack**
- Store voter databases efficiently using **BST** and **AVL Trees**
- Rank polling stations by voter volume using **Merge Sort**
- Retrieve voter information efficiently using **Binary Search**
- Model election logistics and physical routes using **Graphs**, optimizing paths with **Dijkstra's Algorithm**

> **Educational Objective:** Accomplish all of the above using a pure, modular DSA implementation in C++ without relying on STL for core structures.

---

## 🏗️ System Architecture

The system operates as a unified terminal application segmented into highly modular header files:

```
dsa-case-study/
│
├── main_new.cpp           # Central entry point
├── voter.h                # Core structs: Voter, Station, Request
├── ds_stack_queue.h       # Custom Stack<T> and Queue<T> (linked-list based)
├── bst.h                  # Binary Search Tree implementation
├── avl.h                  # Self-balancing AVL Tree
├── hashing.h              # Polynomial rolling hash table (211 buckets)
├── graph.h                # Adjacency list graph
├── algorithms.h           # Merge Sort & Binary Search
├── election_system.h      # Central controller / business logic
├── filehandler.h          # Auto-persistence engine
├── voters.txt             # Pipe-delimited voter data
└── stations.txt           # Pipe-delimited station data
```

---

## 🧩 Data Structures & Algorithms

| Component | Structure / Algorithm | Complexity |
|---|---|---|
| Voter Registration | Queue (FIFO) | Enqueue: `O(1)` |
| Undo Operations | Stack (LIFO) | Pop: `O(1)` |
| Identity Verification | Hash Table (Polynomial Rolling Hash) | Lookup: `O(1)` avg |
| Voter Ledger | BST + AVL Tree | Insert/Search: `O(log N)` |
| Logistics Network | Graph (Adjacency List) | — |
| Shortest Route | Dijkstra's Algorithm | `O((V+E) log V)` |
| Network Traversal | BFS & DFS | `O(V+E)` |
| Station Ranking | Merge Sort | `O(N log N)` |
| Voter Retrieval | Binary Search | `O(log N)` |

---

## 🔧 Implementation Highlights

- **Custom Templates** — `Stack<T>` and `Queue<T>` built from scratch using linked lists
- **Custom Hashing** — Polynomial rolling hash distributes voter IDs across a 211-bucket array of vectors
- **Self-Balancing Logic** — AVL tree manually computes balance factors and performs Left/Right rotations
- **Auto-Persistence Engine** — `FileHandler` reads/writes pipe-delimited `.txt` files to preserve data across compilations

---

## ⚙️ Setup & Execution

### Prerequisites
- C++ compiler (`g++` recommended)

### Steps

```bash
# 1. Navigate to the project directory
cd "/Users/harishkumavat/Desktop/dsa-case-study/"

# 2. Compile the source
g++ main_new.cpp -o election_system

# 3. Run the executable
./election_system
```

---

## 🖥️ Sample Walkthrough

### Step 1 — Submit a Registration Request (Queue)

```
Choose option: 1

Enter name: Harish Malviya
Enter age: 20
Enter gender (M/F/O): M
Enter Voter Card No (unique ID proof): VC10013
Enter target Polling Station ID: 3

→ Registration request added to queue.
```

### Step 2 — Process Registration & Verify Identity (Hash Table)

```
Choose option: 2
→ Voter registered successfully. Assigned Voter ID: 1013

Choose option: 6
Enter Voter Card No: VC10013
→ Identity verified via hash lookup. Vote cast successfully for Voter ID 1013.
```

### Step 3 — Undo Last Action (Stack)

```
Choose option: 4
→ Undo: Vote by Voter ID 1013 reverted.

Note: BST/AVL ledgers retain the entry permanently for audit transparency.
```

### Step 4 — Rank Polling Stations (Merge Sort)

```
Choose option: 8

Polling Stations Ranked by Voter Volume (High to Low):
1. Central Park Booth (Central Zone)  — Registered: 6 | Capacity: 1000
2. Sector 12 Booth (North Zone)       — Registered: 3 | Capacity: 800
3. Riverside Hall (East Zone)         — Registered: 3 | Capacity: 600
```

### Step 5 — Shortest Logistics Route (Dijkstra)

```
Choose option: 10 → 3 (Dijkstra from HQ)

Shortest distance from Election HQ:
  Election HQ        →  0 km
  Sector 12 Booth    → 12 km
  Riverside Hall     → 20 km
  Central Park Booth → 20 km
  Hillview School    → 30 km
```

---

## 📊 Results & Observations

- The **Hash Table** provided instantaneous O(1) verification and successfully prevented duplicate votes
- The **Stack** reliably reverted both registration counts and vote casts without corrupting the historical BST/AVL ledger
- **Dijkstra's Algorithm** correctly calculated optimal dispatch distances accounting for variable edge weights (km)
- The **modular architecture** proved highly maintainable — all data structures interact seamlessly via `election_system.h`

---

## 📚 Conclusion

The Smart Election Management & Voter Analytics System demonstrates how classical DSA can modernize real-world civic administration:

- **Queues** → Fairness in registration processing
- **Stacks** → Safe, reversible administrative actions
- **Hash Tables** → Instantaneous, fraud-proof identity checks
- **AVL Trees** → A tamper-evident, balanced voter ledger
- **Graphs + Dijkstra** → Optimal logistics routing
- **Merge Sort** → Dynamic station prioritization

> Built as a comprehensive demonstration of memory-efficient data management and modular C++ design.
