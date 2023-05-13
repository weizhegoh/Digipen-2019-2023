#include <iostream>
#include <sstream>
#include <string>
#include <vector>
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
	AI::Log log;

	AI::Task{ "0.1" }(&log);
	AI::Task{ "0.2", AI::State::Failure }(&log);
	AI::Task{ "0.3", AI::State::Success }(&log);

	std::string actual = log.str();
	std::string expected = "\
Task(0.1)\n\
L Undefined\n\
Task(0.2)\n\
L Failure\n\
Task(0.3)\n\
L Success\n\
";

	if (actual != expected)
		std::cout << "Test 0 : Failed (" << std::endl << actual << ')' << std::endl;
}

void test1()
{
	AI::Log log;

	AI::CheckState{ AI::Task{ "1.1", AI::State::Success }(), AI::State::Success }(&log);
	AI::CheckState{ AI::Task{ "1.2", AI::State::Success }(), AI::State::Failure }(&log);

	std::string actual = log.str();
	std::string expected = "\
CheckState(1.1,Success)\n\
L Success\n\
CheckState(1.2,Failure)\n\
L Failure\n\
";

	std::cout << "Test 1 : ";
	if (actual == expected)
		std::cout << "Pass" << std::endl;
	else
		std::cout << "Failed (" << std::endl << actual << ')' << std::endl;
}

void test2()
{
	using _ = AI::SMART;

	AI::Log log;

	AI::Selector{ }(&log);
	AI::Selector
	{
		{
			_{ new AI::Task{ "2.1", AI::State::Failure } },
			_{ new AI::Task{ "2.2", AI::State::Success } },
			_{ new AI::Task{ "2.3", AI::State::Success } }
		}
	}(&log);

	std::string actual = log.str();
	std::string expected = "\
Selector()\n\
L Failure\n\
Selector()\n\
| Task(2.1)\n\
| L Failure\n\
| Task(2.2)\n\
| L Success\n\
L Success\n\
";

	std::cout << "Test 2 : ";
	if (actual == expected)
		std::cout << "Pass" << std::endl;
	else
		std::cout << "Failed (" << std::endl << actual << ')' << std::endl;
}

void test3()
{
	using _ = AI::SMART;

	AI::Log log;

	AI::Sequence{ }(&log);
	AI::Sequence
	{
		{
			_{ new AI::Task{ "3.1", AI::State::Success } },
			_{ new AI::Task{ "3.2", AI::State::Success } },
			_{ new AI::Task{ "3.3", AI::State::Failure } },
			_{ new AI::Task{ "3.4", AI::State::Success } },
			_{ new AI::Task{ "3.5", AI::State::Success } }
		}
	}(&log);

	std::string actual = log.str();
	std::string expected = "\
Sequence()\n\
L Success\n\
Sequence()\n\
| Task(3.1)\n\
| L Success\n\
| Task(3.2)\n\
| L Success\n\
| Task(3.3)\n\
| L Failure\n\
L Failure\n\
";

	std::cout << "Test 3 : ";
	if (actual == expected)
		std::cout << "Pass" << std::endl;
	else
		std::cout << "Failed (" << std::endl << actual << ')' << std::endl;
}

void test4()
{
	using _ = AI::SMART;

	AI::Log log;

	AI::RandomSelector
	{
		{
			_{ new AI::Task{ "4", AI::State::Success } },
			_{ new AI::Task{ "4", AI::State::Success } },
			_{ new AI::Task{ "4", AI::State::Success } }
		}
	}(&log);

	std::string actual = log.str();
	std::string expected = "\
RandomSelector()\n\
| Task(4)\n\
| L Success\n\
L Success\n\
";

	std::cout << "Test 4 : ";
	if (actual == expected)
		std::cout << "Pass" << std::endl;
	else
		std::cout << "Failed (" << std::endl << actual << ')' << std::endl;
}

void test5()
{
	using _ = AI::SMART;

	AI::Log log;

	AI::Inverter{ _{ new AI::Task{ "5.1", AI::State::Success } } }(&log);
	AI::Inverter{ _{ new AI::Task{ "5.2", AI::State::Failure } } }(&log);

	std::string actual = log.str();
	std::string expected = "\
Inverter()\n\
| Task(5.1)\n\
| L Success\n\
L Failure\n\
Inverter()\n\
| Task(5.2)\n\
| L Failure\n\
L Success\n\
";

	std::cout << "Test 5 : ";
	if (actual == expected)
		std::cout << "Pass" << std::endl;
	else
		std::cout << "Failed (" << std::endl << actual << ')' << std::endl;
}

