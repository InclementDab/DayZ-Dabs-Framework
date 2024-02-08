[WorkbenchPluginAttribute("Build Mod", "Build the whole damn mod... all of it!", "F8", "", {"ScriptEditor", "ResourceManager"})]
class PluginBuildMod: PluginProject
{	
	void PluginBuildMod()
	{
		if (m_WorkbenchSettings.Profiles == string.Empty) {
			ErrorDialog("You need to set the Profiles setting in Plugins -> Configure -> Configure Project");
			return;
		}		
		
		if (m_WorkbenchSettings.Missions == string.Empty) {
			ErrorDialog("You need to set the Missions setting in Plugins -> Configure -> Configure Project");
			return;
		}		
		
		if (m_WorkbenchSettings.Mods == string.Empty) {
			ErrorDialog("You need to set the Mods setting in Plugins -> Configure -> Configure Project");
			return;
		}
				
		string mod_prefix = GetModPrefix();
		// Set up our sym link for workshop modding :drool:
		string game_directory_stable = m_GameDirectory;
		if (!FileExist(string.Format("%1\\!Workshop", game_directory_stable))) {
			Print("Couldnt find workshop folder in current directory, probably experimental.");
			game_directory_stable = GetDirectory(game_directory_stable) + "\\DayZ";
			if (!FileExist(string.Format("%1\\!Workshop", game_directory_stable))) {
				Print("Womp womp");
				//ErrorDialog("Could not find DayZ Workshop folder. Is your Source Data correctly set?");
				//return;
			}
		}
		
		array<string> dependencies = EnumerateDirectories(string.Format("%1\\Dependencies", m_Repository));
		foreach (string dependency: dependencies) {
			PromiseSymLink(string.Format("%1\\Dependencies\\%2", m_Repository, dependency), GetAbsolutePath(string.Format("$Workdrive:%1", dependency)));
		}
		
		PromiseSymLink(string.Format("%1\\!Workshop", game_directory_stable), m_WorkbenchSettings.Mods);
		
		string args = m_WorkbenchSettings.Args;
		if (m_WorkbenchSettings.Key != string.Empty) {
			args += string.Format(" +K=%1",  m_WorkbenchSettings.Key);
		}
		
		string cli_mod;
		GetCLIParam("mod", cli_mod);
		if (m_WorkbenchSettings.Dependencies) {			
			array<string> mod_splits = {};
			cli_mod.Split(";", mod_splits);
			foreach (string mod_split: mod_splits) {
				// Packed mod, already build
				if (mod_split.Contains("@")) {
					continue;
				}
				
				BuildFolder(mod_split, args);
			}
		}
		
		// Move contents of Addons folder
		if (m_WorkbenchSettings.CopyAddons) {
			//CopyFiles(string.Format("%1\\Addons", m_Repository), mod_output + PATH_SEPERATOR_ALT + "Addons");
		}
	}
	
	int BuildFolder(string mod_input, string args)
	{
		array<string> input_path = {};
		mod_input.Split("\\", input_path);
		string mod_output = string.Format("%1\\@%2", m_WorkbenchSettings.Mods, input_path[input_path.Count() - 1]);
		Print(mod_output);
		return 0;
		
		PrintFormat("Building mod %1 to %2 with args %3", mod_input, mod_output, args);
		MakeDirectory(mod_output);
		MakeDirectory(mod_output + PATH_SEPERATOR_ALT + "Addons");
		MakeDirectory(mod_output + PATH_SEPERATOR_ALT + "Keys");
		return Workbench.RunCmd(string.Format("%1 -Mod=%2 %3 %4", m_WorkbenchSettings.Command, mod_output, mod_input, args));
	}
}