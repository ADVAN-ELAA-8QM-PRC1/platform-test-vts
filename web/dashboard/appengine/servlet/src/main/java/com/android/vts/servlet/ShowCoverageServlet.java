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

package com.android.vts.servlet;

import com.android.vts.proto.VtsReportMessage;
import com.android.vts.proto.VtsReportMessage.CoverageReportMessage;
import com.android.vts.proto.VtsReportMessage.TestCaseReportMessage;
import com.android.vts.proto.VtsReportMessage.TestReportMessage;
import com.android.vts.util.BigtableHelper;
import com.google.gson.Gson;
import org.apache.hadoop.hbase.TableName;
import org.apache.hadoop.hbase.client.Result;
import org.apache.hadoop.hbase.client.ResultScanner;
import org.apache.hadoop.hbase.client.Scan;
import org.apache.hadoop.hbase.client.Table;
import org.apache.hadoop.hbase.util.Bytes;
import java.io.IOException;
import java.util.ArrayList;
import java.util.HashMap;
import java.util.List;
import java.util.Map;
import java.util.logging.Level;
import javax.servlet.RequestDispatcher;
import javax.servlet.ServletException;
import javax.servlet.http.HttpServletRequest;
import javax.servlet.http.HttpServletResponse;


/**
 * Servlet for handling requests to show code coverage.
 */
public class ShowCoverageServlet extends BaseServlet {

    private static final String COVERAGE_JSP = "WEB-INF/jsp/show_coverage.jsp";
    private static final byte[] FAMILY = Bytes.toBytes("test");
    private static final byte[] QUALIFIER = Bytes.toBytes("data");
    private static final String ALL_TESTCASES_LABEL = "All";

    @Override
    public List<String[]> getNavbarLinks(HttpServletRequest request) {
        List<String[]> links = new ArrayList<>();
        Page root = Page.HOME;
        String[] rootEntry = new String[]{root.getUrl(), root.getName()};
        links.add(rootEntry);

        Page table = Page.TABLE;
        String testName = request.getParameter("testName");
        String name = table.getName() + testName;
        String url = table.getUrl() + "?testName=" + testName;
        String[] tableEntry = new String[]{url, name};
        links.add(tableEntry);

        Page coverage = Page.COVERAGE;
        String startTime = request.getParameter("startTime");
        url = coverage.getUrl() + "?testName=" + testName + "&startTime=" + startTime;
        String[] coverageEntry = new String[]{url, coverage.getName()};
        links.add(coverageEntry);
        return links;
    }

    @Override
    public void doGetHandler(HttpServletRequest request, HttpServletResponse response)
            throws IOException {
        RequestDispatcher dispatcher = null;
        Table table = null;
        TableName tableName = null;
        tableName = TableName.valueOf(TABLE_PREFIX + request.getParameter("testName"));

        String timeString = request.getParameter("startTime");
        Scan scan = new Scan();
        long time = -1;
        try {
            time = Long.parseLong(timeString);
            scan.setStartRow(Long.toString(time - 1).getBytes());
            scan.setStopRow(Long.toString(time).getBytes());
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
                if (time == currentTestReportMessage.getStartTimestamp() * MILLI_TO_MICRO) {
                    testReportMessage = currentTestReportMessage;
                    break;
                }
            }
        }
        scanner.close();

