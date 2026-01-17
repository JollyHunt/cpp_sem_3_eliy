#ifndef DIJKSTRA_H
#define DIJKSTRA_H

#include "../UGraph.h"
#include <functional>
#include <queue>
#include <map>
#include <limits>

using EdgeFilter = std::function<bool(const CommonEdge*)>;

inline ArraySequence<CommonEdge*> dijkstra(
    const UGraph& graph,
    const CommonVertex& start,
    const CommonVertex& end,
    EdgeFilter filter = nullptr) {

    std::map<CommonVertex, double> dist;
    std::map<CommonVertex, std::pair<CommonVertex, CommonEdge*>> parent;

    auto cmp = [](const std::pair<double, CommonVertex>& a,
                  const std::pair<double, CommonVertex>& b) {
        return a.first > b.first;
    };
    std::priority_queue<std::pair<double, CommonVertex>,
                        std::vector<std::pair<double, CommonVertex>>,
                        decltype(cmp)> pq(cmp);

    dist[start] = 0.0;
    pq.push({0.0, start});

    while (!pq.empty()) {
        auto [d, u] = pq.top();
        pq.pop();

        if (dist.find(u) == dist.end() || dist[u] < d) continue;
        if (u.getId() == end.getId()) break;

        auto edges = graph.getEdgesOf(u);
        for (size_t i = 0; i < edges.Size(); ++i) {
            CommonEdge* e = edges.Get(i);

            if (filter && !filter(e)) continue;

            CommonVertex v = (e->getFrom().getId() == u.getId()) ? e->getTo() : e->getFrom();
            double w = e->getWeight();

            if ( w > 1e9 || w < 0) continue;

            if (dist.find(v) == dist.end() || dist[u] + w < dist[v]) {
                dist[v] = dist[u] + w;
                parent[v] = {u, e};
                pq.push({dist[v], v});
            }
        }
    }

    ArraySequence<CommonEdge*> path;
    if (dist.find(end) == dist.end()) return path;

    CommonVertex cur = end;
    while (!(cur.getId() == start.getId())) {
        auto it = parent.find(cur);
        if (it == parent.end()) break;
        path.Prepend(it->second.second);
        cur = it->second.first;
    }

    return path;
}

#endif