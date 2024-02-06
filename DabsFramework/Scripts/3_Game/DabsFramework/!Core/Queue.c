class Queue<Class T>: Managed
{
	static const T EMPTY;
	
	protected ref array<ref T> m_Values = {};
		
	T Pop()
	{
		int index = Count() - 1;
		if (index == -1) {
			return EMPTY;
		}
		
		T value = m_Values[index];
		m_Values.Remove(index);
		return value;
	}
	
	T Peek()
	{
		int index = Count() - 1;
		if (index == -1) {
			return EMPTY;
		}
		
		return m_Values[index];
	}
	
	void Push(T value)
	{
		m_Values[this.Count()] = value;
	}
	
	void Set(int n, T value)
	{
		m_Values[n] = value;
	}
	
	T Get(int n)
	{
		return m_Values[n];
	}
	
	array<T> ToArray()
	{
		array<T> values = {};
		values.Copy(m_Values);
		return values;
	}
	
	int Count()
	{
		return m_Values.Count();
	}
	
	void Clear()
	{
		m_Values.Clear();
	}
	
	typename GetType()
	{
		return T;
	}
}