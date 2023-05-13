#include <iostream> 
#include <cstdio>
#include <cstring>
#include <cstdlib>

using std::cout;
using std::endl;
using std::printf;

int SHOWADDRESS1 = 0;
int SHOWADDRESS2 = 0;
int SHOWREAL = 0;
int SHOW_EXCEPTIONS = 0;

#include "ObjectAllocator.h"
#include "PRNG.h"

struct Student
{
    int Age;
    float GPA;
    long Year;
    long ID;
};

struct Employee
{
    Employee* Next;
    char lastName[12];
    char firstName[12];
    float salary;
    int years;
};


ObjectAllocator* studentObjectMgr;
ObjectAllocator* employeeObjectMgr;

// Support functions
void PrintCounts(const ObjectAllocator* nm);
void PrintCounts2(const ObjectAllocator* nm);
void PrintConfig(const ObjectAllocator* nm);
void DumpPages(const ObjectAllocator* nm, unsigned width = 16);
void DumpPagesEx(const ObjectAllocator* nm, unsigned width = 16);

void DoStudents(unsigned padding = 0, bool printall = false);    // debug, padding=X
void DoEmployees(void);               // debug
void TestPadding(unsigned size);               // DoStudents(padding=16)
void TestBasicHeaderBlocks();         // debug, header
void TestCorruption(void);            // debug, padding=8
void DisableOA(void);                 // cpp_mem_mgr
void TestLeak(void);                  // debug
void TestValidate(void);              // debug, padding=8
void TestAlignment(void);             // debug, padding=2, align=8
void TestFreeEmptyPages1(void);       // debug, padding=2
void TestFreeEmptyPages2(void);       // debug, padding=2, header, align=16
void TestFreeEmptyPages3(void);       // debug, padding=6
void StressFreeChecking(void);        //
void Stress(bool UseNewDelete);       // 

struct Person
{
    char lastName[12];
    char firstName[12];
    float salary;
    int years;
};

struct Person PEOPLE[] = {
  {"Faith", "Ian", 80000, 10},
  {"Tufnel", "Nigel", 90000, 12},
  {"Savage", "Viv", 50000, 4},
  {"Shrimpton", "Mick", 50000, 4},
  {"Besser", "Joe", 40000, 1},
  {"Smalls", "Derek", 80000, 10},
  {"St.Hubbins", "David", 90000, 12},
  {"Fleckman", "Bobbi", 120000, 8},
  {"Eton-Hogg", "Denis", 250000, 22},
  {"Upham", "Denny", 60000, 5},
  {"McLochness", "Ross", 60000, 5},
  {"Pudding", "Ronnie", 50000, 2},
  {"Schindler", "Danny", 60000, 3},
  {"Pettibone", "Jeanine", 85000, 3},
  {"Fame", "Duke", 95000, 8},
  {"Fufkin", "Artie", 45000, 1},
  {"DiBergi", "Marty", 15000, 7},
  {"Floyd", "Pink", 25000, 6},
  {"Zeppelin", "Led", 35000, 3},
  {"Mason", "Nick", 15000, 7},
  {"Wright", "Richard", 17000, 9},
  {"Waters", "Roger", 10000, 3},
  {"Gilmore", "David", 19000, 5}
};

void FillEmployee(Employee& emp)
{
    static unsigned int index = 0;

    strcpy(emp.firstName, PEOPLE[index].firstName);
    strcpy(emp.lastName, PEOPLE[index].lastName);
    emp.salary = PEOPLE[index].salary;
    emp.years = PEOPLE[index].years;
    index++;
    if (index >= sizeof(PEOPLE) / sizeof(*PEOPLE))
        index = 0;
}

void DumpCallback(const void* block, size_t actual_size)
{
    size_t size = actual_size;
    // limit to 16 bytes
    if (actual_size > 16)
        size = 16;

    unsigned char* data = const_cast<unsigned char*>(static_cast<const unsigned char*>(block));

    if (SHOWADDRESS1)
        printf("Block at 0x%p, %u bytes long.\n", block, static_cast<unsigned>(actual_size));
    else
        printf("Block at 0x00000000, %u bytes long.\n", static_cast<unsigned>(actual_size));

    // If we were passed a NULL pointer, do nothing
    if (!block)
        return;

    printf(" Data: <");
    for (unsigned int i = 0; i < size; i++)
    {
        unsigned char c = *data++;
        if (c > 31 && c < 128) // printable range
            printf("%c", c);
        else
            printf(" ");
    }
    printf(">");

    data = const_cast<unsigned char*>(static_cast<const unsigned char*>(block));
    for (unsigned int i = 0; i < size; i++)
        printf(" %02X", static_cast<int>(*data++));
    printf("\n");
}

void DumpCallback2(const void*, size_t)
{
}

void CheckAndDumpLeaks(const ObjectAllocator* oa)
{
    if (oa->GetStats().ObjectsInUse_)
    {
        printf("Detected memory leaks!\n");
        printf("Dumping objects ->\n");
        unsigned leaks = oa->DumpMemoryInUse(DumpCallback);
        printf("Object dump complete. [%u]\n", leaks);
    }
    else
        printf("No leaks detected.\n");
}

void ValidateCallback(const void* block, size_t actual_size)
{
    if (SHOWADDRESS2)
        printf("Block at 0x%p, %u bytes long.\n", block, static_cast<unsigned>(actual_size));
    else
        printf("Block at 0x00000000, %u bytes long.\n", static_cast<unsigned>(actual_size));

    if (!block)
        return;
}

//****************************************************************************************************
//****************************************************************************************************
int RandomInt(int low, int high)
{
    //return std::rand() % (high - low + 1) + low;
    return Digipen::Utils::Random(low, high);
}

template <typename T>
void SwapT(T& a, T& b)
{
    T temp = a;
    a = b;
    b = temp;
}

template <typename T>
void Shuffle(T* array, unsigned count)
{
    for (unsigned int i = 0; i < count; i++)
    {
        int r = RandomInt(i, static_cast<int>(count) - 1);
        SwapT(array[i], array[r]);
    }
}

template <typename T>
void PrintArray(T* array, unsigned count)
{
    for (unsigned i = 0; i < count; i++)
        std::cout << array[i] << std::endl;
}

const unsigned objects = 4096;
const unsigned pages = 100;
const unsigned total = objects * pages;
void* ptrs[total];

#include <ctime>
void Stress(bool UseNewDelete)
{
    ObjectAllocator* oa;

    try
    {
        bool newdel = UseNewDelete;
        bool debug = false;
        unsigned padbytes = 0;
        OAConfig::HeaderBlockInfo header(OAConfig::hbNone);
        unsigned alignment = 0;

        OAConfig config(newdel, objects, pages, debug, padbytes, header, alignment);
        oa = new ObjectAllocator(sizeof(Student), config);
        for (unsigned i = 0; i < total; i++)
        {
            void* p = oa->Allocate();
            ptrs[i] = p;
        }

        Shuffle(ptrs, total);
        for (unsigned i = 0; i < total; i++)
        {
            oa->Free(ptrs[i]);
        }

        delete oa;
    }
    catch (const OAException& e)
    {
        if (SHOW_EXCEPTIONS)
            cout << e.what() << endl;
        else
            cout << "Exception thrown during construction in Stress." << endl;

        return;
    }
}

void StressFreeChecking(const OAConfig::HeaderBlockInfo& header)
{
    unsigned objects;
    unsigned pages;

    objects = 1000;
    pages = 10;

    const unsigned total = objects * pages;

    ObjectAllocator* oa;

    char** ptrs = new char* [total];

    try
    {
        bool newdel = false;
        bool debug = true;
        unsigned padbytes = 0;
        unsigned alignment = 0;

        OAConfig config(newdel, objects, pages, debug, padbytes, header, alignment);
        oa = new ObjectAllocator(sizeof(Student), config);

        for (unsigned int i = 0; i < total; i++)
        {
            void* p = oa->Allocate();
            ptrs[i] = static_cast<char*>(p);
        }

        PrintConfig(oa);
        PrintCounts(oa);
        //DumpPagesEx(oa, 42);

        Shuffle(ptrs, total);
        for (unsigned int i = 0; i < total; i++)
        {
            oa->Free(ptrs[i]);
        }

        oa->DumpMemoryInUse(DumpCallback2);
        delete oa;
    }
    catch (const OAException& e)
    {
        if (SHOW_EXCEPTIONS)
            cout << e.what() << endl;
        else
        {
            cout << "Exception thrown during StressFreeChecking." << endl;
            fflush(stdout);
        }

        return;
    }
    delete[] ptrs;
}

