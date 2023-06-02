#include "graph.hpp"
#include "visitor.hpp"

Vertex::Vertex(int n){
    number = n;
    weight = 0;
    label = "";
}

int Vertex::Number() const {
    return number;
}

bool Vertex::operator<(const Vertex& other) const{
    return (Number() < other.Number());
}

Edge::Edge(Vertex* V0, Vertex* V1){
    v0 = V0;
    v1 = V1;
    weight = 0;
    label = "";
}

Vertex* Edge::Mate(Vertex* v){
    if (v == v1)
        return v0;
    if (v == v0)
        return v1;
    return NULL;
}