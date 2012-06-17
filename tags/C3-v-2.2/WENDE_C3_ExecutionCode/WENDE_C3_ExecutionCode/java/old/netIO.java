// NetIO - Class to handle Network Input and Output
// Created for Lockheed Martin ELDP WENDE Project 2012
//
// Code Written by Jonathan Ford
// 02-2012 thru 04-2012
//


import java.net.*;
import java.util.Properties;
import java.io.*;
import com.camera.*;
import com.google.protobuf.*;

public class netIO{
	
	int role = 0;		// server = 0 client = 1
	int port = 1111;
	String host = "localhost";
	String testText = "ABC01234";
	byte msgType = 0;
	boolean VERBOSE = true;
	
	int count = 0;	// message counter
	
	ServerSocket serverSocket = null;
	Socket clientSocket = null;
	
	ObjectOutputStream serverOutStream = null;
	ObjectInputStream serverInputStream = null;
	
	CameraMsgs.cameraStatus.Builder sStatusBuilder = null;
	CameraMsgs.cameraStatus sMsgStatus = null;
	
	//Client Objects
	Socket cSocket= null;
	ObjectOutputStream clientOutStream = null;
	ObjectInputStream clientInputStream = null;
	CameraMsgs.cameraStatus cStatusIn = null;
	CameraMsgs.cameraTracks cTracksIn = null;
	CameraMsgs.cameraImage cImageIn = null;
	
	BufferedReader stdIn = null;
	
	//Constructor
	public netIO(int duty) throws IOException
	{
		role = duty;
		netStart();
	}	
	
	//Constructor
	public netIO(int duty, byte messageType) throws IOException
	{
		msgType = messageType;
		role = duty;
		netStart();
	}	
	// Constructor
	public netIO(int duty, String message) throws IOException
	{
		role = duty;
		testText = message;
		netStart();
	}
		
	// Constructor
//	public netIO(int duty, Queue q1) throws IOException
//	{
//		role = duty;
//		//		Queue msgQueue = q1;
//		netStart();
//	}
	
	private void netStart() throws IOException
	{
		getConfig();
		
		switch (role) {
			case 0:
//				serverInit();	//server needs to be initialized by calling function.
//				server();
				break;
			case 1:
				clientInit();
//				client();
				break;
			default:
				System.out.println("Something went wrong, did not recieve correct role");
				System.err.println("Didn't choose server or client.");
				System.exit(1);
		}	// End Switch on 'role'
//		System.out.println("Net Started on port" + port);
		return;	
//		close();	// close sockets and IO channels
	}

	public void close() throws IOException
	{
		try {
			if(role == 0)		// Server - Closes Server connections
			{
				serverOutStream.writeInt(-1);		// Write byte so client knows we are done
				serverOutStream.close();
				serverOutStream.close();
				clientSocket.close();
				serverSocket.close();
			}
			else				// Client - Closes Client Connections
			{
				
				clientOutStream.close();
				clientInputStream.close();
				stdIn.close();
				cSocket.close();
			}
			return;
		}
		catch (IOException e) {
			System.out.println("Couldn't Close correctly");
		}
		finally {
			return;
		}
		
	}
	private void getConfig() throws IOException
	{
		Properties configFile = new Properties();
		configFile.load(this.getClass().getClassLoader().getResourceAsStream("./config/net_config.txt"));
		this.host = configFile.getProperty("host");
		this.port = Integer.parseInt((configFile.getProperty("port"))) + msgType;	//add message type to port for unique port/message
		this.testText = configFile.getProperty("testText");
		this.VERBOSE = Boolean.parseBoolean(configFile.getProperty("verbose"));		
	}
	// Main Class
    public static void main(String[] args) throws IOException {

		int role = Integer.parseInt(args[0]);
		byte type = 0;
		if(args[1] != null)
		{
			int tempType = Integer.parseInt(args[1]);
			type = (byte)tempType;
		}

		netIO msg = new netIO(role, type);		
		switch (role) {
			case 0:
				msg.server_old();
				break;
			case 1:
				msg.client();
				break;
			default:
				System.out.println("Problem with Main switch on Role");
				break;
		}
		msg.close();
		System.out.println("Done with function");
		return;
	} // end Main Function
	
