#include <iostream>
#include <set>
#include <vector>
#include <ctime>
#include <algorithm>
#include <fstream>
#include "graph.hpp"
#include "visitor.hpp"

template <typename T>
class SetPartition {
        std::vector<std::set<T>> partition;
        int maxPower;
        std::set<T> empty_set;
    public:
        SetPartition(const std::vector<T>& collection);
        void join(std::set<T>& first, std::set<T>& second);
        std::set<T>& find(T element);
        int getMaxPower();
        void printPartition();
};

template <typename T>
SetPartition<T>::SetPartition(const std::vector<T>& collection) {
    for (const T& element : collection) {
        std::set<T> singleton;
        singleton.insert(element);
        partition.push_back(singleton);
    }
    maxPower = 1;
}

template <typename T>
void SetPartition<T>::join(std::set<T>& first, std::set<T>& second){
    std::set<T> empty_set;
    if (first.size() + second.size() > maxPower){
        maxPower = first.size() + second.size();
    }
    for (T element : second){
        first.insert(element);
    }

    auto it = std::find(partition.begin(), partition.end(), second);
    if (it != partition.end()) {
        partition.erase(it);
    }
}

template <typename T>
std::set<T>& SetPartition<T>::find(T element){
    for (auto it = partition.begin(); it != partition.end(); ++it) {
        std::set<T>& s = *it;
        for (auto e : s) {
            if (e == element) {
                return s;
            }
        }
    }
    std::cerr << "Blad funkcji find\n";
    return empty_set;
}

template <typename T>
int SetPartition<T>::getMaxPower(){
    return maxPower;
}

template <typename T>
void SetPartition<T>::printPartition(){}

template <>
void SetPartition<Vertex*>::printPartition(){
    for (auto s : partition){
        for (auto e : s){
            std::cout << e->Number() << " ";
        }
        std::cout << "\n";
    }
}

class Labyrinth {
    int width, height;
    GraphAsMatrix* graph;
    GraphAsMatrix* spanningTree;

    public:
        Labyrinth(int n, int m){
            width = m;
            height = n;
            graph = new GraphAsMatrix(width * height, false);
            spanningTree = new GraphAsMatrix(width * height, false);

            for (int i = 0; i < graph->NumberOfVertices(); i++){
                if (i + width < graph->NumberOfVertices()){
                    graph->AddEdge(i, i + width);
                }
                if ((i + 1) % width != 0){
                    graph->AddEdge(i, i + 1);
                }
            }
        }

        void modifiedKruskalAlgorithm(){
            //inicjalizacja struktury podzialu zbioru
            std::vector<Vertex*> vertices;
            Iterator<Vertex>& vi = graph->VerticesIter();
            while (!vi.IsDone()){
                vertices.push_back(&*vi);
                ++vi;
            }
            SetPartition<Vertex*> partition(vertices);

            //generowanie labiryntu
            std::vector<int> generated;
            srand(time(NULL));
            while (partition.getMaxPower() < graph->NumberOfVertices()){
                int randomNumber;
                Edge* randomEdge;
                do {
                    randomNumber = generate(graph->NumberOfEdges(), generated);
                    randomEdge = graph->SelectEdge(randomNumber);
                }
                while (partition.find(randomEdge->V0()) == partition.find(randomEdge->V1()));
                
                spanningTree->AddEdge(randomEdge->V0()->Number(), randomEdge->V1()->Number());
                std::set<Vertex*>& first = partition.find(randomEdge->V0());
                std::set<Vertex*>& second = partition.find(randomEdge->V1());
                partition.join(first, second);
                delete randomEdge;
            }
        }

        void saveLabyrinth(){
            std::fstream file;
            file.open("edges_to_visualize.txt", std::ios::out);

            if (!file.good()){
                std::cerr << "Blad podczas otwarcia pliku\n";
            }
            else {
                file << height << "\n" << width << "\n";
                Iterator<Edge>& ei = spanningTree->EdgesIter();
                while (!ei.IsDone()){
                    file << (*ei).V0()->Number() << "\n" << (*ei).V1()->Number() << "\n";
                    ++ei;
                }
            std::cout << "Zapisano plik\n";
            file.close();
            }
        }

        ~Labyrinth(){
            delete graph;
            delete spanningTree;
        }

    private:
        int generate(int max, std::vector<int>& generated, int counter = 30){
        int randomNumber = rand() % max;
        for (auto i : generated){
            if (i == randomNumber){
                if (counter > 0)
                    return generate(max, generated, counter - 1);
            }
        }
        generated.push_back(randomNumber);
        return randomNumber;
        }
};

int main(){
    int height, width;
    std::cout << "Podaj wysokosc siatki: ";
    std::cin >> height;
    std::cout << "Podaj szerokosc siatki: ";
    std::cin >> width;
    Labyrinth* lab = new Labyrinth(height, width);
    lab->modifiedKruskalAlgorithm();
    lab->saveLabyrinth();
    delete lab;
    std::cout << "\n";
}