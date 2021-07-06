class ListBoxPrefabController<Class T>: ViewController
{
	ref ObservableCollection<ref ListBoxPrefabEntry<T>> ListBoxData = new ObservableCollection<ref ListBoxPrefabEntry<T>>(this);
}