class RegisterScriptedEntity: Class
{
	static ref array<ref Param3<typename, string, string>> Instances = {};
	
	void RegisterScriptedEntity(typename type, string display_name, string icon = "set:solid image:circle")
	{		
		Instances.Insert(new Param3<typename, string, string>(type, display_name, icon));
	}
}