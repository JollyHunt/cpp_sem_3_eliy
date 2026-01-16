#include "algo_lib/dijkstra.h"
#include <gtest/gtest.h>

TEST(DijkstraTest, FindPathByTime) {
    UGraph graph;

    Vertex A("A", 0.0, 0.0);
    Vertex B("B", 0.0, 0.0);
    Vertex C("C", 0.0, 0.0);
    Vertex D("D", 0.0, 0.0);
    Vertex E("E", 0.0, 0.0);

    graph.addVertex(A);
    graph.addVertex(B);
    graph.addVertex(C);
    graph.addVertex(D);
    graph.addVertex(E);

    graph.addEdge(new CarEdge(A, B, 2.0, 10.0, 5.0, 10.0, "clear"));
    graph.addEdge(new TruckEdge(B, C, 3.0, 20.0, 10.0, 20.0, "clear"));
    graph.addEdge(new PlaneEdge(C, D, 1.0, 50.0, 2.0, 5.0, "clear"));
    graph.addEdge(new ShipEdge(D, E, 4.0, 30.0, 50.0, 100.0, "clear"));
    graph.addEdge(new PlaneEdge(A, C, 5.0, 60.0, 2.0, 5.0, "clear"));

    UGraph path = dijkstra(graph, A, E, 1.0, 1.0, 1.0, 1.0, true);

    EXPECT_FALSE(path.isEmpty());
    EXPECT_EQ(path.edgeCount(), 4);
}

TEST(DijkstraTest, FindPathByCost) {
    UGraph graph;

    Vertex A("A", 0.0, 0.0);
    Vertex B("B", 0.0, 0.0);
    Vertex C("C", 0.0, 0.0);
    Vertex D("D", 0.0, 0.0);
    Vertex E("E", 0.0, 0.0);

    graph.addVertex(A);
    graph.addVertex(B);
    graph.addVertex(C);
    graph.addVertex(D);
    graph.addVertex(E);

    graph.addEdge(new CarEdge(A, B, 2.0, 10.0, 5.0, 10.0, "clear"));
    graph.addEdge(new TruckEdge(B, C, 3.0, 20.0, 10.0, 20.0, "clear"));
    graph.addEdge(new PlaneEdge(C, D, 1.0, 50.0, 2.0, 5.0, "clear"));
    graph.addEdge(new ShipEdge(D, E, 4.0, 30.0, 50.0, 100.0, "clear"));
    graph.addEdge(new PlaneEdge(A, C, 5.0, 60.0, 2.0, 5.0, "clear"));

    UGraph path = dijkstra(graph, A, E, 1.0, 1.0, 1.0, 1.0, false);

    EXPECT_FALSE(path.isEmpty());
    EXPECT_EQ(path.edgeCount(), 4);
}

TEST(DijkstraTest, NoPathDueToCargo) {
    UGraph graph;

    Vertex A("A", 0.0, 0.0);
    Vertex B("B", 0.0, 0.0);

    graph.addVertex(A);
    graph.addVertex(B);


    graph.addEdge(new CarEdge(A, B, 2.0, 10.0, 5.0, 10.0, "clear"));

    UGraph path = dijkstra(graph, A, B, 10.0, 1.0, 1.0, 1.0, true);

    EXPECT_TRUE(path.isEmpty());
}
