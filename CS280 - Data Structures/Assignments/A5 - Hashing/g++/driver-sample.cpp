#include <iostream>
#include <iomanip>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "ChHashTable.h"

using std::cout;
using std::endl;
using std::ostream;
using std::setprecision;

const unsigned ID_LEN = 6;
struct Person
{
  char ID[ID_LEN + 1];
  char lastName[26];   
  char firstName[26];  
  float salary;        
  unsigned years;           
};

struct Person PEOPLE[] = {
  {"101001", "Faith", "Ian", 80000, 10},
  {"102001", "Tufnel", "Nigel", 90000, 12},
  {"103001", "Savage", "Viv", 50000, 4},
  {"104001", "Shrimpton", "Mick", 50000, 4},
  {"105001", "Besser", "Joe", 40000, 1},
  {"106001", "Smalls", "Derek", 80000, 10},
  {"107001", "St.Hubbins", "David", 90000, 12},
  {"108001", "Fleckman", "Bobbi", 120000, 8},
  {"109001", "Eton-Hogg", "Denis", 250000, 22},
  {"110001", "Upham", "Danny", 60000, 5},
  {"111001", "McLochness", "Ross", 60000, 5},
  {"112001", "Pudding", "Ronnie", 50000, 2},
  {"113001", "Schindler", "Danny", 60000, 3},
  {"114001", "Pettibone", "Jeanine", 85000, 3},
  {"115001", "Fame", "Duke", 95000, 8},
  {"116001", "Fufkin", "Artie", 45000, 1},
  {"117001", "DiBergi", "Marty", 15000, 7},
  {"118001", "Floyd", "Pink", 25000, 6},
  {"119001", "Zeppelin", "Led", 35000, 3},
  {"120001", "Mason", "Nick", 15000, 7},
  {"121001", "Wright", "Richard", 17000, 9},
  {"122001", "Waters", "Roger", 10000, 3},
  {"123001", "Gilmore", "David", 19000, 5}
};

Person **PersonRecs;

ostream& operator<<(ostream &os, const Person &person)
{
  char buffer[1000];
  sprintf(buffer, "Key: %8s, Name: %12s, %12s \nSalary:%7.0f, Years: %2d",person.ID, person.lastName, person.firstName, person.salary, person.years);
  os << buffer;
  return os;
}

void FreePersonRecs(void)
{
  unsigned count = sizeof(PEOPLE) / sizeof(*PEOPLE);

  for (unsigned i = 0; i < count; i++)
    delete PersonRecs[i];

  delete[] PersonRecs;
}

void FillPersonRecs(void)
{
  unsigned count = sizeof(PEOPLE) / sizeof(*PEOPLE);
  PersonRecs = (Person **) new Person[count];

  unsigned index = 0;
  for (unsigned i = 0; i < count; i++)
  {
    Person *person = new Person;
    strcpy(person->ID, PEOPLE[i].ID);
    strcpy(person->lastName, PEOPLE[i].lastName);
    strcpy(person->firstName, PEOPLE[i].firstName);
    person->salary = PEOPLE[i].salary;
    person->years = PEOPLE[i].years;
    PersonRecs[index++] = person;
  }
}

unsigned ConstantHash(const char *, unsigned)
{
  return 1;
}

unsigned ReflexiveHash(const char *Key, unsigned TableSize)
{
  return atoi(Key) % TableSize;
}

void RevString(char *Key)
{
  size_t len = strlen(Key);
  for (size_t i = 0; i < len / 2; i++)
  {
    char t = Key[i];
    Key[i] = Key[len - i - 1];
    Key[len - i - 1] = t;
  }
}

