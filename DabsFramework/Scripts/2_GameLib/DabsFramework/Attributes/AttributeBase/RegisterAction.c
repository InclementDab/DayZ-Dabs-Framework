class RegisterAction: Class
{
	static ref map<int, typename> RegisteredTypes = new map<int, typename>();
	
	void RegisterAction(typename action_type)
	{		
		if (!RegisteredTypes) {
			RegisteredTypes = new map<int, typename>();
		}
		
		int hash = action_type.ToString().Hash();
		RegisteredTypes[hash] = action_type;
		
		// Always sort, so load order between server & client makes no difference
		//Sort(RegisteredTypes, RegisteredTypes.Count());
	}
}