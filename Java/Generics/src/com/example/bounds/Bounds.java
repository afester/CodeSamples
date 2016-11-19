package com.example.bounds;

import java.util.List;

public class Bounds {

    public static void main(String[] args) {
        new Bounds().run();
    }
    
    
    private void run() {
        Container<Data> container = new Container<>(new IntegerData());
        // Container<String> container2;

        container.performOperation();

        List<IntegerData> someList = null;

        // public void setData(T data, List<T> otherData):
        // => The method setData(Data, List<Data>) 
        //    in the type Container<Data> is not applicable for the arguments (IntegerData, List<IntegerData>)

        // public void setData(T data, List<? extends T> otherData):
        // => Works.

        container.setData(new IntegerData(), someList);
    }
}
