//---------------------------------------------------------------------------
#ifndef PRNG_H
#define PRNG_H
//---------------------------------------------------------------------------

namespace Digipen
{
  namespace Utils
  {
    unsigned rand(void);              // returns a random 32-bit integer
    void srand(unsigned, unsigned);   // seed the generator
    int Random(int low, int high);    // range
  }
}
#endif
