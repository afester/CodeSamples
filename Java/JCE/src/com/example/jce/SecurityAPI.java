
package com.example.jce;

import java.awt.BorderLayout;
import java.awt.Font;
import java.awt.GridBagConstraints;
import java.awt.GridBagLayout;
import java.awt.Insets;
import java.awt.event.ActionEvent;
import java.awt.event.ActionListener;
import java.security.KeyPair;
import java.security.KeyPairGenerator;
import java.security.MessageDigest;
import java.security.NoSuchAlgorithmException;
import java.security.Provider;
import java.security.Provider.Service;
import java.security.SecureRandom;
import java.util.Map;
import java.util.Set;
import java.util.TreeMap;
import java.util.Vector;

import javax.crypto.Cipher;
import javax.crypto.NoSuchPaddingException;
import javax.swing.JButton;
import javax.swing.JComboBox;
import javax.swing.JFrame;
import javax.swing.JLabel;
import javax.swing.JPanel;
import javax.swing.JTextArea;


public class SecurityAPI implements ActionListener {

	private Map<String, Vector<Service>> typeMap = new TreeMap<String, Vector<Service>>();
	//private Vector<String> types = new Vector<String>();
	private JComboBox<String> typeList;
	private JComboBox<ServiceItem> algorithmList;
	private JTextArea inputField;
	private JTextArea keyField;
	private JHexTextArea outputField;
	private JTextArea infoField;
	private JButton executeButton;
	private Service selectedService;

	public void getProvidersByType() {

		Provider[] providers = java.security.Security.getProviders();
		for (Provider p : providers) {
			// System.out.println("    " + p.getName());
			Set<Service> services = p.getServices();
			for (Service s : services) {
				// System.out.println("        " + s.getAlgorithm()+ " (" + s.getType() + ")");

				Vector<Service> servicesOfType = typeMap.get(s.getType());
				if (servicesOfType == null) {
					servicesOfType = new Vector<Service>();
					typeMap.put(s.getType(), servicesOfType);
					//types.add(s.getType());
				}
				servicesOfType.add(s);
			}
		}
	}


	public void allProviders() {
		System.out.println("List of all installed security providers:");
		System.out.println("=================================================");
		
		Provider[] providers = java.security.Security.getProviders();
		for (Provider p : providers) {
			System.out.println("    " + p.getName());
			Set<Service> services = p.getServices();
			for (Service s : services) {
				System.out.println("        " + s.getAlgorithm()+ " (" + s.getType() + ")");
			}
		}
	}

	/**
	 * @param args
	 */
	public static void main(String[] args) {
		SecurityAPI api = new SecurityAPI();
		api.getProvidersByType();
		api.run();
	}

	
	public void run() {
		JFrame frame = new JFrame("Security API Demo");
		frame.setDefaultCloseOperation(JFrame.EXIT_ON_CLOSE);
		
		JPanel centerPanel = new JPanel();
		centerPanel.setLayout(new GridBagLayout());

		/* Combo box for provider type */
		centerPanel.add(new JLabel("Engine Class:"), new GridBagConstraints(0, 0, 1, 1, 0, 0, 
												GridBagConstraints.CENTER, GridBagConstraints.BOTH,
												new Insets(5, 5, 0, 5), 0, 0));
		Vector<String> typeNames = new Vector<String>();
		for (String typeName : typeMap.keySet()) {
			typeNames.add(typeName);
		}
		typeList = new JComboBox<>(typeNames);
		centerPanel.add(typeList, new GridBagConstraints(0, 1, 1, 1, 0, 0, 
								  GridBagConstraints.CENTER, GridBagConstraints.BOTH,
								  new Insets(0, 5, 5, 5), 0, 0));
		typeList.addActionListener(this);

		/* Combo box for Algorithm/Provider */
		centerPanel.add(new JLabel("Algorithm:"), new GridBagConstraints(1, 0, 1, 1, 0, 0, 
												GridBagConstraints.CENTER, GridBagConstraints.BOTH,
												new Insets(5, 5, 0, 5), 0, 0));
		algorithmList = new JComboBox<>();
		centerPanel.add(algorithmList, new GridBagConstraints(1, 1, 1, 1, 0, 0, 
												GridBagConstraints.CENTER, GridBagConstraints.BOTH,
												new Insets(0, 5, 5, 5), 0, 0));
		algorithmList.addActionListener(this);

		/* Input data */
		centerPanel.add(new JLabel("Input data:"), new GridBagConstraints(0, 2, 1, 1, 0, 0, 
				GridBagConstraints.CENTER, GridBagConstraints.BOTH,
				new Insets(5, 5, 0, 5), 0, 0));
		inputField = new JTextArea(10,10);
		centerPanel.add(inputField, new GridBagConstraints(0, 3, 2, 1, 0, 0, 
							GridBagConstraints.CENTER, GridBagConstraints.BOTH,
							new Insets(5, 5, 5, 5), 0, 0));

		centerPanel.add(new JLabel("Key:"), new GridBagConstraints(0, 4, 1, 1, 0, 0, 
				GridBagConstraints.CENTER, GridBagConstraints.BOTH,
				new Insets(5, 5, 0, 5), 0, 0));
		keyField = new JTextArea(10,10);
		centerPanel.add(keyField, new GridBagConstraints(0, 5, 2, 1, 0, 0, 
							GridBagConstraints.CENTER, GridBagConstraints.BOTH,
							new Insets(5, 5, 5, 5), 0, 0));

		/* Button to execute some action */
		executeButton = new JButton("Execute");
		centerPanel.add(executeButton, new GridBagConstraints(0, 6, 2, 1, 0, 0, 
							GridBagConstraints.CENTER, GridBagConstraints.BOTH,
							new Insets(5, 5, 5, 5), 0, 0));
		executeButton.addActionListener(this);

		/* result data */
		outputField = new JHexTextArea(10, 10);
		centerPanel.add(outputField, new GridBagConstraints(0, 7, 2, 1, 0, 0, 
							GridBagConstraints.CENTER, GridBagConstraints.BOTH,
							new Insets(5, 5, 5, 5), 0, 0));

		infoField = new JTextArea(5, 80);
		infoField.setEditable(false);
		infoField.setFont(new Font("Courier", Font.PLAIN, 10));
      centerPanel.add(infoField, new GridBagConstraints(0, 8, 2, 1, 0, 0, 
            GridBagConstraints.CENTER, GridBagConstraints.BOTH,
            new Insets(5, 5, 5, 5), 0, 0));
		
		frame.getContentPane().add(centerPanel, BorderLayout.CENTER);
		frame.pack();
		frame.setVisible(true);
	}


