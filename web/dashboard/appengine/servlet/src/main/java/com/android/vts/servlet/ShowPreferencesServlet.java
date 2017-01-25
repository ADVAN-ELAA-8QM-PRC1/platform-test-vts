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

import com.android.vts.util.BigtableHelper;
import com.google.appengine.api.users.User;
import com.google.appengine.api.users.UserService;
import com.google.appengine.api.users.UserServiceFactory;
import com.google.gson.Gson;
import org.apache.commons.lang.StringUtils;
import org.apache.hadoop.hbase.Cell;
import org.apache.hadoop.hbase.HTableDescriptor;
import org.apache.hadoop.hbase.TableName;
import org.apache.hadoop.hbase.client.Delete;
import org.apache.hadoop.hbase.client.Get;
import org.apache.hadoop.hbase.client.Put;
import org.apache.hadoop.hbase.client.Result;
import org.apache.hadoop.hbase.client.Table;
import org.apache.hadoop.hbase.util.Bytes;

import java.io.IOException;
import java.util.ArrayList;
import java.util.HashSet;
import java.util.List;
import java.util.Set;
import java.util.logging.Level;
import javax.servlet.RequestDispatcher;
import javax.servlet.ServletException;
import javax.servlet.http.HttpServletRequest;
import javax.servlet.http.HttpServletResponse;

/**
 * Represents the servlet that is invoked on loading the preferences page to manage favorites.
 */
public class ShowPreferencesServlet extends BaseServlet {

    private static final String PREFERENCES_JSP = "WEB-INF/jsp/show_preferences.jsp";
    private static final String DASHBOARD_MAIN_LINK = "/";
    private static final byte[] EMAIL_FAMILY = Bytes.toBytes("email_to_test");
    private static final byte[] TEST_FAMILY = Bytes.toBytes("test_to_email");
    private static final String STATUS_TABLE = "vts_status_table";

    @Override
    public List<String[]> getNavbarLinks(HttpServletRequest request) {
        List<String[]> links = new ArrayList<>();
        Page root = Page.HOME;
        String[] rootEntry = new String[]{root.getUrl(), root.getName()};
        links.add(rootEntry);

        Page prefs = Page.PREFERENCES;
        String[] prefsEntry = new String[]{CURRENT_PAGE, prefs.getName()};
        links.add(prefsEntry);
        return links;
    }

    @Override
    public void doGetHandler(HttpServletRequest request, HttpServletResponse response)
            throws IOException {
        // Get the user's information
        UserService userService = UserServiceFactory.getUserService();
        User currentUser = userService.getCurrentUser();
        RequestDispatcher dispatcher = null;

        Table table = BigtableHelper.getTable(TableName.valueOf(STATUS_TABLE));
        List<String> subscribedTests = new ArrayList<>();

        String email = currentUser.getEmail().trim().toLowerCase();
        if (!StringUtils.isBlank(email)) {
            /* Get all cells for the row key = user email address within the column family EMAIL_FAMILY.
             * Data is stored in the column qualifiers (test names) to represent a sparse
             * adjacency matrix.
             */
            Get get = new Get(Bytes.toBytes(currentUser.getEmail()));
            get.addFamily(EMAIL_FAMILY);
            Result result = table.get(get);
            if (result != null) {
                List<Cell> cells = result.listCells();
                if (cells != null) {
                    for (Cell cell : cells) {
                        String test = Bytes.toString(cell.getQualifierArray());
                        String val = Bytes.toString(cell.getValueArray());
                        if (test != null && test.startsWith(TABLE_PREFIX) && val.equals("1")) {
                            subscribedTests.add(test.substring(TABLE_PREFIX.length()));
                        }
                    }
                }
            }
        }

        HTableDescriptor[] tables = BigtableHelper.getTables();
        List<String> allTests = new ArrayList<>();

        for (HTableDescriptor descriptor : tables) {
            String tableName = descriptor.getNameAsString();
            if (tableName.startsWith(TABLE_PREFIX)) {
                allTests.add(tableName.substring(TABLE_PREFIX.length()));
            }
        }

        request.setAttribute("allTestsJson", new Gson().toJson(allTests));
        request.setAttribute("subscribedTests", subscribedTests);
        request.setAttribute("subscribedTestsJson", new Gson().toJson(subscribedTests));
        dispatcher = request.getRequestDispatcher(PREFERENCES_JSP);
        try {
            dispatcher.forward(request, response);
        } catch (ServletException e) {
            logger.log(Level.SEVERE, "Servlet Excpetion caught : ", e);
        }
    }

    @Override
    public void doPost(HttpServletRequest request, HttpServletResponse response) throws IOException {
        // Get the current user's information.
        UserService userService = UserServiceFactory.getUserService();
        User currentUser = userService.getCurrentUser();
        String email = currentUser.getEmail().trim().toLowerCase();

        // Retrieve the added tests from the request.
        String addedTestsString = request.getParameter("addedTests");
        String[] addedTests = new String[0];
        if (!StringUtils.isBlank(addedTestsString)) {
            addedTests = addedTestsString.trim().split(",");
        }

        // Retrieve the removed tests from the request.
        String removedTestsString = request.getParameter("removedTests");
        String[] removedTests = new String[0];
        if (!StringUtils.isBlank(removedTestsString)) {
            removedTests = removedTestsString.trim().split(",");
        }

        // Get all of the table containing test data.
        HTableDescriptor[] tables = BigtableHelper.getTables();
        Set<String> allTables = new HashSet<>();
        for (HTableDescriptor descriptor : tables) {
            String tableName = descriptor.getNameAsString();
            if (tableName.startsWith(TABLE_PREFIX)) {
                allTables.add(tableName);
            }
        }

        if (!StringUtils.isBlank(email)) {
            Table table = BigtableHelper.getTable(TableName.valueOf(STATUS_TABLE));
            // Add subscriptions
            for (String test : addedTests) {
                String tableName = TABLE_PREFIX + test;
                if (StringUtils.isBlank(test) || !allTables.contains(tableName)) continue;
                List<Put> puts = new ArrayList<>();

                // Create the edge <test name> --> <email address> in the adjacency matrix
                Put put = new Put(Bytes.toBytes(tableName));
                put.addColumn(TEST_FAMILY, Bytes.toBytes(email), Bytes.toBytes("1"));
                puts.add(put);

                // Create the edge <email address> --> <test name> in the adjacency matrix
                put = new Put(Bytes.toBytes(email));
                put.addColumn(EMAIL_FAMILY, Bytes.toBytes(tableName), Bytes.toBytes("1"));
                puts.add(put);
                table.put(puts);
            }

            // Remove subscriptions
            for (String test : removedTests) {
                String tableName = TABLE_PREFIX + test;
                if (StringUtils.isBlank(test)) continue;
                List<Delete> deletes = new ArrayList<>();

                // Delete the edge <test name> --> <email address> from the adjacency matrix
                Delete del = new Delete(Bytes.toBytes(tableName));
                del.addColumns(TEST_FAMILY, Bytes.toBytes(email));
                deletes.add(del);

                // Delete the edge <email address> --> <test name> from the adjacency matrix
                del = new Delete(Bytes.toBytes(email));
                del.addColumns(EMAIL_FAMILY, Bytes.toBytes(tableName));
                deletes.add(del);
                table.delete(deletes);
            }
        }
        response.sendRedirect(DASHBOARD_MAIN_LINK);
    }
}
