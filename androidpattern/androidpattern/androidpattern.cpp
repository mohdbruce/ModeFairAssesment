#include <iostream>
#include <time.h>
#include <ctime>
#include <stdio.h>
#include <stdlib.h>
#include <fstream>
#include <random>
#include <vector>
#include <algorithm>

using namespace std;



const int POP_SIZE = 40;
const int Node = 9;
const int GENE = Node;


const double CROSSOVRER_PROBABILITY = 0.5;
const double MUTATION_PROBABILITY = 0.8;
const int MAXIMUM_GENERATION = 500;
int chromosome[POP_SIZE][GENE];
int parents[2][GENE];
int children[2][GENE];
int children1[GENE];
int children2[GENE];
int fitnessValue[POP_SIZE];
int survivor[POP_SIZE][GENE];
int newFitnessValue[POP_SIZE + 2];
int childFitnessValue[2];
int parentFitnessValue[2];
int winner[2];
int averagefitness = 0;
int bestfitness = 10000;
int bestchromosome[GENE];
double MSTsample = 127;


void initializePopulation();
//void numPattern();
void printChromosome();
void evaluateChromosome();
void parentSelection();
void crossover();
void mutation();
void survivalSelection();
void copyChromosome();
void printChromosomefinal();




int main()
{
	initializePopulation();
	printChromosome();
	evaluateChromosome();
	//for (int j = 1; j < MAXIMUM_GENERATION; j++)
	//{
		printChromosome();
		evaluateChromosome();
		//for (int i = 1; i < POP_SIZE / 2; i++)
		//{
			parentSelection();
			crossover();
			mutation();
			survivalSelection();
		//}
		copyChromosome();
	//}
	printChromosomefinal();
	

	return 0;
}




void initializePopulation()
{

	// Define the range of integer values
	int minValue = 1;
	int maxValue = Node;

	// Specify the number of sets and the size of each array
	int numSets = POP_SIZE;
	int arraySize = GENE;

	// Create a random number generator
	random_device rd;
	mt19937 mt(rd());

	// Create an array to store each set of arrays
	vector<vector<int>> setsOfArrays;

	// Generate 40 sets of arrays
	for (int i = 0; i < numSets; i++) {
		vector<int> array;
		for (int j = minValue; j <= maxValue; j++) {
			array.push_back(j);
		}

		// Shuffle the array randomly
		
		shuffle(array.begin()+1, array.end()-1, mt);

		setsOfArrays.push_back(array);
	}

	// Print the 40 sets of arrays
	for (int c = 0; c < POP_SIZE; c++) {
		for (int g = 0; g < GENE; g++) {
			chromosome[c][g] = setsOfArrays[c][g];
			if (chromosome[c][g] == 3)
				chromosome[c][g] = 9;
			else if (chromosome[c][g] == 9)
				chromosome[c][g] = 3;
		}


	}
}


void printChromosome()
{

	cout << "INITIALIZE POPULATION" << endl << endl;
	for (int c = 0; c < POP_SIZE; c++) {
		cout << "\tChromosome " << c + 1 << ": ";
		for (int g = 0; g < GENE; g++) {
			cout << chromosome[c][g] << " ";
		}
		cout << endl;
	}

}


