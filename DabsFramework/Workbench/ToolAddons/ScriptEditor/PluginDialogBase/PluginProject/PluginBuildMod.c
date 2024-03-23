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
				ErrorDialog("Could not find DayZ Workshop folder. Is your Source Data correctly set?");
				return;
			}
		}
		
		array<string> dependencies = EnumerateDirectories(string.Format("%1\\Dependencies", m_LaunchSettings.Repository));
		foreach (string dependency: dependencies) {
			PromiseSymLink(string.Format("%1\\Dependencies\\%2", m_LaunchSettings.Repository, dependency), GetAbsolutePath(string.Format("$Workdrive:%1", dependency)));
		}
		
		PromiseSymLink(string.Format("%1\\!Workshop", game_directory_stable), m_LaunchSettings.Mods);				
		// Set up our mod output correctly if not done so already
		string mod_output = string.Format("%1\\@%2", m_LaunchSettings.Mods, mod_prefix);
		
		MakeDirectory(mod_output);
		MakeDirectory(mod_output + PATH_SEPERATOR_ALT + "Addons");
		MakeDirectory(mod_output + PATH_SEPERATOR_ALT + "Keys");
		
		// Move contents of Addons folder
		if (m_BuildSettings.CopyAddons) {
			CopyFiles(string.Format("%1\\Addons", m_LaunchSettings.Repository), mod_output + PATH_SEPERATOR_ALT + "Addons");
		}
		
		array<string> mod_splits = {};
		m_ProjectSettings["Mods"].Split(";", mod_splits);
		foreach (string mod_split: mod_splits) {
			string mod_split_edit = mod_split;
			mod_split_edit.Replace("@", "");
			
		}
		
		string args = m_BuildSettings.Args;
		if (m_BuildSettings.Key != string.Empty) {
			args += string.Format(" +K=%1",  m_BuildSettings.Key);
		}

		Workbench.RunCmd(string.Format("%3 %4 -Mod=%1 P:\\%2", mod_output, mod_prefix, m_BuildSettings.Command, args));
	}
}