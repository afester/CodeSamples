package com.example.encryption;

public class CaesarEncryption {

	public static void main(String[] args) {
		CaesarEncryption ce = new CaesarEncryption();

		final String input =  "ABCDEFHGIJKLMNOPQRSTUVWXYZ";

		String encrypted = ce.encrypt(5, input);
		System.err.println(encrypted);

		String decrypted = ce.decrypt(5, encrypted);
		System.err.println(decrypted);
	}

	private String encrypt(int key, String input) {
		StringBuffer result = new StringBuffer();
		input.chars().forEach(c -> {
			if (c == 32 || c == '!' || c == '.') {
				result.append((char) c);
			} else {
				// get character code point and check for valid range
				int code = c - 'A';
				if (code < 0 || code > 25) {
					throw new RuntimeException("Invalid character: " + (char) c + "/" + c);
				}

				// "encrypt" the character
				code = (code + key) % 26;
				result.append((char) (code + 'A'));
			}

		});
		
		return result.toString();
	}
	
	
	private String decrypt(int key, String input) {
		StringBuffer result = new StringBuffer();
		input.chars().forEach(c -> {
			if (c == 32 || c == '!' || c == '.') {
				result.append((char) c);
			} else {
				// get character code point and check for valid range
				int code = c - 'A' + 1;
				if (code < 1 || code > 26) {
					throw new RuntimeException("Invalid character: " + (char) c + "/" + c);
				}

				// "decrypt" the character
				code = code - key;
				if (code < 1) {
					code = code + 26;
				}

				result.append((char) (code + 'A' - 1));
			}

		});
			
		return result.toString();
	}
}
