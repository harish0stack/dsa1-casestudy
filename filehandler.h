#ifndef FILEHANDLER_H
#define FILEHANDLER_H

#include <fstream>
#include <sstream>
#include <vector>
#include "voter.h"
using namespace std;

/* ---------- [filehandler.h] Save/Load Voter & Station Data to Files ---------- */
class FileHandler
{
public:
    static void saveVoters(vector<Voter> &voters)
    {
        ofstream out("voters.txt");
        for (size_t i = 0; i < voters.size(); i++)
        {
            Voter &v = voters[i];
            out << v.voterId << "|" << v.name << "|" << v.age << "|" << v.gender << "|"
                << v.voterCardNo << "|" << v.stationId << "|" << v.hasVoted << "\n";
        }
    }
    static vector<Voter> loadVoters()
    { // returns empty vector if file doesn't exist yet
        vector<Voter> list;
        ifstream in("voters.txt");
        string line;
        while (getline(in, line))
        {
            if (line.empty())
                continue;
            stringstream ss(line);
            vector<string> f;
            string field;
            while (getline(ss, field, '|'))
                f.push_back(field);
            Voter v{stoi(f[0]), f[1], stoi(f[2]), f[3], f[4], stoi(f[5]), f[6] == "1"};
            list.push_back(v);
        }
        return list;
    }
    static void saveStations(vector<PollingStation> &stations)
    {
        ofstream out("stations.txt");
        for (size_t i = 0; i < stations.size(); i++)
        {
            PollingStation &s = stations[i];
            out << s.stationId << "|" << s.name << "|" << s.area << "|" << s.capacity << "|"
                << s.registeredVoters << "|" << s.votesCast << "|" << s.graphNodeId << "\n";
        }
    }
    static vector<PollingStation> loadStations()
    {
        vector<PollingStation> list;
        ifstream in("stations.txt");
        string line;
        while (getline(in, line))
        {
            if (line.empty())
                continue;
            stringstream ss(line);
            vector<string> f;
            string field;
            while (getline(ss, field, '|'))
                f.push_back(field);
            PollingStation s{stoi(f[0]), f[1], f[2], stoi(f[3]), stoi(f[4]), stoi(f[5]), stoi(f[6])};
            list.push_back(s);
        }
        return list;
    }
};

#endif
