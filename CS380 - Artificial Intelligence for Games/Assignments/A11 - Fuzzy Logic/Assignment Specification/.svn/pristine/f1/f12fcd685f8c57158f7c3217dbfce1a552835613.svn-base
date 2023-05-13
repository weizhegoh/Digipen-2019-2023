#include <iostream>
#include <sstream>
#include <string>
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
	AI::FuzzySet_LeftShoulder fs(100.0f, 50.0f, 50.0f);
	float actual1 = fs.calculateDOM(75.0f);
	float actual2 = fs.calculateDOM(100.0f);
	float actual3 = fs.calculateDOM(125.0f);
	float expected1 = 1.0f;
	float expected2 = 1.0f;
	float expected3 = 0.5f;

	std::cout << "Test 1 : ";
	if (actual1 == expected1 &&
		actual2 == expected2 &&
		actual3 == expected3)
		std::cout << "Pass" << std::endl;
	else
		std::cout << "Failed (" << std::endl << actual1 << ','
		<< actual2 << ','
		<< actual3 << ')' << std::endl;
}

void test2()
{
	AI::FuzzySet_RightShoulder fs(100.0f, 50.0f, 50.0f);
	float actual1 = fs.calculateDOM(75);
	float actual2 = fs.calculateDOM(100);
	float actual3 = fs.calculateDOM(125);
	float expected1 = 0.5f;
	float expected2 = 1.0f;
	float expected3 = 1.0f;

	std::cout << "Test 2 : ";
	if (actual1 == expected1 &&
		actual2 == expected2 &&
		actual3 == expected3)
		std::cout << "Pass" << std::endl;
	else
		std::cout << "Failed (" << std::endl << actual1 << ','
		<< actual2 << ','
		<< actual3 << ')' << std::endl;
}


void test3()
{
	AI::FuzzySet_Singleton fs(100.0f, 50.0f, 50.0f);
	float actual1 = fs.calculateDOM(49.0f);
	float actual2 = fs.calculateDOM(51.0f);
	float actual3 = fs.calculateDOM(149.0f);
	float expected1 = 0;
	float expected2 = 1.0f;
	float expected3 = 1.0f;

	std::cout << "Test 3 : ";
	if (actual1 == expected1 &&
		actual2 == expected2 &&
		actual3 == expected3)
		std::cout << "Pass" << std::endl;
	else
		std::cout << "Failed (" << std::endl << actual1 << ','
		<< actual2 << ','
		<< actual3 << ')' << std::endl;
}

void test4()
{
	AI::FuzzySet_Triangle fs(100.0f, 50.0f, 50.0f);
	float actual1 = fs.calculateDOM(75.0f);
	float actual2 = fs.calculateDOM(100.0f);
	float actual3 = fs.calculateDOM(125.0f);
	float expected1 = 0.5f;
	float expected2 = 1.0f;
	float expected3 = 0.5f;

	std::cout << "Test 4 : ";
	if ((actual1 == expected1 &&
		actual2 == expected2 &&
		actual3 == expected3))
		std::cout << "Pass" << std::endl;
	else
		std::cout << "Failed (" << std::endl << actual1 << ','
		<< actual2 << ','
		<< actual3 << ')' << std::endl;
}

void test5()
{
	AI::FuzzyVariable fv;
	fv.addLeftShoulderSet("left", 0.0f, 50.0f, 100.0f)
		.addTriangularSet("center", 50.0f, 100.0f, 150.0f)
		.addRightShoulderSet("right", 100.0f, 150.0f, 200.0f)
		.fuzzify(87.5f);

	fv.ORwithDOM(0.5f);

	float actual1 = fv.getSet("left")->getDOM();
	float actual2 = fv.getSet("center")->getDOM();
	float actual3 = fv.getSet("right")->getDOM();
	float expected1 = 0.5f;
	float expected2 = 0.75f;
	float expected3 = 0.5f;

	float actual4 = fv.deFuzzifyMaxAv();
	float expected4 = 100.0f;
	float actual5 = fv.deFuzzifyCentroid(15);
	float expected5 = 104.966896f;

	std::cout << "Test 5 : ";
	if (actual1 == expected1 &&
		actual2 == expected2 &&
		actual3 == expected3 &&
		actual4 == expected4 &&
		actual5 == expected5)
		std::cout << "Pass" << std::endl;
	else
		std::cout << "Failed (" << std::endl << actual1 << ','
		<< actual2 << ','
		<< actual3 << ','
		<< actual4 << ','
		<< actual5 << ')' << std::endl;
}

void test6()
{
	AI::FuzzyAND f({
		std::shared_ptr<AI::FuzzySet>((new AI::FuzzySet_Triangle(100.0f, 50.0f, 50.0f))->fuzzify(87.5f)),
		std::shared_ptr<AI::FuzzySet>((new AI::FuzzySet_Triangle(200.0f, 50.0f, 50.0f))->fuzzify(225.0f))
	});

	float actual = f.getDOM();
	float expected = 0.5f;

	std::cout << "Test 6 : ";
	if (actual == expected)
		std::cout << "Pass" << std::endl;
	else
		std::cout << "Failed (" << std::endl << actual << ')' << std::endl;
}

