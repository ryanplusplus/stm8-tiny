__lib_stm8-tiny_path := $(dir $(realpath $(lastword $(MAKEFILE_LIST))))

LIBS := stm8-tiny $(LIBS)

INC_DIRS += \
  $(__lib_stm8-tiny_path)lib/tiny/include \
  $(__lib_stm8-tiny_path)src \

stm8-tiny_LIB_ROOT := $(__lib_stm8-tiny_path)

stm8-tiny_ASFLAGS := $(ASFLAGS)
stm8-tiny_CPPFLAGS := $(CPPFLAGS)
stm8-tiny_CFLAGS := $(CFLAGS)

stm8-tiny_DEFINES := $(DEFINES)

stm8-tiny_INC_DIRS := \
  $(__lib_stm8-tiny_path)lib/tiny/include \

stm8-tiny_SYS_INC_DIRS := \

stm8-tiny_SRC_FILES := \

stm8-tiny_SRC_DIRS := \
  $(__lib_stm8-tiny_path)src \
  $(__lib_stm8-tiny_path)lib/tiny/src \
