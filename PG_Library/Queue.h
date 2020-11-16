#pragma once

template <typename T>
class Node;

template <class T>
class Queue
{
public:
	Queue()
		: m_Front(nullptr)
		, m_Back(nullptr)
		, m_Size(0)
	{
	}

	bool IsEmpty() { return m_Front == nullptr; }

	void PushBack(T value)
	{
		if (FirstInitialize(value))
		{
			return;
		}

		Node<T>* newNode = new Node<T>(value);
		m_Back->m_Next = newNode;
		m_Size++;
	}

	void PushFront(T value)
	{
		if (FirstInitialize(value))
		{
			return;
		}

		Node<T>* newNode = new Node<T>(value);
		newNode->m_Next = m_Front;
		m_Front = newNode;
		m_Size++;
	}

	bool Contains (T value) const
	{
		for (Node<T>* it = m_Front; it != nullptr; it = it->m_Next)
		{
			if (it->m_Value == value)
			{
				return true;
			}
		}

		return false;
	}

	int Size() const { return m_Size; }

	T& operator[] (int index)
	{
		static T ms_DefaultValue;
		if (index < 0 || index > m_Size)
		{
			// error: out of bounds
			fprintf(stderr, "Out of bounds");
			return ms_DefaultValue;
		}

		int count = 0;
		for (Node<T>* it = m_Front; it != nullptr; it = it->m_Next)
		{
			if (count == index)
			{
				return it->m_Value;
			}
			count++;
		}

		// error: out of bounds
		fprintf(stderr, "Out of bounds");
		return ms_DefaultValue;
	}

	const T& operator[] (int index) const
	{
		static T ms_DefaultValue;
		if (index < 0 || index > m_Size)
		{
			// error: out of bounds
			fprintf(stderr, "Out of bounds");
			return ms_DefaultValue;
		}

		int count = 0;
		for (Node<T>* it = m_Front; it != nullptr; it = it->m_Next)
		{
			if (count == index)
			{
				return it->m_Value;
			}
			count++;
		}

		// error: out of bounds
		fprintf(stderr, "Out of bounds");
		return ms_DefaultValue;
	}

private:
	bool FirstInitialize(T value)
	{
		if (!IsEmpty())
		{
			return false;
		}

		m_Front = m_Back = new Node<T>();
		m_Front->m_Value = value;
		m_Size++;

		return true;
	}


private:
	Node<T>* m_Front;
	Node<T>* m_Back;
	int m_Size;
};

template <typename T>
class Node
{
public:
	Node()
		: m_Value()
		, m_Next(nullptr)
	{
	}

	Node(T value)
		: m_Value(value)
		, m_Next(nullptr)
	{
	}

public:
	T m_Value;
	Node<T>* m_Next;
};

