class PluginLaunchGameBase: PluginProject
{	
	void LaunchGame(notnull LaunchSettings launch_settings)
	{
		string root = GetRootDirectory();
		string mod_prefix = GetPrefix();
		string workbench_directory = GetWorkbenchDirectory();
		// finding DayZ / DayZ Exp dir		
		string game_directory = GetDayZDirectory(launch_settings);		
		string game_exe = game_directory + PATH_SEPERATOR + launch_settings.Executable;
		if (!FileExist(game_exe)) {
			ErrorDialog(string.Format("Could not find the game at %1", game_exe));
			return;
		}
		
		if (workbench_directory == string.Empty) {
			Error("CWD is not workbench, you must launch via gproj");
			return;
		}
		
		DeleteFile(string.Format("%1\\steam_appid.txt", workbench_directory));
		CopyFile(string.Format("%1\\steam_appid.txt", game_directory), string.Format("%1\\steam_appid.txt", workbench_directory));
						
		//! Game launch script
		// append prefix of current mod
		if (!m_ProjectSettings["ServerMod"].ToInt() && !launch_settings.DisableMod) {			
			m_ProjectSettings["Mods"] = m_ProjectSettings["Mods"] + ";@" + mod_prefix;
		} else {
			m_ProjectSettings["ServerMod"] = m_ProjectSettings["ServerMod"];
		}
		
		if (launch_settings.LaunchType & 16) {
			m_ProjectSettings["Mods"] = m_ProjectSettings["Mods"] + ";@DayZ-Editor";
		}
			
		if (launch_settings.Repository == string.Empty) {
			ErrorDialog("You need to set the Repository setting in Plugins -> Configure -> Configure Project");
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

		if (launch_settings.SandboxieEnabled) {
			if (launch_settings.SandboxieBoxPath == string.Empty || !FileExist(launch_settings.SandboxieBoxPath)) {
				ErrorDialog("You need to set (a valid) Sandboxie Box (Steam Box Path) setting in Plugins -> Configure Project");
				return;
			}
			if (launch_settings.SandboxieInstallPath == string.Empty || !FileExist(launch_settings.SandboxieInstallPath)) {
				ErrorDialog("You need to set (a valid) Sandboxie Box (Sandboxie Install Path) setting in Plugins -> Configure Project");
				return;
			}
			if (launch_settings.SandboxieBoxPath == string.Empty || !FileExist(launch_settings.SandboxieBoxPath)) {
				ErrorDialog("You need to set (a valid) Sandboxie Box (Steam Box Path) setting in Plugins -> Configure Project");
				return;
			}
		}
				
		if (launch_settings.AutoClose) {
			KillTask(launch_settings.GetExecutableName());
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
				prefix.Replace(PATH_SEPERATOR, PATH_SEPERATOR_ALT);
				prefix.Split(PATH_SEPERATOR_ALT, prefix_split);
				
				string built_path = workbench_directory + PATH_SEPERATOR_ALT;
				if (prefix_split.Count() < 1) {
					continue;
				}
				
				// Add each root dir to exclude
				folders_to_save.Insert(prefix_split[0]);
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
		
		// Copy raw CLE files
		string repository_mission = string.Format("%1\\Missions\\%2.%3", launch_settings.Repository, mod_prefix, launch_settings.Map);
		if (FileExist(string.Format("%1\\ce", repository_mission))) {
			array<string> map_exports = Directory.EnumerateFiles(string.Format("%1\\ce\\map", repository_mission), "*.map");
			if (map_exports.Count() == 1) {
				DeleteFile(string.Format("%1\\areaflags.map", repository_mission));
				CopyFile(map_exports[0], string.Format("%1\\areaflags.map", repository_mission));
			}
			
			MakeDirectory(string.Format("%1\\env", repository_mission));
			array<string> territory_exports = Directory.EnumerateFiles(string.Format("%1\\ce\\territoryTypes", repository_mission), "*.xml");
			foreach (FileSystem territory_export: territory_exports) {
				CopyFile(territory_export, string.Format("%1\\env\\%2", repository_mission, territory_export.GetFileName()));
			}
		}
				
		MakeDirectory(launch_settings.Profiles);
		MakeDirectory(launch_settings.Missions);
		
		string client_profile_directory = string.Format("%1\\%2\\%3", launch_settings.Profiles, mod_prefix, LaunchSettings.CLIENT_PROFILE_NAME);
		string client2_profile_directory  = string.Format("%1\\%2\\%3", launch_settings.Profiles, mod_prefix, LaunchSettings.CLIENT2_PROFILE_NAME);
		string server_profile_directory = string.Format("%1\\%2\\%3", launch_settings.Profiles, mod_prefix, LaunchSettings.SERVER_PROFILE_NAME);		
		string server_mission = string.Format("%1\\%2.%3", launch_settings.Missions, mod_prefix, launch_settings.Map);
		
		// Make the folders if they dont exist yet
		MakeDirectory(client_profile_directory);

		if (launch_settings.SandboxieEnabled) {
			string sandboxie_profile_directory = string.Format("%1\\user\\current\\Documents\\DayZ Projects\\Profiles\\%2\\linking", launch_settings.SandboxieBoxPath, mod_prefix);
			MakeDirectory(sandboxie_profile_directory);
			if (!FileExist(client2_profile_directory)) {
				Workbench.RunCmd(string.Format("cmd /c mklink /j \"%1\" \"%2\"", client2_profile_directory, sandboxie_profile_directory), true);
			}

			if (!FileExist(client2_profile_directory)) {
				ErrorDialog("Client2 directory is not creating correctly, likely an issue you need to investigate on your own.");
				return;
			}

		}

		MakeDirectory(server_profile_directory);
		MakeDirectory(server_mission);
		
		CleanLogFolder(client_profile_directory);
		CleanLogFolder(client2_profile_directory);
		CleanLogFolder(server_profile_directory);
				
		// Copy maps and mission info
		CopyFiles(string.Format("%1\\Profiles\\Client", launch_settings.Repository), client_profile_directory);
		CopyFiles(string.Format("%1\\Profiles\\Client", launch_settings.Repository), client2_profile_directory);
		CopyFiles(string.Format("%1\\Profiles\\Global", launch_settings.Repository), server_profile_directory);
		CopyFiles(string.Format("%1\\Profiles\\Maps\\%2", launch_settings.Repository, launch_settings.Map), server_profile_directory);
		if (m_ProjectSettings["Profile"] != string.Empty) {
			CopyFiles(string.Format("%1\\Profiles\\%2", launch_settings.Repository, m_ProjectSettings["Profile"]), server_profile_directory);
		}
		
		CopyFiles(string.Format("%1\\Missions\\%3.%2", launch_settings.Repository, launch_settings.Map, mod_prefix), server_mission);
		CopyFiles(string.Format("%1\\Missions\\Global", launch_settings.Repository), server_mission);
		CopyFiles(string.Format("%1\\Missions\\Dev", launch_settings.Repository), server_mission);
		
		string client_launch_params = LaunchSettings.BASE_LAUNCH_PARAMS + string.Format(" \"-mod=%1\" \"-profiles=%2\"", formatted_mod_list, client_profile_directory);
		string client2_launch_params = LaunchSettings.BASE_LAUNCH_PARAMS + string.Format(" \"-mod=%1\" \"-profiles=%2\"", formatted_mod_list, client2_profile_directory);
		string server_launch_params = LaunchSettings.BASE_LAUNCH_PARAMS + string.Format(" \"-mod=%1\" \"-profiles=%2\" \"-serverMod=%3\" \"-config=%4\" \"-mission=%5\" -server -port=%6", formatted_mod_list, server_profile_directory, formatted_server_mod_list, m_ServerConfig, server_mission, launch_settings.Port);
		string offline_launch_params = LaunchSettings.BASE_LAUNCH_PARAMS + string.Format(" \"-mod=%1\" \"-profiles=%2\" \"-mission=%3\"", formatted_mod_list, client_profile_directory, repository_mission);		
		
		string ip, password;
		int port;
		array<string> args;
		if (GetConnectionArguments(ip, port, password, args)) {
			client_launch_params += string.Format(" -connect=%1 -port=%2", ip, port);
			client2_launch_params += string.Format(" -connect=%1 -port=%2", ip, port);
			if (password) {
				client_launch_params += string.Format(" -password=%1", password);
				client2_launch_params += string.Format(" -password=%1", password);
			}
			server_launch_params += string.Format(" -port=%1", port);
		}
		
		if (launch_settings.EnableHive) {
			server_launch_params += "-useDevHive";
			offline_launch_params += "-useDevHive";
		}
				 
		if (launch_settings.FilePatching) {
			client_launch_params += " -filePatching";
			client2_launch_params += " -filePatching";
			server_launch_params += " -filePatching -world=none";
			offline_launch_params += " -filePatching -world=none";
		}
		
		if ((launch_settings.LaunchType & GameLaunchType.CLIENT) == GameLaunchType.CLIENT) {
			Workbench.RunCmd(string.Format("%1 %2 -mission=dayzOffline.%3", game_exe, client_launch_params, m_LaunchSettings.Map));

			if (launch_settings.SandboxieEnabled) {
				array<string> outArr = {};
				launch_settings.SandboxieBoxPath.Split("\\", outArr);
				string steam_box_name = outArr[outArr.Count() - 1];
				
				Workbench.RunCmd(string.Format("\"%1\\Start.exe\" /box:%2 \"%3\" -client2 %4 -mission=dayzOffline.%5", launch_settings.SandboxieInstallPath, steam_box_name, game_exe, client2_launch_params, m_LaunchSettings.Map));
			}
		}	
		
		if ((launch_settings.LaunchType & GameLaunchType.SERVER) == GameLaunchType.SERVER) {
			Workbench.RunCmd(game_exe + " " + server_launch_params);
		}
		
		if ((launch_settings.LaunchType & GameLaunchType.OFFLINE) == GameLaunchType.OFFLINE) {
			// I DONT LIEK THIS :(
			if (FileExist(string.Format("%1\\storage_-1", repository_mission))) {
				Workbench.RunCmd(string.Format("cmd /c rmdir /s /q \"%1\"", GetAbsolutePath(string.Format("%1\\storage_-1", repository_mission))));
			}
			
			Print(game_exe + " " + offline_launch_params);
			Workbench.RunCmd(game_exe + " " + offline_launch_params);
		}
	}
	
	bool GetConnectionArguments(out string ip, out int port, out string password, out array<string> args)
	{
		if ((m_LaunchSettings.LaunchType != GameLaunchType.BOTH && !m_LaunchSettings.SandboxieEnabled)) {
			return false;
		}
		
		ip = "127.0.0.1";
		port = 2302;
		password = m_LaunchSettings.ServerPassword;		
		return true;
	}
}