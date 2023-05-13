#include <iostream>
#include <iomanip>
#include "BList.h"
#include "PRNG.h"

#include <algorithm>

int RandomInt(int low, int high)
{
  return Digipen::Utils::Random(low, high);
}

template <typename T>
void SwapT(T &a, T &b)
{
  T temp = a;
  a = b;
  b = temp;
}

template <typename T>
void Shuffle(T *array, unsigned count)
{
  for (unsigned int i = 0; i < count; i++)
  {
    int r = RandomInt(i, static_cast<int>(count) - 1);
    SwapT(array[i], array[r]);
  }
}

template <typename T, unsigned Size>
void DumpStats(const BList<T, Size>& blist)
{
  BListStats stats = blist.GetStats();

  size_t nodes = stats.NodeCount;
  size_t items = stats.ItemCount;
  size_t asize = stats.ArraySize;

  double ipn;
  if (nodes)
    ipn = static_cast<double>(items) / static_cast<double>(nodes);
  else
    ipn = 0;

  double util = ipn / static_cast<double>(asize) * 100;

  std::cout << "Asize: " << asize << std::endl;
  std::cout << "Items: " << items << std::endl;
  std::cout << "Nodes: " << nodes << std::endl;
  std::cout << "Average items per node: " << ipn << std::endl;
  std::cout << "Node utilization: " << std::setprecision(3) << util << "%" << std::endl;
  std::cout << std::endl;
}


template <typename T, unsigned Size>
void DumpList(const BList<T, Size>& blist, bool flat = false)
{
  const typename BList<T, Size>::BNode *node = blist.GetHead();
  unsigned count = 0;
  if (flat)
    std::cout << "List: ";
  while (node)
  {
    if (!flat)
      std::cout << "Node " << std::setw(3) << ++count << " (" << std::setw(2) << node->count << "): ";

    for (unsigned i = 0; i < node->count; i++)
    {
      std::cout << node->values[i] << " ";
    }

    if (!flat)
      std::cout << std::endl;
    node = node->next;
  }
  std::cout << std::endl;
}

template <typename T, unsigned Size>
void DumpListFlat(const BList<T, Size>& blist)
{
  const typename BList<T, Size>::BNode *node = blist.GetHead();
  while (node)
  {
    for (unsigned i = 0; i < node->count; i++)
    {
      std::cout << node->values[i] << " ";
    }
    node = node->next;
  }
}

///////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////
// push_front
void test1_1()
{
  std::cout << "==================== test1_1 ====================\n";
  const unsigned asize = 1;

  BList<int, asize> bl;
  std::cout << "nodesize is " << bl.nodesize() << std::endl;
  for (int i = 1; i <= 16; i++)
  {
    bl.push_front(i);
    //DumpList(bl);
    //std::cout << "==========================\n";
  }
  DumpList(bl, false);
  DumpList(bl, true);
  DumpStats(bl);
  std::cout << std::endl;
}

void test1_2()
{
  std::cout << "==================== test1_2 ====================\n";
  const unsigned asize = 2;

  BList<int, asize> bl;
  std::cout << "nodesize is " << bl.nodesize() << std::endl;
  for (int i = 1; i <= 16; i++)
  {
    bl.push_front(i);
    //DumpList(bl);
    //std::cout << "==========================\n";
  }
  DumpList(bl, false);
  DumpList(bl, true);
  DumpStats(bl);
  std::cout << std::endl;
}

void test1_4()
{
  std::cout << "==================== test1_4 ====================\n";
  const unsigned asize = 4;

  BList<int, asize> bl;
  std::cout << "nodesize is " << bl.nodesize() << std::endl;
  for (int i = 1; i <= 16; i++)
  {
    bl.push_front(i);
    //DumpList(bl);
    //std::cout << "==========================\n";
  }
  DumpList(bl, false);
  DumpList(bl, true);
  DumpStats(bl);
  std::cout << std::endl;
}

void test1_8()
{
  std::cout << "==================== test1_8 ====================\n";
  const unsigned asize = 8;

  BList<int, asize> bl;
  std::cout << "nodesize is " << bl.nodesize() << std::endl;
  for (int i = 1; i <= 16; i++)
  {
    bl.push_front(i);
    //DumpList(bl);
    //std::cout << "==========================\n";
  }
  DumpList(bl, false);
  DumpList(bl, true);
  DumpStats(bl);
  std::cout << std::endl;
}

void test1_16()
{
  std::cout << "==================== test1_16 ====================\n";
  const unsigned asize = 16;

  BList<int, asize> bl;
  std::cout << "nodesize is " << bl.nodesize() << std::endl;
  for (int i = 1; i <= 16; i++)
  {
    bl.push_front(i);
    //DumpList(bl);
    //std::cout << "==========================\n";
  }
  DumpList(bl, false);
  DumpList(bl, true);
  DumpStats(bl);
  std::cout << std::endl;
}

///////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////
// push_back
void test2_1()
{
  std::cout << "==================== test2_1 ====================\n";
  const unsigned asize = 1;

  BList<int, asize> bl;
  std::cout << "nodesize is " << bl.nodesize() << std::endl;
  for (int i = 1; i <= 16; i++)
  {
    bl.push_back(i);
    //DumpList(bl);
    //std::cout << "==========================\n";
  }
  DumpList(bl, false);
  DumpList(bl, true);
  DumpStats(bl);
  std::cout << std::endl;
}

void test2_2()
{
  std::cout << "==================== test2_2 ====================\n";
  const unsigned asize = 2;

  BList<int, asize> bl;
  std::cout << "nodesize is " << bl.nodesize() << std::endl;
  for (int i = 1; i <= 16; i++)
  {
    bl.push_back(i);
    //DumpList(bl);
    //std::cout << "==========================\n";
  }
  DumpList(bl, false);
  DumpList(bl, true);
  DumpStats(bl);
  std::cout << std::endl;
}

