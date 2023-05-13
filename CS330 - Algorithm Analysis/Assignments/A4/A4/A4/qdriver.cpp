#include <iostream>
#include <fstream>
#include <vector>
#include "q.h"

int main() {
#if defined(DEBUG) | defined(_DEBUG)
    _CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);
#endif

  std::vector<int> nums, nuts, bolts;
  int choice;
  std::cin >> choice;

  switch(choice) {
    case 0:   nums.push_back(54);                // Testing Partition with unique values
              nums.push_back(26);
              nums.push_back(93);
              nums.push_back(17);
              nums.push_back(77);
              nums.push_back(31);
              nums.push_back(44);
              nums.push_back(55);
              nums.push_back(20); 
              std::cout << CS330::divide::partition(nums,0,nums.size()-1) << std::endl;
              CS330::divide::print(nums);              
              break;
    case 1:   nums.push_back(1);                 // Testing Partition with duplicate values
              nums.push_back(57);
              nums.push_back(1);
              nums.push_back(34);
              std::cout << CS330::divide::partition(nums,0,nums.size()-1) << std::endl;
              CS330::divide::print(nums);
              break;
    case 2:   nums.push_back(44);                // Testing find_k with unique values
              nums.push_back(17);
              nums.push_back(20);
              nums.push_back(31);
              nums.push_back(26);
              nums.push_back(54);
              nums.push_back(77);
              nums.push_back(55);
              nums.push_back(93);
              std::cout << CS330::divide::find_k(nums, 0, nums.size()-1, nums.size()/2) << std::endl;
              break;
    case 3:   nums.push_back(10);                // Testing find_k with duplicates values
              nums.push_back(18);
              nums.push_back(16);
              nums.push_back(28);
              nums.push_back(25);
              nums.push_back(32);
              nums.push_back(11);
              nums.push_back(18);
              std::cout << CS330::divide::find_k(nums, 0, nums.size()-1, 5) << std::endl;
              break;
    case 4:  { // load data from file rnd.txt    
              std::ifstream infile("rnd.txt");
              
              if (!infile.is_open()) {
                std::cout << "Could not open the file" << std::endl;
                return EXIT_FAILURE;
              }
              
              int num;
              while(infile >> num) nums.push_back(num);
              infile.close();
              
              // Testing find_k with large sized input
              std::cout << CS330::divide::find_k(nums,0,nums.size()-1,nums.size()/2) << std::endl;              
              break;}
    case 5:   nums.push_back(-10);                // Testing neg_bef_pos
              nums.push_back(-18);
              nums.push_back(16);
              nums.push_back(28);
              nums.push_back(-25);
              nums.push_back(32);
              nums.push_back(-11);
              nums.push_back(18);
              CS330::divide::neg_bef_pos(nums);
              std::cout<<CS330::divide::check_neg_bef_pos(nums)<<std::endl;
              break;    
    case 6:   nuts.push_back(0);                // Testing bolt and nuts match
              nuts.push_back(8);
              nuts.push_back(6);
              nuts.push_back(7);
              nuts.push_back(5);
              nuts.push_back(2);
              nuts.push_back(1);
              nuts.push_back(3);
              nuts.push_back(4);
              nuts.push_back(9);
              bolts.push_back(4);
              bolts.push_back(6);
              bolts.push_back(9);
              bolts.push_back(1);
              bolts.push_back(0);
              bolts.push_back(2);
              bolts.push_back(5);
              bolts.push_back(7);
              bolts.push_back(8);
              bolts.push_back(3);

              CS330::divide::nuts_bolts_match(nuts, bolts, 0, nuts.size()-1);
              CS330::divide::print(nuts); 
              CS330::divide::print(bolts);
              break;
    }
}

