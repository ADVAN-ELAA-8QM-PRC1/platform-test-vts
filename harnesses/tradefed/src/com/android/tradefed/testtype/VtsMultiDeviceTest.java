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

import com.android.ddmlib.testrunner.ITestRunListener;
import com.android.compatibility.common.tradefed.build.CompatibilityBuildHelper;
import com.android.tradefed.build.IBuildInfo;
import com.android.tradefed.config.Option;
import com.android.tradefed.config.OptionClass;
import com.android.tradefed.device.DeviceNotAvailableException;
import com.android.tradefed.device.ITestDevice;
import com.android.tradefed.log.LogUtil.CLog;
import com.android.ddmlib.Log.LogLevel;
import com.android.tradefed.result.ITestInvocationListener;
import com.android.tradefed.util.ArrayUtil;
import com.android.tradefed.util.CommandResult;
import com.android.tradefed.util.CommandStatus;
import com.android.tradefed.util.FileUtil;
import com.android.tradefed.util.JsonUtil;
import com.android.tradefed.util.IRunUtil;
import com.android.tradefed.util.RunUtil;

import org.json.JSONArray;
import org.json.JSONObject;
import org.json.JSONException;


import java.io.File;
import java.io.FileNotFoundException;
import java.io.FileWriter;
import java.io.IOException;
import java.nio.file.Paths;
import java.util.TreeSet;
import java.util.Set;

/**
 * A Test that runs a vts multi device test package (part of Vendor Test Suite,
 * VTS) on given device.
 */