unsigned PJWHash(const char *Key, unsigned TableSize)
{
    // Initial value of hash
  unsigned hash = 0;

    // Process each char in the string
  while (*Key)
  {
      // Shift hash left 4
    hash = (hash << 4);
      
      // Add in current char
    hash = hash + (*Key);   

      // Get the four high-order bits
    unsigned bits = hash & 0xF0000000;

      // If any of the four bits are non-zero,
    if (bits)
    {
        // Shift the four bits right 24 positions (...bbbb0000)
        // and XOR them back in to the hash
      hash = hash ^ (bits >> 24); 

        // Now, XOR the four bits back in
      hash = hash ^ bits;         
    }

      // Next char
    Key++;   
  }

    // Modulo so hash is within 0 - TableSize
  return hash % TableSize;  
}

unsigned SimpleHash(const char *Key, unsigned TableSize)
{
    // Initial value of hash
  unsigned hash = 0;

    // Process each char in the string
  while (*Key)
  {
      // Add in current char
    hash += *Key;

      // Next char
    Key++;
  }

    // Modulo so hash is within the table
  return hash % TableSize;
}

unsigned RSHash(const char *Key, unsigned TableSize)
{
  unsigned hash = 0;         // Initial value of hash
  unsigned multiplier = 127; // Prevent anomalies

    // Process each char in the string
  while (*Key)
  {
      // Adjust hash total
    hash = hash * multiplier;

      // Add in current char and mod result
    hash = (hash + *Key) % TableSize;

      // Next char
    Key++;
  }

    // Hash is within 0 - TableSize
  return hash;
}

unsigned UHash(const char *Key, unsigned TableSize)
{
  unsigned hash = 0;       // Initial value of hash
  unsigned rand1 = 31415; // "Random" 1
  unsigned rand2 = 27183; // "Random" 2

    // Process each char in string
  while (*Key)
  {
      // Multiply hash by random
    hash = hash * rand1;

      // Add in current char, keep within TableSize
    hash = (hash + *Key) % TableSize;

      // Update rand1 for next "random" number
    rand1 = (rand1 * rand2) % (TableSize - 1);

      // Next char
    Key++;
  }
    // Hash value is within 0 - TableSize - 1
  return hash;
}


struct HashData
{
  HASHFUNC Fn;
  const char *Name;
};

enum HASHFUNCS {NONE, CONSTANT, REFLEXIVE, SIMPLE, RS, UNIVERSAL, PJW};

HashData HashingFuncs[] = {
  {0, "None (Linear probing)"}
  ,{ConstantHash, "Constant Hash (1)"}
  ,{ReflexiveHash, "Reflexive Hash"}
  ,{SimpleHash, "Simple Hash"}
  ,{RSHash, "RS Hash"}
  ,{UHash, "Universal Hash"}
  ,{PJWHash, "PJW Hash"}
};

void Dispose(void *)
{
}

template <typename T>
void DummyDumpTable(const ChHashTable<T> &)
{
}


template <typename T>
void DumpTable(const ChHashTable<T> &ht)
{
  char buffer[80];
  const typename ChHashTable<T>::ChHTHeadNode *head = ht.GetTable();

  for (unsigned i = 0; i < ht.GetStats().TableSize_; i++)
  {
    const typename ChHashTable<T>::ChHTNode *start = head[i].Nodes; 
    sprintf(buffer, "Slot: %3d ", i);
    cout << buffer;
    while (start)
    {
      sprintf(buffer, "--> %s ", start->Key);
      cout << buffer;
      start = start->Next;
    }
    cout << endl;
  }
}

template <typename T>
void DummyDumpStats(const ChHashTable<T> &, ostream &)
{
}

template <typename T>
void DumpStats(const ChHashTable<T> &ht, ostream &os = cout)
{
  os << "Number of probes: " << ht.GetStats().Probes_ << endl;
  os << "Number of expansions: " << ht.GetStats().Expansions_ << endl;
  os << "Items: " << ht.GetStats().Count_ << ", TableSize: " << ht.GetStats().TableSize_ << endl;
  os << "Load factor: " << setprecision(3) << (double) ht.GetStats().Count_ / (double) ht.GetStats().TableSize_ << endl;
}

