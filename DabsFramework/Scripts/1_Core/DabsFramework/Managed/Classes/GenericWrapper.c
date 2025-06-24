// Use GenericWrapper1, not this
class GenericWrapper: Managed
{
	static ref array<GenericWrapper> s_All = {};
	
	void GenericWrapper()
	{
		if (!s_All) {
			s_All = {};
		}
		
		s_All.Insert(this);
	}
	
	void ~GenericWrapper()
	{
		if (s_All) {
			s_All.RemoveItem(this);
		}
	}
	
	// this variable type is going from 0 -> 1 due to the requirement of s_All.
	// its a thin veil but im not engineering it any different
    typename GetDataType(int variable_index = 1)
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