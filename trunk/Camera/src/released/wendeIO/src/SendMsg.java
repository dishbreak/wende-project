// Jonathan Ford
// 27 February 2012


//import ReadWriteTextFileWithEncoding;
import java.io.*;
import com.camera.*;

import java.util.Properties;
import java.util.Random;  // for generating graphic
//import Queue;

// class for Queue of type Item
public class SendMsg implements Runnable{
	static byte STATUS = 0;
	static byte TRACKS = 1;
	static byte IMAGE = 2;
	int STATUS_DELAY = 250;
	int TRACKS_DELAY = 100;
	int IMAGE_DELAY = 1000;
	static int MAIN_RUNTIME = 30000;
	boolean VERBOSE = true;
	
	int type = -1;
	static boolean running = true;
	
	static boolean statusMsgReady = false;
	static boolean tracksMsgReady = false;
	static boolean imageMsgReady = false;
	
	CameraMsgs.cameraStatus.Builder statusBuilder = null;
	public CameraMsgs.cameraStatus status= null;
	
	CameraMsgs.cameraTracks.Builder tracksBuilder = null;
	CameraMsgs.cameraTracks tracks = null;
	
	CameraMsgs.cameraImage.Builder imageBuilder = null;
	CameraMsgs.cameraImage image = null;
	
	CameraMsgs.systemStatus sysStatus = CameraMsgs.systemStatus.UNKNOWN;
	
	// Status Message Variables
	//setStatus(long time,CameraMsgs.systemStatus sysStat, boolean laserOn, String text)
	static long status_time = 0;
	static CameraMsgs.systemStatus sysStat = CameraMsgs.systemStatus.UNKNOWN; // used for both status and tracks messages
	static boolean laserOn = false;		// used for both status and tracks messages
	static String status_text = "The Camera IPT Rocks!";
	
	// Tracks Message Variables
//	setTracks(long time,	CameraMsgs.systemStatus sysStat, int[][] targets, int[][] lasers, boolean laserOn)
	static long tracks_time = 0;
	static int[][] targets = {{700,5},{0,0}};
	static int[][] lasers = {{700,0}};
	
	// Image Message Variables
//	setImage(long time, int channels, int sizeX, int sizeY, com.google.protobuf.ByteString serialImage)
	static long image_time = 0;
	static int channels = 3;
	static int sizeX;
	static int sizeY;
	static byte[] serialImage = new byte[sizeX*sizeY*channels];
//	static com.google.protobuf.ByteString serialImage = com.google.protobuf.ByteString.copyFrom(new byte[(channels * sizeX * sizeY)]);
	
	static netIO netStat = null;
	static netIO netTracks = null;
	static netIO netImage = null;
	
	public void p(String printString)
	{
		if(VERBOSE)
			System.out.println(type + " -> " +	printString);
		return;
	}
	
	public SendMsg() throws IOException
	{
		getConfig();
		netStat = new netIO(0, STATUS);
		netTracks = new netIO(0, TRACKS);
		netImage = new netIO(0, IMAGE);
		p("Leaving SendMsg Constructor");
		return;
	}
	
	public SendMsg(int netType) throws IOException
	{
		type = netType;
		getConfig();
		p("new SendMsg type= "+ type);
		switch (type) {
			case 0:		//STATUS
				netStat = new netIO(0, STATUS);
				break;
			case 1:		//TRACKS
				netTracks = new netIO(0, TRACKS);
				break;
			case 2:		//IMAGE
				netImage = new netIO(0, IMAGE);
				break;
			default:
				p("Didn't match any cases in constructor");
				break;
		}
		return;
	}
	
	public void run()
	{
		p("Run()");
		try {
			switch (type) {
				case 0:		//STATUS
					netStat.serverInit();
//					try {
//						Thread.sleep( 1000 );
//						
//					}
//					catch ( InterruptedException e) {
//						e.printStackTrace();
//					}	
					statusRun();
					netStat.close();
					break;
				case 1:		//TRACKS
					netTracks.serverInit();
//					try {
//						Thread.sleep( 900 );
//						
//					}
//					catch ( InterruptedException e) {
//						e.printStackTrace();
//					}	
					tracksRun();
					netTracks.close();
					break;
				case 2:		//IMAGE
					netImage.serverInit();
//					try {
//						Thread.sleep( 300 );
//						
//					}
//					catch ( InterruptedException e) {
//						e.printStackTrace();
//					}	
					imageRun();
					netImage.close();
					break;
				default:
					p("Didn't match any cases in constructor");
					break;
			}
		}
		catch (IOException e) {
			e.printStackTrace();
		}
		finally {
			p("Done");
		}
		
		return;
	}
	
