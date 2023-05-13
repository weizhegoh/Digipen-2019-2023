/*!*****************************************************************************
\file functions.h
\author Vadim Surov, Goh Wei Zhe
\par DP email: vsurov\@digipen.edu, weizhe.goh\@digipen.edu
\par Course: CS380
\par Section: B
\par Programming Assignment 11
\date 07-26-2021
\brief
This file has declarations and definitions that are required for submission
*******************************************************************************/
#ifndef FUNCTIONS_H
#define FUNCTIONS_H

#include <iostream>
#include <list>
#include <map>
#include <climits>
#include <cfloat> // FLT_EPSILON, ...
#include <memory> // shared_ptr

#include "data.h"

#define UNUSED(x) (void)x;

namespace AI
{
	// Comparison function for floats
	inline bool isEqual(float a, float b)
	{
		float epsilon = 128 * FLT_EPSILON;
		float abs_th = FLT_MIN;

		if (a == b) return true;

		float diff = std::abs(a - b);
		float norm = std::min((std::abs(a) + std::abs(b)), 
						std::numeric_limits<float>::max());

		return diff < std::max(abs_th, epsilon* norm);
	}

	//  Definition of the base fuzzy set class
	class FuzzySet
	{
	protected:
		// Members that define the shape
		float peakPoint;
		float leftOffset;
		float rightOffset;

		// representativeValue - the maximum of the set's membership function. 
		//For instance, if the set is triangular then this will be the peak 
		//point of the triangular. If the set has a plateau then this value 
		//will be the mid point of the plateau. This value is set in creation 
		//to avoid run-time calculation of mid-point values.
		float representativeValue;

		// DOM - holds the degree of membership of a given value in this set
		float DOM;

	public:
		FuzzySet(float peakPoint, float leftOffset, float rightOffset, 
			float representativeValue)
			: peakPoint{ peakPoint }, leftOffset{ leftOffset }, 
			rightOffset{ rightOffset },
			representativeValue{ representativeValue }, DOM{ 0.0f }{}

		virtual ~FuzzySet(){}

		/***********************************************************************
		\brief
		Calculates the degree of membership for a particular value. It does not 
		set DOM to the value passed as the parameter because the centroid 
		defuzzification method also uses this method to determind the degree of 
		memberships of the values it uses as its sample points. 

		\param val
		Value to calculate for.

		\return
		Returns the degree of membership
		***********************************************************************/
		virtual float calculateDOM(float val) const
		{
			UNUSED(val);

			return 0.0f;
		}

		void clearDOM() 
		{ 
			DOM = 0.0f; 
		}

		float getDOM() const 
		{ 
			return DOM; 
		}

		void setDOM(float val)
		{ 
			DOM = val; 
		}

		float getRepresentativeValue() const
		{
			return representativeValue;
		}

		/***********************************************************************
		\brief
		If this fuzzy set is part of a consequent floatiable, and it is fired by
		a rule then this method sets the DOM (in this context, DOM represents a 
		confidence level) to the maximum of the parameter value or the set's 
		existing member DOM value. 

		\param val
		Value to caculate for.
		***********************************************************************/
		void ORwithDOM(float val)
		{
			if (val > DOM)
				DOM = val;
		}

		// Fuzzify a value by calculating its degree of membership
		FuzzySet* fuzzify(float val)
		{
			DOM = calculateDOM(val);
			return this;
		}
	};

	//  Definition of a fuzzy set that has a left shoulder shape.
	class FuzzySet_LeftShoulder : public FuzzySet
	{
	public:
		FuzzySet_LeftShoulder(float peakPoint, float leftOffset,
			float rightOffset)
			: FuzzySet(peakPoint, leftOffset, rightOffset,
				peakPoint - leftOffset / 2){}

