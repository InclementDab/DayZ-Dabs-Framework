class RegisterMissionSetting: Class
{
	static ref map<typename, string> s_RegisteredInstances = new map<typename, string>();
    static ref map<typename, ref GenericWrapper> s_RegisteredAttributes = new map<typename, ref GenericWrapper>();

	void RegisterMissionSetting(GenericWrapper template_wrapper, string file_path)
	{
		if (!SystemPath.IsPathRooted(file_path)) {
			file_path = SystemPath.Combine(SystemPath.Mission(), file_path);
		}

        typename type = template_wrapper.GetDataType();
		s_RegisteredInstances[type] = file_path;
        s_RegisteredAttributes[type] = template_wrapper;
	}

    // wtf going on here
    static string DataToStringStatic(Managed data)
    {
        string data_string;
        GenericWrapper data_class_wrapper = s_RegisteredAttributes[data.Type()];
        if (!data_class_wrapper) {
            ErrorEx("No data class wrapper");
            return data_string;
        }

        g_Script.CallFunction(data_class_wrapper, "GetDataString", data_string, data);
        return data_string;
    }
}