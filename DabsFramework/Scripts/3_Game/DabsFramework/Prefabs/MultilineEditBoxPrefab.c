class MultilineEditBoxPrefab: PrefabBase<string>
{	
	MultilineEditBoxWidget ContentText;
		
	override string GetLayoutFile() 
	{
		return "DabsFramework/gui/Layouts/prefabs/MultilineEditBoxPrefab.layout";
	}
}