		/***********************************************************************
		\brief
		Calculates the degree of membership for a particular value. It does not
		set DOM to the value passed as the parameter because the centroid
		defuzzification method also uses this method to determind the degree of
		memberships of the values it uses as its sample points.

		\param val
		Value to calculate for.

		\return
		Returns the degree of membership
		***********************************************************************/
		float calculateDOM(float val) const
		{
			// Test for the case where the left or right offsets are zero
			if (isEqual(this->peakPoint, val) &&
				(isEqual(this->leftOffset, 0.0f) ||
				isEqual(this->rightOffset, 0.0f)))
				return 1.0f;
			
			// Find DOM if right of center
			if ((val >= this->peakPoint) &&
				(val < (this->peakPoint + this->rightOffset)))
				return (1.0f / -this->rightOffset) * 
				(val - (this->peakPoint)) + 1.0f;

			// Find DOM if left of center
			if ((val < this->peakPoint) &&
				(val >= this->peakPoint - this->leftOffset))
				return 1.0f;

			// Out of range
			return 0.0f;
		}

	};

	// Definition of a fuzzy set that has a right shoulder shape.
	class FuzzySet_RightShoulder : public FuzzySet
	{
	public:
		FuzzySet_RightShoulder(float peakPoint, float leftOffset,
			float rightOffset)
			: FuzzySet(peakPoint, leftOffset, rightOffset,
				peakPoint + rightOffset / 2){}

		/***********************************************************************
		\brief
		Calculates the degree of membership for a particular value. It does not
		set DOM to the value passed as the parameter because the centroid
		defuzzification method also uses this method to determind the degree of
		memberships of the values it uses as its sample points.

		\param val
		Value to calculate for.

		\return
		Returns the degree of membership
		***********************************************************************/
		float calculateDOM(float val) const
		{
			// Test for the case where the left or right offsets are zero
			// (to prevent divide by zero errors below)
			if (isEqual(this->peakPoint, val) &&
				(isEqual(this->leftOffset, 0.0f) ||
				isEqual(this->rightOffset, 0.0f)))
				return 1.0f;

			// Find DOM if left of center
			if ((val <= this->peakPoint) &&
				(val > (this->peakPoint - this->leftOffset)))
				return (1.0f / this->leftOffset) *
				(val - (this->peakPoint - this->leftOffset));

			// Find DOM if right of center and less than center + right offset
			if ((val > this->peakPoint) &&
				(val <= this->peakPoint + this->rightOffset))
				return 1.0f;

			// Out of range
			return 0.0f;
		}
	};

	// This defines a fuzzy set that is a singleton (a range
	// over which the DOM is always 1.0f)
	class FuzzySet_Singleton : public FuzzySet
	{
	public:
		FuzzySet_Singleton(float peakPoint, float leftOffset, float rightOffset)
			: FuzzySet(peakPoint, leftOffset, rightOffset, peakPoint){}

		/***********************************************************************
		\brief
		Calculates the degree of membership for a particular value. It does not
		set DOM to the value passed as the parameter because the centroid
		defuzzification method also uses this method to determind the degree of
		memberships of the values it uses as its sample points.

		\param val
		Value to calculate for.

		\return
		Returns the degree of membership
		***********************************************************************/
		float calculateDOM(float val) const
		{
			if ((val >= this->peakPoint - this->leftOffset) &&
				(val <= this->peakPoint + this->rightOffset))
				return 1.0f;

			return 0.0f;
		}
	};

	// This is a simple class to define fuzzy sets that have a triangular 
	// shape and can be defined by a mid point, a left displacement and a
	// right displacement. 
	class FuzzySet_Triangle : public FuzzySet
	{
	public:
		FuzzySet_Triangle(float peakPoint, float leftOffset, float rightOffset)
			: FuzzySet(peakPoint, leftOffset, rightOffset, peakPoint){}

		/***********************************************************************
		\brief
		Calculates the degree of membership for a particular value. It does not
		set DOM to the value passed as the parameter because the centroid
		defuzzification method also uses this method to determind the degree of
		memberships of the values it uses as its sample points.

		\param val
		Value to calculate for.

		\return
		Returns the degree of membership
		***********************************************************************/
		float calculateDOM(float val) const
		{
			// Test for the case where the left or right offsets are zero
			// (to prevent divide by zero errors below)
			if (isEqual(this->peakPoint, val) &&
				(isEqual(this->leftOffset, 0.0) ||
				isEqual(this->rightOffset, 0.0)))
				return 1.0f;

			// Find DOM if left of center
			if ((val <= this->peakPoint) &&
				(val >= (this->peakPoint - this->leftOffset)))
				return (1.0f / this->leftOffset) *
						(val - (this->peakPoint - this->leftOffset));

			// Find DOM if right of center
			if ((val > this->peakPoint) &&
				(val < (this->peakPoint + this->rightOffset)))
				return (1.0f / -this->rightOffset) *
						(val - (this->peakPoint)) + 1.0f;

			// Out of range
			return 0.0f;
		}
	};

