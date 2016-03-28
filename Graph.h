#include <iostream>
#include <cstdlib>

struct Graph::Node
{
	int lastEdge,label;
};
struct Graph::Edge
{
	int pre, back, weight;
};//pre-->previous edge at same node; back-->node the edge point to; 
class Graph
{
	int eNum,nNum;//number of edges and nodes
	int source,target;

	vector<Node>nodes;
	vector<Edge>edges;
	Graph()
	{
		
	}
	void write();
	void read();
	void refine();
};