#include <iostream>
#include <sstream>
#include <string>
#include <vector>
#include <numeric>
#include "functions.h"

void test0();
void test1();
void test2();
void test3();
void test4();
void test5();
void test6();
void test7();
void test8();
void test9();
void test10();

int main(int argc, char* argv[])
{
    void (*f[])() = { test1, test2, test3, test4, test5, test6, test7, test8, test9, test10 };
    const int SIZE = sizeof(f) / sizeof(f[0]);
    int id = -1;

    if (argc == 2)
    {
        if (argv[1][0] == 'i')
        {
            std::cout << "Enter the test number or 0 to run all tests:" << std::endl;
            std::cin >> id;
        }
        else
            id = atoi(argv[1]);
    }
    else
        std::cin >> id;

    test0();

    if (id == 0)
        for (int i = 0; i < SIZE; ++i)
            f[i]();
    else if (0 < id && id <= SIZE)
        f[id - 1]();
    else
        std::cout << "Test " << id << " not found." << std::endl;

    return 0;
}


// Non evaluated selftest
void test0()
{
    
}

void test1()
{
    using Gene = AI::Gene<int, AI::Seed>;

    int actual1 = Fitness_Accumulate<Gene>()({ 1, 2, 3, 4, 5, 6, 7, 8 });
    int expected1 = 1 + 2 + 3 + 4 + 5 + 6 + 7 + 8;

    int actual2 = Fitness_Nbits<Gene>()({ 0, 1, 0, 1, 0, 1, 0, 1 });
    int expected2 = 50;

    int actual3 = Fitness_8queens<Gene>()({ 1, 2, 3, 4, 5, 6, 7, 8 });
    int expected3 = 0;

    int actual4 = Fitness_8queens<Gene>()({ 4, 2, 0, 6, 1, 7, 5, 3 });
    int expected4 = 100;

    std::cout << "Test 1 : ";
    if (actual1 == expected1 && 
        actual2 == expected2 &&
        actual3 == expected3 &&
        actual4 == expected4)
        std::cout << "Pass" << std::endl;
    else
        std::cout << "Failed (" << std::endl << actual1 << ',' 
                                             << actual2 << ','
                                             << actual3 << ','
                                             << actual4 << ')' << std::endl;
}

void test2()
{
    const size_t sizeOfChromosome = 8;
    using Gene = AI::Gene<int>;
    using Chromosome = AI::Chromosome<Gene, Fitness_Accumulate<Gene>, sizeOfChromosome>;

    Chromosome chromosome;

    chromosome.setGenes({ 1, 2, 3, 4, 5, 6, 7, 8 });

    int actual = chromosome.getFitness();

    int expected = 1 + 2 + 3 + 4 + 5 + 6 + 7 + 8;

    std::cout << "Test 2 : ";
    if (actual == expected)
        std::cout << "Pass" << std::endl;
    else
        std::cout << "Failed (" << std::endl << actual << ')' << std::endl;
}

void test3()
{
    const size_t sizeOfChromosome = 8;
    const int seedValue = 3;
    using Gene = AI::Gene<int, AI::Seed_Value<seedValue>>;
    using Chromosome = AI::Chromosome<Gene, Fitness_Accumulate<Gene>, sizeOfChromosome>;

    AI::Individual<Chromosome> individual;

    int actual = individual.getFitness();

    int expected = seedValue * sizeOfChromosome;

    std::cout << "Test 3 : ";
    if (actual == expected)
        std::cout << "Pass" << std::endl;
    else
        std::cout << "Failed (" << std::endl << actual << ')' << std::endl;
}

