/* =====================================================================
   SMART ELECTION MANAGEMENT & VOTER ANALYTICS SYSTEM
   B.Tech CSE | DSA with CPP | Modular Multi-Header Build

   This project is organized into separate header files:
     [voter.h]            -> data models
     [filehandler.h]      -> save/load voters & stations (file handling)
     [ds_stack_queue.h]   -> generic Stack<T> / Queue<T>
     [hashing.h]          -> VoterHashTable
     [bst.h]              -> VoterBST
     [avl.h]              -> VoterAVL
     [graph.h]            -> LogisticsGraph (BFS/DFS/Dijkstra)
     [algorithms.h]       -> sorting + searching functions
     [election_system.h]  -> ElectionSystem (business logic + menu)
     [main.cpp]           -> entry point
   ===================================================================== */
#include <iostream>
#include "election_system.h"
using namespace std;

/* ---------------------------- [main.cpp] Entry Point ---------------------------- */
int main()
{
    ElectionSystem app;
    app.run();
    return 0;
}
