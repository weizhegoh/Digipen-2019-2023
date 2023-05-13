/******************************************************************************/
/*!
\file:		ObjectAllocator.cpp
\author:    Goh Wei Zhe, weizhe.goh, 440000119
\par email: weizhe.goh\@digipen.edu
\date:		January 21, 2021
\brief		To learn about and understand data structures and their 
			interfaces.
            
Copyright (C) 2021 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the
prior written consent of DigiPen Institute of Technology is prohibited.
*/
/******************************************************************************/

#include "ObjectAllocator.h"
#include <cstring>

#define u_char unsigned char 
/******************************************************************************/
/*!
\fn     ObjectAllocator::ObjectAllocator(size_t ObjectSize, 
        const OAConfig& config)

\brief  Constructor to calculate and initialise class variables

\param  ObjectSize - the size of the object allocateed 
\param  config - contains the OAConfig details for object allocator 
*/
/******************************************************************************/
ObjectAllocator::ObjectAllocator(size_t ObjectSize, const OAConfig& config)
	:OAConfig_{ config }, OAStats_{}, PageList_{ nullptr }, FreeList_{nullptr},
    midBlockSize{0}, offset{0}
{ 
    OAStats_.ObjectSize_ = ObjectSize;
    OAStats_.PageSize_ = getPageSize();
   
    //calculate size of block  
    midBlockSize = OAConfig_.HBlockInfo_.size_ + OAConfig_.PadBytes_ * 2 + 
    OAStats_.ObjectSize_ + OAConfig_.InterAlignSize_;

    //calculate page size 
    OAStats_.PageSize_ = sizeof(void*) + OAConfig_.LeftAlignSize_ + 
    (OAConfig_.ObjectsPerPage_ * midBlockSize) - OAConfig_.InterAlignSize_;
    
    //calculate offset to start at s
    //offset = P + al + h + s 
    offset = sizeof(void*) + OAConfig_.LeftAlignSize_ +
            OAConfig_.HBlockInfo_.size_ + OAConfig_.PadBytes_;

     //if memory is allocated 
    if(OAConfig_.UseCPPMemManager_)
        return;
        
    //Page Allocation 
    addPage();
}

/******************************************************************************/
/*!     
\fn     ObjectAllocator::~ObjectAllocator()
\brief  Destructor for ObjectAllocator. Remove pages.
*/
/******************************************************************************/
ObjectAllocator::~ObjectAllocator()
{
    u_char* page;
    
    while (PageList_ != NULL)
    {
        page = reinterpret_cast<u_char*>(PageList_);
        
        PageList_ = PageList_->Next;
        delete [] page;
    }
}