void test7()
{
	AI::FuzzyOR f({
		std::shared_ptr<AI::FuzzySet>((new AI::FuzzySet_Triangle(100.0f, 50.0f, 50.0f))->fuzzify(87.5f)),
		std::shared_ptr<AI::FuzzySet>((new AI::FuzzySet_Triangle(200.0f, 50.0f, 50.0f))->fuzzify(225.0f))
	});

	float actual = f.getDOM();
	float expected = 0.75f;

	std::cout << "Test 7 : ";
	if (actual == expected)
		std::cout << "Pass" << std::endl;
	else
		std::cout << "Failed (" << std::endl << actual << ')' << std::endl;
}

void test8()
{
	AI::FuzzyRule rule(
		std::shared_ptr<AI::FuzzyOperator>(new AI::FuzzyAND({
			 std::shared_ptr<AI::FuzzySet>((new AI::FuzzySet_Triangle(100.0f, 50.0f, 50.0f))->fuzzify(87.5f)),
			 std::shared_ptr<AI::FuzzySet>((new AI::FuzzySet_Triangle(200.0f, 50.0f, 50.0f))->fuzzify(225.0f))
		})),
		std::shared_ptr<AI::FuzzySet>(new AI::FuzzySet_Triangle(300.0f, 50.0f, 50.0f))
	);

	float actual = rule.calculate()->getDOM();
	float expected = 0.5f;

	std::cout << "Test 8 : ";
	if (actual == expected)
		std::cout << "Pass" << std::endl;
	else
		std::cout << "Failed (" << std::endl << actual << ')' << std::endl;
}

// In next tests fuzzy logic is used to determine the desirability of a weapon. 
// Each weapon owns its own instance of a fuzzy module because each has  
// a different rule set for inferring desirability.

void test9()
{
	AI::FuzzyModule m;

	m.createVariable("distToTarget")
		.addLeftShoulderSet("close", 0.0f, 25.0f, 150.0f)
		.addTriangularSet("medium", 25.0f, 150.0f, 300.0f)
		.addRightShoulderSet("far", 150.0f, 300.0f, 1000.0f);

	m.createVariable("desirability")
		.addRightShoulderSet("verydesirable", 50.0f, 75.0f, 100.0f)
		.addTriangularSet("desirable", 25.0f, 50.0f, 75.0f)
		.addLeftShoulderSet("undesirable", 0.0f, 25.0f, 50.0f);

	m.createVariable("ammoStatus")
		.addRightShoulderSet("loads", 30.0f, 60.0f, 100.0f)
		.addTriangularSet("okay", 0.0f, 30.0f, 60.0f)
		.addTriangularSet("low", 0.0f, 0.0f, 30.0f);

	//close
	m.addRule(
		std::shared_ptr<AI::FuzzyOperator>(new AI::FuzzyAND({
			m.getVariable("distToTarget").getSet("close"),
			m.getVariable("ammoStatus").getSet("loads")
		})),
		m.getVariable("desirability").getSet("verydesirable"));

	m.addRule(
		std::shared_ptr<AI::FuzzyOperator>(new AI::FuzzyAND({
			m.getVariable("distToTarget").getSet("close"),
			m.getVariable("ammoStatus").getSet("okay")
		})),
		m.getVariable("desirability").getSet("verydesirable"));

	m.addRule(
		std::shared_ptr<AI::FuzzyOperator>(new AI::FuzzyAND({
			m.getVariable("distToTarget").getSet("close"),
			m.getVariable("ammoStatus").getSet("low")
		})),
		m.getVariable("desirability").getSet("verydesirable"));

	//medium
	m.addRule(
		std::shared_ptr<AI::FuzzyOperator>(new AI::FuzzyAND({
			m.getVariable("distToTarget").getSet("medium"),
			m.getVariable("ammoStatus").getSet("loads")
		})),
		m.getVariable("desirability").getSet("verydesirable"));

	m.addRule(
		std::shared_ptr<AI::FuzzyOperator>(new AI::FuzzyAND({
			m.getVariable("distToTarget").getSet("medium"),
			m.getVariable("ammoStatus").getSet("okay")
		})),
		m.getVariable("desirability").getSet("desirable"));

	m.addRule(
		std::shared_ptr<AI::FuzzyOperator>(new AI::FuzzyAND({
			m.getVariable("distToTarget").getSet("medium"),
			m.getVariable("ammoStatus").getSet("low")
		})),
		m.getVariable("desirability").getSet("undesirable"));

	//far
	m.addRule(
		std::shared_ptr<AI::FuzzyOperator>(new AI::FuzzyAND({
			m.getVariable("distToTarget").getSet("far"),
			m.getVariable("ammoStatus").getSet("loads")
		})),
		m.getVariable("desirability").getSet("desirable"));

	m.addRule(
		std::shared_ptr<AI::FuzzyOperator>(new AI::FuzzyAND({
			m.getVariable("distToTarget").getSet("far"),
			m.getVariable("ammoStatus").getSet("okay")
		})),
		m.getVariable("desirability").getSet("undesirable"));

	m.addRule(
		std::shared_ptr<AI::FuzzyOperator>(new AI::FuzzyAND({
			m.getVariable("distToTarget").getSet("far"),
			m.getVariable("ammoStatus").getSet("low")
		})),
		m.getVariable("desirability").getSet("undesirable"));

	// Fuzzify distance and amount of ammo
	m.fuzzify("distToTarget", 150.0f);
	m.fuzzify("ammoStatus", 50.0f);

	// Calculate and defuzzify the result 
	float actual = m.deFuzzify("desirability", AI::FuzzyModule::DefuzzifyMethod::max_av); 

	float expected = 75.0f;

	std::cout << "Test 9 : ";
	if (actual == expected)
		std::cout << "Pass" << std::endl;
	else
		std::cout << "Failed (" << std::endl << actual << ')' << std::endl;
}

