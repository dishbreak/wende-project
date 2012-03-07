// Jonathan Ford
// 27 February 2012


//import ReadWriteTextFileWithEncoding;
import java.io.*;
import com.camera.*;
//import Queue;

// class for Queue of type Item
public class SendMsg
{
	public static void main(String... aArgs) throws IOException
	{
//		String fileName = aArgs[0];
//		String encoding = aArgs[1];
//		String message =  aArgs[2];
//		ReadWriteTextFileWithEncoding rwText = new ReadWriteTextFileWithEncoding( fileName, encoding);
//		rwText.write(message);
		Queue<Integer> myQueue = new Queue<Integer>();
		Queue<CameraMsgs.cameraStatus.Builder> statusQueue = new Queue<CameraMsgs.cameraStatus.Builder>();
		myQueue.enQueue(5);
		boolean empty = myQueue.isEmpty();
		System.out.println(empty);
		System.out.println(myQueue.deQueue());
		System.out.println(myQueue.isEmpty());
	}
	
	SendMsg(String aFileName, String aEncoding)
	{
		String fileName = aFileName;
		String encoding = aEncoding;
	}

}	// End class SendMsg