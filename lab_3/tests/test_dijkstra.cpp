#include "algo_lib/dijkstra.h"
#include "ege/CarEdge.h"
#include "ege/TruckEdge.h"
#include "ege/PlaneEdge.h"
#include "ege/ShipEdge.h"
#include <gtest/gtest.h>

TEST(DijkstraTest, FindPathByTime) {
    UGraph graph;
    CommonVertex A("A"), B("B"), C("C"), D("D"), E("E");
    graph.addVertex(A); graph.addVertex(B); graph.addVertex(C); graph.addVertex(D); graph.addVertex(E);

    graph.addEdge(new CarEdge(A, B, 2.0, 10.0, 5.0, 10.0, "clear"));
    graph.addEdge(new TruckEdge(B, C, 3.0, 20.0, 10.0, 20.0, "clear"));
    graph.addEdge(new PlaneEdge(C, D, 1.0, 50.0, 2.0, 5.0, "clear"));
    graph.addEdge(new ShipEdge(D, E, 4.0, 30.0, 50.0, 100.0, "clear"));
    graph.addEdge(new PlaneEdge(A, C, 5.0, 60.0, 2.0, 5.0, "clear"));

    auto path = dijkstra(graph, A, E, [](const CommonEdge* e) {
        return e->canCarry(1.0, 1.0);
    });

    EXPECT_EQ(path.Size(), 4);
}

TEST(DijkstraTest, NoPathDueToCargo) {
    UGraph graph;
    CommonVertex A("A"), B("B");
    graph.addVertex(A); graph.addVertex(B);
    graph.addEdge(new CarEdge(A, B, 2.0, 10.0, 5.0, 10.0, "clear"));

    auto path = dijkstra(graph, A, B, [](const CommonEdge* e) {
        return e->canCarry(10.0, 1.0);
    });

    EXPECT_EQ(path.Size(), 0);
}