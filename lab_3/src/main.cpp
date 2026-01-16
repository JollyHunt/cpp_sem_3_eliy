#include "visualisation/GV.h"
#include "algo_lib/dijkstra.h"
#include <iostream>

int main() {
    int choice;
    std::cout << "Select graph: ";
    std::cin >> choice;

    if (choice == 1) {
        UGraph graph;
        Vertex A("A", 0.5, 10.0);
        Vertex B("B", 0.3, 8.0);
        Vertex C("C", 0.4, 7.0);
        Vertex E("E", 0.2, 5.0);
        Vertex G("G", 0.6, 12.0);
        Vertex F("F", 0.7, 15.0);
        Vertex H("H", 0.35, 9.0);

        graph.addVertex(A);
        graph.addVertex(B);
        graph.addVertex(C);
        graph.addVertex(E);
        graph.addVertex(F);
        graph.addVertex(G);
        graph.addVertex(H);

        graph.addEdge(new CarEdge(A, B, 8.0, 50.0, 2.0, 5.0, "clear"));
        graph.addEdge(new TruckEdge(A, C, 12.0, 80.0, 10.0, 20.0, "light_rain"));
        graph.addEdge(new PlaneEdge(B, H, 3.0, 200.0, 1.0, 2.0, "clear"));
        graph.addEdge(new ShipEdge(C, E, 48.0, 300.0, 100.0, 200.0, "storm"));
        graph.addEdge(new CarEdge(C, H, 20.0, 100.0, 2.0, 5.0, "heavy_rain"));
        graph.addEdge(new PlaneEdge(A, E, 5.0, 250.0, 1.0, 2.0, "fog"));
        graph.addEdge(new TruckEdge(B, G, 15.0, 90.0, 10.0, 20.0, "snow"));
        graph.addEdge(new CarEdge(G, C, 6.0, 40.0, 2.0, 5.0, "clear"));
        graph.addEdge(new ShipEdge(E, F, 72.0, 500.0, 100.0, 200.0, "heavy_rain"));
        graph.addEdge(new PlaneEdge(H, F, 8.0, 400.0, 1.0, 2.0, "storm"));
        graph.addEdge(new TruckEdge(G, F, 30.0, 200.0, 10.0, 20.0, "light_rain"));
        graph.addEdge(new CarEdge(E, G, 18.0, 120.0, 2.0, 5.0, "snow"));

        GV::visualize(graph, "full_graph_7v");

        UGraph path = dijkstra(graph, A, F, 1.0, 1.0, 1.0, 1.0, true);
        if (!path.isEmpty()) {
            GV::visualize(path, "path_7v");
        } else {
            std::cout << "No path found.\n";
        }

    } else if (choice == 2) {
        UGraph graph;
        Vertex A("A", 0.2, 5.0);
        Vertex B("B", 0.1, 3.0);
        Vertex C("C", 0.3, 7.0);
        Vertex D("D", 0.15, 4.0);
        Vertex E("E", 0.25, 6.0);

        graph.addVertex(A);
        graph.addVertex(B);
        graph.addVertex(C);
        graph.addVertex(D);
        graph.addVertex(E);

        graph.addEdge(new CarEdge(A, B, 2.0, 10.0, 2.0, 5.0, "clear"));
        graph.addEdge(new TruckEdge(B, C, 3.0, 20.0, 10.0, 20.0, "light_rain"));
        graph.addEdge(new PlaneEdge(C, D, 1.0, 50.0, 1.0, 2.0, "clear"));
        graph.addEdge(new ShipEdge(D, E, 4.0, 30.0, 50.0, 100.0, "storm"));
        graph.addEdge(new CarEdge(A, C, 5.0, 25.0, 2.0, 5.0, "clear"));
        graph.addEdge(new TruckEdge(C, E, 6.0, 35.0, 10.0, 20.0, "snow"));

        GV::visualize(graph, "full_graph_5v-clearAC");

        UGraph path = dijkstra(graph, A, E, 1.0, 1.0, 1.0, 1.0, true);
        if (!path.isEmpty()) {
            GV::visualize(path, "path_5v-clearAC");
        } else {
            std::cout << "No path found.\n";
        }

    } else {
        std::cout << "Invalid choice!\n";
        return 1;
    }

    return 0;
}