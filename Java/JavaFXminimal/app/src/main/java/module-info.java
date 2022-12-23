module javafxminimal {
    requires javafx.controls;
    requires javafx.fxml;
	requires transitive javafx.graphics;

    opens de.labcorner.javafxminimal to javafx.graphics;
    exports de.labcorner.javafxminimal;
}
