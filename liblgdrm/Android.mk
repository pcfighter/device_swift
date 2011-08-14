
LOCAL_PATH := $(call my-dir)

include $(LOCAL_PATH)/LGDrmCFG.min

#----------------------------------------------------------
# Porting interface
# Output: liblgdrmport.a
#----------------------------------------------------------
include $(CLEAR_VARS)
include $(LOCAL_PATH)/LGDrmPortLib.min

# Source
LOCAL_SRC_FILES := $(LGDRM_PORT_SOURCES)

# Include path
LOCAL_C_INCLUDES := \
		$(LGDRM_PORT_INCLUDE) \
		$(call include-path-for, system-core)/cutils

# Flags
LOCAL_CFLAGS := $(LGDRM_PORT_APPFLAG)

# Output
LOCAL_MODULE := liblgdrmport

# Build type
include $(BUILD_STATIC_LIBRARY)

#----------------------------------------------------------
# Open Source: Expat
# Output: liblgdrmexpat.a
#----------------------------------------------------------
ifeq ($(LGDRMLIB_USE_EXPAT), yes)
include $(CLEAR_VARS)
include $(LOCAL_PATH)/LGDrmExpat.min

# Source
LOCAL_SRC_FILES := $(LGDRM_EXPAT_SOURCES)

# Include path
LOCAL_C_INCLUDES := \
		$(LGDRM_EXPAT_INCLUDE) \
		$(call include-path-for, system-core)/cutils
		
# Flags
LOCAL_CFLAGS := $(LGDRM_EXPAT_APPFLAG)

# Static library
LOCAL_STATIC_LIBRARIES := liblgdrmport

# Output
LOCAL_MODULE := liblgdrmexpat

# Build type
include $(BUILD_STATIC_LIBRARY)

endif


#----------------------------------------------------------
# Open Source: Lib wbxml
# Output: liblgdrmwbxml.so
#----------------------------------------------------------
ifeq ($(LGDRMLIB_USE_LIBWBXML), yes)
include $(CLEAR_VARS)
include $(LOCAL_PATH)/LGDrmWBXml.min

# Source
LOCAL_SRC_FILES := $(LGDRM_WBXML_SOURCES)

# Include path
LOCAL_C_INCLUDES := \
		$(LGDRM_WBXML_INCLUDE) \
		$(call include-path-for, system-core)/cutils
		
# Flags
LOCAL_CFLAGS := $(LGDRM_WBXML_APPFLAG)

# Static library
LOCAL_STATIC_LIBRARIES := liblgdrmport liblgdrmexpat

# Shared library
LOCAL_SHARED_LIBRARIES := \
		libutils \
		libcutils \
		liblog

# Output
LOCAL_MODULE := liblgdrmwbxml

# Prelink option
LOCAL_PRELINK_MODULE := false

# Build type
include $(BUILD_SHARED_LIBRARY)

endif