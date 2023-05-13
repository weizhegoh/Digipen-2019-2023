#include <iostream>
#include <fstream>
#include <vector>
#include "q.h"

int main() {
#if defined(DEBUG) | defined(_DEBUG)
    _CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);
#endif

  int choice;
  std::cin >> choice;

  switch(choice) {
    case 0: {  // Testing Q1: Charm Bracelet with 3 charms, optimal solution is 22
      int M = 5;
      std::vector<int> W{1,2,3};
      std::vector<int> D{6,10,12};
      std::cout << CS330::dp::charm_bracelet(M,W,D) << std::endl;           
      break;
    }
    case 1: {  // Testing Q1: Charm Bracelet with 4 charms, optimal solution is 23
      int M = 6;
      std::vector<int> W{1,2,3,2};
      std::vector<int> D{4,6,12,7};
      std::cout << CS330::dp::charm_bracelet(M,W,D) << std::endl;           
      break;
    }
    case 2: {  // Testing Q1: Charm Bracelet with 5 charms, optiaml solution is 51
      int M = 26;
      std::vector<int> W{12,7,11,8,9};
      std::vector<int> D{24,13,23,15,16};
      std::cout << CS330::dp::charm_bracelet(M,W,D) << std::endl;           
      break;
    }
    case 3: {  // Testing Q1: Charm Bracelet with 15 charms, optimal solution is 1458
      int M = 750;
    std::vector<int> W{70,73,77,80,82,87,90,94,98,106,110,113,115,118,120};
      std::vector<int> D{135,139,149,150,156,163,173,184,192,201,210,214,221,229,240};
      std::cout << CS330::dp::charm_bracelet(M,W,D) << std::endl;           
      break;
    }
    case 4:  {  // Testing Q1: Charm Bracelet with 24 charms, optimal solution is 13549094
      int M = 6404180;
    std::vector<int> W{382745,799601,909247,729069,467902,44328,34610,698150,823460,
                       903959,853665,551830,610856,670702,488960,951111,323046,446298,
                       931161,31385,496951,264724,224916,169684};
      std::vector<int> D{825594,1677009,1676628,1523970,943972,97426,69666,1296457,
                         1679693,1902996,1844992,1049289,1252836,1319836,953277,2067538,
                         675367,853655,1826027,65731,901489,577243,466257,369261};
      std::cout << CS330::dp::charm_bracelet(M,W,D) << std::endl;           
      break;
    }
    case 5:   {  // Testing Q2: coin change, optimal solution is 4
      int changes = 29;
      std::vector<int> denominations{1,4,5,8,11};
      std::cout << CS330::dp::coin_changes(changes,denominations) << std::endl;           
      break;
    }
    case 6:   {  // Testing Q2: coin change, optimal solution is 2
                 // The greedy algorithm will not get the minimal solution.
      int changes = 96;
      std::vector<int> denominations{1,2,6,12,24,48,60};
      std::cout << CS330::dp::coin_changes(changes,denominations) << std::endl;           
      break;
    }
    case 7:   {  // Testing Q3: tree tower, optimal solution is 59
      int weight[5][5] = {{9, 0, 0, 0, 0}, 
                   {12,15,0, 0, 0},
                   {10,6, 8, 0, 0},
                   {2,18, 9, 5, 0},
                   {19,7, 10,4,16}
                  };
      std::cout << CS330::dp::tree_tower(5,5,&weight[0][0]) << std::endl;           
      break;
    }
  }
}

