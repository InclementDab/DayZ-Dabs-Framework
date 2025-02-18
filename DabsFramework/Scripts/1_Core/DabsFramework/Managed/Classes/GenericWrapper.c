// Use GenericWrapper1, not this
class GenericWrapper: Managed
{
    typename GetDataType(int variable_index = 0)
    {
        return Type().GetVariableType(variable_index);
    }
	
	// Implemented in GenericWrapper1
	bool WriteToJson(notnull Managed value, out string json_string, bool nice = true)
	{
		return false;
	}
	
	bool ReadFromJson(inout notnull Managed value, string json_string, out string json_error)
	{
		return false;
	}
}