	// Fuzzy logic works with membership values in a way that mimics Boolean
	//logic. Replacements for basic logic operators AND and OR are defined here.

	// Definition of the base operator class
	class FuzzyOperator
	{
	protected:
		std::list<std::shared_ptr<FuzzySet>> sets;

	public:
		FuzzyOperator(std::initializer_list<std::shared_ptr<FuzzySet>> sets={})
			: sets{ sets }{}

		virtual ~FuzzyOperator(){}

		// Returns the minimum DOM of the sets it is operating on
		virtual float getDOM()
		{
			return 0;
		}

		void clearDOM()
		{
			for (std::shared_ptr<FuzzySet> set : sets)
				set->clearDOM();
		}

		void ORwithDOM(float val)
		{
			for (std::shared_ptr<FuzzySet> set : sets)
				set->ORwithDOM(val);
		}
	};

	//  Definition of the AND operator class
	class FuzzyAND : public FuzzyOperator
	{
	public:
		FuzzyAND(std::initializer_list<std::shared_ptr<FuzzySet>> sets = {})
			: FuzzyOperator{ sets }{}

		/***********************************************************************
		\brief
		Retrieve the smallest DOM value within the sets.

		\return
		Returns the smallest DOM value. 
		***********************************************************************/
		float getDOM()
		{
			float smallest = static_cast<float>(INT_MAX);

			for (auto& set : sets)
			{
				float cur = set->getDOM();
				if (cur < smallest)
					smallest = cur;
			}
			return smallest == static_cast<float>(INT_MAX) ? 0.0f : smallest;
		}
	};

	// Definition of the OR operator class
	class FuzzyOR : public FuzzyOperator
	{
	public:
		FuzzyOR(std::initializer_list<std::shared_ptr<FuzzySet>> sets = {})
			: FuzzyOperator{ sets }{}

		/***********************************************************************
		\brief
		Retrieve the largest DOM value within the sets.

		\return
		Returns the largest DOM value.
		***********************************************************************/
		float getDOM()
		{
			float largest = static_cast<float>(INT_MIN);

			for (auto& set : sets)
			{
				float cur = set->getDOM();
				if (cur > largest)
					largest = cur;
			}
			return largest == static_cast<float>(INT_MIN) ? 0.0f : largest;
		}
	};

	// Definition of the fuzzy variable class
	class FuzzyVariable
	{
	protected:
		// A map of the fuzzy sets that comprise this variable
		std::map<std::string, std::shared_ptr<FuzzySet>> sets;

		// The minimum and maximum value of the range of this variable
		float minRange;
		float maxRange;

	public:
		FuzzyVariable(): sets{ }, minRange{ 0.0f }, maxRange{ 0.0f }{}

		virtual ~FuzzyVariable(){}

		std::shared_ptr<FuzzySet> getSet(const std::string& name)
		{
			return sets[name];
		}

		// This method is called with the upper and lower bound of a set 
		//each time a new set is added to adjust the upper and lower range 
		//values accordingly
		void adjustRangeToFit(float minBound, float maxBound)
		{
			if (minBound < minRange)
				minRange = minBound;
			if (maxBound > maxRange)
				maxRange = maxBound;
		}

		// The following methods create instances of the sets named in the 
		//method name and add them to the member set map. Each time a set of 
		//any type is added the minRange and maxRange are adjusted accordingly.

		// Adds a left shoulder type set
		FuzzyVariable& addLeftShoulderSet(const std::string& name, 
			float minBound, float peak, float maxBound)
		{
			sets.insert(std::pair<std::string, std::shared_ptr<FuzzySet>>(name, 
				std::shared_ptr<FuzzySet>(new FuzzySet_LeftShoulder(peak,
					peak - minBound, maxBound - peak))));
			
			adjustRangeToFit(minBound, maxBound);
			return *this;
		}

