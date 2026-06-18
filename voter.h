#ifndef VOTER_H
#define VOTER_H

#include <string>
using namespace std;

/* ---------------------- [voter.h] Data Models ---------------------- */
struct Voter
{
    int voterId;
    string name;
    int age;
    string gender;
    string voterCardNo;
    int stationId;
    bool hasVoted = false;
};

struct PollingStation
{
    int stationId;
    string name;
    string area;
    int capacity;
    int registeredVoters = 0;
    int votesCast = 0;
    int graphNodeId;
};

struct RegistrationRequest
{
    string name;
    int age;
    string gender;
    string voterCardNo;
    int stationId;
};

struct Action
{                // used by the recovery Stack for undo support
    string type; // "REGISTER" or "VOTE"
    int voterId;
    int stationId;
};

#endif
