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

import logging

from vts.runners.host import base_test
from vts.runners.host import test_runner
from vts.utils.python.mirror_objects import mirror


class SampleCameraTest(base_test.BaseTestClass):
    """A sample testcase for the non-HIDL, conventional Camera HAL."""

    def setUpClass(self):
        self.hal_mirror = mirror.Mirror(["/data/local/tmp/32/hal"])
        self.hal_mirror.InitHal("camera", 2.1, bits=32)

    def testCameraOpenFirst(self):
        """A simple testcase which just calls an open function."""
        self.hal_mirror.camera.common.methods.open()  # should not crash b/29053974

    def testCameraInit(self):
        """A simple testcase which just calls an init function."""
        self.hal_mirror.camera.init()  # expect an exception? (can be undefined)

    def testCameraNormal(self):
        """A simple testcase which just emulates a normal usage pattern."""
        result = self.hal_mirror.camera.get_number_of_cameras()
        count = result.return_type.primitive_value[0].int32_t
        logging.info(count)
        for index in range(0, count):
          arg = self.hal_mirror.camera.camera_info_t(facing=0)
          logging.info(self.hal_mirror.camera.get_camera_info(index, arg))
        # uncomment when undefined function is handled gracefully.
        # self.hal_mirror.camera.init()
        self.hal_mirror.camera.camera_module_callbacks_t()
        self.hal_mirror.camera.set_callbacks()
        self.hal_mirror.camera.common.methods.open()


if __name__ == "__main__":
    test_runner.main()
