# Define any macros of for when you build and install the libraries
LibBuildMacros=MapValue=void*

LibInstallDir=/usr/lib
HeaderInstallDir=/usr/include

# this should ether be test or dist
# This varable effects what flags are used to build the final libraries.
BuildType = test # test or dist