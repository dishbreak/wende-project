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
	int channels = 3;
	int sizeX = 640;
	int sizeY = 480;
	
	int type = -1;
	static boolean running = true;
	
	static boolean statusMsgReady = false;
	static boolean tracksMsgReady = false;
	static boolean imageMsgReady = false;
	
	CameraMsgs.cameraStatus.Builder statusBuilder = null;
	CameraMsgs.cameraStatus status= null;
	
	CameraMsgs.cameraTracks.Builder tracksBuilder = null;
	CameraMsgs.cameraTracks tracks = null;
	
	CameraMsgs.cameraImage.Builder imageBuilder = null;
	CameraMsgs.cameraImage image = null;
	
	CameraMsgs.systemStatus sysStatus = CameraMsgs.systemStatus.UNKNOWN;
	
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
					p("send Status");
					Thread.sleep( STATUS_DELAY );
				}
				else {
					statusMsgReady = setStatus(System.currentTimeMillis(),this.sysStatus.OPERATIONAL,false,"TEST-STRING");	// setup status message
					p("setStatus");
				}					
			}
			catch ( InterruptedException e) {
				e.printStackTrace();
			}
			catch (IOException e) {
				e.printStackTrace();
			}
		} // end while loop
	}	// End statusRun method
	
	private void tracksRun() throws IOException
	{
		while (running) {
			try {
				if (tracksMsgReady) {
					p("ready to send Tracks");
					tracksMsgReady = !(sendTracks());
					Thread.sleep( TRACKS_DELAY );
				}
				else {
					int[][] targets = {{1,1},{100,100},{100,10}};
					int[][] lasers = {{22,54}};
					tracksMsgReady = setTracks(System.currentTimeMillis(),this.sysStatus.OPERATIONAL,targets,lasers,true);	// setup tracks message
				}					
			}
			catch ( InterruptedException e) {
				e.printStackTrace();
			}
			catch (IOException e) {
				e.printStackTrace();
			}
		} // end while loop
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
					byte[] serialImage = new byte[(channels * sizeX * sizeY)];
					
					Random generator = new Random();
					int index = 0;
					for (int i = 0; i<channels; i++) {
						for (int j = 0; j<sizeY; j++) {
							for (int k = 0; k<sizeX; k++) {
								serialImage[index++] = (byte)generator.nextInt(128);	// Generate random number 0-127 MAX_SIZE for byte
							}
						}
					}
					imageMsgReady = setImage(System.currentTimeMillis(),channels,sizeX,sizeY,com.google.protobuf.ByteString.copyFrom(serialImage));	// setup status message
				}
			}
			catch ( InterruptedException e) {
				e.printStackTrace();
			}
			catch (IOException e) {
				e.printStackTrace();
			}
		} // end while loop
	}	// End tracksRun method
	
	
	
	public static void main(String... aArgs) throws IOException
	{
		if (aArgs.length > 0) {
			SendMsg msg = new SendMsg();
		}
		else {
			SendMsg mStatus = new SendMsg(0);  // Create object instance
			SendMsg mTracks = new SendMsg(1);
			SendMsg mImage = new SendMsg(2);
			
			Thread tStatus = new Thread(mStatus);
			Thread tTracks = new Thread(mTracks);
			Thread tImage = new Thread(mImage);

			tStatus.start();
			System.out.println("tStatus Started");
			
			tTracks.start();
			System.out.println("tTracks Started");

			tImage.start();
			System.out.println("tImage Started");			
		}
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
		//
//		for (int n=0; n<10; n++) {
//
//
//
//		
//		statusMsgReady = msg.setStatus(System.currentTimeMillis(),msg.sysStatus.OPERATIONAL,false,"TEST-STRING");	// setup status message
//		tracksMsgReady = msg.setTracks(System.currentTimeMillis(),msg.sysStatus.OPERATIONAL,targets,lasers,true);	// setup tracks message
////		imageMsgReady = msg.setImage(System.currentTimeMillis(),image.length,image[0].length,image[0][0].length,com.google.protobuf.ByteString.copyFrom(serialImage));	// setup status message
//		imageMsgReady = msg.setImage(System.currentTimeMillis(),channels,sizeX,sizeY,com.google.protobuf.ByteString.copyFrom(serialImage));	// setup status message
//
//		statusMsgReady = !(msg.sendStatus());
//		System.out.println("Send Status");
//				
//		tracksMsgReady = !(msg.sendTracks(netTracks));
//			System.out.println("Send Tracks");
//
//		imageMsgReady = !(msg.sendImage(netImage));
//			System.out.println("Send Image");

//		} // End For Loop		
		// Close Network interfaces
//		netImage.close();
//		netTracks.close();
//		netStat.close();
		

		//netImage.close();
	}	// End Main Function
	
	
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
	public boolean setTracks(long time,	CameraMsgs.systemStatus sysStat, int[][] targets, int[][] lasers, boolean laserOn)
	{
		tracksBuilder = CameraMsgs.cameraTracks.newBuilder();
		tracksBuilder	= tracksBuilder.setTime(time)
		.setStatus(sysStat);

		for (int i=0; i<targets.length; i++) {
			tracksBuilder = tracksBuilder.addTarget(CameraMsgs.track.newBuilder().setX(targets[i][0]).setY(targets[i][1]));
		}
		
		for (int i=0; i<lasers.length; i++) {
			tracksBuilder.addLaser(CameraMsgs.track.newBuilder().setX(lasers[i][0])
								   .setY(lasers[i][1]).build());
		}
		tracks = tracksBuilder.setLaserOn(laserOn)
		.build();
		return true;
	}	// end setTracks method
	
	public boolean setImage(long time, int channels, int sizeX, int sizeY, com.google.protobuf.ByteString serialImage)
	{
		imageBuilder = CameraMsgs.cameraImage.newBuilder();
		image = imageBuilder.setTime(time)
		.setChannels(channels)
		.setSizeX(sizeX)
		.setSizeY(sizeY)
		.setImageData(serialImage)
		.build();
		return true;
	}	// End setimage method
	
	public boolean sendStatus() throws IOException
	{
		if (status == null) {
			p("status message not sent");
			return true;
		}
		
		netStat.server(status);
		p("status out : " + System.currentTimeMillis());
		return true;
	}	// End sendStatus method
	
	public boolean sendTracks() throws IOException
	{
		if (tracks == null) {
			p("track message not sent");
			return true;
		}
		
		netTracks.server(tracks);
		p("Tracks Out : " + System.currentTimeMillis());
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
}	// End class SendMsg