void test2_4()
{
  std::cout << "==================== test2_4 ====================\n";
  const unsigned asize = 4;

  BList<int, asize> bl;
  std::cout << "nodesize is " << bl.nodesize() << std::endl;
  for (int i = 1; i <= 16; i++)
  {
    bl.push_back(i);
    //DumpList(bl);
    //std::cout << "==========================\n";
  }
  DumpList(bl, false);
  DumpList(bl, true);
  DumpStats(bl);
  std::cout << std::endl;
}

void test2_8()
{
  std::cout << "==================== test2_8 ====================\n";
  const unsigned asize = 8;

  BList<int, asize> bl;
  std::cout << "nodesize is " << bl.nodesize() << std::endl;
  for (int i = 1; i <= 16; i++)
  {
    bl.push_back(i);
    //DumpList(bl);
    //std::cout << "==========================\n";
  }
  DumpList(bl, false);
  DumpList(bl, true);
  DumpStats(bl);
  std::cout << std::endl;
}

void test2_16()
{
  std::cout << "==================== test2_16 ====================\n";
  const unsigned asize = 16;

  BList<int, asize> bl;
  std::cout << "nodesize is " << bl.nodesize() << std::endl;
  for (int i = 1; i <= 16; i++)
  {
    bl.push_back(i);
    //DumpList(bl);
    //std::cout << "==========================\n";
  }
  DumpList(bl, false);
  DumpList(bl, true);
  DumpStats(bl);
  std::cout << std::endl;
}


///////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////
// insert
void test3_1()
{
  std::cout << "==================== test3_1 ====================\n";
  const unsigned asize = 1;

  Digipen::Utils::srand(2, 1);
  BList<int, asize> bl;
  std::cout << "nodesize is " << bl.nodesize() << std::endl;
  unsigned low = 10;
  unsigned high = 99;
  unsigned range = high - low;
  unsigned size = 16;
  int *ia = new int[range];
  for (unsigned i = low; i < high; i++)
    ia[i - low] = i + 1;

  Shuffle(ia, range);
  for (unsigned i = 0; i < size; i++) 
  {
    bl.insert(ia[i]);
    //DumpList(bl, true);
    //DumpList(bl);
    //std::cout << "==========================\n";
  }
  DumpList(bl, false);
  DumpList(bl, true);
  DumpStats(bl);
  std::cout << std::endl;
  delete [] ia;
}

void test3_2()
{
  std::cout << "==================== test3_2 ====================\n";
  const unsigned asize = 2;

  Digipen::Utils::srand(2, 1);
  BList<int, asize> bl;
  std::cout << "nodesize is " << bl.nodesize() << std::endl;
  unsigned low = 10;
  unsigned high = 99;
  unsigned range = high - low;
  unsigned size = 16;
  int *ia = new int[range];
  for (unsigned i = low; i < high; i++)
    ia[i - low] = i + 1;

  Shuffle(ia, range);
  for (unsigned i = 0; i < size; i++) 
  {
    bl.insert(ia[i]);
    //DumpList(bl, true);
    //DumpList(bl);
    //std::cout << "==========================\n";
  }
  DumpList(bl, false);
  DumpList(bl, true);
  DumpStats(bl);
  std::cout << std::endl;
  delete [] ia;
}

void test3_4()
{
  std::cout << "==================== test3_4 ====================\n";
  const unsigned asize = 4;

  Digipen::Utils::srand(2, 1);
  BList<int, asize> bl;
  std::cout << "nodesize is " << bl.nodesize() << std::endl;
  unsigned low = 10;
  unsigned high = 99;
  unsigned range = high - low;
  unsigned size = 16;
  int *ia = new int[range];
  for (unsigned i = low; i < high; i++)
    ia[i - low] = i + 1;

  Shuffle(ia, range);
  for (unsigned i = 0; i < size; i++) 
  {
    bl.insert(ia[i]);
    //DumpList(bl, true);
    //DumpList(bl);
    //std::cout << "==========================\n";
  }
  DumpList(bl, false);
  DumpList(bl, true);
  DumpStats(bl);
  std::cout << std::endl;
  delete [] ia;
}

void test3_8()
{
  std::cout << "==================== test3_8 ====================\n";
  const unsigned asize = 8;

  Digipen::Utils::srand(2, 1);
  BList<int, asize> bl;
  std::cout << "nodesize is " << bl.nodesize() << std::endl;
  unsigned low = 10;
  unsigned high = 99;
  unsigned range = high - low;
  unsigned size = 16;
  int *ia = new int[range];
  for (unsigned i = low; i < high; i++)
    ia[i - low] = i + 1;

  Shuffle(ia, range);
  for (unsigned i = 0; i < size; i++) 
  {
    bl.insert(ia[i]);
    //DumpList(bl, true);
    //DumpList(bl);
    //std::cout << "==========================\n";
  }
  DumpList(bl, false);
  DumpList(bl, true);
  DumpStats(bl);
  std::cout << std::endl;
  delete [] ia;
}

void test3_16()
{
  std::cout << "==================== test3_16 ====================\n";
  const unsigned asize = 16;

  Digipen::Utils::srand(2, 1);
  BList<int, asize> bl;
  std::cout << "nodesize is " << bl.nodesize() << std::endl;
  unsigned low = 10;
  unsigned high = 99;
  unsigned range = high - low;
  unsigned size = 16;
  int *ia = new int[range];
  for (unsigned i = low; i < high; i++)
    ia[i - low] = i + 1;

  Shuffle(ia, range);
  for (unsigned i = 0; i < size; i++) 
  {
    bl.insert(ia[i]);
    //DumpList(bl, true);
    //DumpList(bl);
    //std::cout << "==========================\n";
  }
  DumpList(bl, false);
  DumpList(bl, true);
  DumpStats(bl);
  std::cout << std::endl;
  delete [] ia;
}


