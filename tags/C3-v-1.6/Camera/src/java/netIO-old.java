/*
 * Copyright (c) 1995, 2008, Oracle and/or its affiliates. All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 *
 *   - Redistributions of source code must retain the above copyright
 *     notice, this list of conditions and the following disclaimer.
 *
 *   - Redistributions in binary form must reproduce the above copyright
 *     notice, this list of conditions and the following disclaimer in the
 *     documentation and/or other materials provided with the distribution.
 *
 *   - Neither the name of Oracle or the names of its
 *     contributors may be used to endorse or promote products derived
 *     from this software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS
 * IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO,
 * THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR
 * PURPOSE ARE DISCLAIMED.  IN NO EVENT SHALL THE COPYRIGHT OWNER OR
 * CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL,
 * EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
 * PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR
 * PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF
 * LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING
 * NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
 * SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */ 

import java.net.*;
import java.util.Properties;
import java.io.*;
import com.camera.*;
import com.google.protobuf.*;

public class netIO{
	public netIO(int role, String message) throws IOException
	{
		Properties configFile = new Properties();
		configFile.load(this.getClass().getClassLoader().getResourceAsStream("./config/net_config.txt"));
		String host = configFile.getProperty("host");
		int port = Integer.parseInt((configFile.getProperty("port")));
	
		String testText = message;
		switch (role) {
			case 0:
				server(host,port,testText);
				break;
			case 1:
				client(host,port);
				break;
			default:
				System.out.println("Something went wrong, did not recieve correct role");
				System.err.println("Didn't choose server or client.");
				System.exit(1);
		}	// End Switch on 'role'
	}
	public netIO(int role) throws IOException
	{
		Properties configFile = new Properties();
		configFile.load(this.getClass().getClassLoader().getResourceAsStream("./config/net_config.txt"));
		String host = configFile.getProperty("host");
		int port = Integer.parseInt((configFile.getProperty("port")));
		String testText = configFile.getProperty("testText");
//		String testText = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz1234567890";		
		switch (role) {
			case 0:
				server(host,port,testText);
				break;
			case 1:
				client(host,port);
				break;
			default:
				System.out.println("Something went wrong, did not recieve correct role");
				System.err.println("Didn't choose server or client.");
				System.exit(1);
		}	// End Switch on 'role'
	}
	public netIO(int role, Queue q1) throws IOException
	{
		Properties configFile = new Properties();
		configFile.load(this.getClass().getClassLoader().getResourceAsStream("./config/net_config.txt"));
		String host = configFile.getProperty("host");
		int port = Integer.parseInt((configFile.getProperty("port")));
		String testText = configFile.getProperty("testText");
		//		String testText = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz1234567890";		
		switch (role) {
			case 0:
				server(host,port,testText);
				break;
			case 1:
				client(host,port);
				break;
			default:
				System.out.println("Something went wrong, did not recieve correct role");
				System.err.println("Didn't choose server or client.");
				System.exit(1);
		}	// End Switch on 'role'
	}
	
    public static void main(String[] args) throws IOException {
		

		
//		String host = "localhost";
		int role = Integer.parseInt(args[0]);
//		int port = Integer.parseInt(args[1]);
		
		if (args.length > 1) {
			String message = args[2];
			netIO msg = new netIO(role,message);
		}
		else {
			netIO msg = new netIO(role);
		}

		System.out.println("Done with function");
		return;
	} // end Main Function
	
	// Server
	private void server(String host, int port, String message) throws IOException {
					
		ServerSocket serverSocket = null;
		CameraMsgs.cameraStatus.Builder sStatusBuilder = null;
		CameraMsgs.cameraStatus sMsgStatus = null;
		ObjectOutputStream serverOutStream = null;
		ObjectInputStream serverInputStream = null;
		try {
			serverSocket = new ServerSocket(port);
			} catch (IOException e) {
				System.err.println("Could not listen on port: "+port);
				System.exit(1);
			}

		Socket clientSocket = null;
		try {
			clientSocket = serverSocket.accept();
			System.out.println("Client Accepted");
			} catch (IOException e) {
				System.err.println("Accept failed.");
				System.exit(1);
			}

		serverOutStream = new ObjectOutputStream(clientSocket.getOutputStream());
		serverInputStream = new ObjectInputStream(clientSocket.getInputStream());
				
		System.out.println("Ready for Inputs");

		int count = 0;
		long t= System.currentTimeMillis();
		long end = t+1000;

		while(System.currentTimeMillis() < end){

			// create Builder
			sStatusBuilder = CameraMsgs.cameraStatus.newBuilder();
					
			// Set initial conditions and create message object
			sMsgStatus = sStatusBuilder.setSensorStatus(true)
				.setLaserOn(true)
				.setText("Test ProtoBuf " + count + " " + message)
				.build();
			serverOutStream.writeByte(sMsgStatus.getSerializedSize()); 
			sMsgStatus.writeTo(serverOutStream);
			count++;
		}
		serverOutStream.writeByte(-1);
		serverOutStream.close();
		serverOutStream.close();
		clientSocket.close();
		serverSocket.close();
	} // End Server Method
				
	private void client(String host, int port) throws IOException {

		// create Builder
		CameraMsgs.cameraStatus.Builder cStatus = CameraMsgs.cameraStatus.newBuilder();

		// Set initial conditions and create message object
		CameraMsgs.cameraStatus cMsgStatus = cStatus.setSensorStatus(true)
			.setLaserOn(true)
			.setText("Test ProtoBuf")
			.build();

		Socket cSocket= null;
		ObjectOutputStream clientOutStream = null;
		ObjectInputStream clientInputStream = null;
		CameraMsgs.cameraStatus cStatusIn = null;

		try {
			// Create network socket
			cSocket = new Socket(host, port);
				
			//Create Object Streams
			clientOutStream = new ObjectOutputStream(cSocket.getOutputStream());
			clientInputStream = new ObjectInputStream(cSocket.getInputStream());
				
			} catch (UnknownHostException e) {
				System.err.println("Don't know about host: " + host);
				System.exit(1);
			} catch (IOException e) {
				System.err.println("Couldn't get I/O for the connection to: " + host);
				System.exit(1);
			}
			
		// Input from user for text input
		BufferedReader stdIn = new BufferedReader(new InputStreamReader(System.in));
		System.out.println("Waiting for Server");
		byte inSize = 0;

		while ((inSize = clientInputStream.readByte())  != 0) {
			if(inSize == -1)
			break;
			byte []bytes = new byte[inSize]; 
			clientInputStream.readFully(bytes); 
			cStatusIn = CameraMsgs.cameraStatus.parseFrom(bytes);
		} // End While Loop

		System.out.println("Server: " + cStatusIn.getText());
			
		clientOutStream.close();
		clientInputStream.close();
		stdIn.close();
		cSocket.close();
	} // End Client method
}	// End netIO Class