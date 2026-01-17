#ifndef UGRAPH_H
#define UGRAPH_H

#include "ege/CommonEdge.h"
#include "vertex/CommonVertex.h"
#include "sequence/ArraySequence.h"
#include <stdexcept>

class UGraph {
private:
    ArraySequence<CommonVertex> vertices_;
    ArraySequence<CommonEdge*> edges_;

    bool hasVertex(const CommonVertex& v) const {
        for (size_t i = 0; i < vertices_.Size(); ++i) {
            if (vertices_.Get(i) == v) return true;
        }
        return false;
    }

public:
    UGraph() = default;
    UGraph(const UGraph&) = delete;
    UGraph& operator=(const UGraph&) = delete;

    UGraph(UGraph&& other) noexcept
        : vertices_(std::move(other.vertices_)), edges_(std::move(other.edges_)) {}

    ~UGraph() {
        for (size_t i = 0; i < edges_.Size(); ++i) {
            delete edges_.Get(i);
        }
    }

    void addVertex(const CommonVertex& v) {
        if (!hasVertex(v)) {
            vertices_.Append(v);
        }
    }

    void addEdge(CommonEdge* edge) {
        if (!edge) return;
        if (!hasVertex(edge->getFrom()) || !hasVertex(edge->getTo())) {
            delete edge;
            throw std::invalid_argument("Vertex not found");
        }
        edges_.Append(edge);
    }

    const ArraySequence<CommonVertex>& getVertices() const { return vertices_; }
    const ArraySequence<CommonEdge*>& getEdges() const { return edges_; }

    ArraySequence<CommonEdge*> getEdgesOf(const CommonVertex& v) const {
        if (!hasVertex(v)) {
            throw std::invalid_argument("Vertex not found");
        }
        ArraySequence<CommonEdge*> incident;
        for (size_t i = 0; i < edges_.Size(); ++i) {
            CommonEdge* e = edges_.Get(i);
            if (e->getFrom() == v || e->getTo() == v) {
                incident.Append(e);
            }
        }
        return incident;
    }

    size_t vertexCount() const { return vertices_.Size(); }
    size_t edgeCount() const { return edges_.Size(); }
    bool isEmpty() const { return vertices_.IsEmpty(); }
};

#endif