@OptionClass(alias = "vtsmultidevicetest")
public class VtsMultiDeviceTest implements IDeviceTest, IRemoteTest, ITestFilterReceiver,
IRuntimeHintProvider, ITestCollector, IBuildReceiver {

    static final String ANDROIDDEVICE = "AndroidDevice";
    static final String BUILD = "build";
    static final String BUILD_ID = "build_id";
    static final String BUILD_TARGET = "build_target";
    static final String DATA_FILE_PATH = "data_file_path";
    static final String LOG_PATH = "log_path";
    static final String NAME = "name";
    static final String PYTHONPATH = "PYTHONPATH";
    static final String SERIAL = "serial";
    static final String TEST_SUITE = "test_suite";
    static final String VTS = "vts";
    static final String CONFIG_FILE_EXTENSION = ".config";
    static final String INCLUDE_FILTER = "include_filter";
    static final String EXCLUDE_FILTER = "exclude_filter";
    static final String TEST_RUN_SUMMARY_FILE_NAME = "test_run_summary.json";
    static final float DEFAULT_TARGET_VERSION = -1;
    static final String DEFAULT_TESTCASE_CONFIG_PATH = "vts/tools/vts-tradefed/res/default/DefaultTestCase.config";

    private ITestDevice mDevice = null;

    @Option(name = "test-timeout", description = "maximum amount of time"
            + "(im milliseconds) tests are allowed to run",
            isTimeVal = true)
    private static long TEST_TIMEOUT = 1000 * 60 * 5;

    @Option(name = "test-case-path",
            description = "The path for test case.")
    private String mTestCasePath = null;

    @Option(name = "test-config-path",
            description = "The path for test case config file.")
    private String mTestConfigPath = null;

    @Option(name = "use-stdout-logs",
            description = "Flag that determines whether to use std:out to parse output.")
    private boolean mUseStdoutLogs = true;

    @Option(name = "include-filter",
            description = "The positive filter of the test names to run.")
    private Set<String> mIncludeFilters = new TreeSet<>();

    @Option(name = "exclude-filter",
            description = "The negative filter of the test names to run.")
    private Set<String> mExcludeFilters = new TreeSet<>();

    @Option(name = "runtime-hint", description = "The hint about the test's runtime.",
            isTimeVal = true)
    private long mRuntimeHint = 60000;  // 1 minute

    @Option(name = "collect-tests-only",
            description = "Only invoke the test binary to collect list of applicable test cases. "
                    + "All test run callbacks will be triggered, but test execution will "
                    + "not be actually carried out.")
    private boolean mCollectTestsOnly = false;

    // This variable is set in order to include the directory that contains the
    // python test cases. This is set before calling the method.
    // {@link #doRunTest(IRunUtil, String, String)}.
    public String mPythonPath = null;

    @Option(name = "python-binary", description = "python binary to use "
            + "(optional)")
    private String mPythonBin = null;
    private IRunUtil mRunUtil = null;
    private IBuildInfo mBuildInfo = null;
    private String mRunName = "VtsHostDrivenTest";
    // the path of a dir which contains the test data files.
    private String mTestCaseDataDir = "./";

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
    @Override
    public void addIncludeFilter(String filter) {
        mIncludeFilters.add(cleanFilter(filter));
    }

    /**
     * {@inheritDoc}
     */
    @Override
    public void addAllIncludeFilters(Set<String> filters) {
        for (String filter : filters) {
            mIncludeFilters.add(cleanFilter(filter));
        }
    }

    /**
     * {@inheritDoc}
     */
    @Override
    public void addExcludeFilter(String filter) {
        mExcludeFilters.add(cleanFilter(filter));
    }

    /**
     * {@inheritDoc}
     */
    @Override
    public void addAllExcludeFilters(Set<String> filters) {
        for (String filter : filters) {
            mExcludeFilters.add(cleanFilter(filter));
        }
    }

    /*
     * Conforms filters using a {@link com.android.ddmlib.testrunner.TestIdentifier} format
     * to be recognized by the GTest executable.
     */
    private String cleanFilter(String filter) {
        return filter.replace('#', '.');
    }

    /**
     * {@inheritDoc}
     */
    @Override
    public long getRuntimeHint() {
        return mRuntimeHint;
    }

    /**
     * {@inheritDoc}
     */
    @Override
    public void setCollectTestsOnly(boolean shouldCollectTest) {
        mCollectTestsOnly = shouldCollectTest;
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

        setPythonPath();

        if (mPythonBin == null) {
            mPythonBin = getPythonBinary();
        }

        if (mRunUtil == null){
            mRunUtil = new RunUtil();
            mRunUtil.setEnvVariable(PYTHONPATH, mPythonPath);
        }

        doRunTest(listener);
    }

    @Override
    public void setBuild(IBuildInfo buildInfo) {
        mBuildInfo = buildInfo;
    }

    /**
     * Populate a jsonObject with default fields.
     * This method uses deepMergeJsonObjects method from JsonUtil to merge the default config file with the target
     * config file. Field already defined in target config file will not be overwritten. Also, JSONArray will not be
     * deep merged.
     *
     * @param jsonObject the target json object to populate
     * @param testCaseDataDir data file path
     * @throws IOException
     * @throws JSONException
     */
    private void populateDefaultJsonFields(JSONObject jsonObject, String testCaseDataDir)
            throws IOException, JSONException {
        CLog.i("Populating default fields to json object from %s", DEFAULT_TESTCASE_CONFIG_PATH);
        String content = FileUtil.readStringFromFile(new File(mTestCaseDataDir, DEFAULT_TESTCASE_CONFIG_PATH));
        JSONObject defaultJsonObject = new JSONObject(content);

        JsonUtil.deepMergeJsonObjects(jsonObject, defaultJsonObject);
    }

    /**
     * This method reads the provided VTS runner test json config, adds or updates some of its
     * fields (e.g., to add build info and device serial IDs), and returns the updated json object.
     * This method calls populateDefaultJsonFields to populate the config JSONObject if the config file is missing
     * or some required fields is missing from the JSONObject. If test name is not specified, this method
     * will use the config file's file name file without extension as test name. If config file is missing, this method
     * will use the test case's class name as test name.
     *
     * @param log_path the path of a directory to store the VTS runner logs.
     * @return the updated JSONObject as the new test config.
     */
    private JSONObject getUpdatedVtsRunnerTestConfig(String log_path)
            throws IOException, JSONException, RuntimeException {
        JSONObject jsonObject = null;
        CLog.i("Load original test config %s %s", mTestCaseDataDir, mTestConfigPath);
        String content = null;

        if (mTestConfigPath != null) {
            content = FileUtil.readStringFromFile(new File(Paths.get(mTestCaseDataDir, mTestConfigPath).toString()));
        }

        CLog.i("Loaded original test config %s", content);
        if (content != null) {
            jsonObject = new JSONObject(content);
        } else {
            jsonObject = new JSONObject();
        }
        populateDefaultJsonFields(jsonObject, mTestCaseDataDir);
        CLog.i("Built a Json object using the loaded original test config");

        JSONArray deviceArray = new JSONArray();
        JSONObject deviceItemObject = new JSONObject();
        deviceItemObject.put(SERIAL, mDevice.getSerialNumber());
        try {
            deviceItemObject.put("product_type", mDevice.getProductType());
            deviceItemObject.put("product_variant", mDevice.getProductVariant());
            deviceItemObject.put("build_alias", mDevice.getBuildAlias());
            deviceItemObject.put("build_id", mDevice.getBuildId());
            deviceItemObject.put("build_flavor", mDevice.getBuildFlavor());
        } catch (DeviceNotAvailableException e) {
            CLog.e("A device not available - continuing");
            throw new RuntimeException("Failed to get device information");
        }
        deviceArray.put(deviceItemObject);

        JSONArray testBedArray = (JSONArray) jsonObject.get("test_bed");
        if (testBedArray.length() == 0) {
            JSONObject device = new JSONObject();
            String testName;
            if (mTestConfigPath != null) {
                testName = new File(mTestConfigPath).getName();
                testName = testName.replace(CONFIG_FILE_EXTENSION, "");
            } else {
                testName = new File(mTestCasePath).getName();
            }
            CLog.logAndDisplay(LogLevel.INFO, "Setting test name as %s", testName);
            device.put(NAME, testName);
            device.put(ANDROIDDEVICE, deviceArray);
            testBedArray.put(device);
        } else if (testBedArray.length() == 1) {
            JSONObject device = (JSONObject) testBedArray.get(0);
            device.put(ANDROIDDEVICE, deviceArray);
        } else {
            CLog.e("Multi-device not yet supported: %d devices requested",
                    testBedArray.length());
            throw new RuntimeException("Failed to produce VTS runner test config");
        }
        jsonObject.put(DATA_FILE_PATH, mTestCaseDataDir);
        CLog.i("Added %s = %s to the Json object", DATA_FILE_PATH, mTestCaseDataDir);

        JSONObject build = new JSONObject();
        build.put(BUILD_ID, mBuildInfo.getBuildId());
        build.put(BUILD_TARGET, mBuildInfo.getBuildTargetName());
        jsonObject.put(BUILD, build);
        CLog.i("Added %s to the Json object", BUILD);

        JSONObject suite = new JSONObject();
        suite.put(NAME, mBuildInfo.getTestTag());
        suite.put(INCLUDE_FILTER, new JSONArray(mIncludeFilters));
        CLog.i("Added include filter to test suite: %s", mIncludeFilters);
        suite.put(EXCLUDE_FILTER, new JSONArray(mExcludeFilters));
        CLog.i("Added exclude filter to test suite: %s", mExcludeFilters);
        jsonObject.put(TEST_SUITE, suite);
        CLog.i("Added %s to the Json object", TEST_SUITE);

        jsonObject.put(LOG_PATH, log_path);
        CLog.i("Added %s to the Json object", LOG_PATH);
        return jsonObject;
    }

    /**
     * This method prepares a command for the test and runs the python file as
     * given in the arguments.
     *
     * @param listener
     * @param runUtil
     * @param mTestCasePath
     * @param mTestConfigPath
     * @throws RuntimeException
     * @throws IllegalArgumentException
     */
    private void doRunTest(ITestRunListener listener) throws RuntimeException, IllegalArgumentException {
        CLog.i("Device serial number: " + mDevice.getSerialNumber());

        JSONObject jsonObject = null;
        File vtsRunnerLogDir = null;
        try {
            vtsRunnerLogDir = FileUtil.createTempDir("vts-runner-log");
            jsonObject = getUpdatedVtsRunnerTestConfig(vtsRunnerLogDir.getAbsolutePath());
        } catch(IOException e) {
            throw new RuntimeException("Failed to read test config json file");
        } catch(JSONException e) {
            throw new RuntimeException("Failed to build updated test config json data");
        }
        CLog.i("config json: %s", jsonObject.toString());

        String jsonFilePath = null;
        try {
            File tmpFile = FileUtil.createTempFile(
                    mBuildInfo.getTestTag() + "-config-" + mBuildInfo.getDeviceSerial(), ".json");
            jsonFilePath = tmpFile.getAbsolutePath();
            CLog.i("config json file path: %s", jsonFilePath);
            FileWriter fw = new FileWriter(jsonFilePath);
            fw.write(jsonObject.toString());
            fw.close();
        } catch(IOException e) {
            throw new RuntimeException("Failed to create device config json file");
        }

        if (mPythonBin == null){
            mPythonBin = getPythonBinary();
        }
        String[] baseOpts = {mPythonBin, "-m"};
        String[] testModule = {mTestCasePath, jsonFilePath};
        String[] cmd;
        cmd = ArrayUtil.buildArray(baseOpts, testModule);

        CommandResult commandResult = mRunUtil.runTimedCmd(TEST_TIMEOUT, cmd);

        if (commandResult != null && commandResult.getStatus() !=
                CommandStatus.SUCCESS) {
            CLog.e("Python process failed");
            CLog.e("Python path: %s", mPythonPath);
            CLog.e("Stderr: %s", commandResult.getStderr());
            CLog.e("Stdout: %s", commandResult.getStdout());
            printVtsLogs(vtsRunnerLogDir);
            throw new RuntimeException("Failed to run VTS test");
        }
        if (commandResult != null){
            CLog.i("Standard output is: %s", commandResult.getStdout());
            CLog.i("Parsing test result: %s", commandResult.getStderr());
        }

        VtsMultiDeviceTestResultParser parser = new VtsMultiDeviceTestResultParser(listener,
                mRunName);

        if (mUseStdoutLogs) {
            if (commandResult.getStdout() == null) {
                CLog.e("The std:out is null for CommandResult.");
                throw new RuntimeException("The std:out is null for CommandResult.");
            }
            parser.processNewLines(commandResult.getStdout().split("\n"));
        } else {
            // parse from test_run_summary.json instead of std:out
            String jsonData = null;
            JSONObject object = null;
            File testRunSummary = getFileTestRunSummary(vtsRunnerLogDir);
            if (testRunSummary == null) {
                throw new RuntimeException("Couldn't locate the file : " +
                        TEST_RUN_SUMMARY_FILE_NAME);
            }
            try {
                jsonData = FileUtil.readStringFromFile(testRunSummary);
                object = new JSONObject(jsonData);
            } catch (IOException e) {
                CLog.e("Error occurred in parsing Json file : %s", testRunSummary.toPath());
            } catch (JSONException e) {
                CLog.e("Error occurred in parsing Json String : %s", jsonData);
            }
            if (object == null) {
                CLog.e("Json object is null.");
                throw new RuntimeException("Json object is null.");
            }
            parser.processJsonFile(object);
        }
        printVtsLogs(vtsRunnerLogDir);
    }

    /**
     * This method return the file test_run_details.txt which is then used to parse logs.
     *
     * @param logDir : The file that needs to be converted
     * @return the file named test_run_details.txt
     * @throws IllegalArgumentException
     */
    private File getFileTestRunSummary (File logDir) throws IllegalArgumentException {
        File[] children;
        if (logDir == null) {
            throw new IllegalArgumentException("Argument logDir is null.");
        }
        children = logDir.listFiles();
        if (children != null) {
            for (File child : children) {
                if (!child.isDirectory()) {
                    if (child.getName().equals(TEST_RUN_SUMMARY_FILE_NAME)) {
                        return child;
                    }
                } else {
                    File file = getFileTestRunSummary(child);
                    if (file != null) {
                        return file;
                    }
                }
            }
        }
        return null;
    }

    /**
     * The method prints all VTS runner log files
     *
     * @param logDir the File instance of the base log dir.
     */
    private void printVtsLogs(File logDir) {
        File[] children;
        if (logDir == null) {
            CLog.e("Scan VTS log dir: null\n");
            return;
        }
        CLog.i("Scan VTS log dir %s\n", logDir.getAbsolutePath());
        children = logDir.listFiles();
        if (children != null) {
            for (File child : children) {
                if (child.isDirectory()) {
                    printVtsLogs(child);
                } else {
                    CLog.i("VTS log file %s\n", child.getAbsolutePath());
                    try {
                        if (child.getName().equals("vts_agent.log")) {
                            CLog.i("Content: %s\n", FileUtil.readStringFromFile(child));
                        } else {
                            CLog.i("skip %s\n", child.getName());
                        }
                    } catch (IOException e) {
                        CLog.e("I/O error\n");
                    }
                }
            }
        }
    }

    /**
     * This method sets the python path. It's based on the based on the
     * assumption that the environment variable $ANDROID_BUILD_TOP is set.
     */
    private void setPythonPath() {
        StringBuilder sb = new StringBuilder();
        sb.append(System.getenv(PYTHONPATH));

        // to get the path for android-vts/testcases/ which keeps the VTS python code under vts.
        if (mBuildInfo != null) {
            CompatibilityBuildHelper mBuildHelper = new CompatibilityBuildHelper(mBuildInfo);
            mBuildHelper.init(VTS, null);

            File testDir = null;
            try {
                testDir = mBuildHelper.getTestsDir();
            } catch(FileNotFoundException e) {
                /* pass */
            }
            if (testDir != null) {
                sb.append(":");
                mTestCaseDataDir = testDir.getAbsolutePath();
                sb.append(mTestCaseDataDir);
            } else if (mBuildInfo.getFile(VTS) != null) {
                sb.append(":");
                sb.append(mBuildInfo.getFile(VTS).getAbsolutePath()).append("/..");
            }
        }

        // for when one uses PythonVirtualenvPreparer.
        if (mBuildInfo.getFile(PYTHONPATH) != null) {
            sb.append(":");
            sb.append(mBuildInfo.getFile(PYTHONPATH).getAbsolutePath());
        }
        if (System.getenv("ANDROID_BUILD_TOP") != null) {
            sb.append(":");
            sb.append(System.getenv("ANDROID_BUILD_TOP")).append("/test");
        }
        mPythonPath = sb.toString();
        CLog.i("mPythonPath: %s", mPythonPath);
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
