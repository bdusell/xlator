#include "Graph.h"
#include <string>
#include <iostream>
#include <istream>
#include <fstream>
#include <cstdlib>
using namespace std;

int main(int argc, char **argv) {

	Graph<int, string, float> G;

	string cmd;

	int k1, k2;
	string v1, v2;
	float weight;

	cout << G << endl;

	while(cin) {
		cout << "graph_test>> ";
		cin >> cmd;
		if(cmd == "setVertex") {
			cin >> k1 >> v1;
			G.setVertex(k1, v1);
			cout << G << endl;
		}
		else if(cmd == "getVertex") {
			try {
				cin >> k1;
				cout << G.getVertex(k1) << endl;
			}
			catch(invalid_argument &e) {
				cout << "Error: " << e.what() << endl;
			}
		}
		else if(cmd == "setEdge") {
			try {
				cin >> k1 >> k2 >> weight;
				G.setEdge(k1, k2, weight);
				cout << G << endl;
			}
			catch(invalid_argument &e) {
				cout << "Error: " << e.what() << endl;
			}
		}
		else if(cmd == "getEdge") {
			try {
				cin >> k1 >> k2;
				cout << G.getEdge(k1, k2) << endl;
			}
			catch(invalid_argument &e) {
				cout << "Error: " << e.what() << endl;
			}

		}
		else if(cmd == "removeVertex") {
			cin >> k1;
			G.removeVertex(k1);
			cout << G << endl;
		}
		else if(cmd == "removeEdge") {
			cin >> k1 >> k2;
			G.removeEdge(k1, k2);
			cout << G << endl;
		}
		else if(cmd == "hasKey") {
			cin >> k1;
			cout << (G.hasKey(k1) ? "yes" : "no") << endl;
		}
		else if(cmd == "connected") {
			cin >> k1 >> k2;
			cout << (G.connected(k1, k2) ? "yes" : "no") << endl;
		}
		else if(cmd == "numVertices") {
			cout << G.numVertices() << endl;
		}
		else if(cmd == "keys") {
			vector<int> graphKeys = G.keys();
			for(int i = 0; i < graphKeys.size(); ++i) {
				cout << graphKeys[i] << ' ';
			}
			cout << endl;
		}
		else if(cmd == "cyclic") {
			cout << (G.cyclic() ? "yes" : "no") << endl;
		}
		else if(cmd == "help") {
			system("cat graph_test_help.txt");
		}
		else if(cmd == "exit") {
			break;
		}
	}

}

