[WorkbenchPluginAttribute("Open Script Logs", "Open Latest DayZ Script Logs", "Ctrl+]", "", {"ScriptEditor", "ResourceManager"})]
class PluginOpenScriptLogs: PluginProject
{
    override void Run()
    {
		super.Run();
		
        if (!FileExist(m_LaunchSettings.Profiles)) {
            return;
        }
		
        string profile_folder = SystemPath.Combine(m_LaunchSettings.Profiles, GetPrefix());
		
		bool is_dl = m_LaunchSettings.Repository.Contains("DabsLabs");
		if (is_dl) {
			profile_folder = string.Format("%1\\Missions", m_LaunchSettings.Repository);
		}
		
		array<string> subfolders = {};
		if (m_LaunchSettings.LaunchType & GameLaunchType.SERVER) {
            subfolders.Insert("server");
		}

        if (m_LaunchSettings.LaunchType & (GameLaunchType.CLIENT | GameLaunchType.OFFLINE)) {
			if (!is_dl) {
            	subfolders.Insert("client");
			} else {
				if (m_LaunchSettings.LaunchType & GameLaunchType.CLIENT) {
					subfolders.Insert("client");
				}
				
				if (m_LaunchSettings.LaunchType & GameLaunchType.OFFLINE) {
					subfolders.Insert("offline");
				}
			}
        }
				
        array<string> files_to_open = {};
        string launch_command = string.Empty;
        foreach (string subfolder: subfolders) {
            string folder = SystemPath.Combine(profile_folder, subfolder);
            array<string> log_files = Directory.EnumerateFiles(folder, GetFileFilter());
            map<int, string> file_by_timestamp = new map<int, string>();
            foreach (string log_file: log_files) {
                if (!log_file.Contains(GetFileContains())) {
                    continue;
                }

                string log_file_name = File.GetName(log_file);                    
                DateTime file_timestamp = FileNameToTimestamp(log_file_name);
                
                //PrintFormat("%1: %2", log_file_name, file_timestamp);
                file_by_timestamp[file_timestamp] = log_file;
            }

            int highest = -1;
            string highest_file;
            foreach (int file_time_stamp, string file_name: file_by_timestamp) {
                if (file_time_stamp > highest) {
                    highest_file = file_name;
                    highest = file_time_stamp;
                }
            }

            if (highest_file) {
                string command = string.Format("code -g %1:%2:9999", highest_file, int.MAX);
                if (launch_command) {
                    launch_command += " && ";
                }

                launch_command += command;
            }
        }

        int result = Workbench.RunCmd(string.Format("cmd /q /c \"%1\"", launch_command));
        if (result) {
            ErrorDialog("Failed to open vscode");
        }
    }

    string GetFileFilter()
    {
        return "*.log";
    }

    string GetFileContains()
    {
        return "script_";
    }

    DateTime FileNameToTimestamp(string log_file_name)
    {
        array<string> log_file_name_split_major = {};
        // date[1]
        // time[2]
        log_file_name.Split("_", log_file_name_split_major);

        array<string> log_file_name_split_minor = {};
        // getting date
        log_file_name_split_major[1].Split("-", log_file_name_split_minor);
        int year = log_file_name_split_minor[0].ToInt();
        int month = log_file_name_split_minor[1].ToInt();
        int day = log_file_name_split_minor[2].ToInt();

        log_file_name_split_minor.Clear();
        log_file_name_split_major[2].Split("-", log_file_name_split_minor);
        int hour = log_file_name_split_minor[0].ToInt();
        int minute = log_file_name_split_minor[1].ToInt();
        int second = log_file_name_split_minor[2].ToInt();
        return DateTime.Create(year, month, day, hour, minute, second);
    }
}