// insert
void Test1(HashData *phd)
{
  const char *test = "Test1";
  cout << endl << "==================== " << test << " ====================" << endl;

  HASHFUNC phf = phd->Fn;

  unsigned initial_size = 11;
  double max_load_factor = 3.0;
  double growth_factor = 2.0;

  cout << endl << "Creating table:" << endl;
  cout << "Hash function: " << phd->Name << endl;
  cout << "Initial size: " << initial_size << endl;
  cout << "Max load factor: " << max_load_factor << endl;
  cout << "Growth factor: " << growth_factor << endl;

  typedef Person * T;
  ChHashTable<T> ht(ChHashTable<T>::HTConfig(initial_size, phf, max_load_factor, growth_factor, 0));
  try
  {
    char buf[10];
    //cout << "Inserting " << size << " items...\n";
    unsigned size = 30/*(unsigned)(initial_size * max_load_factor + 100)*/;
    for (unsigned i = 101; i <= size + 101; i++)
    {
      sprintf(buf, "%3i001", i);
      ht.insert(buf, 0);
      //DumpTable<T>(*ht);
      //DumpStats<T>(*ht);
      //cout << "==============================================================" << std::endl;
    }
    DumpTable<T>(ht);
    DumpStats<T>(ht);
    cout << endl;
  }
  catch (HashTableException &e)
  {
    std::cout << "Caught HashTableException in " << test << ": ";
    int value = e.code();
    if (value == HashTableException::E_DUPLICATE)
      std::cout << "E_DUPLICATE" << std::endl;
    else if (value == HashTableException::E_NO_MEMORY)
      std::cout << "E_NO_MEMORY" << std::endl;
    else if (value == HashTableException::E_ITEM_NOT_FOUND)
      std::cout << "E_ITEM_NOT_FOUND" << std::endl;
    else
      std::cout << "Unknown error code." << std::endl;
  }
  catch (...) 
  {
    cout << endl << "**** Something bad happened inserting in " << test << endl << endl;
  }
}

// insert/delete
void Test2(HashData *phd)
{
  const char *test = "Test2";
  cout << endl << "==================== " << test << " ====================" << endl;

  HASHFUNC phf = phd->Fn;

  unsigned initial_size = 11;
  double max_load_factor = 3.0;
  double growth_factor = 2.0;

  cout << endl << "Creating table:" << endl;
  cout << "Hash function: " << phd->Name << endl;
  cout << "Initial size: " << initial_size << endl;
  cout << "Max load factor: " << max_load_factor << endl;
  cout << "Growth factor: " << growth_factor << endl;

  typedef Person * T;
  ChHashTable<T> ht(ChHashTable<T>::HTConfig(initial_size, phf, max_load_factor, growth_factor, 0));
  try
  {
    char buf[10];
    //cout << "Inserting " << size << " items...\n";
    unsigned size = 30/*(unsigned)(initial_size * max_load_factor + 100)*/;
    for (unsigned i = 101; i <= size + 101; i++)
    {
      sprintf(buf, "%3i001", i);
      ht.insert(buf, 0);
    }
    DumpTable<T>(ht);
    DumpStats<T>(ht);
    cout << endl;

    ht.remove("110001");
    ht.remove("101001");
    ht.remove("102001");
    ht.remove("130001");
    ht.remove("125001");
    ht.remove("104001");
    ht.remove("109001");

    DumpTable<T>(ht);
    DumpStats<T>(ht);
    cout << endl;
  }
  catch (HashTableException &e)
  {
    std::cout << "Caught HashTableException in " << test << ": ";
    int value = e.code();
    if (value == HashTableException::E_DUPLICATE)
      std::cout << "E_DUPLICATE" << std::endl;
    else if (value == HashTableException::E_NO_MEMORY)
      std::cout << "E_NO_MEMORY" << std::endl;
    else if (value == HashTableException::E_ITEM_NOT_FOUND)
      std::cout << "E_ITEM_NOT_FOUND" << std::endl;
    else
      std::cout << "Unknown error code." << std::endl;
  }
  catch (...) 
  {
    cout << endl << "**** Something bad happened inserting in " << test << endl << endl;
  }
}

