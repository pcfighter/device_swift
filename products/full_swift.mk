# Adding ALL Languages

$(call inherit-product, $(SRC_TARGET_DIR)/product/languages_full.mk)

# European SUPL Server
$(call inherit-product, device/common/gps/gps_eu_supl.mk)

# Used by BusyBox
KERNEL_MODULES_DIR:= /system/lib/modules

ifeq ($(TARGET_PREBUILT_KERNEL),)
	LOCAL_KERNEL := kernel
else
	LOCAL_KERNEL := $(TARGET_PREBUILT_KERNEL)
endif

PRODUCT_COPY_FILES += \
    $(LOCAL_KERNEL):kernel


include frameworks/base/data/sounds/AudioPackage4.mk

DEVICE_PACKAGE_OVERLAYS += device/lg/swift/overlay
    
PRODUCT_PACKAGES += \
    VoiceDialer \
    LiveWallpapers \
    LiveWallpapersPicker \
    MagicSmokeWallpapers \
    VisualizationWallpapers \
    Launcher2 \
    Provision \
    GoogleSearch \
    LatinIME \
    Stk \
    copybit.swift \
    hcitool \
    Superuser \
    FileManager \
    TSCalibration \
    libmm-omxcore \
    librs_jni \
    libOmxCore \
    libstagefrighthw \
    libopencorehw

PRODUCT_PROPERTY_OVERRIDES += \
    keyguard.no_require_sim=true \
    ro.media.dec.jpeg.memcap=10000000 \
    ro.com.android.dataroaming=false \
    ro.setupwizard.enable_bypass=1 \
    ro.com.android.dateformat=MM-dd-yyyy \
    ro.config.ringtone=DonMessWivIt.ogg

# Wifi
PRODUCT_PROPERTY_OVERRIDES += \
    wifi.interface=wlan0 \
    wifi.supplicant_scan_interval=15 \
    ro.wifi.channels=14


# Ril Config
PRODUCT_PROPERTY_OVERRIDES += \
rild.libpath=/system/lib/libril-qc-1.so \
rild.libargs=-d /dev/smd0

# Enable Compcache by default on D/S
PRODUCT_PROPERTY_OVERRIDES += \
    ro.compcache.default=1

# density in DPI of the LCD of this board. This is used to scale the UI
# appropriately. If this property is not defined, the default value is 160 dpi. 
PRODUCT_PROPERTY_OVERRIDES += \
    ro.sf.lcd_density=160

# Enable JIT by default
PRODUCT_PROPERTY_OVERRIDES += \
    dalvik.vm.execution-mode=int:jit \
    dalvik.vm.heapsize=24m

# The OpenGL ES API level that is natively supported by this device.
# This is a 16.16 fixed point number
PRODUCT_PROPERTY_OVERRIDES += \
	ro.opengles.version=131072

$(call inherit-product, $(SRC_TARGET_DIR)/product/generic.mk)

# Discard inherited values and use our own instead.
PRODUCT_NAME := full_swift
PRODUCT_DEVICE := swift
PRODUCT_MODEL := LG GT540
