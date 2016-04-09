#include <iostream>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <cmath>
#include <vector>
#include "Graph.h"
using namespace std;

const int antNum=33; //蚂蚁数量
const double ALPHA=1.0; //启发因子，信息素的重要程度
const double BETA=0.3;   //期望因子，城市间距离的重要程度
const double ROU=0.8; //信息素残留参数
const double DBQ=100.0; //总的信息素
const double DB_MAX=10e9; //一个标志数，10的9次方
//const double RAND_MAX=32767;   //

class CAnt
{
	public:
		double totLength;
		int pathNum;
		int pRec[21];
	public:
		CAnt(void);
		~CAnt(void);
	public:
		int* m_nPath; //长者走的路径
		double m_dbPathLength; //长者走过的路径长度
		int* m_nAllowedCity; //长者没去过的国家
		//int m_nUnMovedCity[N_CITY_COUNT]; //长者没去过的西方国家
		int m_nCurCityNo; //当前所在国家编号
		int m_nMovedCityCount; //已经去过的西方国家数量
	public:
 		int notCross(int *a,int *b);
		void makeCross(int *a,int *b);
		int ChooseNextCity(Graph& dataGraph); //选择下一个国家
		void Init(Graph &dataGraph); //初始化
		int Move(Graph &dataGraph); //蚂蚁在国家间移动
		void Search(Graph &dataGraph); //搜索路径
		void CalPathLength(Graph &dataGraph); //计算蚂蚁走过的路径长度
		double rnd(double nLow,double nUpper);
};
//构造函数
CAnt::CAnt(void)
{
}
//析构函数
CAnt::~CAnt(void)
{
}

double CAnt::rnd(double nLow,double nUpper)
{
	return nLow+(nUpper-nLow)*((double)rand()/(RAND_MAX));
}

//初始化函数，蚂蚁搜索前调用
void CAnt::Init(Graph &dataGraph)
{
	int n=dataGraph.nNum;
	m_nPath=new int[n];
	m_nAllowedCity=new int[n];
	for (int i=0;i<n;i++)
	{
		m_nAllowedCity[i]=1; //设置全部城市为没有去过
		m_nPath[i]=0; //蚂蚁走的路径全部设置为0
	}
	//蚂蚁走过的路径长度设置为0
	m_dbPathLength=0.0;
	pathNum=0;
	//随机选择一个出发城市
	m_nCurCityNo=dataGraph.source;
	//把出发城市保存入路径数组中
	//m_nPath[0]=m_nCurCityNo;
	//标识出发城市为已经去过了
	m_nAllowedCity[m_nCurCityNo]=0;
	//已经去过的城市数量设置为1
	m_nMovedCityCount=1;
}
//选择下一个城市
//返回值 为城市编号
int CAnt::notCross(int *a,int *b)
{
	for(int i=0;i<20;i++)
		if(a[i]&b[i])
			return 0;
	return 1;
}
void CAnt::makeCross(int *a,int *b)
{
	for(int i=0;i<=20;i++)
		a[i]=(a[i]|b[i]);
}
int CAnt::ChooseNextCity(Graph& dataGraph)
{
	int n=dataGraph.nNum;
	int nSelectedCity=-1; //返回结果，先暂时把其设置为-1
	//==============================================================================
	//计算当前城市和没去过的城市之间的信息素总和
	
	double dbTotal=0.0;   
	double prob[n]; //保存各个城市被选中的概率
	//int pRec[21];
	//memset(pRec,0,sizeof(pRec));
	
	for(int i = dataGraph.nodes[m_nCurCityNo].lastEdge; i; i = dataGraph.edges[i].pre)
	if(notCross(dataGraph.edges[i].path,pRec))
	{
		int orient = dataGraph.edges[i].back;
		//if(dataGraph.nodes[orient].order==7&&dataGraph.nodes[m_nCurCityNo].order==3)
		//	printf("^(&*^(*&(*^(*&(& %d %d\n",m_nCurCityNo,orient);
		if (m_nAllowedCity[orient] == 1) //城市没去过
		{
			prob[orient]+=pow(dataGraph.edges[i].msg,ALPHA);//*pow((1.0+dataGraph.nodes[orient].label)/dataGraph.edges[i].weight,BETA); //该城市和当前城市间的信息素
			dbTotal=dbTotal+prob[orient]; //累加信息素，得到总和
		}
		else //如果城市去过了，则其被选中的概率值为0
		{
			prob[orient]=0.0;
		}
	}
	//==============================================================================
	//进行轮盘选择
	double dbTemp=0.0;
	if (dbTotal > 0.0) //总的信息素值大于0
	{
		dbTemp=rnd(0.0,dbTotal); //取一个随机数

		for(int i = dataGraph.nodes[m_nCurCityNo].lastEdge; i; i = dataGraph.edges[i].pre)
		if(notCross(dataGraph.edges[i].path,pRec))
		{
			int orient = dataGraph.edges[i].back;
			if (m_nAllowedCity[orient] == 1) //城市没去过
			{
				dbTemp=dbTemp-prob[orient]; //这个操作相当于转动轮盘，如果对轮盘选择不熟悉，仔细考虑一下
				if (dbTemp < 0.0) //轮盘停止转动，记下城市编号，直接跳出循环
  				{
 					m_nPath[pathNum]=i;
	 				pathNum++;
 					nSelectedCity=orient;
 					makeCross(pRec,dataGraph.edges[i].path);
 					totLength+=dataGraph.edges[i].weight;
					break;
				}
			}
		}
	}
	//==============================================================================
	if (nSelectedCity == -1)
	{
		for(int i = dataGraph.nodes[m_nCurCityNo].lastEdge; i; i = dataGraph.edges[i].pre)
		{
			int orient = dataGraph.edges[i].back;
			if (m_nAllowedCity[orient] == 1) 
			{
				nSelectedCity=orient;
				makeCross(pRec,dataGraph.edges[i].path);
				totLength+=dataGraph.edges[i].weight;
 				m_nPath[pathNum]=i;
 				pathNum++;
				break;
			}
		}
	}
	//==============================================================================
	return nSelectedCity;
}

