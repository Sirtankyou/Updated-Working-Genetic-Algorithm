#pragma once
#include "Individual.h"
#include <iostream>

// --------------------------------- Globals ------------------------------------ //
const int POPULATION_SIZE = 6;
const double CROSSOVER_RATE = 0.07;
const double MUTATION_RATE = 0.001;

// -------------------------------- declaration -------------------------------- //


/* -------------------------------- randdouble =========================================//
	*   "random number engine  is a function that generates uniformly distributed
	*	sequence of integer values."
	*	"Distribution is a function that generates a sequence of values according
	*	to a mathematical formaula given a sequence of value from an engine."
	*	Stroustrup (2014: p914)
	*///===========================================================================//

double randDouble(double min, double max)// generates random number
{
	static std::default_random_engine ran;
	return std::uniform_real_distribution<>{min, max}(ran);
}

class Population
{
public:
	
	// initial population
	Population() : population_size(POPULATION_SIZE) {
		generatePopulation(population_size);
		generateFitnessRatio();
	}
	// repopulate with two carry over
	Population(Bag& p1, Bag& p2) : population_size(POPULATION_SIZE) {
		population.push_back(p1);
		population.push_back(p2);
		generatePopulation(population_size - 2); // rePopulate rest of population
		generateFitnessRatio();
	}


	// == non- modifying operations == //
	int getPopulationSize() const { return population_size; }
	Bag getBag(int i) const { return population[i]; }
	double getSumFitness()const { return sum_of_fitness; }
	Bag getMom() const { return mom; }
	Bag getDad() const { return dad; }

	// == modifying operations == //
	void setPopulationSize(int n) { population_size = n; }
	void addPopulation(Bag p) { population.push_back(p); }
	void setSumFitness(double s) { sum_of_fitness = s; }
	void setMum(Bag m) { mom = m; }
	void setDad(Bag d) { dad = d; }

	//Generate a certain number of individuals
	void generatePopulation(int n) { 
		for (int i = 0; i != n; i++) {
			Bag p;
			population.push_back(p);
		}
	}

	void generateFitnessRatio()
	{
		int sum = 0;
		for (int i = 0; i != population_size; i++) { // get sum of all populations fitness
			sum += population[i].getFitness();
		}

		sum_of_fitness = sum;

		for (int i = 0; i != population_size; i++) { // calculates and sets each fitness ratio from sum of fitness
			double fr = population[i].getFitness() / sum_of_fitness * 100;
			population[i].getFitnessratio();
		}
	}

	// ------------------- bubblesort --------------------- //
	void bubblesort()
	{
		for (int i = 0; i != population_size - 1; i++) {
			for (int j = 0; j != population_size - i - 1; j++) {
				if (population[j].getFitnessratio() < population[j + 1].getFitnessratio()) {
					Bag temp = population[j];
					population[j] = population[j + 1];
					population[j + 1] = temp;
				}
			}
		}
	}

private:
	vector<Bag> population;
	int population_size;
	double sum_of_fitness;
	Bag mom, dad;
};

// ---------------------------------- Helper functions ------------------------------------- //
// Print the entire population
void printPopulation(Population& population) 
{
	cout << "-----------------------" << endl;
	for (int i = 0; i != population.getPopulationSize(); i++) {
		Bag p(population.getBag(i));
		cout << p << '\n';
	}
	cout << "-----------------------" << endl;
}

// ---------------------------- Selection Methods -------------------------------------------//

