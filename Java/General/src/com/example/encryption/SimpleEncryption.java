package com.example.encryption;

public class SimpleEncryption {

	public static void main(String[] args) {
		new SimpleEncryption().encrypt("GUT GEMACHT DU HAST DEINEN ERSTEN SATZ ENTSCHLUESSELT");
	}

	private void encrypt(String input) {
		input.chars().forEach(c -> {
			if (c == 32) {
				System.err.print("_  ");
			} else {
				int code = c - 'A' + 1;
				if (code < 1 || code > 26) {
					throw new RuntimeException("Invalid character: " + (char) c + "/" + c);
				}
				System.err.print(code + "  ");
			}
		} );
	}
}
