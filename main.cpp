#include <iostream>
#include <cstdlib>
#include "main.h"
#include "Graph.h"
#include "TSP.h"

int main(int argc, char const *argv[])
{
	Graph dataGraph;
	Graph.read();
	Graph testGraph(Graph.refine());
	TSP tsp;
	tsp.init(testGraph);
	tsp.optimize(testGraph);
	tsp.write();
	return 0;
}