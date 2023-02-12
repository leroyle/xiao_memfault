#pragma once

//! @file
//!
//! Copyright (c) Memfault, Inc.
//! See License.txt for details
//! Experimental CLI for diagnostics using nrf SDK

extern "C" {
void mflt_cli_init(void);
void mflt_cli_try_process(void);
}
