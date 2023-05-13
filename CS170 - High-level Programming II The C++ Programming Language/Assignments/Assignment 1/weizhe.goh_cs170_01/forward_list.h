#ifndef FORWARD_LIST_H
#define FORWARD_LIST_H

/* Forward declarations of encapsulating data types. */
struct forward_list;

/* Creates a new forward list. */
forward_list* create_list();

/* Destroys an existing forward list. */
void destroy_list(forward_list* const that);

/* Returns the count of the elements in a forward list. */
unsigned int count_list(const forward_list* const that);

/* Adds a new value to the beginning of a forward list. */
void push_front_list(forward_list* const that, int value);

/* Adds a new value to the end of a forward list. */
void push_back_list(forward_list* const that, int value);

/* Prints the contents of a forward list. */
void print_list(const forward_list* const that);

#endif
