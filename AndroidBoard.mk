LOCAL_PATH := $(call my-dir)
USE_CAMERA_STUB := false

ALL_PREBUILT += $(INSTALLED_KERNEL_TARGET)

# Install the features available on this device.
## Boot logo, boot & init scripts
PRODUCT_COPY_FILES += \
	device/lg/swift/proprietary/initlogo.rle:root/initlogo.rle \
	device/lg/swift/proprietary/default.prop:root/default.prop \
	device/lg/swift/proprietary/init.swift.rc:root/init.swift.rc \
	device/lg/swift/proprietary/init.qcom.post_boot.sh:root/init.qcom.post_boot.sh

## Some stricte swift stuff
PRODUCT_COPY_FILES += \
	device/lg/swift/prebuilt/apns-conf.xml:system/etc/apns-conf.xml \
	device/lg/swift/prebuilt/AudioFilter.csv:system/etc/AudioFilter.csv \
	device/lg/swift/prebuilt/AutoVolumeControl.txt:system/etc/AutoVolumeControl.txt \
	device/lg/swift/prebuilt/AVRCP.kl:system/usr/keylayout/AVRCP.kl \
	device/lg/swift/prebuilt/qwerty.kl:system/usr/keylayout/qwerty.kl \
	device/lg/swift/prebuilt/msm_touchscreen.kcm.bin:system/usr/keychars/msm_touchscreen.kcm.bin \
	device/lg/swift/prebuilt/qwerty.kcm.bin:system/usr/keychars/qwerty.kcm.bin \
	device/lg/swift/prebuilt/qwerty2.kcm.bin:system/usr/keychars/qwerty2.kcm.bin

## Google apps & some other apps
PRODUCT_COPY_FILES += \
	device/lg/swift/proprietary/app/Gmail.apk:system/app/Gmail.apk \
	device/lg/swift/proprietary/app/DSPManager.apk:system/app/DSPManager.apk \
	device/lg/swift/proprietary/app/MemoWidget.apk:system/app/MemoWidget.apk \
	device/lg/swift/proprietary/app/CalendarWidget.apk:system/app/CalendarWidget.apk \
	device/lg/swift/proprietary/app/Facebook.apk:system/app/Facebook.apk \
	device/lg/swift/proprietary/app/Twitter.apk:system/app/Twitter.apk \
	device/lg/swift/proprietary/app/CalibrationTS.apk:system/app/CalibrationTS.apk \
	device/lg/swift/proprietary/app/GoogleCalendarSyncAdapter.apk:system/app/GoogleCalendarSyncAdapter.apk \
	device/lg/swift/proprietary/app/GoogleContactsSyncAdapter.apk:system/app/GoogleContactsSyncAdapter.apk \
	device/lg/swift/proprietary/app/googlevoice.apk:system/app/googlevoice.apk \
	device/lg/swift/proprietary/app/MarketUpdater.apk:system/app/MarketUpdater.apk \
	device/lg/swift/proprietary/app/QuickOffice.apk:system/app/QuickOffice.apk \
	device/lg/swift/proprietary/app/Talk.apk:system/app/Talk.apk \
	device/lg/swift/proprietary/app/Vending.apk:system/app/Vending.apk \
	device/lg/swift/proprietary/app/YouTube.apk:system/app/YouTube.apk \
	device/lg/swift/proprietary/app/GoogleServicesFramework.apk:system/app/GoogleServicesFramework.apk \
	device/lg/swift/proprietary/app/GoogleBackupTransport.apk:system/app/GoogleBackupTransport.apk \
	device/lg/swift/proprietary/app/NetworkLocation.apk:system/app/NetworkLocation.apk \
	device/lg/swift/proprietary/app/MediaUploader.apk:system/app/MediaUploader.apk \
	device/lg/swift/proprietary/app/kickback.apk:system/app/kickback.apk \
	device/lg/swift/proprietary/app/talkback.apk:system/app/talkback.apk \
	device/lg/swift/proprietary/app/libmicrobes_jni.so:system/lib/libmicrobes_jni.so \
	device/lg/swift/proprietary/app/libspeech.so:system/lib/libspeech.so \
	device/lg/swift/proprietary/app/libvoicesearch.so:system/lib/libvoicesearch.so \
	device/lg/swift/proprietary/app/com.google.android.maps.jar:system/framework/com.google.android.maps.jar \
    device/lg/swift/proprietary/etc/permissions/com.google.android.maps.xml:system/etc/permissions/com.google.android.maps.xml \
	device/lg/swift/proprietary/etc/permissions/android.software.sip.voip.xml:system/etc/permissions/android.software.sip.voip.xml