///////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////
// subscript

template <typename T, unsigned Size>
void print_subscript(const BList<T, Size>& list)
{
  for (unsigned i = 0; i < list.size(); i++)  
  {
    std::cout << list[i] << " ";
  }
}

void test4_1()
{
  std::cout << "==================== test4_1 ====================\n";
  const unsigned asize = 1;

  Digipen::Utils::srand(2, 1);
  BList<int, asize> bl;
  std::cout << "nodesize is " << bl.nodesize() << std::endl;
  unsigned low = 10;
  unsigned high = 99;
  unsigned range = high - low;
  unsigned size = 16;
  int *ia = new int[range];
  for (unsigned i = low; i < high; i++)
    ia[i - low] = i + 1;

  Shuffle(ia, range);
  for (unsigned i = 0; i < size; i++) 
  {
    bl.insert(ia[i]);
    //DumpList(bl, true);
    //DumpList(bl);
    //std::cout << "==========================\n";
  }
  DumpList(bl, false);

  print_subscript(bl);
  std::cout << std::endl << std::endl;

  DumpList(bl, false);
  DumpList(bl, true);
  DumpStats(bl);
  std::cout << std::endl;
  delete [] ia;
}

void test4_2()
{
  std::cout << "==================== test4_2 ====================\n";
  const unsigned asize = 2;

  Digipen::Utils::srand(2, 1);
  BList<int, asize> bl;
  std::cout << "nodesize is " << bl.nodesize() << std::endl;
  unsigned low = 10;
  unsigned high = 99;
  unsigned range = high - low;
  unsigned size = 16;
  int *ia = new int[range];
  for (unsigned i = low; i < high; i++)
    ia[i - low] = i + 1;

  Shuffle(ia, range);
  for (unsigned i = 0; i < size; i++) 
  {
    bl.insert(ia[i]);
    //DumpList(bl, true);
    //DumpList(bl);
    //std::cout << "==========================\n";
  }
  DumpList(bl, false);

  for (unsigned i = 0; i < size; i++) 
  {
    std::cout << bl[i] << " ";
  }
  std::cout << std::endl << std::endl;

  DumpList(bl, false);
  DumpList(bl, true);
  DumpStats(bl);
  std::cout << std::endl;
  delete [] ia;
}

void test4_4()
{
  std::cout << "==================== test4_4 ====================\n";
  const unsigned asize = 4;

  Digipen::Utils::srand(2, 1);
  BList<int, asize> bl;
  std::cout << "nodesize is " << bl.nodesize() << std::endl;
  unsigned low = 10;
  unsigned high = 99;
  unsigned range = high - low;
  unsigned size = 16;
  int *ia = new int[range];
  for (unsigned i = low; i < high; i++)
    ia[i - low] = i + 1;

  Shuffle(ia, range);
  for (unsigned i = 0; i < size; i++) 
  {
    bl.insert(ia[i]);
    //DumpList(bl, true);
    //DumpList(bl);
    //std::cout << "==========================\n";
  }
  DumpList(bl, false);

  for (unsigned i = 0; i < size; i++) 
  {
    std::cout << bl[i] << " ";
  }
  std::cout << std::endl << std::endl;

  DumpList(bl, false);
  DumpList(bl, true);
  DumpStats(bl);
  std::cout << std::endl;
  delete [] ia;
}

void test4_8()
{
  std::cout << "==================== test4_8 ====================\n";
  const unsigned asize = 8;

  Digipen::Utils::srand(2, 1);
  BList<int, asize> bl;
  std::cout << "nodesize is " << bl.nodesize() << std::endl;
  unsigned low = 10;
  unsigned high = 99;
  unsigned range = high - low;
  unsigned size = 16;
  int *ia = new int[range];
  for (unsigned i = low; i < high; i++)
    ia[i - low] = i + 1;

  Shuffle(ia, range);
  for (unsigned i = 0; i < size; i++) 
  {
    bl.insert(ia[i]);
    //DumpList(bl, true);
    //DumpList(bl);
    //std::cout << "==========================\n";
  }
  DumpList(bl, false);

  for (unsigned i = 0; i < size; i++) 
  {
    std::cout << bl[i] << " ";
  }
  std::cout << std::endl << std::endl;

  DumpList(bl, false);
  DumpList(bl, true);
  DumpStats(bl);
  std::cout << std::endl;
  delete [] ia;
}

void test4_16()
{
  std::cout << "==================== test4_16 ====================\n";
  const unsigned asize = 1;

  Digipen::Utils::srand(2, 1);
  BList<int, asize> bl;
  std::cout << "nodesize is " << bl.nodesize() << std::endl;
  unsigned low = 10;
  unsigned high = 99;
  unsigned range = high - low;
  unsigned size = 16;
  int *ia = new int[range];
  for (unsigned i = low; i < high; i++)
    ia[i - low] = i + 1;

  Shuffle(ia, range);
  for (unsigned i = 0; i < size; i++) 
  {
    bl.insert(ia[i]);
    //DumpList(bl, true);
    //DumpList(bl);
    //std::cout << "==========================\n";
  }
  DumpList(bl, false);

  for (unsigned i = 0; i < size; i++) 
  {
    std::cout << bl[i] << " ";
  }
  std::cout << std::endl << std::endl;

  DumpList(bl, false);
  DumpList(bl, true);
  DumpStats(bl);
  std::cout << std::endl;
  delete [] ia;
}

