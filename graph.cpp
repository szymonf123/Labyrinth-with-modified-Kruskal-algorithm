#include "graph.hpp"
#include "visitor.hpp"
#include <vector>
#include <iostream>

GraphAsMatrix::GraphAsMatrix(int n, bool b){
    numberOfVertices = n;
    isDirected = b;

    adjacencyMatrix.resize(numberOfVertices);

    for (int i = 0; i < numberOfVertices; i++){
        vertices.push_back(new Vertex(i));
        for (int j = 0; j < numberOfVertices; j++){
            adjacencyMatrix[i].push_back(NULL);
        }
    }
}

int GraphAsMatrix::NumberOfVertices(){
    return numberOfVertices;
}

bool GraphAsMatrix::IsDirected(){
    return isDirected;
}

int GraphAsMatrix::NumberOfEdges(){
    return numberOfEdges;
}

bool GraphAsMatrix::IsEdge(int u, int v){
    if (u < 0 || u >= numberOfVertices || v < 0 || v >= numberOfVertices || adjacencyMatrix[u][v] == NULL){
        return false;
    }
    return true;
}

void GraphAsMatrix::MakeNull(){
    for (int i = 0; i < numberOfVertices; i++){
        for (int j = 0; j < numberOfVertices; j++){
            Edge* tmp = adjacencyMatrix[i][j];
            adjacencyMatrix[i][j] = NULL;
            delete tmp;
        }
    }
    numberOfEdges = 0;
}

void GraphAsMatrix::AddEdge(int u, int v){
    if (u >= 0 && u < numberOfVertices && v >= 0 && v < numberOfVertices && adjacencyMatrix[u][v] == NULL){ 
        Edge* e = new Edge(vertices[u], vertices[v]);
        adjacencyMatrix[u][v] = e;
        edges.push_back(e); //nowa linijka w implementacji
        if (!IsDirected())
            adjacencyMatrix[v][u] = e;
        numberOfEdges++;
    }
}

Edge* GraphAsMatrix::SelectEdge(int u, int v){
    if (u >= 0 && u < numberOfVertices && v >= 0 && v < numberOfVertices){
        return adjacencyMatrix[u][v];
    }
    return NULL;
}

Edge* GraphAsMatrix::SelectEdge(int n){ //nowa metoda
    return edges[n];
}

Vertex* GraphAsMatrix::SelectVertex(int u){
    if (u >= 0 && u < numberOfVertices){
        return vertices[u];
    }
    return NULL;
}

Iterator<Vertex>& GraphAsMatrix::VerticesIter(){
    Iterator<Vertex>* vi = new AllVerticesIter(this);
    return *vi;
}

Iterator<Edge>& GraphAsMatrix::EdgesIter(){
    Iterator<Edge>* ei = new AllEdgesIter(this);
    return *ei;
}

Iterator<Edge>& GraphAsMatrix::EmanatingEdgesIter(int v){
    Iterator<Edge>* eei = new EmanEdgesIter(this, v);
    return *eei;
}

Iterator<Edge>& GraphAsMatrix::IncidentEdgesIter(int v){
    Iterator<Edge>* iei = new InciEdgesIter(this, v);
    return *iei;
}

void GraphAsMatrix::DFS(Vertex* v){
    std::vector<bool> visited;
    for (int i = 0; i < NumberOfVertices(); i++){
        visited.push_back(false);
    }

    DFS1(v, visited);

    Iterator<Vertex>& allVertices = this->VerticesIter();
    while (!allVertices.IsDone()){
        if (!visited.at((*allVertices).Number())){
            Vertex& x = *allVertices;
            DFS1(&x, visited);
        }
        ++allVertices;
    }
}