/******************************************************************************/
/*!
\fn     ObjectAllocator::Allocate(const char* label)

\brief  Allocates memory for page and return to client. If UseCPPMemManager is 
         true, bypass OA and use new. 

\param  label - the size of the object allocateed 

\return A pointer to a block of object 
*/
/******************************************************************************/
void* ObjectAllocator::Allocate(const char* label)
{ 
    if(OAConfig_.UseCPPMemManager_)
    {        
        try
        {
            u_char* newObj = new u_char[OAStats_.ObjectSize_];
            updateOAStats();
            
            return newObj;
        }
        catch (std::bad_alloc& )  
        {
            throw OAException(OAException::E_NO_MEMORY, "Not enough memory!");
        }
    }

    //Freelist is empty, no page allocated 
    if(FreeList_ == nullptr)
    {
        if(OAConfig_.MaxPages_ == 0 || OAStats_.PagesInUse_<OAConfig_.MaxPages_)
        {     
            addPage();
        }
        else 
        {
            throw OAException(OAException::E_NO_PAGES, "No available page!"); 
        }
    }
 
    GenericObject* object = FreeList_;
    FreeList_ = FreeList_->Next;
    
    if(OAConfig_.DebugOn_)
    {
        memset(object, ALLOCATED_PATTERN, OAStats_.ObjectSize_);
    }
    
    updateOAStats();
      
   switch(OAConfig_.HBlockInfo_.type_)
   {
       case OAConfig::hbBasic:
       {
           //Start of header 
           u_char* headerStart = reinterpret_cast<u_char*>(object) - 
            OAConfig_.PadBytes_ - OAConfig_.HBlockInfo_.size_;
            
           unsigned* allocationNumber =reinterpret_cast<unsigned*>(headerStart);
           *allocationNumber = OAStats_.Allocations_;
           
           //Start of flag 
           u_char* flag = reinterpret_cast<u_char*>(headerStart + 
            sizeof(unsigned));
            
           *flag = true;
           
           break;
       }
       case OAConfig::hbExtended:
       {
           //Start of Flag
           u_char* flagStart = reinterpret_cast<u_char*>(object) - 
           OAConfig_.PadBytes_ - sizeof(u_char);
            
           *flagStart = true;
            
           //Start of allocation number 
           u_char* allocNumStart = flagStart - sizeof(unsigned);
           
           unsigned *allocationNumber = 
           reinterpret_cast<unsigned*>(allocNumStart);
           
           *allocationNumber = OAStats_.Allocations_;
            
           //Start of use-count 
           u_char* useCountStart = allocNumStart - sizeof(unsigned short);
            
           unsigned short* useCount = 
           reinterpret_cast<unsigned short*>(useCountStart);
           
           ++(*useCount);
           
           break;
       }
       case OAConfig::hbExternal:
       {
           //one unsigned for allocation number, one flag to determine on off 
           u_char* headerStart = reinterpret_cast<u_char*>(object) - 
           OAConfig_.PadBytes_ - OAConfig_.HBlockInfo_.size_;
           
           MemBlockInfo** memPtr = 
           reinterpret_cast <MemBlockInfo**>(headerStart);
           
           try 
           {
               *memPtr = new MemBlockInfo;
               
               (*memPtr)->in_use = true;
               (*memPtr)->label = nullptr;
               (*memPtr)->alloc_num = OAStats_.Allocations_;
               
               if(label)
               {
                   try
                   {
                       (*memPtr)->label = new char[strlen(label) + 1];
                   }
                   catch (std::bad_alloc& )
                   {
                       throw OAException(OAException::E_NO_MEMORY,
                       "Out of memory!");
                   }                       
                   
                   strcpy((*memPtr)->label, label);
               }
           }
           catch(std::bad_alloc& )
           {
               throw OAException(OAException::E_NO_MEMORY, "Out of memory!");
           }
           break;
       }
       
       default: break;
   }
        
	return object;    
}

