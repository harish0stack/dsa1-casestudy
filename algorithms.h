#ifndef ALGORITHMS_H
#define ALGORITHMS_H

#include <vector>
#include "voter.h"
using namespace std;

/* --------------- [algorithms.h] Sorting & Searching --------------- */
void mergeStations(vector<PollingStation> &v, int l, int m, int r)
{ // merge step, descending by voter volume
    vector<PollingStation> tmp;
    int i = l, j = m + 1;
    while (i <= m && j <= r)
    {
        if (v[i].registeredVoters >= v[j].registeredVoters)
            tmp.push_back(v[i++]);
        else
            tmp.push_back(v[j++]);
    }
    while (i <= m)
        tmp.push_back(v[i++]);
    while (j <= r)
        tmp.push_back(v[j++]);
    for (int k = l; k <= r; k++)
        v[k] = tmp[k - l];
}

void mergeSortStations(vector<PollingStation> &v, int l, int r)
{ // ranks stations by voter volume
    if (l >= r)
        return;
    int m = (l + r) / 2;
    mergeSortStations(v, l, m);
    mergeSortStations(v, m + 1, r);
    mergeStations(v, l, m, r);
}

int linearSearchVoter(vector<Voter> &v, int id, int *comparisons = nullptr)
{
    int count = 0;
    for (int i = 0; i < (int)v.size(); i++)
    {
        count++;
        if (v[i].voterId == id)
        {
            if (comparisons)
                *comparisons = count;
            return i;
        }
    }
    if (comparisons)
        *comparisons = count;
    return -1;
}

int binarySearchVoter(vector<Voter> &v, int id, int *comparisons = nullptr)
{ // v must be sorted ascending by voterId
    int lo = 0, hi = (int)v.size() - 1, count = 0;
    while (lo <= hi)
    {
        count++;
        int mid = (lo + hi) / 2;
        if (v[mid].voterId == id)
        {
            if (comparisons)
                *comparisons = count;
            return mid;
        }
        else if (v[mid].voterId < id)
            lo = mid + 1;
        else
            hi = mid - 1;
    }
    if (comparisons)
        *comparisons = count;
    return -1;
}

#endif