# Publish that we support the live wallpaper feature.
PRODUCT_COPY_FILES += \
    packages/wallpapers/LivePicker/android.software.live_wallpaper.xml:/system/etc/permissions/android.software.live_wallpaper.xml

## LG IME 
PRODUCT_COPY_FILES += \
	device/lg/swift/proprietary/app/AIME.apk:system/app/AIME.apk \
	device/lg/swift/proprietary/lib/libautohan_jni.so:system/lib/libautohan_jni.so \
	device/lg/swift/proprietary/lib/libjni_xt9input.so:system/lib/libjni_xt9input.so \
	device/lg/swift/proprietary/lib/libxt9core.so:system/lib/libxt9core.so \
	device/lg/swift/proprietary/system/usr/xt9/config/databases.conf:system/usr/xt9/config/databases.conf \
	device/lg/swift/proprietary/system/usr/xt9/databases/kdb/azerty.french.center.kdb:system/usr/xt9/databases/kdb/azerty.french.center.kdb \
	device/lg/swift/proprietary/system/usr/xt9/databases/kdb/keyboard.regional.qwerty.english.center.kdb:system/usr/xt9/databases/kdb/keyboard.regional.qwerty.english.center.kdb \
device/lg/swift/proprietary/system/usr/xt9/databases/kdb/keypad.arabic.kdb:system/usr/xt9/databases/kdb/keypad.arabic.kdb \
device/lg/swift/proprietary/system/usr/xt9/databases/kdb/keypad.bulgarian.kdb:system/usr/xt9/databases/kdb/keypad.bulgarian \
device/lg/swift/proprietary/system/usr/xt9/databases/kdb/keypad.english.kdb:system/usr/xt9/databases/kdb/keypad.english.kdb \
device/lg/swift/proprietary/system/usr/xt9/databases/kdb/keypad.greek.kdb:system/usr/xt9/databases/kdb/keypad.greek.kdb \
device/lg/swift/proprietary/system/usr/xt9/databases/kdb/keypad.hebrew.kdb:system/usr/xt9/databases/kdb/keypad.hebrew.kdb \
device/lg/swift/proprietary/system/usr/xt9/databases/kdb/keypad.macedonian.kdb:system/usr/xt9/databases/kdb/keypad.macedonian.kdb \
device/lg/swift/proprietary/system/usr/xt9/databases/kdb/keypad.russian.kdb:system/usr/xt9/databases/kdb/keypad.russian.kdb \
device/lg/swift/proprietary/system/usr/xt9/databases/kdb/keypad.thai.kdb:system/usr/xt9/databases/kdb/keypad.thai.kdb \
device/lg/swift/proprietary/system/usr/xt9/databases/kdb/keypad.ukrainian.kdb:system/usr/xt9/databases/kdb/keypad.ukrainian.kdb \
device/lg/swift/proprietary/system/usr/xt9/databases/kdb/keypad.vietnam.kdb:system/usr/xt9/databases/kdb/keypad.vietnam.kdb \
device/lg/swift/proprietary/system/usr/xt9/databases/kdb/qwerty.arabic.center.kdb:system/usr/xt9/databases/kdb/qwerty.arabic.center.kdb \
device/lg/swift/proprietary/system/usr/xt9/databases/kdb/qwerty.bulgarian.center.kdb:system/usr/xt9/databases/kdb/qwerty.bulgarian.center.kdb \
device/lg/swift/proprietary/system/usr/xt9/databases/kdb/qwerty.english.center.kdb:system/usr/xt9/databases/kdb/qwerty.english.center \
device/lg/swift/proprietary/system/usr/xt9/databases/kdb/qwerty.greek.center.kdb:system/usr/xt9/databases/kdb/qwerty.greek.center.kdb \
device/lg/swift/proprietary/system/usr/xt9/databases/kdb/qwerty.hebrew.center.kdb:system/usr/xt9/databases/kdb/qwerty.hebrew.center.kdb \
device/lg/swift/proprietary/system/usr/xt9/databases/kdb/qwerty.macedonian.center.kdb:system/usr/xt9/databases/kdb/qwerty.macedonian.center.kdb \
device/lg/swift/proprietary/system/usr/xt9/databases/kdb/qwerty.russian.center.kdb:system/usr/xt9/databases/kdb/qwerty.russian.center.kdb \
device/lg/swift/proprietary/system/usr/xt9/databases/kdb/qwerty.thai.center.kdb:system/usr/xt9/databases/kdb/qwerty.thai.center.kdb \
device/lg/swift/proprietary/system/usr/xt9/databases/kdb/qwerty.ukrainian.center.kdb:system/usr/xt9/databases/kdb/qwerty.ukrainian.center.kdb \
device/lg/swift/proprietary/system/usr/xt9/databases/kdb/qwerty.vietnam.center.kdb:system/usr/xt9/databases/kdb/qwerty.vietnam.center.kdb \
device/lg/swift/proprietary/system/usr/xt9/databases/kdb/qwertz.german.center.kdb:system/usr/xt9/databases/kdb/qwertz.german.center.kdb \
device/lg/swift/proprietary/system/usr/xt9/databases/kdb/reduced_azerty.french.kdb:system/usr/xt9/databases/kdb/reduced_azerty.french.kdb \
device/lg/swift/proprietary/system/usr/xt9/databases/kdb/reduced_qwerty.english.kdb:system/usr/xt9/databases/kdb/reduced_qwertk.english.kdb \
device/lg/swift/proprietary/system/usr/xt9/databases/kdb/reduced_qwertz.german.kdb:system/usr/xt9/databases/kdb/reduced_qwertz.german.kdb \
device/lg/swift/proprietary/system/usr/xt9/databases/kdb/ZHhpd_pinyin_bpmf_stroke_cangjie.kdb:system/usr/xt9/databases/kdb/ZHhpd_pinyin_bpmf_stroke_cangjie.kdb \
device/lg/swift/proprietary/system/usr/xt9/databases/ldb/ARlsUN_xt9.ldb:system/usr/xt9/databases/ldb/ARlsUN_xt9.ldb \
device/lg/swift/proprietary/system/usr/xt9/databases/ldb/BGlsUN_xt9.ldb:system/usr/xt9/databases/ldb/BGlsUN_xt9.ldb \
device/lg/swift/proprietary/system/usr/xt9/databases/ldb/CSlsUN_xt9.ldb:system/usr/xt9/databases/ldb/CSlsUN_xt9.ldb \
device/lg/swift/proprietary/system/usr/xt9/databases/ldb/DAusUN_xt9.ldb:system/usr/xt9/databases/ldb/DAusUN_xt9.ldb \
device/lg/swift/proprietary/system/usr/xt9/databases/ldb/DEusUN_xt9.ldb:system/usr/xt9/databases/ldb/DEusUN_xt9.ldb \
device/lg/swift/proprietary/system/usr/xt9/databases/ldb/ELlsUN_xt9.ldb:system/usr/xt9/databases/ldb/ELlsUN_xt9.ldb \
device/lg/swift/proprietary/system/usr/xt9/databases/ldb/ENubUN_xt9.ldb:system/usr/xt9/databases/ldb/ENubUN_xt9.ldb \
device/lg/swift/proprietary/system/usr/xt9/databases/ldb/ESusUN_xt9.ldb:system/usr/xt9/databases/ldb/ESusUN_xt9.ldb \
device/lg/swift/proprietary/system/usr/xt9/databases/ldb/ETlsUN_xt9.ldb:system/usr/xt9/databases/ldb/ETlsUN_xt9.ldb \
device/lg/swift/proprietary/system/usr/xt9/databases/ldb/FRusUN_xt9.ldb:system/usr/xt9/databases/ldb/FRusUN_xt9.ldb \
device/lg/swift/proprietary/system/usr/xt9/databases/ldb/HElsUN_xt9.ldb:system/usr/xt9/databases/ldb/HElsUN_xt9.ldb \
device/lg/swift/proprietary/system/usr/xt9/databases/ldb/HUlsUN_xt9.ldb:system/usr/xt9/databases/ldb/HUlsUN_xt9.ldb \
device/lg/swift/proprietary/system/usr/xt9/databases/ldb/IDlbUN_xt9.ldb:system/usr/xt9/databases/ldb/IDlbUN_xt9.ldb \
device/lg/swift/proprietary/system/usr/xt9/databases/ldb/ITusUN_xt9.ldb:system/usr/xt9/databases/ldb/ITusUN_xt9.ldb \
device/lg/swift/proprietary/system/usr/xt9/databases/ldb/LTlsUN_xt9.ldb:system/usr/xt9/databases/ldb/LTlsUN_xt9.ldb \
device/lg/swift/proprietary/system/usr/xt9/databases/ldb/LVlsUN_xt9.ldb:system/usr/xt9/databases/ldb/LVlsUN_xt9.ldb \
device/lg/swift/proprietary/system/usr/xt9/databases/ldb/MKlsUN_xt9.ldb:system/usr/xt9/databases/ldb/MKlsUN_xt9.ldb \
device/lg/swift/proprietary/system/usr/xt9/databases/ldb/MSlbUN_xt9.ldb:system/usr/xt9/databases/ldb/MSlbUN_xt9.ldb \
device/lg/swift/proprietary/system/usr/xt9/databases/ldb/NLlsUN_xt9.ldb:system/usr/xt9/databases/ldb/NLlsUN_xt9.ldb \
device/lg/swift/proprietary/system/usr/xt9/databases/ldb/NOlsUN.ldb:system/usr/xt9/databases/ldb/NOlsUN.ldb \
device/lg/swift/proprietary/system/usr/xt9/databases/ldb/PLlsUN_xt9.ldb:system/usr/xt9/databases/ldb/PLlsUN_xt9.ldb \
device/lg/swift/proprietary/system/usr/xt9/databases/ldb/PTusUN_xt9.ldb:system/usr/xt9/databases/ldb/PTusUN_xt9.ldb \
device/lg/swift/proprietary/system/usr/xt9/databases/ldb/ROlsUN_xt9.ldb:system/usr/xt9/databases/ldb/ROlsUN_xt9.ldb \
device/lg/swift/proprietary/system/usr/xt9/databases/ldb/RUlsUN_xt9.ldb:system/usr/xt9/databases/ldb/RUlsUN_xt9.ldb \
device/lg/swift/proprietary/system/usr/xt9/databases/ldb/SHlsUN_xt9.ldb:system/usr/xt9/databases/ldb/SHlsUN_xt9.ldb \
device/lg/swift/proprietary/system/usr/xt9/databases/ldb/SKlsUN_xt9.ldb:system/usr/xt9/databases/ldb/SKlsUN_xt9.ldb \
device/lg/swift/proprietary/system/usr/xt9/databases/ldb/SLlsUN_xt9.ldb:system/usr/xt9/databases/ldb/SLlsUN_xt9.ldb \
device/lg/swift/proprietary/system/usr/xt9/databases/ldb/SRlsUN_xt9.ldb:system/usr/xt9/databases/ldb/SRlsUN_xt9.ldb \
device/lg/swift/proprietary/system/usr/xt9/databases/ldb/SVusUN_xt9.ldb:system/usr/xt9/databases/ldb/SVusUN_xt9.ldb \
device/lg/swift/proprietary/system/usr/xt9/databases/ldb/THlsUN_xt9.ldb:system/usr/xt9/databases/ldb/THlsUN_xt9.ldb \
device/lg/swift/proprietary/system/usr/xt9/databases/ldb/TRlsUN_xt9.ldb:system/usr/xt9/databases/ldb/TRlsUN_xt9.ldb \
device/lg/swift/proprietary/system/usr/xt9/databases/ldb/UKlsUN_xt9.ldb:system/usr/xt9/databases/ldb/UKlsUN_xt9.ldb \
device/lg/swift/proprietary/system/usr/xt9/databases/ldb/VIlsUN_xt9.ldb:system/usr/xt9/databases/ldb/VIlsUN_xt9.ldb \
device/lg/swift/proprietary/system/usr/xt9/databases/ldb/ZHsbUNps_GB2312_xt9.ldb:system/usr/xt9/databases/ldb/ZHsbUNps_GB2312_xt9.ldb \
device/lg/swift/proprietary/system/usr/xt9/databases/ldb/ZHtbUNps_Big5_bpmf_pinyin_CJ_xt9.ldb:system/usr/xt9/databases/ldb/ZHtbUNps_Big5_bpmf_pinyin_CJ_xt9.ldb \
device/lg/swift/proprietary/system/usr/xt9/databases/mdb/DE.txt:system/usr/xt9/databases/mdb/DE.txt \
device/lg/swift/proprietary/system/usr/xt9/databases/mdb/EN.txt:system/usr/xt9/databases/mdb/EN.txt \
device/lg/swift/proprietary/system/usr/xt9/databases/mdb/ES.txt:system/usr/xt9/databases/mdb/ES.txt \
device/lg/swift/proprietary/system/usr/xt9/databases/mdb/FR.txt:system/usr/xt9/databases/mdb/FR.txt \
device/lg/swift/proprietary/system/usr/xt9/databases/mdb/heads_of_state.txt:system/usr/xt9/databases/mdb/heads_of_state.txt \
device/lg/swift/proprietary/system/usr/xt9/databases/mdb/IT.txt:system/usr/xt9/databases/mdb/IT.txt \
device/lg/swift/proprietary/system/usr/xt9/databases/mdb/mdb.py:system/usr/xt9/databases/mdb/mdb.py \
device/lg/swift/proprietary/system/usr/xt9/databases/mdb/mdb.txt:system/usr/xt9/databases/mdb/mdb.txt \
device/lg/swift/proprietary/system/usr/xt9/databases/mdb/PL.txt:system/usr/xt9/databases/mdb/PL.txt

