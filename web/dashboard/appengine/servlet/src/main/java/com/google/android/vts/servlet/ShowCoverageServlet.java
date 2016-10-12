/*
 * Copyright (c) 2016 Google Inc. All Rights Reserved.
 *
 * Licensed under the Apache License, Version 2.0 (the "License"); you
 * may not use this file except in compliance with the License. You may
 * obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or
 * implied. See the License for the specific language governing
 * permissions and limitations under the License.
 */

package com.google.android.vts.servlet;

import com.google.android.vts.proto.VtsReportMessage;
import com.google.android.vts.proto.VtsReportMessage.CoverageReportMessage;
import com.google.android.vts.proto.VtsReportMessage.TestCaseReportMessage;
import com.google.android.vts.proto.VtsReportMessage.TestReportMessage;
import com.google.appengine.api.users.User;
import com.google.appengine.api.users.UserService;
import com.google.appengine.api.users.UserServiceFactory;
import org.apache.hadoop.hbase.TableName;
import org.apache.hadoop.hbase.client.Result;
import org.apache.hadoop.hbase.client.ResultScanner;
import org.apache.hadoop.hbase.client.Scan;
import org.apache.hadoop.hbase.client.Table;
import org.apache.hadoop.hbase.util.Bytes;
import org.slf4j.Logger;
import org.slf4j.LoggerFactory;
import java.io.File;
import java.io.IOException;
import java.util.ArrayList;
import java.util.List;
import javax.servlet.RequestDispatcher;
import javax.servlet.ServletException;
import javax.servlet.annotation.WebServlet;
import javax.servlet.http.HttpServlet;
import javax.servlet.http.HttpServletRequest;
import javax.servlet.http.HttpServletResponse;


/**
 * Servlet for handling requests to show code coverage.
 */
@WebServlet(name = "show_coverage", urlPatterns = {"/show_coverage"})
public class ShowCoverageServlet extends HttpServlet {

    private static final byte[] FAMILY = Bytes.toBytes("test");
    private static final byte[] QUALIFIER = Bytes.toBytes("data");
    private static final String TABLE_PREFIX = "result_";
    private static final Logger logger = LoggerFactory.getLogger(ShowCoverageServlet.class);

    @Override
    public void doGet(HttpServletRequest request, HttpServletResponse response) throws IOException {
        UserService userService = UserServiceFactory.getUserService();
        User currentUser = userService.getCurrentUser();
        String loginURI = userService.createLoginURL(request.getRequestURI());
        String logoutURI = userService.createLogoutURL(loginURI);
        RequestDispatcher dispatcher = null;
        Table table = null;
        TableName tableName = null;
        tableName = TableName.valueOf(TABLE_PREFIX + request.getParameter("testName"));

        // key is a unique combination of build Id and timestamp that helps identify the
        // corresponding build id.
        String key = request.getParameter("key");
        Scan scan = new Scan();
        long time = -1;
        try {
            time = Long.parseLong(key);
            scan.setStartRow(key.getBytes());
            scan.setStopRow(Long.toString((time + 1)).getBytes());
        } catch (NumberFormatException e) { }  // Use unbounded scan

        TestReportMessage testReportMessage = null;

        table = BigtableHelper.getTable(tableName);
        ResultScanner scanner = table.getScanner(scan);
        for (Result result = scanner.next(); result != null; result = scanner.next()) {
            byte[] value = result.getValue(FAMILY, QUALIFIER);
            TestReportMessage currentTestReportMessage = VtsReportMessage.TestReportMessage.
                parseFrom(value);
            String buildId = currentTestReportMessage.getBuildInfo().getId().toStringUtf8();

            // filter empty build IDs and add only numbers
            if (buildId.length() > 0) {
                try {
                    Integer.parseInt(buildId);
                    if (time == currentTestReportMessage.getStartTimestamp()) {
                      testReportMessage = currentTestReportMessage;
                      break;
                    }
                } catch (NumberFormatException e) {
                    /* skip a non-post-submit build */
                }
            }
        }
        scanner.close();

        List<List<String>> coverageInfo = new ArrayList<>();
        for (TestCaseReportMessage testCaseReportMessage : testReportMessage.getTestCaseList()) {
            for (CoverageReportMessage coverageReportMessage : testCaseReportMessage.getCoverageList()) {
                ArrayList<String> entry = new ArrayList<>();
                String dirPath = coverageReportMessage.getDirPath().toStringUtf8();
                String fileName = coverageReportMessage.getFileName().toStringUtf8();
                String path = dirPath + File.separator + fileName;
                entry.add(testCaseReportMessage.getName().toStringUtf8());
                entry.add(path);
                entry.add(coverageReportMessage.getHtml().toStringUtf8());
                coverageInfo.add(entry);
            }
        }
        request.setAttribute("logoutURL", logoutURI);
        request.setAttribute("email", currentUser.getEmail());
        request.setAttribute("testName", request.getParameter("testName"));
        request.setAttribute("coverageInfo", coverageInfo);
        request.setAttribute("startTime", request.getParameter("startTime"));
        request.setAttribute("endTime", request.getParameter("endTime"));
        response.setContentType("text/plain");
        dispatcher = request.getRequestDispatcher("/show_coverage.jsp");

        try {
            dispatcher.forward(request, response);
        } catch (ServletException e) {
            logger.error("Servlet Excpetion caught : ", e);
        }
    }
}
