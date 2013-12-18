#ifndef _GRAPH_H
#define	_GRAPH_H

#include <string>
#include <vector>
#include <map>
#include <iostream>
#include <stdexcept>
using namespace std;

// Custom Graph exception class
class GraphException : public runtime_error {
public:
	GraphException(const string &msg);
};

// A directed graph class template.
// This implementation favors the efficency of vertex and edge lookup according
//     to key values. Key values are used to uniquely identify vertices, such
//     that vertex objects in the graph may have the same state or attribute
//     values yet still be distinguishable. These key values are mapped to
//     vertex objects and their indices in the underlying adjacency matrix used
//     to store edge data.
template <typename Key, typename V, typename E>
class Graph {

public:

	// Default constructor initializes an empty graph
	Graph() :
		vertices(), edges() {
	}

	// Copy constructor
	Graph(const Graph &copy) {
		vertices = copy.vertices;
		edges = copy.edges;
		for(int i = 0; i < copy.edges.size(); ++i) {
			for(int j = 0; j < copy.edges[i].size(); ++j) {
				if(copy.edges[i][j]) edges[i][j] = new E(*copy.edges[i][j]);
			}
		}
	}

	// Destructor
	~Graph() {
		for(int i = 0; i < edges.size(); ++i) {
			for(int j = 0; j < edges[i].size(); ++j) {
				if(edges[i][j]) delete edges[i][j];
			}
		}
	}

	// Sets the value of the vertex associated with a certain key; adds a
	// new vertex to the graph if the key is not already associated with
	// another vertex
	void setVertex(const Key &k, const V &v) {

		// Simply reassign an existing value
		if(hasKey(k)) vertices[k].first = v;

		// Else add a new vertex entirely
		else {
			// Create mapping of key value to vertex object and table index
			vertices[k] = pair<V, int>(v, vertices.size());

			// Add new column
			for(int i = 0; i < edges.size(); ++i) {
				edges[i].push_back((E *) NULL);
			}

			// Add new row
			edges.push_back(vector<E *>(numVertices(), (E *) NULL));
		}
	}

	// Get a reference to the vertex object associated with a certain key
	// value; throws invalid_argument exception if the key is not mapped to
	// a vertex in the graph
	V &getVertex(const Key &k) {
		if(!hasKey(k)) throw invalid_argument("No vertex mapped to key value");
		return vertices[k].first;
	}

	const V &getVertex(const Key &k) const {
		if(!hasKey(k)) throw invalid_argument("No vertex mapped to key value");
		return vertices.at(k).first;
	}

	// Set the value of the edge associated with a vertex key pair; throws
	// invalid_argument exception if either key is not mapped to a vertex
	void setEdge(const Key &k1, const Key &k2, const E &e) {
		if(hasKey(k1) && hasKey(k2)) {
			E * &entry = edges[vertices[k1].second][vertices[k2].second];
			if(entry) *entry = e; // Reassign existing entries
			else entry = new E(e); // Allocate new entries
		}
		else throw invalid_argument("No vertex mapped to key value");
	}
	
	// Get a reference to the edge object associated with a vertex key pair;
	// throws invalid_argument exception if the vertices are not connected
	// or if either key is not mapped to a vertex
	E &getEdge(const Key &k1, const Key &k2) const {
		if(hasKey(k1) && hasKey(k2)) {
			E *edgePtr = edges[vertices.find(k1)->second.second][vertices.find(k2)->second.second];
			if(edgePtr) return *edgePtr;
			else throw invalid_argument("Vertices not connected");
		}
		throw invalid_argument("No vertex mapped to key value");
	}