	private void statusRun() throws IOException
	{
		p("in StatusRun()");
		while (running) {
			try{
				if (statusMsgReady) {
					statusMsgReady = !(sendStatus());
					//p("send Status");
					Thread.sleep( STATUS_DELAY );
				}
				else {
//					statusMsgReady = setStatus(System.currentTimeMillis(),this.sysStatus.OPERATIONAL,false,"TEST-STRING");	// setup status message
					statusMsgReady = setStatus(this.status_time,this.sysStat,this.laserOn,this.status_text);	// setup status message
					//p("setStatus");
				}					
			}
			catch ( InterruptedException e) {
				e.printStackTrace();
			}
			catch (IOException e) {
				e.printStackTrace();
			}
		} // end while loop
		return;
	}	// End statusRun method
	
	private void tracksRun() throws IOException
	{
		while (running) {
			try {
				if (tracksMsgReady) {
					//p("ready to send Tracks");
					tracksMsgReady = !(sendTracks());
					Thread.sleep( TRACKS_DELAY );
				}
				else {
					int[][] targets = {{1,1},{0,0},{100,10}};
					int[][] lasers = {{22,54}};
//					tracksMsgReady = setTracks(System.currentTimeMillis(),this.sysStatus.OPERATIONAL,targets,lasers,true);	// setup tracks message
					tracksMsgReady = setTracks(this.tracks_time,this.sysStat,this.targets,this.lasers,this.laserOn,true,true);	// setup tracks message
				}					
			}
			catch ( InterruptedException e) {
				e.printStackTrace();
			}
			catch (IOException e) {
				e.printStackTrace();
			}
		} // end while loop
		return;
	}	// End tracksRun method
	
	private void imageRun()
	{
		while (running) {
			try {
				if (imageMsgReady) {
					imageMsgReady = !(sendImage());
					Thread.sleep( IMAGE_DELAY );					
				}
				else {
					if(serialImage.length != (channels * sizeX * sizeY))
					{
						serialImage = new byte[(channels * sizeX * sizeY)];						
					} // else use already defined byte array
					
					Random generator = new Random();
					int index = 0;
					for (int i = 0; i<channels; i++) {
						for (int j = 0; j<sizeY; j++) {
							for (int k = 0; k<sizeX; k++) {
								serialImage[index++] = (byte)(generator.nextInt(128));	// Generate random number 0-127 MAX_SIZE for byte							
							}
						}
					}
//					imageMsgReady = setImage(System.currentTimeMillis(),channels,sizeX,sizeY,com.google.protobuf.ByteString.copyFrom(serialImage));	// setup status message
					imageMsgReady = setImage(this.image_time,this.channels,this.sizeX,this.sizeY,this.serialImage);	// setup status message
				}
			}
			catch ( InterruptedException e) {
				e.printStackTrace();
			}
			catch (IOException e) {
				e.printStackTrace();
			}
		} // end while loop
		return;
	}	// End tracksRun method
	
	
	
//	public static void main(String... aArgs) throws IOException
//	{
//		SendMsg mainMsg = new SendMsg();
//		mainMsg.runProgram(aArgs);
//		System.out.println("Out of Run Program");
//		return;
//	}
	public void runProgram(String... aArgs) throws IOException
	{
//		
//		if (aArgs.length > 0) {
//			if(Integer.parseInt(aArgs[0]) == 1) {
//				p("In matlab Specific initialization");
//			}
//				
//			SendMsg msg = new SendMsg();
//		}
//		else {
			SendMsg mStatus = new SendMsg(0);  // Create object instance
			SendMsg mTracks = new SendMsg(1);
			SendMsg mImage = new SendMsg(2);
			
//			Thread tStatus = new Thread(mStatus);
//			Thread tTracks = new Thread(mTracks);
//			Thread tImage = new Thread(mImage);
//
//			tStatus.start();
//			System.out.println("tStatus Started");
//			
//			tTracks.start();
//			System.out.println("tTracks Started");
//
//			tImage.start();
//			System.out.println("tImage Started");			
//		}
		long time = System.currentTimeMillis();
		long duration = MAIN_RUNTIME;
		while (System.currentTimeMillis() < time+duration) {
			//Do Nothing - everything in threads
			try {
				Thread.sleep (MAIN_RUNTIME );
			}
			catch (InterruptedException e) {
				e.printStackTrace();
			}
		}
		running	= false;	// stop threads
		// Close Network interfaces
		netImage.close();
		netTracks.close();
		netStat.close();
		System.out.println("Ending Program");
		return;
	}	// End runProgram Method
	
	
	public boolean setStatus(long time,CameraMsgs.systemStatus sysStat, boolean laserOn, String text) throws IOException
	{
		statusBuilder = CameraMsgs.cameraStatus.newBuilder();
		status = statusBuilder.setTime(time)
		.setStatus(sysStat)
		.setLaserOn(laserOn)
		.setText(text)
		.build();
		return true;
	}
	public boolean setTracks(long time,	CameraMsgs.systemStatus sysStat, int[][] targets, int[][] lasers, boolean laserOn, boolean target0, boolean target1)
	{
		tracksBuilder = CameraMsgs.cameraTracks.newBuilder();
		tracksBuilder	= tracksBuilder.setTime(time)
		.setStatus(sysStat);

		if(!target0)
		{
			p("No tracks to Send");
		}
		else if(!target1)
		{
			tracksBuilder = tracksBuilder.addTarget(CameraMsgs.track.newBuilder().setX(targets[0][0]).setY(targets[0][1]));
			p("TCoordinate : ("+targets[0][0] + "," + targets[0][1]);
		}
		else
		{
			for (int i=0; i<targets.length; i++) {
				tracksBuilder = tracksBuilder.addTarget(CameraMsgs.track.newBuilder().setX(targets[i][0]).setY(targets[i][1]));
				p("TCoordinate "+i+": ("+targets[i][0] + "," + targets[i][1]);
			}
		}
		if(laserOn)
		{
			for (int i=0; i<lasers.length; i++) {
				tracksBuilder.addLaser(CameraMsgs.track.newBuilder().setX(lasers[i][0])
									   .setY(lasers[i][1]).build());
				p("LCoordinate "+i+": ("+lasers[i][0] + "," + lasers[i][1]);
			}			
		}
		else
			p("No Laser Track");

		tracks = tracksBuilder.setLaserOn(laserOn)
		.build();
		return true;
	}	// end setTracks method
	