///////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////
// push_back/find
void test5_1()
{
  std::cout << "==================== test5_1 ====================\n";
  const unsigned asize = 1;

  BList<int, asize> bl;
  std::cout << "nodesize is " << bl.nodesize() << std::endl;
  unsigned low = 10;
  unsigned high = 99;
  unsigned range = high - low;
  unsigned size = 16;
  int *ia = new int[range];
  for (unsigned i = low; i < high; i++)
    ia[i - low] = i + 1;

  Shuffle(ia, range);

  for (unsigned i = 0; i < size; i++)
    bl.push_back(ia[i]);

  DumpList(bl, true);
  DumpList(bl, false);
  for (unsigned i = 0; i < size; i++)
  {
    int index = bl.find(ia[i]);
    if (index == -1)
      std::cout << "Item: " << ia[i] << " not found. Something's wrong.\n";
    else
      std::cout << "Found " << ia[i] << " at index " << index << std::endl;
  }
  std::cout << std::endl;
  DumpStats(bl);
  std::cout << std::endl;

  delete [] ia;
}

void test5_2()
{
  std::cout << "==================== test5_2 ====================\n";
  const unsigned asize = 2;

  BList<int, asize> bl;
  std::cout << "nodesize is " << bl.nodesize() << std::endl;
  unsigned low = 10;
  unsigned high = 99;
  unsigned range = high - low;
  unsigned size = 16;
  int *ia = new int[range];
  for (unsigned i = low; i < high; i++)
    ia[i - low] = i + 1;

  Shuffle(ia, range);

  for (unsigned i = 0; i < size; i++)
    bl.push_back(ia[i]);

  DumpList(bl, true);
  DumpList(bl, false);
  for (unsigned i = 0; i < size; i++)
  {
    int index = bl.find(ia[i]);
    if (index == -1)
      std::cout << "Item: " << ia[i] << " not found. Something's wrong.\n";
    else
      std::cout << "Found " << ia[i] << " at index " << index << std::endl;
  }
  std::cout << std::endl;
  DumpStats(bl);
  std::cout << std::endl;

  delete [] ia;
}

void test5_4()
{
  std::cout << "==================== test5_4 ====================\n";
  const unsigned asize = 4;

  BList<int, asize> bl;
  std::cout << "nodesize is " << bl.nodesize() << std::endl;
  unsigned low = 10;
  unsigned high = 99;
  unsigned range = high - low;
  unsigned size = 16;
  int *ia = new int[range];
  for (unsigned i = low; i < high; i++)
    ia[i - low] = i + 1;

  Shuffle(ia, range);

  for (unsigned i = 0; i < size; i++)
    bl.push_back(ia[i]);

  DumpList(bl, true);
  DumpList(bl, false);
  for (unsigned i = 0; i < size; i++)
  {
    int index = bl.find(ia[i]);
    if (index == -1)
      std::cout << "Item: " << ia[i] << " not found. Something's wrong.\n";
    else
      std::cout << "Found " << ia[i] << " at index " << index << std::endl;
  }
  std::cout << std::endl;
  DumpStats(bl);
  std::cout << std::endl;

  delete [] ia;
}

void test5_8()
{
  std::cout << "==================== test5_8 ====================\n";
  const unsigned asize = 8;

  BList<int, asize> bl;
  std::cout << "nodesize is " << bl.nodesize() << std::endl;
  unsigned low = 10;
  unsigned high = 99;
  unsigned range = high - low;
  unsigned size = 16;
  int *ia = new int[range];
  for (unsigned i = low; i < high; i++)
    ia[i - low] = i + 1;

  Shuffle(ia, range);

  for (unsigned i = 0; i < size; i++)
    bl.push_back(ia[i]);

  DumpList(bl, true);
  DumpList(bl, false);
  for (unsigned i = 0; i < size; i++)
  {
    int index = bl.find(ia[i]);
    if (index == -1)
      std::cout << "Item: " << ia[i] << " not found. Something's wrong.\n";
    else
      std::cout << "Found " << ia[i] << " at index " << index << std::endl;
  }
  std::cout << std::endl;
  DumpStats(bl);
  std::cout << std::endl;

  delete [] ia;
}

void test5_16()
{
  std::cout << "==================== test5_16 ====================\n";
  const unsigned asize = 16;

  BList<int, asize> bl;
  std::cout << "nodesize is " << bl.nodesize() << std::endl;
  unsigned low = 10;
  unsigned high = 99;
  unsigned range = high - low;
  unsigned size = 16;
  int *ia = new int[range];
  for (unsigned i = low; i < high; i++)
    ia[i - low] = i + 1;

  Shuffle(ia, range);

  for (unsigned i = 0; i < size; i++)
    bl.push_back(ia[i]);

  DumpList(bl, true);
  DumpList(bl, false);
  for (unsigned i = 0; i < size; i++)
  {
    int index = bl.find(ia[i]);
    if (index == -1)
      std::cout << "Item: " << ia[i] << " not found. Something's wrong.\n";
    else
      std::cout << "Found " << ia[i] << " at index " << index << std::endl;
  }
  std::cout << std::endl;
  DumpStats(bl);
  std::cout << std::endl;

  delete [] ia;
}


///////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////
// insert/find
void test6_16()
{
  std::cout << "==================== test6_16 ====================\n";
  const unsigned asize = 16;

  BList<int, asize> bl;
  std::cout << "nodesize is " << bl.nodesize() << std::endl;
  unsigned low = 100;
  unsigned high = 999;
  unsigned range = high - low;
  unsigned size = 300;
  int *ia = new int[range];
  for (unsigned i = low; i < high; i++)
    ia[i - low] = i + 1;

  Shuffle(ia, range);

  for (unsigned i = 0; i < size; i++)
    bl.insert(ia[i]);

  //DumpList(bl, true);
  DumpList(bl, false);
  for (unsigned i = 0; i < size; i++)
  {
    int index = bl.find(ia[i]);
    if (index == -1)
      std::cout << "Item: " << ia[i] << " not found. Something's wrong.\n";
    else
      if (!(i % 20))
        std::cout << "Found " << ia[i] << " at index " << index << std::endl;
  }
  std::cout << std::endl;
  DumpStats(bl);
  std::cout << std::endl;

  delete [] ia;
}

