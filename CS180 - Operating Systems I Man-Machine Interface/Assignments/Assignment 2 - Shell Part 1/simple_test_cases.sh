# <- This is a comment
	# <- This is a comment too (it's a tab)
 # <- This is a comment too (space)
echo this should print #and this should too
echo but this # won't print
echo this         should             be           side      by           side

changeprompt #hello
echo <-- this should be #hello
changeprompt uShell

# ################################ #



# The above empty lines should be ignored, and shouldn't be taken a 'Invalid command'

# setvar simple cases to check:
setvar HAHA one
echo ${HAHA}        # Should print one
setvar HAHA two
echo ${HAHA}        # Should print two instead of onetwo
setvar hey
echo ${hey}         # Should print empty
echo ${${HAHA}}     # Should print ${two}

# Simple program invocation
ls.exe -l -lh -s

# Should be able to compile using g++
# for example, just create a simple hello world program that takes in two arugements and print them
g++.exe helloworld.cpp helloworld.h
./a.exe this should be printed

# The program should return 555 instead of 0 or 5
# exit 555

# Make sure the beneath works too and not go into a infinite loop
exit