void TestFreeEmptyPages1(void)
{
    ObjectAllocator* oa;
    const int objects = 4;
    const int pages = 3;
    const int total = objects * pages;
    void* ptrs[total];
    try
    {
        bool newdel = false;
        bool debug = true;
        unsigned padbytes = 2;
        OAConfig::HeaderBlockInfo header(OAConfig::hbNone);
        unsigned alignment = 0;

        OAConfig config(newdel, objects, pages, debug, padbytes, header, alignment);
        oa = new ObjectAllocator(sizeof(Student), config);

        unsigned width = 32;

        for (int i = 0; i < total; i++)
        {
            void* p = oa->Allocate();
            ptrs[i] = p;
        }
        //****************************************************************************
        PrintConfig(oa);
        PrintCounts(oa);
        DumpPages(oa, width);

        for (int i = 0; i < objects; i++)
            oa->Free(ptrs[i + 0]);

        for (int i = 0; i < objects; i++)
            oa->Free(ptrs[i + 8]);

        //****************************************************************************
        PrintCounts(oa);
        DumpPages(oa, width);

        //****************************************************************************
        oa->FreeEmptyPages();
        PrintCounts(oa);
        DumpPages(oa, width);

        //****************************************************************************
        for (int i = 0; i < objects - 1; i++)
            oa->Free(ptrs[i + 4]);
        oa->FreeEmptyPages();
        PrintCounts(oa);
        DumpPages(oa, width);

        //****************************************************************************
        oa->Free(ptrs[7]);
        oa->FreeEmptyPages();
        PrintCounts(oa);
        DumpPages(oa, width);


        //****************************************************************************
        printf("\n\n");
        void* p = oa->Allocate();
        oa->FreeEmptyPages();
        PrintCounts(oa);
        DumpPages(oa, width);

        oa->Free(p);
        //oa->FreeEmptyPages();
        PrintCounts(oa);
        DumpPages(oa, width);

        oa->FreeEmptyPages();
        PrintCounts(oa);
        DumpPages(oa, width);

        delete oa;
    }
    catch (const OAException& e)
    {
        if (SHOW_EXCEPTIONS)
            cout << e.what() << endl;
        else
            cout << "Exception thrown during TestFreeEmptyPages1." << endl;

        return;
    }
}

void TestFreeEmptyPages2(void)
{
    ObjectAllocator* oa;
    const int objects = 4;
    const int pages = 3;
    const int total = objects * pages;
    void* ptrs[total];
    try
    {
        bool newdel = false;
        bool debug = true;
        unsigned padbytes = 2;
        OAConfig::HeaderBlockInfo header(OAConfig::hbBasic);
        unsigned alignment = 16;

        OAConfig config(newdel, objects, pages, debug, padbytes, header, alignment);
        oa = new ObjectAllocator(sizeof(Student), config);

        unsigned width = 32;

        for (int i = 0; i < total; i++)
        {
            void* p = oa->Allocate();
            ptrs[i] = p;
        }
        //****************************************************************************
        PrintConfig(oa);
        PrintCounts(oa);
        DumpPages(oa, width);

        for (int i = 0; i < objects; i++)
            oa->Free(ptrs[i + 0]);

        for (int i = 0; i < objects; i++)
            oa->Free(ptrs[i + 8]);

        //****************************************************************************
        PrintCounts(oa);
        DumpPages(oa, width);

        //****************************************************************************
        oa->FreeEmptyPages();
        PrintCounts(oa);
        DumpPages(oa, width);

        //****************************************************************************
        for (int i = 0; i < objects - 1; i++)
            oa->Free(ptrs[i + 4]);
        oa->FreeEmptyPages();
        PrintCounts(oa);
        DumpPages(oa, width);

        //****************************************************************************
        oa->Free(ptrs[7]);
        oa->FreeEmptyPages();
        PrintCounts(oa);
        DumpPages(oa, width);

        delete oa;
    }
    catch (const OAException& e)
    {
        if (SHOW_EXCEPTIONS)
            cout << e.what() << endl;
        else
            cout << "Exception thrown during TestFreeEmptyPages2." << endl;

        return;
    }
}

void TestFreeEmptyPages3(void)
{
    ObjectAllocator* oa;
    const int objects = 8;
    const int pages = 48;
    const int total = objects * pages;
    //  void *ptrs[total];
    try
    {
        bool newdel = false;
        bool debug = true;
        unsigned padbytes = 6;
        OAConfig::HeaderBlockInfo header(OAConfig::hbExtended, 9);
        unsigned alignment = 0;

        OAConfig config(newdel, objects, pages, debug, padbytes, header, alignment);
        oa = new ObjectAllocator(sizeof(Student), config);

        unsigned width = 32;

        for (int i = 0; i < total; i++)
        {
            void* p = oa->Allocate();
            ptrs[i] = p;
        }
        PrintConfig(oa);
        PrintCounts(oa);
        //DumpPages(oa, width);

        Shuffle(ptrs, total);
        for (int i = 0; i < total - 5; i++)
        {
            oa->Free(ptrs[i]);
        }
        PrintCounts(oa);
        //DumpPages(oa, width);

        unsigned count = oa->FreeEmptyPages();
        PrintCounts(oa);
        DumpPages(oa, width);
        printf("%i pages freed\n", count);

        delete oa;
    }
    catch (const OAException& e)
    {
        if (SHOW_EXCEPTIONS)
            cout << e.what() << endl;
        else
            cout << "Exception thrown during TestFreeEmptyPages3." << endl;

        return;
    }
}

//****************************************************************************************************
//****************************************************************************************************
void TestBasicHeaderBlocks()
{
    ObjectAllocator* oa = 0;

    try
    {
        bool newdel = false;
        bool debug = true;
        unsigned padbytes = 2;
        OAConfig::HeaderBlockInfo header(OAConfig::hbBasic);
        unsigned alignment = 1;

        OAConfig config(newdel, 4, 2, debug, padbytes, header, alignment);
        oa = new ObjectAllocator(sizeof(Student), config);
    }
    catch (const OAException& e)
    {
        if (SHOW_EXCEPTIONS)
            cout << e.what() << endl;
        else
            cout << "Exception thrown during construction in TestBasicHeaderBlocks." << endl;

        return;
    }

    unsigned wrap = 32;
    PrintConfig(oa);
    PrintCounts(oa);
    DumpPages(oa, wrap);

    Student* pStudent1 = 0;
    try
    {
        pStudent1 = static_cast<Student*>(oa->Allocate());
    }
    catch (const OAException& e)
    {
        if (SHOW_EXCEPTIONS)
            cout << e.what() << endl;
        else
            cout << "Exception thrown from Allocate in TestBasicHeaderBlocks." << endl;
    }
    catch (...)
    {
        cout << "Unexpected exception thrown from Allocate in TestBasicHeaderBlocks." << endl;
    }

    Student* pStudent2 = 0;
    try
    {
        pStudent2 = static_cast<Student*>(oa->Allocate());
    }
    catch (const OAException& e)
    {
        if (SHOW_EXCEPTIONS)
            cout << e.what() << endl;
        else
            cout << "Exception thrown from Allocate in TestBasicHeaderBlocks." << endl;
    }
    catch (...)
    {
        cout << "Unexpected exception thrown from Allocate in TestBasicHeaderBlocks." << endl;
    }

    DumpPages(oa, wrap);

    try
    {
        oa->Free(pStudent1);
    }
    catch (const OAException& e)
    {
        if (SHOW_EXCEPTIONS)
            cout << e.what() << endl;
        else
            cout << "Exception thrown from Free in TestBasicHeaderBlocks." << endl;
    }
    catch (...)
    {
        cout << "Unexpected exception thrown from Free in TestBasicHeaderBlocks." << endl;
    }

    DumpPages(oa, wrap);

    try
    {
        oa->Free(pStudent2);
    }
    catch (const OAException& e)
    {
        if (SHOW_EXCEPTIONS)
            cout << e.what() << endl;
        else
            cout << "Exception thrown from Free in TestBasicHeaderBlocks." << endl;
    }
    catch (...)
    {
        cout << "Unexpected exception thrown from Free in TestBasicHeaderBlocks." << endl;
    }

    DumpPages(oa, wrap);

    try
    {
        pStudent1 = static_cast<Student*>(oa->Allocate());
    }
    catch (const OAException& e)
    {
        if (SHOW_EXCEPTIONS)
            cout << e.what() << endl;
        else
            cout << "Exception thrown from Allocate in TestBasicHeaderBlocks." << endl;
    }
    catch (...)
    {
        cout << "Unexpected exception thrown from Allocate in TestBasicHeaderBlocks." << endl;
    }

    DumpPages(oa, wrap);
    delete oa;
}

