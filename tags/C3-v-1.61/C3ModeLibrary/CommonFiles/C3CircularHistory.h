#pragma once

#include <vector>
#include "C3Point.h"

using std::vector;

class C3CircularHistory
{
	private:
		// the current starting point
		unsigned int m_curIndex;
		// the previous starting point
		unsigned int m_prevIndex;
		// the size of the circular buffer
		unsigned int m_size;
		// the history buffer
		vector<C3Point> m_history;
		// Flag to tell that the buffer is full and not repeating indexs
		bool m_isFull;

	public:
		// default constructor
		C3CircularHistory(void);
		// default destructor
		~C3CircularHistory(void);
		// assignment operator
		// TODO ... ADD THIS
		// copy constructor
		// TODO ... ADD THIS

	public:
		// the overide operator for []
		C3Point operator [] (const unsigned int index);

	public:
		// add a point to the circular buffer
		void addPosition(const C3Point cameraRoverPosition);

	public:
		// returns the number of points in the array
		unsigned int getNumHistoryPoints() const;
		// returns the last point added to the history
		C3Point getLastHistoryPoint() const;
		// returns the point at index ii from the history
		C3Point getPoint(const unsigned int index ) const;
		//
};

