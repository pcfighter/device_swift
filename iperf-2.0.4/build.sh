#!/bin/bash

export ac_cv_func_malloc_0_nonnull=yes
./configure \
	--build=`uname -m`-linux \
	--host=arm-none-linux-gnueabi \
	--prefix=`pwd`/__build
