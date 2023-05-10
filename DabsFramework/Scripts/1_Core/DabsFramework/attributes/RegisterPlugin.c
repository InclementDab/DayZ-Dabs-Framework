typedef Param3<typename, bool, bool> RegisterPluginData;

class RegisterPluginAttribute
{
	static ref array<ref RegisterPluginData> Instances = {};
	
	void RegisterPluginAttribute(typename plugin, bool client, bool server)
	{
		if (!Instances) {
			Instances = {};
		}
				
		Instances.Insert(new RegisterPluginData(plugin, client, server));
	}
}