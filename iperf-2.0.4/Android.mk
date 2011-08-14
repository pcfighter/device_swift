LOCAL_PATH := $(call my-dir)
include $(CLEAR_VARS)

LOCAL_SRC_FILES := \
	compat/Thread.c \
	compat/error.c \
	compat/delay.cpp \
	compat/gettimeofday.c \
	compat/inet_ntop.c \
	compat/inet_pton.c \
	compat/signal.c \
	compat/snprintf.c \
	compat/string.c \
	src/Client.cpp \
	src/Extractor.c \
	src/Launch.cpp \
	src/List.cpp \
	src/Listener.cpp \
	src/Locale.c \
	src/PerfSocket.cpp \
	src/ReportCSV.c \
	src/ReportDefault.c \
	src/Reporter.c \
	src/Server.cpp \
	src/Settings.cpp \
	src/SocketAddr.c \
	src/gnu_getopt.c \
	src/gnu_getopt_long.c \
	src/main.cpp \
	src/service.c \
	src/sockets.c \
	src/stdio.c \
	src/tcp_window_size.c

LOCAL_C_INCLUDES := \
	$(LOCAL_PATH) \
	$(LOCAL_PATH)/compat \
	$(LOCAL_PATH)/include

LOCAL_MODULE := iperf
LOCAL_CFLAGS := -DHAVE_CONFIG_H
#LOCAL_SHARED_LIBRARIES := libpthread
include $(BUILD_EXECUTABLE)