// insert/grow
void Test3(HashData *phd)
{
  const char *test = "Test3";
  cout << endl << "==================== " << test << " ====================" << endl;

  HASHFUNC phf = phd->Fn;

  unsigned initial_size = 11;
  double max_load_factor = 3.0;
  double growth_factor = 1.5;

  cout << endl << "Creating table:" << endl;
  cout << "Hash function: " << phd->Name << endl;
  cout << "Initial size: " << initial_size << endl;
  cout << "Max load factor: " << max_load_factor << endl;
  cout << "Growth factor: " << growth_factor << endl;

  typedef Person * T;
  ChHashTable<T> ht(ChHashTable<T>::HTConfig(initial_size, phf, max_load_factor, growth_factor, 0));
  try
  {
    char buf[10];
    //cout << "Inserting " << size << " items...\n";
    unsigned size = 60/*(unsigned)(initial_size * max_load_factor + 100)*/;
    for (unsigned i = 101; i <= size + 101; i++)
    {
      if ((ht.GetStats().Count_ + 1) / (double)ht.GetStats().TableSize_ > max_load_factor)
      {
        DumpTable<T>(ht);
        DumpStats<T>(ht);
        //cout << "==============================================================" << std::endl;
      }
      sprintf(buf, "%3i001", i);
      ht.insert(buf, 0);
    }
    DumpTable<T>(ht);
    DumpStats<T>(ht);
    cout << endl;
  }
  catch (HashTableException &e)
  {
    std::cout << "Caught HashTableException in " << test << ": ";
    int value = e.code();
    if (value == HashTableException::E_DUPLICATE)
      std::cout << "E_DUPLICATE" << std::endl;
    else if (value == HashTableException::E_NO_MEMORY)
      std::cout << "E_NO_MEMORY" << std::endl;
    else if (value == HashTableException::E_ITEM_NOT_FOUND)
      std::cout << "E_ITEM_NOT_FOUND" << std::endl;
    else
      std::cout << "Unknown error code." << std::endl;
  }
  catch (...) 
  {
    cout << endl << "**** Something bad happened inserting in " << test << endl << endl;
  }
}


