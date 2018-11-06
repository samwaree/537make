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
