[WorkbenchPluginAttribute("New File", "Creates New File", "Ctrl+N", "", {"ScriptEditor"})]
class PluginNewFile: PluginDialogBase
{	
	[Attribute("", "editbox", "Directory (folders will be created automatically)")]
	string Folder;
	
	[Attribute("", "editbox", "File Name (with or without extension)")]
	string FileName;
	
	[Attribute("Managed", "editbox", "Parent class type")]
	string Parent = "Managed";
		
	protected string m_FinalFileName;
	
	void PluginNewFile()
	{		
		string current_file_relative;
		if (!m_ScriptEditor.GetCurrentFile(current_file_relative)) {
			Error("Failed to acquire current file");
			return;
		}
		
		// Reconstructing the directory
		array<string> current_file_path = {};
		current_file_relative.Split("/", current_file_path);
		
		for (int i = 0; i < current_file_path.Count() - 1; i++) {
			Folder += current_file_path[i];
			if (i != current_file_path.Count() - 2) {
				Folder += "/";
			}
		}
		
		string parent_folder = current_file_path[current_file_path.Count() - 2];
		
		if (parent_folder.ToType()) {
			Parent = parent_folder;
		}
		
		Workbench.ScriptDialog("New File", DIALOG_TAB_SIZE, this);
	}
	
	void ~PluginNewFile()
	{
		if (m_FinalFileName != string.Empty) {
			m_ScriptEditor.SetOpenedResource(m_FinalFileName);
		}
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
		
		string suffix;
		if (Parent != string.Empty) {
			suffix += ": " + Parent;
		}
		
		string template = string.Format("class %1%2\n{\n}", file_split[0], suffix);
		if (file_split[0].ToType()) {
			template = string.Format("modded class %1\n{\n}", file_split[0]);
		}
				
		FPrint(file_handle, template);
		CloseFile(file_handle);
		
		m_FinalFileName = Folder + SystemPath.SEPERATOR + FileName;
	}

	[ButtonAttribute("Cancel")]
	void Cancel()
	{
	}
}