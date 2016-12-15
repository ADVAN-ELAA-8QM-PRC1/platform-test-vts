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

from vts.runners.host import asserts
from vts.runners.host import base_test_with_webdb
from vts.runners.host import const
from vts.runners.host import keys
from vts.runners.host import test_runner
from vts.utils.python.controllers import android_device

from vts.testcases.kernel.linux_kselftest import kselftest_config as config

class LinuxKselftestTest(base_test_with_webdb.BaseTestWithWebDbClass):
    """Runs Linux Kselftest test cases against Android OS kernel.

    Attributes:
        _dut: AndroidDevice, the device under test as config
        _shell: ShellMirrorObject, shell mirror
        _testcases: TestCaseParser, test case input parser
    """
    _32BIT = "32"
    _64BIT = "64"

    def setUpClass(self):
        """Creates a remote shell instance, and copies data files."""
        required_params = [keys.ConfigKeys.IKEY_DATA_FILE_PATH,
            keys.ConfigKeys.KEY_TEST_SUITE]
        self.getUserParams(required_params)

        logging.info("%s: %s", keys.ConfigKeys.IKEY_DATA_FILE_PATH,
            self.data_file_path)
        logging.info("%s: %s", keys.ConfigKeys.KEY_TEST_SUITE, self.test_suite)

        self._dut = self.registerController(android_device)[0]
        self._dut.shell.InvokeTerminal("one")
        self._shell = self._dut.shell.one

    def tearDownClass(self):
        """Deletes all copied data."""
        self._shell.Execute("rm -rf %s" % config.KSFT_DIR)

    def PushFiles(self, n_bit):
        """adb pushes related file to target.

        Args:
            n_bit: _32BIT or 32 for 32-bit tests;
                _64BIT or 64 for 64-bit tests;
        """
        self._shell.Execute("mkdir %s -p" % config.KSFT_DIR)
        self._dut.adb.push("%s/%s/linux-kselftest/. %s" %
            (self.data_file_path, n_bit, config.KSFT_DIR))

    def RunTestcase(self, testcase):
        """Runs the given testcase and asserts the result.

        Args:
            testcase: string, format testsuit/testname, specifies which
                test case to run.
        """
        cmd = "cd %s && chmod 755 %s && %s" % \
            (config.KSFT_DIR, testcase, testcase)
        logging.info("Executing: %s", cmd)

        result = self._shell.Execute(cmd)
        logging.info("EXIT_CODE: %s:", result[const.EXIT_CODE])

        ret_code = result[const.EXIT_CODE][0]
        asserts.assertEqual(ret_code, config.ExitCode.KSFT_PASS)

    def TestNBits(self, n_bit):
        """Runs all 32-bit or all 64-bit tests.

        Args:
            n_bit: _32BIT or 32 for 32-bit tests;
                _64BIT or 64 for 64-bit tests;
        """
        logging.info("[Test Case] test%sBits SKIP", n_bit)
        self.PushFiles(n_bit)

        self.runGeneratedTests(
            test_func=self.RunTestcase,
            settings=config.KSFT_CASES,
            name_func=
                lambda name: "%s_%sbit" % (name.replace('/','_'), n_bit))
        logging.info("[Test Case] test%sBits", n_bit)
        asserts.skip("Finished generating {} bit tests.".format(n_bit))

    def test32Bits(self):
        """Runs all 32-bit tests."""
        self.TestNBits(self._32BIT)

    def test64Bits(self):
        """Runs all 64-bit tests."""
        self.TestNBits(self._64BIT)

if __name__ == "__main__":
    test_runner.main()
