class MessageBoxPrefab: ScriptView
{
	protected RichTextWidget CaptionText;
	
	void MessageBoxPrefab(string content)
	{
		CaptionText.SetText(content);
	}
	
	override string GetLayoutFile() 
	{
		return "DabsFramework/gui/Layouts/prefabs/MessageBoxPrefab.layout";
	}
}
