package Interface;

import java.awt.EventQueue;

import javax.swing.JFrame;
import javax.swing.JTabbedPane;
import java.awt.Color;

import javax.swing.JPanel;
import java.awt.Image;

import javax.swing.AbstractButton;
import javax.swing.BorderFactory;
import javax.swing.ButtonGroup;
import javax.swing.ImageIcon;
import javax.swing.JButton;
import javax.swing.JLabel;
import java.awt.Font;
import javax.swing.JCheckBox;
import javax.swing.JRadioButton;
import javax.swing.JSlider;
import javax.swing.JTextField;
import javax.swing.border.BevelBorder;
import javax.swing.border.Border;

import MonitorComponents.ImageComponent;
import clientSocket.ClientSocket;

import java.awt.event.ActionListener;
import java.awt.event.FocusAdapter;
import java.awt.event.FocusEvent;
import java.io.IOException;
import java.net.InetAddress;
import java.util.Enumeration;
import java.awt.event.ActionEvent;

public class Interface {

	private JFrame frame;
	private JTextField textFieldIP;
	private JTextField textFieldPort;
	private JTextField textFieldExplorationName;
	private Border myRaisedBorder = BorderFactory.createBevelBorder(BevelBorder.LOWERED, Color.GRAY, Color.GRAY);

	//buttons
	private JButton btnConnect = null;
	private JButton btnDisconnect = null;
	private JButton btnOk = null;
	private JButton btnReset = null;
	private JCheckBox chckbxCamera = null;
	private JCheckBox chckbxTemperature  = null;
	private JCheckBox chckbxHumidity = null;
	private JCheckBox chckbxAirQuality = null;
	private JCheckBox chckbxProximity = null;
	private JCheckBox chckbxFire = null;
	private JCheckBox chckbxLight = null;
	private JCheckBox chckbxGps = null;
	private JCheckBox chckbxLed = null;
	ButtonGroup group = new ButtonGroup();

	
    // Connect status constants
    public final static int NULL = 0;
    public final static int DISCONNECTED = 1;
    public final static int DISCONNECTING = 2;
    public final static int BEGIN_CONNECT = 3;
    public final static int CONNECTED = 4;
    
    // Other constants
    public final static String statusMessages[] = {
       " Error! Could not connect!", " Disconnected",
       " Disconnecting...", " Connecting...", " Connected"
    };
    
    // Connection info
    public static String hostIP = "localhost";
    public static String explorationName = "NONE";
    public static int port = 1234;
    public static int connectionStatus = DISCONNECTED;
    ClientSocket client = null;
    public static String options = "";

	/**
	 * Launch the application.
	 */
	public static void main(String[] args) {
		EventQueue.invokeLater(new Runnable() {
			public void run() {
				try {
					Interface window = new Interface();
					window.frame.setVisible(true);
				} catch (Exception e) {
					e.printStackTrace();
				}
			}
		});
	}

	/**
	 * Create the application.
	 */
	public Interface() {
		initialize();
	}

