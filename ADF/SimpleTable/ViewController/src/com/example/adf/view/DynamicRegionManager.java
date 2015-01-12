package com.example.adf.view;

import java.io.Serializable;

import oracle.adf.controller.TaskFlowId;

public class DynamicRegionManager implements Serializable {
    private String taskFlowId = "/WEB-INF/flows/AddressTableTF.xml#AddressTableTF";

    public DynamicRegionManager() {
    }

    public TaskFlowId getDynamicTaskFlowId() {
        return TaskFlowId.parse(taskFlowId);
    }

    public void setDynamicTaskFlowId(String taskFlowId) {
        this.taskFlowId = taskFlowId;
    }

    public String inputFocusTF() {
        setDynamicTaskFlowId("/WEB-INF/flows/InputFocusTF.xml#InputFocusTF");
        return null;
    }

    public String addressTableTF() {
        setDynamicTaskFlowId("/WEB-INF/flows/AddressTableTF.xml#AddressTableTF");
        return null;
    }

    public String dynamicDataTF() {
        setDynamicTaskFlowId("/WEB-INF/flows/DynamicDataTF.xml#DynamicDataTF");
        return null;
    }

    public String customModelTF() {
        setDynamicTaskFlowId("/WEB-INF/flows/CustomModelTF.xml#CustomModelTF");
        return null;
    }

    public String dynamicBeanSampleTF() {
        setDynamicTaskFlowId("/WEB-INF/flows/DynamicBeanSampleTF.xml#DynamicBeanSampleTF");
        return null;
    }
}
