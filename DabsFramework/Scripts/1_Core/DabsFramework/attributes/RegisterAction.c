class RegisterAction
{
	static ref array<typename> RegisteredTypes = {};
	
	void RegisterAction(typename action_type)
	{		
		if (!RegisteredTypes) {
			RegisteredTypes = {};
		}
		
		RegisteredTypes.Insert(action_type);
	}
}