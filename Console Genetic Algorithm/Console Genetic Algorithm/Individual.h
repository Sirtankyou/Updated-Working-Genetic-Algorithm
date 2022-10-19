#pragma once
#include <string>
#include <random>
#include <iostream>
#include <list>

using namespace std;

// -------------------- Global Variables ------------------------------ //
const int CHROMOSOME_SIZE = 8;
const int TERMINATION = 255;

// --------------------- Declearations --------------------------//
int randInt(int min, int max);
string createChromosome();
int decodeInt(string chromosome);
int listItems(int choice, string chromosome);
void testPerson();

// ------------------------- Objects --------------------------- //
struct Items
{
	int id;
	string name;
	int value;
	int weight;
	Items(int itemId, string ItemName, int itemValue, int itemWeight) : id(itemId), name(ItemName), value(itemValue), weight(itemWeight)
	{
	}
};

class Bag //Chromosome, Use chromosome to find all item weights, Fitness = Total item value 
{
public:
	Bag() 
	{
		chromosome = createChromosome();
		decodedInt = decodeInt(chromosome);
		fitness = listItems(1, chromosome);
		weight = listItems(2, chromosome);
		fitnessRatio = 0.0;
	}

	Bag(string chromo)
	{
		chromosome = chromo;
		decodedInt = decodeInt(chromosome);
		fitness = listItems(1, chromosome);
		weight = listItems(2, chromosome);
		fitnessRatio = 0.0;
	}

	Bag(string chromo, int decode, int fit, int W, double fr) : chromosome(chromo), decodedInt(decode), fitness(fit), weight(W), fitnessRatio(fr) {
		
	}

	/* Non modifying operations */
	string getChromosome() const { return chromosome; }
	int getDecodedInt() const { return decodedInt; }
	int getFitness() const { return fitness; }
	int getWeight() const { return weight; }
	int getFitnessratio() const { return fitnessRatio; }

	/* Modifying Operations */
	void setChromosome(string chromo) { chromosome = chromo; }
	void setDecodedInt(int decode) { decodedInt = decode; }
	void setFitness(int fit) { fitness = fit; }
	void setWeight(int W) { weight = W; }
	void setFitnessratio(double fr) { fitnessRatio = fr; }


private:
	string chromosome;
	int decodedInt;
	int fitness;
	int weight;
	double fitnessRatio;
};

// ------------------ Helper functions --------------------- // 

int randInt(int min, int max)// generates random number
{
	static std::default_random_engine ran;
	return std::uniform_int_distribution<>{min, max}(ran);
}

string createChromosome()
{
	
	string gnome = "";
	for (int i = 0; i < CHROMOSOME_SIZE; i++)
	{
		gnome += to_string(randInt(0, 1));
	}
	return gnome;
}

int decodeInt(string chromosome)
{
	string num = chromosome;
	int dec_value = 0;

	int base = 1;
	int len = num.length();
	for (int i = len - 1; i >= 0; i--)
	{
		if (num[i] == '1')
		{
			dec_value += base;
		}
		base = base * 2;
	}
	return dec_value;
}

int listItems(int choice, string chromosome)
{
	list<Items> listofItems =
	{
		Items(1, "Tent", 500, 2000), Items(2, "Sleeping bags", 450, 635), Items(3, "Headlamp", 300, 51),
		Items(4, "Camping Chair", 100, 907), Items(5, "Stove", 350, 1474), Items(6, "Frying pan", 350, 485),
		Items(7, "Binoculars", 150, 850), Items(8, "Multi-Tool", 250, 241)
	};

	list<Items>::iterator it;

	int fitness = 0;
	int Totalweight = 0;
	int i = 0;
	for (it = listofItems.begin(); it != listofItems.end(); it++)
	{
		
		int id = it->id;
		string name = it->name;
		int value = it->value;
		int weight = it->weight;

		if (chromosome[i] == '1')
		{
			fitness += value;
			Totalweight += weight;
		}
		i++;
	}
	if (choice == 1)
	{
		return fitness;
	}
	else if (choice == 2)
	{
		return Totalweight;
	}
}

// ------------------- Operators ------------------------- //
ostream& operator<<(ostream& os, Bag& bag)
{
	const char colon = ':';
	return os << bag.getChromosome() << colon << bag.getDecodedInt()
		<< colon << bag.getFitness() << colon << bag.getFitnessratio() << endl;
}


void testPerson()
{
	Bag p;

	cout << p;
}

