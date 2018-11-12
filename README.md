# **537make**

Programming assignment 3 for CS 537 at UW-Madison

Assignment details: http://pages.cs.wisc.edu/~bart/537/programs/program3.html

# **About**

This assignment focused on creating a simple version of the make command implementing the skills learned in class of creating new processes, loading new programs, and waiting for those processes to complete.

To do this, we first implemented a text parsing method that goes through the given Makefile and gathers the targets as well as its dependencies and sends them to our build specification representation method. This method creates a linked list of Buildspecs which holds the information of a target and its dependencies to represent the build specification given in the Makefile. As the Makefile continues to be parsed, targets, their dependencies, and their commands are gathered and added to this linked list using the various methods found in the build specification representation file. Also included is an int variable to keep track of where the target was found to give the user accurate error information if needed.

Once the Makefile has finished parsing and the build specifications have been built, we check for cycles in the build using the checkCycles algorithm which iterates through the linked list built prior and, as the method name implies, ensures there are no cycles in the current build. The program then either runs a build on a target file, if specified, or runs a build on the whole Makefile using a post-order traversal on a target and all its dependencies. While doing this, if a current build of a file already exists, its modify date is compared to the current build date to determine whether or not the dependency and the target that is dependent on that file need to be rebuilt or not. After that is determined, its commands are gathered, parsed into an array, and sent to runCommand to execute.

# **Usage**

537make \<target\> [-f \<name of Makefile\>]

or

537make [-f \<name of Makefile\>] \<target\>

or for makefiles named "Makefile" or "makefile"

537make

# **Edits for 3-1 (Valgrind)**

## **build_spec_graph.c:**

All these were for memory leak errors that may have become too large if the input file was too big. Essentially, we were creating a new Linked List with every command without freeing the past ones.

20: Changed return type for commandToArgs to Node\*.

25 - 29: Copied the result of strtok to a new char pointer so we can free it later.

33: Returned Node\* temp_command so we have access to free it later.

43 - 55: Moved lines from 29-38 in commandToArgs to here.

64 - 68: Freed both the 2-D char array and the Linked List.


## **linked_list.c:**

For 15, 16, and 50, the error was: "Uninitialized value created by heap allocation." This meant that we were not initializing some of the values we were allocating. By setting them to NULL, this fixed the issue. For lines 67 - 75, creating this freeList method removed some memory leak errors.

15 and 16: Set Node's values to NULL.

50: Set the Node->Next->Next value to NULL.

67 - 75: Added method freeList that recursively freed a given Linked List.

## **linked_list.h:**

10: Added the freeList method to the .h file.

## **text_parsing.c:**

For lines 137 - 156 the error was: "Uninitialized value created by heap allocation." Similar to in linked_list.c, we didn't initialize any of the char pointers. To fix this, we used memset to set all the values to 0. For lines 172 and 173 we simply freed the char pointers that we were using to read in the Makefile.

137 and 156: Used memset to initialize all the values in the char pointer to zero.

172 and 173: Freed buffer and line pointer.

## **Remaining Errors using Valgrind**

The remaining errors came from a leak of memory that was allocated but does not need to be freed before exit. It will be freed by the OS when it regains control.