// Use random seed function
void test4()
{
    const size_t sizeOfChromosome = 8;
    const int Max = 10;
    using Gene = AI::Gene<int, AI::Seed_Random<Max>>;
    using Chromosome = AI::Chromosome<Gene, Fitness_Accumulate<Gene>, sizeOfChromosome>;

    AI::Individual<Chromosome> individual;

    Chromosome& c = individual.getChromosome();

    bool actual = (
        c.getGene(0).getValue() >= 0 && c.getGene(0).getValue() < Max &&
        c.getGene(1).getValue() >= 0 && c.getGene(1).getValue() < Max &&
        c.getGene(2).getValue() >= 0 && c.getGene(2).getValue() < Max &&
        c.getGene(3).getValue() >= 0 && c.getGene(3).getValue() < Max &&
        c.getGene(4).getValue() >= 0 && c.getGene(4).getValue() < Max &&
        c.getGene(5).getValue() >= 0 && c.getGene(5).getValue() < Max &&
        c.getGene(6).getValue() >= 0 && c.getGene(6).getValue() < Max &&
        c.getGene(7).getValue() >= 0 && c.getGene(7).getValue() < Max &&
        (   c.getGene(0).getValue() != c.getGene(1).getValue() ||
            c.getGene(1).getValue() != c.getGene(2).getValue() ||
            c.getGene(2).getValue() != c.getGene(3).getValue() ||
            c.getGene(3).getValue() != c.getGene(4).getValue() ||
            c.getGene(4).getValue() != c.getGene(5).getValue() ||
            c.getGene(5).getValue() != c.getGene(6).getValue() ||
            c.getGene(6).getValue() != c.getGene(7).getValue())
        );

    bool expected = true;

     std::cout << "Test 4 : ";
     if (actual == expected)
         std::cout << "Pass" << std::endl;
     else
         std::cout << "Failed (" << std::endl << actual << ')' << std::endl;
}

void test5()
{
    const size_t sizeOfChromosome = 3;
    const size_t sizeOfPopulation = 5;
    using Gene = AI::Gene<int>;
    using Chromosome = AI::Chromosome<Gene, Fitness_Accumulate<Gene>, sizeOfChromosome>;

    AI::Population<AI::Individual<Chromosome>> population{ sizeOfPopulation };

    population.getIndividual(0).setGenes({ 1,2,3 });
    population.getIndividual(1).setGenes({ 10,20,30 });
    population.getIndividual(2).setGenes({ 10000,20000,30000 });
    population.getIndividual(3).setGenes({ 100,200,300 });
    population.getIndividual(4).setGenes({ 1000,2000,3000 });
    population.updateFittest();

    AI::Individual<Chromosome>* fittest = population.getFittest();

    int actual = fittest ? fittest->getFitness() : -1;

    int expected = 10000 + 20000 + 30000;

    std::cout << "Test 5 : ";
    if (actual == expected)
        std::cout << "Pass" << std::endl;
    else
        std::cout << "Failed (" << std::endl << actual << ')' << std::endl;
}

// Tests Selection()
void test6()
{
    const size_t sizeOfChromosome = 8;
    const size_t Max = 2;
    using Gene = AI::Gene<int, AI::Seed_Random<Max>>;
    using Chromosome = AI::Chromosome<Gene, Fitness_Nbits<Gene>, sizeOfChromosome>;

    const size_t sizeOfPopulation = 10;

    AI::GeneticAlgorithm<AI::Individual<Chromosome>> ga;
    AI::Population<AI::Individual<Chromosome>>* newGeneration = ga.selection(sizeOfPopulation);

    bool actual = true;

    if (!newGeneration)
        actual = false;
    else
        for (size_t i = 0; i < newGeneration->getSize(); ++i)
        {
            AI::Individual<Chromosome>& individual = newGeneration->getIndividual(i);
            if (Fitness_Nbits<Gene>()(individual.getGenes()) !=
                individual.getFitness())
            {   
                actual = false;
                break;
            }
        }

    bool expected = true;

    delete newGeneration;

    std::cout << "Test 6 : ";
    if (actual == expected)
        std::cout << "Pass" << std::endl;
    else
        std::cout << "Failed (" << std::endl << actual << ')' << std::endl;
}

