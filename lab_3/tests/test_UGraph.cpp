#include "UGraph.h"
#include "ege/ShipEdge.h"
#include "ege/TruckEdge.h"
#include "ege/PlaneEdge.h"
#include "ege/CarEdge.h"
#include <gtest/gtest.h>
#include <stdexcept>

TEST(UGraphTest, AddVertexAndEdge) {
    UGraph g;
    CommonVertex A("Port");
    CommonVertex B("City");

    g.addVertex(A);
    g.addVertex(B);

    g.addEdge(new ShipEdge(A, B, 24.0, 100.0, 500.0, 1000.0, "storm"));
    g.addEdge(new TruckEdge(A, B, 8.0, 50.0, 10.0, 20.0, "light_rain", 1.5));

    EXPECT_EQ(g.vertexCount(), 2);
    EXPECT_EQ(g.edgeCount(), 2);

    auto edges = g.getEdgesOf(A);
    EXPECT_EQ(edges.Size(), 2);

    CommonEdge* e1 = edges.Get(0);
    CommonEdge* e2 = edges.Get(1);

    if (e1->getWeight() == 24.0 * 2.0) {
        EXPECT_DOUBLE_EQ(e1->getWeight(), 48.0);
        EXPECT_DOUBLE_EQ(e2->getWeight(), 8.0 * 1.2 * 1.5);
    } else {
        EXPECT_DOUBLE_EQ(e1->getWeight(), 8.0 * 1.2 * 1.5);
        EXPECT_DOUBLE_EQ(e2->getWeight(), 48.0);
    }
}

TEST(UGraphTest, EdgeWithInvalidVertex) {
    UGraph g;
    CommonVertex A("A"), B("B");
    g.addVertex(A);

    EXPECT_THROW(
        g.addEdge(new ShipEdge(A, B, 1.0, 1.0, 1.0, 1.0, "clear")),
        std::invalid_argument
    );
}

TEST(UGraphTest, CanCarry) {
    UGraph g;
    CommonVertex A("A"), B("B");
    g.addVertex(A);
    g.addVertex(B);

    g.addEdge(new TruckEdge(A, B, 8.0, 50.0, 10.0, 20.0, "clear"));

    auto edges = g.getEdgesOf(A);
    CommonEdge* e = edges.Get(0);

    EXPECT_TRUE(e->canCarry(5.0, 10.0));
    EXPECT_FALSE(e->canCarry(15.0, 10.0));
    EXPECT_FALSE(e->canCarry(5.0, 25.0));
}