void evaluateChromosome()
{

	int mid_elem[10][10] = { 0 };
	mid_elem[1][3] = mid_elem[3][1] = 2;
	mid_elem[1][7] = mid_elem[7][1] = 4;
	mid_elem[3][9] = mid_elem[9][3] = 6;
	mid_elem[7][9] = mid_elem[9][7] = 8;
	mid_elem[1][9] = mid_elem[9][1] = mid_elem[3][7] = mid_elem[7][3] = mid_elem[2][8] = mid_elem[8][2] = mid_elem[4][6] = mid_elem[6][4] = 5;

	int pathmark; int pointVisited[8]; int pointtojump;
	cout << "\nFITNESS EVALUATION" << endl << endl;
	for (int c = 0; c < POP_SIZE; c++) {
		pathmark = 1000; int C = 0; int I = 0;
		for (int g = 1; g < GENE; g++) {

			if (chromosome[c][g] == 3)
				C = g;

			if (chromosome[c][g] == 9)
				I = g;

			
			//visited or not
			pointVisited[g-1] = chromosome[c][g];
			cout << pointVisited[g - 1] << ", ";
			pointtojump = mid_elem[chromosome[c][g-1]][chromosome[c][g]];
			cout << pointtojump << ", ";

			bool visited = true;
			if (pointtojump != 0)
			{
				for (int k = 0; k < g; k++)
				{

					if (pointtojump == pointVisited[k]) 
					{
						visited = true;
						break;
					}
					else
						visited = false;
				}
			}
				if (visited == 0)
					pathmark -= 20;
				else
					pathmark += 20;

				cout << visited << endl;
			

		}

		if (C != 0 && I != 0)
		{
			if (C > I)
				pathmark += 500;
			else
				pathmark -= 500;
		}

		fitnessValue[c] = pathmark;

		if (c + 1 < 10)
			cout << "\tChromosome 0" << c + 1 << ": ";
		else
			cout << "\tChromosome " << c + 1  << ": ";
		cout << " \t Fitness = " << fitnessValue[c] << endl;
	}

}


void parentSelection()
{

	cout << "\nPARENT SELECTION" << endl;
	int indexPlayer1, indexPlayer2;
	int lowestFitness = 1000,secLowestFitness = 1200;
	int winner1, winner2 = 0;

	
	do {
		for (int p = 0; p < 2; p++) {
			do {
				indexPlayer1 = rand() % POP_SIZE; //lowestFitness;//
				do {
					indexPlayer2 = rand() % POP_SIZE; // secLowestFitness;//rand() % POP_SIZE;
				} while (indexPlayer1 == indexPlayer2);
			} while (winner[0] == indexPlayer1);

			
			cout << endl << "\t" << "Player " << indexPlayer1 + 1 << " vs Player " << indexPlayer2 + 1;

			if (fitnessValue[indexPlayer1] >> fitnessValue[indexPlayer2]) {
				winner[p] = indexPlayer1;
				cout << "\n\tWinner is PLAYER " << indexPlayer1 + 1 << endl;
			}
			else {
				winner[p] = indexPlayer2;
				cout << "\n\tWinner is PLAYER " << indexPlayer2 + 1 << endl;
			}
		}// end tournament
	} while (winner[0] == winner[1]);

	for (int r = 0; r < 2; r++) {
		cout << "\n\tParents " << r + 1 << ": ";
		for (int g = 0; g < GENE; g++) {
			parents[r][g] = chromosome[winner[r]][g];
			cout << parents[r][g] << " ";
		}
	}
	cout << endl;



}