//****************************************************************************************************
//****************************************************************************************************
void TestExtendedHeaderBlocks(unsigned size)
{
    ObjectAllocator* oa = 0;
    unsigned wrap = 32;

    try
    {
        bool newdel = false;
        bool debug = true;
        unsigned padbytes = 2;
        OAConfig::HeaderBlockInfo header(OAConfig::hbExtended, size);
        unsigned alignment = 0;

        OAConfig config(newdel, 4, 2, debug, padbytes, header, alignment);
        oa = new ObjectAllocator(sizeof(Student), config);
    }
    catch (const OAException& e)
    {
        if (SHOW_EXCEPTIONS)
            cout << e.what() << endl;
        else
            cout << "Exception thrown during construction in TestExtendedHeaderBlocks." << endl;

        return;
    }

    PrintConfig(oa);
    PrintCounts(oa);
    DumpPages(oa, wrap);

    ///////////////////////////////////////////////////////////////////////////////////////////////
    ///////////////////////////////////////////////////////////////////////////////////////////////
    Student* pStudent1 = 0;
    try
    {
        pStudent1 = static_cast<Student*>(oa->Allocate());
    }
    catch (const OAException& e)
    {
        if (SHOW_EXCEPTIONS)
            cout << e.what() << endl;
        else
            cout << "Exception thrown from Allocate in TestExtendedHeaderBlocks." << endl;
    }
    catch (...)
    {
        cout << "Unexpected exception thrown from Allocate in TestExtendedHeaderBlocks." << endl;
    }

    DumpPages(oa, wrap);

    ///////////////////////////////////////////////////////////////////////////////////////////////
    ///////////////////////////////////////////////////////////////////////////////////////////////
    Student* pStudent2 = 0;
    try
    {
        pStudent2 = static_cast<Student*>(oa->Allocate());
    }
    catch (const OAException& e)
    {
        if (SHOW_EXCEPTIONS)
            cout << e.what() << endl;
        else
            cout << "Exception thrown from Allocate in TestExtendedHeaderBlocks." << endl;
    }
    catch (...)
    {
        cout << "Unexpected exception thrown from Allocate in TestExtendedHeaderBlocks." << endl;
    }

    DumpPages(oa, wrap);

    ///////////////////////////////////////////////////////////////////////////////////////////////
    ///////////////////////////////////////////////////////////////////////////////////////////////
    try
    {
        oa->Free(pStudent1);
    }
    catch (const OAException& e)
    {
        if (SHOW_EXCEPTIONS)
            cout << e.what() << endl;
        else
            cout << "Exception thrown from Free in TestExtendedHeaderBlocks." << endl;
    }
    catch (...)
    {
        cout << "Unexpected exception thrown from Free in TestExtendedHeaderBlocks." << endl;
    }

    DumpPages(oa, wrap);

    ///////////////////////////////////////////////////////////////////////////////////////////////
    ///////////////////////////////////////////////////////////////////////////////////////////////
    try
    {
        oa->Free(pStudent2);
    }
    catch (const OAException& e)
    {
        if (SHOW_EXCEPTIONS)
            cout << e.what() << endl;
        else
            cout << "Exception thrown from Free in TestExtendedHeaderBlocks." << endl;
    }
    catch (...)
    {
        cout << "Unexpected exception thrown from Free in TestExtendedHeaderBlocks." << endl;
    }

    DumpPages(oa, wrap);

    ///////////////////////////////////////////////////////////////////////////////////////////////
    ///////////////////////////////////////////////////////////////////////////////////////////////
    try
    {
        pStudent1 = static_cast<Student*>(oa->Allocate());
        //pStudent2 = static_cast<Student *>( oa->Allocate() );
    }
    catch (const OAException& e)
    {
        if (SHOW_EXCEPTIONS)
            cout << e.what() << endl;
        else
            cout << "Exception thrown from Allocate in TestExtendedHeaderBlocks." << endl;
    }
    catch (...)
    {
        cout << "Unexpected exception thrown from Allocate in TestExtendedHeaderBlocks." << endl;
    }

    DumpPages(oa, wrap);

    ///////////////////////////////////////////////////////////////////////////////////////////////
    ///////////////////////////////////////////////////////////////////////////////////////////////
    try
    {
        for (int i = 0; i < 5; i++)
        {
            oa->Free(pStudent1);
            pStudent1 = static_cast<Student*>(oa->Allocate());
        }
    }
    catch (const OAException& e)
    {
        if (SHOW_EXCEPTIONS)
            cout << e.what() << endl;
        else
            cout << "Exception thrown from Allocate in TestExtendedHeaderBlocks." << endl;
    }
    catch (...)
    {
        cout << "Unexpected exception thrown from Allocate in TestExtendedHeaderBlocks." << endl;
    }

    DumpPages(oa, wrap);

    ///////////////////////////////////////////////////////////////////////////////////////////////
    ///////////////////////////////////////////////////////////////////////////////////////////////
    try
    {
        pStudent2 = static_cast<Student*>(oa->Allocate());
    }
    catch (const OAException& e)
    {
        if (SHOW_EXCEPTIONS)
            cout << e.what() << endl;
        else
            cout << "Exception thrown from Allocate in TestExtendedHeaderBlocks." << endl;
    }
    catch (...)
    {
        cout << "Unexpected exception thrown from Allocate in TestExtendedHeaderBlocks." << endl;
    }

    DumpPages(oa, wrap);
    delete oa;
}

//****************************************************************************************************
//****************************************************************************************************
void TestExternalHeaderBlocks()
{
    ObjectAllocator* oa = 0;
    unsigned wrap = 32;

    try
    {
        bool newdel = false;
        bool debug = true;
        unsigned padbytes = 2;
        OAConfig::HeaderBlockInfo header(OAConfig::hbExternal);
        unsigned alignment = 0;

        OAConfig config(newdel, 4, 1, debug, padbytes, header, alignment);
        oa = new ObjectAllocator(sizeof(Student), config);
    }
    catch (const OAException& e)
    {
        if (SHOW_EXCEPTIONS)
            cout << e.what() << endl;
        else
            cout << "Exception thrown during construction in TestExternalHeaderBlocks." << endl;

        return;
    }

    PrintConfig(oa);
    PrintCounts(oa);
    DumpPagesEx(oa, wrap);

    Student* pStudent1 = 0;
    try
    {
        pStudent1 = static_cast<Student*>(oa->Allocate("First student"));
    }
    catch (const OAException& e)
    {
        if (SHOW_EXCEPTIONS)
            cout << e.what() << endl;
        else
            cout << "Exception thrown from Allocate in TestExternalHeaderBlocks." << endl;
    }
    catch (...)
    {
        cout << "Unexpected exception thrown from Allocate in TestExternalHeaderBlocks." << endl;
    }

    PrintCounts(oa);
    DumpPagesEx(oa, wrap);

    Student* pStudent2 = 0;
    try
    {
        pStudent2 = static_cast<Student*>(oa->Allocate("Second student"));
    }
    catch (const OAException& e)
    {
        if (SHOW_EXCEPTIONS)
            cout << e.what() << endl;
        else
            cout << "Exception thrown from Allocate in TestExternalHeaderBlocks." << endl;
    }
    catch (...)
    {
        cout << "Unexpected exception thrown from Allocate in TestExternalHeaderBlocks." << endl;
    }

    PrintCounts(oa);
    DumpPagesEx(oa, wrap);

    try
    {
        oa->Free(pStudent1);
    }
    catch (const OAException& e)
    {
        if (SHOW_EXCEPTIONS)
            cout << e.what() << endl;
        else
            cout << "Exception thrown from Free in TestExternalHeaderBlocks." << endl;
    }
    catch (...)
    {
        cout << "Unexpected exception thrown from Free in TestExternalHeaderBlocks." << endl;
    }

    PrintCounts(oa);
    DumpPagesEx(oa, wrap);

    try
    {
        oa->Free(pStudent2);
    }
    catch (const OAException& e)
    {
        if (SHOW_EXCEPTIONS)
            cout << e.what() << endl;
        else
            cout << "Exception thrown from Free in TestExternalHeaderBlocks." << endl;
    }
    catch (...)
    {
        cout << "Unexpected exception thrown from Free in TestExternalHeaderBlocks." << endl;
    }

    PrintCounts(oa);
    DumpPagesEx(oa, wrap);

    delete oa;
}

