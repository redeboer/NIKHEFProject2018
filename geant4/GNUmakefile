# -------------------------------------------------------------
# Author: Lizette Lamers
# Date: June 11th, 2018
# For NIKHEF Project 2018
# --------------------------------------------------------------

name := pbm
G4TARGET := $(name)
G4EXLIB := true

ifndef G4INSTALL
  G4INSTALL = ../../..
endif

.PHONY: all
all: lib bin

include $(G4INSTALL)/config/binmake.gmk

visclean:
	rm -f g4*.prim g4*.eps g4*.wrl
	rm -f .DAWN_*

