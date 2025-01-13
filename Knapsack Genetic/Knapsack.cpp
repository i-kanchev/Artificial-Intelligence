#include <algorithm>
#include <ctime>
#include <iostream>
#include <vector>

const int POPULATION_SIZE = 1000;
const int GENERATIONS = 25;
// percents
const int MUTATION_RATE = 5;

struct Item {
	int weight;
	int value;

	// the value of an item is calculated in value per unit of weight
	bool operator>(const Item& other) {
		return static_cast<double>(this->value) / this->weight > static_cast<double>(other.value) / other.weight;
	}
};

struct Individual {
	// 0 - no item, 1 - has item
	std::vector<int> chromosome;
	int fitness = 0;

	Individual(const int n, const int ratio, const int maxWeight, const std::vector<Item>& items) {
		for (int i = 0; i < n; i++) {
			if (rand() % 100 < ratio) {
				this->chromosome.push_back(1);
			}
			else {
				this->chromosome.push_back(0);
			}
		}

		this->calculateFitness(maxWeight, items);
	}

	void calculateFitness(const int maxWeight, const std::vector<Item>& items) {
		int currWeight = 0;
		int currValue = 0;

		for (int i = 0; i < this->chromosome.size(); i++) {
			if (this->chromosome[i]) {
				currWeight += items[i].weight;
				currValue += items[i].value;
			}
		}

		// if the bag has more items than it can hold, we remove the most useless ones
		int index = this->chromosome.size() - 1;
		while (currWeight > maxWeight) {
			if (this->chromosome[index]) {
				this->chromosome[index] = 0;
				currWeight -= items[index].weight;
				currValue -= items[index].value;
			}
			index--;
		}

		this->fitness = currValue;
	}
};

// a number in the range 1 - 100 showing how often an item may occur
int getRatio(const int n, const int maxWeight, const std::vector<Item>& items) {
	int totalWeight = 0;

	for (int i = 0; i < n; i++) {
		totalWeight += items[i].weight;
	}

	return 100 * maxWeight / totalWeight;
}

int getTotalFitness(const std::vector<Individual>& population) {
	int total = 0;

	for (int i = 0; i < population.size(); i++) {
		total += population[i].fitness;
	}

	return total;
}

int getFittest(const std::vector<Individual>& population) {
	int index = -1;
	int maxFitness = -1;

	for (int i = 0; i < population.size(); i++) {
		if (population[i].fitness > maxFitness) {
			index = i;
			maxFitness = population[i].fitness;
		}
	}

	return index;
}

int getSecondFittest(const std::vector<Individual>& population, const int indexFirst) {
	int index = -1;
	int maxFitness = -1;

	for (int i = 0; i < population.size(); i++) {
		if (i != indexFirst && population[i].fitness > maxFitness) {
			index = i;
			maxFitness = population[i].fitness;
		}
	}

	return index;
}

void elitism(const std::vector<Individual>& population, std::vector<Individual>& offsprings) {
	int indexFittest = getFittest(population);
	int indexSecondFittest = getSecondFittest(population, indexFittest);

	offsprings.push_back(population[indexFittest]);
	offsprings.push_back(population[indexSecondFittest]);
}

Individual rouletteWheelSelection(const std::vector<Individual>& population, const int totalFittness) {
	int rnd = rand() % totalFittness;
	int sum = 0;

	for (int i = 0; i < population.size(); i++) {
		sum += population[i].fitness;

		if (sum > rnd) {
			return population[i];
		}
	}

	return population[0];
}

void crossover(Individual& individual1, Individual& individual2, const int maxWeight, const std::vector<Item>& items) {	
	for (int i = 0; i < individual1.chromosome.size(); i++) {
		if (rand() % 2) {
			std::swap(individual1.chromosome[i], individual2.chromosome[i]);
		}
	}

	individual1.calculateFitness(maxWeight, items);
	individual2.calculateFitness(maxWeight, items);
}

void mutate(Individual& individual, const int maxWeight, const std::vector<Item>& items) {
	for (int i = 0; i < individual.chromosome.size(); i++) {
		if (rand() % 100 < MUTATION_RATE) {
			individual.chromosome[i] = individual.chromosome[i]++ % 2;
		}
	}

	individual.calculateFitness(maxWeight, items);
}

int main()
{
	srand((unsigned int)time(NULL));

	int maxWeight, n;

	std::cin >> maxWeight >> n;

	std::vector<Item> items(n);

	for (int i = 0; i < n; i++) {
		std::cin >> items[i].weight >> items[i].value;
	}

	std::sort(items.begin(), items.end(), std::greater<>());
	
	int ratio = getRatio(n, maxWeight, items);

	std::vector<Individual> population;

	for (int i = 0; i < POPULATION_SIZE; i++) {
		population.push_back(Individual(n, ratio, maxWeight, items));
	}

	for (int i = 0; i < GENERATIONS; i++) {
		std::vector<Individual> offsprings;

		elitism(population, offsprings);

		int totalFittness = getTotalFitness(population);

		while (offsprings.size() < POPULATION_SIZE) {
			Individual parent1 = rouletteWheelSelection(population, totalFittness);
			Individual parent2 = rouletteWheelSelection(population, totalFittness);

			crossover(parent1, parent2, maxWeight, items);

			mutate(parent1, maxWeight, items);
			mutate(parent2, maxWeight, items);

			offsprings.push_back(parent1);
			offsprings.push_back(parent2);
		}

		population = offsprings;
		int fittestIndex = getFittest(population);

		std::cout << population[fittestIndex].fitness << std::endl;
	}

	return 0;
}