## OMX libs
PRODUCT_COPY_FILES += \
    device/lg/swift/proprietary/lib/libOmxAacEnc.so:system/lib/libOmxAacEnc.so \
    device/lg/swift/proprietary/lib/libOmxAdpcmDec.so:system/lib/libOmxAdpcmDec.so \
    device/lg/swift/proprietary/lib/libOmxAmrDec.so:system/lib/libOmxAmrDec.so \
    device/lg/swift/proprietary/lib/libOmxAmrEnc.so:system/lib/libOmxAmrEnc.so \
    device/lg/swift/proprietary/lib/libOmxAmrRtpDec.so:system/lib/libOmxAmrRtpDec.so \
    device/lg/swift/proprietary/lib/libOmxAmrwbDec.so:system/lib/libOmxAmrwbDec.so \
    device/lg/swift/proprietary/lib/libOmxEvrcDec.so:system/lib/libOmxEvrcDec.so \
    device/lg/swift/proprietary/lib/libOmxEvrcEnc.so:system/lib/libOmxEvrcEnc.so \
    device/lg/swift/proprietary/lib/libOmxH264Dec.so:system/lib/libOmxH264Dec.so \
    device/lg/swift/proprietary/lib/libOmxMpeg4Dec.so:system/lib/libOmxMpeg4Dec.so \
    device/lg/swift/proprietary/lib/libOmxQcelp13Enc.so:system/lib/libOmxQcelp13Enc.so \
    device/lg/swift/proprietary/lib/libOmxQcelpDec.so:system/lib/libOmxQcelpDec.so \
    device/lg/swift/proprietary/lib/libOmxWmaDec.so:system/lib/libOmxWmaDec.so \
    device/lg/swift/proprietary/lib/libOmxWmvDec.so:system/lib/libOmxWmvDec.so \
	device/lg/swift/proprietary/lib/libOmxAacDec.so:system/lib/libOmxAacDec.so \
    device/lg/swift/proprietary/lib/libOmxMp3Dec.so:system/lib/libOmxMp3Dec.so \
	device/lg/swift/proprietary/lib/libOmxCore.so:system/lib/libOmxCore.so \
	device/lg/swift/proprietary/lib/libOmxVidEnc.so:system/lib/libOmxVidEnc.so 

