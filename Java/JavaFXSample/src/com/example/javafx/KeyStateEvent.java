package com.example.javafx;

import javafx.event.Event;
import javafx.event.EventType;
import javafx.scene.input.KeyCode;

public class KeyStateEvent extends Event {

    private final static EventType<KeyStateEvent> EVENT_TYPE = new EventType<>("KEY_CHANGE_EVENT");
    private KeyCode key;

    public KeyStateEvent(KeyCode activeKey) {
        super(null, null, EVENT_TYPE);
        this.key = activeKey;
    }
    
    
    public KeyCode getKeyCode() {
        return key;
    }

    @Override
    public String toString() {
        return "KeyChangeEvent[" + key + "]";
    }
}
