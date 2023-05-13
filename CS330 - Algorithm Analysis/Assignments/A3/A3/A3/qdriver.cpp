#include <iostream>
#include <vector>
#include "q.h"


int main() {
#if defined(DEBUG) | defined(_DEBUG)
    _CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);
#endif

  std::vector<int> set;
  std::vector<int> subset;
  int choice;
  std::cin >> choice;

  switch(choice) {
    case 0: 	set.push_back(3);
              set.push_back(5);
              set.push_back(6);
              set.push_back(7);
              subset = CS330::subsetsum::subset_sum(set, 15);
              break;
    case 1: 	set.push_back(3);
              set.push_back(5);
              set.push_back(6);
              set.push_back(7);
              subset = CS330::subsetsum::subset_sum(set, 17);
              break;
    case 2: 	set.push_back(7);
              set.push_back(3);
              set.push_back(2);
              set.push_back(5);
              set.push_back(8);
              subset = CS330::subsetsum::subset_sum(set, 14);
              break;  
  }

  if (subset.size()==0)
    std::cout << "not found!";
  else
    for (unsigned long i = 0; i < subset.size(); ++i) { std::cout << subset[i] << " "; }
  std::cout << std::endl;
}

