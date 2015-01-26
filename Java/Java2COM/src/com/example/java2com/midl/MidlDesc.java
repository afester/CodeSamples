package com.example.java2com.midl;

import java.util.ArrayList;
import java.util.HashMap;
import java.util.List;
import java.util.Map;

public class MidlDesc {

    private List<EnumDesc> enums = new ArrayList<>();
    private List<InterfaceDesc> interfaces = new ArrayList<>();
    private Map<String, EnumDesc> enumsById = new HashMap<>();
    private Map<String, InterfaceDesc> interfacesById = new HashMap<>();

    public void addInterface(InterfaceDesc currentInterface) {
        interfaces.add(currentInterface);
        interfacesById.put(currentInterface.getName(), currentInterface);
    }

    public List<InterfaceDesc> getInterfaces() {
        return interfaces;
    }

    
    public InterfaceDesc getInterface(String id) {
        return interfacesById.get(id);
    }


    public void addEnum(EnumDesc enumDesc) {
        enums.add(enumDesc);
        enumsById.put(enumDesc.getName(), enumDesc);
    }

    public List<EnumDesc> getEnums() {
        return enums;
    }
    
    public EnumDesc getEnum(String id) {
        return enumsById.get(id);
    }

}
