__lib_stm8-tiny_path := $(call current_path)

include $(__lib_stm8-tiny_path)lib/tiny/lib_tiny.mk

$(call create_lib_with_defaults,stm8-tiny)

INC_DIRS += \
  $(__lib_stm8-tiny_path)src \

stm8-tiny_DEFINES := $(DEFINES)

stm8-tiny_INC_DIRS := \
  $(INC_DIRS) \

stm8-tiny_SYS_INC_DIRS := \

stm8-tiny_SRC_FILES := \

stm8-tiny_SRC_DIRS := \
  $(__lib_stm8-tiny_path)src \
