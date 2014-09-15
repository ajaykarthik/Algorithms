/*
This program implements the Travelling saleman problem
This solution finds the shortest path of a tour given a starting and end point can be used 
in conjunction with the max space K clustering to find subproblems in some isntances of the TSP
*/
#include<iostream>
#include<fstream>
#include<math.h>
#include<vector>
#include<map>
#include<time.h>

using namespace std;

struct node
{
	double x;
	double y;
};

//funciton declarations
void readGraph(vector<node>& graph);
void printGraph(vector<node>& graph);
void cmb(int depth, int index, int n, int m, int* buffer);
double minTSPcost(vector<node>& graph);

vector<int> sequence;

//globals
int numCities;
int buffer[32];

//constants
const double MAX_DIST = 999999999;

void main()
{

	vector<node> graph;
	readGraph(graph);
	//printGraph(graph); //test
	clock_t t = clock();
	cout << minTSPcost(graph) << endl;
	t = clock() - t;
	//cout << t << endl;
	system("pause");
}

void printGraph(vector<node>& graph)
{
	for (int i = 0; i < numCities; i++)
	{
		cout << graph[i].x << '\t' << graph[i].y << endl;
	}
}

void readGraph(vector<node>& graph)
{
	ifstream input("test.txt");
	input >> numCities;
	node currentCity;
	for (int i = 0; i < numCities; i++)
	{
		input >> currentCity.x;
		input >> currentCity.y;
		graph.push_back(currentCity);
	}
}

double minTSPcost(vector<node>& graph)
{
	map<int, double> costs1, costs2; // the int is the unique number generated by the sequence, double is the cost
	costs1.insert(make_pair(1, 0)); //base case A[S,1]; S = {1} j bits -> 26 to 30; 2^25 = 1st bit 
	costs2.insert(costs1.begin(), costs1.end());
	costs1.clear();
	for (int m = 2; m <= numCities; m++)
	{
		//current sequence
		sequence.clear();
		cmb(0, 1, numCities, m - 1, buffer); //1 is always in the sequence
		for (int s = 0; s < sequence.size(); s++) //use powered sum of sets instead of explicit set	
		{
			int sequenceValue = sequence[s];
			vector<int> currentBits; //bits in sequence
			int tempJValue = sequenceValue;
			int tempKValue = tempJValue;
			//convert to binary
			while (tempJValue >= 1)
			{
				if (tempJValue % 2 == 0)
				{
					currentBits.push_back(0);
				}
				else
				{
					currentBits.push_back(1);
				}
				tempJValue /= 2;
			}
			//reverse(currentBits.begin(), currentBits.end());
			int tempJIndex = 1; //index 0 is 1
			for (int j = 1; j < m; j++)
			{
				while (currentBits[tempJIndex] == 0)
				{
					tempJIndex++;
				}
				//remove currentJBit number
				int currentSequenceValue = sequenceValue - pow(2, tempJIndex);
				double currentMin = MAX_DIST;
				double currentCost;
				int tempKIndex = 0;
				for (int k = 0; k < m; k++)
				{
					while (currentBits[tempKIndex] == 0)
					{
						tempKIndex++;
					}
					if (tempKIndex != tempJIndex && costs2.find(currentSequenceValue + tempKIndex*pow(2, 25)) != costs2.end()) // k!=j and entry is defined
					{
						currentCost = costs2.find(currentSequenceValue + tempKIndex * pow(2, 25))->second +
							(sqrt((graph[tempJIndex].x - graph[tempKIndex].x)*(graph[tempJIndex].x - graph[tempKIndex].x) + (graph[tempJIndex].y - graph[tempKIndex].y)*(graph[tempJIndex].y - graph[tempKIndex].y))); //eucledian distnace
						if (currentCost < currentMin)
						{
							currentMin = currentCost;
						}
					}
					tempKIndex++;

				}
				costs1.insert(make_pair(sequenceValue + tempJIndex * pow(2, 25), currentMin));
				tempJIndex++;
			}
		}
		costs2.clear();
		costs2 = costs1;
		costs1.clear();
	}

	int allNodeValue = 0;
	for (int i = 0; i < numCities; i++)
	{
		allNodeValue += pow(2, i);
	}
	return costs2.find(allNodeValue + pow(2, 25))->second;
}

//ref: bayram: coursera
void cmb(int depth, int index, int n, int m, int* buffer)
{
	if (depth > m - 1)
	{
		unsigned long int num = 0;
		for (int j = 0; j < m; j++)
		{
			//generate an integer corresponding to this element
			num = num | (1 << (buffer[j] - 1));
		}
		sequence.push_back(++num); //add the first bit (1)
	}
	else
	{
		for (int p = index; p < n - m + 1 + depth; p++)
		{
			buffer[depth] = p + 1;
			cmb(depth + 1, p + 1, n, m, buffer);
		}
	}
}