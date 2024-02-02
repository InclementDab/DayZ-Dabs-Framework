
/*

 Base class for Observable Types
 The design behind them is you wont need to use NotifyPropertyChanged() upon setting a variable
 They are current used to support Collections (array, set, map)

Example: 


class TestController: ViewController
{
	// "LeftList" goes into Binding_Name
	ref ObservableCollection<string> LeftList; 	
	
	// "LeftListSelection" goes into Selected_Item
	string LeftListSelection; 		
	
	void TestController()
	{
		LeftList = new ObservableCollection<string>(this);
		LeftList.Insert("Line1");
	}

	override void CollectionChanged(string property_name, CollectionChangedEventArgs args)
	{
		switch (property_name)
		{
			case "LeftListSelection": {
				Print("LeftList selection changed: " + LeftListSelection);
				break;
			}
		}
	}
}

*/

class ObservableCollection<Class TValue> : Observable
{
	protected ref array<autoptr TValue> m_Data = {};

	void ObservableCollection(ViewController controller)
	{
		m_Type = TemplateType<TValue>.GetType();
	}

	void ~ObservableCollection()
	{
		delete m_Data;
	}

	int Insert(TValue value)
	{
		int index = m_Data.Insert(value);
		if (index != -1) {
			CollectionChanged(new CollectionChangedEventArgs(this, NotifyCollectionChangedAction.Insert, index, new Param1<TValue>(value)));
		}

		return index;
	}

	int InsertAt(TValue value, int index)
	{
		int new_index = m_Data.InsertAt(value, index);
		CollectionChanged(new CollectionChangedEventArgs(this, NotifyCollectionChangedAction.InsertAt, index, new Param1<TValue>(value)));
		return new_index;
	}
	
	void InsertAll(notnull array<TValue> from)
	{
		for (int i = 0; i < from.Count(); i++) {
			Insert(from[i]);
		}
	}
	
	// 0: TValue value
	// 1: int index
	/*int InsertAtEx(TypeConverter typeConverter, int index)
	{
		TValue value; // scoofed
		g_Script.CallFunction(typeConverter, "GetData", value, null);
		Print(value);
		int new_index = m_Data.InsertAt(value, index);
		CollectionChanged(new CollectionChangedEventArgs(this, NotifyCollectionChangedAction.InsertAt, index, new Param1<TValue>(value)));
		return new_index;
	}*/

	void Remove(int index)
	{
		CollectionChanged(new CollectionChangedEventArgs(this, NotifyCollectionChangedAction.Remove, index, new Param1<TValue>(m_Data.Get(index))));
		m_Data.Remove(index);
	}
	
	void RemoveOrdered(int index)
	{	// not sure if anything other than the Editor Log Entries removal needs this. but it works
		CollectionChanged(new CollectionChangedEventArgs(this, NotifyCollectionChangedAction.Remove, index, new Param1<TValue>(m_Data.Get(index))));
		m_Data.RemoveOrdered(index);
	}

	void RemoveRange(int start, int end)
	{
		for (int i = start; i < end; i++)
		{
			CollectionChanged(new CollectionChangedEventArgs(this, NotifyCollectionChangedAction.Remove, start, new Param1<TValue>(m_Data.Get(start))));
			m_Data.RemoveOrdered(start);
		}
	}

	void Remove(TValue value)
	{
		int index = m_Data.Find(value);
		if (index != -1) {
			Remove(index);
		}
	}

	void Set(int index, TValue value)
	{
		CollectionChanged(new CollectionChangedEventArgs(this, NotifyCollectionChangedAction.Replace, index, new Param1<TValue>(value)));
		m_Data.Set(index, value);
	}

	int MoveIndex(int index, int moveIndex)
	{
		TValue value = m_Data.Get(index);
		int new_index = m_Data.MoveIndex(index, moveIndex);
		CollectionChanged(new CollectionChangedEventArgs(this, NotifyCollectionChangedAction.Move, new_index, new Param1<TValue>(value)));
		return new_index;
	}

	void SwapItems(int itemA, int itemB)
	{
		m_Data.SwapItems(itemA, itemB);
		CollectionChanged(new CollectionChangedEventArgs(this, NotifyCollectionChangedAction.Swap, -1, new CollectionSwapArgs(itemA, itemB)));
	}

	void Clear()
	{
		m_Data.Clear();
		CollectionChanged(new CollectionChangedEventArgs(this, NotifyCollectionChangedAction.Clear, -1, null));
	}

	TValue Get(int index)
	{
		return m_Data.Get(index);
	}

	override int Count()
	{
		return m_Data.Count();
	}
	
	array<ref TValue> GetArray()
	{
		return m_Data;
	}

	int Find(TValue value)
	{
		return m_Data.Find(value);
	}
}