void crossover()
{


	float randomValue;
	int start_crossover_point;
	int end_crossover_point;
	int crossoverHere;

	cout << "\n\nCROSSOVER" << endl;
	for (int r = 0; r < 2; r++) { //copy children from parent
		for (int c = 0; c < GENE; c++) {
			children[r][c] = parents[r][c];
		}
	}

	for (int c = 0; c < 2; c++) {
		cout << "\n\tParent " << c + 1 << ": ";
		for (int g = 0; g < GENE; g++) {
			cout << parents[c][g] << " ";
		}
	}
	randomValue = (rand() % 10) / 10.0;
	if (randomValue < CROSSOVRER_PROBABILITY)
	{
		
		 do
			 crossoverHere = rand() % 8 + 2;//2
		 while (crossoverHere == 1 && crossoverHere == 3);
	
		for (int r = 0; r < 2; r++) { 
			
			for (int c = 1; c < GENE-1; c++) {
					
				if (children[r][c] == crossoverHere)
				{
					int tempIndex1 = c;
					int tempvalue = children[r][c];
					for (int d = 0; d < GENE-1; d++) {

						if (r == 0)
						{
							int tempindex2 = 0;
							int tempvalue2 = children[r + 1][tempIndex1];
							children[r + 1][tempIndex1] = children[r][c];
							children[r][c] = tempvalue2;
						}
						else {
							int tempindex2 = 0;
							int tempvalue2 = children[r - 1][tempIndex1];
							children[r - 1][tempIndex1] = children[r][c];
							children[r][c] = tempvalue2;
						}
						

					}


				}

			}
		}

		for (int p = 0; p < 2; p++)
		{
			cout << endl;
			cout << "\tCHILDREN " << p + 1 << ": " << " ";
			for (int g = 0; g < GENE; g++)
			{
				cout << children[p][g] << " ";
			}
		}

	}

	/*if (randomValue < CROSSOVRER_PROBABILITY) {

		for (int i = 0; i < crossoverPoint; i++)
		{

			crossoverHere[i] = rand() % 8 + 2;

		}
		start_crossover_point = rand() % 8+2;
		end_crossover_point = start_crossover_point + 3;

		cout << endl << endl << "\tOrder 1 Crossover happen..." << endl;
		cout << "\tRandomly selecting set from first parent is from index " << start_crossover_point << " to " << end_crossover_point << endl;

		for (int g = start_crossover_point; g < end_crossover_point + 1; g++)
		{
			children[0][g] = parents[1][g];
			children[1][g] = parents[0][g];
		}

		//adjustment to change repeated number

		int mid_elem[10][10] = { 0 };
		mid_elem[1][3] = mid_elem[3][1] = 2;
		mid_elem[1][7] = mid_elem[7][1] = 4;
		mid_elem[3][9] = mid_elem[9][3] = 6;
		mid_elem[7][9] = mid_elem[9][7] = 8;
		mid_elem[1][9] = mid_elem[9][1] = mid_elem[3][7] = mid_elem[7][3] = mid_elem[2][8] = mid_elem[8][2] = mid_elem[4][6] = mid_elem[6][4] = 5;

		int pathmark; int pointVisited[8]; int pointtojump;

		for (int c = 0; c < 2; c++)
		{
			for (int g = 1; g < GENE; g++)
			{



				pointVisited[g - 1] = children[c][g];

				bool visited = true;
				if (pointtojump != 0)
				{
					for (int k = 0; k < g; k++)
					{

						if (pointtojump == pointVisited[k])
						{
							visited = true;
							break;
						}
						else
							visited = false;
					}
				}

				


			}


		}


		for (int p = 0; p < 2; p++)
		{
			cout << endl;
			cout << "\tCHILDREN " << p + 1 << ": " << " ";
			for (int g = 0; g < GENE; g++)
			{
				cout << children[p][g] << " ";
			}
		}


	}*/
	cout << endl;



}
void mutation()
{



	float randomValue;
	int mutation_bit, mutation_bit2;
	int tempValue;
	cout << "\n\nMutation" << endl << endl;
	for (int c = 0; c < 2; c++) {
		randomValue = (rand() % 11) / 10.0;
		if (randomValue < MUTATION_PROBABILITY) {
			do
				mutation_bit = rand() % GENE;
			while (mutation_bit == 0);
			tempValue = children[c][mutation_bit];
			do 
				mutation_bit2 = rand() % GENE;
			while (mutation_bit2 == 0);
			children[c][mutation_bit] = children[c][mutation_bit2];
			children[c][mutation_bit2] = tempValue;
			if (c == 0) {
				cout << "\tChildren " << c + 1 << ": Swap position between index " << mutation_bit << " & " << mutation_bit2 << endl;
			}
			else if (c == 1) {
				cout << "\tChildren " << c + 1 << ": Swap position between index " << mutation_bit << " & " << mutation_bit2 << endl;
			}



		}
	}

	for (int c = 0; c < 2; c++) {
		for (int g = 0; g < GENE; g++) {

			if (c == 0)
				children1[g] = children[c][g];
			else
				children2[g] = children[c][g];
		}
	}


	for (int c = 0; c < 2; c++) {
		for (int g = 0; g < GENE; g++) {


			if (c == 0)
			{
				// Check for duplicate values
				vector<int> array(children1, children1 + sizeof(children1) / sizeof(children1[0]));
				std::sort(array.begin(), array.end()); // Sort the array
				bool hasDuplicates = false;
				for (int i = 1; i < array.size(); i++) {
					if (array[i] == array[i - 1]) {
						hasDuplicates = true;
						break;
					}
				}

				// If there are duplicates, replace them with values not present in the array
				if (hasDuplicates) {
					std::vector<int> uniqueValues;
					for (int i = 1; i <= 11; i++) {
						if (std::find(array.begin(), array.end(), i) == array.end()) {
							uniqueValues.push_back(i);
						}
					}

					for (int i = 0; i < array.size(); i++) {
						for (int j = i + 1; j < array.size(); j++) {
							if (array[i] == array[j]) {
								array[j] = uniqueValues.back();
								uniqueValues.pop_back();
							}
						}
					}
				}
				for (int g = 0; g < GENE; g++) {
					chromosome[0][g] = array[g];
				}
			}
			else {

				// Check for duplicate values
				vector<int> array(children2, children2 + sizeof(children2) / sizeof(children2[0]));
				std::sort(array.begin(), array.end()); // Sort the array
				bool hasDuplicates = false;
				for (int i = 1; i < array.size(); i++) {
					if (array[i] == array[i - 1]) {
						hasDuplicates = true;
						break;
					}
				}

				// If there are duplicates, replace them with values not present in the array
				if (hasDuplicates) {
					std::vector<int> uniqueValues;
					for (int i = 1; i <= 11; i++) {
						if (std::find(array.begin(), array.end(), i) == array.end()) {
							uniqueValues.push_back(i);
						}
					}

					for (int i = 0; i < array.size(); i++) {
						for (int j = i + 1; j < array.size(); j++) {
							if (array[i] == array[j]) {
								array[j] = uniqueValues.back();
								uniqueValues.pop_back();
							}
						}
					}
				}


				for (int g = 0; g < GENE; g++) {
					chromosome[1][g] = array[g];
				}

			}


		}


	}



	for (int c = 0; c < 2; c++) {
		cout << "\n\tAfter mutation > Children " << c + 1 << ": ";
		for (int g = 0; g < GENE; g++) {
			cout << children[c][g] << " ";
		}
	}
	cout << endl;


}

