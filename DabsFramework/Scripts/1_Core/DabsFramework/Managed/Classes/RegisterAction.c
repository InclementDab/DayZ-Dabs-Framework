class RegisterAction
{
	static ref array<typename> RegisteredTypes = {};
	
	void RegisterAction(typename action_type)
	{		
		if (!RegisteredTypes) {
			RegisteredTypes = {};
		}
		
		RegisteredTypes.Insert(action_type);
		
		// Always sort, so load order between server & client makes no difference
		Sort(RegisteredTypes, RegisteredTypes.Count());
	}
}