/******************************************************************************/
/*!
\fn     void ObjectAllocator::Free(void* Object)

\brief  Free block memory from Allocate() If UseCPPMemManager is true, bypass 
         OA and use delete. If debug is on, perform check to see if block is 
         corrupted and within same boundaries. 

\param  Object - A pointer to object allocated from Allocate().
*/
/******************************************************************************/
void ObjectAllocator::Free(void* Object)
{
    if(OAConfig_.UseCPPMemManager_)
    {
        delete[] reinterpret_cast<u_char*>(Object);
        
        --OAStats_.ObjectsInUse_;
        ++OAStats_.Deallocations_;
        ++OAStats_.FreeObjects_;
        
        return;
    }
    
    GenericObject* page = PageList_;
    u_char* objAddress = reinterpret_cast<u_char*>(Object);

    //Only perform check when debugging 
    if(OAConfig_.DebugOn_)
    {
        bool onBound = false;
        bool corrupted = false;
        bool alreadyFreed = true;
        
        while(page!= NULL)
        {
            if( reinterpret_cast<u_char*>(page) < objAddress &&
            objAddress < reinterpret_cast<u_char*>(page) + OAStats_.PageSize_)
            {
                u_char* blockStart = reinterpret_cast<u_char*>(page) + offset;
                
                size_t separation = midBlockSize;
                
                if((objAddress - blockStart) % separation == 0)
                {
                    onBound = true;
                }
            }
            
            page = page->Next;
        }
        
        if(onBound == false)
        {
            throw(OAException(OAException::E_BAD_BOUNDARY, 
            "Object not on a block boundary"));
        }
        else 
        {
            //check data block 
            for(unsigned i = sizeof(u_char*); i < OAStats_.ObjectSize_; ++i)
            {
                if(objAddress[i] != FREED_PATTERN)
                {
                    alreadyFreed = false;
                    break;
                }
            }
        
            //Check padding blocks
            u_char* leftPad = objAddress - OAConfig_.PadBytes_;
            u_char* rightPad = objAddress + OAStats_.ObjectSize_;
            
            for(unsigned int i = 0; i < OAConfig_.PadBytes_; ++i)
            {
                if(leftPad[i] != PAD_PATTERN || rightPad[i] != PAD_PATTERN)
                {
                    corrupted = true;
                    break;
                }
            }
        }
        
        if(alreadyFreed == true)
        {
            throw(OAException(OAException::E_MULTIPLE_FREE, 
            "Block already freed once"));
        }
        else if (corrupted == true)
        {
            throw(OAException(OAException::E_CORRUPTED_BLOCK, 
            "Block is corrupted"));
        }
    }    
    
    u_char* headerStart = reinterpret_cast<u_char*>(objAddress) - 
    OAConfig_.PadBytes_ - OAConfig_.HBlockInfo_.size_;
    
    switch(OAConfig_.HBlockInfo_.type_)
    {
        case OAConfig::hbBasic:
        {
            memset(headerStart, 0, OAConfig::BASIC_HEADER_SIZE);
            break;
        }
        case OAConfig::hbExtended:
        {
            memset(headerStart + OAConfig_.HBlockInfo_.additional_ + 
            sizeof(unsigned short), 0, OAConfig::BASIC_HEADER_SIZE);
            
            break;
        }
        case OAConfig::hbExternal:
        {
            MemBlockInfo** memPtr = 
            reinterpret_cast<MemBlockInfo**>(headerStart);
            
            delete[] (*memPtr)->label;
            delete *memPtr;
            
            memset(memPtr, 0, OAConfig_.HBlockInfo_.size_);
        }
        default: break;
    }
    
    if(OAConfig_.DebugOn_)
    {
        memset(Object, FREED_PATTERN, OAStats_.ObjectSize_);
    }
    
    GenericObject* object = reinterpret_cast<GenericObject*>(Object);
    object->Next = FreeList_;
    FreeList_ = object;
    
    --OAStats_.ObjectsInUse_;
    ++OAStats_.Deallocations_;
    ++OAStats_.FreeObjects_;
}

/******************************************************************************/
/*!
\fn     unsigned ObjectAllocator::DumpMemoryInUse(DUMPCALLBACK fn) const

\brief  Run through pages in OA and calls the given function if the block 
         data are active.
        
\param  fn - A function to call for each active object. 

\return Number of active objects 
*/
/******************************************************************************/
unsigned ObjectAllocator::DumpMemoryInUse(DUMPCALLBACK fn) const
{
    GenericObject* page = PageList_;
    
    u_char* block;
  
    while (page)
    {
        block = reinterpret_cast<u_char*>(page) + offset;
          
        for (unsigned i = 0; i < OAConfig_.ObjectsPerPage_; ++i)
        {
          if (*block == ALLOCATED_PATTERN)
          {
            fn(block, OAStats_.ObjectSize_);
          }
          if (i < OAConfig_.ObjectsPerPage_ - 1)
          {
            block += midBlockSize;    
          }
        }
        
        page = page->Next;
    }

    return OAStats_.ObjectsInUse_;
}

