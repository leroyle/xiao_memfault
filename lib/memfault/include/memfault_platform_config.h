#pragma once
#ifndef _MEMFAULT_PLATFORM_CONFIG_H_
#define _MEMFAULT_PLATFORM_CONFIG_H_
//! @file
//!
//! Copyright (c) Memfault, Inc.
//! See License.txt for details
//!
//! Platform overrides for the default configuration settings in the memfault-firmware-sdk.
//! Default configuration settings can be found in "memfault/config.h"

#define MEMFAULT_USE_GNU_BUILD_ID 1
#define MEMFAULT_PLATFORM_HAS_LOG_CONFIG 1
//LAL #define MEMFAULT_COMPACT_LOG_ENABLE 1
#define MEMFAULT_COMPACT_LOG_ENABLE 0
#define MEMFAULT_COREDUMP_INCLUDE_BUILD_ID 1

// Enable capture of entire ISR state at time of crash
#define MEMFAULT_NVIC_INTERRUPTS_TO_COLLECT 64

#endif
