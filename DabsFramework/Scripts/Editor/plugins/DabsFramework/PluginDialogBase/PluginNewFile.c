[WorkbenchPluginAttribute("New File", "Creates New File", "Ctrl+N", "", {"ScriptEditor"})]
class PluginNewFile: PluginDialogBase
{	
	[Attribute("", "editbox", "Directory (folders will be created automatically)")]
	string Folder;
	
	[Attribute("", "editbox", "File Name (with or without extension)")]
	string FileName;
	
	//[Attribute("Managed", "editbox", "Parent class type")]
	//string Parent;
		
	protected string m_FinalFileName;
	
	void PluginNewFile()
	{		
		//Parent = "Managed";
		FileName = string.Empty;
	}
	
	void ~PluginNewFile()
	{
		
	}
	
	override void Run()
	{
		super.Run();
		
		string current_file_relative;
		if (!m_ScriptEditor.GetCurrentFile(current_file_relative)) {
			Error("Failed to acquire current file");
			return;
		}
				
		Folder = Directory.GetDirectory(current_file_relative);				
		FileName = string.Empty;
				
		Workbench.ScriptDialog("New File", DIALOG_TAB_SIZE, this);
	}
	
	[ButtonAttribute("OK", true)]
	void Ok()
	{		
		if (FileName.Length() == 0) {
			return;
		}
		
		// Appending the extension
		array<string> file_split = {};
		FileName.Split(".", file_split);
		if (file_split.Count() < 2) {
			FileName = file_split[0] + DEFAULT_EXTENSION;
		}
		
		string absolute_file = GetAbsolutePath(Folder) + SystemPath.SEPERATOR + FileName;
		if (FileExist(absolute_file)) {
			ErrorDialog(string.Format("File %1 already exists!", FileName));
			return;
		}
		
		FileHandle file_handle = CreateFile(absolute_file);
		if (!file_handle) {
			return;
		}
		
		CloseFile(file_handle);
		
		m_FinalFileName = Folder + SystemPath.SEPERATOR + FileName;
		if (m_FinalFileName != string.Empty) {
			m_ScriptEditor.SetOpenedResource(m_FinalFileName);
		}
	}

	[ButtonAttribute("Cancel")]
	void Cancel()
	{
	}
}