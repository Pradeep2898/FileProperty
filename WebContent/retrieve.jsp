<%@ page contentType="text/html;charset=UTF-8" language="java" %>
<%@ page import="java.sql.*" %>

<html>
<head>
    <title>Retrieve</title>
</head>
<body>
<center>
<h1>Retrieve data from database</h1>
<table border="3">
    <tr>
        <td><b><i>Who</i></b></td>
        <td><b><i>For</i></b></td>
        <td><b><i>File Path</i></b></td>
        <td><b><i>Operation</i></b></td>
        <td><b><i>Email-id</i></b></td>
        <td><b><i>When</i></b></td>
    </tr>
<%
    try{
        Class.forName("com.mysql.jdbc.Driver");
        Connection conn = DriverManager.getConnection("jdbc:mysql://sql12.freemysqlhosting.net:3306/sql12360017","sql12360017","4h2pX2xIP2");
        Statement statement=conn.createStatement();
        String sql ="select * from demo";
        ResultSet resultSet = statement.executeQuery(sql);
        while(resultSet.next()){
%>
    <tr>
        <td><%=resultSet.getString("Username") %></td>
        <td><%=resultSet.getString("User") %></td>
        <td><%=resultSet.getString("Path") %></td>
        <td><%=resultSet.getString("Permission") %></td>
        <td><%=resultSet.getString("Email id") %></td>
        <td><%=resultSet.getString("Time") %></td>
    </tr>
    <%
        }
            conn.close();
        } catch (Exception e) {
            out.print(e);
            e.printStackTrace();
        }
    %>
</table>
</center>
</body>
</html>