//****************************************************************************************************
//****************************************************************************************************
void DoStudents(unsigned padding, bool printall)
{
    try
    {
        bool newdel = false;
        bool debug = true;
        unsigned padbytes = padding;
        OAConfig::HeaderBlockInfo header(OAConfig::hbNone);
        unsigned alignment = 0;

        OAConfig config(newdel, 4, 2, debug, padbytes, header, alignment);
        studentObjectMgr = new ObjectAllocator(sizeof(Student), config);
    }
    catch (const OAException& e)
    {
        if (SHOW_EXCEPTIONS)
            cout << e.what() << endl;
        else
            cout << "Exception thrown during construction in DoStudents." << endl;
        return;
    }
    catch (...)
    {
        cout << "Unexpected exception thrown from constructor in DoStudents." << endl;
    }

    unsigned wrap = 32;
    PrintConfig(studentObjectMgr);
    PrintCounts(studentObjectMgr);
    DumpPages(studentObjectMgr, wrap);

    Student* pStudent1 = 0, * pStudent2 = 0, * pStudent3 = 0;
    try
    {
        pStudent1 = static_cast<Student*>(studentObjectMgr->Allocate());
        PrintCounts(studentObjectMgr);
        if (printall)
            DumpPages(studentObjectMgr, wrap);
        pStudent2 = static_cast<Student*>(studentObjectMgr->Allocate());
        PrintCounts(studentObjectMgr);
        if (printall)
            DumpPages(studentObjectMgr, wrap);
        pStudent3 = static_cast<Student*>(studentObjectMgr->Allocate());
        PrintCounts(studentObjectMgr);
        if (printall)
            DumpPages(studentObjectMgr, wrap);
    }
    catch (const OAException& e)
    {
        if (SHOW_EXCEPTIONS)
            cout << e.what() << endl;
        else
            cout << "Exception thrown from Allocate in DoStudents." << endl;
    }
    catch (...)
    {
        cout << "Unexpected exception thrown from Allocate in DoStudents." << endl;
    }

    Student* pStudent4[6];
    for (int i = 0; i < 6; i++)
    {
        try
        {
            pStudent4[i] = static_cast<Student*>(studentObjectMgr->Allocate());
            if (pStudent4[i] == 0)
                break;
        }
        catch (const OAException& e)
        {
            if (SHOW_EXCEPTIONS)
                cout << e.what() << endl;
            else
                cout << "Exception thrown from Allocate (2) in DoStudents." << endl;

            break;
        }
        catch (...)
        {
            cout << "Unexpected exception thrown from Allocate (2) in DoStudents." << endl;
        }

    }
    PrintCounts(studentObjectMgr);
    if (printall)
        DumpPages(studentObjectMgr, wrap);

    try
    {
        studentObjectMgr->Free(pStudent1);
        PrintCounts(studentObjectMgr);
        if (printall)
            DumpPages(studentObjectMgr, wrap);
        studentObjectMgr->Free(pStudent2);
        PrintCounts(studentObjectMgr);
        if (printall)
            DumpPages(studentObjectMgr, wrap);
        studentObjectMgr->Free(pStudent3);
        PrintCounts(studentObjectMgr);
        if (printall)
            DumpPages(studentObjectMgr, wrap);
    }
    catch (const OAException& e)
    {
        if (SHOW_EXCEPTIONS)
            cout << e.what() << endl;
        else
            cout << "Exception thrown from Free in DoStudents." << endl;

    }
    catch (...)
    {
        cout << "Unexpected exception thrown from Free in DoStudents." << endl;
    }

    cout << "Most students in use: " << studentObjectMgr->GetStats().MostObjects_ << endl;
    delete studentObjectMgr;
}

//****************************************************************************************************
//****************************************************************************************************
void TestPadding(unsigned size)
{
    DoStudents(size);
}

//****************************************************************************************************
//****************************************************************************************************
void DoEmployees(void)
{
    try
    {
        bool newdel = false;
        bool debug = true;
        unsigned padbytes = 0;
        OAConfig::HeaderBlockInfo header(OAConfig::hbNone);
        unsigned alignment = 0;

        OAConfig config(newdel, 4, 2, debug, padbytes, header, alignment);
        employeeObjectMgr = new ObjectAllocator(sizeof(Employee), config);
    }
    catch (const OAException& e)
    {
        if (SHOW_EXCEPTIONS)
            cout << e.what() << endl;
        else
            cout << "Exception thrown during construction in DoEmployees." << endl;
        return;
    }
    catch (...)
    {
        cout << "Unexpected exception thrown during constructor in DoEmployees." << endl;
    }

    PrintConfig(employeeObjectMgr);
    PrintCounts(employeeObjectMgr);

    Employee* pEmployee1 = 0, * pEmployee2 = 0, * pEmployee3 = 0;
    try
    {
        pEmployee1 = static_cast<Employee*>(employeeObjectMgr->Allocate());
        PrintCounts(employeeObjectMgr);
        pEmployee2 = static_cast<Employee*>(employeeObjectMgr->Allocate());
        PrintCounts(employeeObjectMgr);
        pEmployee3 = static_cast<Employee*>(employeeObjectMgr->Allocate());
        PrintCounts(employeeObjectMgr);
    }
    catch (const OAException& e)
    {
        if (SHOW_EXCEPTIONS)
            cout << e.what() << endl;
        else
            cout << "Exception thrown from Allocate in DoEmployees." << endl;
    }
    catch (...)
    {
        cout << "Unexpected exception thrown from Allocate in DoEmployees." << endl;
    }

    Employee* pEmployee4[7];
    for (int i = 0; i < 7; i++)
    {
        try
        {
            // Need to use /EHa, not /EHs
            // ******************************************* Puts &employeeObjMgr in ecx
            pEmployee4[i] = static_cast<Employee*>(employeeObjectMgr->Allocate());
        }
        catch (const OAException& e)
        {
            if (SHOW_EXCEPTIONS)
            {
                // ******************** Hoses the ecx register containing &employeeObjMgr
                cout << e.what() << endl;
            }
            else
                cout << "Exception thrown from Allocate (2) in DoEmployees." << endl;

            break;
        }
        catch (...)
        {
            cout << "Unexpected exception thrown from Allocate (2) in DoEmployees." << endl;
        }
    }
    // ************************* blindly pushes ecx as if it still has &empObjMgr
    PrintCounts(employeeObjectMgr);

    try
    {
        employeeObjectMgr->Free(pEmployee1);
        PrintCounts(employeeObjectMgr);
        employeeObjectMgr->Free(pEmployee2);
        PrintCounts(employeeObjectMgr);
        employeeObjectMgr->Free(pEmployee3);
        PrintCounts(employeeObjectMgr);
    }
    catch (const OAException& e)
    {
        if (SHOW_EXCEPTIONS)
            cout << e.what() << endl;
        else
            cout << "Exception thrown from Free in DoEmployees." << endl;
    }
    catch (...)
    {
        cout << "Unexpected exception thrown from Free in DoEmployees." << endl;
    }

    // Free an object twice
    try {
        employeeObjectMgr->Free(pEmployee1);
    }
    catch (const OAException& e)
    {
        if (SHOW_EXCEPTIONS)
            cout << e.what() << endl;
        else
            cout << "Exception thrown from Free (Freeing object twice) in DoEmployees." << endl;
    }
    catch (...)
    {
        cout << "Unexpected exception thrown from Free (Freeing object twice) in DoEmployees." << endl;
    }

    // Free an invalid pointer (bad boundary condition)
    try
    {
        employeeObjectMgr->Free(reinterpret_cast<char*>(pEmployee4[0] + 4));
    }
    catch (const OAException& e)
    {
        if (SHOW_EXCEPTIONS)
            cout << e.what() << endl;
        else
            cout << "Exception thrown from Free (Freeing address on bad boundary) in DoEmployees." << endl;
    }
    catch (...)
    {
        cout << "Unexpected exception thrown from Free (Freeing address on bad boundary) in DoEmployees." << endl;
    }

    cout << "Most employees in use: " << employeeObjectMgr->GetStats().MostObjects_ << endl;
    delete employeeObjectMgr;
}

