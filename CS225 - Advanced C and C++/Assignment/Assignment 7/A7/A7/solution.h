
using std::placeholders::_1;
void print_file_names(const file_records& map)
{
	std::vector<file_name> names;
	std::transform(std::begin(map), std::end(map),
		std::back_inserter(names), split);
	std::for_each(std::begin(names), std::end(names), print_file_name);
}

size_t print_non_empty_files(const file_records& map)
{
	file_records temp;
	std::copy_if(std::begin(map), std::end(map),
		std::inserter(temp, std::end(temp)),
		std::bind(check_if_empty, _1, true));
	print_file_names(temp);
	return std::count_if(std::begin(map), std::end(map),
		std::bind(check_if_empty, _1, true));
}

size_t print_empty_files(const file_records& map)
{
	file_records temp;
	std::copy_if(std::begin(map), std::end(map),
		std::inserter(temp, std::end(temp)),
		std::bind(check_if_empty, _1, false));
	print_file_names(temp);
	return std::count_if(std::begin(map), std::end(map),
		std::bind(check_if_empty, _1, false));
}

std::tuple<file_records&> get_parameters(file_records& map)
{
	return std::forward_as_tuple(map);
}

void remove_empty(file_records& map)
{
	file_records temp;
	std::copy_if(std::begin(map), std::end(map),
		std::inserter(temp, std::end(temp)),
		std::bind(check_if_empty, _1, true));
	std::swap(map, temp);
}