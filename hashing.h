#ifndef HASHING_H
#define HASHING_H

#include <vector>
#include <string>
using namespace std;

/* ---------------- [hashing.h] Voter Verification Hash Table ---------------- */
class VoterHashTable
{
    static const int SIZE = 211; // prime bucket count
    vector<vector<pair<string, int>>> table;
    int hashFunc(string key)
    {
        long long h = 0;
        for (size_t i = 0; i < key.size(); i++)
            h = (h * 31 + (int)key[i]) % SIZE;
        return (int)h;
    }

public:
    VoterHashTable() : table(SIZE) {}
    void insert(string key, int voterId) { table[hashFunc(key)].push_back({key, voterId}); }
    int verify(string key)
    { // returns voterId, or -1 if not found
        vector<pair<string, int>> &bucket = table[hashFunc(key)];
        for (size_t i = 0; i < bucket.size(); i++)
            if (bucket[i].first == key)
                return bucket[i].second;
        return -1;
    }
    void remove(string key)
    {
        vector<pair<string, int>> &bucket = table[hashFunc(key)];
        for (size_t i = 0; i < bucket.size(); i++)
            if (bucket[i].first == key)
            {
                bucket.erase(bucket.begin() + i);
                return;
            }
    }
};

#endif
