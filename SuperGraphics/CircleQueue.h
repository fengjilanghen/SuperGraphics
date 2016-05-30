#ifndef _CIRCLE_QUEUE_H_
#define _CIRCLE_QUEUE_H_

template<typename T, int NUM>
class CCircleQueue
{
public:
	typedef T value_type;
	CCircleQueue() :m_Front(0), m_Rear(0), m_Size(0){}
	T* front() { return isEmpty() ? nullptr : &m_Data[m_Front]; }
	T* front() const { return isEmpty() ? nullptr : &m_Data[m_Front]; }

	T* second() { return m_Size < 2 ? nullptr : &m_Data[(m_Front + 1) % NUM]; }
	T* second() const { return m_Size < 2 ? nullptr : &m_Data[(m_Front + 1) % NUM]; }

	void pop()
	{
		if (m_Size > 0)
		{
			m_Front = (m_Front + 1) % NUM;
			--m_Size;
		}
	}

	bool push(const T &v)
	{
		if (!isFull())
		{
			m_Data[m_Rear] = v;
			m_Rear = (m_Rear + 1) % NUM;
			++m_Size;
			return true;
		}
		return false;
	}
	bool isFull() const { return m_Size >= NUM - 1; }
	bool isEmpty() const { return m_Size == 0; }
	void clear() { m_Front = m_Rear = m_Size = 0; }
	int size() const { return m_Size; }

	CCircleQueue(const CCircleQueue&) = delete;
	CCircleQueue& operator=(const CCircleQueue&) = delete;
private:
	int			m_Size;
	int			m_Front;
	int			m_Rear;
	T			m_Data[NUM];
};

#endif //_CIRCLE_QUEUE_H_