/******************************************************************************/
/*!
\fn     unsigned ObjectAllocator::ValidatePages(VALIDATECALLBACK fn) const

\brief  Run through each pages to check if each data block is corrupted or not.  

\param  fn - A function to call once a data block found corrupted.

\return Number of corrupted data blocks. 
*/
/******************************************************************************/
unsigned ObjectAllocator::ValidatePages(VALIDATECALLBACK fn) const
{   
  if (OAConfig_.PadBytes_ == 0)
  {
    return 0;
  }

  unsigned corruptedBlocks = 0;
  GenericObject* page = PageList_;
  
  u_char* block;
  u_char* leftPad;
  u_char* rightPad;

  while (page)
  {
    block = reinterpret_cast<u_char*>(page) + offset;
    
    for (unsigned i = 0; i < OAConfig_.ObjectsPerPage_; ++i)
    {
      // Check padding blocks
      leftPad = block - OAConfig_.PadBytes_;
      rightPad = block + OAStats_.ObjectSize_;
      
      for (unsigned j = 0; j < OAConfig_.PadBytes_; ++j)
      {
        if (leftPad[j] != PAD_PATTERN || rightPad[j] != PAD_PATTERN)
        {
          fn(block, OAStats_.ObjectSize_);
          ++corruptedBlocks;  
          break;
        }
      }

      // Prevents pointer arithmetic overrun 
      if (i < OAConfig_.ObjectsPerPage_ - 1)
      {
        block += midBlockSize;
      }  
    }
    page = page->Next;
  }

  return corruptedBlocks;
}

/******************************************************************************/
/*!
\fn     unsigned ObjectAllocator::FreeEmptyPages()

\brief  Run through each page to check if page is empty. If empty, returns
         memory back to OS. 

\return Number of page freed. 
*/
/******************************************************************************/
unsigned ObjectAllocator::FreeEmptyPages()
{   
    if(PageList_ == nullptr)
        return 0;
        
    unsigned emptyPages = 0;
    
    GenericObject* temp = PageList_;
    GenericObject* previous = nullptr;
    
    //If head node holds a key or multiple key
    while(temp != nullptr && isPageEmpty(temp))
    {
        //Change head 
        PageList_ = temp->Next;
        freePage(temp);
        
        //Change temp 
        temp = PageList_;
        ++emptyPages;
    }
    
    //Delete occurance other than head 
    while(temp != nullptr)
    {
      //Search key to be deleted
      //Keep track of previous node as we need to change previous->Next
      while(temp != nullptr && !isPageEmpty(temp))
      {
        previous = temp;
        temp = temp->Next;
      }
      
      //If key not present in linked list 
      if(temp == nullptr)
        return emptyPages;
        
      //Unlink node from linked list 
      previous->Next = temp->Next;
      freePage(temp);
      
      //Update temp for next loop
      temp = previous->Next;
      ++emptyPages;
    }
    return emptyPages;
}

/******************************************************************************/
/*!
\fn     void ObjectAllocator::SetDebugState(bool State)

\brief  Set current OA to the given debug state. 
         
\param  State - Debug on (True), Debug off (False)

*/
/******************************************************************************/
void ObjectAllocator::SetDebugState(bool State)
{
	OAConfig_.DebugOn_ = State;
}	

/******************************************************************************/
/*!
\fn     const void* ObjectAllocator::GetFreeList() const

\brief  Returns the free list, a linked list to determine which block of memory
         are free.
         
\return The free list. 

*/
/******************************************************************************/
const void* ObjectAllocator::GetFreeList() const
{
	return FreeList_;
}

/******************************************************************************/
/*!
\fn     const void* ObjectAllocator::GetPageList() const

\brief  Returns the page list, a linked list that chains all the page. 
         
\return The page list. 

*/
/******************************************************************************/
const void* ObjectAllocator::GetPageList() const
{
	return PageList_;
}	

/******************************************************************************/
/*!
\fn     OAConfig ObjectAllocator::GetConfig() const

\brief  Returns a copy of the current configuration. 

\return The copy of the current configuration

*/
/******************************************************************************/
OAConfig ObjectAllocator::GetConfig() const
{
	return OAConfig_;
}

/******************************************************************************/
/*!
\fn     OAStats ObjectAllocator::GetStats() const

\brief  Returns a copy of the current stats. 

\return The copy of the current stats

*/
/******************************************************************************/
OAStats ObjectAllocator::GetStats() const
{
	return OAStats_;
}

