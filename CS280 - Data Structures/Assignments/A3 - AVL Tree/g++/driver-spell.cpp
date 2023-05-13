#include <iostream>
#include <map>
#include <vector>
#include <algorithm>
#include <cstdlib>
#include <ctime>
#include <typeinfo>
#include <sstream>
#include <cstring>
#include <fstream>

#include "BSTree.h"
#include "AVLTree.h"
#include "PRNG.h"
#include "ObjectAllocator.h"

#define SHOW_COUNTSx
const char *gFile = "lexicon.txt";
bool gShowTree = false;

using std::cout;
using std::endl;

//*********************************************************************
// Printing/Debugging
//*********************************************************************
static int Position;
template <typename T>
void SetTreePositions(const typename BSTree<T>::BinTreeNode *tree, 
                      int depth, 
                      std::map<const typename BSTree<T>::BinTreeNode *, 
                      std::pair<int, int> >& NodePositions)
{
  if (!tree)
    return;

  SetTreePositions<T>(tree->left, depth + 1, NodePositions);
  std::pair<int, int>XY(Position++, depth);
  std::pair<const typename BSTree<T>::BinTreeNode *, std::pair<int, int> > pr(tree, XY);
  NodePositions.insert(pr);
  SetTreePositions<T>(tree->right, depth + 1, NodePositions);
}

template <typename T>
void SetTreePositions(const BSTree<T> &tree, 
                      std::map<const typename BSTree<T>::BinTreeNode *, 
                      std::pair<int, int> >& NodePositions)
{
  Position = 0;
  NodePositions.clear();
  SetTreePositions<T>(tree.root(), 0, NodePositions);
}

template <typename T>
std::vector<std::pair<const typename BSTree<T>::BinTreeNode *, int> >
GetNodesAtLevel(int level, 
                std::map<const typename BSTree<T>::BinTreeNode *, 
                std::pair<int, int> >& NodePositions)
{
  std::vector<std::pair<const typename BSTree<T>::BinTreeNode *, int> >nodes;
  typename std::map<const typename BSTree<T>::BinTreeNode *, std::pair<int, int> >::iterator it;
  for (it = NodePositions.begin(); it != NodePositions.end(); ++it)
  {
    std::pair<int, int>XY = it->second;
    if (XY.second == level)
    {
      std::pair<const typename BSTree<T>::BinTreeNode *, int> pr( it->first, XY.first);
      nodes.push_back(pr);
    }
  }
  return nodes;
}  

template <typename T>
bool SortNodes(const std::pair<const typename BSTree<T>::BinTreeNode *, int>&a, 
               const std::pair<const typename BSTree<T>::BinTreeNode *, int>&b)
{
  if (a.second < b.second)
    return true;
  else
    return false;
}

template <typename T>
class fSortNodes
{
public:
  bool operator()(const std::pair<const typename BSTree<T>::BinTreeNode *, int>&a, 
                  const std::pair<const typename BSTree<T>::BinTreeNode *, int>&b)
  {
    if (a.second < b.second)
      return true;
    else
      return false;
  }
};

template <typename T>
void PrintBST(const BSTree<T> &tree)
{
  std::map<const typename BSTree<T>::BinTreeNode *, std::pair<int, int> > NodePositions;

  SetTreePositions(tree, NodePositions);
  int height = tree.height();
  int offset = 0;
  int fudge = 5;
  for (int i = 0; i <= height; i++)
  {
    std::vector<std::pair<const typename BSTree<T>::BinTreeNode *, int> >nodes = GetNodesAtLevel<T> (i, NodePositions);
    // Borland doesn't like the function, wants a function object
    //std::sort(nodes.begin(), nodes.end(), SortNodes<T>);
    std::sort(nodes.begin(), nodes.end(), fSortNodes<T>());
    typename std::vector<std::pair<const typename BSTree<T>::BinTreeNode *, int> >::iterator iter;
    char buffer[1024 * 2] = {0};
    std::memset(buffer, ' ', 1024 * 2);

    for (iter = nodes.begin(); iter != nodes.end(); ++iter)
    {
      //char data[60] = {0};
      T value = (*iter).first->data;
      //std::sprintf(data, "%g", value);  // <<**>>

      std::stringstream ss;
      #ifdef SHOW_COUNTS
      ss << value << "[" << (*iter).first->count << "]";
      #else
      ss << value;
      #endif

      offset = (height / 2) + iter->second * fudge;  // fudge factor (change to 4 for big numbers)
      //strncpy(buffer + offset, data, strlen(data));
      strncpy(buffer + offset, ss.str().c_str(), ss.str().length());
    }
    buffer[offset + fudge * 2] = 0; // should handle 4 digits easily
    std::cout << buffer << std::endl << std::endl;
  }
}

