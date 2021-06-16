
// Base class for all Observable Types (ObservableCollection, ObservableSet, ObservableDictionary)
class Observable
{
	// Assigned on Constructor of super
	private	typename m_Type;

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

	TypeConverter GetTypeConverter()
	{
		return LayoutBindingManager.GetTypeConversion(m_Type);
	}

	// Abstract
	int Count();
};