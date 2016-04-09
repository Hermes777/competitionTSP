 #pragma once
#include <iostream>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <vector>
#include <map>
#include <queue>   

using namespace std;

class Graph
{
	public:
	struct Node
	{
		int lastEdge,label,order,h;
	};
	struct Edge
	{
		int pre, back,order;
		int path[21];
		double weight,msg;
		vector<int> subEdgeOrders;
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
	void reverse(Graph& dataGraph,int st,int pRec[][21],int eRec[][601],int nRec[]);
	Graph refinement(Graph& dataGraph);
	void heap_down(int *a,int i,int n);
	void heap_up(int *a,int i,int n);
	void heap_add(int *a,int *s,int& p,int value);
	int heap_pop(int *a,int& p);
	int Astar(Graph& dataGraph,int K,int st,int en,int* pRec);
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
void Graph::heap_down(int *a,int i,int n)
{
	if(2*i+1>n-1)
		return;
	if(2*i+2>n-1)
	{
		if(a[i]>a[i*2+1])
		{
			int k=a[i*2+1];
			a[i*2+1]=a[i];
			a[i]=k;
			heap_down(a,i*2+1,n);
		}
		return;
	}
	if(a[i]>=a[i*2+1]&&a[i]>=a[i*2+2])
		return;
	if(a[i*2+1]<a[i*2+2])
	{
		int k=a[i*2+1];
		a[i*2+1]=a[i];
		a[i]=k;
		heap_down(a,i*2+1,n);
	}
	else
	{
		int k=a[i*2+2];
		a[i*2+2]=a[i];
		a[i]=k;
		heap_down(a,i*2+2,n);
	}
}
void Graph::heap_up(int *a,int i,int n)
{
	if(i<=0)
		return;
	if(a[i]<a[(i-1)/2])
	{
		int k=a[i];
		a[i]=a[(i-1)/2];
		a[(i-1)/2]=k;
		heap_up(a,(i-1)/2,n);
	}
	return;
}
void Graph::heap_add(int *a,int *s,int& p,int value)
{
	a[p]=value;
	heap_up(a,p,p);
	p++;
	return;
}
int Graph::heap_pop(int *a,int& p)
{
	int value=a[0];
	p--;
	a[0]=a[p];
	heap_down(a,0,p);
	return value;
}
struct Statement  
{  
       int v,d,h,from;  
       int path[21];
       bool operator <( Statement a )const  
       {    return a.d+a.h<d+h;   }  
};  //
Graph Graph::refinement(Graph& dataGraph)
{
	int n=dataGraph.nNum;
	Graph newGraph;
	int num[51];
	newGraph.nNum=0;
	for(int i=0;i<n;i++)
	if(dataGraph.nodes[i].label==1)
	{
		Node temp;
		temp.lastEdge=0;
		temp.order=dataGraph.nodes[i].order;
		temp.label=1;
		num[newGraph.nNum]=i;
		newGraph.nodes.push_back(temp);
		if(dataGraph.source==i)
			newGraph.source=newGraph.nNum;
		if(dataGraph.target==i)
			newGraph.target=newGraph.nNum;
		newGraph.nNum++;
	}

	int pRec[601][21];
	int eRec[601][601],nRec[601];//eRec-->edges,nRec-->number of edges
	/*
	for(int i=0;i<newGraph.nNum;i++)
		for(int j=0;j<newGraph.nNum;j++)
		if(i!=j)
		{
			printf("**%d %d\n",dataGraph.nodes[num[i]].order,dataGraph.nodes[num[j]].order);
			Edge temp;
			temp.pre=newGraph.nodes[i].lastEdge;
			temp.msg=1.0;
			temp.back=j;
			temp.weight=Astar(dataGraph,1,i,j,pRec);
			//printf("0000000000000\n");
			for(int k=0;k<=20;k++)
				temp.path[k]=pRec[k];
			newGraph.edges.push_back(temp);
			newGraph.nodes[i].lastEdge=i+1;
		}
		*/

	Edge tempZero;
	tempZero.order=-1;
	newGraph.edges.push_back(tempZero);
	newGraph.eNum=0;


	for(int i=0;i<newGraph.nNum;i++)
	{
		reverse(dataGraph,num[i],pRec,eRec,nRec);
		printf("=======================  %d==========================",newGraph.nodes[i].order);
		for(int j=0;j<newGraph.nNum;j++)
		if(i!=j&&dataGraph.nodes[num[j]].h<20000)
		{
			printf("\nk= %d has dis= %d\n",newGraph.nodes[j].order,dataGraph.nodes[num[j]].h);
			printf("%x\n",pRec[num[j]][0]);
			newGraph.eNum++;
			Edge temp;
			temp.pre=newGraph.nodes[j].lastEdge;
			temp.msg=1.0;
			temp.back=i;
			temp.weight=dataGraph.nodes[num[j]].h;

			for(int k=0;k<nRec[num[j]];k++)
			{
				printf("%d\n",dataGraph.edges[eRec[num[j]][k]].order);
				temp.subEdgeOrders.push_back(eRec[num[j]][k]);
			}
			for(int k=0;k<=20;k++)
				temp.path[k]=pRec[num[j]][k];
			newGraph.edges.push_back(temp);
			newGraph.nodes[j].lastEdge=newGraph.eNum;
		}
	}
	return newGraph;
}
/*
int Graph::Astar(Graph& dataGraph,int K,int st,int en,int* pRec)
{
	reverse(dataGraph,en);
	printf("%d %d\n",st,en);
	
	Statement cur,next;
	//priority_queue<Q>q;   
	priority_queue<Statement>FstQ;  
	int cnt[601]; 
	int from[601];
	for(int i=0;i<dataGraph.nNum;i++)
		from[i]=-1;
	memset(cnt,0,sizeof(cnt));  
	memset(pRec,0,sizeof(pRec));  
	cur.v=st;  
	cur.d=0;
	//from[st]=-1;
	cur.h=dataGraph.nodes[st].h;

	FstQ.push(cur);  
	printf("==============\n");
	while(!FstQ.empty()) 
	{
		cur=FstQ.top();  
		FstQ.pop(); 

 		cnt[cur.v]++;  
		if(cnt[cur.v]>K)
			continue;  
		if(cnt[en]==K)
		{
			int now_state=en;
			printf("0000000000000\n");
			for(int i=0;i<20;i++)
				printf("%d ",from[i]);
			printf("\n");
			while(from[now_state]!=st)
			{
				//printf("%d\n",now_state);
				int value=now_state;
				pRec[value/30]=(pRec[value/30]|(1<<(value%30)));
				now_state=from[now_state];
			}
			return cur.d;  
		}
		if(dataGraph.nodes[cur.v].label==1&&cur.v!=st)
			continue;
		printf("%d %d \n",dataGraph.nodes[cur.v].order,dataGraph.nodes[cur.v].h);
		for( int temp=dataGraph.nodes[cur.v].lastEdge;temp;temp=dataGraph.edges[temp].pre)  
		{
			int v=dataGraph.edges[temp].back;  
			next.d=cur.d+dataGraph.edges[temp].weight;  
			next.v=v;  
			for(int i=0;i<20;i++)
				next.path[i]=cur.path[i]
			pRec[value/30]=(pRec[value/30]|(1<<(value%30)));

			from[next.v]=cur.v;
			next.h=dataGraph.nodes[v].h;  
			FstQ.push(next);  
		}  
	} 
	return -1;  
}*/
void Graph::reverse(Graph& dataGraph,int st,int pRec[][21],int eRec[][601],int nRec[])
{
	Graph counterGraph;
	for(int i=0;i<nNum;i++)
	{
		Node temp;
		temp.lastEdge=0;
		counterGraph.nodes.push_back(temp);
	}
	
	Edge tempZero;
	tempZero.order=-1;
	counterGraph.edges.push_back(tempZero);
	counterGraph.eNum=0;
	int from[601];
	for(int i=0;i<dataGraph.nNum;i++)
		from[i]=-1;
	int frome[601];
	for(int i=0;i<dataGraph.nNum;i++)
		frome[i]=0;

	
	int n=dataGraph.nNum;
	for(int i=0;i<n;i++)
		for(int j = dataGraph.nodes[i].lastEdge; j; j = dataGraph.edges[j].pre)
		{
			int v=dataGraph.edges[j].back;

			counterGraph.eNum++;
			Edge temp;
			temp.pre=counterGraph.nodes[v].lastEdge;
			temp.back=i;
			temp.weight=dataGraph.edges[j].weight;
			temp.order=dataGraph.edges[j].order;
			counterGraph.edges.push_back(temp);
			counterGraph.nodes[v].lastEdge=counterGraph.eNum;
		}
	
	bool vis[601];
	int h[601];
	memset(vis,0,sizeof(vis));
	for(int i=0;i<n;i++)
		dataGraph.nodes[i].h=10000000;
	dataGraph.nodes[st].h=0;

	
	for(int i=0;i<n;i++)
	{
		int _min=2100000000;
		int k=-1;
		for(int j=0;j<n;j++)
		{
			if((dataGraph.nodes[j].label!=1||j==st) && vis[j]==false && _min>dataGraph.nodes[j].h)  
	               		_min=dataGraph.nodes[j].h,k=j;
		}
		if(k==-1||_min>=10000000)
			break;  
		vis[k]=true;  
		for(int temp=counterGraph.nodes[k].lastEdge;temp;temp=counterGraph.edges[temp].pre)
		{
			int v=counterGraph.edges[temp].back;
			if(dataGraph.nodes[v].h>dataGraph.nodes[k].h+counterGraph.edges[temp].weight)
			{
				dataGraph.nodes[v].h=dataGraph.nodes[k].h+counterGraph.edges[temp].weight;
				from[v]=k;
				frome[v]=temp;
			}
		}
	}
	for(int k=0;k<n;k++)
		if(dataGraph.nodes[k].label==1&&dataGraph.nodes[k].h<=20000)
		{
			for(int i=0;i<21;i++)
				pRec[k][i]=0;
			int now_state=k;
			nRec[k]=0;
			while(from[now_state]!=-1)
			{
				//printf("%d\n",now_state);
				int value=now_state;
				eRec[k][nRec[k]]=counterGraph.edges[frome[now_state]].order;
				nRec[k]++;
				if(now_state!=k)
				pRec[k][value/30]=(pRec[k][value/30]|(1<<(value%30)));
				now_state=from[now_state];
			}
		}
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
		//printf("%d-->%d\n",s,t);
		Input temp;
		temp.order=order,temp.s=s,temp.t=t,temp.w=w;
		inputs.push_back(temp);
	}

	Edge tempZero;
	tempZero.order=-1;
	edges.push_back(tempZero);
	for(int i=0;i<nNum;i++)
	{
		printf("%d ",nodes[i].lastEdge);

	}

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
	//printf("\n===========\n");
	//printf("nodes status:\n");
	//for(int i=0;i<nNum;i++)
	//{
	//	printf("%d %d %d\n",i,nodes[i].lastEdge,nodes[i].label);
	//}
	//printf("\\0..0/");
	
	//printf("====**=====1");
	fclose(fi);
	//printf("====**=====");
	
	return;
}