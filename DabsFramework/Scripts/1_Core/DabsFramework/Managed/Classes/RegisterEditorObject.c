class RegisterScriptedEntity: Class
{
	static ref array<ref Param3<typename, string, string>> Instances = {};
	
	void RegisterEditorObject(typename type, string display_name, string icon)
	{		
		Instances.Insert(new Param3<typename, string, string>(type, display_name, icon));
	}
}