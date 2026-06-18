#ifndef ELECTION_SYSTEM_H
#define ELECTION_SYSTEM_H

#include <iostream>
#include <vector>
#include <string>
#include <limits>
#include <iomanip>
#include "voter.h"
#include "ds_stack_queue.h"
#include "hashing.h"
#include "bst.h"
#include "avl.h"
#include "graph.h"
#include "algorithms.h"
#include "filehandler.h"
using namespace std;

/* ------------- [election_system.h] Core Business Logic + Menu ------------- */
class ElectionSystem
{
    vector<Voter> voters; // always kept sorted ascending by voterId (IDs assigned in order)
    vector<PollingStation> stations;
    Stack<Action> recovery;             // data recovery / undo
    Queue<RegistrationRequest> pending; // registration request pipeline
    VoterHashTable verifier;
    VoterBST bst;
    VoterAVL avl;
    LogisticsGraph logistics;
    int nextVoterId = 1001;
    int nextStationId = 1;

    void rebuildIndexes()
    { // after loading voters from file, rebuild BST/AVL/Hash from them
        for (size_t i = 0; i < voters.size(); i++)
        {
            bst.insert(voters[i]);
            avl.insert(voters[i]);
            verifier.insert(voters[i].voterCardNo, voters[i].voterId);
        }
    }
    void rebuildGraph()
    {                                     // after loading stations from file, rebuild graph nodes + fixed routes
        logistics.addNode("Election HQ"); // node 0
        for (size_t i = 0; i < stations.size(); i++)
            logistics.addNode(stations[i].name);
        if (stations.size() >= 4)
        {
            logistics.addRoute(0, 1, 12);
            logistics.addRoute(0, 2, 20);
            logistics.addRoute(1, 3, 8);
            logistics.addRoute(2, 3, 15);
            logistics.addRoute(3, 4, 10);
        }
    }
    void saveAll()
    {
        FileHandler::saveVoters(voters);
        FileHandler::saveStations(stations);
    } // call after every change

    PollingStation *findStation(int id)
    {
        for (size_t i = 0; i < stations.size(); i++)
            if (stations[i].stationId == id)
                return &stations[i];
        return nullptr;
    }
    Voter *findVoterById(int id)
    {
        int idx = binarySearchVoter(voters, id);
        return idx == -1 ? nullptr : &voters[idx];
    }
    void addStation(string name, string area, int capacity)
    {
        int gid = logistics.addNode(name);
        stations.push_back({nextStationId++, name, area, capacity, 0, 0, gid});
    }
    void registerVoterDirect(string name, int age, string gender, string card, int stationId)
    {
        PollingStation *st = findStation(stationId);
        if (!st)
            return;
        Voter v{nextVoterId++, name, age, gender, card, stationId, false};
        voters.push_back(v);
        bst.insert(v);
        avl.insert(v);
        verifier.insert(v.voterCardNo, v.voterId);
        st->registeredVoters++;
    }
    void seedVoter(string name, int age, string gender, string card, int stationId, bool voted)
    {
        registerVoterDirect(name, age, gender, card, stationId);
        if (voted)
        {
            voters.back().hasVoted = true;
            PollingStation *st = findStation(stationId);
            if (st)
                st->votesCast++;
        }
    }
    void seedDemoData()
    {
        logistics.addNode("Election HQ");                       // graph node 0
        addStation("Sector 12 Booth", "North Zone", 800);       // station 1
        addStation("Riverside Hall", "East Zone", 600);         // station 2
        addStation("Central Park Booth", "Central Zone", 1000); // station 3
        addStation("Hillview School", "South Zone", 500);       // station 4
        logistics.addRoute(0, 1, 12);
        logistics.addRoute(0, 2, 20);
        logistics.addRoute(1, 3, 8);
        logistics.addRoute(2, 3, 15);
        logistics.addRoute(3, 4, 10);
        seedVoter("Rahul Sharma", 34, "M", "VC10001", 1, true);
        seedVoter("Priya Nair", 29, "F", "VC10002", 1, false);
        seedVoter("Amit Kumar", 41, "M", "VC10003", 1, false);
        seedVoter("Sneha Patil", 25, "F", "VC10004", 2, true);
        seedVoter("Vikram Singh", 38, "M", "VC10005", 2, false);
        seedVoter("Ananya Rao", 31, "F", "VC10006", 3, true);
        seedVoter("Karthik Iyer", 27, "M", "VC10007", 3, true);
        seedVoter("Divya Menon", 45, "F", "VC10008", 3, false);
        seedVoter("Rohit Verma", 22, "M", "VC10009", 3, false);
        seedVoter("Pooja Joshi", 36, "F", "VC10010", 3, false);
        seedVoter("Manoj Tiwari", 50, "M", "VC10011", 4, false);
    }

public:
    ElectionSystem()
    {
        stations = FileHandler::loadStations();
        voters = FileHandler::loadVoters();
        if (stations.empty())
        { // no save files yet -> first run
            seedDemoData();
            saveAll();
            cout << "No saved data found. Starting with demo dataset (and creating voters.txt / stations.txt).\n";
        }
        else
        {
            rebuildGraph();
            rebuildIndexes();
            for (size_t i = 0; i < stations.size(); i++)
                nextStationId = max(nextStationId, stations[i].stationId + 1);
            for (size_t i = 0; i < voters.size(); i++)
                nextVoterId = max(nextVoterId, voters[i].voterId + 1);
            cout << "Loaded " << voters.size() << " voters and " << stations.size() << " stations from file.\n";
        }
    }

