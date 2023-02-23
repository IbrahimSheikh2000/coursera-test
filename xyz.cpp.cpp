#include <iostream>
#include <fstream>
#include <bits/stdc++.h>
#include <string>
using namespace std;
#define n 5
#define infinity INT_MAX
int f_path[n+1]; //salesman final path
bool traversed[n]; // checks if the city is already visited or not
int OptimalSolution = infinity; // stores the minimum cost


void Final_path(int current_path[]) // this functions takes the value in current path and stores them in final path
{
	for (int i=0; i<n; i++){
		f_path[i] = current_path[i];
	f_path[n] = current_path[0];
}
}


int f_Min(int costMatrix[n][n], int i) // we calculate the first minimum edge cost
{
	int minimum = infinity;
	for (int j=0; j<n; j++)
		if (costMatrix[i][j]<minimum && i != j)
			minimum = costMatrix[i][j];
	return minimum;
}

int s_Min(int costMatrix[n][n], int i) // we calculate the second minimum edge cost
{
	int first = infinity, second = infinity;
	for (int j=0; j<n; j++)
	{
		if (i == j)
			continue;

		if (costMatrix[i][j] <= first)
		{
			second = first;
			first = costMatrix[i][j];
		}
		else if (costMatrix[i][j] <= second && costMatrix[i][j] != first){
			second = costMatrix[i][j];
			}
	}
	return second;
}
// recursive tsp function until leaf node is reached
void recursiveTSP(int costMatrix[n][n], int currentBound, int currentWeight, int level, int currentPath[]) 
{
	if (level==n)
	{
		if (costMatrix[currentPath[level-1]][currentPath[0]] != 0) // checks if there is path from immediate vertex to root vertex
		{
			int currentResult = currentWeight +costMatrix[currentPath[level-1]][currentPath[0]]; // total cost we got so far

			if (currentResult < OptimalSolution) // this updates the path and minimum cost
			{
				Final_path(currentPath);
				OptimalSolution = currentResult;
			}
		}
		return;
	}

	for (int i=0; i<n; i++) // this for loop builds state space tree recursively
	{

		if (costMatrix[currentPath[level-1]][i] != 0 && traversed[i] == false) // here we take next vertex and compute lower bound
		{
			int temp = currentBound;
			currentWeight += costMatrix[currentPath[level-1]][i];

			if (level==1)
			currentBound -= ((f_Min(costMatrix, currentPath[level-1]) + f_Min(costMatrix, i))/2);
			else
			currentBound -= ((s_Min(costMatrix, currentPath[level-1]) +f_Min(costMatrix, i))/2);

			if (currentBound + currentWeight < OptimalSolution) // if condition is true, we need to explore the next nodes
			{
				currentPath[level] = i;
				traversed[i] = true;

				recursiveTSP(costMatrix, currentBound, currentWeight, level+1, currentPath); // recursive call
			}
			
			currentWeight -= costMatrix[currentPath[level-1]][i]; // we prune the node if the condition is false
			currentBound = temp;

			memset(traversed, false, sizeof(traversed));
			for (int j=0; j<=level-1; j++)
				traversed[currentPath[j]] = true;
		}
	}
}

void TravelingSalesman(int costMatrix[n][n]) // tsp function, this function is called for root node
{
	int currentPath[n+1];
	// initialization
	int currentBound = 0;
	memset(currentPath, -1, sizeof(currentPath)); // path initialized
	memset(traversed, 0, sizeof(currentPath)); // nodes traversed till now

	for (int i=0; i<n; i++) // calculating lower bound for root node
		currentBound += (f_Min(costMatrix, i) + s_Min(costMatrix, i));

	currentBound = (currentBound&1)? currentBound/2 + 1 : // rounds off to an integer
								currentBound/2;
								
	traversed[0] = true;
	currentPath[0] = 0;

	recursiveTSP(costMatrix, currentBound, 0, 1, currentPath); // recursive calling for the next level
}

int main(){
	
	int costMatrix[n][n]; // matrix is declared
	ifstream inFile("Input.txt"); // file is opened from where the input is taken
	if(inFile.is_open()){ 
		for(int i=0;i<n; i++){ 
			for (int j=0; j<n; j++){ 
				inFile>>costMatrix[i][j]; // input is taken and stored in array
			}
		}
		inFile.close(); // file is closed
	}
 
	TravelingSalesman(costMatrix); // traveling salesman function is called
	
	ofstream myFile; // we make an object of ofstream
	myFile.open("Output.txt",ios::out); // file for output is opened
	if(myFile.is_open()){
	myFile<<"Minimum cost :"<< OptimalSolution<<endl; //minimum is outputted
	myFile<<"Path taken :"<<endl; 
	for (int i=0; i<=n; i++){
	myFile<<f_path[i]<<"->"; // path taken is outputted
}
}
myFile.close(); // file is closed
	return 0;
}

