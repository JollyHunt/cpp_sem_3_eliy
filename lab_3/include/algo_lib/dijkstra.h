#ifndef DIJKSTRA_H
#define DIJKSTRA_H

#include "../UGraph.h"
#include "../ege/AbstractEdge.h"
#include "../ege/TransportType.h"
#include "../ege/CarEdge.h"
#include "../ege/PlaneEdge.h"
#include "../ege/ShipEdge.h"
#include "../ege/TruckEdge.h"
#include <map>
#include <queue>
#include <functional>

struct State {
    Vertex vertex;
    TransportType transport;

    State() : vertex(), transport(TransportType::SHIP) {}

    State(const Vertex& v, TransportType t) : vertex(v), transport(t) {}

    bool operator<(const State& other) const {
        if (vertex < other.vertex) return true;
        if (other.vertex < vertex) return false;
        return static_cast<int>(transport) < static_cast<int>(other.transport);
    }

    bool operator==(const State& other) const {
        return vertex == other.vertex && transport == other.transport;
    }
};

struct QueueItem {
    double priority;
    State state;
    double totalTime;

    QueueItem(double p, const State& s, double t) : priority(p), state(s), totalTime(t) {}

    bool operator<(const QueueItem& other) const {
        return priority > other.priority;
    }
};

UGraph dijkstra(const UGraph& graph,
                const Vertex& start,
                const Vertex& end,
                double cargoWeight,
                double cargoSize,
                double trafficFactor = 1.0,
                double timeCostMultiplier = 1.0,
                bool optimizeByTime = true) {

    std::map<State, double> dist;
    std::map<State, std::pair<State, AbstractEdge*>> parent;

    std::priority_queue<QueueItem> pq;

    auto startEdges = graph.getEdgesOf(start);
    for (size_t i = 0; i < startEdges.Size(); ++i) {
        AbstractEdge* e = startEdges.Get(i);
        if (!e->canCarry(cargoWeight, cargoSize)) continue;

        double travelTime = e->getActualTime(trafficFactor);
        double travelCost = e->getActualCost(timeCostMultiplier);
        double distance = optimizeByTime ? travelTime : travelCost;

        State nextState(e->getTo(), e->getTransportType());
        dist[nextState] = distance;
        parent[nextState] = std::make_pair(State(start, e->getTransportType()), e);
        pq.push(QueueItem(distance, nextState, travelTime));
    }

    UGraph pathGraph;

    while (!pq.empty()) {
        QueueItem current = pq.top();
        pq.pop();
        State currentState = current.state;
        double currentTime = current.totalTime;

        if (dist.find(currentState) != dist.end() && dist[currentState] < current.priority)
            continue;

        if (currentState.vertex == end) {
            pathGraph.addVertex(start);
            pathGraph.addVertex(end);

            State cur = currentState;
            while (!(cur.vertex == start)) {
                auto it = parent.find(cur);
                if (it == parent.end()) break;

                AbstractEdge* orig = it->second.second;
                if (!orig) break;

                Vertex from = orig->getFrom();
                Vertex to = orig->getTo();

                pathGraph.addVertex(from);
                pathGraph.addVertex(to);

                TransportType tt = orig->getTransportType();
                if (tt == TransportType::SHIP) {
                    pathGraph.addEdge(new ShipEdge(
                        from, to,
                        orig->getActualTime() / orig->getWeatherFactor(),
                        orig->getActualCost(),
                        orig->getMaxWeight(), orig->getMaxSize(),
                        "clear"
                    ));
                } else if (tt == TransportType::TRUCK) {
                    pathGraph.addEdge(new TruckEdge(
                        from, to,
                        orig->getActualTime() / (orig->getWeatherFactor() * trafficFactor),
                        orig->getActualCost() / timeCostMultiplier,
                        orig->getMaxWeight(), orig->getMaxSize(),
                        "clear"
                    ));
                } else if (tt == TransportType::PLANE) {
                    pathGraph.addEdge(new PlaneEdge(
                        from, to,
                        (orig->getActualTime() / orig->getWeatherFactor()) - 0.5,
                        orig->getActualCost(),
                        orig->getMaxWeight(), orig->getMaxSize(),
                        "clear", 0.5
                    ));
                } else if (tt == TransportType::CAR) {
                    pathGraph.addEdge(new CarEdge(
                        from, to,
                        orig->getActualTime() / (orig->getWeatherFactor() * trafficFactor),
                        orig->getActualCost() / timeCostMultiplier,
                        orig->getMaxWeight(), orig->getMaxSize(),
                        "clear"
                    ));
                }

                cur = it->second.first;
            }

            return std::move(pathGraph);
        }

        auto edges = graph.getEdgesOf(currentState.vertex);
        for (size_t i = 0; i < edges.Size(); ++i) {
            AbstractEdge* e = edges.Get(i);
            if (!e->canCarry(cargoWeight, cargoSize)) continue;

            Vertex next = (e->getFrom() == currentState.vertex) ? e->getTo() : e->getFrom();
            TransportType nextTransport = e->getTransportType();

            double edgeTime = e->getActualTime(trafficFactor);
            double edgeCost = e->getActualCost(timeCostMultiplier);

            double transferTime = next.getTransferTime();
            double transferCost = next.getTransferCost();

            double newTime = currentTime + transferTime + edgeTime;
            double newCost = (dist[currentState] - (optimizeByTime ? currentTime : 0)) + transferCost + edgeCost;
            double newDistance = optimizeByTime ? newTime : newCost;

            State nextState(next, nextTransport);

            if (dist.find(nextState) == dist.end() || newDistance < dist[nextState]) {
                dist[nextState] = newDistance;
                parent[nextState] = std::make_pair(currentState, e);
                pq.push(QueueItem(newDistance, nextState, newTime));
            }
        }
    }

    return pathGraph;
}

#endif