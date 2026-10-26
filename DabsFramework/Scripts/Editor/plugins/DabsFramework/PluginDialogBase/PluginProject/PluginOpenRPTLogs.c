[WorkbenchPluginAttribute("Open RPT Logs", "Open Latest DayZ RPT Logs", "Ctrl+[", "", {"ScriptEditor", "ResourceManager"})]
class PluginOpenRPTLogs: PluginOpenScriptLogs
{
    override string GetFileFilter()
    {
        return "*.RPT";
    }

    override string GetFileContains()
    {
        return "DayZ";
    }

    override DateTime FileNameToTimestamp(string log_file_name)
    {
        array<string> log_file_name_split_major = {};
        // date[2]
        // time[3]
        log_file_name.Split("_", log_file_name_split_major);

        array<string> log_file_name_split_minor = {};
        // getting date
        log_file_name_split_major[2].Split("-", log_file_name_split_minor);
        int year = log_file_name_split_minor[0].ToInt();
        int month = log_file_name_split_minor[1].ToInt();
        int day = log_file_name_split_minor[2].ToInt();

        log_file_name_split_minor.Clear();
        log_file_name_split_major[3].Split("-", log_file_name_split_minor);
        int hour = log_file_name_split_minor[0].ToInt();
        int minute = log_file_name_split_minor[1].ToInt();
        int second = log_file_name_split_minor[2].ToInt();
        return DateTime.Create(year, month, day, hour, minute, second);
    }
}