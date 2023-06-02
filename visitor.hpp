#pragma once

#include "graph.hpp"

class Vertex;

template <typename T>
class Visitor {
    public:
        virtual void Visit (T& element) = 0;
        virtual bool IsDone () const{ return false; }
};

class CountingVisitor : public Visitor<Vertex*> {
    int counter;
    public:
        CountingVisitor();
        void Visit(Vertex*& element);
        int getCounter();
};
