#include <cstdio>
#include <iostream>
#include <cstdlib>
#include <vector>
#include <map>

using namespace std;
struct Node
{
	int lastEdge,label;
};
struct Edge
{
	int pre, back, weight,order;
};//pre-->previous edge at same node; back-->node the edge point to; 
struct Input
{
	int order,s,t,w;
};
int eNum,nNum;
int source,target;

void mapAdd(map<int,int >& mapint,int& nNum,int& s,vector<Node>& nodes,int label)
{
	if(mapint.find(s)==mapint.end())
	{
		Node temp;
		temp.lastEdge=0;
		temp.label=label;
		nodes.push_back(temp);
		
		mapint[s]=nNum;
		nNum++;
	}
	s=mapint[s];
	return;
}
int main()
{
	int order,s,t,w;
	FILE *fi;

	map<int , int >mapint;
	//printf("SDFYSIYDF  %d",mapint[5]);
	vector<Node>nodes;
	vector<Edge>edges;
	//========================================Node input========================================
	fi=fopen("demand.csv","r");
	fscanf(fi,"%d,%d,",&source,&target);
	mapAdd(mapint,nNum,source,nodes,0);
	mapAdd(mapint,nNum,target,nodes,0);

	while(fscanf(fi,"%d|",&s)!=-1)
	{
		printf("%d\n",s);
		mapAdd(mapint,nNum,s,nodes,1);
	}
	printf("%d,%d\n",source,target);
	fclose(fi);

	//========================================Edge Input=========================================
	fi=fopen("topo.csv","r");
	vector<Input>inputs;
	while(fscanf(fi,"%d,%d,%d,%d",&order,&s,&t,&w)!=-1)
	{
		eNum++;
		mapAdd(mapint,nNum,s,nodes,0);
		mapAdd(mapint,nNum,t,nodes,0);
		Input temp;
		temp.order=order,temp.s=s,temp.t=t,temp.w=w;
		inputs.push_back(temp);
	}

	Edge tempZero;
	tempZero.order=-1;
	edges.push_back(tempZero);
	for(int i=0;i<eNum;i++)
	{
		order=inputs[i].order;
		s=inputs[i].s;
		t=inputs[i].t;
		w=inputs[i].w;

		Edge temp;
		temp.order=order;
		temp.pre=nodes[s].lastEdge;
		temp.back=t;
		temp.weight=w;
		edges.push_back(temp);
		nodes[s].lastEdge=i+1;
		printf("%d %d %d %d \n",edges[i].order,inputs[i].s,inputs[i].t,inputs[i].w);
	}
	//printf("nodes status:\n");
	//for(int i=0;i<nNum;i++)
	//{
	//	printf("%d %d %d\n",i,nodes[i].lastEdge,nodes[i].label);
	//}
	fclose(fi);
}