//****************************************************************************************************
//****************************************************************************************************
void DisableOA(void)
{
    try
    {
        bool newdel = true;
        bool debug = false;
        unsigned padbytes = 0;
        OAConfig::HeaderBlockInfo header(OAConfig::hbNone);
        unsigned alignment = 0;

        OAConfig config(newdel, 4, 2, debug, padbytes, header, alignment);
        studentObjectMgr = new ObjectAllocator(sizeof(Student), config);
    }
    catch (const OAException& e)
    {
        if (SHOW_EXCEPTIONS)
            cout << e.what() << endl;
        else
            cout << "Exception thrown during constructor in DisableOA." << endl;
        return;
    }
    catch (...)
    {
        cout << "Unexpected exception thrown during constructor in DisableOA." << endl;
    }

    PrintConfig(studentObjectMgr);
    PrintCounts2(studentObjectMgr);

    Student* pStudent1 = 0, * pStudent2 = 0, * pStudent3 = 0;
    try
    {
        pStudent1 = static_cast<Student*>(studentObjectMgr->Allocate());
        PrintCounts2(studentObjectMgr);
        pStudent2 = static_cast<Student*>(studentObjectMgr->Allocate());
        PrintCounts2(studentObjectMgr);
        pStudent3 = static_cast<Student*>(studentObjectMgr->Allocate());
        PrintCounts2(studentObjectMgr);
    }
    catch (const OAException& e)
    {
        if (SHOW_EXCEPTIONS)
            cout << e.what() << endl;
        else
            cout << "Exception thrown from Allocate in DisableOA." << endl;
    }
    catch (...)
    {
        cout << "Unexpected exception thrown from Allocate in DisableOA." << endl;
    }

    Student* pStudent4[6];
    for (int i = 0; i < 6; i++)
    {
        try
        {
            pStudent4[i] = static_cast<Student*>(studentObjectMgr->Allocate());
        }
        catch (const OAException& e)
        {
            if (SHOW_EXCEPTIONS)
                cout << e.what() << endl;
            else
                cout << "Exception thrown from Allocate (2) in DisableOA." << endl;
            break;
        }
        catch (...)
        {
            cout << "Unexpected exception thrown from Allocate (2) in DisableOA." << endl;
        }

    }
    PrintCounts2(studentObjectMgr);

    try
    {
        studentObjectMgr->Free(pStudent1);
        PrintCounts2(studentObjectMgr);
        studentObjectMgr->Free(pStudent2);
        PrintCounts2(studentObjectMgr);
        studentObjectMgr->Free(pStudent3);
        PrintCounts2(studentObjectMgr);
    }
    catch (const OAException& e)
    {
        if (SHOW_EXCEPTIONS)
            cout << e.what() << endl;
        else
            cout << "Exception thrown from Free in DisableOA." << endl;
    }
    catch (...)
    {
        cout << "Unexpected exception thrown from Free in DIsableOA." << endl;
    }

    for (int i = 0; i < 6; i++)
    {
        try
        {
            studentObjectMgr->Free(pStudent4[i]);
        }
        catch (const OAException& e)
        {
            if (SHOW_EXCEPTIONS)
                cout << e.what() << endl;
            else
                cout << "Exception thrown from Free in DisableOA." << endl;
            break;
        }
        catch (...)
        {
            cout << "Unexpected exception thrown from Free in DisableOA." << endl;
        }

    }
    PrintCounts2(studentObjectMgr);

    cout << "Most students in use: " << studentObjectMgr->GetStats().MostObjects_ << endl;
    delete studentObjectMgr;
}

//****************************************************************************************************
//****************************************************************************************************
void TestCorruption(void)
{
    ObjectAllocator* oa = 0;
    unsigned char* p;
    unsigned padbytes = 8;
    unsigned i;
    unsigned wrap = 32;
    Student* pStudent1, * pStudent2 = 0;
    try
    {
        bool newdel = false;
        bool debug = true;
        //unsigned header = 0;
        //OAConfig::HBLOCK_TYPE header = OAConfig::hbNone;
        OAConfig::HeaderBlockInfo header(OAConfig::hbBasic);
        unsigned alignment = 0;

        OAConfig config(newdel, 4, 2, debug, padbytes, header, alignment);
        //OAConfig config(false, 4, 2, true, padbytes);
        oa = new ObjectAllocator(sizeof(Student), config);
        pStudent1 = static_cast<Student*>(oa->Allocate());
        pStudent2 = static_cast<Student*>(oa->Allocate());

        PrintConfig(oa);
        PrintCounts(oa);
        DumpPages(oa, wrap);
    }
    catch (const OAException& e)
    {
        if (SHOW_EXCEPTIONS)
            cout << e.what() << endl;
        else
            cout << "Exception thrown during construction/allocation in TestCorruption." << endl;

        return;
    }

    // corrupt left pad bytes
    p = const_cast<unsigned char*>(reinterpret_cast<const unsigned char*>(pStudent1)) - padbytes;
    for (i = 0; i < padbytes - 2; i++)
        *p++ = 0xFF;

    // corrupt right pad bytes
    p = const_cast<unsigned char*>(reinterpret_cast<const unsigned char*>(pStudent2)) + sizeof(Student);
    for (i = 0; i < padbytes - 2; i++)
        *p++ = 0xEE;

    try
    {
        oa->Free(pStudent1);
    }
    catch (const OAException& e)
    {
        if (SHOW_EXCEPTIONS)
            cout << e.what() << endl;
        else
        {
            if (e.code() == e.E_CORRUPTED_BLOCK)
                cout << "Exception thrown from Free: E_CORRUPTED_BLOCK on left" << endl;
            else
                cout << "****** Unknown OAException thrown from Free in TestCorruption. ******" << endl;
        }
    }
    catch (...)
    {
        cout << "Unexpected exception thrown from Free in TestCorruption." << endl;
    }

    try
    {
        oa->Free(pStudent2);
    }
    catch (const OAException& e)
    {
        if (SHOW_EXCEPTIONS)
            cout << e.what() << endl;
        else
        {
            if (e.code() == e.E_CORRUPTED_BLOCK)
                cout << "Exception thrown from Free: E_CORRUPTED_BLOCK on right" << endl;
            else
                cout << "****** Unknown OAException thrown from Free (2) in TestCorruption. ******" << endl;
        }
    }
    catch (...)
    {
        cout << "Unexpected exception thrown from Free (2) in TestCorruption." << endl;
    }

    delete oa;
}

