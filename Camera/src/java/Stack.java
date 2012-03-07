// Jonathan Ford
// 4-4-2010
// Homework for CPTR152 Bill Wolfer

// addapted from example at http://www.cs.princeton.edu/introcs/43stack/

// ---------------------------------------
// This program is to:
// - Create a Stack
// - Check if it is empty
// - if empty create a stack of 10 random numbers.
// - if not empty, display contents
// - Push a value on the list
// - Display contents of list
// - Pop a value off the list
// - Display contents of list

import java.util.*;
import java.util.Iterator;
import java.util.Random;

// class for stack of type Item
public class Stack<Item> implements Iterable<Item>
{

	private int N;				// Size of list
	private Element first;

	private class Element
	{
		private Item value;		// value of element
		private	Element next;	// next element in list or null
	}
	
	// Create Empty Stack
	public Stack()
	{
		first = null;
		N = 0;
	}
	
	public boolean isEmpty() 
	{
		return first == null; 
	}
	
	// Get the size of list
	public int size() 
	{
		return N;
	}
	
	// first item of stack without removing.
	public Item peek()
	{
		return first.value;
	}
	
	// put a value on the stack
	public void push(Item value)
	{
		Element oldFirst = first;
		first = new Element();
		first.value = value;
		first.next = oldFirst;
		N++;
	}
	
	// Take value off of stack and return
	public Item pop()
	{
		if(isEmpty())
		{
			System.out.println("Stack is Empty");
			System.exit(0);
		}
		Item value = first.value;
		first = first.next;
		N--;
		return value;
	}
	
	// return an iterator to walk the list
	public Iterator<Item> iterator()
	{
		return new StackIterator();
	}
	
	// implements iterator for moving through a linked list
	private class StackIterator implements Iterator<Item>
	{
		private Element current = first;
		public boolean hasNext()
		{
			return current != null;
		}
		
		// don't use this but need for implemenetation
		public void remove()
		{
			System.out.println("Remove is not supported here");
		}
		
		// sets next item.
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
	
	// main program to do specifics of assignment
	public static void main(String[] args)
	{
		Stack<Integer> s1 = new Stack<Integer>();		// create stack of type Integer
		Iterator iterator;								// create iterator not defined yet
		System.out.println("Check if stack is empty...");
		if(s1.isEmpty())
		{
			System.out.println("Stack is empty, so add values to it");
			Random generator = new Random();
			for (int i = 0; i< 10; i++) {
				s1.push(generator.nextInt( 1000 ));		// insert some random numbers
			}
		}	// End if
		else {
			System.out.println("Stack was not empty.  It had the following:");
			iterator = s1.iterator();
			
			//Print out list
			while (iterator.hasNext()) 
			{
				System.out.print(iterator.next()+" ");
			}
		}
		
		//Push some values on stack
		System.out.println("\nPush 1 onto stack");
		s1.push(1);
		System.out.println("\nPush 2 onto stack");
		s1.push(2);
		iterator = s1.iterator();			// define iterator
		
		System.out.println("\nPrint out contents of stack");
		while (iterator.hasNext()) 
		{
			System.out.print(iterator.next()+" ");
		}
		System.out.println("\nPop value '" + s1.pop() + "' from stack \n\nStack has:");
		
		
		while(!s1.isEmpty())
		{
			System.out.println(s1.pop());		// pop values and print
		}
	}	// end main
		
}	// End class Stack