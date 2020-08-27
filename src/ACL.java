
import java.io.*;
import java.net.URLEncoder;
import java.sql.*;
import javax.servlet.*;
import javax.servlet.http.*;

public class ACL extends HttpServlet{
	
	private static final long serialVersionUID = 1L;
	
	public void doGet(HttpServletRequest req,HttpServletResponse res)throws ServletException,IOException  
	{  
	   Details obj =new Details();
	   int len;
	   String pathDir = req.getParameter("path");
	   res.setContentType("text/html");		//setting the content type
	   PrintWriter pw=res.getWriter();		//get the stream to write the data  
	   pw.println("<html><body><center>");
	   pw.println("<h3>Welcome to servlet,File owner is: <b>");
	   pw.println(obj.sayHello(pathDir));
	   pw.println("</b></h3><br><br>");
	   String[] prop = obj.returndays(pathDir);
	   pw.println("The File property is ::<br><table><tr><th>User or Group Name</th><th>Permission[These checkboxes are not modifiable]</th></tr>");
	   len=prop.length;
	   String[] arr = new String[len];
	   for(int i=0;i<len;i++)
	   {
		 String[] parts = prop[i].split("\\\\");
		 arr[i] = parts[0];
	     pw.println("<tr><td>");
	     pw.println(parts[0]+"\\ "+parts[1]);
	     pw.println("</td><td>");
	     if(parts[2].contains("Full Control"))
	     {
	    	 pw.println("<label><input type='checkbox' value='FullControl' checked='checked' onclick='return false;' name='"+parts[0]+"FullControl"+"'/>Full Control </label>"
	    	 		+ "<label><input type='checkbox' value='Read' checked='checked' onclick='return false;'/>Read </label>"
	    	 		+ "<label><input type='checkbox' value='Write' checked='checked' onclick='return false;'/>Write </label>"
	    	 		+ "<label><input type='checkbox' value='Execute' checked='checked' onclick='return false;'/>Execute</label>");
	     } 
	     else
	     {
	    	pw.println("<label><input type='checkbox' value='FullControl' onclick='return false;' name='"+parts[0]+"FullControl"+"'/>Full Control</label>");
	    	if(parts[2].contains("Read")){
	    		pw.println("<label><input type='checkbox' value='Read' checked='checked' onclick='return false;'name='"+parts[0]+"ReadControl"+"'/>Read</label>");
	    	}
	        if(parts[2].contains("Execute")) {
	        	pw.println("<label><input type='checkbox' value='Execute' checked='checked' onclick='return false;'name='"+parts[0]+"ExecuteControl"+"'/>Execute</label>");
	        }
	        if(parts[2].contains("Write")) {
	        	pw.println("<label><input type='checkbox' value='Write' checked='checked' onclick='return false;'name='"+parts[0]+"WriteControl"+"'/>Write</label>");
	        }
	     }
	     pw.println("<td></tr>");
	   }
	   pw.println("</table>");
	   pw.println("<hr>");
	   pw.println("<h4>Enter details to modify ACL</h4><i>Check the box if you want to grant that permission, uncheck to deny.<br>User need not to select other checkbox if FullControl is being selected. </i><br><br>");
	   pw.println("<form action='ownerInfo' method='POST'>");
	   pw.println("<input type='hidden' name='path' value='");
	   pw.println(pathDir);
	   pw.println("'>");
	   pw.println("<label for='user'>Select a user:</label><select name='user'>");
	   for(int j=0;j<len;j++) {
		   pw.println("<option value='");
		   pw.println(arr[j]);
		   pw.println("'>");
		   pw.println(arr[j]);
		   pw.println("</option>");
	   }
	   pw.println("</select>&nbsp;");
	   pw.println("<label><input type='checkbox' name='modifyFullProperty' value='FullControl' checked='checked'/>Full Control</label>"
   	 		+ "<label><input type='checkbox' name='modifyReadProperty' value='Read'/>Read</label>"
   	 		+ "<label><input type='checkbox' name='modifyWriteProperty' value='Write'/>Write</label><br><br>");
	   pw.println("<input type='submit' value='Modify ACL'></form>");
	   pw.println("</center></body></html>");
	   pw.close();		//closing the stream  
	}
	
	public void doPost(HttpServletRequest request, HttpServletResponse response) throws ServletException, IOException {
		String pathDir=URLEncoder.encode(request.getParameter("path"), "UTF-8");
		String username=URLEncoder.encode(request.getParameter("user"), "UTF-8");
		Details obj =new Details();
		String permission="";
		//pw.println(username+pathDir+request.getParameter("modifyFullProperty")+request.getParameter("modifyReadProperty")+request.getParameter("modifyWriteProperty"));
		//obj.modifyACL("C:/Users/pradeep-pt3689/Desktop/Details.h", "SYSTEM", 'r', 2);
		if(request.getParameter("modifyFullProperty")!=null) {
			permission+="Full Control is allowed, ";
			obj.modifyACL(pathDir, username, 'a', 1);
		}
		else {
			permission+="Full Control is denied, ";
			obj.modifyACL(pathDir, username, 'a', 2);
		}			
		if(request.getParameter("modifyReadProperty")!=null) {
			permission+="Read is allowed, ";
			obj.modifyACL(pathDir, username, 'r', 1);
		}
			
		else {
			permission+="Read is denied, ";
			obj.modifyACL(pathDir, username, 'r', 2);
		}
			
		if(request.getParameter("modifyWriteProperty")!=null) {
			permission+="Write is allowed, ";
			obj.modifyACL(pathDir, username, 'w', 1);
		}
			
		else {
			permission+="Write is denied, ";
			obj.modifyACL(pathDir, username, 'w', 2);
		}
		
		Timestamp timestamp = new Timestamp(System.currentTimeMillis());
		//Pushing modifying details to database
		/*
		Class.forName("com.mysql.jdbc.Driver");
        Connection conn = DriverManager.getConnection("jdbc:mysql://sql12.freemysqlhosting.net:3306/sql12358721","sql12358721","7t8epd1Iej");
        Statement st=conn.createStatement();
        st.executeUpdate("insert into demo(Username,Path,Permission,'Permission Type',Time)values('" + username + "','" + pathDir + "','" + permission + "','" + null + "','" + timestamp + "')");
        */
		response.sendRedirect("retrieve.jsp");
	}
}