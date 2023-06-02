#include "graph.hpp"

GraphAsMatrix::AllVerticesIter::AllVerticesIter(GraphAsMatrix* other){
    owner = other;
    current = 0;
}

bool GraphAsMatrix::AllVerticesIter::IsDone(){
    return (current == owner->vertices.size());
}

Vertex& GraphAsMatrix::AllVerticesIter::operator*(){
    return *owner->vertices[current];
}

void GraphAsMatrix::AllVerticesIter::operator++(){
    current++;
}

GraphAsMatrix::AllEdgesIter::AllEdgesIter(GraphAsMatrix* other){
    owner = other;
    row = 0;
    col = -1;
    next();
}

void GraphAsMatrix::AllEdgesIter::next(){
    for (int i = row; i < owner->NumberOfVertices(); i++){
        for (int j = (i == row) ? col + 1 : 0; j < owner->NumberOfVertices(); j++){
            if (owner->adjacencyMatrix[i][j] != NULL){
                row = i;
                col = j;
                return;
            }
            if (!owner->IsDirected() && i == j){ //rozszerzenie implementacji
                break;
            }
        }
    }
    row = owner->NumberOfVertices();
    col = owner->NumberOfVertices();
}

bool GraphAsMatrix::AllEdgesIter::IsDone(){
    return (row >= owner->NumberOfVertices() && col >= owner->NumberOfVertices());
}

Edge& GraphAsMatrix::AllEdgesIter::operator*(){
    return *(owner->adjacencyMatrix[row][col]);
}

GraphAsMatrix::EmanEdgesIter::EmanEdgesIter(GraphAsMatrix* other, int v){
    owner = other;
    row = v;
    col = -1;
    next();
}

void GraphAsMatrix::EmanEdgesIter::next(){
    for (int i = col + 1; i < owner->NumberOfVertices(); i++){
        if (owner->adjacencyMatrix[row][i] != NULL){
            col = i;
            return;
        }
    }
    col = owner->NumberOfVertices();
}

bool GraphAsMatrix::EmanEdgesIter::IsDone(){
    return (col == owner->NumberOfVertices());
}

Edge& GraphAsMatrix::EmanEdgesIter::operator*(){
    return *(owner->adjacencyMatrix[row][col]);
}

GraphAsMatrix::InciEdgesIter::InciEdgesIter(GraphAsMatrix* other, int v){
    owner = other;
    row = -1;
    col = v;
    next();
}

void GraphAsMatrix::InciEdgesIter::next(){
    for (int i = row + 1; i < owner->NumberOfVertices(); i++){
        if (owner->adjacencyMatrix[i][col] != NULL){
            row = i;
            return;
        }
    }
    row = owner->NumberOfVertices();
}

bool GraphAsMatrix::InciEdgesIter::IsDone(){
    return (row == owner->NumberOfVertices());
}

Edge& GraphAsMatrix::InciEdgesIter::operator*(){
    return *(owner->adjacencyMatrix[row][col]);
}