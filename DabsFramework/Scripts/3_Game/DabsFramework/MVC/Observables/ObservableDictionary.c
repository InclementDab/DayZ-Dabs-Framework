/*
	Note: Many actions on this use array.Find, since the indexing of all widget collections requires an integer index
*/
class ObservableDictionary<Class TKey, Class TValue>: Observable
{
	protected ref map<TKey, ref TValue> m_Data = new map<TKey, ref TValue>();

	void ObservableDictionary(ViewController controller)
	{
		m_Type = TemplateType<TValue>.GetType();
	}

	void ~ObservableDictionary()
	{
		delete m_Data;
	}

	bool Insert(TKey key, TValue value)
	{		
		if (!m_Data.Insert(key, value)) {
			return false;
		}
		
		CollectionChanged(new CollectionChangedEventArgs(this, NotifyCollectionChangedAction.Insert, m_Data.GetValueArray().Find(value), new Param1<TValue>(value)));		
		return true;
	}

	void Remove(TKey key)
	{
		if (m_Data.Contains(key)) {
			CollectionChanged(new CollectionChangedEventArgs(this, NotifyCollectionChangedAction.Remove, m_Data.GetKeyArray().Find(key), new Param1<TValue>(m_Data[key])));
			m_Data.Remove(key);
		}
	}

	void Remove(int index)
	{
		CollectionChanged(new CollectionChangedEventArgs(this, NotifyCollectionChangedAction.Remove, index, new Param1<TValue>(m_Data.GetElement(index))));
		m_Data.RemoveElement(index);
	}

	void RemoveRange(int start, int end)
	{
		for (int i = start; i < end; i++) {
			CollectionChanged(new CollectionChangedEventArgs(this, NotifyCollectionChangedAction.Remove, i, new Param1<TValue>(m_Data.GetElement(i))));
			m_Data.RemoveElement(i);
		}
	}

	void Clear()
	{
		m_Data.Clear();
		CollectionChanged(new CollectionChangedEventArgs(this, NotifyCollectionChangedAction.Clear, -1, null));
	}

	void Set(TKey key, TValue value)
	{
		m_Data[key] = value;
		
		NotifyCollectionChangedAction action = NotifyCollectionChangedAction.Replace;
		if (!m_Data.Contains(key)) {
			action = NotifyCollectionChangedAction.InsertAt;
		}
		
		CollectionChanged(new CollectionChangedEventArgs(this, action, m_Data.GetValueArray().Find(value), new Param1<TValue>(value)));
	}

	TValue Get(TKey key)
	{
		return m_Data.Get(key);
	}

	TKey GetKey(int index)
	{
		return m_Data.GetKey(index);
	}
	
	TValue GetElement(int index)
	{
		return m_Data.GetElement(index);
	}

	bool Contains(TKey key)
	{
		return m_Data.Contains(key);
	}

	override int Count()
	{
		return m_Data.Count();
	}
}