/******************************************************************************/
/*!
\fn     void ObjectAllocator::addPage()

\brief  Allocate new page. Checks if it has hit the maximum page allocated 
         allowed to allocate. 
*/
/******************************************************************************/
void ObjectAllocator::addPage()
{
    //Check if maximum pages is reached 
    if(OAConfig_.MaxPages_ && (OAStats_.PagesInUse_ == OAConfig_.MaxPages_))
    {
        throw OAException(OAException::E_NO_PAGES, "maximum page allocated");
    }
    
    GenericObject* newPage;
        
    try 
    {
        //allocate memory for new page 
        newPage = 
        reinterpret_cast<GenericObject*>(new u_char[OAStats_.PageSize_]);
        
        ++OAStats_.PagesInUse_;
    }
    catch(std::bad_alloc &)
    {
        throw OAException(OAException::E_NO_PAGES, 
        "Not enough memory to add new page");
    }
    
    //link PageList_ 
    newPage->Next = PageList_;
    PageList_ = newPage;
    
    //Putting object onto free list     
    unsigned char* temp = reinterpret_cast<unsigned char*>(newPage);
    temp += sizeof(void*);
    
    memset(temp, ALIGN_PATTERN, OAConfig_.LeftAlignSize_); 
    
    temp += (offset - sizeof(void*));
    
    for(unsigned int i = 0; i < OAConfig_.ObjectsPerPage_; ++i)
    {                
        ++OAStats_.FreeObjects_;
        
        if(OAConfig_.DebugOn_)
        {
            memset(temp - OAConfig_.PadBytes_ - OAConfig_.HBlockInfo_.size_, 0,
            OAConfig_.HBlockInfo_.size_);

            memset(temp, UNALLOCATED_PATTERN, OAStats_.ObjectSize_);
            memset(temp - OAConfig_.PadBytes_ ,PAD_PATTERN,OAConfig_.PadBytes_);
            memset(temp + OAStats_.ObjectSize_,PAD_PATTERN,OAConfig_.PadBytes_);
        }
        
         if(i != OAConfig_.ObjectsPerPage_ - 1)
        {
           memset(reinterpret_cast<unsigned char*>(temp) + OAStats_.ObjectSize_ 
           + OAConfig_.PadBytes_, ALIGN_PATTERN, OAConfig_.InterAlignSize_);            
        }
        
        reinterpret_cast<GenericObject*>(temp)->Next = FreeList_;
        FreeList_ = reinterpret_cast<GenericObject*>(temp);
        
        temp += OAStats_.ObjectSize_ + 2* OAConfig_.PadBytes_ + 
        OAConfig_.HBlockInfo_.size_ + OAConfig_.InterAlignSize_;
    }
}

/******************************************************************************/
/*!
\fn     void ObjectAllocator::updateOAStats()

\brief  Increment stats every time an object is allocated. 
*/
/******************************************************************************/
void ObjectAllocator::updateOAStats()
{
    ++OAStats_.Allocations_;
    ++OAStats_.ObjectsInUse_;
    
    if(OAStats_.ObjectsInUse_ > OAStats_.MostObjects_)
        OAStats_.MostObjects_ = OAStats_.ObjectsInUse_;
    
    --OAStats_.FreeObjects_;
}

