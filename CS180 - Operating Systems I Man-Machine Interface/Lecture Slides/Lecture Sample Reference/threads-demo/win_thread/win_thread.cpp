// win_thread.cpp
// -- Win32 threads: single simple thread
// cs180 1/08

#include <iostream>
#include <cstdlib>
#include <ctime>
#include <windows.h>
using namespace std;

char *table[] = { "A nut for a jar of tuna.",
                  "Rats live on no evil star.",
                  "I prefer pi.",
                  "Gary knits a stinky rag.",
                  "Nate bit a Tibetan.",
                  "Sleep on no peels."
                };
const int table_size = 6;


DWORD WINAPI anythingreally(LPVOID) {
  srand(unsigned(time(0)));
  int max = 1 + rand()%10;
  for (int i=0; i < max; ++i) {
    cout << table[rand()%table_size] << endl;
    Sleep(500);
  }
  return 0;
}


int main(void) {
  HANDLE thread_handle = CreateThread(0,0,anythingreally,0,0,0);
  //WaitForSingleObject(thread_handle,INFINITE);
  //CloseHandle(thread_handle);
  return 0;
}
