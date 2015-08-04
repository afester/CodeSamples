package com.example.adf.view.bean;

import java.util.HashSet;
import java.util.Set;

import javax.faces.application.NavigationHandler;
import javax.faces.context.FacesContext;
import javax.faces.event.AbortProcessingException;
import javax.faces.event.ValueChangeEvent;

import oracle.adf.view.rich.component.rich.input.RichSelectOneChoice;

public class AddressTableBB {
    private Object choiceValue;

    public AddressTableBB() {
    }

    public void setChoiceValue(Object choiceValue) {
        System.err.println("==> Setting choice value ..." + choiceValue);
        this.choiceValue = choiceValue;
    }

    public Object getChoiceValue() {
        System.err.println("==> Getting choice value ... " + choiceValue);
        return choiceValue;
    }
}
