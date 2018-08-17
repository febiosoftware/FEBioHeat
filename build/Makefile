FCDIR = $(dir $(CURDIR))
export PLGN = febioheat

# This needs to be set in order to find the febio .h files.
# FEBINC = $(HOME)/FEBio2
# Comment out the previous line and uncomment the next line if you are using the sdk on Linux.
# FEBINC = $(HOME)/FEBio2
# Or uncomment the next line if you are using OSX.
FEBINC = /Applications/FEBio2.8.2/sdk/include

# This needs to be set in order to find the febio library files.
# FEBLIB = $(HOME)/FEBio2/build/lib
# Comment out the previous line and uncomment the next line if you are using the sdk on Linux.
# FEBLIB = $(HOME)/FEBio2/build/lib
# Or uncomment the next line if you are using OSX.
FEBLIB = /Applications/FEBio2.8.2/sdk/lib

lnx64: PLAT = lnx64
lnx64d: PLAT = lnx64d
lnx32: PLAT = lnx32
osx: PLAT = osx
osxd: PLAT = osxd
gcc: PLAT = gcc
gcc64: PLAT = gcc64
lnx64clean: PLAT = lnx64
lnx64dclean: PLAT = lnx64
lnx32clean: PLAT = lnx32
osxclean: PLAT = osx
osxdclean: PLAT = osxd
gccclean: PLAT = gcc
gcc64clean: PLAT = gcc64

export PLAT
export FEBLIB
export FCDIR

# Assumes the Intel C++ compiler.  Use g++ for the gnu compiler and run "make gcc".
export CC = icpc
export FLG = -O3 -fPIC -stdc++11
export INTELROOT = $(subst /mkl,,$(MKLROOT))

# Can be removed if not using the Intel compiler.
export LNKFLG = -static-intel

export INC = -I$(FEBINC)
export INTEL_LIB = -L$(INTELROOT)/compiler/lib/

lnx64 lnx64d lnx32 gcc gcc64:
	( mkdir -p $(PLAT); cd $(PLAT); $(MAKE) -f ../$(PLGN).mk SFX="so" )

osx osxd:
	( mkdir -p $(PLAT); cd $(PLAT); $(MAKE) -f ../$(PLGN).mk SFX="dylib" )

lnx64clean lnx64dclean lnx32clean gccclean gcc64clean:
	( cd $(PLAT); $(MAKE) -f ../$(PLGN).mk SFX="so" clean )

osxclean osxdclean:
	( cd $(PLAT); $(MAKE) -f ../$(PLGN).mk SFX="dylib" clean )

.PHONY: lnx64 lnx64d lnx32 osx osxd gcc gcc64
