class RegisterMissionType: Class
{
	static ref array<ref Param2<typename, int>> Types = {};
	
	void RegisterMissionType(typename action_type, int priority)
	{				
		Types.Insert(new Param2<typename, int>(action_type, priority));
	}
}