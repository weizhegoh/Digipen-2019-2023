#ifndef FUNCTIONS_H
#define FUNCTIONS_H

#include <iostream>
#include <sstream>
#include <vector>

#include "data.h"

#define UNUSED(x) (void)x;

// Returns result of addition of all genes in a chromosome
template<typename Gene>
struct Fitness_Accumulate
{
    int operator()(const std::vector<Gene>& genes) const
    {
        UNUSED(genes);

        // Your code ...

        return 0;
    }
};

// Returns a measure in percentages that indicates the fitness of  
// a particular chromosome into a particular solution.
// Fittest chromosome has all genes equal 1.
template<typename Gene>
struct Fitness_Nbits
{
    int operator()(const std::vector<Gene>& genes) const
    {
        UNUSED(genes);

        // Your code ...

        return 0;
    }
};

// Returns a measure in percentages that indicates the fitness of  
// a particular chromosome into a particular solution of 8 queens 
// problem
template<typename Gene>
struct Fitness_8queens
{
    int operator()(const std::vector<Gene>& genes) const
    {
        UNUSED(genes);

        // Your code ...

        return 0;
    }
};


namespace AI
{
    // Crossover methods for the genetic algorithm
    enum CrossoverMethod { Middle, Random  };
    
    // Simplest gene seeding class/function
    struct Seed
    {
        int operator()(int p = 0) const
        {
            return p;
        }
    };

    // Gene seeding class/function with a fixed value
    template<int Val = 0>
    struct Seed_Value
    {
        int operator()(int /* p */ = 0) const
        {
            return Val;
        }
    };

    // Gene random seeding class/function
    template<int Max>
    struct Seed_Random
    {
        int operator()(int /* p */ = 0) const
        {
            // Your code ...
            return 0;
        }
    };

    // Gene class
    template<typename T = int, typename S = Seed>
    class Gene
    {
        T value;

    public:
        Gene(int p = 0)
            : value{ S()(p) }
        {
        }

        T getValue() const
        {
            return value;
        }

        void setValue(T v)
        {
            value = v;
        }

        friend std::ostream& operator<<(std::ostream& os, const Gene& rhs)
        {
            os << rhs.value;
            return os;
        }
    };

    // Chromosome class
    template<typename Gene, typename Fitness, size_t Size>
    class Chromosome
    {
        std::vector<Gene> genes;
        int fitness;

    public:

        using gene_type = Gene;

        static const size_t size = Size;

        Chromosome()
            : genes(Size), fitness{ Fitness()(genes) }
        {
        }

        std::vector<Gene>& getGenes()
        {
            return genes;
        }

        void setGenes(const std::vector<Gene>& v)
        {
            genes = v;
            fitness = Fitness()(genes);
        }

        Gene getGene(size_t i) const
        {
            return genes[i];
        }

        void setGene(size_t i, const Gene& v)
        {
            genes[i] = v;
            fitness = Fitness()(genes);
        }

        int getFitness() const
        {
            return fitness;
        }

        // Select a random mutation point and change 
        // gene at the mutation point
        void randomMutation()
        {
            setGene(std::rand() % Chromosome::size, Gene());
        }

        // Copy genes from a source
        void copyGenesFrom(Chromosome& src)
        {
            std::copy(src.genes.begin(), src.genes.end(), genes.begin());
            fitness = Fitness()(genes);
        }

        friend std::ostream& operator<<(std::ostream& os, 
            const Chromosome& rhs)
        {
            os << '[';
            for (auto it = rhs.genes.begin(); it != rhs.genes.end(); ++it)
                os << *it << (it + 1 != rhs.genes.end() ? "," : "");
            os << "]=" << rhs.fitness;
            return os;
        }
    };

    // Individual class
    template<typename Chromosome>
    class Individual
    {
        Chromosome chromosome;
 
    public:

        using chromosome_type = Chromosome;
        using gene_type = typename Chromosome::gene_type;

