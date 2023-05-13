////////////////////////////////////////////////////////////////////////////////
#ifndef Q_H
#define Q_H
////////////////////////////////////////////////////////////////////////////////

namespace CS330
{
  namespace divide
  {
    // Functions that are defined already
    void print(std::vector<int> & nums);
    bool check_neg_bef_pos(std::vector<int> & nums);

    // Functions that are to be implemented    
    int partition(std::vector<int> & nums, int l, int r);    
    int find_k(std::vector<int> & nums, int l, int r, int k);
    void neg_bef_pos(std::vector<int> & nums);
    int partition_pivot(std::vector<int> & nums, int l, int r, int pivot);
    void nuts_bolts_match(std::vector<int> & nuts, std::vector<int> & bolts, int l, int r);
  }
}

#endif
////////////////////////////////////////////////////////////////////////////////