//****************************************************************************************************
//****************************************************************************************************
void TestLeak(void)
{
    unsigned count = 0;
    Employee** pEmps = 0;

    try
    {
        bool newdel = false;
        bool debug = true;
        unsigned padbytes = 0;
        OAConfig::HeaderBlockInfo header(OAConfig::hbBasic);
        unsigned alignment = 0;

        OAConfig config(newdel, 4, 8, debug, padbytes, header, alignment);
        employeeObjectMgr = new ObjectAllocator(sizeof(Employee), config);
    }
    catch (const OAException& e)
    {
        if (SHOW_EXCEPTIONS)
            cout << e.what() << endl;
        else
            cout << "Exception thrown during constructor in TestLeak." << endl;

        return;
    }
    catch (...)
    {
        cout << "Unexpected exception thrown during constructor in TestLeak." << endl;
    }

    PrintConfig(employeeObjectMgr);
    PrintCounts(employeeObjectMgr);

    count = employeeObjectMgr->GetConfig().MaxPages_ * employeeObjectMgr->GetConfig().ObjectsPerPage_;
    pEmps = new Employee * [count];

    for (unsigned i = 0; i < count; i++)
    {
        try
        {
            pEmps[i] = static_cast<Employee*>(employeeObjectMgr->Allocate());
            FillEmployee(*pEmps[i]);
        }
        catch (const OAException& e)
        {
            if (SHOW_EXCEPTIONS)
                cout << e.what() << endl;
            else
                cout << "Exception thrown from Allocate in TestLeak." << endl;

            break;
        }
        catch (...)
        {
            cout << "Unexpected exception thrown from Allocate in TestLeak." << endl;
        }
    }
    PrintCounts(employeeObjectMgr);

    try
    {
        for (unsigned i = 0; i < count; i += 2)
            employeeObjectMgr->Free(pEmps[i]);
    }
    catch (const OAException& e)
    {
        if (SHOW_EXCEPTIONS)
            cout << e.what() << endl;
        else
            cout << "Exception thrown from Free in TestLeak." << endl;
    }
    catch (...)
    {
        cout << "Unexpected exception thrown from Free in TestLeak." << endl;
    }

    PrintCounts(employeeObjectMgr);

    cout << "Most employees in use: " << employeeObjectMgr->GetStats().MostObjects_ << endl;

    cout << "\nChecking for leaks...\n";
    CheckAndDumpLeaks(employeeObjectMgr);

    try
    {
        for (unsigned i = 1; i < count; i += 2)
            employeeObjectMgr->Free(pEmps[i]);
    }
    catch (const OAException& e)
    {
        if (SHOW_EXCEPTIONS)
            cout << e.what() << endl;
        else
            cout << "Exception thrown from Free (2) in TestLeak." << endl;
    }
    catch (...)
    {
        cout << "Unexpected exception thrown from Free (2) in TestLeak." << endl;
    }

    cout << "\nChecking for leaks...\n";
    CheckAndDumpLeaks(employeeObjectMgr);

    delete employeeObjectMgr;
    delete[] pEmps;
}


//****************************************************************************************************
//****************************************************************************************************
void TestValidate(void)
{
    ObjectAllocator* oa = 0;
    unsigned char* p;
    unsigned i, padbytes = 8;
    unsigned wrap = 32;
    Student* pStudent1 = 0, * pStudent2 = 0, * pStudent7 = 0;
    try
    {
        bool newdel = false;
        bool debug = true;
        OAConfig::HeaderBlockInfo header(OAConfig::hbBasic);
        unsigned alignment = 0;

        OAConfig config(newdel, 4, 2, debug, padbytes, header, alignment);
        oa = new ObjectAllocator(sizeof(Student), config);

        pStudent1 = static_cast<Student*>(oa->Allocate());
        pStudent2 = static_cast<Student*>(oa->Allocate());
        oa->Allocate(); // 3
        oa->Allocate(); // 4
        oa->Allocate(); // 5
        oa->Allocate(); // 6
        pStudent7 = static_cast<Student*>(oa->Allocate());
        PrintConfig(oa);
        PrintCounts(oa);
        DumpPages(oa, wrap);
    }
    catch (const OAException& e)
    {
        if (SHOW_EXCEPTIONS)
            cout << e.what() << endl;
        else
            cout << "Exception thrown during construction/allocation in TestValidate." << endl;
        return;
    }

    try
    {
        // Validate a good heap
        unsigned count = oa->ValidatePages(ValidateCallback);
        cout << "Number of corruptions: " << count << endl << endl;
    }
    catch (const OAException& e)
    {
        if (SHOW_EXCEPTIONS)
            cout << e.what() << endl;
        else
        {
            if (e.code() == e.E_CORRUPTED_BLOCK)
                cout << "Exception thrown from Free: E_CORRUPTED_BLOCK on left" << endl;
            else
                cout << "****** Unknown OAException thrown from Free in TestValidate. ******" << endl;
        }
    }
    catch (...)
    {
        cout << "Unexpected exception thrown from Free in TestValidate." << endl;
    }

    // corrupt left pad bytes of 1
    p = const_cast<unsigned char*>(reinterpret_cast<const unsigned char*>(pStudent1)) - padbytes;
    for (i = 0; i < padbytes - 2; i++)
        *p++ = 0xFF;

    // corrupt right pad bytes of 2
    p = const_cast<unsigned char*>(reinterpret_cast<const unsigned char*>(pStudent2)) + sizeof(Student);
    for (i = 0; i < padbytes - 2; i++)
        *p++ = 0xEE;

    PrintCounts(oa);
    DumpPages(oa, wrap);

    try
    {
        // Validate a corrupted heap
        unsigned count = oa->ValidatePages(ValidateCallback);
        cout << "Number of corruptions: " << count << endl << endl;
    }
    catch (const OAException& e)
    {
        if (SHOW_EXCEPTIONS)
            cout << e.what() << endl;
        else
        {
            if (e.code() == e.E_CORRUPTED_BLOCK)
                cout << "Exception thrown from Free: E_CORRUPTED_BLOCK on left" << endl;
            else
                cout << "****** Unknown OAException thrown from Free (2) in TestValidate. ******" << endl;
        }
    }
    catch (...)
    {
        cout << "Unexpected exception thrown from Free (2) in TestValidate." << endl;
    }

    // corrupt left pad bytes of 7
    p = const_cast<unsigned char*>(reinterpret_cast<const unsigned char*>(pStudent7)) - padbytes;
    for (i = 0; i < padbytes - 2; i++)
        *p++ = 0xFF;

    // corrupt right pad bytes of 7
    p = const_cast<unsigned char*>(reinterpret_cast<const unsigned char*>(pStudent7)) + sizeof(Student);
    for (i = 0; i < padbytes - 2; i++)
        *p++ = 0xEE;

    PrintCounts(oa);
    DumpPages(oa, wrap);

    try
    {
        // Validate a corrupted heap
        unsigned count = oa->ValidatePages(ValidateCallback);
        cout << "Number of corruptions: " << count << endl << endl;
    }
    catch (const OAException& e)
    {
        if (SHOW_EXCEPTIONS)
            cout << e.what() << endl;
        else
        {
            if (e.code() == e.E_CORRUPTED_BLOCK)
                cout << "Exception thrown from Free: E_CORRUPTED_BLOCK on left" << endl;
            else
                cout << "****** Unknown OAException thrown from Free (3) in TestValidate. ******" << endl;
        }
    }
    catch (...)
    {
        cout << "Unexpected exception thrown from Free (3) in TestValidate." << endl;
    }
    if (oa)
        delete oa;
}

//****************************************************************************************************
//****************************************************************************************************
void TestAlignment(void)
{
    ObjectAllocator* oa = 0;
    try
    {
        unsigned wrap = 32;

        bool newdel = false;
        bool debug = true;
        unsigned padbytes = 2;
        //unsigned header = 0;
        OAConfig::HeaderBlockInfo header(OAConfig::hbBasic);
        unsigned alignment = 8;

        OAConfig config(newdel, 3, 2, debug, padbytes, header, alignment);

        if (!oa)
            oa = new ObjectAllocator(sizeof(Student), config);

        PrintConfig(oa);
        PrintCounts(oa);
        DumpPages(oa, wrap);

        oa->Allocate(); // 1
        oa->Allocate(); // 2
        oa->Allocate(); // 3
        oa->Allocate(); // 4
        oa->Allocate(); // 5
        oa->Allocate(); // 6

        PrintCounts(oa);
        DumpPages(oa, wrap);
    }
    catch (const OAException& e)
    {
        if (SHOW_EXCEPTIONS)
            cout << e.what() << endl;
        else
            cout << "Exception thrown during construction/allocation in TestAlignment." << endl;
        return;
    }
    if (oa)
        delete oa;
}


void PrintCounts(const ObjectAllocator* nm)
{
    OAStats stats = nm->GetStats();
    cout << "Pages in use: " << stats.PagesInUse_;
    cout << ", Objects in use: " << stats.ObjectsInUse_;
    cout << ", Available objects: " << stats.FreeObjects_;
    cout << ", Allocs: " << stats.Allocations_;
    cout << ", Frees: " << stats.Deallocations_ << endl;
}

void PrintCounts2(const ObjectAllocator* nm)
{
    OAStats stats = nm->GetStats();
    cout << "Allocs: " << stats.Allocations_;
    cout << ", Frees: " << stats.Deallocations_ << endl;
}

