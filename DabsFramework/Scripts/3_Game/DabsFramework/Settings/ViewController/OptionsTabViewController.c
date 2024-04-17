class OptionsTabViewController: ViewController
{
	ref ObservableCollection<ref OptionsCategoryView> OptionCategories = new ObservableCollection<ref OptionsCategoryView>(this);
	
	void ~OptionsTabViewController()
	{
		delete OptionCategories;
	}
}