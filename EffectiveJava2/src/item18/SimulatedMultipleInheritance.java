package item18;

public class SimulatedMultipleInheritance // extends SuperClass1, SuperClass 2{
                                          implements SuperClass1IF, SuperClass2IF {
   private SuperClass1IF s1 = new SuperClass1();
   private SuperClass2IF s2 = new SuperClass2();

   public void method1() {   s1.method1();   }
   public void method2() {   s1.method2();   }
   public void method3() {   s2.method3();   }
   public void method4() {   s2.method4();   }
}