void GraphAsMatrix::DFS1(Vertex* v, std::vector<bool>& visited){
    std::cout << v->Number() << " ";
    visited.at(v->Number()) = true;
    Iterator<Edge>& neighbours = EmanatingEdgesIter(v->Number());
    while (!neighbours.IsDone()){
        if (!visited.at((*neighbours).Mate(v)->Number())){
            DFS1((*neighbours).Mate(v), visited);
        }
        ++neighbours;
    }
}

int GraphAsMatrix::DFSCount(){
    std::vector<bool> visited;
    for (int i = 0; i < NumberOfVertices(); i++){
        visited.push_back(false);
    }

    CountingVisitor* visitor = new CountingVisitor();
    DFS1(visitor, vertices.at(0), visited);

    Iterator<Vertex>& allVertices = this->VerticesIter();
    while (!allVertices.IsDone()){
        if (!visited.at((*allVertices).Number())){
            Vertex& x = *allVertices;
            DFS1(visitor, &x, visited);
        }
        ++allVertices;
    }
    int result = visitor->getCounter();
    delete visitor;
    return result;
}

void GraphAsMatrix::DFS1(CountingVisitor* visitor, Vertex* v, std::vector<bool>& visited){
    visitor->Visit(v);
    visited.at(v->Number()) = true;
    Iterator<Edge>& neighbours = EmanatingEdgesIter(v->Number());
    while (!neighbours.IsDone()){
        if (!visited.at((*neighbours).Mate(v)->Number())){
            DFS1(visitor, (*neighbours).Mate(v), visited);
        }
        ++neighbours;
    }
}

bool GraphAsMatrix::IsConnected(){
    // do pierwszej czesci zadania
    /*std::vector<bool> visited;
    for (int i = 0; i < NumberOfVertices(); i++){
        visited.push_back(false);
    }

    CountingVisitor* visitor = new CountingVisitor();
    DFS1(visitor, vertices.at(0), visited);

    int visitedVertices = visitor->getCounter();
    delete visitor;

    return (visitedVertices == NumberOfVertices());*/

    std::vector<bool> visited;
    for (int i = 0; i < NumberOfVertices(); i++){
        visited.push_back(false);
    }
    int visitedVertices;

    if (!IsDirected()){
        CountingVisitor* visitor = new CountingVisitor();
        DFS1(visitor, vertices.at(0), visited);

        visitedVertices = visitor->getCounter();
        delete visitor;

        return (visitedVertices == NumberOfVertices());
    }
    else {
        Iterator<Vertex>& it = VerticesIter();
        while (!it.IsDone()){
            CountingVisitor* visitor = new CountingVisitor();
            Vertex& x = *it;
            DFS1(visitor, &x, visited);

            visitedVertices = visitor->getCounter();
            delete visitor;

            if (visitedVertices != NumberOfVertices()){
                return false;
            }
            else {
                for (auto i = visited.begin(); i != visited.end(); i++)
                    *i = false;
            }
            ++it;
        }
        return true;
    }
}

void GraphAsMatrix::DFS_Spanning_Tree(Vertex* v){
    if (!IsConnected()){
        std::cout << "Graf nie jest spojny\n";
        return;
    }    

    std::vector<bool> visited(NumberOfVertices(), false);
    std::vector<int> parent(NumberOfVertices(), -1);

    DFS_Spanning_Tree_1(v,visited, parent);
    std::cout << "\n";
}

void GraphAsMatrix::DFS_Spanning_Tree_1(Vertex* v, std::vector<bool>& visited, std::vector<int>& parent){
    visited[v->Number()] = true;

    Iterator<Edge>& neighbours = EmanatingEdgesIter(v->Number());
    while (!neighbours.IsDone()){
        if (!visited[(*neighbours).Mate(v)->Number()]){
            parent[(*neighbours).Mate(v)->Number()] = v->Number();
            std::cout << parent[(*neighbours).Mate(v)->Number()] << "->" << (*neighbours).Mate(v)->Number() << "\n";
            DFS_Spanning_Tree_1((*neighbours).Mate(v), visited, parent);
        }
        ++neighbours;
    }
}