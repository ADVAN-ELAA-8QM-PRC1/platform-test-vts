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

import com.android.vts.helpers.BigtableHelper;
import com.google.appengine.api.users.User;
import com.google.appengine.api.users.UserService;
import com.google.appengine.api.users.UserServiceFactory;
import org.apache.commons.lang.StringUtils;
import org.apache.hadoop.hbase.Cell;
import org.apache.hadoop.hbase.HTableDescriptor;
import org.apache.hadoop.hbase.TableName;
import org.apache.hadoop.hbase.client.Get;
import org.apache.hadoop.hbase.client.Result;
import org.apache.hadoop.hbase.client.Table;
import org.apache.hadoop.hbase.util.Bytes;

import java.io.IOException;
import java.util.ArrayList;
import java.util.Collections;
import java.util.HashSet;
import java.util.List;
import java.util.Set;
import java.util.logging.Level;
import javax.servlet.RequestDispatcher;
import javax.servlet.ServletException;
import javax.servlet.http.HttpServletRequest;
import javax.servlet.http.HttpServletResponse;

/**
 * Represents the servlet that is invoked on loading the first page of dashboard.
 */
public class DashboardMainServlet extends BaseServlet {

    private static final String DASHBOARD_MAIN_JSP = "/dashboard_main.jsp";
    private static final String DASHBOARD_ALL_LINK = "/?showAll=true";
    private static final String DASHBOARD_FAVORITES_LINK = "/";
    private static final byte[] EMAIL_FAMILY = Bytes.toBytes("email_to_test");
    private static final String STATUS_TABLE = "vts_status_table";
    private static final String ALL_HEADER = "All Tests";
    private static final String FAVORITES_HEADER = "Favorites";
    private static final String NO_FAVORITES_ERROR = "No subscribed tests. Click the edit button to add to favorites.";
    private static final String NO_TESTS_ERROR = "No test results available.";
    private static final String FAVORITES_BUTTON = "Show Favorites";
    private static final String ALL_BUTTON = "Show All";
    private static final String UP_ARROW = "keyboard_arrow_up";
    private static final String DOWN_ARROW = "keyboard_arrow_down";


    @Override
    public void doGetHandler(HttpServletRequest request, HttpServletResponse response)
            throws IOException {
        UserService userService = UserServiceFactory.getUserService();
        User currentUser = userService.getCurrentUser();
        RequestDispatcher dispatcher = null;

        Table table = BigtableHelper.getTable(TableName.valueOf(STATUS_TABLE));
        List<String> displayedTests = new ArrayList<>();

        HTableDescriptor[] tables = BigtableHelper.getTables();
        Set<String> allTables = new HashSet<>();
        boolean showAll = request.getParameter("showAll") != null;
        String header;
        String buttonLabel;
        String buttonIcon;
        String buttonLink;
        String error = null;

        for (HTableDescriptor descriptor : tables) {
            String tableName = descriptor.getNameAsString();
            if (tableName.startsWith(TABLE_PREFIX)) {
                allTables.add(tableName);
            }
        }

        if (showAll) {
            for (String tableName : allTables) {
                displayedTests.add(tableName.substring(7));
            }
            if (displayedTests.size() == 0) {
                error = NO_TESTS_ERROR;
            }
            header = ALL_HEADER;
            buttonLabel = FAVORITES_BUTTON;
            buttonIcon = UP_ARROW;
            buttonLink = DASHBOARD_FAVORITES_LINK;
        } else {
            String email = currentUser.getEmail().trim().toLowerCase();
            if (!StringUtils.isBlank(email)) {
                Get get = new Get(Bytes.toBytes(currentUser.getEmail()));
                get.addFamily(EMAIL_FAMILY);
                Result result = table.get(get);
                if (result != null) {
                    List<Cell> cells = result.listCells();
                    if (cells != null) {
                        for (Cell cell : cells) {
                            String test = Bytes.toString(cell.getQualifierArray());
                            String val = Bytes.toString(cell.getValueArray());
                            if (test != null && val.equals("1") && allTables.contains(test)) {
                                displayedTests.add(test.substring(TABLE_PREFIX.length()));
                            }
                        }
                    }
                }
            }
            if (displayedTests.size() == 0) {
                error = NO_FAVORITES_ERROR;
            }
            header = FAVORITES_HEADER;
            buttonLabel = ALL_BUTTON;
            buttonIcon = DOWN_ARROW;
            buttonLink = DASHBOARD_ALL_LINK;
        }
        Collections.sort(displayedTests);

        String[] testArray = new String[displayedTests.size()];
        displayedTests.toArray(testArray);
        response.setStatus(HttpServletResponse.SC_OK);
        request.setAttribute("testNames", testArray);
        request.setAttribute("headerLabel", header);
        request.setAttribute("showAll", showAll);
        request.setAttribute("buttonLabel", buttonLabel);
        request.setAttribute("buttonIcon", buttonIcon);
        request.setAttribute("buttonLink", buttonLink);
        request.setAttribute("error", error);
        dispatcher = request.getRequestDispatcher(DASHBOARD_MAIN_JSP);
        try {
            dispatcher.forward(request, response);
        } catch (ServletException e) {
            logger.log(Level.SEVERE, "Servlet Excpetion caught : ", e);
        }
    }
}
