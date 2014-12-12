package com.example.adf.view;

import javax.faces.component.UIComponent;
import javax.faces.context.FacesContext;
import javax.faces.event.ActionEvent;

import javax.faces.event.ValueChangeEvent;

import oracle.adf.view.rich.component.rich.nav.RichButton;

public class ButtonBean {
    public ButtonBean() {
    }

    public String buttonPressed() {
        System.err.println("BUTTON ACTION");
        return null; // "SimpleTable.jspx";
    }

    public void buttonAction(ActionEvent actionEvent) {
        System.err.println("BUTTON ACTION LISTENER");
        
        System.err.println("  " + actionEvent.getSource().getClass().getName());
        System.err.println("  " + actionEvent.getComponent().getClass().getName());
        
        oracle.adf.view.rich.component.rich.nav.RichButton but = (RichButton) actionEvent.getSource();
    }

    public void inputValidator(FacesContext facesContext, UIComponent uIComponent, Object object) {
        System.err.println("INPUT VALIDATOR: " + uIComponent.getClientId());
    }

    public void inputChangeListener(ValueChangeEvent valueChangeEvent) {
        System.err.println("INPUT CHANGE LISTENER");
    }

    public void inputValidator2(FacesContext facesContext, UIComponent uIComponent, Object object) {
        System.err.println("INPUT VALIDATOR: " + uIComponent.getClientId());
    }
}
