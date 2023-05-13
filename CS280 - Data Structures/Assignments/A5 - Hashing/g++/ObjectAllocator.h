//---------------------------------------------------------------------------
#ifndef OBJECTALLOCATORH
#define OBJECTALLOCATORH
//---------------------------------------------------------------------------

#include <string>

// If the client doesn't specify these:
static const int DEFAULT_OBJECTS_PER_PAGE = 4;  
static const int DEFAULT_MAX_PAGES = 3;

class OAException
{
  public:
      // Possible exception codes
    enum OA_EXCEPTION 
    {
      E_NO_MEMORY,      // out of physical memory (operator new fails)
      E_NO_PAGES,       // out of logical memory (max pages has been reached)
      E_BAD_BOUNDARY,   // block address is on a page, but not on any block-boundary
      E_MULTIPLE_FREE,  // block has already been freed
      E_CORRUPTED_BLOCK // block has been corrupted (pad bytes have been overwritten)
    };

    OAException(OA_EXCEPTION ErrCode, const std::string& Message) : error_code_(ErrCode), message_(Message) {};

    virtual ~OAException() {
    }

    OA_EXCEPTION code(void) const { 
      return error_code_; 
    }

    virtual const char *what(void) const {
      return message_.c_str();
    }
  private:  
    OA_EXCEPTION error_code_;
    std::string message_;
};

// ObjectAllocator configuration parameters
struct OAConfig
{
  static const size_t BASIC_HEADER_SIZE = sizeof(unsigned) + 1; // allocation number + flags
  static const size_t EXTERNAL_HEADER_SIZE = sizeof(void*);     // just a pointer

  enum HBLOCK_TYPE{hbNone, hbBasic, hbExtended, hbExternal};
  struct HeaderBlockInfo
  {
    HBLOCK_TYPE type_;
    size_t size_;
    size_t additional_;
    HeaderBlockInfo(HBLOCK_TYPE type = hbNone, unsigned additional = 0) : type_(type), size_(0), additional_(additional)
    {
      if (type_ == hbBasic)
        size_ = BASIC_HEADER_SIZE;
      else if (type_ == hbExtended) // alloc # + use counter + flag byte + user-defined
        size_ = sizeof(unsigned int) + sizeof(unsigned short) + sizeof(char) + additional_;
      else if (type_ == hbExternal)
        size_ = EXTERNAL_HEADER_SIZE;
    };
  };

  OAConfig(bool UseCPPMemManager = false,
           unsigned ObjectsPerPage = DEFAULT_OBJECTS_PER_PAGE, 
           unsigned MaxPages = DEFAULT_MAX_PAGES, 
           bool DebugOn = false, 
           unsigned PadBytes = 0,
           const HeaderBlockInfo &HBInfo = HeaderBlockInfo(),
           unsigned Alignment = 0) : UseCPPMemManager_(UseCPPMemManager),
                                     ObjectsPerPage_(ObjectsPerPage), 
                                     MaxPages_(MaxPages), 
                                     DebugOn_(DebugOn), 
                                     PadBytes_(PadBytes),
                                     HBlockInfo_(HBInfo),
                                     Alignment_(Alignment)
  {
    HBlockInfo_ = HBInfo;
    LeftAlignSize_ = 0;  
    InterAlignSize_ = 0;
  }

  bool UseCPPMemManager_;   // by-pass the functionality of the OA and use new/delete
  unsigned ObjectsPerPage_; // number of objects on each page
  unsigned MaxPages_;       // maximum number of pages the OA can allocate (0=unlimited)
  bool DebugOn_;            // enable/disable debugging code (signatures, checks, etc.)
  unsigned PadBytes_;          // size of the left/right padding for each block
  HeaderBlockInfo HBlockInfo_; // size of the header for each block (0=no headers)
  unsigned Alignment_;      // address alignment of each block

  unsigned LeftAlignSize_;  // number of alignment bytes required to align first block
  unsigned InterAlignSize_; // number of alignment bytes required between remaining blocks
};

struct MemBlockInfo
{
  bool in_use;        // Is the block free or in use?
  char *label;        // A dynamically allocated NUL-terminated string
  unsigned alloc_num; // The allocation number (count) of this block
};

class ObjectAllocator
{
  public:
    ObjectAllocator(size_t ObjectSize, const OAConfig& config);
    void *Allocate(const char *label = 0);
    void Free(void *Object);
  private:
    OAConfig Config_;
    size_t ObjectSize_;
};

#endif
