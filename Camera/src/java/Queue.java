// Jonathan Ford
// 4-4-2010
// Homework for CPTR152 Bill Wolfer

// addapted from example at http://www.cs.princeton.edu/introcs/43stack/

// ---------------------------------------
// This program is to:
// 1) Create a Queue
// 2) Check if it is empty
// 3) if empty create a queue of 10 random numbers.
// 4) if not empty, display contents
// 5) Add a value on the list
// 6) Display contents of list
// 7) Remove a value off the list
// 8) Display contents of list

import java.util.*;
import java.util.Iterator;
import java.util.Random;

// class for Queue of type Item
public class Queue<Item> implements Iterable<Item>
{

	private int N;
	private Element first;		// pointer to first (oldest) item
	private Element last;		// pointer to last (most recent) item

	private class Element
	{
		private Item value;
		private	Element next;
	}
	
	// Create Empty Queue
	public Queue()
	{
		first = null;
		last = null;
		N = 0;
	}
	
	public boolean isEmpty() {return first == null; }
	public int size() { return N; }
	
	public Item peek()
	{
		return first.value;
	}
	
	// add an element to the Queue
	public void enQueue(Item value)
	{
		Element x = new Element();
		x.value = value;
		x.next = null;
		if(isEmpty())
		{
			first = x;
			last = x;
		}
		else {
			last.next = x;
			last = x;
		}
		N++;
	}
	
	// remove an element from the Queue
	public Item deQueue()
	{
		if(isEmpty())
		{
			System.out.println("Queue is Empty");
			System.exit(0);
		}
		Item value = first.value;
		first = first.next;
		N--;
		return value;
	}
	
	// Iterator for walking through list	
	public Iterator<Item> iterator()
	{
		return new QueueIterator();
	}
	
	// Iterator for walking through list
	private class QueueIterator implements Iterator<Item>
	{
		private Element current = first;
		public boolean hasNext()
		{
			return current != null;
		}
		
		public void remove()
		{
			System.out.println("Remove is not supported here");
		}
		
		public Item next()
		{
			if(!hasNext())
			{
				System.out.println("There are no more elements in list");
			}
			Item value = current.value;
			current = current.next;
			return value;
		}
	}
	
	// Main program to do specifics of assignment
	public static void main(String[] args)
	{
		Queue<Integer> q1 = new Queue<Integer>();	// create queue
		Iterator iterator;
		System.out.println("Check if queue is empty...");
		if(q1.isEmpty())
		{
			System.out.println("Queue is empty, so add values to it");
			Random generator = new Random();
			for (int i = 0; i< 10; i++) {
				q1.enQueue(generator.nextInt( 1000 ));		// random numbers into queue
			}
		}	// End if
		else {
			System.out.println("Queue was not empty.  It had the following:");
			iterator = q1.iterator();
			while (iterator.hasNext()) 
			{
				System.out.print(iterator.next()+" ");
			}
		}
		System.out.println("\nAdd 1 to queue");
		q1.enQueue(1);
		System.out.println("\nAdd 2 to queue");
		q1.enQueue(2);
		iterator = q1.iterator();
		
		System.out.println("\nPrint out contents of queue");
		while (iterator.hasNext()) 
		{
			System.out.print(iterator.next()+" ");
		}
		System.out.println("\nTake value '" + q1.deQueue() + "' from queue \n\nQueue has:");
		
		
		while(!q1.isEmpty())
		{
			System.out.println(q1.deQueue());
		}
	}	// end main
		
}	// End class Queue