		// Adds a left shoulder type set
		FuzzyVariable& addRightShoulderSet(const std::string& name, 
			float minBound, float peak, float maxBound)
		{
			sets.insert(std::pair<std::string, std::shared_ptr<FuzzySet>>(name, 
				std::shared_ptr<FuzzySet>(new FuzzySet_RightShoulder(peak,
					peak - minBound, maxBound - peak))));
			
			adjustRangeToFit(minBound, maxBound);
			return *this;
		}

		// Adds a triangular shaped fuzzy set to the variable
		FuzzyVariable& addTriangularSet(const std::string& name, float minBound,
			float peak, float maxBound)
		{
			sets.insert(std::pair<std::string, std::shared_ptr<FuzzySet>>(name, 
				std::shared_ptr<FuzzySet>(new FuzzySet_Triangle(peak, 
					peak - minBound, maxBound - peak))));
			
			adjustRangeToFit(minBound, maxBound);
			return *this;
		}

		// Adds a singleton to the variable
		FuzzyVariable& addSingletonSet(const std::string& name, float minBound,
			float peak, float maxBound)
		{
			sets.insert(std::pair<std::string, std::shared_ptr<FuzzySet>>(name, 
				std::shared_ptr<FuzzySet>(new FuzzySet_Singleton(peak, 
					peak - minBound, maxBound - peak))));
			
			adjustRangeToFit(minBound, maxBound);
			return *this;
		}

		// Fuzzify a value by calculating its degree of membership in each of 
		//this variable's subsets. Takes a crisp value and calculates its 
		//degree of membership for each set in the variable.
		FuzzyVariable* fuzzify(float val)
		{
			//for each set in the flv calculate the degree of membership for 
			//the given value
			for (std::pair<std::string, std::shared_ptr<FuzzySet>> set : sets)
				set.second->setDOM(set.second->calculateDOM(val));
			
			return this;
		}

		// Method for updating the DOM of a consequent when a rule fires
		void ORwithDOM(float val)
		{
			for (std::pair<std::string, std::shared_ptr<FuzzySet>> set : sets)
				set.second->ORwithDOM(val);
		}

		/***********************************************************************
		\brief
		Defuzzifies the value by averaging the maxima of the sets that have
		fired. 

		\return
		Returns sum of (maxima * degree of membership) / sum (degree of
		memberships)
		***********************************************************************/
		float deFuzzifyMaxAv()
		{
			float bottom = 0.0f;
			float top = 0.0f;

			for (std::pair<std::string, std::shared_ptr<FuzzySet>> set : sets)
			{
				bottom += set.second->getDOM();

				top += set.second->getRepresentativeValue() *
						set.second->getDOM();
			}

			// Make sure bottom is not equal to zero
			if (isEqual(0, bottom))
				return 0.0f;

			return top / bottom;
		}

		/***********************************************************************
		\brief
		Defuzzify the variable using the centroid method. 

		\param numSamples
		Number of Samples.

		\return
		Returns the centroid by dividing total area by the sum of moments, like
		calculating the center of mass of an object. 
		***********************************************************************/
		float deFuzzifyCentroid(int numSamples)
		{
			// Calculate the step size
			float stepSize = (this->maxRange - this->minRange) /
								static_cast<float>(numSamples);
			
			float totalArea = 0.0f;
			float sumOfMoments = 0.0f;

			// Step through the range of this variable in increments equal to 
			// stepSize adding up the contribution (lower of calculateDOM or the
			// actual degree of membership of this variable's fuzzified value)
			// for each subset. This gives an approximation of the total area of
			// the fuzzy manifold.(This is similar to how the area under a curve
			// is calculated using calculus... the heights of lots of 'slices' 
			// are summed to give the total area.)
			// In addition the moment of each slice is calculated and summed. 
			for (int samp = 1; samp <= numSamples; ++samp)
			{
				// for each set get the contribution to the area. This is the 
				// lower of the value returned from calculateDOM or the actual
				// degree of membership of the fuzzified value itself   
				for (std::pair<std::string, 
					std::shared_ptr<FuzzySet>> set : sets)
				{
					float contribution = std::min(
					set.second->calculateDOM(this->minRange + samp * stepSize),
					set.second->getDOM());

					totalArea += contribution;
					sumOfMoments += (this->minRange + samp * stepSize) * 
										contribution;
				}
			}

			// Make sure total area is not equal to zero
			if (isEqual(0, totalArea))
				return 0.0;

			return sumOfMoments / totalArea;
		}
	};

