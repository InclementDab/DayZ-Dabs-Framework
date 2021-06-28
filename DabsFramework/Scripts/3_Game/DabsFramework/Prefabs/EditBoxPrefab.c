class EditBoxPrefab: SimplePrefabBase<string>
{	
	EditBoxWidget ContentText;
		
	override string GetLayoutFile() 
	{
		return "DabsFramework/gui/Layouts/prefabs/EditBoxPrefab.layout";
	}
}