void PrintConfig(const ObjectAllocator* oa)
{
    cout << "Object size = " << oa->GetStats().ObjectSize_;
    cout << ", Page size = " << oa->GetStats().PageSize_;
    cout << ", Pad bytes = " << oa->GetConfig().PadBytes_;
    cout << ", ObjectsPerPage = " << oa->GetConfig().ObjectsPerPage_;
    cout << ", MaxPages = " << oa->GetConfig().MaxPages_;
    cout << ", MaxObjects = " << oa->GetConfig().ObjectsPerPage_ * oa->GetConfig().MaxPages_;
    cout << endl;
    cout << "Alignment = " << oa->GetConfig().Alignment_;
    cout << ", LeftAlign = " << oa->GetConfig().LeftAlignSize_;
    cout << ", InterAlign = " << oa->GetConfig().InterAlignSize_;
    cout << ", HeaderBlocks = ";
    if (oa->GetConfig().HBlockInfo_.type_ == OAConfig::hbNone)
        cout << "None";
    else if (oa->GetConfig().HBlockInfo_.type_ == OAConfig::hbBasic)
        cout << "Basic";
    else if (oa->GetConfig().HBlockInfo_.type_ == OAConfig::hbExtended)
        cout << "Extended";
    else if (oa->GetConfig().HBlockInfo_.type_ == OAConfig::hbExternal)
        cout << "External";
    cout << ", Header size = " << oa->GetConfig().HBlockInfo_.size_;
    cout << endl;
}

void DumpPages(const ObjectAllocator* nm, unsigned width)
{
    const unsigned char* pages = static_cast<const unsigned char*>(nm->GetPageList());
    const unsigned char* realpage = pages;

    size_t header_size = nm->GetConfig().HBlockInfo_.size_;

    while (pages)
    {
        unsigned count = 0;

        if (SHOWREAL)
            printf("%p\n", pages);
        else
            printf("XXXXXXXX\n");

        // print column header
        for (unsigned j = 0; j < width; j++)
            printf(" %2i", j);
        printf("\n");

        // "Next page" pointer in the page
        if (SHOWREAL)
        {
            for (unsigned i = 0; i < sizeof(void*); i++, count++)
                printf(" %02X", *pages++);
        }
        else
        {
            for (unsigned j = 0; j < sizeof(void*); pages++, count++, j++)
                printf(" %s", "XX");
        }


        // Left leading alignment bytes
        if (nm->GetConfig().Alignment_ > 1)
        {
            // leading alignment block (if any)
            for (unsigned j = 0; j < nm->GetConfig().LeftAlignSize_; count++, j++)
            {
                if (count >= width)
                {
                    printf("\n");
                    count = 0;
                }
                printf(" %02X", *pages++);
            }
        }

        // Dump each object and its associated info
        for (unsigned int i = 0; i < nm->GetConfig().ObjectsPerPage_; i++)
        {
            // inter-block alignment (not on first block)
            if (i > 0)
            {
                for (unsigned j = 0; j < nm->GetConfig().InterAlignSize_; count++, j++)
                {
                    if (count >= width)
                    {
                        printf("\n");
                        count = 0;
                    }
                    printf(" %02X", *pages++);
                }
            }

            // header block bytes
            for (unsigned j = 0; j < header_size; count++, j++)
            {
                if (count >= width)
                {
                    printf("\n");
                    count = 0;
                }
                printf(" %02X", *pages++);
            }

            // left padding
            for (unsigned j = 0; j < nm->GetConfig().PadBytes_; count++, j++)
            {
                if (count >= width)
                {
                    printf("\n");
                    count = 0;
                }
                printf(" %02X", *pages++);
            }

            // possible next pointer (zero it out)
            for (unsigned j = 0; j < sizeof(void*); count++, pages++, j++)
            {
                if (count >= width)
                {
                    printf("\n");
                    count = 0;
                }
                if (SHOWREAL)
                    printf(" %02X", *pages);
                else
                    printf(" %s", "XX");
            }

            // remaining bytes
            for (unsigned j = 0; j < nm->GetStats().ObjectSize_ - sizeof(void*); count++, j++)
            {
                if (count >= width)
                {
                    printf("\n");
                    count = 0;
                }
                printf(" %02X", *pages++);
            }

            // right pad bytes
            for (unsigned j = 0; j < nm->GetConfig().PadBytes_; count++, j++)
            {
                if (count >= width)
                {
                    printf("\n");
                    count = 0;
                }
                printf(" %02X", *pages++);
            }

        }
        printf("\n\n");

        pages = reinterpret_cast<const unsigned char*>((reinterpret_cast<const GenericObject*>(realpage))->Next);
        realpage = pages;
    }
}

void DumpExternalHeaders(const ObjectAllocator* oa, const unsigned char* p)
{
    unsigned char* page = const_cast<unsigned char*>(p);

    unsigned padbytes = oa->GetConfig().PadBytes_;
    size_t header_size = oa->GetConfig().HBlockInfo_.size_;
    unsigned leftalign = oa->GetConfig().LeftAlignSize_;
    unsigned interalign = oa->GetConfig().InterAlignSize_;
    unsigned count = oa->GetConfig().ObjectsPerPage_;
    size_t offset = padbytes * 2 + header_size + interalign + oa->GetStats().ObjectSize_;

    page += sizeof(void*); // the 'next' pointer
    page += leftalign;      // the left alignment bytes (if any)

      // We're now pointing at the beginning of the header block

    MemBlockInfo* pm = reinterpret_cast<MemBlockInfo*>(*reinterpret_cast<MemBlockInfo**>(page));
    cout << "  Label: " << (pm && pm->label ? pm->label : "") << std::endl;
    cout << " In use: " << (pm ? pm->in_use : 0) << std::endl;
    cout << "Alloc #: " << (pm && pm->in_use ? pm->alloc_num : 0) << std::endl;

    for (unsigned i = 1; i < count; i++)
    {
        page += offset;
        MemBlockInfo* pm = reinterpret_cast<MemBlockInfo*>(*reinterpret_cast<MemBlockInfo**>(page));
        cout << "  Label: " << (pm && pm->label ? pm->label : "") << std::endl;
        cout << " In use: " << (pm ? pm->in_use : 0) << std::endl;
        cout << "Alloc #: " << (pm && pm->in_use ? pm->alloc_num : 0) << std::endl;
    }
}

void DumpPagesEx(const ObjectAllocator* nm, unsigned width)
{
    const unsigned char* pages = static_cast<const unsigned char*>(nm->GetPageList());
    const unsigned char* realpage = pages;

    size_t header_size = nm->GetConfig().HBlockInfo_.size_;

    while (pages)
    {
        unsigned count = 0;

        if (SHOWREAL)
            printf("%p\n", pages);
        else
            printf("XXXXXXXX\n");

        // print column header
        for (unsigned j = 0; j < width; j++)
            printf(" %2i", j);
        printf("\n");

        if (SHOWREAL)
        {
            for (unsigned i = 0; i < sizeof(void*); i++, count++)
                printf(" %02X", *pages++);
        }
        else
        {
            for (unsigned j = 0; j < sizeof(void*); pages++, count++, j++)
                printf(" %s", "XX");
        }


        // Left leading alignment bytes
        if (nm->GetConfig().Alignment_ > 1)
        {
            // leading alignment block (if any)
            for (unsigned j = 0; j < nm->GetConfig().LeftAlignSize_; count++, j++)
            {
                if (count >= width)
                {
                    printf("\n");
                    count = 0;
                }
                printf(" %02X", *pages++);
            }
        }

        for (unsigned int i = 0; i < nm->GetConfig().ObjectsPerPage_; i++)
        {
            // inter-block alignment (not on first block)
            if (i > 0)
            {
                for (unsigned j = 0; j < nm->GetConfig().InterAlignSize_; count++, j++)
                {
                    if (count >= width)
                    {
                        printf("\n");
                        count = 0;
                    }
                    printf(" %02X", *pages++);
                }
            }

            // header block bytes
            for (unsigned j = 0; j < header_size; count++, j++, pages++)
            {
                if (count >= width)
                {
                    printf("\n");
                    count = 0;
                }
                /*////////////////////======================================================================
                In external mode, pointer address is embedded into the header memory
                /////////////////////=======================================================================*/
                if (SHOWREAL)
                    printf(" %02X", *pages);
                else
                    printf(" %s", "XX");
            }

            // left padding
            for (unsigned j = 0; j < nm->GetConfig().PadBytes_; count++, j++)
            {
                if (count >= width)
                {
                    printf("\n");
                    count = 0;
                }
                printf(" %02X", *pages++);
            }

            // possible next pointer (zero it out)
            for (unsigned j = 0; j < sizeof(void*); count++, pages++, j++)
            {
                if (count >= width)
                {
                    printf("\n");
                    count = 0;
                }
                if (SHOWREAL)
                    printf(" %02X", *pages);
                else
                    printf(" %s", "XX");
            }

            // remaining bytes
            for (unsigned j = 0; j < nm->GetStats().ObjectSize_ - sizeof(void*); count++, j++)
            {
                if (count >= width)
                {
                    printf("\n");
                    count = 0;
                }
                printf(" %02X", *pages++);
            }

            // right pad bytes
            for (unsigned j = 0; j < nm->GetConfig().PadBytes_; count++, j++)
            {
                if (count >= width)
                {
                    printf("\n");



                    count = 0;
                }
                printf(" %02X", *pages++);
            }

        }
        printf("\n");
        DumpExternalHeaders(nm, realpage);
        printf("\n");

        pages = reinterpret_cast<const unsigned char*>((reinterpret_cast<const GenericObject*>(realpage))->Next);
        realpage = pages;
    }
}

