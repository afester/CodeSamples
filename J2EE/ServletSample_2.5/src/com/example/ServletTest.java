package com.example;


import java.io.IOException;
import java.io.PrintStream;

import javax.servlet.ServletException;
import javax.servlet.http.HttpServlet;
import javax.servlet.http.HttpServletRequest;
import javax.servlet.http.HttpServletResponse;

/**
 * Sample servlet implementation class, NOT using annotations
 */
public class ServletTest extends HttpServlet {
	private static final long serialVersionUID = 1L;
       
    /**
     * @see HttpServlet#HttpServlet()
     */
    public ServletTest() {
        super();
    }

	/**
	 * @see HttpServlet#doGet(HttpServletRequest request, HttpServletResponse response)
	 */
	protected void doGet(HttpServletRequest request, HttpServletResponse response) throws ServletException, IOException {
	   System.err.println("  # SampleServlet::doGet()");

      PrintStream out = new PrintStream(response.getOutputStream());

      out.println("<!DOCTYPE html>\n<html><head></head><body>");

      out.println("<h2>GET Request</h2>"); 
      out.println("Server info: "  + getServletConfig().getServletContext().getServerInfo() + "<br/>");
      out.println("Servlet version: "  + getServletConfig().getServletContext().getEffectiveMajorVersion() +
                  "." + getServletConfig().getServletContext().getEffectiveMinorVersion());

      out.println("</body>");
	}

	/**
	 * @see HttpServlet#doPost(HttpServletRequest request, HttpServletResponse response)
	 */
	protected void doPost(HttpServletRequest request, HttpServletResponse response) throws ServletException, IOException {
      System.err.println("  # SampleServlet::doPost()");

      PrintStream out = new PrintStream(response.getOutputStream());
      out.println("Hello POST " + getServletConfig().getServletContext().getServerInfo());
	}

}