void test6()
{
	using _ = AI::SMART;

	AI::Log log;

	AI::Succeeder{ _{ new AI::Task{ "6.1", AI::State::Success } } }(&log);
	AI::Succeeder{ _{ new AI::Task{ "6.2", AI::State::Failure } } }(&log);

	std::string actual = log.str();
	std::string expected = "\
Succeeder()\n\
| Task(6.1)\n\
| L Success\n\
L Success\n\
Succeeder()\n\
| Task(6.2)\n\
| L Failure\n\
L Success\n\
";

	std::cout << "Test 6 : ";
	if (actual == expected)
		std::cout << "Pass" << std::endl;
	else
		std::cout << "Failed (" << std::endl << actual << ')' << std::endl;
}

void test7()
{
	using _ = AI::SMART;

	AI::Log log;

	AI::Repeater{ }(&log);
	AI::Repeater{ _{ new AI::Task{ "7", AI::State::Success } }, 3 }(&log);

	std::string actual = log.str();
	std::string expected = "\
Repeater(0)\n\
L Success\n\
Repeater(3)\n\
| Task(7)\n\
| L Success\n\
| Task(7)\n\
| L Success\n\
| Task(7)\n\
| L Success\n\
L Success\n\
";

	std::cout << "Test 7 : ";
	if (actual == expected)
		std::cout << "Pass" << std::endl;
	else
		std::cout << "Failed (" << std::endl << actual << ')' << std::endl;
}

void test8()
{
	using _ = AI::SMART;

	AI::Log log;

	AI::Repeat_until_fail{ }(&log);
	AI::Repeat_until_fail{ _{ new AI::Task{ "8", AI::State::Failure } } }(&log);

	std::string actual = log.str();
	std::string expected = "\
Repeat_until_fail()\n\
L Success\n\
Repeat_until_fail()\n\
| Task(8)\n\
| L Failure\n\
L Success\n\
";

	std::cout << "Test 8 : ";
	if (actual == expected)
		std::cout << "Pass" << std::endl;
	else
		std::cout << "Failed (" << std::endl << actual << ')' << std::endl;
}

void test9()
{
	using _ = AI::SMART;

	AI::Log log;

	AI::Sequence
	{
		{
			_{ new AI::Task{ "9.1", AI::State::Success } },
			_{ new AI::Selector
				{ 
					{
						_{ new AI::Sequence
							{
								{
									_{ new AI::Inverter{ _{	new AI::Task{ "9.2", AI::State::Success } } } },
									_{ new AI::Inverter{ _{ new AI::Task{ "9.3", AI::State::Success } } } }
								}
							}
						},
						_{ new AI::Sequence
							{
								{
									_{ new AI::Task{ "9.4", AI::State::Success } },
									_{ new AI::Task{ "9.5", AI::State::Success } }
								}
							}
						}
					} 
				} 
			}
		} 
	}(&log);

	std::string actual = log.str();
	std::string expected = "\
Sequence()\n\
| Task(9.1)\n\
| L Success\n\
| Selector()\n\
| | Sequence()\n\
| | | Inverter()\n\
| | | | Task(9.2)\n\
| | | | L Success\n\
| | | L Failure\n\
| | L Failure\n\
| | Sequence()\n\
| | | Task(9.4)\n\
| | | L Success\n\
| | | Task(9.5)\n\
| | | L Success\n\
| | L Success\n\
| L Success\n\
L Success\n\
";

	std::cout << "Test 9 : ";
	if (actual == expected)
		std::cout << "Pass" << std::endl;
	else
		std::cout << "Failed (" << std::endl << actual << ')' << std::endl;
}

// Same test as 9 but using named pointers to tasks
void test10()
{
	using _ = AI::SMART;

	AI::Log log;

	_ p10_1{ new AI::Task{ "10.1", AI::State::Success } };
	_ p10_2{ new AI::Task{ "10.2", AI::State::Success } };
	_ p10_3{ new AI::Task{ "10.3", AI::State::Success } };
	_ p10_4{ new AI::Task{ "10.4", AI::State::Success } };
	_ p10_5{ new AI::Task{ "10.5", AI::State::Success } };

	AI::Sequence
	{
		{
			p10_1,
			_{ new AI::Selector
				{
					{
						_{ new AI::Sequence
							{
								{
									_{ new AI::Inverter{ p10_2 } },
									_{ new AI::Inverter{ p10_3 } }
								}
							}
						},
						_{ new AI::Sequence
							{
								{
									p10_4,
									p10_5
								}
							}
						}
					}
				}
			}
		}
	}(&log);

	std::string actual = log.str();
	std::string expected = "\
Sequence()\n\
| Task(10.1)\n\
| L Success\n\
| Selector()\n\
| | Sequence()\n\
| | | Inverter()\n\
| | | | Task(10.2)\n\
| | | | L Success\n\
| | | L Failure\n\
| | L Failure\n\
| | Sequence()\n\
| | | Task(10.4)\n\
| | | L Success\n\
| | | Task(10.5)\n\
| | | L Success\n\
| | L Success\n\
| L Success\n\
L Success\n\
";

	std::cout << "Test 10 : ";
	if (actual == expected)
		std::cout << "Pass" << std::endl;
	else
		std::cout << "Failed (" << std::endl << actual << ')' << std::endl;
}
