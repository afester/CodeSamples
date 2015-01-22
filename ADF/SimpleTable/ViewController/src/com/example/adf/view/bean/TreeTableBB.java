package com.example.adf.view.bean;

import java.util.Collection;
import java.util.List;

import oracle.adf.share.ADFContext;
import oracle.adf.view.rich.component.rich.data.RichTreeTable;

import oracle.adf.view.rich.context.AdfFacesContext;

import oracle.jbo.uicli.binding.JUCtrlHierBinding;

import oracle.jbo.uicli.binding.JUCtrlHierNodeBinding;

import org.apache.myfaces.trinidad.model.CollectionModel;
import org.apache.myfaces.trinidad.model.RowKeySet;
import org.apache.myfaces.trinidad.model.RowKeySetImpl;

public class TreeTableBB {
    private RichTreeTable taskTreeTable;

    public TreeTableBB() {
        System.err.println("Create " + getClass().getName() + "/" + this);
    }

    public void setTaskTreeTable(RichTreeTable taskTreeTable) {
        this.taskTreeTable = taskTreeTable;
    }

    public RichTreeTable getTaskTreeTable() {
        return taskTreeTable;
    }
}