        List<String> sourceFiles = new ArrayList<>();
        List<List<Integer>> coverageVectors = new ArrayList<>();
        List<String> projects = new ArrayList<>();
        List<String> commits = new ArrayList<>();
        List<String> indicators = new ArrayList<>();
        Map<String, List<Integer>> sectionMap = new HashMap<>();
        if (testReportMessage != null) {
            List<Integer> indices = new ArrayList<>();
            for (CoverageReportMessage coverageReportMessage : testReportMessage.getCoverageList()) {
               if (coverageReportMessage.getLineCoverageVectorCount() == 0 ||
                   !coverageReportMessage.hasFilePath() ||
                   !coverageReportMessage.hasProjectName() ||
                   !coverageReportMessage.hasRevision()) {
                   continue;
               }
               int total = coverageReportMessage.getTotalLineCount();
               int covered = coverageReportMessage.getCoveredLineCount();
               indices.add(coverageVectors.size());
               coverageVectors.add(coverageReportMessage.getLineCoverageVectorList());
               sourceFiles.add(coverageReportMessage.getFilePath().toStringUtf8());
               projects.add(coverageReportMessage.getProjectName().toStringUtf8());
               commits.add(coverageReportMessage.getRevision().toStringUtf8());
               String indicator = "";
               if (total > 0) {
                   double pct = Math.round(covered * 10000d / total) / 100d;
                   String color = pct >= 70 ? "green" : "red";
                   indicator = "<div class=\"right indicator " + color + "\">" +
                               pct + "%</div>" + "<span class=\"right total-count\">" +
                               covered + "/" + total + "</span>";
               }
               indicators.add(indicator);
            }
            if (indices.size() > 0) sectionMap.put(ALL_TESTCASES_LABEL, indices);
            for (TestCaseReportMessage testCaseReportMessage : testReportMessage.getTestCaseList()) {
                if (!testCaseReportMessage.hasName()) continue;
                indices = new ArrayList<>();
                for (CoverageReportMessage coverageReportMessage :
                     testCaseReportMessage.getCoverageList()) {
                    if (coverageReportMessage.getLineCoverageVectorCount() == 0 ||
                        !coverageReportMessage.hasFilePath() ||
                        !coverageReportMessage.hasProjectName() ||
                        !coverageReportMessage.hasRevision()) {
                        continue;
                    }
                    int total = coverageReportMessage.getTotalLineCount();
                    int covered = coverageReportMessage.getCoveredLineCount();
                    indices.add(coverageVectors.size());
                    coverageVectors.add(coverageReportMessage.getLineCoverageVectorList());
                    sourceFiles.add(coverageReportMessage.getFilePath().toStringUtf8());
                    projects.add(coverageReportMessage.getProjectName().toStringUtf8());
                    commits.add(coverageReportMessage.getRevision().toStringUtf8());
                    String indicator = "";
                    if (total > 0) {
                        double pct = Math.round(covered * 10000d / total) / 100d;
                        String color = pct >= 70 ? "green" : "red";
                        indicator = "<div class=\"right indicator " + color + "\">" +
                                    pct + "%</div>" + "<span class=\"right total-count\">" +
                                    covered + "/" + total + "</span>";
                    }
                    indicators.add(indicator);
                }
                if (indices.size() > 0) {
                    sectionMap.put(testCaseReportMessage.getName().toStringUtf8(), indices);
                }
            }
        }

        request.setAttribute("testName", request.getParameter("testName"));
        request.setAttribute("gerritURI", new Gson().toJson(GERRIT_URI));
        request.setAttribute("gerritScope", new Gson().toJson(GERRIT_SCOPE));
        request.setAttribute("clientId", new Gson().toJson(CLIENT_ID));
        request.setAttribute("coverageVectors", new Gson().toJson(coverageVectors));
        request.setAttribute("sourceFiles", new Gson().toJson(sourceFiles));
        request.setAttribute("projects", new Gson().toJson(projects));
        request.setAttribute("commits", new Gson().toJson(commits));
        request.setAttribute("indicators", new Gson().toJson(indicators));
        request.setAttribute("sectionMap", new Gson().toJson(sectionMap));
        request.setAttribute("startTime", request.getParameter("startTime"));
        dispatcher = request.getRequestDispatcher(COVERAGE_JSP);

        try {
            dispatcher.forward(request, response);
        } catch (ServletException e) {
            logger.log(Level.SEVERE, "Servlet Exception caught : ", e);
        }
    }
}