	public boolean setMessage(CameraMsgs.cameraStatus status)
	{
		msgType = 1;
		sMsgStatus = status;
		return true;
	}
		
		
	// Server
	public void serverInit() throws IOException {
					

		try {
			serverSocket = new ServerSocket(port);
			} catch (IOException e) {
				System.err.println("Could not listen on port: "+port);
				System.exit(1);
			}


		try {
			System.out.println("Waiting on Client on port "+ port);
			clientSocket = serverSocket.accept();		// waits for client to connect
			System.out.println("Client Accepted");
			} catch (IOException e) {
				System.err.println("Accept failed.");
				System.exit(1);
			}

		// define server Streams
		serverOutStream = new ObjectOutputStream(clientSocket.getOutputStream());
		serverInputStream = new ObjectInputStream(clientSocket.getInputStream());
		return;
	}	// End Server init Method

	// overloaded server function - one for each message type
	public void server(CameraMsgs.cameraStatus sMsg) throws IOException
	{
			serverOutStream.writeInt(sMsg.getSerializedSize());
			serverOutStream.writeByte(msgType);		// write type of message to stream
			sMsg.writeTo(serverOutStream);
			count++;
	} // End Server Method

	// overloaded server function - one for each message type
	public void server(CameraMsgs.cameraTracks sMsg) throws IOException
	{
		serverOutStream.writeInt(sMsg.getSerializedSize());
		serverOutStream.writeByte(msgType);		// write type of message to stream
		sMsg.writeTo(serverOutStream);
		count++;
	} // End Server Method

	// overloaded server function - one for each message type
	public void server(CameraMsgs.cameraImage sMsg) throws IOException
	{
//		System.out.println("Image Serial Size = "+ sMsg.getSerializedSize());
		serverOutStream.writeInt(sMsg.getSerializedSize());
		serverOutStream.writeByte(msgType);		// write type of message to stream
		sMsg.writeTo(serverOutStream);
		count++;
	} // End Server Method
	

	public void server_old() throws IOException
	{
		count = 0;
		long t= System.currentTimeMillis();
		long end = t+10000;


		while(System.currentTimeMillis() < end){

			// create Builder
			sStatusBuilder = CameraMsgs.cameraStatus.newBuilder();
					
			// Set initial conditions and create message object
			sMsgStatus = sStatusBuilder.setTime(System.currentTimeMillis())
				.setLaserOn(true)
				.setText("Test ProtoBuf " + count + " " + testText)
				.build();
			serverOutStream.writeByte(sMsgStatus.getSerializedSize());
			serverOutStream.writeByte(msgType);		// write type of message to stream
			sMsgStatus.writeTo(serverOutStream);
			count++;
		}

	} // End Server Method
				
	private void clientInit() throws IOException {

		// create Builder
		CameraMsgs.cameraStatus.Builder cStatus = CameraMsgs.cameraStatus.newBuilder();

		// Set initial conditions and create message object
		CameraMsgs.cameraStatus cMsgStatus = cStatus.setLaserOn(true)
			.setText("Test ProtoBuf")
			.build();
		int tries = 1;
		while(cSocket == null)
		{
			
			try {
				// Create network socket
				System.out.println(tries + " -> Client connecting to "+host+" on port "+port);
				cSocket = new Socket(host, port);
				
				//Create Object Streams
				clientOutStream = new ObjectOutputStream(cSocket.getOutputStream());
				clientInputStream = new ObjectInputStream(cSocket.getInputStream());
				
			} catch (UnknownHostException e) {
				System.err.println("Don't know about host: " + host);
				//				System.exit(1);
			} catch (IOException e) {
				System.err.println(tries + " -> Couldn't get I/O for the connection to: " + host);
				//				System.exit(1);
			}
			try {
				Thread.sleep( 1000 );
				
			}
			catch ( InterruptedException e) {
				e.printStackTrace();
			}
			tries++;
		}	// End While Loop
		
		// Input from user for text input
		stdIn = new BufferedReader(new InputStreamReader(System.in));
		System.out.println("Waiting for Server");
		return;
	}	// End clientInit Method
	
