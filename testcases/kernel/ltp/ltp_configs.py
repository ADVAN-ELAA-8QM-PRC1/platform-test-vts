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

import os


# string, ltp build root directory on target
LTPDIR = "/data/local/tmp/ltp"
LTPTMPDIR = os.path.join(LTPDIR, "temp")
LTPTMP = os.path.join(LTPTMPDIR, "tmp")


REQUIREMENTS_TO_TESTCASE = {
    "loop_device_support": [
        "syscalls-mount01",
        "syscalls-fchmod06",
        "syscalls-ftruncate04",
        "syscalls-ftruncate04_64",
        "syscalls-inotify03",
        "syscalls-link08",
        "syscalls-linkat02",
        "syscalls-mkdir03",
        "syscalls-mkdirat02",
        "syscalls-mknod07",
        "syscalls-mknodat02",
        "syscalls-mmap16",
        "syscalls-mount01",
        "syscalls-mount02",
        "syscalls-mount03",
        "syscalls-mount04",
        "syscalls-mount06",
        "syscalls-rename11",
        "syscalls-renameat01",
        "syscalls-rmdir02",
        "syscalls-umount01",
        "syscalls-umount02",
        "syscalls-umount03",
        "syscalls-umount2_01",
        "syscalls-umount2_02",
        "syscalls-umount2_03",
        "syscalls-utime06",
        "syscalls-utimes01",
        "syscalls-mkfs01",
        ],
    }

REQUIREMENT_FOR_ALL = ["ltptmp_dir"]

REQUIREMENT_TO_TESTSUITE = {}

DISABLED_TESTS = []
