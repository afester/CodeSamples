package com.example.adf.view;

import java.io.Serializable;

import javax.faces.application.NavigationHandler;
import javax.faces.context.FacesContext;
import javax.faces.event.AbortProcessingException;
import javax.faces.event.ValueChangeEvent;

public class DataUpdater implements Serializable  {
    private static final long serialVersionUID = 722451214484353508L;

    public DataUpdater() {
    }

    public void updateData(ValueChangeEvent valueChangeEvent) {
        String dest = valueChangeEvent.getNewValue().toString();
        System.err.println("Restarting task flow ..."); // Updating data with " + dest);

        FacesContext context = FacesContext.getCurrentInstance();
        NavigationHandler handler = context.getApplication().getNavigationHandler();
        handler.handleNavigation(context, null, "default");
        context.renderResponse();

        // The following line might or not be needed depending where the code is called and the context
        throw new AbortProcessingException(); // Prevents further listeners from being notified*/

        // Add event code here...
    }
}
