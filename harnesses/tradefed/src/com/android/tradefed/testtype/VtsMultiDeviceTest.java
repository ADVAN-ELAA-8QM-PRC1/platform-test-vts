/*
 * Copyright (C) 2016 The Android Open Source Project
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *      http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

package com.android.tradefed.testtype;

import com.android.tradefed.config.Option;
import com.android.tradefed.config.OptionClass;
import com.android.tradefed.device.DeviceNotAvailableException;
import com.android.tradefed.device.ITestDevice;
import com.android.tradefed.log.LogUtil.CLog;
import com.android.tradefed.result.ITestInvocationListener;
import com.android.tradefed.util.ArrayUtil;
import com.android.tradefed.util.CommandResult;
import com.android.tradefed.util.CommandStatus;
import com.android.tradefed.util.IRunUtil;
import com.android.tradefed.util.RunUtil;

/**
 * A Test that runs a vts multi device test package (part of Vendor Test Suite,
 * VTS) on given device.
 */

@OptionClass(alias = "vtsmultidevicetest")
public class VtsMultiDeviceTest implements IDeviceTest, IRemoteTest {

    public static String PYTHONPATH = "PYTHONPATH";
    static final float DEFAULT_TARGET_VERSION = -1;

    private ITestDevice mDevice = null;

    @Option(name = "test-timeout", description = "maximum amount of time"
        + "(im milliseconds) tests are allowed to run",
        isTimeVal = true)
    public static long TEST_TIMEOUT = 1000 * 60 * 5;

    @Option(name = "test-case-path",
        description = "The path for test case.")
    private String mTestCasePath = null;

    @Option(name = "test-config-path",
        description = "The path for test case config file.")
    private String mTestConfigPath = null;

    // This variable is set in order to include the directory that contains the
    // python test cases. This is set before calling the method.
    // {@link #doRunTest(IRunUtil, String, String)}.
    public String mPythonPath = null;

    @Option(name = "python-binary", description = "python binary to use "
        + "(optional)")
    private String mPythonBin = null;

    private IRunUtil mRunUtil = null;

    /**
     * @return the mRunUtil
     */
    public IRunUtil getRunUtil() {
        return mRunUtil;
    }

    /**
     * @param mRunUtil the mRunUtil to set
     */
    public void setRunUtil(IRunUtil mRunUtil) {
        this.mRunUtil = mRunUtil;
    }

    /**
     * {@inheritDoc}
     */
    @Override
    public void setDevice(ITestDevice device) {
        mDevice = device;
    }

    /**
     * {@inheritDoc}
     */
    @Override
    public ITestDevice getDevice() {
        return mDevice;
    }

    public void setTestCasePath(String path){
        mTestCasePath = path;
    }

    public void setTestConfigPath(String path){
        mTestConfigPath = path;
    }

    /**
     * {@inheritDoc}
     */
    @SuppressWarnings("deprecation")
    @Override
    public void run(ITestInvocationListener listener)
        throws IllegalArgumentException, DeviceNotAvailableException {
        if (mDevice == null) {
            throw new DeviceNotAvailableException("Device has not been set");
        }

        if (mTestCasePath == null) {
            throw new IllegalArgumentException("test-case-path is not set.");
        }

        if (mTestConfigPath == null) {
            throw new IllegalArgumentException("test-config-path is not set.");
        }

        setPythonPath();

        if (mPythonBin == null) {
            mPythonBin = getPythonBinary();
        }

        if (mRunUtil == null){
            mRunUtil = new RunUtil();
            mRunUtil.setEnvVariable(PYTHONPATH, mPythonPath);
        }
        doRunTest(mRunUtil, mTestCasePath, mTestConfigPath);
    }

    /**
     * This method prepares a command for the test and runs the python file as
     * given in the arguments.
     *
     * @param runUtil
     * @param mTestCasePath
     * @param mTestConfigPath
     */

    private boolean doRunTest(IRunUtil runUtil, String mTestCasePath,
        String mTestConfigPath) throws RuntimeException {

        if (mPythonBin == null){
            mPythonBin = getPythonBinary();
        }
        String[] baseOpts = {mPythonBin, "-m"};
        String[] testModule = {mTestCasePath, mTestConfigPath};
        String[] cmd;
        cmd = ArrayUtil.buildArray(baseOpts, testModule);

        CommandResult commandResult = runUtil.runTimedCmd(TEST_TIMEOUT, cmd);

        if (commandResult != null && commandResult.getStatus() !=
              CommandStatus.SUCCESS) {
            CLog.e("Python process failed");
            CLog.e("Python path: %s", mPythonPath);
            CLog.e("Stderr: %s", commandResult.getStderr());
            CLog.e("Stdout: %s", commandResult.getStdout());
            throw new RuntimeException("Failed to run python unit test");
        }
        if (commandResult != null){
            CLog.i("Standard output is: %s", commandResult.getStdout());
            CLog.i("Parsing test result: %s", commandResult.getStderr());
        }

        return commandResult.getStatus() == CommandStatus.SUCCESS ?
            true : false;
     }

    /**
     * This method sets the python path. It's based on the based on the
     * assumption that the environment variable $ANDROID_BUILD_TOP is set.
     */
    private void setPythonPath() {
        StringBuilder sb = new StringBuilder();
        sb.append(":" + System.getenv("ANDROID_BUILD_TOP") + "/test");
        mPythonPath = sb.toString();
    }

    /**
     * This method gets the python binary
     */
    private String getPythonBinary() {
        IRunUtil runUtil = RunUtil.getDefault();
        CommandResult c = runUtil.runTimedCmd(1000, "which", "python");
        String pythonBin = c.getStdout().trim();
        if (pythonBin.length() == 0) {
            throw new RuntimeException("Could not find python binary on host "
                + "machine");
        }
        return pythonBin;
    }
 }
