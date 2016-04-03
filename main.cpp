#include <iostream>
#include <cstdio>
#include <cstdlib>
#include "Graph.h"
#include "TSP.h"

int main()
{
	Graph dataGraph;
	//printf("dfajsidfjoa");
	dataGraph.read();
	//printf("\\0..0/");
	
	//dataGraph.write();
	//Graph testGraph(Graph.refine());
	TSP tsp;
	tsp.init(dataGraph);
	
	//tsp.optimize(testGraph);
	//tsp.write();
	return 0;
}
