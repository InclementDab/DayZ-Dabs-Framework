class RegisterAction
{
	static ref array<typename> RegisteredTypes = {};
	
	void RegisterAction(typename action_type)
	{
		if (!action_type.IsInherited(ActionBase_Basic)) {
			Error("Invalid action type");
			return;
		}
		
		if (!RegisteredTypes) {
			RegisteredTypes = {};
		}
		
		RegisteredTypes.Insert(action_type);
	}
}