void test6_64()
{
  std::cout << "==================== test6_64 ====================\n";
  const unsigned asize = 64;

  BList<int, asize> bl;
  std::cout << "nodesize is " << bl.nodesize() << std::endl;
  unsigned low = 100;
  unsigned high = 999;
  unsigned range = high - low;
  unsigned size = 300;
  int *ia = new int[range];
  for (unsigned i = low; i < high; i++)
    ia[i - low] = i + 1;

  Shuffle(ia, range);

  for (unsigned i = 0; i < size; i++)
    bl.insert(ia[i]);

  //DumpList(bl, true);
  DumpList(bl, false);
  for (unsigned i = 0; i < size; i++)
  {
    int index = bl.find(ia[i]);
    if (index == -1)
      std::cout << "Item: " << ia[i] << " not found. Something's wrong.\n";
    else
      if (!(i % 20))
        std::cout << "Found " << ia[i] << " at index " << index << std::endl;
  }
  std::cout << std::endl;
  DumpStats(bl);
  std::cout << std::endl;

  delete [] ia;
}

void test6_512()
{
  std::cout << "==================== test6_512 ====================\n";
  const unsigned asize = 512;

  BList<int, asize> bl;
  std::cout << "nodesize is " << bl.nodesize() << std::endl;
  unsigned low = 1000;
  unsigned high = 9999;
  unsigned range = high - low;
  unsigned size = 8000;
  int *ia = new int[range];
  for (unsigned i = low; i < high; i++)
    ia[i - low] = i + 1;

  Shuffle(ia, range);

  for (unsigned i = 0; i < size; i++)
    bl.insert(ia[i]);

  //DumpList(bl, true);
  //DumpList(bl, false);
  for (unsigned i = 0; i < size; i++)
  {
    int index = bl.find(ia[i]);
    if (index == -1)
      std::cout << "Item: " << ia[i] << " not found. Something's wrong.\n";
    else
      if (!(i % 400))
        std::cout << "Found " << ia[i] << " at index " << index << std::endl;
  }
  std::cout << std::endl;
  DumpStats(bl);
  std::cout << std::endl;

  delete [] ia;
}

///////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////
// remove by index
void test7_1()
{
  std::cout << "==================== test7_1 ====================\n";
  const unsigned asize = 1;

  Digipen::Utils::srand(2, 1);
  BList<int, asize> bl;
  std::cout << "nodesize is " << bl.nodesize() << std::endl;
  unsigned low = 10;
  unsigned high = 99;
  unsigned range = high - low;
  unsigned size = 16;
  int *ia = new int[range];
  for (unsigned i = low; i < high; i++)
    ia[i - low] = i + 1;

  Shuffle(ia, range);
  for (unsigned i = 0; i < size; i++) 
    bl.push_back(ia[i]);

  DumpList(bl, false);
  //DumpStats(bl);

  for (int i = size - 1; i >= 0; i -= 2)
  {
    std::cout << "Removing from index " << i << std::endl;
    bl.remove(i);
    DumpList(bl, true);
  }
  
  DumpList(bl, false);
  //DumpStats(bl);
  delete [] ia;
}

void test7_2()
{
  std::cout << "==================== test7_2 ====================\n";
  const unsigned asize = 2;

  Digipen::Utils::srand(2, 1);
  BList<int, asize> bl;
  std::cout << "nodesize is " << bl.nodesize() << std::endl;
  unsigned low = 10;
  unsigned high = 99;
  unsigned range = high - low;
  unsigned size = 16;
  int *ia = new int[range];
  for (unsigned i = low; i < high; i++)
    ia[i - low] = i + 1;

  Shuffle(ia, range);
  for (unsigned i = 0; i < size; i++) 
    bl.push_back(ia[i]);

  DumpList(bl, false);
  //DumpStats(bl);

  for (int i = size - 1; i >= 0; i -= 2)
  {
    std::cout << "Removing from index " << i << std::endl;
    bl.remove(i);
    DumpList(bl, true);
  }
  
  DumpList(bl, false);
  //DumpStats(bl);
  delete [] ia;
}

void test7_4()
{
  std::cout << "==================== test7_4 ====================\n";
  const unsigned asize = 4;

  Digipen::Utils::srand(2, 1);
  BList<int, asize> bl;
  std::cout << "nodesize is " << bl.nodesize() << std::endl;
  unsigned low = 10;
  unsigned high = 99;
  unsigned range = high - low;
  unsigned size = 16;
  int *ia = new int[range];
  for (unsigned i = low; i < high; i++)
    ia[i - low] = i + 1;

  Shuffle(ia, range);
  for (unsigned i = 0; i < size; i++) 
    bl.push_back(ia[i]);

  DumpList(bl, false);
  //DumpStats(bl);

  for (int i = size - 1; i >= 0; i -= 2)
  {
    std::cout << "Removing from index " << i << std::endl;
    bl.remove(i);
    DumpList(bl, true);
  }
  
  DumpList(bl, false);
  //DumpStats(bl);
  delete [] ia;
}

void test7_8()
{
  std::cout << "==================== test7_8 ====================\n";
  const unsigned asize = 8;

  Digipen::Utils::srand(2, 1);
  BList<int, asize> bl;
  std::cout << "nodesize is " << bl.nodesize() << std::endl;
  unsigned low = 10;
  unsigned high = 99;
  unsigned range = high - low;
  unsigned size = 16;
  int *ia = new int[range];
  for (unsigned i = low; i < high; i++)
    ia[i - low] = i + 1;

  Shuffle(ia, range);
  for (unsigned i = 0; i < size; i++) 
    bl.push_back(ia[i]);

  DumpList(bl, false);
  //DumpStats(bl);

  for (int i = size - 1; i >= 0; i -= 2)
  {
    std::cout << "Removing from index " << i << std::endl;
    bl.remove(i);
    DumpList(bl, true);
  }
  
  DumpList(bl, false);
  //DumpStats(bl);
  delete [] ia;
}