#	device/lg/swift/proprietary/bootanimation.zip:system/media/bootanimation.zip 
#	

## Camera proprietaries
PRODUCT_COPY_FILES += \
device/lg/swift/proprietary/liboemcamera.so:system/lib/liboemcamera.so \
device/lg/swift/proprietary/libmmjpeg.so:system/lib/libmmjpeg.so \
device/lg/swift/proprietary/libmmipl.so:system/lib/libmmipl.so 

PRODUCT_COPY_FILES += \
    device/lg/swift/prebuilt/media_profiles.xml:/system/etc/media_profiles.xml \
    device/lg/swift/prebuilt/dhcpcd.conf:system/etc/dhcpcd/dhcpcd.conf

PRODUCT_COPY_FILES += \
    frameworks/base/data/etc/handheld_core_hardware.xml:system/etc/permissions/handheld_core_hardware.xml \
    frameworks/base/data/etc/android.hardware.camera.autofocus.xml:system/etc/permissions/android.hardware.camera.autofocus.xml \
    frameworks/base/data/etc/android.hardware.telephony.gsm.xml:system/etc/permissions/android.hardware.telephony.gsm.xml \
    frameworks/base/data/etc/android.hardware.location.gps.xml:system/etc/permissions/android.hardware.location.gps.xml \
    frameworks/base/data/etc/android.hardware.wifi.xml:system/etc/permissions/android.hardware.wifi.xml \
