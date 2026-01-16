#ifndef GV_H
#define GV_H

#include "../UGraph.h"
#include "../ege/AbstractEdge.h"
#include <string>
#include <sstream>
#include <cstdlib>

class GV {
public:
    static void visualize(const UGraph& graph, const std::string& filename) {
        FILE* fp = fopen((filename + ".dot").c_str(), "w");
        if (!fp) return;

        fprintf(fp, "digraph G {\n");
        fprintf(fp, "  node [shape=box, style=filled, fillcolor=\"#E0E0E0\"];\n");
        fprintf(fp, "  rankdir=LR;\n");

        const auto& vertices = graph.getVertices();
        for (size_t i = 0; i < vertices.Size(); ++i) {
            const Vertex& v = vertices.Get(i);
            std::ostringstream label;
            label << v.getName() << "\\n"
                  << "t: " << v.getTransferTime() << "h\\n"
                  << "c: " << v.getTransferCost() << "$";
            fprintf(fp, "  \"%s\" [label=\"%s\"];\n",
                    v.getName().c_str(),
                    label.str().c_str());
        }

        const auto& edges = graph.getEdges();
        for (size_t i = 0; i < edges.Size(); ++i) {
            AbstractEdge* e = edges.Get(i);
            if (!e) continue;

            double time = e->getActualTime(1.0);
            double cost = e->getActualCost(1.0);

            std::ostringstream label;
            label << e->getTypeName() << "\\n"
                  << "t: " << time << "h\\n"
                  << "c: " << cost << "$";

            fprintf(fp, "  \"%s\" -> \"%s\" [label=\"%s\", color=black];\n",
                    e->getFrom().getName().c_str(),
                    e->getTo().getName().c_str(),
                    label.str().c_str());

            fprintf(fp, "  \"%s\" -> \"%s\" [label=\"%s\", color=black];\n",
                    e->getTo().getName().c_str(),
                    e->getFrom().getName().c_str(),
                    label.str().c_str());
        }

        fprintf(fp, "}\n");
        fclose(fp);

        std::string cmd = "dot -Tpng " + filename + ".dot -o " + filename + ".png";
        system(cmd.c_str());
    }
};

#endif