modded class PluginManager
{
	override void Init()
	{
		foreach (Param4<typename, bool, bool, PluginType> plugin_data: RegisterPluginAttribute.Instances) {
			switch (plugin_data.param4) {
				case PluginType.STABLE: {
					RegisterPlugin(plugin_data.param1.ToString(), plugin_data.param2, plugin_data.param3);
					break;
				}
				
				case PluginType.DIAG: {
					RegisterPluginDiag(plugin_data_diag.param1.ToString(), plugin_data_diag.param2, plugin_data_diag.param3);
					break;
				}
				
				
				case PluginType.DEBUG: {
					RegisterPluginDebug(plugin_data_debug.param1.ToString(), plugin_data_debug.param2, plugin_data_debug.param3);
					break;
				}
			}	
		}
		
		// Initializing after since OnUpdate gets queued at the end of super.Init
        super.Init();
	}
}