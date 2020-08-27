import java.io.*;
import java.net.URLEncoder;
import java.sql.*;
import javax.servlet.*;
import javax.servlet.http.*;

public class User extends HttpServlet{
	
	private static final long serialVersionUID = 1L;
	
	public void doGet(HttpServletRequest req,HttpServletResponse res)throws ServletException,IOException  
	{  
		String email = req.getParameter("email");
        String password = req.getParameter("password");
        String username = req.getParameter("name");
        if(!email.isEmpty() || !password.isEmpty() || !username.isEmpty())
        {
        /*Class.forName("com.mysql.jdbc.Driver");
        Connection conn = DriverManager.getConnection("jdbc:mysql://sql12.freemysqlhosting.net:3306/sql12358721","sql12358721","7t8epd1Iej");
        Statement st=conn.createStatement();
        st.executeUpdate("insert into user(username,email,password)values('" + username + "','" + email + "','" + password + "')");*/
        res.sendRedirect("index.jsp");
        }
        else
        {
        	String message = "FILL ALL FIELD";
            req.setAttribute("message", message);
            RequestDispatcher dispatcher = req.getRequestDispatcher("register.jsp");
            dispatcher.forward(req, res);
        }
	}
	
	public void doPost(HttpServletRequest req, HttpServletResponse res) throws ServletException, IOException 
	{
		String email = req.getParameter("email");
        String password = req.getParameter("password");
        /*
        Class.forName("com.mysql.jdbc.Driver");
        Connection conn = DriverManager.getConnection("jdbc:mysql://sql12.freemysqlhosting.net:3306/sql12358721","sql12358721","7t8epd1Iej");
        Statement st=conn.createStatement();
        ResultSet rs=st.executeQuery("select * from user where email='"+email+"' and password='"+password+"'");
        rs.next();
      	if(rs.getString("password").equals(password)&&rs.getString("email").equals(email))
       	{
      		res.sendRedirect("process.jsp");
       	}
      	else
      	{
      		String message = "INVALID EMAIL/PASSWORD";
            req.setAttribute("message", message);
            RequestDispatcher dispatcher = req.getRequestDispatcher("index.jsp");
            dispatcher.forward(req, res);
      	}*/   
	}
}