# Device permissions
PRODUCT_COPY_FILES += \
    frameworks/base/data/etc/android.hardware.sensor.accelerometer.xml:system/etc/permissions/android.hardware.sensor.accelerometer.xml \
    frameworks/base/data/etc/android.hardware.sensor.compass.xml:system/etc/permissions/android.hardware.sensor.compass.xml \
    frameworks/base/data/etc/platform.xml:system/etc/permissions/platform.xml \
	frameworks/base/data/etc/com.google.android.maps.xml:system/etc/permissions/com.google.android.maps.xml \
	frameworks/base/data/etc/android.hardware.touchscreen.xml:system/etc/permissions/android.hardware.touchscreen.xml 

PRODUCT_COPY_FILES += \
    device/lg/swift/proprietary/libcommondefs.so:obj/lib/libcommondefs.so \
    device/lg/swift/proprietary/libgps.so:obj/lib/libgps.so \
    device/lg/swift/proprietary/libloc_api.so:obj/lib/libloc_api.so \
    device/lg/swift/proprietary/libloc.so:obj/lib/libloc.so \
    device/lg/swift/proprietary/libloc-rpc.so:obj/lib/libloc-rpc.so \
    device/lg/swift/prebuilt/yamato_pfp.fw:system/etc/firmware/yamato_pfp.fw \
    device/lg/swift/prebuilt/yamato_pm4.fw:system/etc/firmware/yamato_pm4.fw \
    device/lg/swift/prebuilt/wpa_supplicant.conf:system/etc/wifi/wpa_supplicant.conf \
    device/lg/swift/proprietary/akmd2:system/bin/akmd2  
    