const char *ReadableType(const char *name)
{
  if (std::strstr(name, "AVL"))
    return "AVLTree";
  else
    return "BSTree";
}

template <typename T>
void PrintInfo(const T& tree)
{
  std::cout << "type: " << ReadableType(typeid(T).name()) << ", height: " << tree.height();
  std::cout << ", nodes: " << tree.size() << std::endl;
}

//*********************************************************************
// End Printing/Debugging
//*********************************************************************

int RandomInt(int low, int high)
{
  //return std::rand() % (high - low + 1) + low;
  return Digipen::Utils::Random(low, high);
}

template <typename T>
void SwapInt(T &a, T &b)
{
  T temp = a;
  a = b;
  b = temp;
}

template <typename T> void Shuffle(T *array, int count, int stride = 1)
{
  for (int i = 0; i < count; i += stride)
  {
    int r = RandomInt(0, count - 1);
    SwapInt(array[i], array[r]);
  }
}

void GetValues(int *array, int size)
{
  //std::srand(1);
  Digipen::Utils::srand(1, 2);
  for (int i = 0; i < size; i++)
    array[i] = i;

  Shuffle(array, size);
}

char *mystrupr(char *string)
{
  char *p = string;
  while (*p)
  {
    if (*p >= 'a' && *p <= 'z')
      *p -= 32;
    p++;
  }

  return string;
}

template <typename T>
bool LoadDictionary(T& tree, const char *filename)
{
  std::ifstream infile(filename);
  if (!infile.is_open())
  {
    std::cout << "Can't open file: " << filename << std::endl;
    return false;
  }

  std::string word;
  while (!infile.eof())
  {
    if (std::getline(infile, word).eof())
      break;

    mystrupr(const_cast<char *>(word.c_str()));
    tree.insert(word);
  }
  return true;
}

#include <string>
template <typename T>
void TestSpellCheck1(void)
{
  const char *test = "TestSpellCheck1";
  std::cout << "\n====================== " << test << " ======================\n";

  try
  {
    T tree;

    if (!LoadDictionary(tree, gFile))
      return;

    PrintInfo(tree);
    if (gShowTree)
      PrintBST(tree);

    unsigned compares;
    bool found;
    std::string value = "LIME";
    compares = 0;
    found = tree.find(value, compares);
    if (found)
      std::cout << "Value " << value << " found with " << compares << " compares\n";
    else
      std::cout << "Value " << value << " NOT found with " << compares << " compares\n";

    value = "ORANGE";
    compares = 0;
    found = tree.find(value, compares);
    if (found)
      std::cout << "Value " << value << " found with " << compares << " compares\n";
    else
      std::cout << "Value " << value << " NOT found with " << compares << " compares\n";

    value = "WATERMELON";
    compares = 0;
    found = tree.find(value, compares);
    if (found)
      std::cout << "Value " << value << " found with " << compares << " compares\n";
    else
      std::cout << "Value " << value << " NOT found with " << compares << " compares\n";

    value = "FRUIT";
    compares = 0;
    found = tree.find(value, compares);
    if (found)
      std::cout << "Value " << value << " found with " << compares << " compares\n";
    else
      std::cout << "Value " << value << " NOT found with " << compares << " compares\n";

    //for (unsigned i = 0; i < tree.size(); i++)
      //std::cout << "Index " << i << ": " << tree[i]->data << std::endl;
  }
  catch (const BSTException &e)
  {
    std::cout << "Caught BSTException in: " << test << ": ";
    int value = e.code();
    if (value == BSTException::E_NO_MEMORY)
      std::cout << "E_NO_MEMORY" << std::endl;
    else
      std::cout << "Unknown error code." << std::endl;
  }
  catch(...)
  {
    std::cout << "Unknown exception." << std::endl;
  }
}