//  Roulette wheel selection
void roulette_wheel(Population& population)
{
	// Selecting the individual for mom and dad
	int selection_mum = randDouble(1, 100);
	int selection_dad = randDouble(1, 100);

	// Sorting the population based on fitness
	//cout << "selection: " << selection_mum << '\t' << selection_dad << endl;
	bool mumPicked = false, dadPicked = false;
	population.bubblesort();
	//printPopulation(population);

	// Cycling through the population until the sum of fitness if bigger than the number selected
	double sumOfFitness = 0;
	for (int i = population.getPopulationSize() - 1; i >= 0; i--) {

		sumOfFitness += population.getBag(i).getFitnessratio();
		if (!mumPicked)
			if (selection_mum < sumOfFitness)
			{
				Bag tempMum = population.getBag(i); // Setting the chosen individual as mom
				//cout << "sel mum:" << tempMum << endl;
				population.setMum(tempMum);
				mumPicked = true;
			}
		if (!dadPicked)
			if (selection_dad < sumOfFitness) // Setting the chosen individual as dad
			{
				Bag tempDad = population.getBag(i);
				//cout << "sel dad:" << tempDad << endl;
				population.setDad(tempDad);
				dadPicked = true;
			}
	}
}

// Ranking Selection 

// Trucation selection


// Adding a mutation to the selected chromosomes
Population mutation(Population& population, string childChromo1, string childChromo2)
{
	double pm = randDouble(0, 1);
	int randMutMom = randInt(0, CHROMOSOME_SIZE - 1);
	int randMutDad = randInt(0, CHROMOSOME_SIZE - 1);
	//cout << "pm: " << pm << endl;
	if (pm > MUTATION_RATE) {
		if (childChromo1[randMutMom] == '1')childChromo1[randMutMom] = '0';
		else childChromo1[randMutMom] = '1';
		if (childChromo2[randMutDad] == '1')childChromo1[randMutDad] = '0';
		else childChromo2[randMutDad] = '1';
	}

	//cout << "Mutation1: " << childChromo1 << endl;
	//cout << "Mutation2: " << childChromo2 << endl;

	Bag offSpring1(childChromo1);
	Bag offSpring2(childChromo2);
	Population rePopulation(offSpring1, offSpring2);
	return rePopulation;
}

// Crossover breeding method
void crossover(Population& population)
{
	//cout << "Mum: " << population.getMum().getChromo() << endl;
	//cout << "Dad: " << population.getDad().getChromo() << endl;

	// Creating a random number to determine if crossover will take place
	double Pc = randDouble(0, 1);

	// Selecting what section of the chromosome will cross over
	int randSplit = randInt(0, CHROMOSOME_SIZE - 1);
	//cout << "PC: " << Pc << '\t' << "split" << randSplit << endl;

	string childChromo1, childChromo2;

	// Crossing the chromosomes with each other between mom and dad
	if (Pc > CROSSOVER_RATE) {

		for (int i = 0; i != randSplit; i++) {
			childChromo1 += population.getMom().getChromosome()[i];
			childChromo2 += population.getDad().getChromosome()[i];
		}
		for (int i = randSplit; i != CHROMOSOME_SIZE; i++) {
			childChromo1 += population.getDad().getChromosome()[i];
			childChromo2 += population.getMom().getChromosome()[i];
		}
	}
	// Else create a clone if crossover does not happen
	else {
		childChromo1 = population.getMom().getChromosome();
		childChromo2 = population.getDad().getChromosome();
	}
	//cout << "Child1: " << childChromo1 << endl;
	//cout << "Child2: " << childChromo2 << endl;

	// Passing new chromosomes through mutation
	population = mutation(population, childChromo1, childChromo2);
}

// To check if the sequence is complete 
bool is_termination(Population& population)
{
	// loop through the population
	for (int i = 0; i != POPULATION_SIZE; i++) {
		// Check decoded integers against the Termination integer
		if (population.getBag(i).getDecodedInt() == TERMINATION)
		{
			return true;
		}
	}
	return false;
}

void reGenerate(Population& population)
{
	roulette_wheel(population);
	Bag m = population.getMom();
	Bag d = population.getDad();
	crossover(population);
}

void popTest()
{
	Population population;
	int generations = 0;
	while (!is_termination(population)) {
		generations++;
		reGenerate(population);

	}
	printPopulation(population);
	cout << generations;
}