#pragma once

#include "visitor.hpp"
#include <string>
#include <vector>

class CountingVisitor;

class Vertex {
    int number;
public:
    int weight;
    std::string label;
    Vertex(int n);
    int Number() const;
    bool operator<(const Vertex& other) const; //nowy operator
};

class Edge {
protected:
    Vertex* v0;
    Vertex* v1;
public:
    int weight;
    std::string label;
    Edge (Vertex *V0, Vertex* V1);
    Vertex* V0 (){return v0;};
    Vertex* V1 (){return v1;};
    Vertex* Mate (Vertex* v);
};

template <typename T>
class Iterator{
public:
    virtual ~Iterator (){;}
    Iterator(){;}
    virtual bool IsDone () = 0;
    virtual T & operator * () = 0;
    virtual void operator ++ () = 0;
};

class GraphAsMatrix {
    std::vector<Vertex*> vertices;
    std::vector<std::vector<Edge*>> adjacencyMatrix;
    std::vector<Edge*> edges;
    bool isDirected;
    int numberOfVertices;
    int numberOfEdges=0;

    class AllVerticesIter: public Iterator<Vertex>{
        GraphAsMatrix* owner;
        int current;
    public:
        AllVerticesIter(GraphAsMatrix* owner);
        bool IsDone();
        Vertex & operator*();
        void operator++();
    };

    class AllEdgesIter: public Iterator<Edge>{
        GraphAsMatrix* owner;
        int row;
        int col;
    public:
        void next();
        AllEdgesIter(GraphAsMatrix* owner);
        bool IsDone();
        Edge & operator*();
        void operator++(){next();}
    };

    class EmanEdgesIter: public Iterator<Edge>{
        GraphAsMatrix* owner;
        int row;
        int col;
    public:
        void next();
        EmanEdgesIter(GraphAsMatrix* owner,int v);
        bool IsDone();
        Edge & operator*();
        void operator++(){next();}
    };

    class InciEdgesIter: public Iterator<Edge>{
        GraphAsMatrix* owner;
        int row;
        int col;
    public:
        void next();
        InciEdgesIter(GraphAsMatrix* owner,int v);
        bool IsDone();
        Edge & operator*();
        void operator++(){next();}
    };

public:
    GraphAsMatrix (int n, bool b);
    int NumberOfVertices();
    bool IsDirected();
    int NumberOfEdges();
    bool IsEdge(int u, int v);
    void MakeNull();
    void AddEdge (int u, int v);
    Edge* SelectEdge (int u, int v);
    Edge* SelectEdge(int n); //nowa metoda
    Vertex* SelectVertex(int v);
    Iterator<Vertex>& VerticesIter();
    Iterator<Edge>& EdgesIter();
    Iterator<Edge>& EmanatingEdgesIter(int v);
    Iterator<Edge>& IncidentEdgesIter(int v);
    void DFS(Vertex* v);
    int DFSCount();
    bool IsConnected();
    void DFS_Spanning_Tree(Vertex* v);

private:
    void DFS1(Vertex* v, std::vector<bool>& visited);
    void DFS1(CountingVisitor* visitor, Vertex* v, std::vector<bool>& visited);
    void DFS_Spanning_Tree_1(Vertex* v, std::vector<bool>& visited, std::vector<int>& parent);

    friend class AllVerticesIter;
    friend class AllEdgesIter;
    friend class EmanEdgesIter;
    friend class InciEdgesIter;
};
