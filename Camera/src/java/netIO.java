// NetIO - Class to handle Network Input and Output
// Created for Lockheed Martin ELDP WENDE Project 2012
//
// Code Written by Jonathan Ford
// 02-2012 thru 05-2012
//

import java.nio.ByteBuffer;
import java.net.*;
import java.util.Properties;
import java.io.*;
import com.camera.*;
import com.google.protobuf.*;

//Class to Handle Input and Output on the TCP/IP Network for Camera-C3 Messages
public class netIO{

	int role = 0;					// server = 0 client = 1
	int port = 1111;				// Default port if not specified in config file
	String host = "localhost";		// default host
	String testText = "ABC01234";
	byte msgType = 0;				// status = 0, tracks = 1, image = 2
	boolean VERBOSE = true;			//Controls screen output - set in config file
	int STATUS_DELAY = 250;
	int TRACKS_DELAY = 100;
	int IMAGE_DELAY = 100;
	static int MAIN_RUNTIME = 10000;	// how long (ms) the main program should run
	
	int count = 0;	// message counter
	
	long minTime = 0;
	long maxTime = 0;
	long avgTime = 0;
	long cTime = 0;		// current message time
	long pTime = 0;		// previous message time
	long eTime = 0;		// elapsed time
	long localProcTimeStart = 0;
	long localProcTimeStop = 0;
	
	// Byte Arrays for image data
	byte[] serialImage;
	byte[][][] image;
	
	// Network Socket placeholder
	ServerSocket serverSocket = null;
	Socket clientSocket = null;
	
	// Input and Output Stream handles - not used now?? - JF?
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
	
	// Constructor
	// duty ROV: server = 0 client = 1
	// messageType ROV: status = 0, tracks = 1, image = 2
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
	
