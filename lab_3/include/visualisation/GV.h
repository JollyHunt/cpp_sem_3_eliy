#ifndef GV_H
#define GV_H

#include "../UGraph.h"
#include "../ege/TransportEdge.h"
#include <string>
#include <sstream>
#include <cstdlib>
#include <set>

inline std::string escapeDot(const std::string& s) {
    std::string result;
    for (char c : s) {
        if (c == '"' || c == '\\') result += '\\';
        result += c;
    }
    return result;
}

inline void visualizeGraph(const UGraph& graph, const std::string& filename) {
    std::string dotFile = filename + ".dot";
    FILE* fp = fopen(dotFile.c_str(), "w");
    if (!fp) return;

    fprintf(fp, "graph G {\n");
    fprintf(fp, "  node [shape=box, style=filled, fillcolor=\"#E0E0E0\"];\n");
    fprintf(fp, "  rankdir=LR;\n");

    const auto& vertices = graph.getVertices();
    for (size_t i = 0; i < vertices.Size(); ++i) {
        const CommonVertex& v = vertices.Get(i);
        std::string id = escapeDot(v.getId());
        fprintf(fp, "  \"%s\" [label=\"%s\"];\n", id.c_str(), id.c_str());
    }

    const auto& edges = graph.getEdges();
    for (size_t i = 0; i < edges.Size(); ++i) {
        CommonEdge* e = edges.Get(i);
        if (!e) continue;

        const TransportEdge* te = dynamic_cast<const TransportEdge*>(e);
        if (!te) continue;

        std::string from = escapeDot(e->getFrom().getId());
        std::string to = escapeDot(e->getTo().getId());

        double time = te->getActualTime();
        double cost = te->getActualCost();
        std::string type = te->getTypeName();

        std::ostringstream label;
        label << type << "\nt: " << time << "h\nc: " << cost << "$";

        fprintf(fp, "  \"%s\" -- \"%s\" [label=\"%s\", color=black];\n",
                from.c_str(), to.c_str(), escapeDot(label.str()).c_str());
    }

    fprintf(fp, "}\n");
    fclose(fp);

    std::string cmd = "dot -Tpng " + dotFile + " -o " + filename + ".png 2>/dev/null && xdg-open " + filename + ".png &";
    system(cmd.c_str());
}

inline void visualizeWithPath(const UGraph& graph,
                             const ArraySequence<CommonEdge*>& path,
                             const std::string& filename) {
    std::string dotFile = filename + ".dot";
    FILE* fp = fopen(dotFile.c_str(), "w");
    if (!fp) return;

    fprintf(fp, "graph G {\n");
    fprintf(fp, "  node [shape=box, style=filled, fillcolor=\"#E0E0E0\"];\n");
    fprintf(fp, "  rankdir=LR;\n");

    std::set<const CommonEdge*> pathSet;
    for (size_t i = 0; i < path.Size(); ++i) {
        pathSet.insert(path.Get(i));
    }

    const auto& vertices = graph.getVertices();
    for (size_t i = 0; i < vertices.Size(); ++i) {
        const CommonVertex& v = vertices.Get(i);
        std::string id = escapeDot(v.getId());
        fprintf(fp, "  \"%s\" [label=\"%s\"];\n", id.c_str(), id.c_str());
    }

    const auto& edges = graph.getEdges();
    for (size_t i = 0; i < edges.Size(); ++i) {
        CommonEdge* e = edges.Get(i);
        if (!e) continue;

        const TransportEdge* te = dynamic_cast<const TransportEdge*>(e);
        if (!te) continue;

        std::string from = escapeDot(e->getFrom().getId());
        std::string to = escapeDot(e->getTo().getId());

        double time = te->getActualTime();
        double cost = te->getActualCost();
        std::string type = te->getTypeName();

        std::ostringstream label;
        label << type << "\nt: " << time << "h\nc: " << cost << "$";

        std::string color = (pathSet.find(e) != pathSet.end()) ? "red" : "black";
        std::string penwidth = (pathSet.find(e) != pathSet.end()) ? "2.5" : "1.0";

        fprintf(fp, "  \"%s\" -- \"%s\" [label=\"%s\", color=%s, penwidth=%s];\n",
                from.c_str(), to.c_str(),
                escapeDot(label.str()).c_str(),
                color.c_str(), penwidth.c_str());
    }

    fprintf(fp, "}\n");
    fclose(fp);

    std::string cmd = "dot -Tpng " + dotFile + " -o " + filename + ".png 2>/dev/null && xdg-open " + filename + ".png &";
    system(cmd.c_str());
}

#endif