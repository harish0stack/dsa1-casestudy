#ifndef GRAPH_H
#define GRAPH_H

#include <vector>
#include <string>
#include <queue>
#include <climits>
using namespace std;

/* ------------- [graph.h] Logistics Graph: BFS, DFS, Dijkstra ------------- */
class LogisticsGraph
{
    vector<vector<pair<int, int>>> adj; // adjacency list: {neighbor, distanceKm}
    vector<string> names;

public:
    int addNode(string name)
    {
        adj.push_back({});
        names.push_back(name);
        return (int)names.size() - 1;
    }
    void addRoute(int u, int v, int distKm)
    {
        adj[u].push_back({v, distKm});
        adj[v].push_back({u, distKm});
    }
    int size() { return (int)names.size(); }
    string name(int id) { return names[id]; }

    vector<int> bfs(int src)
    {
        vector<bool> vis(size(), false);
        vector<int> order;
        queue<int> q;
        q.push(src);
        vis[src] = true;
        while (!q.empty())
        {
            int u = q.front();
            q.pop();
            order.push_back(u);
            for (size_t i = 0; i < adj[u].size(); i++)
            {
                int v = adj[u][i].first;
                if (!vis[v])
                {
                    vis[v] = true;
                    q.push(v);
                }
            }
        }
        return order;
    }
    void dfsUtil(int u, vector<bool> &vis, vector<int> &order)
    {
        vis[u] = true;
        order.push_back(u);
        for (size_t i = 0; i < adj[u].size(); i++)
        {
            int v = adj[u][i].first;
            if (!vis[v])
                dfsUtil(v, vis, order);
        }
    }
    vector<int> dfs(int src)
    {
        vector<bool> vis(size(), false);
        vector<int> order;
        dfsUtil(src, vis, order);
        return order;
    }
    vector<int> dijkstra(int src)
    {
        vector<int> dist(size(), INT_MAX);
        dist[src] = 0;
        priority_queue<pair<int, int>, vector<pair<int, int>>, greater<pair<int, int>>> pq;
        pq.push({0, src});
        while (!pq.empty())
        {
            pair<int, int> top = pq.top();
            pq.pop();
            int d = top.first, u = top.second;
            if (d > dist[u])
                continue;
            for (size_t i = 0; i < adj[u].size(); i++)
            {
                int v = adj[u][i].first, w = adj[u][i].second;
                if (dist[u] + w < dist[v])
                {
                    dist[v] = dist[u] + w;
                    pq.push({dist[v], v});
                }
            }
        }
        return dist;
    }
};

#endif
