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

import re
import os
import logging

from vts.testcases.kernel.ltp import ltp_enums
from vts.testcases.kernel.ltp import ltp_configs


class TestCase(object):
    """Stores name, path, and param information for each test case.

    All class initiation inputs are assumed to be already validated by
    test case parser.

    Attributes:
        testsuite: string, name of testsuite to which the testcase belongs
        testname: string, name of the test case
        command: string, the command to run the test case
        _args: list of string, test case command line arguments
        requirement_state: RequirementState, enum representing requirement
                            check results
        note: string, a place to store additional note for the test case
              such as what environment requirement did not satisfy.
    """

    def __init__(self, testsuite, testname, command):
        self.testsuite = testsuite
        self.testname = testname
        self._command = command
        self.requirement_state = ltp_enums.RequirementState.UNCHECKED
        self.note = ""

    @property
    def note(self):
        """Get the note"""
        return self._note

    @note.setter
    def note(self, note):
        """Set the note"""
        self._note = note

    @property
    def requirement_state(self):
        """Get the requirement state"""
        return self._requirement_state

    @requirement_state.setter
    def requirement_state(self, requirement_state):
        """Set the requirement state"""
        self._requirement_state = requirement_state

    @property
    def testsuite(self):
        """Get the test suite's name."""
        return self._testsuite

    @testsuite.setter
    def testsuite(self, testsuite):
        """Set the test suite's name."""
        self._testsuite = testsuite

    @property
    def testname(self):
        """Get the test case's name."""
        return self._testname

    @testname.setter
    def testname(self, testname):
        """Set the test case's name."""
        self._testname = testname

    def InternalAddLtpPathToCommand(self, command):
        """Internal function to change binary in commands to their full path"""
        tokens = command.strip().split()

        # If not ltp executables:
        if (tokens[0] in ltp_configs.EXTERNAL_BINS or tokens[0].find('=') > 0):
            return command
        else:  # Is Ltp executable
            tokens[0] = os.path.join(ltp_configs.LTPBINPATH, tokens[0])
            return ' '.join(tokens)

    def GetCommand(self):
        """Get test case's command.

        Get the test case's command where ltp test binary names have been
        replaced with their full paths
        """
        return '&&'.join((self.InternalAddLtpPathToCommand(command)
                          for command in self._command.split('&&')))

    def InternalGetExecutableNames(self):
        """Get a generator of all required executable file names"""
        executables = (command.strip().split()[0]
                       for command in self._command.split('&&'))

        # In some test definitions there were command starting with
        # > TDsrc='mktemp ...'. We use regex to remove quotes
        pattern = re.compile('[\'|\"]')
        return (pattern.sub('', executable.split('=')[1])
                if executable.find('=') > 0 else executable
                for executable in executables)

    def GetRequiredExecutablePaths(self):
        """Get required executables' paths.

        Get a generator of all executables' paths that will be needed
        by its command. For LTP's executables, absolute path will be
        returned. For binaries in system's PATH, only the name will be
        returned.
        """
        return (os.path.join(ltp_configs.LTPBINPATH, executable)
                if executable not in ltp_configs.EXTERNAL_BINS else executable
                for executable in self.InternalGetExecutableNames())

    @property
    def fullname(self):
        """Return full test name in <testsuite-testname> format"""
        return "%s-%s" % (self.testsuite, self.testname)

    def __str__(self):
        return self.fullname