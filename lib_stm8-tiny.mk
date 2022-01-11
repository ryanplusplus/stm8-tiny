__lib_stm8-tiny_path := $(subst $(shell pwd)/,,$(dir $(realpath $(lastword $(MAKEFILE_LIST)))))

include $(__lib_stm8-tiny_path)lib/tiny/lib_tiny.mk

LIBS := stm8-tiny $(LIBS)

INC_DIRS += \
  $(__lib_stm8-tiny_path)src \

stm8-tiny_ASFLAGS := $(ASFLAGS)
stm8-tiny_CPPFLAGS := $(CPPFLAGS)
stm8-tiny_CFLAGS := $(CFLAGS)

stm8-tiny_DEFINES := $(DEFINES)

stm8-tiny_INC_DIRS := \
  $(INC_DIRS) \

stm8-tiny_SYS_INC_DIRS := \

stm8-tiny_SRC_FILES := \

stm8-tiny_SRC_DIRS := \
  $(__lib_stm8-tiny_path)src \