	@Override
	public void actionPerformed(ActionEvent arg0) {
		if (arg0.getSource() == typeList) {
			String selectedType = (String) typeList.getSelectedItem();
			Vector<Service> services = typeMap.get(selectedType);
			algorithmList.removeAllItems();
			for (Service s : services) {
				algorithmList.addItem(new ServiceItem(s));
			}
		} else if (arg0.getSource() == algorithmList) {
			ServiceItem selectedItem = (ServiceItem) algorithmList.getSelectedItem();
			if (selectedItem != null) {
				selectedService = selectedItem.getService();
			}
		} else if (arg0.getSource() == executeButton) {
			useService();
		}
	}


	private void useService() {
		if (selectedService != null) {
			String engineClass = (String) typeList.getSelectedItem();
			String algorithm = selectedService.getAlgorithm();
			Provider prov = selectedService.getProvider();

			System.out.println("Engine Class: " + engineClass + 
							   ", Algorithm: "  + algorithm +
							   ", Provider: " + prov);

			try {
				if (engineClass.equals(MessageDigest.class.getSimpleName())) {
					MessageDigest md = MessageDigest.getInstance(algorithm, prov);
					md.update(inputField.getText().getBytes());
					
					byte[] result = md.digest();
               outputField.setData(result);
               
               infoField.setText(javax.xml.bind.DatatypeConverter.printHexBinary(result));
				} else if (engineClass.equals(SecureRandom.class.getSimpleName())) {
					SecureRandom sr = SecureRandom.getInstance(algorithm, prov);
//					sr.setSeed(0x12345);

					byte[] buffer = new byte[10];
					sr.nextBytes(buffer);
					outputField.setData(buffer);
				} else if (engineClass.equals(KeyPairGenerator.class.getSimpleName())) {
					KeyPairGenerator kpg = KeyPairGenerator.getInstance(algorithm, prov);
					KeyPair kp = kpg.generateKeyPair();
					outputField.setText("Private Key:\n---------------------------\n");
					outputField.addData(kp.getPrivate().getEncoded());
					outputField.append("Public Key:\n-----------------------------\n");
					outputField.addData(kp.getPublic().getEncoded());
				} else if (engineClass.equals(Cipher.class.getSimpleName())) {
				    algorithm = "AES/CBC/PKCS5Padding";
					Cipher cipher = Cipher.getInstance(algorithm, prov);
					System.out.println(cipher);
					// Key key; //  = new Key();
					//cipher.init(Cipher.ENCRYPT_MODE, key);
				} else {
					throw new RuntimeException("Not yet implemented: "+ engineClass);
				}
			} catch (NoSuchAlgorithmException e) {
				// TODO Auto-generated catch block
				e.printStackTrace();
			} catch (NoSuchPaddingException e) {
				// TODO Auto-generated catch block
				e.printStackTrace();
			}
		}
	}
}
