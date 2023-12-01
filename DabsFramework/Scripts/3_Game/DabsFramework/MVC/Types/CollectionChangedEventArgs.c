class CollectionChangedEventArgs: Managed
{
	// Event Args for Collection ChangedIndex
	// 0: Collection that was changed
	Observable Source;
	
	// 1: Action of change
	NotifyCollectionChangedAction ChangedAction;	
	
	// 2: Index of changed collection (used in lists of widgets)
	int ChangedIndex;
	
	// 3: Value of change
	Param ChangedValue;
	
	void CollectionChangedEventArgs(Observable source, NotifyCollectionChangedAction changed_action, int changed_index, Param changed_value)
	{
		Source = source;
		ChangedAction = changed_action;
		ChangedIndex = changed_index;
		ChangedValue = changed_value;
	}
}