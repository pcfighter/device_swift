#
# Swift Config
#
PRODUCT_SPECIFIC_DEFINES += TARGET_PRELINKER_MAP=device/lg/swift/prelink-linux-arm.map
TARGET_BOARD_PLATFORM := msm7k
TARGET_BOARD_PLATFORM_GPU := qcom-adreno200
TARGET_ARCH_VARIANT := armv5te-vfp
TARGET_CPU_ABI := armeabi
TARGET_BOOTLOADER_BOARD_NAME := swift
BOARD_USES_QCOM_HARDWARE := true
BOARD_USES_ADRENO_200 := true
#HAVE_ADRENO200_SOURCE := true
#HAVE_ADRENO200_SC_SOURCE := true
HAVE_ADRENO200_FIRMWARE := true

BOARD_USE_FROYO_LIBCAMERA := true

TARGET_NO_BOOTLOADER := false
TARGET_NO_RECOVERY := true
TARGET_NO_KERNEL := false
TARGET_NO_RADIO := true
BOARD_HAVE_QCOM_FM := true
BOARD_HAVE_FM_RADIO := true
BOARD_GLOBAL_CFLAGS += -DHAVE_FM_RADIO
BOARD_USE_BROADCOM_FM_VOLUME_HACK := true

BOARD_USE_USB_MASS_STORAGE_SWITCH := true
TARGET_USES_16BPPSURFACE_FOR_OPAQUE := true
TARGET_LIBAGL_USE_GRALLOC_COPYBITS := true
BOARD_NO_RGBX_8888 := true

BOARD_KERNEL_CMDLINE :=  mem=214M console=tty0,115200n8 androidboot.hardware=swift uart.mode=arm11_uart_disable crash=off
BOARD_KERNEL_BASE := 0x00200000
BOARD_PAGE_SIZE := 0x00000800

#BOARD_WPA_SUPPLICANT_DRIVER := WEXT
#BOARD_WLAN_DEVICE           := bcm4325
##WPA_SUPPLICANT_VERSION      := VER_0_5_X
#WIFI_DRIVER_MODULE_PATH     := "/system/lib/modules/wireless.ko"
##WIFI_DRIVER_FW_STA_PATH     := "/system/etc/wl/rtecdc.bin"
#WIFI_DRIVER_MODULE_ARG      := "firmware_path=/system/etc/wl/rtecdc.bin nvram_path=/system/etc/wl/nvram.txt"
#WIFI_DRIVER_MODULE_NAME     := "wireless"

BOARD_WPA_SUPPLICANT_DRIVER := WEXT
WPA_SUPPLICANT_VERSION      := VER_0_5_X
BOARD_WLAN_DEVICE           := bcm4325
WIFI_DRIVER_MODULE_PATH     := "/system/lib/modules/wireless.ko"
WIFI_DRIVER_FW_STA_PATH     := "/system/etc/wl/rtecdc.bin"
WIFI_DRIVER_FW_AP_PATH      := "/system/etc/wl/rtecdc-apsta.bin"
WIFI_DRIVER_MODULE_ARG      := "firmware_path=/system/etc/wl/rtecdc.bin nvram_path=/system/etc/wl/nvram.txt"
WIFI_DRIVER_MODULE_NAME     := wireless


USE_CAMERA_STUB := false

#Optimization 
ENABLE_WTF_USE_ACCELERATED_COMPOSITING := true
ENABLE_JSC_JIT := true


BOARD_USES_QCOM_LIBS := true
BOARD_USES_QCOM_HARDWARE := true
BOARD_HAVE_BLUETOOTH := true
BOARD_HAVE_BLUETOOTH_BCM := true
WITH_A2DP := true
TINY_TOOLBOX := true
BOARD_GPS_LIBRARIES := libloc_api
WITH_WINDOWS_MEDIA:= true
BUILD_PV_VIDEO_ENCODERS := true

# fix this up by examining /proc/mtd on a running device
BOARD_BOOTIMAGE_PARTITION_SIZE := 0x00380000
BOARD_RECOVERYIMAGE_PARTITION_SIZE := 0x00480000
BOARD_SYSTEMIMAGE_PARTITION_SIZE := 0x08c60000
BOARD_USERDATAIMAGE_PARTITION_SIZE := 0x105c0000
BOARD_FLASH_BLOCK_SIZE := 131072

TARGET_PREBUILT_KERNEL := kernel

BOARD_EGL_CFG := device/lg/swift/egl.cfg

#BOARD_USES_GENERIC_AUDIO := false
#BOARD_USE_HTC_LIBSENSORS := false
#TARGET_HARDWARE_3D := false
BUILD_WITH_FULL_STAGEFRIGHT:=true
BOARD_USE_NASTY_PTHREAD_CREATE_HACK := true
BUILD_OLD_LIBCAMERA := true