void test10()
{
	AI::FuzzyModule m;

	m.createVariable("distToTarget")
		.addLeftShoulderSet("close", 0.0f, 25.0f, 150.0f)
		.addTriangularSet("medium", 25.0f, 150.0f, 300.0f)
		.addRightShoulderSet("far", 150.0f, 300.0f, 1000.0f);

	m.createVariable("desirability")
		.addRightShoulderSet("verydesirable", 50.0f, 75.0f, 100.0f)
		.addTriangularSet("desirable", 25.0f, 50.0f, 75.0f)
		.addLeftShoulderSet("undesirable", 0.0f, 25.0f, 50.0f);

	m.createVariable("ammoStatus")
		.addRightShoulderSet("loads", 10.0f, 30.0f, 100.0f)
		.addTriangularSet("okay", 0.0f, 10.0f, 30.0f)
		.addTriangularSet("low", 0.0f, 0.0f, 10.0f);

	//close
	m.addRule(
		std::shared_ptr<AI::FuzzyOperator>(new AI::FuzzyAND({
			m.getVariable("distToTarget").getSet("close"),
			m.getVariable("ammoStatus").getSet("loads")
		})),
		m.getVariable("desirability").getSet("undesirable"));

	m.addRule(
		std::shared_ptr<AI::FuzzyOperator>(new AI::FuzzyAND({
			m.getVariable("distToTarget").getSet("close"),
			m.getVariable("ammoStatus").getSet("okay")
		})),
		m.getVariable("desirability").getSet("undesirable"));

	m.addRule(
		std::shared_ptr<AI::FuzzyOperator>(new AI::FuzzyAND({
			m.getVariable("distToTarget").getSet("close"),
			m.getVariable("ammoStatus").getSet("low")
		})),
		m.getVariable("desirability").getSet("undesirable"));

	//medium
	m.addRule(
		std::shared_ptr<AI::FuzzyOperator>(new AI::FuzzyAND({
			m.getVariable("distToTarget").getSet("medium"),
			m.getVariable("ammoStatus").getSet("loads")
		})),
		m.getVariable("desirability").getSet("verydesirable"));

	m.addRule(
		std::shared_ptr<AI::FuzzyOperator>(new AI::FuzzyAND({
			m.getVariable("distToTarget").getSet("medium"),
			m.getVariable("ammoStatus").getSet("okay")
		})),
		m.getVariable("desirability").getSet("verydesirable"));

	m.addRule(
		std::shared_ptr<AI::FuzzyOperator>(new AI::FuzzyAND({
			m.getVariable("distToTarget").getSet("medium"),
			m.getVariable("ammoStatus").getSet("low")
		})),
		m.getVariable("desirability").getSet("desirable"));

	//far
	m.addRule(
		std::shared_ptr<AI::FuzzyOperator>(new AI::FuzzyAND({
			m.getVariable("distToTarget").getSet("far"),
			m.getVariable("ammoStatus").getSet("loads")
		})),
		m.getVariable("desirability").getSet("desirable"));

	m.addRule(
		std::shared_ptr<AI::FuzzyOperator>(new AI::FuzzyAND({
			m.getVariable("distToTarget").getSet("far"),
			m.getVariable("ammoStatus").getSet("okay")
		})),
		m.getVariable("desirability").getSet("undesirable"));

	m.addRule(
		std::shared_ptr<AI::FuzzyOperator>(new AI::FuzzyAND({
			m.getVariable("distToTarget").getSet("far"),
			m.getVariable("ammoStatus").getSet("low")
		})),
		m.getVariable("desirability").getSet("undesirable"));

	// Fuzzify distance and amount of ammo
	m.fuzzify("distToTarget", 200.0f);
	m.fuzzify("ammoStatus", 8.0f);

	// Calculate and defuzzify the result 
	float actual = m.deFuzzify("desirability", AI::FuzzyModule::DefuzzifyMethod::max_av); 
	float expected = 60.4166641f;

	std::cout << "Test 10 : ";
	if (actual == expected)
		std::cout << "Pass" << std::endl;
	else
		std::cout << "Failed (" << std::endl << actual << ')' << std::endl;
}