## Modules
PRODUCT_COPY_FILES += \
device/lg/swift/proprietary/lib/modules/dma_test.ko:system/lib/modules/dma_test.ko \
device/lg/swift/proprietary/lib/modules/evbug.ko:system/lib/modules/evbug.ko \
device/lg/swift/proprietary/lib/modules/hid-dummy.ko:system/lib/modules/hid-dummy.ko \
device/lg/swift/proprietary/lib/modules/ip_gre.ko:system/lib/modules/ip_gre.ko \
device/lg/swift/proprietary/lib/modules/librasdioif.ko:system/lib/modules/librasdioif.ko \
device/lg/swift/proprietary/lib/modules/logger_test.ko:system/lib/modules/logger_test.ko \
device/lg/swift/proprietary/lib/modules/mtd_oobtest.ko:system/lib/modules/mtd_oobtest.ko \
device/lg/swift/proprietary/lib/modules/mtd_pagetest.ko:system/lib/modules/mtd_pagetest.ko \
device/lg/swift/proprietary/lib/modules/mtd_readtest.ko:system/lib/modules/mtd_readtest.ko \
device/lg/swift/proprietary/lib/modules/mtd_speedtest.ko:system/lib/modules/mtd_speedtest.ko \
device/lg/swift/proprietary/lib/modules/mtd_stresstest.ko:system/lib/modules/mtd_stresstest.ko \
device/lg/swift/proprietary/lib/modules/mtd_subpagetest.ko:system/lib/modules/mtd_subpagetest.ko \
device/lg/swift/proprietary/lib/modules/mtd_torturetest.ko:system/lib/modules/mtd_torturetest.ko \
device/lg/swift/proprietary/lib/modules/oprofile.ko:system/lib/modules/oprofile.ko \
device/lg/swift/proprietary/lib/modules/pmem_kernel_test.ko:system/lib/modules/pmem_kernel_test.ko \
device/lg/swift/proprietary/lib/modules/reset_modem.ko:system/lib/modules/reset_modem.ko \
device/lg/swift/proprietary/lib/modules/wireless.ko:system/lib/modules/wireless.ko

