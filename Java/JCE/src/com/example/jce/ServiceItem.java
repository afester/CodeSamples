package com.example.jce;

import java.security.Provider.Service;

public class ServiceItem {
	private Service theService;

	public ServiceItem(Service s) {
		theService = s;
	}

	public Service getService() {
		return theService;
	}

	public String toString() {
		return theService.getAlgorithm() + "(" + theService.getProvider().getName() + " v" + 
											theService.getProvider().getVersion() + ")";
	}
}
