/* Start Header
*****************************************************************/
/*!
\file   solution.h
\author Goh Wei Zhe, weizhe.goh, 440000119
\par    email: weizhe.goh\@digipen.edu
\date   December 6, 2020

\brief  Practice use of STL templates and algorithms.
        Practice working with STL containers.
        Practice use of callable objects and predicates in processing
        data within containers.

Copyright (C) 2020 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the
prior written consent of DigiPen Institute of Technology is prohibited.
*/
/* End Header
*******************************************************************/

using std::placeholders::_1;
void print_file_names(const file_records& map)
{
   std::vector<file_name> name;
   
   std::transform(std::begin(map),std::end(map),std::back_inserter(name),split);
   std::for_each(std::begin(name), std::end(name), print_file_name);
}

size_t print_non_empty_files(const file_records& map)
{
    file_records records;
	
    std::copy_if(std::begin(map), std::end(map), 
				 std::inserter(records, std::end(records)), 
				 std::bind(check_if_empty, _1, true));
                 
	print_file_names(records);
	return std::count_if(std::begin(map), std::end(map), 
						 std::bind(check_if_empty, _1, true));
}

size_t print_empty_files(const file_records& map)
{
    file_records records;
	
    std::copy_if(std::begin(map), std::end(map), 
				 std::inserter(records, std::end(records)), 
				 std::bind(check_if_empty, _1, false));
	
    print_file_names(records);
	
    return std::count_if(std::begin(map), std::end(map), 
						 std::bind(check_if_empty, _1, false));
}

std::tuple<file_records&> get_parameters(file_records& map)
{
	return std::forward_as_tuple(map);
}

void remove_empty(file_records& map)
{
    file_records records;
    
	std::copy_if(std::begin(map), std::end(map), 
				 std::inserter(records, std::end(records)), 
				 std::bind(check_if_empty, _1, true));
	std::swap(map, records);
}