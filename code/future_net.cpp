#include <iostream>
#include <cstdio>
#include <cstdlib>
#include <ctime>
#include "Graph.h"
#include "TSP.h"

int main()
{
	srand(time(NULL));
	Graph dataGraph;
	//printf("dfajsidfjoa");
	dataGraph.read();
	printf("====**=====");
	Graph testGraph;
	testGraph=dataGraph.refinement(dataGraph);
	//printf("ans= %d\n",dataGraph.Astar(dataGraph,1,dataGraph.source,dataGraph.target));
	//printf("\\0..0/");
	
	//dataGraph.write();
	//Graph testGraph(Graph.refine());
	//TSP tsp;
	//tsp.init(dataGraph);
	
	//tsp.optimize(testGraph);
	//tsp.write();
	return 0;
}