    void requestRegistration()
    {
        RegistrationRequest r;
        cout << "Enter name: ";
        getline(cin, r.name);
        cout << "Enter age: ";
        cin >> r.age;
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
        cout << "Enter gender (M/F/O): ";
        getline(cin, r.gender);
        cout << "Enter Voter Card No (unique ID proof): ";
        getline(cin, r.voterCardNo);
        cout << "Enter target Polling Station ID: ";
        cin >> r.stationId;
        pending.push(r);
        cout << "Registration request added to queue.\n";
    }
    void processNextRegistration()
    {
        if (pending.empty())
        {
            cout << "No pending registration requests.\n";
            return;
        }
        RegistrationRequest r = pending.pop();
        PollingStation *st = findStation(r.stationId);
        if (!st)
        {
            cout << "Invalid polling station ID in request. Request discarded.\n";
            return;
        }
        int assignedId = nextVoterId;
        registerVoterDirect(r.name, r.age, r.gender, r.voterCardNo, r.stationId);
        recovery.push({"REGISTER", assignedId, r.stationId});
        saveAll();
        cout << "Voter registered successfully. Assigned Voter ID: " << assignedId << "\n";
    }
    void viewQueue()
    {
        vector<RegistrationRequest> snap = pending.snapshot();
        if (snap.empty())
        {
            cout << "Registration queue is empty.\n";
            return;
        }
        cout << "\nPending Registration Requests (FIFO order):\n";
        for (size_t i = 0; i < snap.size(); i++)
            cout << i + 1 << ". " << snap[i].name << " (Card:" << snap[i].voterCardNo << ") -> Station " << snap[i].stationId << "\n";
    }
    void undoLastAction()
    {
        if (recovery.empty())
        {
            cout << "No operations available to undo.\n";
            return;
        }
        Action a = recovery.pop();
        PollingStation *st = findStation(a.stationId);
        if (a.type == "REGISTER")
        {
            int idx = binarySearchVoter(voters, a.voterId);
            if (idx != -1)
            {
                verifier.remove(voters[idx].voterCardNo);
                voters.erase(voters.begin() + idx);
            }
            if (st)
                st->registeredVoters--;
            cout << "Undo: Registration of Voter ID " << a.voterId << " reverted.\n";
        }
        else if (a.type == "VOTE")
        {
            Voter *v = findVoterById(a.voterId);
            if (v)
                v->hasVoted = false;
            if (st)
                st->votesCast--;
            cout << "Undo: Vote by Voter ID " << a.voterId << " reverted.\n";
        }
        cout << "Note: BST/AVL ledgers retain the entry permanently for audit transparency.\n";
        saveAll();
    }
    void searchVoterMenu()
    {
        if (voters.empty())
        {
            cout << "No voters in the database yet.\n";
            return;
        }
        int id;
        cout << "Enter Voter ID to search: ";
        cin >> id;
        int choice;
        cout << "1. Linear Search  2. Binary Search\nChoice: ";
        cin >> choice;
        int comparisons = 0;
        int idx = (choice == 1) ? linearSearchVoter(voters, id, &comparisons) : binarySearchVoter(voters, id, &comparisons);
        if (idx == -1)
        {
            cout << "Voter not found. (Comparisons made: " << comparisons << ")\n";
            return;
        }
        Voter &v = voters[idx];
        cout << "Found in " << comparisons << " comparisons.\n";
        cout << "ID: " << v.voterId << " | Name: " << v.name << " | Age: " << v.age
             << " | Gender: " << v.gender << " | Station: " << v.stationId
             << " | Voted: " << (v.hasVoted ? "Yes" : "No") << "\n";
    }
    void verifyAndVote()
    {
        cout << "Enter Voter Card No: ";
        string card;
        getline(cin, card);
        int vid = verifier.verify(card);
        if (vid == -1)
        {
            cout << "Verification failed: no matching voter record.\n";
            return;
        }
        Voter *v = findVoterById(vid);
        if (!v)
        {
            cout << "Data inconsistency: voter record missing.\n";
            return;
        }
        if (v->hasVoted)
        {
            cout << "This voter has already cast their vote.\n";
            return;
        }
        v->hasVoted = true;
        PollingStation *st = findStation(v->stationId);
        if (st)
            st->votesCast++;
        recovery.push({"VOTE", v->voterId, v->stationId});
        saveAll();
        cout << "Identity verified via hash lookup. Vote cast successfully for Voter ID " << v->voterId << ".\n";
    }
    void addStationMenu()
    {
        string name, area;
        int capacity;
        cout << "Station name: ";
        getline(cin, name);
        cout << "Area / Zone: ";
        getline(cin, area);
        cout << "Capacity: ";
        cin >> capacity;
        addStation(name, area, capacity);
        saveAll();
        cout << "Polling station added with ID " << nextStationId - 1 << ".\n";
    }
    void rankStations()
    {
        if (stations.empty())
        {
            cout << "No polling stations available.\n";
            return;
        }
        vector<PollingStation> ranked = stations;
        mergeSortStations(ranked, 0, (int)ranked.size() - 1);
        cout << "\nPolling Stations Ranked by Voter Volume (High to Low):\n";
        for (size_t i = 0; i < ranked.size(); i++)
            cout << i + 1 << ". " << ranked[i].name << " (" << ranked[i].area << ") - Registered: "
                 << ranked[i].registeredVoters << " | Capacity: " << ranked[i].capacity << "\n";
    }
    void addRouteMenu()
    {
        cout << "Available Nodes:\n0. Election HQ\n";
        for (size_t i = 0; i < stations.size(); i++)
            cout << stations[i].graphNodeId << ". " << stations[i].name << "\n";
        int u, v, w;
        cout << "From node ID: ";
        cin >> u;
        cout << "To node ID: ";
        cin >> v;
        cout << "Distance (km): ";
        cin >> w;
        if (u < 0 || u >= logistics.size() || v < 0 || v >= logistics.size() || w <= 0)
        {
            cout << "Invalid input.\n";
            return;
        }
        logistics.addRoute(u, v, w);
        cout << "Route added successfully.\n";
    }
    void logisticsAnalysis()
    {
        cout << "1. BFS from HQ  2. DFS from HQ  3. Dijkstra Shortest Routes from HQ\nChoice: ";
        int c;
        cin >> c;
        if (c == 1)
        {
            vector<int> order = logistics.bfs(0);
            cout << "BFS Order: ";
            for (size_t i = 0; i < order.size(); i++)
                cout << logistics.name(order[i]) << " -> ";
            cout << "END\n";
        }
        else if (c == 2)
        {
            vector<int> order = logistics.dfs(0);
            cout << "DFS Order: ";
            for (size_t i = 0; i < order.size(); i++)
                cout << logistics.name(order[i]) << " -> ";
            cout << "END\n";
        }
        else if (c == 3)
        {
            vector<int> dist = logistics.dijkstra(0);
            cout << "Shortest distance from Election HQ:\n";
            for (int i = 0; i < logistics.size(); i++)
                cout << logistics.name(i) << ": " << (dist[i] == INT_MAX ? -1 : dist[i]) << " km\n";
        }
    }
    void dashboard()
    {
        int totalVoters = (int)voters.size();
        int totalVotes = 0;
        for (size_t i = 0; i < voters.size(); i++)
            if (voters[i].hasVoted)
                totalVotes++;
        cout << "\n========== ELECTION DASHBOARD ==========\n";
        cout << "Total Registered Voters : " << totalVoters << "\n";
        cout << "Total Votes Cast        : " << totalVotes << "\n";
        cout << "Overall Turnout         : " << fixed << setprecision(1) << (totalVoters ? (totalVotes * 100.0 / totalVoters) : 0) << "%\n";
        cout << "Pending Registrations   : " << pending.size() << "\n";
        cout << "Recovery Stack Depth    : " << recovery.size() << "\n";
        cout << "\nPolling Station Overview:\n";
        cout << left << setw(5) << "ID" << setw(20) << "Name" << setw(14) << "Area" << setw(10) << "Capacity" << setw(12) << "Registered" << setw(8) << "Votes" << "Turnout%\n";
        for (size_t i = 0; i < stations.size(); i++)
        {
            PollingStation &s = stations[i];
            double t = s.registeredVoters ? s.votesCast * 100.0 / s.registeredVoters : 0;
            cout << left << setw(5) << s.stationId << setw(20) << s.name << setw(14) << s.area << setw(10) << s.capacity
                 << setw(12) << s.registeredVoters << setw(8) << s.votesCast << fixed << setprecision(1) << t << "%\n";
        }
    }
    void run()
    {
        int choice = -1;
        do
        {
            cout << "\n" << string(73, '=') << "\n"
                 << "║  " << left << setw(69) << "SMART ELECTION MANAGEMENT & VOTER ANALYTICS SYSTEM" << "║\n"
                 << string(73, '=') << "\n"
                 << "║  " << left << setw(69) << "1.  Submit Voter Registration Request   (Queue)" << "║\n"
                 << "║  " << left << setw(69) << "2.  Process Next Registration            (Queue -> BST/AVL/Hash)" << "║\n"
                 << "║  " << left << setw(69) << "3.  View Pending Registration Queue" << "║\n"
                 << "║  " << left << setw(69) << "4.  Undo Last Operation                  (Stack)" << "║\n"
                 << "║  " << left << setw(69) << "5.  Search Voter by ID                   (Linear/Binary Search)" << "║\n"
                 << "║  " << left << setw(69) << "6.  Verify & Cast Vote                    (Hashing)" << "║\n"
                 << "║  " << left << setw(69) << "7.  Add New Polling Station" << "║\n"
                 << "║  " << left << setw(69) << "8.  Rank Polling Stations by Voter Volume (Sorting)" << "║\n"
                 << "║  " << left << setw(69) << "9.  Logistics Network - Add Route" << "║\n"
                 << "║  " << left << setw(69) << "10. Logistics Network - BFS / DFS / Dijkstra" << "║\n"
                 << "║  " << left << setw(69) << "11. Election Dashboard" << "║\n"
                 << "║  " << left << setw(69) << "0.  Exit" << "║\n"
                 << string(73, '=') << "\n"
                 << "Enter choice: ";
            if (!(cin >> choice))
            {
                cin.clear();
                cin.ignore(numeric_limits<streamsize>::max(), '\n');
                choice = -1;
                continue;
            }
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            switch (choice)
            {
            case 1:
                requestRegistration();
                break;
            case 2:
                processNextRegistration();
                break;
            case 3:
                viewQueue();
                break;
            case 4:
                undoLastAction();
                break;
            case 5:
                searchVoterMenu();
                break;
            case 6:
                verifyAndVote();
                break;
            case 7:
                addStationMenu();
                break;
            case 8:
                rankStations();
                break;
            case 9:
                addRouteMenu();
                break;
            case 10:
                logisticsAnalysis();
                break;
            case 11:
                dashboard();
                break;
            case 0:
                cout << "Exiting Election Management System.\n";
                break;
            default:
                cout << "Invalid choice.\n";
            }
        } while (choice != 0);
    }
};

#endif