//蚂蚁在城市间移动
int CAnt::Move(Graph& dataGraph)
{
	//printf("%d-->",dataGraph.nodes[m_nCurCityNo].order);
	int nCityNo=ChooseNextCity(dataGraph); //选择下一个城市
	//printf("%d\n",dataGraph.nodes[nCityNo].order);
	m_nAllowedCity[nCityNo]=0;//把这个城市设置成已经去过了
	m_nCurCityNo=nCityNo; //改变当前所在城市为选择的城市
	if(dataGraph.nodes[nCityNo].label == 1)
		m_nMovedCityCount++; //已经去过的城市数量加1
	return nCityNo;
}
//蚂蚁进行搜索一次
void CAnt::Search(Graph& dataGraph)
{
	Init(dataGraph); //蚂蚁搜索前，先初始化
	//如果蚂蚁去过的城市数量小于城市数量，就继续移动
	memset(pRec,0,sizeof(pRec));
	totLength=0;
	while(1)
	{
		int k=Move(dataGraph);
		//printf("%d\n",k);
		if(k==-1||k==dataGraph.target)
			break;
	}
	//完成搜索后计算走过的路径长度
 	CalPathLength(dataGraph);
}

//计算蚂蚁走过的路径长度
void CAnt::CalPathLength(Graph& dataGraph)
{
	m_dbPathLength=totLength;
	m_dbPathLength-=12000*m_nMovedCityCount;
}
class TSP
{
	CAnt m_cAntAry[antNum]; //蚂蚁数组
	CAnt m_cBestAnt;
public:
	TSP();
	~TSP();

	void init(Graph& dataGraph);
	//开始搜索
	void Search(Graph& dataGraph);
	//更新环境信息素
	void UpdateMsg(Graph& dataGraph, int x);
	void optimize(Graph& dataGraph);
	double logistic(double x);
};

TSP::TSP()
{
}
TSP::~TSP()
{
}
double TSP::logistic(double x)
{
	return 5.0/(1.0+exp(-x));
}