#GSM
PROPRIETARY += lib/liblgdrmwbxml.so lib/libdll.so lib/libril-qcril-hook-oem.so  lib/libwms.so \
               lib/libnv.so lib/libril_log.so lib/liblgerft.so \
               lib/liboem_rapi.so lib/libdss.so lib/libqmi.so lib/libmmgsdilib.so lib/libcm.so lib/liboncrpc.so lib/libdsm.so lib/libqueue.so \
	       lib/libril-qc-1.so lib/libdiag.so lib/libgsdi_exp.so lib/libgsdi_exp.so lib/libgstk_exp.so lib/libwmsts.so lib/libpbmlib.so \
	       lib/liblgeat.so lib/liblgdrm.so lib/libbcmwl.so lib/libauth.so /bin/port-bridge /bin/qmuxd lib/libril.so /bin/rild

#GPS
PROPRIETARY += lib/libcommondefs.so lib/libgps.so lib/libloc.so  lib/libloc_api.so lib/libloc-rpc.so 

#Wifi
PROPRIETARY += etc/wl/rtecdc.bin etc/wl/nvram.txt etc/wl/rtecdc-mfgtest.bin etc/wifi/wpa_supplicant.conf bin/wl bin/wpa_supplicant /etc/wl/rtecdc-apsta.bin

#Linker
PROPRIETARY += bin/linker #bin/lgesystemd  bin/lgdrmserver

#Bluetooth
PROPRIETARY += bin/BCM4325D1.hcd 

#Audio Flinger for DSPManager
PROPRIETARY += lib/libaudioflinger.so

#OpenGL
PROPRIETARY += lib/egl/libEGL_adreno200.so lib/egl/libGLESv2_adreno200.so lib/egl/libGLESv1_CM_adreno200.so lib/egl/libq3dtools_adreno200.so lib/libgsl.so

#HW
PROPRIETARY +=  lib/hw/lights.swift.so lib/hw/sensors.swift.so lib/hw/gralloc.default.so lib/hw/gralloc.msm7k.so lib/hw/copybit.swift.so

PRODUCT_COPY_FILES += $(foreach i,$(PROPRIETARY),$(LOCAL_PATH)/proprietary/$(notdir $i):system/$i)
