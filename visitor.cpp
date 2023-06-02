#include "visitor.hpp"
#include "graph.hpp"

CountingVisitor::CountingVisitor(){
    counter = 0;
}

void CountingVisitor::Visit(Vertex*& element){
    counter++;
}

int CountingVisitor::getCounter(){
    return counter;
}