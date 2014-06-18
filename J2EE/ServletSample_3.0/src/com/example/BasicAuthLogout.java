package com.example;

import java.io.IOException;

import javax.servlet.ServletException;
import javax.servlet.ServletOutputStream;
import javax.servlet.annotation.WebServlet;
import javax.servlet.http.HttpServlet;
import javax.servlet.http.HttpServletRequest;
import javax.servlet.http.HttpServletResponse;

/**
 * Servlet implementation class BasicAuthLogout
 */
@WebServlet("/BasicAuthLogout")
public class BasicAuthLogout extends HttpServlet {
	private static final long serialVersionUID = 1L;
       
    /**
     * @see HttpServlet#HttpServlet()
     */
    public BasicAuthLogout() {
        super();
    }

	/**
	 * @see HttpServlet#doGet(HttpServletRequest request, HttpServletResponse response)
	 */
	protected void doGet(HttpServletRequest request, HttpServletResponse response) throws ServletException, IOException {
      ServletOutputStream out = response.getOutputStream();

      out.println("<!DOCTYPE html>\n" +
                  "<html>\n" +
                  "<head></head>\n" +
                  "<body>\n"+
                  "   <p>You have been logged out.</p>\n"+
                  "<a href=\"BasicAuth\">Log In</a>\n"+
                  "</body></html>");

      // Sending status 401 removes the remembered authorization 
      response.setStatus(HttpServletResponse.SC_UNAUTHORIZED);
	}


	/**
	 * @see HttpServlet#doPost(HttpServletRequest request, HttpServletResponse response)
	 */
	protected void doPost(HttpServletRequest request, HttpServletResponse response) throws ServletException, IOException {
	}

}