// insert/find/exception (NOT_FOUND)
void Test4(HashData *phd)
{
  const char *test = "Test4";
  cout << endl << "==================== " << test << " ====================" << endl;

  HASHFUNC phf = phd->Fn;

  unsigned initial_size = 7;
  double max_load_factor = 3.0;
  double growth_factor = 2.0;

  cout << endl << "Creating table:" << endl;
  cout << "Hash function: " << phd->Name << endl;
  cout << "Initial size: " << initial_size << endl;
  cout << "Max load factor: " << max_load_factor << endl;
  cout << "Growth factor: " << growth_factor << endl;

  typedef Person * T;
  ChHashTable<T> ht(ChHashTable<T>::HTConfig(initial_size, phf, max_load_factor, growth_factor, 0));
  try
  {
    const char *keys[100];
    //cout << "Inserting " << size << " items...\n";
    unsigned size = 20/*(unsigned)(initial_size * max_load_factor + 100)*/;
    for (unsigned i = 0; i < size; i++)
    {
      Person *person = PersonRecs[i];
      keys[i] = person->ID;
      ht.insert(keys[i], person);
      //DumpTable<T>(*ht);
      //DumpStats<T>(*ht);
      //cout << "==============================================================" << std::endl;
    }
    DumpTable<T>(ht);
    DumpStats<T>(ht);
    cout << endl;
  }
  catch (HashTableException &e)
  {
    std::cout << "Caught HashTableException in " << test << ": ";
    int value = e.code();
    if (value == HashTableException::E_DUPLICATE)
      std::cout << "E_DUPLICATE" << std::endl;
    else if (value == HashTableException::E_NO_MEMORY)
      std::cout << "E_NO_MEMORY" << std::endl;
    else if (value == HashTableException::E_ITEM_NOT_FOUND)
      std::cout << "E_ITEM_NOT_FOUND" << std::endl;
    else
      std::cout << "Unknown error code." << std::endl;
  }
  catch (...) 
  {
    cout << endl << "**** Something bad happened inserting in " << test << endl << endl;
  }

  Person *person;

    // find existing
  const char *key = "106001";
  try
  {
    person = reinterpret_cast<Person *> (ht.find(key));
    cout << *person << endl;
    DumpStats<T>(ht);
    cout << endl;
  }
  catch (HashTableException &e)
  {
    std::cout << "Caught HashTableException in " << test << ": ";
    int value = e.code();
    if (value == HashTableException::E_DUPLICATE)
      std::cout << "E_DUPLICATE" << std::endl;
    else if (value == HashTableException::E_NO_MEMORY)
      std::cout << "E_NO_MEMORY" << std::endl;
    else if (value == HashTableException::E_ITEM_NOT_FOUND)
      std::cout << "E_ITEM_NOT_FOUND" << std::endl;
    else
      std::cout << "Unknown error code." << std::endl;
  }
  catch (...) 
  {
    cout << endl << "**** Something bad happened inserting in " << test << endl << endl;
  }

    // find non-existent
  key = "123456";
  try
  {
    person = reinterpret_cast<Person *> (ht.find(key));
    cout << *person << endl;
    DumpStats<T>(ht);
    cout << endl;
  }
  catch (HashTableException &e)
  {
    std::cout << "Caught HashTableException in " << test << ": ";
    int value = e.code();
    if (value == HashTableException::E_DUPLICATE)
      std::cout << "E_DUPLICATE" << std::endl;
    else if (value == HashTableException::E_NO_MEMORY)
      std::cout << "E_NO_MEMORY" << std::endl;
    else if (value == HashTableException::E_ITEM_NOT_FOUND)
      std::cout << "E_ITEM_NOT_FOUND" << std::endl;
    else
      std::cout << "Unknown error code." << std::endl;
  }
  catch (...) 
  {
    cout << endl << "**** Something bad happened inserting in " << test << endl << endl;
  }
}

// insert/duplicate
void Test5(HashData *phd)
{
  const char *test = "Test5";
  cout << endl << "==================== " << test << " ====================" << endl;

  HASHFUNC phf = phd->Fn;

  unsigned initial_size = 11;
  double max_load_factor = 3.0;
  double growth_factor = 2.0;

  cout << endl << "Creating table:" << endl;
  cout << "Hash function: " << phd->Name << endl;
  cout << "Initial size: " << initial_size << endl;
  cout << "Max load factor: " << max_load_factor << endl;
  cout << "Growth factor: " << growth_factor << endl;

  typedef Person * T;
  ChHashTable<T> ht(ChHashTable<T>::HTConfig(initial_size, phf, max_load_factor, growth_factor, 0));
  try
  {
    char buf[10];
    //cout << "Inserting " << size << " items...\n";
    unsigned size = 30/*(unsigned)(initial_size * max_load_factor + 100)*/;
    for (unsigned i = 101; i <= size + 101; i++)
    {
      sprintf(buf, "%3i001", i);
      ht.insert(buf, 0);
      //DumpTable<T>(*ht);
      //DumpStats<T>(*ht);
      //cout << "==============================================================" << std::endl;
    }
    DumpTable<T>(ht);
    DumpStats<T>(ht);
    cout << endl;

    ht.insert("101001", 0);
  }
  catch (HashTableException &e)
  {
    std::cout << "Caught HashTableException in " << test << ": ";
    int value = e.code();
    if (value == HashTableException::E_DUPLICATE)
      std::cout << "E_DUPLICATE" << std::endl;
    else if (value == HashTableException::E_NO_MEMORY)
      std::cout << "E_NO_MEMORY" << std::endl;
    else if (value == HashTableException::E_ITEM_NOT_FOUND)
      std::cout << "E_ITEM_NOT_FOUND" << std::endl;
    else
      std::cout << "Unknown error code." << std::endl;
  }
  catch (...) 
  {
    cout << endl << "**** Something bad happened inserting in " << test << endl << endl;
  }
}

