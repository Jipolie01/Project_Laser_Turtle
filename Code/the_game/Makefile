#############################################################################
#
# Project Makefile
#
# (c) Wouter van Ooijen (www.voti.nl) 2016
#
# This file is in the public domain.
# 
#############################################################################

SERIAL_PORT ?= COM3

# source files in this project (main.cpp is automatically assumed)
SOURCES := listenerpattern.cpp

# header files in this project
HEADERS := register_game_parameters_class.hpp run_game.hpp display_classes.hpp keypad_class.hpp application_logic_classes.hpp entity_classes.hpp game_time_classes.hpp listenerpattern.hpp speaker_classes.hpp rgb_led_classes.hpp send_ir_classes.hpp ir_receiver_classes.hpp

# other places to look for files for this project
SEARCH  := 

# set BMPTK to the next higher directory and defer to the Makefile.due
BMPTK := $(BMPTK)../
include $(BMPTK)Makefile.due