	public boolean setImage(long time, int channels, int sizeX, int sizeY, byte[] serialImage)
	{
		imageBuilder = CameraMsgs.cameraImage.newBuilder();
		image = imageBuilder.setTime(time)
		.setChannels(channels)
		.setSizeX(sizeX)
		.setSizeY(sizeY)
		.setImageData(com.google.protobuf.ByteString.copyFrom(serialImage))
		.build();
		return true;
	}	// End setimage method
	
	public boolean sendStatus() throws IOException
	{
		if (status == null) {
			//p("status message not sent");
			return true;
		}
		
		netStat.server(status);
		//p("status out : " + System.currentTimeMillis());
		return true;
	}	// End sendStatus method
	
	public boolean sendTracks() throws IOException
	{
		if (tracks == null) {
			//p("track message not sent");
			return true;
		}
		
		netTracks.server(tracks);
		//p("Tracks Out : " + System.currentTimeMillis());
		return true;
	}	// End sendStatus method
	
	public boolean sendImage() throws IOException
	{
		if (image == null) {
			p("image message not sent");
			return true;
		}
		
		netImage.server(image);
		p("Image Out : " + System.currentTimeMillis());
		return true;
	}	// End sendStatus method
	
	private void getConfig() throws IOException
	{
		Properties configFile = new Properties();
		configFile.load(this.getClass().getClassLoader().getResourceAsStream("./config/net_config.txt"));
		
		this.STATUS_DELAY = Integer.parseInt(configFile.getProperty("statusDelay"));
		this.TRACKS_DELAY = Integer.parseInt(configFile.getProperty("tracksDelay"));
		this.IMAGE_DELAY = Integer.parseInt(configFile.getProperty("imageDelay"));
		this.MAIN_RUNTIME = Integer.parseInt(configFile.getProperty("mainRunTime"));
		this.VERBOSE = Boolean.parseBoolean(configFile.getProperty("verbose"));
		this.sizeX = Integer.parseInt(configFile.getProperty("sizeX"));
		this.sizeY = Integer.parseInt(configFile.getProperty("sizeY"));
		this.channels = Integer.parseInt(configFile.getProperty("channels"));		
	}
	