///////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////
// remove by value (first occurrence)
void test8_1()
{
  std::cout << "==================== test8_1 ====================\n";
  const unsigned asize = 1;

  Digipen::Utils::srand(2, 1);
  BList<int, asize> bl;
  std::cout << "nodesize is " << bl.nodesize() << std::endl;
  unsigned low = 10;
  unsigned high = 99;
  unsigned range = high - low;
  unsigned size = 16;
  int *ia = new int[range];
  for (unsigned i = low; i < high; i++)
    ia[i - low] = i + 1;

  Shuffle(ia, range);

  for (unsigned i = 0; i < size; i++) 
    bl.push_back(ia[i]);

  DumpList(bl, false);
  std::sort(ia, ia + size);

  for (unsigned i = 0; i < size - 4; i++)
  {
    std::cout << "Removing value " << ia[i] << std::endl;
    bl.remove_by_value(ia[i]);
  }
  DumpList(bl, false);
  delete [] ia;
}

void test8_2()
{
  std::cout << "==================== test8_2 ====================\n";
  const unsigned asize = 2;

  Digipen::Utils::srand(2, 1);
  BList<int, asize> bl;
  std::cout << "nodesize is " << bl.nodesize() << std::endl;
  unsigned low = 10;
  unsigned high = 99;
  unsigned range = high - low;
  unsigned size = 16;
  int *ia = new int[range];
  for (unsigned i = low; i < high; i++)
    ia[i - low] = i + 1;

  Shuffle(ia, range);

  for (unsigned i = 0; i < size; i++) 
    bl.push_back(ia[i]);

  DumpList(bl, false);
  std::sort(ia, ia + size);

  for (unsigned i = 0; i < size - 4; i++)
  {
    std::cout << "Removing value " << ia[i] << std::endl;
    bl.remove_by_value(ia[i]);
  }
  DumpList(bl, false);
  delete [] ia;
}

void test8_4()
{
  std::cout << "==================== test8_4 ====================\n";
  const unsigned asize = 4;

  Digipen::Utils::srand(2, 1);
  BList<int, asize> bl;
  std::cout << "nodesize is " << bl.nodesize() << std::endl;
  unsigned low = 10;
  unsigned high = 99;
  unsigned range = high - low;
  unsigned size = 16;
  int *ia = new int[range];
  for (unsigned i = low; i < high; i++)
    ia[i - low] = i + 1;

  Shuffle(ia, range);

  for (unsigned i = 0; i < size; i++) 
    bl.push_back(ia[i]);

  DumpList(bl, false);
  std::sort(ia, ia + size);

  for (unsigned i = 0; i < size - 4; i++)
  {
    std::cout << "Removing value " << ia[i] << std::endl;
    bl.remove_by_value(ia[i]);
  }
  DumpList(bl, false);
  delete [] ia;
}

void test8_8()
{
  std::cout << "==================== test8_8 ====================\n";
  const unsigned asize = 8;

  Digipen::Utils::srand(2, 1);
  BList<int, asize> bl;
  std::cout << "nodesize is " << bl.nodesize() << std::endl;
  unsigned low = 10;
  unsigned high = 99;
  unsigned range = high - low;
  unsigned size = 16;
  int *ia = new int[range];
  for (unsigned i = low; i < high; i++)
    ia[i - low] = i + 1;

  Shuffle(ia, range);

  for (unsigned i = 0; i < size; i++) 
    bl.push_back(ia[i]);

  DumpList(bl, false);
  std::sort(ia, ia + size);

  for (unsigned i = 0; i < size - 4; i++)
  {
    std::cout << "Removing value " << ia[i] << std::endl;
    bl.remove_by_value(ia[i]);
  }
  DumpList(bl, false);
  delete [] ia;
}


void test9_1()
{
  std::cout << "==================== test9_1 ====================\n";
  const unsigned asize = 1;

  Digipen::Utils::srand(2, 1);
  BList<int, asize> bl;
  std::cout << "nodesize is " << bl.nodesize() << std::endl;
  unsigned low = 10;
  unsigned high = 99;
  unsigned range = high - low;
  unsigned size = 16;
  int *ia = new int[range];
  for (unsigned i = low; i < high; i++)
    ia[i - low] = i + 1;

  Shuffle(ia, range);

  for (unsigned i = 0; i < size; i++) 
    bl.insert(ia[i]);

  DumpList(bl, false);
  DumpList(bl, true);
  DumpStats(bl);

    // copy construction
  BList<int, asize> bl2(bl);

  DumpList(bl2, false);
  DumpList(bl, true);
  DumpStats(bl2);

  DumpList(bl, true);
  DumpStats(bl);

  delete [] ia;
}

void test9_4()
{
  std::cout << "==================== test9_4 ====================\n";
  const unsigned asize = 4;

  Digipen::Utils::srand(2, 1);
  BList<int, asize> bl;
  std::cout << "nodesize is " << bl.nodesize() << std::endl;
  unsigned low = 10;
  unsigned high = 99;
  unsigned range = high - low;
  unsigned size = 16;
  int *ia = new int[range];
  for (unsigned i = low; i < high; i++)
    ia[i - low] = i + 1;

  Shuffle(ia, range);

  for (unsigned i = 0; i < size; i++) 
    bl.insert(ia[i]);

  DumpList(bl, false);
  DumpList(bl, true);
  DumpStats(bl);

    // copy construction
  BList<int, asize> bl2(bl);

  DumpList(bl2, false);
  DumpList(bl2, true);
  DumpStats(bl2);

  DumpList(bl, true);
  DumpStats(bl);

  delete [] ia;
}