        Individual()
            : chromosome{ }
        {
        }

        Chromosome& getChromosome()
        {
            return chromosome;
        }

        std::vector<gene_type>& getGenes()
        {
            return chromosome.getGenes();
        }

        void setGenes(const std::vector<gene_type>& v)
        {
            chromosome.setGenes(v);
        }

        gene_type getGene(size_t i) const
        {
            return chromosome.getGene(i);
        }

        void copyGenesFrom(Individual& individual)
        {
            chromosome.copyGenesFrom(individual.chromosome);
        }

        void setGene(size_t i, gene_type gene)
        {
            chromosome.setGene(i, gene);
        }

        int getFitness() const
        {
            return chromosome.getFitness();
        }

        friend std::ostream& operator<<(std::ostream& os, Individual& rhs)
        {
            os << rhs.chromosome;
            return os;
        }
    };

    // Population class
    template<typename Individual>
    class Population
    {
        std::vector<Individual> individuals;
        Individual* fittest;

    public:
        Population(size_t size = 0)
            : individuals{ }, fittest{ nullptr }
        {
            if (size)
            {
                individuals.resize(size);
                updateFittest();
            }
        }

        size_t getSize() const
        {
            return individuals.size();
        }

        Individual& getIndividual(size_t i)
        {
            return individuals[i];
        }

        Individual* getFittest() const
        {
            return fittest;
        }

        void updateFittest()
        {
            // Your code ...
        }

        friend std::ostream& operator<<(std::ostream& os, Population& rhs)
        {
            os << " = " << rhs.getFittest()->getFitness() << std::endl;
            for (size_t i = 0; i < rhs.getSize(); ++i)
                os << "  " << i << ':' << rhs.getIndividual(i) << std::endl;
            return os;
        }
    };
 
    // Genetic Algorithm class
    template<typename Individual>
    class GeneticAlgorithm
    {
        Population<Individual>* population;
        int generation;
 
    public:
        GeneticAlgorithm()
            : population{ nullptr }, generation{ 0 }
        {
        }

        ~GeneticAlgorithm()
        {
            // Your code ...
        }

        Individual* getFittest() const
        {

            // Your code ...
            
            return nullptr;
        }

        // Implementation of the Roulette Wheel Selection. The probability of an 
        // individual to be selected is directly proportional to its fitness.
        Population<Individual>* selection(size_t sizeOfPopulation)
        {
            UNUSED(sizeOfPopulation);

            // Your code ...

            return nullptr;
        }

        // Crossover parents genes
        void crossover(Population<Individual>* newGeneration, 
            CrossoverMethod crossoverMethod)
        {
            UNUSED(newGeneration);
            UNUSED(crossoverMethod);

            // Your code ...
        }

        // Do mutation of genes under a random probability
        void mutation(Population<Individual>* newGeneration, 
            int mutationProbability)
        {
            UNUSED(newGeneration);
            UNUSED(mutationProbability);

            // Your code ...
        }

        // Replace existing population if any with a new generation
        void setPopulation(Population<Individual>* newGeneration)
        {
            UNUSED(newGeneration);

            // Your code ...
        }

        // Start the search
        void run(size_t sizeOfPopulation = 100, int mutationProbability = 70, 
                    CrossoverMethod crossoverMethod = CrossoverMethod::Middle, 
                    std::ostringstream* os = nullptr)
        {
            UNUSED(sizeOfPopulation);
            UNUSED(mutationProbability);
            UNUSED(crossoverMethod);
            UNUSED(os);

            // Your code ...
        }

        // Continue the search
        bool next(int mutationProbability, CrossoverMethod crossoverMethod, 
                    std::ostringstream* os)
        {
            UNUSED(mutationProbability);
            UNUSED(crossoverMethod);
            UNUSED(os);

            // Your code ...

            return true;
        }
    };

} // end namespace

#endif