	public boolean initNet(int msg) throws IOException {
		try {
			
		switch(msg) {
		case 0:
			netStat.serverInit();
			break;
		case 1:
			netTracks.serverInit();
			break;
		case 2:
			netImage.serverInit();
			break;
		}  // end Switch
		}
		catch(IOException e) {
			p("Caught IOException");
			return false;
		}
		catch(NullPointerException e) {
			p("Caught NullPointerException");
		}
		return true;
	}  // end InitNet
	
	public boolean closeNet(int msg) throws IOException {
		try {
		switch(msg) {
		case 0:
			netStat.close();
			break;
		case 1:
			netTracks.close();
			break;
		case 2:
			netImage.close();
			break;
		}  // end Switch
		}
		catch(IOException e) {
			p("Caught IOException");
			return false;
		}
		return true;
	}  // end closeNet
		
//		// Image Message Variables
////		setImage(long time, int channels, int sizeX, int sizeY, com.google.protobuf.ByteString serialImage)

//		static byte[] serialImage = new byte[sizeX*sizeY*channels];
////		static com.google.protobuf.ByteString serialImage = com.google.protobuf.ByteString.copyFrom(new byte[(channels * sizeX * sizeY)]);

	public void setStatusTime(long time) {
		this.status_time = time;
		return;
	}
	public void setTracksTime(long time) {
		this.tracks_time = time;
		return;
	}
	public void setImageTime(long time) {
		this.image_time = time;
		return;
	}
	public void setLaserOn(boolean laserState) {
		this.laserOn = laserState;
		return;
	}
	public void setStatusText(String text) {
		this.status_text = text;
		return;
	}
	public void setChannels(int channels) {
		this.channels = channels;
		return;
	}
	public void setSize(int width, int height) {
		this.sizeX = width;
		this.sizeY = height;
		return;
	}
	public void setTargets(int[][] targets) {
		this.targets = targets;
		return;
	}
	public void setLaser(int[][] lasers) {
		this.lasers = lasers;
		return;
	}
	public void setImage(byte[] inputSerial) {
		if(this.serialImage.length != inputSerial.length) {
			this.serialImage = new byte[inputSerial.length];
			p("Image size adjusted to match incoming data");
		}
			this.serialImage = inputSerial;
		return;
	}
	static public void setSystemStatus(int status) {
		switch(status){
		case 0:
			SendMsg.sysStat = CameraMsgs.systemStatus.DOWN;
			break;
		case 1:
			SendMsg.sysStat = CameraMsgs.systemStatus.ERROR;
			break;
		case 2:
			SendMsg.sysStat = CameraMsgs.systemStatus.FAILED;
			break;
		case 3:
			SendMsg.sysStat = CameraMsgs.systemStatus.OPERATIONAL;
			break;
		case 4:
			SendMsg.sysStat = CameraMsgs.systemStatus.READY;
			break;
		case 5:
			SendMsg.sysStat = CameraMsgs.systemStatus.UNKNOWN;
			break;
		default:
			SendMsg.sysStat = CameraMsgs.systemStatus.UNKNOWN;
		} // End Switch
	} // End setSystemStatus Method
	
//	static boolean running = true;
//	
//	static boolean statusMsgReady = false;
//	static boolean tracksMsgReady = false;
//	static boolean imageMsgReady = false;
	
	public void setStatusReady(boolean statusReady) {
		statusMsgReady = statusReady;
		return;
	}
	public void setTracksReady(boolean tracksReady) {
		tracksMsgReady = tracksReady;
		return;
	}
	public void setImageReady(boolean imageReady) {
		imageMsgReady = imageReady;
		return;
	}
// All The GET Methods
		public long getStatusTime() {
			return status_time;
		}
		public long getTracksTime() {
			return tracks_time;
		}
		public long getImageTime() {
			return image_time;
		}
		public boolean getLaserOn() {
			return laserOn;
		}
		public String getStatusText() {
			return status_text;
		}
		public int getChannels() {
			return channels;
		}
		public int getWidth() {
			return sizeX;
		}
		public int getHeight() {
			return sizeY;
		}
		public int[][] getTargets() {
			return targets;
		}
		public int[][] getLaser() {
			return lasers;
		}
		public byte[] getImage() {		
			return serialImage;
		}
		public boolean getStatusReady() {
			return statusMsgReady;
		}	
		public boolean getTracksReady() {
			return tracksMsgReady;
		}	
		public boolean getImageReady() {
			return imageMsgReady;
		}	
		public int getType() {
			return type;
		}
		public CameraMsgs.systemStatus getSystemStatus() {
			return sysStat;
		}
}	// End class SendMsg