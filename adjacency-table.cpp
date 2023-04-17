/*
 * Description  : Making adjacency table for map logic problem
 * Developers   : Dylan Conklin, Yuxin Sun, Michael Fulton
 */
#include <algorithm>
#include <ctime>
#include <fstream>
#include <iomanip>
#include <iostream>
#include <set>
#include <string>
#define SPACING 2

class edge{
  public:
  std::string start;
  std::string end;
  float weight;
  edge(std::string, std::string, float);
  friend bool operator < (const edge &left, const edge &right);
};

edge::edge(std::string start, std::string end, float weight){
  this->start = start;
  this->end = end;
  this->weight = weight;
}

bool operator < (const edge &left, const edge &right){
    return left.weight < right.weight;
}

void print_adjacency_graph(std::set <class edge> edges, std::set <std::string> vertices, unsigned int width){
  std::cout << std::setw(width - SPACING) << " ";
  for(auto vertex : vertices)
    std::cout << std::setw(width) << vertex;
  std::cout << std::endl;
  for(auto y : vertices){
    std::cout << std::setw(width - SPACING) << y;
    for(auto x : vertices){
      bool written = false;
      for(auto edge : edges){
        if((edge.start == y && edge.end == x) || (edge.start == x && edge.end == y)){
          std::cout << std::setw(width) << edge.weight;
          written = true;
        }
      }
      if(written == false)
        std::cout << std::setw(width) << '0';
    }
    std::cout << std::endl;
  }
  std::cout << std::endl;
}

std::set <class edge> kruskal(std::set <class edge> G, std::set <std::string> vertices){
  unsigned int n = G.size(); // defining n
  std::set <std::string> vertices_left (vertices); // vertices that don't have an edge
  std::set <class edge> T; // our answer
  std::set <class edge> E (G); // set of all the edges

  for(int m = 0; m < n-1; m++){
    edge e = *E.begin();
    E.erase(e);
    bool circuit = false;
    if(!(vertices_left.count(e.start) || vertices_left.count(e.end)))
      circuit = true;
    if(circuit == false){
      T.insert(e);
      if(vertices_left.count(e.start))
        vertices_left.erase(e.start);
      if(vertices_left.count(e.end))
        vertices_left.erase(e.end);
    }
  }
  return T;
}

std::set <class edge> prim(std::set <class edge> G, std::set <std::string> vertices){
  unsigned int n = G.size();
  std::set <std::string> vertices_left (vertices); // vertices that don't have an edge
  std::set <class edge> T; // our answer
  for(unsigned int i = 0; i < n-1; i++){
    edge e = *G.begin();
    if(vertices_left.count(e.start) || vertices_left.count(e.end)){
      T.insert(e);
      vertices_left.erase(e.start);
      vertices_left.erase(e.end);
    }
    G.erase(e);
  }
  return T;
}

unsigned int loadData(std::set <class edge> &G, std::set <std::string> &vertices){
  // Temp values for inputs
  std::string input_start;
  std::string input_end;
  float input_weight;
  unsigned int width = 0;

  std::ifstream infile("city-pairs.txt");

  int size = 0;
  std::string input;
  while(std::getline(infile, input)) // Counts number of lines in file
    size++;

  // Goes to beginning of file
  infile.clear();
  infile.seekg(0, std::ios::beg);

  // Loads data into struct
  for(int i = 0; i < size; i++){
    infile >> input_start >> input_end >> input_weight;

    // Add new vertices to set
    if(!(std::count(vertices.begin(), vertices.end(), input_start)))
      vertices.insert(input_start);
    if(!(std::count(vertices.begin(), vertices.end(), input_end)))
      vertices.insert(input_end);

    // Add edges to list
    G.insert(edge(input_start, input_end, input_weight));

    width = (width > ((input_start.size() > input_end.size()) ? input_start.size() : input_end.size())) ? width : ((input_start.size() > input_end.size()) ? input_start.size() : input_end.size());
  }

  infile.close();

  return width+3;
}

int main(){
  std::set <class edge> G; // The whole graph
  std::set <std::string> vertices; // List of vertices
  std::set <class edge> MST; // The graph with smallest path

  unsigned int width = SPACING + loadData(G, vertices);

  std::cout << "Width: " << width << std::endl;

  std::cout << "Adjacency Table:" << std::endl << std::endl;
  print_adjacency_graph(G, vertices, width);

  clock_t k_begin_time = clock();
  MST = kruskal(G, vertices);
  clock_t k_end_time = clock();

  std::cout << "Kruskal's Algorithm:" << std::endl << std::endl;
  print_adjacency_graph(MST, vertices, width);

  clock_t p_begin_time = clock();
  MST = prim(G, vertices);
  clock_t p_end_time = clock();

  std::cout << "Prim's Algorithm:" << std::endl << std::endl;
  print_adjacency_graph(MST, vertices, width);

  std::cout << "Kruskal Time : " << (k_end_time - k_begin_time) / (double) CLOCKS_PER_SEC << std::endl;
  std::cout << "Prim    Time : " << (p_end_time - p_begin_time) / (double) CLOCKS_PER_SEC << std::endl;

  exit(EXIT_SUCCESS);
}