/******************************************************************************/
/*!
\fn     size_t ObjectAllocator::getPageSize()

\brief  Returns the page size

\return The page size
*/
/******************************************************************************/
size_t ObjectAllocator::getPageSize()
{
    //check if left alignment needs to be calculated 
    if(OAConfig_.Alignment_ > 1)
    {
        //calculate byte needed for left alignment 
        size_t left_bytes = sizeof(void*) + OAConfig_.HBlockInfo_.size_ + 
        OAConfig_.PadBytes_;
        
        OAConfig_.LeftAlignSize_ = OAConfig_.Alignment_ - 
        (static_cast<unsigned>(left_bytes) % OAConfig_.Alignment_);
        
        //calculate byte needed for inter alignment 
        size_t inter_bytes = OAStats_.ObjectSize_ + OAConfig_.HBlockInfo_.size_ 
        + OAConfig_.PadBytes_ * 2;
        
        OAConfig_.InterAlignSize_ = OAConfig_.Alignment_ - 
        (static_cast<unsigned>(inter_bytes) % OAConfig_.Alignment_);
    }
    
    size_t size = 0;
    
    //left alignment size 
    //nextP + al (left alignment). Only first block has left alignment 
    size = sizeof(void*) + OAConfig_.LeftAlignSize_;
    
    //InterAlignSize_
    size+= OAConfig_.ObjectsPerPage_* (OAConfig_.HBlockInfo_.size_ + 
    OAConfig_.PadBytes_ + OAStats_.ObjectSize_ + OAConfig_.PadBytes_);
    
    size += (OAConfig_.ObjectsPerPage_ - 1) * OAConfig_.InterAlignSize_;
    
    return size;
}

/******************************************************************************/
/*!
\fn     bool ObjectAllocator::isPageEmpty(GenericObject* page) const 

\brief  Given a page, check if the page is empty by running through the
         freelist and check if there are free items in the page. 

\param page - A page to check if it is empty 

\return Return true is page is empty, otherwise, return false. 
*/
/******************************************************************************/
bool ObjectAllocator::isPageEmpty(GenericObject* page) const 
{
    GenericObject* freeList = FreeList_;
     
    unsigned freeWithinPage = 0;
        
    while(freeList)
    {            
        if(inPage(page, reinterpret_cast<u_char*>(freeList)))
        {
            ++freeWithinPage;
            
            if(freeWithinPage >= OAConfig_.ObjectsPerPage_)
            {
                return true;
            }
        }
        freeList = freeList->Next;
    }
    return false;
}

/******************************************************************************/
/*!
\fn     void ObjectAllocator::freePage(GenericObject* temp) 

\brief  Given a page, free memory by returning it to OS. Removes all data block
         from the freelist, then decrement the stat counter.

\param temp - A page to free.
*/
/******************************************************************************/
void ObjectAllocator::freePage(GenericObject* temp)
{
    removePgObjInFreeList(temp);
    delete[] reinterpret_cast<u_char*>(temp);
    --OAStats_.PagesInUse_;
}

void ObjectAllocator::removePgObjInFreeList(GenericObject* pageAddress)
{
    GenericObject* temp = FreeList_;
    GenericObject* previous = nullptr;
    
    //If head node itself hold key or multiple key 
    while(temp!= nullptr && inPage(pageAddress,reinterpret_cast<u_char*>(temp)))
    {
        //Change head
        FreeList_ = temp->Next;
        
        //Change temp 
        temp = FreeList_;
        --OAStats_.FreeObjects_;
    }
    
    //Delete occurence other than head 
    while(temp != NULL)
    {
        //Search key to be deleted 
        //Keep track of previous node, need to change previous->Next 
        while(temp != nullptr &&
        !inPage(pageAddress, reinterpret_cast<u_char*>(temp)))
        {
            previous = temp;
            temp = temp->Next;
        }
        
        //If key not present in linked list 
        if(temp == nullptr) return;
        
        //unlink node from linked list 
        previous->Next = temp->Next;
        
        --OAStats_.FreeObjects_;
        
        //Update temp for next loop
        temp = previous->Next;
    }
}

/******************************************************************************/
/*!
\fn     bool ObjectAllocator::inPage(GenericObject* pageAddress,
        u_char* address) const 

\brief  Given a page address and any address, check if address is in the page. 

\param  pageAddress - The pointer to the page.
\param  address - The address to check if it is in range. 

\return Returns true if in page, otherwise, return false. 
*/
/******************************************************************************/
bool ObjectAllocator::inPage(GenericObject* pageAddress, u_char* address) const 
{
    return (address >= reinterpret_cast<u_char*>(pageAddress) &&
    address < reinterpret_cast<u_char*>(pageAddress) + OAStats_.PageSize_);
}