template <typename T>
void TestSpellCheck2(void)
{
  const char *test = "TestSpellCheck2";
  std::cout << "\n====================== " << test << " ======================\n";

  try
  {
    T tree;

    if (!LoadDictionary(tree, gFile))
      return;

    PrintInfo(tree);
    //PrintBST(tree);
    std::string words[] = {"FOUR", "SCORE", "AND", "SEVN", "YEARS", "AGO", "AR",
                   "FAWTHERS", "BROUGHT", "FOARTH", "ON", "THIS", "CONTNENT",
                   "A", "GNU", "NASHUN"
                  };

    unsigned compares;
    bool found;
    int num_words = sizeof(words) / sizeof(*words);
    for (int i = 0; i < num_words; i++)
    {
      found = tree.find(words[i], compares);
      if (!found)
        std::cout << words[i] << " is misspelled.\n"; 
    }
  }
  catch (const BSTException &e)
  {
    std::cout << "Caught BSTException in: " << test << ": ";
    int value = e.code();
    if (value == BSTException::E_NO_MEMORY)
      std::cout << "E_NO_MEMORY" << std::endl;
    else
      std::cout << "Unknown error code." << std::endl;
  }
  catch(...)
  {
    std::cout << "Unknown exception." << std::endl;
  }
}

//***********************************************************************
//***********************************************************************
//***********************************************************************

typedef void (*TestFn)(void);

#define WATCHDOG
#if defined(_MSC_VER) && defined(WATCHDOG)

#ifdef _MSC_VER
#include <process.h>
#include <stdlib.h>
#include <windows.h>
#include <time.h>
#endif

static bool Completed = false;

unsigned __stdcall ThreadTest(void *fn)
{
  Completed = false;

  clock_t start, end;
  try
  {
    TestFn testFn = reinterpret_cast<TestFn>(fn);
    start = clock();
    testFn();
    end = clock();
    Completed = true;
  }
  catch(...)
  {
    printf("Unexpected exception thrown in ThreadTest.\n");
  }

  return 0;
}

void ExecuteTest(TestFn fn, int maxwait = 1000, int safewait = 1000)
{
  HANDLE thread;
  unsigned threadID;

  thread = (HANDLE) _beginthreadex(NULL, 0, &ThreadTest, fn, 0, &threadID);

  clock_t start = clock();
  WaitForSingleObject(thread, maxwait);
  clock_t end = clock();
  CloseHandle(thread);

  if (!Completed)
    printf("\n********** Test test took too long to complete (over %i ms). **********\n", maxwait);
  else if (end - start > safewait)
    printf("\n********** Test took %i ms. (Inefficient) **********\n", end - start);
  else 
  {
    //printf("\nTest completed within efficient time limit. (%i ms)\n", safewait);
    //printf("Actual time: %i ms\n", end - start);
  }

  //printf("\n");
}
//***********************************************************************
//***********************************************************************
//***********************************************************************

#else // WATCHDOG

void ExecuteTest(TestFn fn, int maxwait = 1000, int safewait = 1000)
{
  if (maxwait + safewait > 0)
    fn();
}

#endif

int main(int argc, char **argv)
{
    // Test number
  int test_num = 0;
  if (argc > 1)
    test_num = std::atoi(argv[1]);

    // Dictionary
  if (argc > 2)
    gFile = argv[2];

    // Show tree?
  if (argc > 3)
    gShowTree = static_cast<bool>(std::atoi(argv[3]));

  struct TimedTest
  {
    void (*test)(void);
    int maxwait;
    int safewait;
  };

  typedef int T;
  typedef std::string U;
  TimedTest Tests[] = {
                       {TestSpellCheck1<BSTree<U> >,  10000, 5000}, 
                       {TestSpellCheck1<AVLTree<U> >, 10000, 5000}, 
                       {TestSpellCheck2<BSTree<U> >,  10000, 5000}, 
                       {TestSpellCheck2<AVLTree<U> >, 10000, 5000}, 
                      };

  int num = sizeof(Tests) / sizeof(*Tests);
  if (test_num == 0)
  {
    for (int i = 0; i < num; i++)
      ExecuteTest(Tests[i].test, Tests[i].maxwait, Tests[i].safewait);
  }
  else if (test_num > 0 && test_num <= num)
  {
    ExecuteTest(Tests[test_num - 1].test, Tests[test_num - 1].maxwait, Tests[test_num - 1].safewait);
  }

  return 0;
}