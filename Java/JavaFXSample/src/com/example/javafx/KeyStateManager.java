package com.example.javafx;

import java.util.Stack;

import javafx.beans.property.ObjectProperty;
import javafx.beans.property.SimpleObjectProperty;
import javafx.event.EventHandler;
import javafx.scene.Scene;
import javafx.scene.input.KeyCode;

public class KeyStateManager {

    private Stack<KeyCode> keys = new Stack<>();

    /** Event handler handling for digit changes */
    private ObjectProperty<EventHandler<KeyStateEvent>> keyChangedProperty = new SimpleObjectProperty<>();
    public void setOnKeyChangeEvent(EventHandler<KeyStateEvent> handler) { onKeyChangedProperty().set(handler); }
    public final EventHandler<KeyStateEvent> getOnKeyChanged() { return onKeyChangedProperty().get(); }
    public final ObjectProperty<EventHandler<KeyStateEvent>> onKeyChangedProperty() { return keyChangedProperty; }

    public KeyStateManager(Scene scn_main) {
        scn_main.setOnKeyPressed(e-> {

            KeyCode activeKey = e.getCode();
            if (!keys.contains(activeKey)) {
                getOnKeyChanged().handle(new KeyStateEvent(activeKey));
                keys.push(activeKey);
            }
        });

        scn_main.setOnKeyReleased(e -> {
            KeyCode activeKey = e.getCode();
            keys.remove(activeKey);

            if (keys.empty()) {
                getOnKeyChanged().handle(new KeyStateEvent(null));
            }
            else {
                activeKey = keys.peek();
                getOnKeyChanged().handle(new KeyStateEvent(activeKey));
            }

        });

    }

}
