// Todo: This does not work. For some reason the game hard crashes when returning an instance of Mission from another instance of a class.
//		if coming from a global function its fine, which is very strange. Even creating local variables doesnt do the trick, it just crashes.
// 		DayZ you are fucking shit

#ifdef DF_MISSION_FRAMEWORK
Mission CreateMissionWithHandler(string path)
{
	Print(path);	
	array<ref Param2<typename, int>> types = RegisterMissionType.Types;
	int highest_priority = -1;
	MissionWrapper wrapper_with_highest_priority;
	for (int i = types.Count() - 1; i >= 0; --i) {
		Param2<typename, int> type = types[i];
		if (type.param2 > highest_priority || !wrapper_with_highest_priority) {
			MissionWrapper wrapper = MissionWrapper.Cast(type.param1.Spawn());
			if (wrapper && wrapper.MissionLaunchCondition(path)) {
				PrintFormat("Valid wrapper candidate %1, %2", type.param1, type.param2);
				highest_priority = type.param2;
				wrapper_with_highest_priority = wrapper;
			}
		}
	}
	
	if (wrapper_with_highest_priority) {
		PrintFormat("Final Mission Decision: %1, %2", wrapper_with_highest_priority, highest_priority);
		return wrapper_with_highest_priority.CreateMission(path);
	}
	
	// Final Fallback
	return CreateMission(path);
}

class MissionWrapper: Class
{
	// Override to define logic that determines whether or not the mission is an acceptable launch candidate
	bool MissionLaunchCondition(string path)
	{
		return true;
	}
	
	// If you dont override this, its just going to use the default global vanilla logic.
	Mission CreateMission(string path)
	{
		ErrorEx("CreateMission was called but not overriden with any logic. You MUST return a value or the game will crash");
		GetGame().RequestExit(-1);
		return null;
	}
}

/*
// MissionWrapper type must be used
[RegisterMissionType(MyNewMissionWrapper, 5)]
class MyNewMissionWrapper: MissionWrapper
{	
	override bool MissionLaunchCondition(string path)
	{
		return g_Game.IsServer() && g_Game.IsMultiplayer();
	}
	
	override Mission CreateMission(string path)
	{
		return new MyNewMission(path);
	}
}

class MyNewMission: MissionServer
{
	void MyNewMission(string path)
	{
		PrintFormat("My New Mission Launched: %1", path);
	}
}*/
#endif