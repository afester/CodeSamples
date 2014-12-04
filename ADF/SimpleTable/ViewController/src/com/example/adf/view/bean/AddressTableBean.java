package com.example.adf.view.bean;

import javax.faces.application.NavigationHandler;
import javax.faces.context.FacesContext;
import javax.faces.event.AbortProcessingException;
import javax.faces.event.ValueChangeEvent;

public class AddressTableBean {
    public AddressTableBean() {
    }

    public void tableFormatChanged(ValueChangeEvent valueChangeEvent) {
        String dest = valueChangeEvent.getNewValue().toString();
        System.err.println("View format changed: " + dest);

        FacesContext context = FacesContext.getCurrentInstance();
        NavigationHandler handler = context.getApplication().getNavigationHandler();
        handler.handleNavigation(context, null, dest);
        context.renderResponse();

        // The following line might or not be needed depending where the code is called and the context
        throw new AbortProcessingException(); // Prevents further listeners from being notified*/
    }
}
