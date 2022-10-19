#pragma once
#include "Individual.h"
#include <iostream>
#include<stdlib.h>
using namespace std;

// ============================= Globals ================================== //
const int POPULATION_SIZE = 10;

class Population
{
public:
	// Constructor // 
	Population(int n) : population_size(n) {
		generatePopulation();
		generateFitnessRatio();
	}

	/* Non modifying operations */
	int getPopulationSize() const { return population_size; }
	Bag getBag(int i) const { return population[i]; }
	double getSumFitness()const { return sum_of_fitness; }
	Bag getMom() const { return mom; }
	Bag getDad() const { return dad; }

	/* Modifying Operations */
	void setPopulationSize(int n) { population_size = n; }
	void addPopulation(Bag p) { population.push_back(p); }
	void setSumFitness(double s) { sum_of_fitness = s; }
	void setMom(Bag m) { mom = m; }
	void setDad(Bag d) { dad = d; }

	void generatePopulation() {
		for (int i = 0; i != population_size; i++) {
			Bag b;
			population.push_back(b);
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
			population[i].setFitnessratio(fr);
		}
	}

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

// =============== Helper functions ====================== // 

// Print total population
void printPopulation(Population& population)
{
	for (int i = 0; i != population.getPopulationSize(); i++) {
		Bag b(population.getBag(i));
		cout << b << '\n';
	}
}
int randDouble(double min, double max)// generates random number
{
	static std::default_random_engine ran;
	return std::uniform_real_distribution<>{min, max}(ran);
}
// =================================== selection =============================================//

// ========================== roulette wheel ========================================//
void roulette_wheel(Population& population) // Selection made on popbablility based on fitness ratio
{
	
	int selection_mom = randDouble(0, population.getSumFitness());
	int selection_dad = randDouble(0, population.getSumFitness());
	cout << "rand mom:" << selection_mom << " rand dad:" << selection_dad << endl;

	bool momPicked = false, dadPicked = false;

	population.bubblesort(); // Sort the population based on fitness ratio
	printPopulation(population);

	int sumMom = 0;
	int sumDad = 0;
	for (int i = 0; i != population.getPopulationSize(); i++) // Loop through population
	{ 
		if (!momPicked)
		{
			if (sumMom > selection_mom)
			{
				Bag tempMom = population.getBag(i);

				//cout << "TempMom:" << tempMom << endl;
				population.setMom(tempMom);
				momPicked = true;
			}
			else {
				sumMom += population.getBag(i).getFitness();
			}
		}
		if (!dadPicked)
		{
			if (sumDad > selection_dad)
			{
				Bag tempDad = population.getBag(i);

				//cout << "TempDad:" << tempDad << endl;
				population.setDad(tempDad);
				dadPicked = true;
			}
			else {
				sumDad += population.getBag(i).getFitness();
			}
		}
	}
}

Population reGenerate(Population& population)
{
	roulette_wheel(population);
	Bag m = population.getMom();
	Bag d = population.getDad();
	cout << m << '\n';
	cout << d << '\n';
	return population;
}

void popTest()
{
	Population population(POPULATION_SIZE);
	printPopulation(population);

	Population newPopulation = reGenerate(population);
}