	/**
	 * Initialize the contents of the frame.
	 */
	private void initialize() {
		
		frame = new JFrame("Raspirover 2018");
		frame.setBounds(200, 200, 640, 500);
		frame.setDefaultCloseOperation(JFrame.EXIT_ON_CLOSE);
		frame.getContentPane().setLayout(null);
		
		JTabbedPane tabbedPane = new JTabbedPane(JTabbedPane.TOP);
		tabbedPane.setBounds(6, 6, 625, 450);
		frame.getContentPane().add(tabbedPane);
		
		JLabel lblNewLabel = new JLabel("");
		lblNewLabel.setBounds(359, 454, 18, 18);
		lblNewLabel.setIcon(new ImageIcon(new ImageIcon(this.getClass().getResource("../redBall.png")).getImage()));
		frame.getContentPane().add(lblNewLabel);
		
		JPanel tab_Connection = new JPanel();
		tabbedPane.addTab("Connection", null, tab_Connection, null);
		tab_Connection.setLayout(null);
	    
		JLabel lblConnectionStatus = new JLabel("Connection Status:");
		lblConnectionStatus.setFont(new Font("Lucida Grande", Font.PLAIN, 15));
		//lblConnectionStatus.setBounds(6, 380, 142, 16);
		//tab_Connection.add(lblConnectionStatus);
		lblConnectionStatus.setBounds(216, 454, 143, 16);
		frame.getContentPane().add(lblConnectionStatus);
				
		JLabel lblSocketOptions = new JLabel("Socket Options");
		lblSocketOptions.setFont(new Font("Lucida Grande", Font.PLAIN, 16));
		lblSocketOptions.setBounds(239, 30, 125, 16);
		tab_Connection.add(lblSocketOptions);
		
		JLabel lblIpAdress = new JLabel("IP Adress");
		lblIpAdress.setBounds(160, 83, 65, 16);
		tab_Connection.add(lblIpAdress);
		
		JLabel lblPort = new JLabel("Port");
		lblPort.setBounds(160, 111, 65, 16);
		tab_Connection.add(lblPort);
		
		JLabel lblExplorationName = new JLabel("Exploration Name");
		lblExplorationName.setBounds(160, 138, 117, 16);
		tab_Connection.add(lblExplorationName);

		// IP Address input
		textFieldIP = new JTextField(12);
		textFieldIP.setEditable(true);
		textFieldIP.setText(hostIP);
		textFieldIP.setHorizontalAlignment(JTextField.CENTER);
		textFieldIP.setEnabled(true);
		textFieldIP.setBounds(334, 78, 177, 26);
		textFieldIP.setColumns(10);
		textFieldIP.addFocusListener(new FocusAdapter() {
            public void focusLost(FocusEvent e) {
            	textFieldIP.selectAll();
               // Should be editable only when disconnected
               if (connectionStatus != DISCONNECTED) {
                  //changeStatusNTS(NULL, true);
               }
               else {
                  hostIP = textFieldIP.getText();
               }
            }
         });
		tab_Connection.add(textFieldIP);
		
		// Port input
		textFieldPort = new JTextField(10);
		textFieldPort.setEditable(true);
		textFieldPort.setHorizontalAlignment(JTextField.CENTER);
		textFieldPort.setBounds(334, 106, 177, 26);
		textFieldPort.setColumns(10);
		textFieldPort.setText((new Integer(port)).toString());
		tab_Connection.add(textFieldPort);
		textFieldPort.addFocusListener(new FocusAdapter() {
	            public void focusLost(FocusEvent e) {
	               // should be editable only when disconnected
	               if (connectionStatus != DISCONNECTED) {
	                  //changeStatusNTS(NULL, true);
	               }
	               else {
	                  try {
	                     port = Integer.parseInt(textFieldPort.getText());
	                  }
	                  catch (NumberFormatException nfe) {
	                	  	textFieldPort.setText((new Integer(port)).toString());
	                	  	tab_Connection.repaint();
	                  }
	               }
	            }
	         });
	    
		// Exploration Name input
		textFieldExplorationName = new JTextField(40);
		textFieldExplorationName.setHorizontalAlignment(JTextField.CENTER);
		textFieldExplorationName.setEditable(true);
		textFieldExplorationName.setBounds(334, 133, 177, 26);
		textFieldExplorationName.setColumns(10);
		textFieldExplorationName.addFocusListener(new FocusAdapter() {
	            public void focusLost(FocusEvent e) {
	               // should be editable only when disconnected
	               if (connectionStatus != DISCONNECTED) {
	                  //changeStatusNTS(NULL, true);
	               }
	               else {
	                  try {
	                     explorationName = textFieldExplorationName.getText();
	                  }
	                  catch (NumberFormatException nfe) {
	                	  	textFieldExplorationName.setText(explorationName);
	                	  	tab_Connection.repaint();
	                  }
	               }
	            }
	         });
		tab_Connection.add(textFieldExplorationName);
			    
				
		//CONNECT BUTTON
		btnConnect = new JButton("Connect");
		btnConnect.setBounds(139, 197, 117, 29);
		btnConnect.addActionListener(new ActionListener() {
			public void actionPerformed(ActionEvent e) {
				try {
					client = new ClientSocket(InetAddress.getByName(hostIP),port);
				} catch (Exception e2) {
					e2.printStackTrace();
				}
		        
		        System.out.println("\r\nConnected to Server: " + client.socket.getInetAddress());
		        connectionStatus = CONNECTED;
				lblNewLabel.setIcon(new ImageIcon(new ImageIcon(this.getClass().getResource("../greenBall.png")).getImage()));
				textFieldIP.setEnabled(false);
				textFieldPort.setEnabled(false);
				textFieldExplorationName.setEnabled(false);
				btnConnect.setEnabled(false);
				btnDisconnect.setEnabled(true);
				chckbxCamera.setEnabled(true);
				chckbxTemperature.setEnabled(true);
				chckbxHumidity.setEnabled(true);
				chckbxAirQuality.setEnabled(true);
				chckbxProximity.setEnabled(true);
				chckbxFire.setEnabled(true);
				chckbxLight.setEnabled(true);
				chckbxGps.setEnabled(true);
				chckbxLed.setEnabled(true);
				
				btnOk.setEnabled(true);
				btnReset.setEnabled(true);
				
	  			for (Enumeration<AbstractButton> buttons = group.getElements(); buttons.hasMoreElements();) {
		            AbstractButton button = buttons.nextElement();
		            button.setEnabled(true);
		        }
				btnReset.doClick();
		        		        
		       // client.start();

			}
		});
		tab_Connection.add(btnConnect);
		
		//DISCONNECT BUTTON
		btnDisconnect = new JButton("Disconnect");
		btnDisconnect.setEnabled(false);
		btnDisconnect.setBounds(334, 197, 117, 29);
		btnDisconnect.addActionListener(new ActionListener() {
			public void actionPerformed(ActionEvent e) {
		        
		        System.out.println("\r\nDisconnected from Server: " + client.socket.getInetAddress());
		        connectionStatus = DISCONNECTED;
				lblNewLabel.setIcon(new ImageIcon(new ImageIcon(this.getClass().getResource("../redBall.png")).getImage()));
				textFieldIP.setEnabled(true);
				textFieldPort.setEnabled(true);
				textFieldExplorationName.setEnabled(true);
				btnDisconnect.setEnabled(false);
				btnConnect.setEnabled(true);
				chckbxCamera.setEnabled(false);
				chckbxTemperature.setEnabled(false);
				chckbxHumidity.setEnabled(false);
				chckbxAirQuality.setEnabled(false);
				chckbxProximity.setEnabled(false);
				chckbxFire.setEnabled(false);
				chckbxLight.setEnabled(false);
				chckbxGps.setEnabled(false);
				chckbxLed.setEnabled(false);
				
				btnOk.setEnabled(false);
				btnReset.setEnabled(false);
				
	  			for (Enumeration<AbstractButton> buttons = group.getElements(); buttons.hasMoreElements();) {
		            AbstractButton button = buttons.nextElement();
		            button.setEnabled(false);
		        }
				
				btnReset.doClick();
				
		        try {
		        		client.Write("EXIT");
			        	client.close();
				} catch (IOException e1) {
					e1.printStackTrace();
				}  
		        
			}
		});
		tab_Connection.add(btnDisconnect);
		
		/* OPTIONS */
		
		JPanel tab_Options = new JPanel();
		tabbedPane.addTab("Options", null, tab_Options, null);
		tab_Options.setLayout(null);
		
		
		//SENSORS
		
		JLabel lblSensors = new JLabel("Sensors");
		lblSensors.setFont(new Font("Lucida Grande", Font.PLAIN, 16));
		lblSensors.setBounds(263, 16, 66, 23);
		tab_Options.add(lblSensors);

		chckbxCamera = new JCheckBox("Camera");
		chckbxCamera.setBounds(246, 130, 128, 23);
		tab_Options.add(chckbxCamera);

		chckbxHumidity = new JCheckBox("Humidity");
		chckbxHumidity.setBounds(246, 55, 128, 23);
		tab_Options.add(chckbxHumidity);
		
		chckbxTemperature = new JCheckBox("Temperature");
		chckbxTemperature.setBounds(86, 55, 128, 23);
		tab_Options.add(chckbxTemperature);
		
		chckbxFire = new JCheckBox("Fire");
		chckbxFire.setBounds(86, 90, 128, 23);
		tab_Options.add(chckbxFire);
		
		chckbxAirQuality = new JCheckBox("Air Quality");
		chckbxAirQuality.setBounds(404, 55, 128, 23);
		tab_Options.add(chckbxAirQuality);
		
		chckbxLight = new JCheckBox("Light");
		chckbxLight.setBounds(86, 125, 128, 23);
		tab_Options.add(chckbxLight);
		
		chckbxGps = new JCheckBox("GPS");
		chckbxGps.setBounds(404, 90, 128, 23);
		tab_Options.add(chckbxGps);
		
		chckbxLed = new JCheckBox("LED");
		chckbxLed.setBounds(404, 130, 128, 23);
		tab_Options.add(chckbxLed);
		
		chckbxProximity = new JCheckBox("Proximity");
		chckbxProximity.setBounds(246, 90, 128, 23);
		tab_Options.add(chckbxProximity);
				
		//DATA TIME CAPTURE
						
		JLabel lblTimeDetection = new JLabel("Data Capture Time");
		lblTimeDetection.setFont(new Font("Lucida Grande", Font.PLAIN, 16));
		lblTimeDetection.setBounds(225, 179, 161, 19);
		tab_Options.add(lblTimeDetection);
		
		JRadioButton rdbtnSecond = new JRadioButton("1 Second");
		rdbtnSecond.setBounds(86, 211, 141, 23);
		tab_Options.add(rdbtnSecond);
		
		JRadioButton rdbtnSeconds = new JRadioButton("5 Seconds");
		rdbtnSeconds.setBounds(86, 246, 141, 23);
		tab_Options.add(rdbtnSeconds);
		
		JRadioButton rdbtnSeconds_1 = new JRadioButton("10 Seconds");
		rdbtnSeconds_1.setBounds(86, 281, 141, 23);
		tab_Options.add(rdbtnSeconds_1);
		
		JRadioButton rdbtnSeconds_2 = new JRadioButton("30 Seconds");
		rdbtnSeconds_2.setBounds(86, 316, 141, 23);
		tab_Options.add(rdbtnSeconds_2);
		
		JRadioButton rdbtnMinute = new JRadioButton("1 Minute");
		rdbtnMinute.setBounds(404, 211, 141, 23);
		tab_Options.add(rdbtnMinute);
		
		JRadioButton rdbtnMinutes = new JRadioButton("5 Minutes");
		rdbtnMinutes.setBounds(404, 246, 141, 23);
		tab_Options.add(rdbtnMinutes);
		
		JRadioButton rdbtnMinutes_1 = new JRadioButton("10 Minutes");
		rdbtnMinutes_1.setBounds(404, 281, 141, 23);
		tab_Options.add(rdbtnMinutes_1);
		
		JRadioButton rdbtnMinutes_2 = new JRadioButton("30 Minutes");
		rdbtnMinutes_2.setBounds(404, 316, 141, 23);
		tab_Options.add(rdbtnMinutes_2);

		//Only one options
		group.add(rdbtnSecond);
		group.add(rdbtnSeconds);
		group.add(rdbtnSeconds_1);
		group.add(rdbtnSeconds_2);
		group.add(rdbtnMinute);
		group.add(rdbtnMinutes);
		group.add(rdbtnMinutes_1);
		group.add(rdbtnMinutes_2);
		
		if (btnConnect.isEnabled()) {
			chckbxCamera.setEnabled(false);
			chckbxTemperature.setEnabled(false);
			chckbxHumidity.setEnabled(false);
			chckbxAirQuality.setEnabled(false);
			chckbxProximity.setEnabled(false);
			chckbxFire.setEnabled(false);
			chckbxLight.setEnabled(false);
			chckbxGps.setEnabled(false);
			chckbxLed.setEnabled(false);
			chckbxLight.setEnabled(false);
			
  			for (Enumeration<AbstractButton> buttons = group.getElements(); buttons.hasMoreElements();) {
	            AbstractButton button = buttons.nextElement();
	            button.setEnabled(false);
	        }
		}
		
		btnReset = new JButton("Reset");
		btnReset.setEnabled(false);
		btnReset.addActionListener(new ActionListener() {
			public void actionPerformed(ActionEvent e) {
				
				if (chckbxCamera.isSelected()) {
					chckbxCamera.setSelected(false);
				}
				if (chckbxTemperature.isSelected()) {
					chckbxTemperature.setSelected(false);
				}
				if (chckbxHumidity.isSelected()) {
					chckbxHumidity.setSelected(false);
				}
				if (chckbxAirQuality.isSelected()) {
					chckbxAirQuality.setSelected(false);
				}
				if (chckbxProximity.isSelected()) {
					chckbxProximity.setSelected(false);
				}
				if (chckbxFire.isSelected()) {
					chckbxFire.setSelected(false);
				}
				if (chckbxLight.isSelected()) {
					chckbxLight.setSelected(false);
				}
				if (chckbxGps.isSelected()) {
					chckbxGps.setSelected(false);
				}
				if (chckbxLed.isSelected()) {
					chckbxLed.setSelected(false);
				}
				if (chckbxLight.isSelected()) {
					chckbxLight.setSelected(false);
				}				
				
				chckbxCamera.setEnabled(true);
				chckbxTemperature.setEnabled(true);
				chckbxHumidity.setEnabled(true);
				chckbxAirQuality.setEnabled(true);
				chckbxProximity.setEnabled(true);
				chckbxFire.setEnabled(true);
				chckbxLight.setEnabled(true);
				chckbxGps.setEnabled(true);
				chckbxLed.setEnabled(true);
				
				group.clearSelection();
				
    			for (Enumeration<AbstractButton> buttons = group.getElements(); buttons.hasMoreElements();) {
		            AbstractButton button = buttons.nextElement();
		            button.setEnabled(true);
		        }
        			
    			btnOk.setEnabled(true);
    			options = "";
			}
		});
		btnReset.setBounds(297, 356, 117, 29);
		tab_Options.add(btnReset);
		
		btnOk = new JButton("OK");
		btnOk.setEnabled(false);
		btnOk.addActionListener(new ActionListener() {
			public void actionPerformed(ActionEvent e) {
				boolean CheckBoxselected = false;
				boolean RadioButtonselected = false;
				String time[] = null;
				
				if(e.getSource() == btnOk) {
				
					options += "OPT:";
					options += explorationName;
            		options += "/";

					if (chckbxCamera.isSelected()) {
				        client.start();
						options += "CAMERA-";
						CheckBoxselected = true;
					}
					if (chckbxAirQuality.isSelected()) {
						options += "MQ2-";
						CheckBoxselected = true;
					}
					if (chckbxProximity.isSelected()) {
						options += "HCSR04-";
						CheckBoxselected = true;
					}
					if (chckbxFire.isSelected()) {
						options += "FIRE-";
						CheckBoxselected = true;
					}
					if (chckbxLight.isSelected()) {
						options += "LDR-";
						CheckBoxselected = true;
					}
					if (chckbxGps.isSelected()) {
						options += "NEO6MV2-";
						CheckBoxselected = true;
					}
					if (chckbxLed.isSelected()) {
						options += "LED-";
						CheckBoxselected = true;
					}
					
					if (chckbxTemperature.isSelected() && chckbxHumidity.isSelected()) {
						options += "DHT22-";
						CheckBoxselected = true;
					}
					else if(chckbxTemperature.isSelected() || chckbxHumidity.isSelected()) {
						if(chckbxTemperature.isSelected())
							options += "DHT22TEMP-";
						else
							options += "DHT22HUM-";
						CheckBoxselected = true;						
					}

					
					//Time Select
            		int pos = options.lastIndexOf('-');
            		options = options.substring(0, pos);
            		options += "/";

            		for (Enumeration<AbstractButton> buttons = group.getElements(); buttons.hasMoreElements();) {
			            AbstractButton button = buttons.nextElement();

			            if (button.isSelected())
		            		RadioButtonselected = true;
			        }

			        if (RadioButtonselected) {
	            		for (Enumeration<AbstractButton> buttons = group.getElements(); buttons.hasMoreElements();) {
				            AbstractButton button = buttons.nextElement();

				            if (button.isSelected()) {

							    if(button.getText().contains("Minute")) { //Minutes
			                		time = button.getText().split(" ");
			                		options += Integer.parseInt(time[0])*60; //Minutes to Seconds
				                }
				                else { //Seconds
		                			time = button.getText().split(" ");
		                			options += time[0];
			                	}
				            }
			            } //for
		            } //if
		            else {
            			options += 1;
		            }			     
			        
//			        if (CheckBoxselected && RadioButtonselected) {
			        if (CheckBoxselected) {
		        		for (Enumeration<AbstractButton> buttons = group.getElements(); buttons.hasMoreElements();) {
				            AbstractButton button = buttons.nextElement();
				            button.setEnabled(false);
				        }
			        		
						chckbxCamera.setEnabled(false);
						chckbxTemperature.setEnabled(false);
						chckbxHumidity.setEnabled(false);
						chckbxAirQuality.setEnabled(false);
						chckbxProximity.setEnabled(false);
						chckbxFire.setEnabled(false);
						chckbxLight.setEnabled(false);
						chckbxGps.setEnabled(false);
						chckbxLed.setEnabled(false);
						chckbxLight.setEnabled(false);
		        		btnOk.setEnabled(false);
		        		btnReset.setEnabled(false);
		        		//SEND THE MESSAGE
	        			try {
							options = client.Write(options);
						} catch (IOException e1) {
							e1.printStackTrace();
						}

			        }
			        else {
		        		System.out.println("FAIL: YOU MUST SELECT A SENSOR");
		        		btnReset.doClick();
		        		btnOk.setEnabled(true);
			        }
			        
				}
			}
		});
		btnOk.setBounds(181, 356, 117, 29);
		tab_Options.add(btnOk);
		
		/* CONTROLLER */
		JPanel tab_Controller = new JPanel();
		tabbedPane.addTab("Controller", null, tab_Controller, null);
				
		Image imgUp = new ImageIcon(this.getClass().getResource("../butUp.png")).getImage();
		Image imgDown = new ImageIcon(this.getClass().getResource("../butDown.png")).getImage();
		Image imgLeft = new ImageIcon(this.getClass().getResource("../butLeft.png")).getImage();
		Image imgRight = new ImageIcon(this.getClass().getResource("../butRight.png")).getImage();
		
		JButton btnUp = new JButton("");
		btnUp.addActionListener(new ActionListener() {
			public void actionPerformed(ActionEvent e) {
				try {
					client.Write("MOV:FORWARD");
				} catch (IOException e1) {
					// TODO Auto-generated catch block
					e1.printStackTrace();
				}
			}
		});
		btnUp.setBounds(267, 258, 48, 48);
		tab_Controller.setLayout(null);
		btnUp.setIcon(new ImageIcon(imgUp));
		tab_Controller.add(btnUp);
		
		JButton btnDown = new JButton("");
		btnDown.addActionListener(new ActionListener() {
			public void actionPerformed(ActionEvent e) {
				try {
					client.Write("MOV:BACKWARD");
				} catch (IOException e1) {
					// TODO Auto-generated catch block
					e1.printStackTrace();
				}
			}
		});
		btnDown.setBounds(267, 349, 48, 48);
		btnDown.setIcon(new ImageIcon(imgDown));
		tab_Controller.add(btnDown);
		
		JButton btnLeft = new JButton("");
		btnLeft.addActionListener(new ActionListener() {
			public void actionPerformed(ActionEvent e) {
				try {
					client.Write("MOV:LEFT");
				} catch (IOException e1) {
					// TODO Auto-generated catch block
					e1.printStackTrace();
				}
			}
		});		
		btnLeft.setBounds(223, 303, 48, 48);
		btnLeft.setIcon(new ImageIcon(imgLeft));
		tab_Controller.add(btnLeft);
		
		JButton btnRight = new JButton("");
		btnRight.addActionListener(new ActionListener() {
			public void actionPerformed(ActionEvent e) {
				try {
					client.Write("MOV:RIGHT");
				} catch (IOException e1) {
					// TODO Auto-generated catch block
					e1.printStackTrace();
				}
			}
		});	
		btnRight.setBounds(313, 303, 48, 48);
		btnRight.setIcon(new ImageIcon(imgRight));
		tab_Controller.add(btnRight);
		
		JSlider sliderPan = new JSlider();
		sliderPan.setBounds(420, 368, 133, 29);
		tab_Controller.add(sliderPan);
		
		JSlider sliderTilt = new JSlider();
		sliderTilt.setBounds(524, 251, 29, 133);
		sliderTilt.setOrientation(JSlider.VERTICAL);
		tab_Controller.add(sliderTilt);
		
		JLabel lblTilt = new JLabel("Tilt");
		lblTilt.setBounds(524, 238, 29, 16);
		tab_Controller.add(lblTilt);
		
		JLabel lblPan = new JLabel("Pan");
		lblPan.setBounds(401, 368, 37, 16);
		tab_Controller.add(lblPan);
		
		ImageComponent controllerCam = new ImageComponent();
		controllerCam.setBorder(myRaisedBorder);
		controllerCam.setBounds(132, 6, 320, 240);
		tab_Controller.add(controllerCam);
		
		byte [] imagen = null;//Cargar la imagen desde la red

	    controllerCam.set8bitGrayImage( imagen );
	    controllerCam.repaint();
		
					
	}
}