	// This method loads the configuration and starts the class based on input parameters
	private void netStart() throws IOException
	{
		getConfig();
		
		switch (role) {
			case 0:
//				serverInit();	//server must be initialized and started by calling method.
				System.out.println("SERVER FUNCTION CALLED");
//				server();
				break;
			case 1:
				clientInit();
//				client();		// client must be started by calling method
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
	// This method sets the class variables with the respective values from the configuration file
	private void getConfig() throws IOException
	{
		Properties configFile = new Properties();
		configFile.load(this.getClass().getClassLoader().getResourceAsStream("./config/net_config.txt"));
		this.host = configFile.getProperty("host");
		this.port = Integer.parseInt((configFile.getProperty("port"))) + msgType;	//add message type to port for unique port/message
		this.testText = configFile.getProperty("testText");
		this.VERBOSE = Boolean.parseBoolean(configFile.getProperty("verbose"));
		this.STATUS_DELAY = Integer.parseInt(configFile.getProperty("statusDelay"));
		this.TRACKS_DELAY = Integer.parseInt(configFile.getProperty("tracksDelay"));
		this.IMAGE_DELAY = Integer.parseInt(configFile.getProperty("imageDelay"));
		this.MAIN_RUNTIME = Integer.parseInt(configFile.getProperty("mainRunTime"));
	}
	// Main Class for when netIO is called directly
    public static void main(String[] args) throws IOException {

       	int role = 0;
    	byte type = 0;
		
    	if(args[0] != null)							// check if second input parameter
		{
			role = Integer.parseInt(args[0]);		// read in input parameter for server or client
		}
		if(args[1] != null)							// check if second input parameter
		{
			int tempType = Integer.parseInt(args[1]);	// set type of message from 2nd input param
			type = (byte)tempType;
		}

		netIO msg = new netIO(role, type);			// create instance of this class
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
		
		
	// Server Initialization
	// Sets up socket on specified port
	public void serverInit() throws IOException {
		System.out.println("SERVER INIT STARTED");			

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
		System.out.println("SERVER INIT COMPLETE");
		return;
	}	// End Server init Method

	// overloaded server function - one for each message type (status, tracks, image)
	// These methods send the following data out over the network:
	// - An integer representing the number of bytes in the message about to be sent
	// - The message type as a byte (status = 0, tracks = 1, image = 2)
	// - Finally, the Protobuf writeTo() method is called which serializes and writes 
	// 	    the prepared message to the network
	public void server(CameraMsgs.cameraStatus sMsg) throws IOException
	{
		System.out.println("Send Status");
		byte[] bytesLength = ByteBuffer.allocate(4).putInt(sMsg.getSerializedSize()).array();
		byte[] bytesMsg = sMsg.toByteArray(); 						  
		clientSocket.getOutputStream().write(bytesLength, 0, 4);
		clientSocket.getOutputStream().write(bytesMsg, 0, sMsg.getSerializedSize());
		count++;
	} // End Server Method

	// overloaded server function - one for each message type
	public void server(CameraMsgs.cameraTracks sMsg) throws IOException
	{
		System.out.println("Send Track");
		byte[] bytesLength = ByteBuffer.allocate(4).putInt(sMsg.getSerializedSize()).array();
		byte[] bytesMsg = sMsg.toByteArray(); 						  
		clientSocket.getOutputStream().write(bytesLength, 0, 4);
		clientSocket.getOutputStream().write(bytesMsg, 0, sMsg.getSerializedSize());
		count++;
	} // End Server Method

	// overloaded server function - one for each message type
	public void server(CameraMsgs.cameraImage sMsg) throws IOException
	{
		System.out.println("Send Image");
		byte[] bytesLength = ByteBuffer.allocate(4).putInt(sMsg.getSerializedSize()).array();
		byte[] bytesMsg = sMsg.toByteArray(); 						  
		clientSocket.getOutputStream().write(bytesLength, 0, 4);
		clientSocket.getOutputStream().write(bytesMsg, 0, sMsg.getSerializedSize());
		count++;
	} // End Server Method
	
	// Method to generates some test data - needs to be updated - only used by main() 5/15 - JF
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
	
	// method to initialize Client Socket Connection
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
				cSocket = new Socket(host, port);		//Tries to connect to host on port - server needs to be responsive
				
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
		
		// Input from user for text input - not used - Delete? - JF 5/16
		stdIn = new BufferedReader(new InputStreamReader(System.in));
		System.out.println("Waiting for Server");
		return;
	}	// End clientInit Method
	
	// Method for Client Operations - reads data from network 
	// and calls protobuf methods to deserialize data.
	public void client() throws IOException
	{
		int inSize = 0;					// Number of Bytes in Message - sent by server in msg header
		byte msgType = 0;				// Type of Message - sent by server in message header
		boolean error = true;			// error flag
		long startTime = System.currentTimeMillis();		// Timing counter
		int sCount = 0;					// Status message counter
		int tCount = 0;					// tracks message counter
		int iCount = 0;					// image message counter
		long msgTime = 0;
		while ((inSize = clientInputStream.readInt())  != 0) {
			error = true;
//			System.out.println("inSize of " + inSize);
			if(inSize < 0)
			{
				if(inSize == -1)
					break;
				break;
			}
			byte []bytes = new byte[inSize]; 		// initialize byte array for incoming data
			msgType = clientInputStream.readByte();		// read message type byte from stream
			localProcTimeStart = System.currentTimeMillis();
//			System.out.println("msgType = " + msgType + " Insize = " + inSize);
			clientInputStream.readFully(bytes); 		// Read protobuf serialized data from input stream
			count++;
			switch (msgType) {						// Select which protobuf message parser to use
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
			// The following code displays parts of the protobuf message.  - JF - implementation of this should be re-thought
			if (cStatusIn != null) {
				msgTime = cStatusIn.getTime();
				p("Server("+msgType+"): Time->" + cStatusIn.getTime());
				p("Server("+msgType+"): Status->" + cStatusIn.getStatus());
				p("Server("+msgType+"): Laser->" + cStatusIn.getLaserOn());
				sCount++;
				error = false;
			}
			if (cTracksIn != null) {
				msgTime = cTracksIn.getTime();
				p("Server("+msgType+"): Time->" + cTracksIn.getTime());
				p("Server("+msgType+"): Status->" + cTracksIn.getStatus());
				tCount++;
				error = false;
			}
			if (cImageIn != null) {
				msgTime = cImageIn.getTime();
				p("Server("+msgType+"): Time->" + cImageIn.getTime());
				int channels = cImageIn.getChannels();
				int sizeX = cImageIn.getSizeX();
				int sizeY = cImageIn.getSizeY();
				if (iCount == 0) {
					serialImage = new byte[(sizeX * sizeY * channels)];
					image = new byte[channels][sizeY][sizeX];
				}
				if (serialImage.length != (sizeX * sizeY * channels))
				{
					serialImage = new byte[(sizeX * sizeY * channels)];					
				}
				cImageIn.getImageData().copyTo(serialImage,0);
				
				if (image.length != channels && image[0].length != sizeY && image[0][0].length != sizeX) {
					image = new byte[channels][sizeY][sizeX];
				}
				int index = 0;
				for (int i = 0; i<channels; i++) {
					for (int j = 0; j<sizeY; j++) {
						for (int k = 0; k<sizeX; k++) {
							image[i][j][k] = serialImage[index++];
						}
					}
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
			localProcTimeStop = System.currentTimeMillis();
			timeCalc(msgTime, localProcTimeStop - localProcTimeStart, msgType);
		} // End While Loop
		long endTime = System.currentTimeMillis();
		long duration = endTime - startTime;
		System.out.printf("\n\nStatus: %d\nTracks: %d\nImages: %d\n\n",sCount,tCount,iCount);
		System.out.printf("\n\nMinTime: %d\nMaxTime: %d\nAvgTime: %d\n\n",minTime,maxTime,avgTime);
		System.out.printf(" End Time : %d\nStart Time: %d\n -----------\n  Duration: %d\n",endTime,startTime,duration);
	} // End Client method
	
//	public void timeCalc(long msgTime){ timeCalc(msgTime, 0);}
	
	public void timeCalc(long msgTime, long localDelay, byte msgType)
	{
		int delayTime = 0;
		switch (msgType) {
			case 0:
				delayTime = STATUS_DELAY;
				break;
			case 1:
				delayTime = TRACKS_DELAY;
				break;
			case 2:
				delayTime = IMAGE_DELAY;
				break;
			default:
				System.out.println("message Type error " + msgType);
				break;
		}
				
//		cTime = System.currentTimeMillis();
//		eTime = cTime - msgTime;
		long netTime = -1;
		if (pTime == 0) {
			eTime = 0;
		}
		else {
			eTime = msgTime - pTime;
			netTime = eTime - delayTime;
		}		
		pTime = msgTime;
		if (eTime > maxTime) {
			maxTime = eTime;
		}
		else if(0 < eTime && eTime < minTime)
		{
			minTime = eTime;
		}
		if (avgTime == 0) {
			avgTime = netTime;
		}
		avgTime = ( avgTime + netTime ) / 2;
		//p(String.format("\n%d\t%d\t",eTime,delayTime));
		System.out.printf("\n(%d) Elapsed: %d\tNetDelay: %d\tLocalDelay: %d",msgType,eTime,netTime,localDelay);
			
	}	// End of timeCalc Function
		
	public void p(String printString)
	{
		if(VERBOSE)
			System.out.println(printString);
		return;
	}
	
}	// End netIO Class