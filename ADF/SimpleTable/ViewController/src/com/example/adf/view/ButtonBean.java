package com.example.adf.view;

import java.util.ArrayList;
import java.util.Collections;
import java.util.Enumeration;
import java.util.Iterator;
import java.util.List;
import java.util.Map;
import java.util.Map.Entry;

import java.util.Set;

import javax.faces.component.UIComponent;
import javax.faces.context.FacesContext;
import javax.faces.event.ActionEvent;

import javax.faces.event.ValueChangeEvent;

import javax.servlet.http.HttpServletRequest;

import javax.servlet.http.HttpSession;

import oracle.adf.model.BindingContext;
import oracle.adf.model.binding.DCBindingContainer;
import oracle.adf.model.binding.DCDataControl;
import oracle.adf.share.ADFContext;
import oracle.adf.view.rich.component.rich.nav.RichButton;

import oracle.binding.BindingContainer;

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

    public String dumpHTTPRequest() {
        ADFContext ctx = ADFContext.getCurrent();

        // get the http servlet request for the current ADF context
        HttpServletRequest req = (HttpServletRequest) ctx.getEnvironment().getRequest(); 

        // dump the attributes of the http servlet request
        System.err.println("HTTP Request attributes:");
        Enumeration names = req.getAttributeNames();
        List<String> contents = new ArrayList();
        while(names.hasMoreElements()) {
            Object name = names.nextElement();
            String line = String.format("%s=%s", name, req.getAttribute(name.toString()));
            contents.add(line);
        }
        Collections.sort(contents);
        for (String line : contents) {
            System.err.println("   " + line);
        }

        // dump the session attributes of the http servlet request
        System.err.println("HTTP Session attributes:");
        HttpSession hs = req.getSession();

        names = hs.getAttributeNames();
        contents = new ArrayList();
        while(names.hasMoreElements()) {
            Object name = names.nextElement();
            String line = String.format("%s=%s", name, hs.getAttribute(name.toString()));
            contents.add(line);
        }
        Collections.sort(contents);
        for (String line : contents) {
            System.err.println("   " + line);
        }

        return null;
    }

    public String dumpBindings() {
        dumpHTTPRequest();

        ADFContext ctx = ADFContext.getCurrent();

/**************************************/
/* Dump the object maps associated with the ADF context */

/*
        // dump the scope object maps for the current ADF context
        Set<String> scopeNames = ctx.getScopeNames();
        for (String sn : scopeNames) {
            Map scope = ctx.getScope(sn);
            System.err.println("Scope: " + sn);
            dumpMapSorted(scope);
        }

*/
/***************************************************/

        Object bindingContext = ctx.getSessionScope().get("data");
        System.err.println("Binding context: " + bindingContext);
        
        BindingContext bc = BindingContext.getCurrent();
        System.err.println("Binding context current: " + bc);

        for (BindingContainer bcc : bc.bindingContainers()) {
            System.err.println("BINDING CNT:" + bcc + "/" + bcc.getClass().getName() + "@" + System.identityHashCode(bcc));
            for (Object o : bcc.entrySet()) {
                Map.Entry e = (Map.Entry) o;
                System.err.println("   =>" + e.getKey() + "=" + e.getValue() + "/" + e.getValue().getClass());
            }
        }

        Object o = ctx.getRequestScope().get("bindings");
        System.err.println("bindings:" + o + "/" + o.getClass().getName() + "@" + System.identityHashCode(o));
        
        DCBindingContainer container = (DCBindingContainer) bc.getCurrentBindingsEntry();
        System.err.println("getCurrentBindingsEntry:" + container + "/" + container.getClass().getName() + "@" + System.identityHashCode(container));

        dumpBindingContainer(container);

/*

        System.err.println("Dumping binding context:");
        BindingContext bc = BindingContext.getCurrent();
        for (Object o : bc.entrySet()) {
            Map.Entry e = (Map.Entry) o;
            System.err.println("BINDING cTX:" + e.getKey() + "=" + e.getValue() + "/" + e.getValue().getClass());
        }

        Iterator  i = bc.dataControlsIterator();
        while(i.hasNext()) {
            Object o = i.next();
            System.err.println("   DC:" + o + "/" + o.getClass().getName());
        }
        
        for (BindingContainer bcc : bc.bindingContainers()) {
            System.err.println("BINDING CNT:" + bcc + "/" + bcc.getClass().getName());
            for (Object o : bcc.entrySet()) {
                Map.Entry e = (Map.Entry) o;
                System.err.println("   =>" + e.getKey() + "=" + e.getValue() + "/" + e.getValue().getClass());
            }
        }

        BindingContainer bindings = BindingContext.getCurrent().getCurrentBindingsEntry();
        System.err.println("Dumping binding container:");
        for (Object o : bindings.entrySet()) {
            System.err.println("BINDING:" + o + "/" + o.getClass().getName());
        }
*/
        return null;
    }

    private void dumpBindingContainer(DCBindingContainer container) {
        // What if there is more than one data control involved in this container??
        DCDataControl ctrl = container.getDataControl();
        System.err.println("Data control:");
        System.err.println("  " + ctrl);

        System.err.println("\nControl bindings:");
        List bindingList = container.getControlBindings();
        if (bindingList != null) {
            for (Object binding : bindingList) {
                System.err.println("  " + binding + "/" + binding.getClass().getName());
            }
        }

        System.err.println("  Attribute bindings:");
        bindingList = container.getAttributeBindings();
        if (bindingList != null) {
            for (Object binding : bindingList) {
                System.err.println("    " + binding + "/" + binding.getClass().getName());
            }
        }
        System.err.println("  Operation bindings:");
        bindingList = container.getOperationBindings();
        if (bindingList != null) {
            for (Object binding : bindingList) {
                System.err.println("    " + binding+ "/" + binding.getClass().getName());
            }
        }

        System.err.println("\nExecutable bindings:");
        bindingList = container.getExecutableBindings();
        if (bindingList != null) {
            for (Object binding : bindingList) {
                System.err.println("  " +binding + "/" + binding.getClass().getName());
            }
        }
        System.err.println("  Iterator bindings:");
        Iterator i = container.getIterBindings().entrySet().iterator();
        while(i.hasNext()) {
            Entry binding = (Map.Entry) i.next();
            System.err.println("    " + binding.getKey()+"=" + binding.getValue() + "/" + binding.getValue().getClass().getName());
        }

        System.err.println("  Region bindings:");
        bindingList = container.getRegionBindings();
        if (bindingList != null) {
            for (Object binding : bindingList) {
                System.err.println("    " + binding);
            }
        }
    }

    private void dumpMapSorted(Map theMap) {
        List<String> contents = new ArrayList<String>();
        Set<Entry> entries = theMap.entrySet();
        for (Entry entry : entries) {
            String line = String.format("%s=%s", entry.getKey(), entry.getValue());
            contents.add(line);
        }
        Collections.sort(contents);
        for (String line : contents) {
            System.err.println("   " + line);
        }
    }
}
