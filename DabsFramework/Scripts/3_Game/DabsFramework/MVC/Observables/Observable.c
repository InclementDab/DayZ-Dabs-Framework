// Base class for all Observable Types (ObservableCollection, ObservableSet, ObservableDictionary)
class Observable: Managed
{
	// Assigned on Constructor of super
	protected typename m_Type;

	protected ViewController m_Controller;

	void Observable(ViewController controller)
	{
		m_Controller = controller;
	}

	protected void CollectionChanged(CollectionChangedEventArgs args)
	{
		m_Controller.NotifyCollectionChanged(args);
	}

	typename GetType()
	{
		return m_Type;
	}
	
	ViewController GetController()
	{
		return m_Controller;
	}

	TypeConverter GetTypeConverter()
	{
		return GetDayZGame().GetTypeConversion(m_Type);
	}

	// Abstract
	int Count();
}