#############################################################################
#
# Project Makefile
#
# (c) Wouter van Ooijen (www.voti.nl) 2016
#
# This file is in the public domain.
# 
#############################################################################

SERIAL_PORT ?= COM2

# source files in this project (main.cpp is automatically assumed)
SOURCES :=

# header files in this project
HEADERS := application_logic_classes.hpp entity_classes.hpp send_ir_classes.hpp test_class.hpp

# other places to look for files for this project
SEARCH  := 

# set BMPTK to the next higher directory and defer to the Makefile.due
BMPTK := $(BMPTK)../
include $(BMPTK)Makefile.due