	// Definition of the fuzzy rule class of form
	// IF antecedent THEN consequence
	class FuzzyRule
	{
	protected:
		// Antecedent (usually a composite of several fuzzy sets and operators)
		std::shared_ptr<FuzzyOperator> antecedent;

		// Consequence (usually a single fuzzy set, but can be several ANDed 
		//together)
		std::shared_ptr<FuzzySet> consequence;

	public:
		FuzzyRule(std::shared_ptr<FuzzyOperator> antecedent,
			std::shared_ptr<FuzzySet> consequence)
			: antecedent{ antecedent }, consequence{ consequence }{}

		// Updates the DOM (the confidence) of the consequent set with
		// the DOM of the antecedent set. 
		std::shared_ptr<FuzzySet> calculate()
		{
			consequence->ORwithDOM(antecedent->getDOM());
			return consequence;
		}

		void setConfidenceOfConsequentToZero()
		{
			consequence->clearDOM();
		}
	};

	// Definition of the fuzzy module class
	class FuzzyModule
	{
	public:
		// Defuzzify methods supported by this module.
		enum DefuzzifyMethod { max_av = 0, centroid = 1 };

	protected:

		// When calculating the centroid of the fuzzy manifold this value is 
		// used to determine how many cross-sections should be sampled
		int numSamples;

		// A map of all the fuzzy variables this module uses
		std::map<std::string, FuzzyVariable> variables;

		// An array containing all fuzzy rules
		std::list<FuzzyRule> rules;

	public:
		FuzzyModule(): numSamples{ 15 }, variables{ }, rules{ }{}

		FuzzyVariable& getVariable(const std::string& name)
		{
			return variables[name];
		}

		// Zeros the DOMs of the consequents of each rule. Used by Defuzzify()
		void setConfidencesOfConsequentsToZero()
		{
			for (FuzzyRule rule : rules)
				rule.setConfidenceOfConsequentToZero();
		}

		// Creates and return a new 'empty' fuzzy variable.
		FuzzyVariable& createVariable(std::string varName)
		{
			variables.insert(std::pair<std::string, FuzzyVariable>
				(varName, FuzzyVariable()));
			
			return variables[varName];
		}

		// Adds a rule to the module
		void addRule(std::shared_ptr<FuzzyOperator> antecedent, 
			std::shared_ptr<FuzzySet> consequence)
		{
			rules.push_back(FuzzyRule(antecedent, consequence));
		}

		// Calls the Fuzzify method of the variable with the same name
		void fuzzify(const std::string& varName, float val)
		{
			variables[varName].fuzzify(val);
		}

		/***********************************************************************
		\brief
		Defuzzify the variable given a fuzzy variable and a deffuzification 
		method.

		\param varName
		String of variable names. 

		\param method
		Deffuzifaction method. 

		\return
		Returns a crsip value. 
		***********************************************************************/
		float deFuzzify(const std::string& varName, DefuzzifyMethod method)
		{
			// Clear the DOMs of all the consequents of all the rules
			this->setConfidencesOfConsequentsToZero();
			// Process the rules
			for (FuzzyRule rule : rules)
				rule.calculate();
			
			// Defuzzify the resultant conclusion using the specified method
			switch (method) 
			{
				case FuzzyModule::DefuzzifyMethod::centroid:
					return this->variables[varName].deFuzzifyCentroid
							(this->numSamples);
				
				case FuzzyModule::DefuzzifyMethod::max_av:
					return this->variables[varName].deFuzzifyMaxAv();
			}

			return 0.0f;
		}
	};

} // end namespace

#endif