void survivalSelection()
{
	int mid_elem[10][10] = { 0 };
	mid_elem[1][3] = mid_elem[3][1] = 2;
	mid_elem[1][7] = mid_elem[7][1] = 4;
	mid_elem[3][9] = mid_elem[9][3] = 6;
	mid_elem[7][9] = mid_elem[9][7] = 8;
	mid_elem[1][9] = mid_elem[9][1] = mid_elem[3][7] = mid_elem[7][3] = mid_elem[2][8] = mid_elem[8][2] = mid_elem[4][6] = mid_elem[6][4] = 5;

	int pathmark; int pointVisited[8]; int pointtojump;


	int totaldistance;
	int node1 = 0, node2 = 0;
	int firstwinner = 10;
	int seconwinner = 10;
	int firstMinIdx, secondMinIdx;
	cout << "\n\nSURVIVAL SELECTION" << endl;
	cout << "\nFITNESS EVALUATION FOR PARENT" << endl << endl;
	////////////////////////////////////////////////////
	for (int c = 0; c < 2; c++) {
		pathmark = 1000; int C = 0; int I = 0;
		for (int g = 1; g < GENE; g++) {

			if (parents[c][g] == 3)
				C = g;

			if (parents[c][g] == 9)
				I = g;


			//visited or not
			pointVisited[g - 1] = parents[c][g];
			cout << pointVisited[g - 1] << ", ";
			pointtojump = mid_elem[parents[c][g - 1]][parents[c][g]];
			cout << pointtojump << ", ";

			bool visited = true;
			if (pointtojump != 0)
			{
				for (int k = 0; k < g; k++)
				{

					if (pointtojump == pointVisited[k])
					{
						visited = true;
						break;
					}
					else
						visited = false;
				}
			}
			if (visited == 0)
				pathmark -= 20;
			else
				pathmark += 20;

			cout << visited << endl;


		}

		if (C != 0 && I != 0)
		{
			if (C > I)
				pathmark += 500;
			else
				pathmark -= 500;
		}

		newFitnessValue[c] = pathmark;

		cout << "\tParent " << c + 1 << ": ";
		cout << " Total length =" << newFitnessValue[c] << endl;
	}

	///////////////////////////////////////////////////






	cout << "\nFITNESS EVALUATION FOR CHILDREN" << endl << endl;
	//////////////////////////////////////////////////////
	for (int c = 0; c < 2; c++) {
		pathmark = 1000; int C = 0; int I = 0;
		for (int g = 1; g < GENE; g++) {

			if (children[c][g] == 3)
				C = g;

			if (children[c][g] == 9)
				I = g;


			//visited or not
			pointVisited[g - 1] = children[c][g];
			cout << pointVisited[g - 1] << ", ";
			pointtojump = mid_elem[children[c][g - 1]][children[c][g]];
			cout << pointtojump << ", ";

			bool visited = true;
			if (pointtojump != 0)
			{
				for (int k = 0; k < g; k++)
				{

					if (pointtojump == pointVisited[k])
					{
						visited = true;
						break;
					}
					else
						visited = false;
				}
			}
			if (visited == 0)
				pathmark -= 20;
			else
				pathmark += 20;

			cout << visited << endl;


		}

		if (C != 0 && I != 0)
		{
			if (C > I)
				pathmark += 500;
			else
				pathmark -= 500;
		}

		newFitnessValue[c+2] = pathmark;

		cout << "\tChildren " << c + 1 << ": ";
		cout << " Total length =" << newFitnessValue[c + 2] << endl;
	}
	/////////////////////////////////////////////////////

	cout << "\nSURVIVOR SELECTION IS BASED ON THE FITNESS VALUE" << endl;

	for (int i = 0; i < 4; i++) {
		if (newFitnessValue[i] > firstwinner) {
			firstwinner = newFitnessValue[i];
			firstMinIdx = i;

		}
	}
	cout << "\tThe first winner is " << firstwinner << endl;
	for (int i = 0; i < 4; i++) {
		if (newFitnessValue[i] > seconwinner && firstwinner != newFitnessValue[i]) {
			seconwinner = newFitnessValue[i];
			secondMinIdx = i;
		}
	}
	cout << "\tThe second winner is " << seconwinner << endl;


	int childrenIdx = 0, parentIdx = 0;
	switch (firstMinIdx) {
	case 0: parentIdx = 0; break;
	case 1:	parentIdx = 1; break;
	case 2: childrenIdx = 0; break;
	case 3: childrenIdx = 1; break;
	default:
		break;
	}

	childrenIdx = 0, parentIdx = 0, secondMinIdx = 0;
	switch (secondMinIdx) {
	case 0: parentIdx = 0; break;
	case 1:	parentIdx = 1; break;
	case 2: childrenIdx = 0; break;
	case 3: childrenIdx = 1; break;
	default:
		break;
	}


	for (int r = 0; r < 2; r++) { //copy survivor from children/parent
		for (int c = 0; c < GENE; c++) {
			if (r == 0) {
				survivor[r][c] = parents[parentIdx][c];
			}
			else if (r == 1) {
				survivor[r][c] = children[childrenIdx][c];
			}
		}
	}

	for (int c = 0; c < 2; c++) {
		cout << "\n\tSurvivor " << c + 1 << ": ";
		for (int g = 0; g < GENE; g++) {
			cout << survivor[c][g] << " ";
		}
	}
	cout << endl;


}