// insert/clear
void Test6(HashData *phd)
{
  const char *test = "Test6";
  cout << endl << "==================== " << test << " ====================" << endl;

  HASHFUNC phf = phd->Fn;

  unsigned initial_size = 11;
  double max_load_factor = 3.0;
  double growth_factor = 2.0;

  cout << endl << "Creating table:" << endl;
  cout << "Hash function: " << phd->Name << endl;
  cout << "Initial size: " << initial_size << endl;
  cout << "Max load factor: " << max_load_factor << endl;
  cout << "Growth factor: " << growth_factor << endl;

  typedef Person * T;
  ChHashTable<T> ht(ChHashTable<T>::HTConfig(initial_size, phf, max_load_factor, growth_factor, 0));
  try
  {
    char buf[10];
    //cout << "Inserting " << size << " items...\n";
    unsigned size = 30/*(unsigned)(initial_size * max_load_factor + 100)*/;
    for (unsigned i = 101; i <= size + 101; i++)
    {
      sprintf(buf, "%3i001", i);
      ht.insert(buf, 0);
      //DumpTable<T>(*ht);
      //DumpStats<T>(*ht);
      //cout << "==============================================================" << std::endl;
    }
    DumpTable<T>(ht);
    DumpStats<T>(ht);
    cout << endl;

    ht.clear();

    DumpTable<T>(ht);
    DumpStats<T>(ht);
    cout << endl;
 }
  catch (HashTableException &e)
  {
    std::cout << "Caught HashTableException in " << test << ": ";
    int value = e.code();
    if (value == HashTableException::E_DUPLICATE)
      std::cout << "E_DUPLICATE" << std::endl;
    else if (value == HashTableException::E_NO_MEMORY)
      std::cout << "E_NO_MEMORY" << std::endl;
    else if (value == HashTableException::E_ITEM_NOT_FOUND)
      std::cout << "E_ITEM_NOT_FOUND" << std::endl;
    else
      std::cout << "Unknown error code." << std::endl;
  }
  catch (...) 
  {
    cout << endl << "**** Something bad happened inserting in " << test << endl << endl;
  }
}

// small stress with allocator
void Test7(HashData *phd)
{
  cout << endl << "==================== Test7 ====================" << endl;

  HASHFUNC phf = phd->Fn;

  unsigned size = 100;
  unsigned initial_size = 13;
  double max_load_factor = 4.0;
  double growth_factor = 1.2;

  cout << endl << "Creating table:" << endl;
  cout << "Hash function: " << phd->Name << endl;
  cout << "Initial size: " << initial_size << endl;
  cout << "Max load factor: " << max_load_factor << endl;
  cout << "Growth factor: " << growth_factor << endl;

  typedef Person * T;

  OAConfig config(true);
  ObjectAllocator *oa = new ObjectAllocator(sizeof(ChHashTable<T>::ChHTNode), config);
  ChHashTable<T> *ht = new ChHashTable<T>(ChHashTable<T>::HTConfig(initial_size, phf, max_load_factor, growth_factor, 0), oa);
  try
  {
    unsigned i;
    char buf[10];
    cout << "\nInserting " << size << " items...\n";
    for (i = 1; i <= size; i++)
    {
      sprintf(buf, "%09i", i);
      ht->insert(buf, 0);
    }
    
    DumpTable<T>(*ht);
    DumpStats<T>(*ht);
    cout << endl;
#if 1
    cout << "Deleting " << (size - 20) << " items...";
    for (i = 1; i <= size - 20; i++)
    {
      sprintf(buf, "%09i", i);
      ht->remove(buf);
    }
    cout << endl;
    DumpTable<T>(*ht);
    DumpStats<T>(*ht);
    cout << endl;
#endif
  }
  catch (HashTableException &)
  {
    //cout << endl << "errno: " << e.code() << ", " << e.what() << endl << endl;
    cout << endl << "**** Caught exception inserting/deleting item. (SimpleTest2) ****" << endl << endl;
  }
  catch (...) 
  {
    cout << endl << "**** Something bad happened inserting in SimpleTest2" << endl << endl;
  }

  delete ht;
  delete oa;
}

