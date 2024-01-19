class OptionViewController: ViewController
{
	string Label;
	ref ObservableCollection<ref OptionSelectorViewBase> Option = new ObservableCollection<ref OptionSelectorViewBase>(this);
	
	void ~OptionViewController()
	{
		delete Option;
	}
}