void Test1(void)
{
    ObjectAllocator* oa;

    bool newdel = false;
    bool debug = false;
    unsigned padbytes = 0;
    OAConfig::HeaderBlockInfo header(OAConfig::hbExternal);
    unsigned alignment = 0;

    OAConfig config(newdel, 1, 1, debug, padbytes, header, alignment);
    oa = new ObjectAllocator(sizeof(Student), config);

    oa->Allocate();

    PrintConfig(oa);
    PrintCounts(oa);
    DumpPagesEx(oa, 32);

    delete oa;
}


int main(int argc, char** argv)
{
#ifdef _MSC_VER
    _CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);
    _CrtSetReportMode(_CRT_WARN, _CRTDBG_MODE_FILE);
    _CrtSetReportFile(_CRT_WARN, _CRTDBG_FILE_STDERR);
#endif

    int test=0;
    //std::cin >> test;
    if (argc > 1)
        test = std::atoi(argv[1]);
    if (argc > 2)
        SHOWADDRESS1 = std::atoi(argv[2]);
    if (argc > 3)
        SHOWADDRESS2 = std::atoi(argv[3]);
    if (argc > 4)
        SHOWREAL = std::atoi(argv[4]);
    if (argc > 5)
        SHOW_EXCEPTIONS = std::atoi(argv[5]);

    switch (test)
    {
    case 1:
        cout << "============================== Students..." << endl;
        DoStudents(0, false);
        cout << endl;
        break;
    case 2:
        cout << "============================== Students..." << endl;
        DoStudents(0, true);
        cout << endl;
        break;
    case 3:
        cout << "============================== Employees..." << endl;
        DoEmployees();
        cout << endl;
        break;
    case 4:
        cout << "============================== Test padding..." << endl;
        TestPadding(6);
        cout << endl;
        break;
    case 5:
        cout << "============================== Test padding..." << endl;
        TestPadding(10);
        cout << endl;
        break;
    case 6:
        cout << "============================== Test basic header blocks..." << endl;
        TestBasicHeaderBlocks();
        cout << endl;
        break;
    case 7:
        cout << "============================== Test extended header blocks(1)..." << endl;
        TestExtendedHeaderBlocks(1);
        cout << endl;
        break;
    case 8:
        cout << "============================== Test external header blocks..." << endl;
        TestExternalHeaderBlocks();
        cout << endl;
        break;
    case 9:
        cout << "============================== Test corruption..." << endl;
        TestCorruption();
        cout << endl;
        break;
    case 10:
        cout << "============================== Test using new/delete..." << endl;
        DisableOA();
        cout << endl;
        break;
    case 11:
        cout << "============================== Test leak..." << endl;
        TestLeak();
        cout << endl;
        break;
    case 12:
        cout << "============================== Test validate..." << endl;
        TestValidate();
        cout << endl;
        break;
    case 13:
        cout << "============================== Test free checking basic (stress)..." << endl;
        StressFreeChecking(OAConfig::HeaderBlockInfo(OAConfig::hbBasic));
        cout << endl;
        break;
    case 14:
        cout << "============================== Test free checking extended (stress)..." << endl;
        StressFreeChecking(OAConfig::HeaderBlockInfo(OAConfig::hbExtended, 1));
        cout << endl;
        break;
    case 15:
        cout << "============================== Test free checking external (stress)..." << endl;
        StressFreeChecking(OAConfig::HeaderBlockInfo(OAConfig::hbExternal));
        cout << endl;
        break;
    case 16:
        cout << "============================== Test stress using new/delete..." << endl;
        Stress(true);
        cout << endl;
        break;
    case 17:
        cout << "============================== Test stress using allocator..." << endl;
        Stress(false);
        cout << endl;
        break;
#if 1
    case 18:
        cout << "============================== Test alignment..." << endl;
        TestAlignment();
        cout << endl;
        break;
    case 19:
        cout << "============================== Test free empty pages 1..." << endl;
        TestFreeEmptyPages1();
        cout << endl;
        break;
    case 20:
        cout << "============================== Test free empty pages 2..." << endl;
        TestFreeEmptyPages2();
        cout << endl;
        break;
    case 21:
        cout << "============================== Test free empty pages 3..." << endl;
        TestFreeEmptyPages3();
        cout << endl;
        break;
#endif
    default:
        cout << "============================== Students..." << endl;
        DoStudents(0, false);
        cout << endl;
        cout << "============================== Students..." << endl;
        DoStudents(0, true);
        cout << endl;
        cout << "============================== Employees..." << endl;
        DoEmployees();
        cout << endl;
        cout << "============================== Test padding..." << endl;
        TestPadding(6);
        cout << endl;
        cout << "============================== Test padding..." << endl;
        TestPadding(10);
        cout << endl;
        cout << "============================== Test basic header blocks..." << endl;
        TestBasicHeaderBlocks();
        cout << endl;
        cout << "============================== Test extended header blocks(1)..." << endl;
        TestExtendedHeaderBlocks(1);
        cout << endl;
        cout << "============================== Test external header blocks..." << endl;
        TestExternalHeaderBlocks();
        cout << endl;
        cout << "============================== Test corruption..." << endl;
        TestCorruption();
        cout << endl;
        cout << "============================== Test using new/delete..." << endl;
        DisableOA();
        cout << endl;
        cout << "============================== Test leak..." << endl;
        TestLeak();
        cout << endl;
        cout << "============================== Test validate..." << endl;
        TestValidate();
        cout << endl;
        cout << "============================== Test free checking basic (stress)..." << endl;
        StressFreeChecking(OAConfig::HeaderBlockInfo(OAConfig::hbBasic));
        cout << endl;
        cout << "============================== Test free checking extended (stress)..." << endl;
        StressFreeChecking(OAConfig::HeaderBlockInfo(OAConfig::hbExtended, 1));
        cout << endl;
        cout << "============================== Test free checking external (stress)..." << endl;
        StressFreeChecking(OAConfig::HeaderBlockInfo(OAConfig::hbExternal));
        cout << endl;
        cout << "============================== Test stress using new/delete..." << endl;
        Stress(true);
        cout << endl;
        cout << "============================== Test stress using allocator..." << endl;
        Stress(false);
        cout << endl;
        cout << "============================== Test alignment..." << endl;
        TestAlignment();
        cout << endl;
        cout << "============================== Test free empty pages 1..." << endl;
        TestFreeEmptyPages1();
        cout << endl;
        cout << "============================== Test free empty pages 2..." << endl;
        TestFreeEmptyPages2();
        cout << endl;
        cout << "============================== Test free empty pages 3..." << endl;
        TestFreeEmptyPages3(); 
        cout << endl;
        break;
    }

    //OAConfig config(false, 16, 4, false);
    //ObjectAllocator a(sizeof(Student), config);
    //ObjectAllocator b(sizeof(Student), config);
      // These should fail to compile:
    //ObjectAllocator c(a);
    //a = b;


    return 0;
}