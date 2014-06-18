package com.example;

import java.io.IOException;

import javax.servlet.ServletException;
import javax.servlet.ServletOutputStream;
import javax.servlet.annotation.WebServlet;
import javax.servlet.http.HttpServlet;
import javax.servlet.http.HttpServletRequest;
import javax.servlet.http.HttpServletResponse;
import javax.xml.bind.DatatypeConverter;

/**
 * Servlet implementation class BasicAuth
 */
@WebServlet("/BasicAuth")
public class BasicAuth extends HttpServlet {
	private static final long serialVersionUID = 1L;
       
    /**
     * @see HttpServlet#HttpServlet()
     */
    public BasicAuth() {
        super();
        // TODO Auto-generated constructor stub
    }

	/**
	 * @see HttpServlet#doGet(HttpServletRequest request, HttpServletResponse response)
	 */
	protected void doGet(HttpServletRequest request, HttpServletResponse response) throws ServletException, IOException {
	   System.err.println("======================================\ndoGet()");
	   
	   String auth = request.getHeader("authorization");
	   if (auth != null) {
         // authenticated

	      // extract username and password
         auth = auth.substring("Basic ".length());
	      byte[] data = DatatypeConverter.parseBase64Binary(auth);
	      String userPassData = new String(data, "ASCII");
	      String[] userPass = userPassData.split(":", 2);

	      ServletOutputStream out = response.getOutputStream();

	      out.println("<!DOCTYPE html>\n" +
	                  "<html>\n" +
	                  "<head></head>\n" +
	                  "<body>\n");

	      out.println("<ul>\n" +
	                  "    <li>Username: " + userPass[0] + "\n" +
	                  "    <li>Password: " + userPass[1] + "\n" +
	                  "</ul>\n" + 
	                  "<a href=\"BasicAuthLogout\">Log out</a>\n"+
                     "</body></html>");

	      return;
	   }

      // not authenticated
      response.setHeader("WWW-Authenticate", "Basic realm=\"authRealm2\"");
      response.setStatus(HttpServletResponse.SC_UNAUTHORIZED);

      System.err.println("======================================");
	}

	/**
	 * @see HttpServlet#doPost(HttpServletRequest request, HttpServletResponse response)
	 */
	protected void doPost(HttpServletRequest request, HttpServletResponse response) throws ServletException, IOException {
		// TODO Auto-generated method stub
	}

}
