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

public class TreeTableViewBB {

    private RowKeySet disclosedRowKeys = null;

    public TreeTableViewBB() {
        System.err.println("Create " + getClass().getName() + "/" + this);
    }


    public void setDisclosedRowKeys(RowKeySet keys) {
        System.err.println("Setting disclosed row keys: " + keys);
        this.disclosedRowKeys = keys;
    }


    public RowKeySet getDisclosedRowKeys() {
        System.err.println("Getting disclosed row keys: " + disclosedRowKeys);
        if (disclosedRowKeys == null) {
            System.err.println("   Initializing disclosed row keys ...");

            // main issue: how to get the tree component???
            
            // from request scope bean (Note: not officially supported!)
            //TreeTableBB tbb = (TreeTableBB) ADFContext.getCurrent().getRequestScope().get("TreeTableBB");
            TreeTableBB tbb = (TreeTableBB) ADFContext.getCurrent().getScope("backingBeanScope").get("TreeTableBB");
            System.err.println("   TreeTableBB: " + tbb);

            RichTreeTable rtt = tbb.getTaskTreeTable();
            System.err.println("   RichTreeTable: " + rtt);

            CollectionModel model = (CollectionModel) rtt.getValue();
            System.err.println("    Model: " + model);
            JUCtrlHierBinding treeBinding = (JUCtrlHierBinding) model.getWrappedData();

            JUCtrlHierNodeBinding rootNode = treeBinding.getRootNodeBinding();
            System.err.println("    Root node:" + rootNode + System.identityHashCode(rootNode));

            disclosedRowKeys = new RowKeySetImpl();
            List<JUCtrlHierNodeBinding> l = rootNode.getChildren();
            for (JUCtrlHierNodeBinding node : l) {
                System.err.println("      Top level:" + node + "/" + node.getKeyPath());
                disclosedRowKeys.add(node.getKeyPath());
            }

        }

        return disclosedRowKeys;
    }

}
