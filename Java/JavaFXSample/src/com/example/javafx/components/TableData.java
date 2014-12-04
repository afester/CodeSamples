package com.example.javafx.components;

import javafx.beans.property.SimpleStringProperty;
import javafx.beans.property.StringProperty;

/* Note: Need to be in a public class - otherwise some strange exception is thrown, like
 * "loadSkinClass Failed to load skin 'StringProperty [bean: TableView ..."
 */
public class TableData {

   private StringProperty name =  new SimpleStringProperty(this, "name");
   private StringProperty value = new SimpleStringProperty(this, "value");

   public TableData(String name, String value) {
      this.name.set(name);
      this.value.set(value);
   }

   public String getName() {
      return name.get();
   }
   public void setName(String name) {
      this.name.set(name);
   }

   public String getValue() {
      return value.get();
   }
   public void setValue(String value) {
      this.value.set(value);
   }
   
   
   
   public StringProperty nameProperty() { 
      return name; 
  }
   public StringProperty valueProperty() { 
      return value; 
  }
}