	// Remove the vertex from the graph associated with a certain key value;
	// no action if the key is not mapped to a vertex
	void removeVertex(const Key &k) {
		if(hasKey(k)) {

			// Get adjacency matrix index mapped to the vertex
			int index = vertices[k].second;

			// Erase key-vertex pair
			vertices.erase(vertices.find(k));

			// Readjust indices
			for(typename map<Key, pair<V, int> >::iterator i = vertices.begin(); i != vertices.end(); ++i) {
				if(i->second.second > index) --(i->second.second);
			}

			// Erase adjacency matrix entries corresponding to v
			for(int i = 0; i < edges.size(); ++i) {
				if(edges[i][index]) delete edges[i][index]; // Deallocate column edges
				if(edges[index][i] && index != i) delete edges[index][i]; // Deallocate row edges
			}

			// Erase row
			edges.erase(edges.begin() + index);

			// Erase column
			for(int i = 0; i < edges.size(); ++i) {
				edges[i].erase(edges[i].begin() + index);
			}
		}
	}

	// Remove an edge associated with a vertex key pair; no action if the
	// vertices are not connected or do not exist
	void removeEdge(const Key &k1, const Key &k2) {
		if(hasKey(k1) && hasKey(k2)) {
			E * &entry = edges[vertices[k1].second][vertices[k2].second];
			if(entry) {
				delete entry;
				entry = (E *) NULL;
			}
		}
	}

	// Tell if a given key value is mapped to a vertex in the graph
	bool hasKey(const Key &k) const {
		return vertices.find(k) != vertices.end();
	}

	// Tell if an edge exists between a vertex key pair
	bool connected(const Key &k1, const Key &k2) const {
		return edges[vertices.find(k1)->second.second][vertices.find(k2)->second.second];
	}

	// Get the number of vertices in the graph
	int numVertices() const {
		return vertices.size();
	}

	// Get a list of the keys in the graph
	vector<Key> keys() const {
		vector<Key> result;
		for(typename map< Key, pair<V, int> >::const_iterator i = vertices.begin();
			i != vertices.end(); ++i) {
			result.push_back(i->first);
		}
		return result;
	}

	// Cycle detection algorithm.
	bool cyclic() const {
		vector< pair<Key, Color> > vertexMarkings;
		for(typename map< Key, pair<V, int> >::const_iterator i = vertices.begin();
			i != vertices.end(); ++i) {
			vertexMarkings.push_back(make_pair(i->first, WHITE));
		}
		for(int i = 0; i < vertexMarkings.size(); ++i) {
			if(vertexMarkings[i].second == WHITE) {
				if(visit(vertexMarkings, i)) return true;
			}
		}
		return false;
	}


	template <typename _Key, typename _V, typename _E>
	friend ostream &operator<<(ostream &output, const Graph<_Key, _V, _E> &g);

private:
	map<Key, pair<V, int> > vertices;
	vector< vector<E *> > edges;

	// Used in traversal algorithms
	enum Color {WHITE, GREY, BLACK};
        
	// Helper function for cycle detection algorithm.
	bool visit(vector< pair<Key, Color> > &vertexMarkings, int v) const {
		vertexMarkings[v].second = GREY;
		for(int i = 0; i < vertexMarkings.size(); ++i) {
			if(connected(vertexMarkings[v].first, vertexMarkings[i].first)) {
				if(vertexMarkings[i].second == GREY) return true;	
				else if(vertexMarkings[i].second == WHITE) {
					if(visit(vertexMarkings, i)) return true;
				}
			}
		}
		vertexMarkings[v].second = BLACK;
		return false;
	}

};

// Output operator, for the sake of debugging and testability
template <typename Key, typename V, typename E>
ostream &operator<<(ostream &output, const Graph<Key, V, E> &g) {
	output << "vertices = {" << endl;
	for(typename map<Key, pair<V, int> >::const_iterator i = g.vertices.begin(), look = g.vertices.begin(); i != g.vertices.end(); ++i) {
		if(look != g.vertices.end()) ++look;
		output << i->first << " => " 
			<< i->second.first << ", "
			<< i->second.second
			<< (look == g.vertices.end() ? "" : ",") << endl;
	}
	output << '}' << endl;
	output << "edges = [" << endl;
	for(int i = 0; i < g.edges.size(); ++i) {
		for(int j = 0; j < g.edges[i].size(); ++j) {
			if(g.edges[i][j]) output << *g.edges[i][j];
			else output << "null";
			output << ' ';
		}
		output << endl;
	}
	output << ']' << endl;
	return output;
}

#endif

