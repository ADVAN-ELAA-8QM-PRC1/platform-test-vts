#!/usr/bin/env python3.4
#
# Copyright (C) 2016 The Android Open Source Project
#
# Licensed under the Apache License, Version 2.0 (the "License");
# you may not use this file except in compliance with the License.
# You may obtain a copy of the License at
#
#      http://www.apache.org/licenses/LICENSE-2.0
#
# Unless required by applicable law or agreed to in writing, software
# distributed under the License is distributed on an "AS IS" BASIS,
# WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
# See the License for the specific language governing permissions and
# limitations under the License.
#

class ConfigKeys(object):
    RUN_STAGING = "run_staging"

class ExitCode(object):
    """Exit codes for test binaries and test scripts."""
    KSFT_PASS = 0
    KSFT_FAIL = 1
    KSFT_XPASS = 2
    KSFT_XFAIL = 3
    KSFT_SKIP = 4

# Directory on the target where the tests are copied.
KSFT_DIR = "/data/local/tmp/linux-kselftest"

KSFT_CASES_STABLE = [
    "futex/functional/futex_wait_timeout",
    "futex/functional/futex_wait_wouldblock",
    "futex/functional/futex_requeue_pi_mismatched_ops",
    "futex/functional/futex_wait_uninitialized_heap",
    "futex/functional/futex_wait_private_mapped_file",
    "net/socket",
]

KSFT_CASES_STAGING = [
    "timers/posix_timers",
    "timers/nanosleep",
    "timers/nsleep-lat",
    "timers/set-timer-lat",
    "timers/inconsistency-check",
    "timers/alarmtimer-suspend",
    "timers/raw_skew",
    "timers/threadtest",
    "timers/change_skew",
    "timers/skew_consistency",
    "timers/clocksource-switch",
    "timers/set-tai",
    "timers/valid-adjtimex",
    "net/psock_tpacket",
    "pstore/pstore_tests",
]