void TSP::init(Graph& dataGraph)
{
	m_cBestAnt.m_dbPathLength=DB_MAX;
	m_cBestAnt.Init(dataGraph);
	Search(dataGraph); //开始搜索
	//输出结果
	printf("\nThe best tour is :\n");
	FILE *fo;

	fo=fopen("result.csv","w");
	int na=0;
	for(int i=0;i<dataGraph.nNum;i++)
		if(dataGraph.nodes[i].label==1&&i!=dataGraph.source)
		{
			int j=0;
			for (j=0;j<m_cBestAnt.pathNum;j++)
				if(dataGraph.edges[m_cBestAnt.m_nPath[j]].back==i)
					break;
			if(j==m_cBestAnt.pathNum)
			{
				na=1;
				break;
			}
		}
	if(na)
		fprintf(fo,"na\n");
	int i;
	double expense=0;
	for (i=0;i<m_cBestAnt.pathNum-1;i++)
	{
		vector<int>::iterator it;
		for(it  = dataGraph.edges[m_cBestAnt.m_nPath[i]].subEdgeOrders.begin();it != dataGraph.edges[m_cBestAnt.m_nPath[i]].subEdgeOrders.end();it++) 
		{
			printf("%d ",*(it));
			fprintf(fo,"%d|",*(it));
			//printf("%d|",dataGraph.nodes[dataGraph.edges[m_cBestAnt.m_nPath[i]].back].order);
			expense+=dataGraph.edges[m_cBestAnt.m_nPath[i]].weight;
		}
		//if (i % 20 == 0)
		//	printf("\n");
		//printf(cBuf);
	}
	expense+=dataGraph.edges[m_cBestAnt.m_nPath[i]].weight;
	vector<int>::iterator it;
	for(it  = dataGraph.edges[m_cBestAnt.m_nPath[i]].subEdgeOrders.begin();it != dataGraph.edges[m_cBestAnt.m_nPath[i]].subEdgeOrders.end();it++) 
	{
		printf("%d ",*(it));
		if(it != dataGraph.edges[m_cBestAnt.m_nPath[i]].subEdgeOrders.end()-1)
			fprintf(fo,"%d|",*(it));
		else
			fprintf(fo,"%d",*(it));
		//printf("%d|",dataGraph.nodes[dataGraph.edges[m_cBestAnt.m_nPath[i]].back].order);
		expense+=dataGraph.edges[m_cBestAnt.m_nPath[i]].weight;
	}
	fclose(fo);
	//printf("\n\nPress any key to exit!");
	//getchar();
}
void TSP::UpdateMsg(Graph& dataGraph,int x)
{
	//临时数组，保存各只蚂蚁在两两城市间新留下的信息素
	double dbTempAry[dataGraph.eNum];
	memset(dbTempAry,0,sizeof(dbTempAry)); //先全部设置为0
	//计算新增加的信息素,保存到临时数组里
	int m=dataGraph.eNum;
	int n=dataGraph.nNum;
	for (int i=0;i<antNum;i++)
	{
		for(int j=0; j<m_cAntAry[i].pathNum;j++)
		{
			int now=m_cAntAry[i].m_nPath[j];
			dbTempAry[now]+=(DBQ)/m_cAntAry[i].m_dbPathLength*m_cAntAry[i].m_nMovedCityCount;
		}
	}
    //==================================================================
    //更新环境信息素
	double _max=0;
	for (int i=0;i<=m;i++)
		if(dbTempAry[i]>_max)
			_max=dbTempAry[i];
	for (int i=0;i<=m;i++)
		//if(x==1)
			dataGraph.edges[i].msg=1;
		//else
			//dataGraph.edges[i].msg=dataGraph.edges[i].msg*ROU+logistic(dbTempAry[i]-_max/2)*0.1; //最新的环境信息素 = 留存的信息素 + 新留下的信息素
}

void TSP::Search(Graph& dataGraph)
{
	char cBuf[256]; //打印信息用
	//在迭代次数内进行循环
	int maxj=0;
	for (int i=0;i<500;i++)
	{
	//每只蚂蚁搜索一遍
		for (int j=0;j<antNum;j++)
		{
			//printf("++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++\n");
			//printf("==%d==\n",j);
			m_cAntAry[j].Search(dataGraph);
		}
		//保存最佳结果
		for (int j=0;j<antNum;j++)
		{
			//for(int k=0;k<m_cBestAnt.pathNum;k++)
			//		printf("%d ",dataGraph.nodes[dataGraph.edges[m_cAntAry[j].m_nPath[k]].back].order);
			if (m_cAntAry[j].m_dbPathLength < m_cBestAnt.m_dbPathLength)
			{
				//printf("--%d00", m_cAntAry[j].pathNum);
				//m_cBestAnt.Init(dataGraph);
				m_cBestAnt.pathNum=m_cAntAry[j].pathNum;
				printf("\n=%d=\n",i);
				for(int k=0;k<m_cBestAnt.pathNum;k++)
				{
					m_cBestAnt.m_nPath[k]=m_cAntAry[j].m_nPath[k];
					printf("%lf ",dataGraph.edges[m_cAntAry[j].m_nPath[k]].msg);

				}
				printf("\n=\n");
				m_cBestAnt.m_dbPathLength=m_cAntAry[j].m_dbPathLength;
				maxj=j;
			}
			//printf("\n%d %.0f",j+1,m_cAntAry[j].m_dbPathLength);
		}
		//更新环境信息素
		if(i%39==38)
			UpdateMsg(dataGraph,1);
		else
			UpdateMsg(dataGraph,0);
		//输出目前为止找到的最优路径的长度
	}
	printf("\n%d %.0f %d",m_cBestAnt.pathNum,m_cBestAnt.m_dbPathLength,maxj);
}
