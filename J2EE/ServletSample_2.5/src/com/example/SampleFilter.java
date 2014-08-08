package com.example;

import java.io.IOException;
import javax.servlet.Filter;
import javax.servlet.FilterChain;
import javax.servlet.FilterConfig;
import javax.servlet.ServletException;
import javax.servlet.ServletRequest;
import javax.servlet.ServletResponse;

/**
 * Servlet Filter implementation class SampleFilter
 */
public class SampleFilter implements Filter {

    /**
     * Default constructor. 
     */
    public SampleFilter() {
    }

	/**
	 * @see Filter#destroy()
	 */
	public void destroy() {
	}

	/**
	 * @see Filter#doFilter(ServletRequest, ServletResponse, FilterChain)
	 */
	public void doFilter(ServletRequest request, ServletResponse response, FilterChain chain) throws IOException, ServletException {
	   System.err.println("  # SampleFilter::doFilter(): BEFORE");

		// pass the request along the filter chain - the last element in the
	   // chain is the servlet itself!
		chain.doFilter(request, response);

		System.err.println("  # SampleFilter::doFilter(): AFTER");
	}

	/**
	 * @see Filter#init(FilterConfig)
	 */
	public void init(FilterConfig fConfig) throws ServletException {
      System.err.println("  # SampleFilter::init()");
	}

}
