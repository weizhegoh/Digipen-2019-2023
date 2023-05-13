#ifndef _CONSTRUCTOR_H_
#define _CONSTRUCTOR_H_

#include <iostream>
#include <vector>
#include <string>

#include "IFormatterFactory.h"
#include "ISavable.h"

// Produces a list of available formatter names.
std::vector<std::string> get_options();

// Constructs a formatter based on the index of its name from get_options().
IFormatterFactory* construct(int selected_option);

// Saves an object to a stream.
void save(ISavable& savable, std::ostream& stream);

#endif // _CONSTRUCTOR_H_
