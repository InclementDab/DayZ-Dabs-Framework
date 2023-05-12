enum PluginType
{
	STABLE,
	DIAG,
	DEBUG
};

class RegisterPluginAttribute: Managed
{
	static ref array<ref Param4<typename, bool, bool, PluginType>> Instances = {};
	
	void RegisterPluginAttribute(typename plugin, bool client, bool server, PluginType plugin_type = PluginType.STABLE)
	{
		if (!Instances) {
			Instances = {};
		}
				
		Instances.Insert(new Param4<typename, bool, bool, PluginType>(plugin, client, server, plugin_type));
	}
}