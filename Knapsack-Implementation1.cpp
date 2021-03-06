/*
This program implements the Knapsack problem finding the optimal solution
Given the Max_Weight, list of items with their values and weights respectively as an input file
ip format:
[knapsack_size][number_of_items]
[value_1] [weight_1]
[value_2] [weight_2]
*/
#include<iostream>
#include<string>
#include<fstream>
#include<vector>
#include<algorithm>

using namespace std;

int knapsackWeight = 0;
int numItems = 0;

struct knapsack
{
	int value;
	int weight;
};

//function headers
void printSack(vector<knapsack> &sack);
void readInput(string input, vector<knapsack> &sack);
int FindOptimalSolution(vector<knapsack> &sack, int** knapSackArray);


void main()
{
	vector<knapsack> sack;
	readInput("test.txt",sack);
	//dynamicall allocate 2D array
	int** knapSackArray = new int*[numItems+1];
	for (int i = 0; i <= numItems; i++)
	{
		knapSackArray[i] = new int[knapsackWeight+1];
	}
	//printSack(sack); //test
	cout << FindOptimalSolution(sack,knapSackArray)<<endl;
	
	system("pause");
}

int FindOptimalSolution(vector<knapsack> &sack,int** knapSackArray)
{
	//set inital values
	for (int i = 0; i <= knapsackWeight; i++)
	{
		knapSackArray[0][i] = 0;
	}
	for (int i = 0; i <= numItems; i++)
	{
		knapSackArray[i][0] = 0;
	}
	for (int i = 1; i <= numItems; i++)
	{
		for (int j = 0; j <= knapsackWeight; j++)
		{
			knapsack currentItem = sack[i-1];
			int xwi = j - currentItem.weight;
			if (xwi < 0)
			{
				knapSackArray[i][j] = knapSackArray[i - 1][j];
			}
			else
			{
				knapSackArray[i][j] = max(knapSackArray[i - 1][j], knapSackArray[i - 1][xwi] + currentItem.value);
			}
		}
	}
	return(knapSackArray[numItems][knapsackWeight]);
}

void printSack(vector<knapsack> &sack)
{
	for (int i = 0; i < sack.size(); i++)
	{
		cout << sack[i].value << '\t' << sack[i].weight << endl;
	}
}

void readInput(string input,vector<knapsack> &sack)
{
	ifstream file(input);
	file >> knapsackWeight;
	file >> numItems;
	for (int i = 0; i < numItems;i++)
	{
		knapsack newSack;
		file >> newSack.value;
		file >> newSack.weight;
		sack.push_back(newSack);
	}
}