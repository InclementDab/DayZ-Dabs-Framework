typedef Param3<typename, bool, bool> RegisterPluginDebugData;

class RegisterPluginDebugAttribute
{
	static ref array<ref RegisterPluginDebugData> Instances = {};
	
	void RegisterPluginDebugAttribute(typename plugin, bool client, bool server)
	{
		if (!Instances) {
			Instances = {};
		}
				
		Instances.Insert(new RegisterPluginDebugData(plugin, client, server));
	}
}