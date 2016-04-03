 #pragma once
#include <iostream>
#include <cstdio>
#include <cstdlib>
#include <vector>
#include <map>


using namespace std;

class Graph
{
	public:
	struct Node
	{
		int lastEdge,label,order;
	};
	struct Edge
	{
		int pre, back, weight, msg,order;
	};//pre-->previous edge at same node; back-->node the edge point to; 
	struct Input
	{
		int order,s,t,w;
	};
	int eNum,nNum;//number of edges and nodes
	int source,target;

	vector<Node>nodes;
	vector<Edge>edges;
	Graph()
	{
		eNum=0;
		nNum=0;
	}
	void write();
	void read();
	void mapAdd(map<int,int >& mapint,int& nNum,int& s,vector<Node>& nodes,int label);
};
void Graph::write()
{
	FILE *fo;

	fo=fopen("output","w");
	fprintf(fo,"%d %d\n", eNum,nNum);
	fprintf(fo,"%d %d\n", source,target);
	fclose(fo);
	//printf("nodes status:\n");
	//for(int i=0;i<nNum;i++)
	//{
	//	printf("%d %d %d\n",i,nodes[i].lastEdge,nodes[i].label);
	//}
	
}

void Graph::mapAdd(map<int,int >& mapint,int& nNum,int& s,vector<Node>& nodes,int label)
{
	if(mapint.find(s)==mapint.end())
	{
		Node temp;
		temp.lastEdge=0;
		temp.label=label;
		temp.order=s;
		nodes.push_back(temp);
		
		mapint[s]=nNum;
		nNum++;
	}
	s=mapint[s];
	return;
}
void Graph::read()
{
	FILE *fi;

	map<int , int >mapint;
	int order,s,t,w;
	//printf("SDFYSIYDF  %d",mapint[5]);
	//========================================Node input========================================
	fi=fopen("demand.csv","r");
	fscanf(fi,"%d,%d,",&source,&target);
	mapAdd(mapint,nNum,source,nodes,1);
	mapAdd(mapint,nNum,target,nodes,1);

	while(fscanf(fi,"%d|",&s)!=-1)
	{
		mapAdd(mapint,nNum,s,nodes,1);
		printf("%d\n",s);
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
	for(int i=0;i<nNum;i++)
		printf("%d ",nodes[i].lastEdge);
	printf("\n");
	for(int i=0;i<eNum;i++)
	{
		order=inputs[i].order;
		s=inputs[i].s;
		t=inputs[i].t;
		w=inputs[i].w;

		Edge temp;
		temp.order=order;
		temp.pre=nodes[s].lastEdge;
		temp.msg=1.0;
		temp.back=t;
		temp.weight=w;
		edges.push_back(temp);
		nodes[s].lastEdge=i+1;
		//printf("%d %d %d %d %d \n",i+1,edges[i+1].pre,inputs[i].s,inputs[i].t,inputs[i].w);
	}
	//printf("nodes status:\n");
	//for(int i=0;i<nNum;i++)
	//{
	//	printf("%d %d %d\n",i,nodes[i].lastEdge,nodes[i].label);
	//}
	//printf("\\0..0/");
	
	fclose(fi);
}