// stress
void Test8(HashData *phd)
{
  cout << endl << "==================== StressTest ====================" << endl;

  HASHFUNC phf = phd->Fn;

  unsigned size = 1000000;
  unsigned initial_size = 5; //4344479;
  double max_load_factor = 2.5;
  double growth_factor = 1.5;

  cout << endl << "Creating table:" << endl;
  cout << "Hash function: " << phd->Name << endl;
  cout << "Initial size: " << initial_size << endl;
  cout << "Max load factor: " << max_load_factor << endl;
  cout << "Growth factor: " << growth_factor << endl;

  typedef Person * T;

  OAConfig config(true);
  ObjectAllocator *oa = new ObjectAllocator(sizeof(ChHashTable<T>::ChHTNode), config);
  ChHashTable<T> *ht = new ChHashTable<T>(ChHashTable<T>::HTConfig(initial_size, phf, max_load_factor, growth_factor, 0), oa);
  try
  {
    unsigned i;
    char buf[10];
    cout << "\nInserting " << size << " items...\n";
    for (i = 1; i <= size; i++)
    {
      sprintf(buf, "%09i", i);
      ht->insert(buf, 0);
    }
    
    //DumpTable<T>(*ht);
    DumpStats<T>(*ht);
    cout << endl;
#if 1
    cout << "Deleting " << size << " items...";
    for (i = 1; i <= size; i++)
    {
      sprintf(buf, "%09i", i);
      ht->remove(buf);
    }
    cout << endl;
    DumpStats<T>(*ht);
    cout << endl;
#endif
  }
  catch (HashTableException &)
  {
    //cout << endl << "errno: " << e.code() << ", " << e.what() << endl << endl;
    cout << endl << "**** Caught exception inserting/deleting item. (SimpleTest2) ****" << endl << endl;
  }
  catch (...) 
  {
    cout << endl << "**** Something bad happened inserting in SimpleTest2" << endl << endl;
  }

  delete ht;
  delete oa;
}

int main(int argc, char **argv)
{
  FillPersonRecs();

  //enum HASHFUNCS {NONE, CONSTANT, REFLEXIVE, SIMPLE, RS, UNIVERSAL, PJW}; 
  HASHFUNCS hf = UNIVERSAL;

  int test_num = 0;
  std::cin>>test_num;
  if (argc > 1)
    test_num = atoi(argv[1]);

  
  switch(test_num)
  {
    case 1:
      Test1(&HashingFuncs[hf]);  // insert
      break;

    case 2:
      Test2(&HashingFuncs[hf]);  // insert/delete
      break;

    case 3:
      Test3(&HashingFuncs[hf]);  // insert/grow
      break;

    case 4:
      Test4(&HashingFuncs[hf]);  // insert/find/exception (NOT_FOUND)
      break;

    case 5:
      Test5(&HashingFuncs[hf]);  // insert duplicate
      break;

    case 6:
      Test6(&HashingFuncs[hf]);  // insert clear
      break;

    case 7:
      Test7(&HashingFuncs[hf]);  // small stress with allocator
      break;

    case 8:
      Test8(&HashingFuncs[hf]); // stress
      break;

    default:
      Test1(&HashingFuncs[hf]); // insert
      Test2(&HashingFuncs[hf]); // insert/delete
      Test3(&HashingFuncs[hf]); // insert/grow
      Test4(&HashingFuncs[hf]); // insert/find/exception (NOT_FOUND)
      Test5(&HashingFuncs[hf]); // insert duplicate
      Test6(&HashingFuncs[hf]); // insert clear
      Test7(&HashingFuncs[hf]); // small stress with allocator
      Test8(&HashingFuncs[hf]); // stress
      break;
  }

  FreePersonRecs();
  return 0;
}
