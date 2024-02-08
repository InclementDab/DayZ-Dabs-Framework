[WorkbenchPluginAttribute("New File", "Creates New File", "Ctrl+N", "", {"ScriptEditor"})]
class PluginNewFile: PluginProject
{	
	[Attribute("", "editbox", "Directory (folders will be created automatically)")]
	string Directory;
	
	[Attribute("", "editbox", "File Name (with or without extension)")]
	string File;
	
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
			Directory += current_file_path[i];
			if (i != current_file_path.Count() - 2) {
				Directory += "/";
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
		if (File.Length() == 0) {
			return;
		}
		
		// Appending the extension
		array<string> file_split = {};
		File.Split(".", file_split);
		if (file_split.Count() < 2) {
			File = file_split[0] + DEFAULT_EXTENSION;
		}
		
		string absolute_file = GetAbsolutePath(Directory) + PATH_SEPERATOR + File;
		if (FileExist(absolute_file)) {
			ErrorDialog(string.Format("File %1 already exists!", File));
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
		
		FPrint(file_handle, string.Format("class %1%2\n{\n}", file_split[0], suffix));
		CloseFile(file_handle);
		
		m_FinalFileName = Directory + PATH_SEPERATOR + File;
	}

	[ButtonAttribute("Cancel")]
	void Cancel()
	{
	}
}