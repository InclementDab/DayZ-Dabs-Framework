[WorkbenchPluginAttribute("Build Mod", "Build the whole damn mod... all of it!", "F8", "", {"ScriptEditor", "ResourceManager"})]
class PluginBuildMod: PluginProject
{	
	void PluginBuildMod()
	{
		if (m_LaunchSettings.Repository == string.Empty) {
			ErrorDialog("You need to set the Repository setting in Plugins -> Configure -> Configure Project");
			return;
		}		
		
		if (m_LaunchSettings.Profiles == string.Empty) {
			ErrorDialog("You need to set the Profiles setting in Plugins -> Configure -> Configure Project");
			return;
		}		
		
		if (m_LaunchSettings.Missions == string.Empty) {
			ErrorDialog("You need to set the Missions setting in Plugins -> Configure -> Configure Project");
			return;
		}		
		
		if (m_LaunchSettings.Mods == string.Empty) {
			ErrorDialog("You need to set the Mods setting in Plugins -> Configure -> Configure Project");
			return;
		}
		
		string mod_prefix = GetPrefix();
		// Set up our sym link for workshop modding :drool:
		string game_directory_stable = GetSourceDataDirectory();
		if (!FileExist(string.Format("%1\\!Workshop", game_directory_stable))) {
			Print("Couldnt find workshop folder in current directory, probably experimental.");
			game_directory_stable = GetDirectory(game_directory_stable) + "\\DayZ";
			if (!FileExist(string.Format("%1\\!Workshop", game_directory_stable))) {
				Print("Womp womp");
				//ErrorDialog("Could not find DayZ Workshop folder. Is your Source Data correctly set?");
				//return;
			}
		}
		
		array<string> dependencies = EnumerateDirectories(string.Format("%1\\Dependencies", m_LaunchSettings.Repository));
		foreach (string dependency: dependencies) {
			PromiseSymLink(string.Format("%1\\Dependencies\\%2", m_LaunchSettings.Repository, dependency), GetAbsolutePath(string.Format("$Workdrive:%1", dependency)));
		}
		
		PromiseSymLink(string.Format("%1\\!Workshop", game_directory_stable), m_LaunchSettings.Mods);
		
		if (m_BuildSettings.Dependencies) {
			array<string> mod_splits = {};
			m_ProjectSettings["Mods"].Split(";", mod_splits);
			foreach (string mod_split: mod_splits) {
				string mod_split_edit = mod_split;
				mod_split_edit.Replace("@", "");
				
				string mod_folder = GetAbsolutePath(string.Format("$Workdrive:%1", mod_split_edit));
				if (FileExist(mod_folder)) {
					if (BuildFolder(mod_folder, string.Format("%1\\@%2", m_LaunchSettings.Mods, mod_split_edit))) {
						Error(string.Format("Build failed: %1", mod_split));
					}
				}
			}
		}
		
		// Set up our mod output correctly if not done so already
		string mod_output = string.Format("%1\\@%2", m_LaunchSettings.Mods, mod_prefix);		
		string main_mod_folder = GetAbsolutePath(string.Format("$Workdrive:%1", mod_prefix));
		if (BuildFolder(main_mod_folder, mod_output)) {
			Error(string.Format("Build failed: %1", main_mod_folder));
		}
		
		// Move contents of Addons folder
		if (m_BuildSettings.CopyAddons) {
			CopyFiles(string.Format("%1\\Addons", m_LaunchSettings.Repository), mod_output + PATH_SEPERATOR_ALT + "Addons");
		}
	}
	
	int BuildFolder(string mod_input, string mod_output) {
		int ret = -1;
		
		switch (m_BuildSettings.Builder)
		{
		case BuilderType.PBO_PROJECT:
			ret = BuiildUsingPboProject(mod_input, mod_output);
			break;
		case BuilderType.ADDON_BUILDER:
			ret = BuildUsingAddonBuilder(mod_input, mod_output);
			break;
		}
		
		return ret;
	}
	
	int BuiildUsingPboProject(string mod_input, string mod_output)
	{
		string args = m_BuildSettings.PboProject_Args;
		if (m_BuildSettings.Key != string.Empty) {
			args += string.Format(" +K=%1",  m_BuildSettings.Key);
		}
		
		PrintFormat("Building mod %1 to %2 with args %3", mod_input, mod_output, args);
		MakeDirectory(mod_output);
		MakeDirectory(mod_output + PATH_SEPERATOR_ALT + "Addons");
		MakeDirectory(mod_output + PATH_SEPERATOR_ALT + "Keys");
		string cmd = m_BuildSettings.PboProject_Command;
		cmd.Replace(PATH_SEPERATOR_ALT, PATH_SEPERATOR);
		mod_output.Replace(PATH_SEPERATOR_ALT, PATH_SEPERATOR);
		mod_input.Replace(PATH_SEPERATOR_ALT, PATH_SEPERATOR);

		return Workbench.RunCmd(string.Format("\"%1\" -Mod=%2 %3 %4", cmd, mod_output, mod_input, args), true);
	}
	
	int BuildUsingAddonBuilder(string mod_input, string mod_output)
	{
		string args = m_BuildSettings.AddonBuilder_Args;
		if (m_BuildSettings.Key != string.Empty) {
		}
		
		PrintFormat("Building mod %1 to %2 with args %3", mod_input, mod_output, args);
		MakeDirectory(mod_output);
		MakeDirectory(mod_output + PATH_SEPERATOR_ALT + "Addons");
		MakeDirectory(mod_output + PATH_SEPERATOR_ALT + "Keys");
		
		mod_output = mod_output + PATH_SEPERATOR_ALT + "Addons";
		
		string cmd = m_BuildSettings.AddonBuilder_Command;
		cmd.Replace(PATH_SEPERATOR_ALT, PATH_SEPERATOR);
		mod_output.Replace(PATH_SEPERATOR_ALT, PATH_SEPERATOR);
		mod_input.Replace(PATH_SEPERATOR_ALT, PATH_SEPERATOR);
		string exclude = string.Format("-exclude=%1exclude.lst", GetCurrentDirectory());
		exclude.Replace(PATH_SEPERATOR_ALT, PATH_SEPERATOR);
		return Workbench.RunCmd(string.Format("\"%1\" %2 %3 %4 \"%5\"", cmd, mod_input, mod_output, args, exclude), true);
	}
}