class DialogCategoryBaseController: DialogBaseController
{
	ref ObservableCollection<ref DialogCategoryListItem> DialogCategoryData = new ObservableCollection<ref DialogCategoryListItem>(this);		
		
	void ~DialogCategoryBaseController()
	{
		delete DialogCategoryData;
	}
}