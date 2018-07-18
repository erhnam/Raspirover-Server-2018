package clientSocket;

import java.io.BufferedInputStream;
import java.io.BufferedReader;
import java.io.DataInputStream;
import java.io.IOException;
import java.io.InputStreamReader;
import java.io.OutputStream;
import java.io.OutputStreamWriter;
import java.net.InetAddress;
import java.net.Socket;

import org.omg.CORBA.portable.InputStream;

public class ClientSocket extends Thread {
    public Socket socket;
    private OutputStream output;
    private BufferedReader reader;
    private InputStream input;
   // private String message = null;
    DataInputStream dIn;
    byte buffer[] = new byte[76800];
    int length;

    private OutputStreamWriter outputSocketClient;
    
    public ClientSocket(InetAddress serverAddress, int serverPort) throws Exception {
        this.socket = new Socket(serverAddress, serverPort);
        dIn = new DataInputStream(socket.getInputStream());
    }
    
    public String Write(String data) throws IOException {
    		System.out.println(data);
    		outputSocketClient = new OutputStreamWriter(this.socket.getOutputStream());
		outputSocketClient.write(data);
		outputSocketClient.flush();  
		data = "";
		return data;
	}
    
   
    @Override
    public void run() {
    	try {
    		int i = 0;
    		
    		System.out.println("ESCUCHANDO MENSAJES...");
    		
    		while((length = dIn.readInt()) > 0){                    // read length of incoming message
    		    byte[] message = new byte[length];
    		    dIn.readFully(message, 0, message.length); // read the message
    			System.out.println(i+" "+(char)buffer[i]+" "+(byte)buffer[i]);
    		}
    		/*
       	  	while(true){
                buffer[i]=(byte) socket.getInputStream().read();
                if(buffer[i] > 0) {
                	System.out.println(i+" "+(char)buffer[i]+" "+(byte)buffer[i]);
                	i++;
                }
            }
       	  	*/
       	  	/*
    		reader = new BufferedReader(new InputStreamReader(socket.getInputStream()));
       	  	while ((message = reader.readLine()) != null) {
       	  		System.out.println("MESSAGE: " + message);
            }
            */
        } catch (IOException e) {
			// TODO Auto-generated catch block
			e.printStackTrace();
		}
    }
    
    byte [] getData() {
    	return buffer;
    }
    
    public void close() throws IOException {
        this.socket.close();
    }
}