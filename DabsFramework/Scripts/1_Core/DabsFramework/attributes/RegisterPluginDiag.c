typedef Param3<typename, bool, bool> RegisterPluginDiagData;

class RegisterPluginDiagAttribute
{
	static ref array<ref RegisterPluginDiagData> Instances = {};
	
	void RegisterPluginDiagAttribute(typename plugin, bool client, bool server)
	{
		if (!Instances) {
			Instances = {};
		}
		
		Instances.Insert(new RegisterPluginDiagData(plugin, client, server));
	}
}