void test10_1()
{
  std::cout << "==================== test10_1 ====================\n";
  const unsigned asize = 1;

  Digipen::Utils::srand(2, 1);
  BList<int, asize> bl;
  std::cout << "nodesize is " << bl.nodesize() << std::endl;
  unsigned low = 10;
  unsigned high = 99;
  unsigned range = high - low;
  unsigned size = 16;
  int *ia = new int[range];
  for (unsigned i = low; i < high; i++)
    ia[i - low] = i + 1;

  Shuffle(ia, range);

  for (unsigned i = 0; i < size; i++) 
    bl.insert(ia[i]);

  DumpList(bl, false);
  DumpList(bl, true);
  DumpStats(bl);

    // assignment
  BList<int, asize> bl2;
  bl2 = bl;

  DumpList(bl2, false);
  DumpList(bl2, true);
  DumpStats(bl2);

  DumpList(bl, true);
  DumpStats(bl);

  delete [] ia;
}

void test10_4()
{
  std::cout << "==================== test10_4 ====================\n";
  const unsigned asize = 4;

  Digipen::Utils::srand(2, 1);
  BList<int, asize> bl;
  std::cout << "nodesize is " << bl.nodesize() << std::endl;
  unsigned low = 10;
  unsigned high = 99;
  unsigned range = high - low;
  unsigned size = 16;
  int *ia = new int[range];
  for (unsigned i = low; i < high; i++)
    ia[i - low] = i + 1;

  Shuffle(ia, range);

  for (unsigned i = 0; i < size; i++) 
    bl.insert(ia[i]);

  DumpList(bl, false);
  DumpList(bl, true);
  DumpStats(bl);

    // assignment
  BList<int, asize> bl2;
  bl2 = bl;

  DumpList(bl2, false);
  DumpList(bl2, true);
  DumpStats(bl2);

  DumpList(bl, true);
  DumpStats(bl);

  delete [] ia;
}

void test11_8()
{
  std::cout << "==================== test11_8 ====================\n";
  const unsigned asize = 8;

  Digipen::Utils::srand(2, 1);
  BList<int, asize> bl;
  std::cout << "nodesize is " << bl.nodesize() << std::endl;
  unsigned low = 10;
  unsigned high = 99;
  unsigned range = high - low;
  unsigned size = 16;
  int *ia = new int[range];
  for (unsigned i = low; i < high; i++)
    ia[i - low] = i + 1;

  Shuffle(ia, range);
  for (unsigned i = 0; i < size; i++) 
  {
    bl.insert(ia[i]);
    //DumpList(bl, true);
    //DumpList(bl);
    //std::cout << "==========================\n";
  }
  DumpList(bl, false);

  for (unsigned i = 0; i < size; i += 3)  
  {
    bl[i] = 99;
  }
  std::cout << std::endl << std::endl;

  DumpList(bl, false);
  DumpList(bl, true);
  DumpStats(bl);
  std::cout << std::endl;
  delete [] ia;
}

void test12_8()
{
  std::cout << "==================== test12_8 ====================\n";
  const unsigned asize = 8;

  Digipen::Utils::srand(2, 1);
  BList<int, asize> bl;
  std::cout << "nodesize is " << bl.nodesize() << std::endl;
  unsigned low = 10;
  unsigned high = 99;
  unsigned range = high - low;
  unsigned size = 16;
  int *ia = new int[range];
  for (unsigned i = low; i < high; i++)
    ia[i - low] = i + 1;

  Shuffle(ia, range);
  for (unsigned i = 0; i < size; i++) 
  {
    bl.insert(ia[i]);
    //DumpList(bl, true);
    //DumpList(bl);
    //std::cout << "==========================\n";
  }
  DumpList(bl, false);
  DumpStats(bl);

  bl.clear();

  DumpList(bl, false);
  DumpStats(bl);
  std::cout << std::endl;
  delete [] ia;
}

///////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////

void test1()
{
  std::cout << "==================== test1 ====================\n";
  const unsigned asize = 4;

  Digipen::Utils::srand(2, 1);
  BList<std::string, asize> bl;
  std::cout << "nodesize is " << bl.nodesize() << std::endl;

  const char *strings[] = {"this", "is", "just", "a", "bunch", "of", "random",
                           "strings", "in", "the", "sentence"};

  int count = sizeof(strings) / sizeof(*strings);
  for (int i = 0; i < count; i++)
  {
    bl.insert(strings[i]);
    DumpList(bl);
  }

  std::cout << "==========================\n";
  std::cout << std::endl;
}

#if 1
struct Foo
{
  int a, b; 
  //Foo() : a(0), b(0) {}
  Foo(int x = 0, int y = 0) : a(x), b(y) {}
  
  bool operator<(const Foo& rhs) const
  {
    if (a < rhs.a)
      return true;
    if (a == rhs.a)
      return b < rhs.b;

    return false;
  }
  
  bool operator==(const Foo& rhs) const
  {
    return ( (a == rhs.a) && (b == rhs.b));
  }

  friend std::ostream& operator<<(std::ostream &os, const Foo& rhs);
};

std::ostream& operator<<(std::ostream& os, const Foo& rhs)
{
  os << "(" << rhs.a << "," << rhs.b << ")";
  return os;
}

