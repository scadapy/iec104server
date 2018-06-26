LIB60870_HOME=.

include make/target_system.mk

LIB_SOURCE_DIRS = src/common
LIB_SOURCE_DIRS += src/iec60870
LIB_SOURCE_DIRS += src/iec60870/cs101
LIB_SOURCE_DIRS += src/iec60870/cs104
LIB_SOURCE_DIRS += src/iec60870/link_layer
LIB_SOURCE_DIRS += src/iec60870/apl

ifeq ($(HAL_IMPL), WIN32)
LIB_SOURCE_DIRS += src/hal/socket/win32
LIB_SOURCE_DIRS += src/hal/thread/win32
LIB_SOURCE_DIRS += src/hal/time/win32
else ifeq ($(HAL_IMPL), POSIX)
LIB_SOURCE_DIRS += src/hal/socket/linux
LIB_SOURCE_DIRS += src/hal/thread/linux
LIB_SOURCE_DIRS += src/hal/time/unix
LIB_SOURCE_DIRS += src/hal/serial/linux
else ifeq ($(HAL_IMPL), BSD)
LIB_SOURCE_DIRS += src/hal/socket/bsd
LIB_SOURCE_DIRS += src/hal/thread/bsd
LIB_SOURCE_DIRS += src/hal/time/unix
endif

ifdef WITH_MBEDTLS
LIB_SOURCE_DIRS += dependencies/mbedtls-2.6.0/library
LIB_SOURCE_DIRS += src/tls/mbedtls
LIB_INCLUDE_DIRS += src/tls/mbedtls
LIB_INCLUDE_DIRS += dependencies/mbedtls-2.6.0/include
CFLAGS += -D'MBEDTLS_CONFIG_FILE="mbedtls_config.h"'
CFLAGS += -D'CONFIG_CS104_SUPPORT_TLS=1'
endif

LIB_INCLUDE_DIRS += config
LIB_INCLUDE_DIRS += src/inc/api
LIB_INCLUDE_DIRS += src/inc/internal
LIB_INCLUDE_DIRS += src/hal/inc
LIB_INCLUDE_DIRS += src/tls
LIB_INCLUDE_DIRS += src/common/inc


LIB_INCLUDES = $(addprefix -I,$(LIB_INCLUDE_DIRS))

ifndef INSTALL_PREFIX
INSTALL_PREFIX = ./.install
endif

LIB_API_HEADER_FILES = src/hal/inc/hal_time.h 
LIB_API_HEADER_FILES += src/hal/inc/hal_thread.h
LIB_API_HEADER_FILES += src/hal/inc/hal_filesystem.h 
LIB_API_HEADER_FILES += src/inc/api/iec60870_master.h
LIB_API_HEADER_FILES += src/inc/api/iec60870_server.h
LIB_API_HEADER_FILES += src/inc/api/iec60870_common.h
LIB_API_HEADER_FILES += src/inc/api/information_objects.h
LIB_API_HEADER_FILES += src/inc/api/t104_connection.h
LIB_API_HEADER_FILES += src/tls/tls_api.h

LIB_TEST_SOURCES = tests/all_tests.c
LIB_TEST_SOURCES += tests/unity/unity.c

LIB_TEST_INCLUDE_DIRS = tests/unity

TEST_INCLUDES = $(addprefix -I,$(LIB_TEST_INCLUDE_DIRS))

get_sources_from_directory  = $(wildcard $1/*.c)
get_sources = $(foreach dir, $1, $(call get_sources_from_directory,$(dir)))
src_to = $(addprefix $(LIB_OBJS_DIR)/,$(subst .c,$1,$2))

LIB_SOURCES = $(call get_sources,$(LIB_SOURCE_DIRS))

LIB_OBJS = $(call src_to,.o,$(LIB_SOURCES))
TEST_OBJS = $(call src_to,.o,$(LIB_TEST_SOURCES))
CFLAGS += -std=gnu99
#CFLAGS += -Wno-error=format 
CFLAGS += -Wstrict-prototypes

ifneq ($(HAL_IMPL), WIN32)
CFLAGS += -Wuninitialized 
endif

CFLAGS += -Wsign-compare 
CFLAGS += -Wpointer-arith 
CFLAGS += -Wnested-externs 
CFLAGS += -Wmissing-declarations 
CFLAGS += -Wshadow
CFLAGS += -Wall
#CFLAGS += -Werror  

all:	lib

static_checks:	lib
	splint -preproc +posixlib +skip-sys-headers +gnuextensions $(LIB_INCLUDES) $(LIB_SOURCES)

cppcheck:	lib
	cppcheck --force --std=c99 --enable=all $(LIB_INCLUDES) $(LIB_SOURCES) 2> cppcheck-output.xml

lib:	$(LIB_NAME)

tests:	$(TEST_NAME)

dynlib: CFLAGS += -fPIC

dynlib:	$(DYN_LIB_NAME)

.PHONY:	examples

examples:
	cd examples; $(MAKE)
	
$(TEST_NAME):	$(LIB_OBJS) $(TEST_OBJS)
	$(CC) -o $(TEST_NAME) $(LIB_OBJS) $(TEST_OBJS) -lpthread

$(LIB_NAME):	$(LIB_OBJS)
	$(AR) r $(LIB_NAME) $(LIB_OBJS)
	$(RANLIB) $(LIB_NAME)
	
$(DYN_LIB_NAME):	$(LIB_OBJS)
	$(CC) $(LDFLAGS) $(DYNLIB_LDFLAGS) -shared -o $(DYN_LIB_NAME) $(LIB_OBJS) $(LDLIBS)

$(LIB_OBJS_DIR)/%.o: %.c config
	@echo compiling $(notdir $<)
	$(SILENCE)mkdir -p $(dir $@)
	$(CC) $(CFLAGS) -c $(LIB_INCLUDES) $(TEST_INCLUDES) $(OUTPUT_OPTION) $<
	
install:	$(LIB_NAME)
	mkdir -p $(INSTALL_PREFIX)/include
	mkdir -p $(INSTALL_PREFIX)/lib
	cp $(LIB_API_HEADER_FILES) $(INSTALL_PREFIX)/include
	cp $(LIB_NAME) $(INSTALL_PREFIX)/lib

clean:
	rm -f $(EXAMPLES)
	rm -rf $(LIB_OBJS_DIR)

