package com.example.jce;

import java.awt.Font;

import javax.swing.JTextArea;

public class JHexTextArea extends JTextArea {

   private static final long serialVersionUID = 1L;

   public JHexTextArea(int width, int height) {
		super(width, height);

		setFont(new Font("Courier", Font.PLAIN, 10));
	}

	public void addData(byte[] data) {
		int base;
		String result = "";

		for (base = 0; base < data.length;  base += 16) {
			for (int idx = base;  idx < base + 16 && idx < data.length;  idx++) {
				result = result + String.format(" %02X", (int) data[idx] & 0xFF);
			}
			result = result + "  ";
			for (int idx = base;  idx < base + 16 && idx < data.length;  idx++) {
				int c = (int) data[idx] & 0xFF;
				if (c == '\n' || c == '\t') {
					c = '.';
				}
				result = result + String.format("%c", c);
			}
			result = result + "\n";
		}
		
		setText(getText() + result + "\n");
		
	}

	public void setData(byte[] data) {
		setText("");
		addData(data);
	}
}