void test2()
{
  std::cout << "==================== test2 ====================\n";
  const unsigned asize = 4;

  Digipen::Utils::srand(2, 1);
  BList<Foo, asize> bl;
  std::cout << "nodesize is " << bl.nodesize() << std::endl;

  bl.insert(Foo(3,2));
  DumpList(bl, false);  
  bl.insert(Foo(1,2));
  DumpList(bl, false);  
  bl.insert(Foo(2, 5));
  DumpList(bl, false);  
  bl.insert(Foo(2,7));
  DumpList(bl, false);  
  bl.insert(Foo(2,2));
  DumpList(bl, false);  
  bl.insert(Foo(3, 8));
  DumpList(bl, false);  
  DumpList(bl, true); 
  DumpStats(bl);

  int index = bl.find(Foo(2,7));
  std::cout << "Index of " << Foo(2,7) << " is " << index << std::endl;
  index = bl.find(Foo(1,8));
  std::cout << "Index of " << Foo(1,8) << " is " << index << std::endl << std::endl;

  bl.remove(1);
  DumpList(bl, false);  

  bl.remove_by_value(Foo(2,7));
  DumpList(bl, false);  
  DumpStats(bl);

  std::cout << "==========================\n";
  std::cout << std::endl;
}
#endif

// both have room: add to left
void testA()
{
  std::cout << "===== both have room: add to left =====\n";
  const unsigned asize = 2;

  Digipen::Utils::srand(2, 1);
  BList<int, asize> bl;
  std::cout << "nodesize is " << bl.nodesize() << std::endl;

  bl.insert(20);
  bl.insert(40);
  bl.insert(30);
  bl.remove(1);
  DumpList(bl, false);  
  bl.insert(30);
  DumpList(bl, false);  

  std::cout << std::endl;
}

// both are full: split left
void testB()
{
  std::cout << "===== both are full: split left =====\n";
  const unsigned asize = 2;

  Digipen::Utils::srand(2, 1);
  BList<int, asize> bl;
  std::cout << "nodesize is " << bl.nodesize() << std::endl;

  bl.insert(20);
  bl.insert(40);
  bl.insert(30);
  bl.insert(50);
  DumpList(bl, false);  
  bl.insert(35);
  DumpList(bl, false);  

  std::cout << std::endl;
}

// left has room, right is full: add to left
void testC()
{
  std::cout << "===== left has room, right is full: add to left =====\n";
  const unsigned asize = 2;

  Digipen::Utils::srand(2, 1);
  BList<int, asize> bl;
  std::cout << "nodesize is " << bl.nodesize() << std::endl;

  bl.insert(20);
  bl.insert(30);
  bl.insert(40);
  DumpList(bl, false);  
  bl.insert(25);
  DumpList(bl, false);  

  std::cout << std::endl;
}

// left is full, right has room: add to right
void testD()
{
  std::cout << "===== left is full, right has room: add to right =====\n";
  const unsigned asize = 2;

  Digipen::Utils::srand(2, 1);
  BList<int, asize> bl;
  std::cout << "nodesize is " << bl.nodesize() << std::endl;

  bl.insert(20);
  bl.insert(40);
  bl.insert(30);
  DumpList(bl, false);  
  bl.insert(35);
  DumpList(bl, false);  

}

int main(int argc, char **argv)
{
    /*
   int test ;
  std::cin>>test;
  if (argc > 1)
    test = std::atoi(argv[1]);
 */
  
  int test_num = 0;
  if (argc > 1)
    test_num = std::atoi(argv[1]);
 //std::cin>>test_num;
  typedef void (*Test)(void);
  Test Tests[] = {test1_1,   test1_2,   test1_4,   test1_8,    test1_16, //  1 -  5, push_front
                  test2_1,   test2_2,   test2_4,   test2_8,    test2_16, //  6 - 10, push_back
                  test3_1,   test3_2,   test3_4,   test3_8,    test3_16, // 11 - 15, insert
                  test4_1,   test4_2,   test4_4,   test4_8,    test4_16, // 16 - 20, subscript operator (r-val)
                  test5_1,   test5_2,   test5_4,   test5_8,    test5_16, // 21 - 25, push_back/find (linear search)
                  test6_16,  test6_64,  test6_512,                       // 26 - 28, insert/find (binary search)
                  test7_1,   test7_2,   test7_4,   test7_8,              // 29 - 32, remove by index
                  test8_1,   test8_2,   test8_4,   test8_8,              // 33 - 36, remove by value (unsorted)
                  test9_1,   test9_4,                                    // 37 - 38, copy construction
                  test10_1,  test10_4,                                   // 39 - 40, assignment
                  test11_8,                                              // 41     , subscript operator (l-val)
                  test12_8,                                              // 42     , clear
                  testA, testB, testC, testD                             // 43 - 46, 4 cases above
                 };

  int num = sizeof(Tests) / sizeof(*Tests);
 if (test_num == 0)
  {
    for (int i = 0; i < num; i++)
      Tests[i]();
  }
  else if (test_num > 0 && test_num <= num)
  {
      Tests[test_num - 1]();
  }

/*
  switch (test)
  {
    case 1:
      test1_1();
      test1_2();
      test1_4();
      test1_8();
      test1_16();
      break;
    case 2: 
      test2_1();
      test2_2();
      test2_4();
      test2_8();
      test2_16();
      break;
    case 3: 
      test3_1();
      test3_2();
      test3_4();
      test3_8();
      test3_16();
      break;
    case 4: 
      test4_1();
      test4_2();
      test4_4();
      test4_8();
      test4_16();
     
      test5_1();
      test5_2();
      test5_4();
      test5_8();
      test5_16();
       
      test6_16();
      test6_64();
      test6_512();
      break;
    case 7: 
      test7_1();
      test7_2();
      test7_4();
      test7_8();
      break;
    case 8: 
      test8_1();
      test8_2();
      test8_4();
      test8_8();
      break;
    case 9: 
      test9_1();
      test9_4();
      break;
    case 10: 
      test10_1();
      test10_4();
      break;
    case 11: 
      test11_8();
      break;
    case 12: 
      test12_8();
      break;
    case 13: 
      testA();
      testB();
      testC();
      testD();
      break;
  }
  */
  
  return 0;
}