	public void client() throws IOException
	{
		int inSize = 0;
		byte msgType = 0;
		boolean error = true;
		long startTime = System.currentTimeMillis();
		int sCount = 0;
		int tCount = 0;
		int iCount = 0;
		while ((inSize = clientInputStream.readInt())  != 0) {
			error = true;
//			System.out.println("inSize of " + inSize);
			if(inSize < 0)
			{
				if(inSize == -1)
					break;
				break;
			}
			byte []bytes = new byte[inSize]; 
			msgType = clientInputStream.readByte();
//			System.out.println("msgType = " + msgType + " Insize = " + inSize);
			clientInputStream.readFully(bytes); 
			switch (msgType) {
				case 0:
					cStatusIn = CameraMsgs.cameraStatus.parseFrom(bytes);
					break;
				case 1:
					cTracksIn = CameraMsgs.cameraTracks.parseFrom(bytes);
					break;
				case 2:
					cImageIn = CameraMsgs.cameraImage.parseFrom(bytes);
					break;
				default:
					System.out.println("message Type error " + msgType);
					break;
			}
			if (cStatusIn != null) {
				p("Server("+msgType+"): Time->" + cStatusIn.getTime());
				p("Server("+msgType+"): Status->" + cStatusIn.getStatus());
				p("Server("+msgType+"): Laser->" + cStatusIn.getLaserOn());
				sCount++;
				error = false;
			}
			if (cTracksIn != null) {
				p("Server("+msgType+"): Time->" + cTracksIn.getTime());
				p("Server("+msgType+"): Status->" + cTracksIn.getStatus());
				tCount++;
				error = false;
			}
			if (cImageIn != null) {
				p("Server("+msgType+"): Time->" + cImageIn.getTime());
				int channels = cImageIn.getChannels();
				int sizeX = cImageIn.getSizeX();
				int sizeY = cImageIn.getSizeY();
				byte[] serialImage = new byte[(sizeX * sizeY * channels)];
				cImageIn.getImageData().copyTo(serialImage,0);
				byte[][][] image;
				image = new byte[channels][sizeY][sizeX];
				int index = 0;
				for (int i = 0; i<channels; i++) {
					for (int j = 0; j<sizeY; j++) {
						for (int k = 0; k<sizeX; k++) {
//							System.out.print(" " + serialImage[index]);
							image[i][j][k] = serialImage[index++];
						}
//						System.out.println("");
					}
//					System.out.println("Channel "+i);
				}
				
				p("Server("+msgType+"): Channels->" +channels);
				p("Server("+msgType+"): SizeX->" + sizeX);
				p("Server("+msgType+"): SizeY->" + sizeY);

//				System.out.println("Server("+msgType+"): Image->");
				iCount++;
				error = false;
			}
			if (error){
				System.out.println("Server: error msg type = " + msgType);
			}
		} // End While Loop
		long endTime = System.currentTimeMillis();
		long duration = endTime - startTime;
		System.out.printf("\n\nStatus: %d\nTracks: %d\nImages: %d\n\n",sCount,tCount,iCount);
		System.out.printf(" End Time : %d\nStart Time: %d\n -----------\n  Duration: %d\n",endTime,startTime,duration);
	} // End Client method
	
	public void p(String printString)
	{
		if(VERBOSE)
			System.out.println(printString);
		return;
	}
	
}	// End netIO Class