// Test Crossover()
void test7()
{
    const size_t sizeOfChromosome = 8;
    using Gene = AI::Gene<int, AI::Seed>;
    using Chromosome = AI::Chromosome<Gene, Fitness_Nbits<Gene>, sizeOfChromosome>;

    const size_t sizeOfPopulation = 2;
    const AI::CrossoverMethod crossoverMethod = AI::CrossoverMethod::Middle;

    AI::GeneticAlgorithm<AI::Individual<Chromosome>> ga;
    AI::Population<AI::Individual<Chromosome>> newGeneration(sizeOfPopulation);
    newGeneration.getIndividual(0).setGenes({ 0,0,0,0,0,0,0,0 });
    newGeneration.getIndividual(1).setGenes({ 1,1,1,1,1,1,1,1 });
    ga.crossover(&newGeneration, crossoverMethod); 

    std::ostringstream os;
    os << newGeneration.getIndividual(0);

    std::string actual = os.str();

    std::string expected = "[1,1,1,1,0,0,0,0]=50";

    std::cout << "Test 7 : ";
    if (actual == expected)
        std::cout << "Pass" << std::endl;
    else
        std::cout << "Failed (" << std::endl << actual << ')' << std::endl;
}

void test8()
{
    const size_t sizeOfChromosome = 8;
    const size_t Max = 2;
    using Gene = AI::Gene<int, AI::Seed_Random<Max>>;
    using Chromosome = AI::Chromosome<Gene, Fitness_Nbits<Gene>, sizeOfChromosome>;

    const size_t sizeOfPopulation = 10;
    const int mutationProbability = 70;
    const AI::CrossoverMethod crossoverMethod = AI::CrossoverMethod::Middle;

    AI::GeneticAlgorithm<AI::Individual<Chromosome>> ga;
    ga.run(sizeOfPopulation, mutationProbability, crossoverMethod);

    AI::Individual<Chromosome>* fittest = ga.getFittest();

    std::ostringstream os;
    if (fittest)
        os << *fittest;

    std::string actual = os.str();

    std::string expected = "[1,1,1,1,1,1,1,1]=100";

    std::cout << "Test 8 : ";
    if (actual == expected)
        std::cout << "Pass" << std::endl;
    else
        std::cout << "Failed (" << std::endl << actual << ')' << std::endl;
}

void test9()
{
    const size_t sizeOfChromosome = 8;
    const size_t Max = 8;
    using Seed = AI::Seed_Random<Max>;
    using Gene = AI::Gene<int, Seed>;
    using Chromosome = AI::Chromosome<Gene, Fitness_8queens<Gene>, sizeOfChromosome>;

    std::ostringstream os;

    const size_t sizeOfPopulation = 100;
    const int mutationProbability = 70;
    const AI::CrossoverMethod crossoverMethod = AI::CrossoverMethod::Middle;

    AI::GeneticAlgorithm<AI::Individual<Chromosome>> ga;
    ga.run(sizeOfPopulation, mutationProbability, crossoverMethod, &os);

    //std::cout << os.str(); // Show the log for debuging

    int actual = Fitness_8queens<Gene>()(ga.getFittest()->getGenes());

    int expected = 100;

    std::cout << "Test 9 : ";
    if (actual == expected)
        std::cout << "Pass" << std::endl;
    else
        std::cout << "Failed (" << std::endl << actual << ')' << std::endl;
}

// Same as 9 but CrossoverMethod::Random
void test10()
{
    const size_t sizeOfChromosome = 8;
    const size_t Max = 8;
    using Seed = AI::Seed_Random<Max>;
    using Gene = AI::Gene<int, Seed>;
    using Chromosome = AI::Chromosome<Gene, Fitness_8queens<Gene>, sizeOfChromosome>;

    std::ostringstream os;

    const size_t sizeOfPopulation = 100;
    const int mutationProbability = 70;
    const AI::CrossoverMethod crossoverMethod = AI::CrossoverMethod::Random;

    AI::GeneticAlgorithm<AI::Individual<Chromosome>> ga;
    ga.run(sizeOfPopulation, mutationProbability, crossoverMethod, &os);

    //std::cout << os.str(); // Show the log for debuging

    int actual = Fitness_8queens<Gene>()(ga.getFittest()->getGenes());

    int expected = 100;

    std::cout << "Test 10 : ";
    if (actual == expected)
        std::cout << "Pass" << std::endl;
    else
        std::cout << "Failed (" << std::endl << actual << ')' << std::endl;
}