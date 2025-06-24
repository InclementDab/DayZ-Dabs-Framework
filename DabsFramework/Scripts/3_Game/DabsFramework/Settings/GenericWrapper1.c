class GenericWrapper1<Class T>: GenericWrapper
{
    private T _;
	
	void ~GenericWrapper1()
	{
		delete _;
	}

    string GetDataString(T data)
    {
        string data_string, error_message;
        bool success = JsonFileLoader<T>.MakeData(data, data_string, error_message);
        if (!success) {
            return "ERROR: " + error_message; // idk, cursed
        }

        return data_string;
    }
	
	override bool WriteToJson(notnull Managed value, out string json_string, bool nice = true)
	{
		JsonSerializer serializer = new JsonSerializer();
		T local_value;
		if (!Class.CastTo(local_value, value)) {
			return false;
		}
		
		return serializer.WriteToString(local_value, nice, json_string);
	}
	
	override bool ReadFromJson(inout notnull Managed value, string json_string, out string json_error)
	{
		// What hellish things are happening under the hood here?
		JsonSerializer serializer = new JsonSerializer();
		
		// since ReadFromString takes a void parameter, we will need to strongly type the Templated value
		// that way the JsonSerializer reads it properly
		T local_value;
		if (!Class.CastTo(local_value, value)) {
			return false;
		}
		
		bool result = serializer.ReadFromString(local_value, json_string, json_error);
		value = local_value;
		return result;
	}
}