class OptionsCategoryViewController: ViewController
{
	string HeaderName;
	ref ObservableCollection<ref OptionView> Options = new ObservableCollection<ref OptionView>(this);
	
	void ~OptionsCategoryViewController()
	{
		delete Options;
	}
}