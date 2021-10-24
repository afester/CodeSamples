package com.example;

import java.util.ArrayList;
import java.util.List;

import org.reactfx.util.Either;
import org.reactfx.value.Val;
import org.reactfx.value.Var;

import javafx.beans.property.DoubleProperty;
import javafx.beans.property.SimpleDoubleProperty;

public class ReactFXSample {
    public static void main(String[] args) {
        new ReactFXSample().run();
    }


    public void normalerweise() {
        List<Object> objects = new ArrayList<>();
        objects.add("Hello");
        objects.add(42);
        objects.add("World");
        objects.add("Moon");
        objects.add(99);
        objects.add(44.5);    // NO compile time error! Stores a Double!!!
        objects.stream().forEach(e -> System.err.printf("%s%n", e));    // no (direct) type information!
    }


    public void reactFXApproach() {
        List<Either<String, Integer>> objects = new ArrayList<>();
        objects.add(Either.left("Hello"));
        objects.add(Either.right(42));
        objects.add(Either.left("World"));
        objects.add(Either.left("Moon"));
        objects.add(Either.right(99));
        // objects.add(Either.right(44.5));    // compile time error!

        // unify() returns T as a unified type (Boolean, in the example below)
        System.err.printf("unify()%n"); 
        objects.stream().forEach(e -> System.err.printf("  String? %s%n", e.unify(l -> true, 
                                                                                  r -> false)));

        // map() returns Either<L1, L2> where L1 and L2 are two new types to which the original types are mapped
        System.err.printf("map()%n");
        objects.stream().forEach(e -> System.err.printf("  %s%n", e.map(l -> l,                     // l is of Type String and remains a String
                                                                        r -> Double.valueOf(r))));  // r is of type Integer and is converted to Double

        // The previous example can be simplified by using mapRight(), since
        // the left element remains the same type
        System.err.printf("mapRight()%n");
        objects.stream().forEach(e -> System.err.printf("  %s%n", e.mapRight(r -> Double.valueOf(r))));  // r is of type Integer and is converted to Double

        // exec() just executes one of two Functions, depending on the type of the Either
        System.err.printf("exec()%n");
        objects.stream().forEach(e -> e.exec(l -> System.err.printf("  String: %s%n", l),
                                             r -> System.err.printf("  Integer: %s%n",  r)));
    }

    public void run() {
      //  normalerweise();
      //  reactFXApproach();
        valExample();
        varExample();
        valExample2();
    }


    // Val - a better ObservableValue
    private void valExample() {

        DoubleProperty widthProp = new SimpleDoubleProperty(10.0);
        DoubleProperty heightProp = new SimpleDoubleProperty(5.0);

        // Note: DoubleProperty implements Property<Number>, not Property<Double>
        Val<Number> width = Val.wrap(widthProp);
        Val<Number> height = Val.wrap(heightProp);
        Val<Number> area = Val.combine(width,                                        // if either width 
                                       height,                                       // or height changes
                                       (w, h) -> w.doubleValue() * h.doubleValue()); // then execute this function
                                                                                     // and set the combined value to the result

        System.err.println(area.getValue());
        widthProp.set(6.0);
        System.err.println(area.getValue());
        
/*****************************************/
    }

    private void valExample2() {
        DoubleProperty widthProp = new SimpleDoubleProperty(10.0);
        DoubleProperty heightProp = new SimpleDoubleProperty(5.0);
        
        Val<Number> width = Val.wrap(widthProp);
        Val<Number> height = Val.wrap(heightProp);
        Val<String> dim = Val.combine(width,                                    // if either width 
                                      height,                                   // or height changes
                                      (w, h) -> String.format("%sx%s",  w, h)); // then execute this function

        System.err.println(dim.getValue()); // 10.0x5.0
        widthProp.set(6.0);
        System.err.println(dim.getValue()); // 6.0x5.0
    }

    // Var - a Val which can be modified
    private void varExample() {
        DoubleProperty widthProp = new SimpleDoubleProperty(10.0);
        DoubleProperty heightProp = new SimpleDoubleProperty(5.0);

        Var<Double> width = Val.constant(10.0).asVar(v -> System.err.println("F:" + v));//  Var.doubleVar(widthProp);
        Var<Double> height = Var.doubleVar(heightProp);

        Val<Double> area = Val.combine(width,  height, (w, h) -> w * h );
        System.err.println(area.getValue());

        width.setValue(3.0);

        System.err.println(area.getValue());
        
/*********************************************************************/        
                                                                   //           +---------------+
        System.err.println("\nAsVar:\n------------------------");  //           v               |
        Var<Double> var1 = Val.constant(10.0).asVar(v -> System.err.println("F:" + v));//       |
        System.err.println(var1.getValue());                                           //       |
        var1.setValue(3.0); // this does NOT change the value, but instead calls this lambda ---+
        System.err.println(var1.getValue());    // still 10.0
    }
}
