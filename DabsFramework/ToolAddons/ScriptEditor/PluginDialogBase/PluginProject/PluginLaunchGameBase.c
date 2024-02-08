class PluginLaunchGameBase: PluginProject
{	
	void LaunchGame(notnull WorkbenchSettings launch_settings)
	{
		string root = GetWorkdrive();
		string mod_prefix = GetModPrefix();
		string workbench_directory = GetAppRoot();
		string cwd;
		GetCLIParam("cwd", cwd);
		
		if (workbench_directory == string.Empty) {
			Error("CWD is not workbench, you must launch via gproj");
			return;
		}
								
		//! Game launch script
		// append prefix of current mod
		if (!m_ProjectSettings["ServerMod"].ToInt()) {
			m_ProjectSettings["Mods"] = m_ProjectSettings["Mods"] + ";@" + mod_prefix;
		} else {
			m_ProjectSettings["ServerMod"] = m_ProjectSettings["ServerMod"] + ";@" + mod_prefix;
		}
				
		// finding DayZ / DayZ Exp dir		
		string game_directory = GetGameDirectory();		
		string game_exe = game_directory + EXECUTABLE;
		if (!FileExist(game_exe)) {
			ErrorDialog(string.Format("Could not find the game at %1", game_exe));
			return;
		}
		
		string repository = GetRepositoryRoot();
		if (repository == string.Empty) {
			ErrorDialog("You need to pass -repository to Workbench on launch");
			return;
		}		
		
		if (launch_settings.Profiles == string.Empty) {
			ErrorDialog("You need to set the Profiles setting in Plugins -> Configure -> Configure Project");
			return;
		}		
		
		if (launch_settings.Missions == string.Empty) {
			ErrorDialog("You need to set the Missions setting in Plugins -> Configure -> Configure Project");
			return;
		}		
		
		if (launch_settings.Mods == string.Empty) {
			ErrorDialog("You need to set the Mods setting in Plugins -> Configure -> Configure Project");
			return;
		}		
				
		if (launch_settings.AutoClose) {
			KillTask(EXECUTABLE);
		}
		
		// Set up symlinks so game can launch with our cwd
		PromiseSymLink(game_directory + PATH_SEPERATOR_ALT + "Addons", workbench_directory + PATH_SEPERATOR_ALT + "Addons");
		PromiseSymLink(game_directory + PATH_SEPERATOR_ALT + "bliss", workbench_directory + PATH_SEPERATOR_ALT + "bliss");

		// Delete all extra folders in wb directory
		array<string> folders_to_save = {};
		
		// always save these
		folders_to_save.Copy(WB_DIR_DEFAULTS);
		
		// Set up filepatching, needs to either create or delete all links depending on the setting
		if (launch_settings.FilePatching) {
			foreach (string prefix: m_Prefixes) {				
				array<string> prefix_split = {};
				prefix.Split(PATH_SEPERATOR_ALT, prefix_split);
				
				string built_path = workbench_directory + PATH_SEPERATOR_ALT;
				if (prefix_split.Count() < 1) {
					continue;
				}
				
				// Add each root dir to exclude
				folders_to_save.Insert(prefix_split[0]);
				
				for (int k = 0; k < prefix_split.Count() - 1; k++) {
					built_path += prefix_split[k] + PATH_SEPERATOR_ALT;
					MakeDirectory(built_path);
				}
				
				PromiseSymLink(root + PATH_SEPERATOR_ALT + prefix, workbench_directory + PATH_SEPERATOR + prefix);
			}
		}
		
		// Now FindFile each 
		string wb_dir_filename;
		FileAttr wb_dir_fileattr;
		FindFileHandle hdnl = FindFile(workbench_directory + PATH_SEPERATOR + "*", wb_dir_filename, wb_dir_fileattr, FindFileFlags.DIRECTORIES);
		
		if (folders_to_save.Find(wb_dir_filename) == -1) {
			DeleteFile(wb_dir_filename);
		}
		
		while (FindNextFile(hdnl, wb_dir_filename, wb_dir_fileattr)) {
			if (folders_to_save.Find(wb_dir_filename) == -1 && wb_dir_fileattr == FileAttr.DIRECTORY) {
				Workbench.RunCmd(string.Format("cmd /c rmdir /s /q \"%1\"", workbench_directory + PATH_SEPERATOR + wb_dir_filename));
			}
		}
		
		CloseFindFile(hdnl);

		// Reformats mod list
		string formatted_mod_list;
		array<string> mod_list = {};
		m_ProjectSettings["Mods"].Split(";", mod_list);
		for (int i = 0; i < mod_list.Count(); i++) {
			formatted_mod_list += launch_settings.Mods + PATH_SEPERATOR + mod_list[i];
			if (i != mod_list.Count() - 1) {
				formatted_mod_list += ";";
			}
		}
		
		// Reformats server mod list
		string formatted_server_mod_list;
		array<string> server_mod_list = {};
		m_ProjectSettings["ServerMods"].Split(";", server_mod_list);
		for (int j = 0; j < server_mod_list.Count(); j++) {
			formatted_server_mod_list += launch_settings.Mods + PATH_SEPERATOR + server_mod_list[j];
			if (j != server_mod_list.Count() - 1) {
				formatted_server_mod_list += ";";
			}
		}
		
		MakeDirectory(launch_settings.Profiles);
		MakeDirectory(launch_settings.Missions);
		
		string client_profile_directory = string.Format("%1\\%2\\%3", launch_settings.Profiles, mod_prefix, WorkbenchSettings.CLIENT_PROFILE_NAME);
		string server_profile_directory = string.Format("%1\\%2\\%3", launch_settings.Profiles, mod_prefix, WorkbenchSettings.SERVER_PROFILE_NAME);		
		string server_mission = string.Format("%1\\%2.%3", launch_settings.Missions, mod_prefix, launch_settings.Map);
		
		// Make the folders if they dont exist yet
		MakeDirectory(client_profile_directory);
		MakeDirectory(server_profile_directory);
		MakeDirectory(server_mission);
		
		// Always clean them log folders
		if (launch_settings.Deloginator) {
			CleanLogFolder(client_profile_directory);
			CleanLogFolder(server_profile_directory);
		}
		
		// Copy maps and mission info
		CopyFiles(string.Format("%1\\Profiles\\Global", repository), server_profile_directory);
		CopyFiles(string.Format("%1\\Profiles\\Maps\\%2", repository, launch_settings.Map), server_profile_directory);
		if (m_ProjectSettings["Profile"] != string.Empty) {
			CopyFiles(string.Format("%1\\Profiles\\%2", repository, m_ProjectSettings["Profile"]), server_profile_directory);
		}
		
		CopyFiles(string.Format("%1\\Missions\\%3.%2", repository, launch_settings.Map, mod_prefix), server_mission);
		CopyFiles(string.Format("%1\\Missions\\Global", repository), server_mission);
		CopyFiles(string.Format("%1\\Missions\\Dev", repository), server_mission);
		
		string client_launch_params = WorkbenchSettings.BASE_LAUNCH_PARAMS + string.Format(" \"-mod=%1\" \"-profiles=%2\"", formatted_mod_list, client_profile_directory);
		string server_launch_params = WorkbenchSettings.BASE_LAUNCH_PARAMS + string.Format(" \"-mod=%1\" \"-profiles=%2\" \"-serverMod=%3\" \"-config=%4\" \"-mission=%5\" -server", formatted_mod_list, server_profile_directory, formatted_server_mod_list, m_ServerConfig, server_mission);
		string offline_launch_params = WorkbenchSettings.BASE_LAUNCH_PARAMS + string.Format(" \"-mod=@DayZ-Editor;%1\" \"-profiles=%2\" \"-mission=%3\"", formatted_mod_list, client_profile_directory, server_mission);

		string ip, password;
		int port;
		if (GetConnectionArguments(ip, port, password)) {
			client_launch_params += string.Format(" -connect=%1 -port=%2 -password=%3", ip, port, password);
			server_launch_params += string.Format(" -port=%1", port);
		}
		 
		if (launch_settings.FilePatching) {
			client_launch_params += " -filePatching";
			server_launch_params += " -filePatching";
			offline_launch_params += " -filePatching";
		}
						
		if ((launch_settings.LaunchType & GameLaunchType.CLIENT) == GameLaunchType.CLIENT) {
			Workbench.RunCmd(game_exe + " " + client_launch_params);
			//Workbench.RunCmd(game_exe + " -client2 " + client_launch_params);
		}	
		
		if ((launch_settings.LaunchType & GameLaunchType.SERVER) == GameLaunchType.SERVER) {
			Workbench.RunCmd(game_exe + " " + server_launch_params);
		}
		
		if ((launch_settings.LaunchType & GameLaunchType.OFFLINE) == GameLaunchType.OFFLINE) {
			Workbench.RunCmd(game_exe + " " + offline_launch_params);
		}
	}
	
	bool GetConnectionArguments(out string ip, out int port, out string password)
	{
		return false;
	}
}