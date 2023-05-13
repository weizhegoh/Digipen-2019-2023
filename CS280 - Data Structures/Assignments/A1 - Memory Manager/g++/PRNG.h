//---------------------------------------------------------------------------
#ifndef PRNGH
#define PRNGH
//---------------------------------------------------------------------------

namespace Digipen
{
  namespace Utils
  {
    unsigned rand();                // returns a random 32-bit integer
    void srand(unsigned, unsigned); // seed the generator
    int Random(int low, int high);  // range
  }
}
#endif
