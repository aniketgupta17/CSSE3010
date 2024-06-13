########################################################################
# LIBRARY SOURCES - MUST BE IN THE SAME FOLDER as main.c (DO NOT CHANGE)
########################################################################

# Set mylib folder path.
# Do not change the MYLIB_PATH variable name.
# ONLY use relative file paths that start with $(SOURCELIB_ROOT)../
# DO NOT USE absolute file paths (e.g. /home/users/myuser/mydir)
# e.g. MYLIBPATH=$(SOURCELIB_ROOT)/../mylib
MYLIB_PATH=$(SOURCELIB_ROOT)/../repo/mylib

# Set folder path with header files to include.
# ONLY use relative file paths that start with $(SOURCELIB_ROOT)../
# DO NOT USE absolute file paths (e.g. /home/users/myuser/mydir)
CFLAGS += -I$(MYLIB_PATH)

# List all c file locations that must be included (use space as separator
# e.g. LIBSRCS += path_to/file1.c path_to/file2.c)
# ONLY use relative file paths that start with $(SOURCELIB_ROOT)../
# DO NOT USE absolute file paths (e.g. /home/users/myuser/mydir)


LIBSRCS += $(MYLIB_PATH)/s4824063_hamming.c
LIBSRCS += $(MYLIB_PATH)/s4824063_console.c
LIBSRCS += $(MYLIB_PATH)/s4824063_mfs_led.c
LIBSRCS += $(MYLIB_PATH)/s4824063_lta1000g.c
LIBSRCS += $(MYLIB_PATH)/s4824063_mfs_pb.c
LIBSRCS += $(MYLIB_PATH)/s4824063_sysmon.c
LIBSRCS += $(MYLIB_PATH)/s4824063_rgb.c
LIBSRCS += $(MYLIB_PATH)/s4824063_rcmcont.c
LIBSRCS += $(MYLIB_PATH)/s4824063_txradio.c
LIBSRCS += $(MYLIB_PATH)/s4824063_board_pb.c
LIBSRCS += $(MYLIB_PATH)/s4824063_rcmdisplay.c

LIBSRCS += $(FREERTOS_PATH)/portable/MemMang/heap_2.c