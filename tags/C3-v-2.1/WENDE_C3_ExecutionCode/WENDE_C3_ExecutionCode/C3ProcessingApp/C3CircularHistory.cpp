#include "stdafx.h"
#include "C3CircularHistory.h"

C3CircularHistory::C3CircularHistory(void)
	: m_curIndex(0),
	m_prevIndex(0),
	m_size(20),
	m_isFull(false)
{
	m_history.reserve(m_size);
}

C3CircularHistory::~C3CircularHistory(void)
{
}

// add a point to the circular buffer
void C3CircularHistory::addPosition(const C3_TRACK_POINT_DOUBLE cameraRoverPosition)
{
	// add the point to the array
	if (m_isFull == false)
	{
		m_history.push_back(cameraRoverPosition);
	}
	else
	{
		m_history[m_curIndex] = cameraRoverPosition;
	}
	// update the index information
	m_prevIndex = m_curIndex;
	// determine if we are in a wrap around case
	m_curIndex++;
	if (m_curIndex >= m_size)
	{
		m_curIndex = 0;
		m_isFull   = true; 
	}
}

// returns the number of points in the array
unsigned int C3CircularHistory::getNumHistoryPoints() const	
{
	return (m_isFull == true) ? m_history.size() : m_size;
}
// returns the last point added to the history
C3_TRACK_POINT_DOUBLE C3CircularHistory::getLastHistoryPoint() const
{
	return m_history[m_prevIndex];
}
// returns the point at index ii from the history
C3_TRACK_POINT_DOUBLE C3CircularHistory::getPoint(const unsigned int index ) const
{
	C3_TRACK_POINT_DOUBLE point;
	
	if (m_isFull == true)
	{
		point = m_history[index];
	}
	else
	{
		if (m_prevIndex >= index)
		{
			point = m_history[index];
		}
	}

	return point;
}
// the overide operator for []
C3_TRACK_POINT_DOUBLE C3CircularHistory::operator [] (const unsigned int index)
{
	return this->getPoint(index);
}