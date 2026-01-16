#ifndef UGRAPH_H
#define UGRAPH_H

#include "ege/AbstractEdge.h"
#include "sequence/ArraySequence.h"
#include <stdexcept>

class UGraph {
private:
    ArraySequence<Vertex> vertices_;
    ArraySequence<AbstractEdge*> edges_;

    bool hasVertex(const Vertex& v) const {
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

    void addVertex(const Vertex& v) {
        if (!hasVertex(v)) {
            vertices_.Append(v);
        }
    }

    void addEdge(AbstractEdge* edge) {
        if (!edge) return;
        if (!hasVertex(edge->getFrom()) || !hasVertex(edge->getTo())) {
            delete edge;
            throw std::invalid_argument("Vertex not found");
        }
        edges_.Append(edge);
    }

    const ArraySequence<Vertex>& getVertices() const { return vertices_; }
    const ArraySequence<AbstractEdge*>& getEdges() const { return edges_; }

    ArraySequence<AbstractEdge*> getEdgesOf(const Vertex& v) const {
        if (!hasVertex(v)) {
            throw std::invalid_argument("Vertex not found");
        }
        ArraySequence<AbstractEdge*> incident;
        for (size_t i = 0; i < edges_.Size(); ++i) {
            AbstractEdge* e = edges_.Get(i);
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