void copyChromosome()
{
	cout << endl << endl << "~ NEW POPULATION ~";

	for (int g = 0; g < GENE; g++)
	{
		chromosome[winner[0]][g] = survivor[0][g];
		chromosome[winner[1]][g] = survivor[1][g];
	}

	for (int p = 0; p < POP_SIZE; p++)
	{
		cout << endl;
		cout << "\tCHROMOSOME " << p + 1 << ": " << " ";
		for (int g = 0; g < GENE; g++)
		{
			cout << chromosome[p][g] << " ";
		}
	}
	cout << endl;
}

void printChromosomefinal()
{
	system("cls");
	//cout << "INITIALIZE POPULATION" << endl << endl;
	cout << "\tPattern " << ": ";
	for (int c = 0; c < POP_SIZE; c++) {
		if (fitnessValue[c] > 1500)
		{
			for (int g = 0; g < GENE; g++) {

				switch (chromosome[c][g]) {
				case 1: cout << 'A';
					break;
				case 2: cout << 'B';
					break;
				case 3: cout << 'C';
					break;
				case 4: cout << 'D';
					break;
				case 5: cout << 'E';
					break;
				case 6: cout << 'F';
					break;
				case 7: cout << 'G';
					break;
				case 8: cout << 'H';
					break;
				case 9: cout << 'I';
					break;
				}
			}
			cout << ", ";
		}
	}

}
