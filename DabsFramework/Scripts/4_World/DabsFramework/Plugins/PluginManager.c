modded class PluginManager
{
	override void Init()
	{
		foreach (RegisterPluginData plugin_data: RegisterPluginAttribute.Instances) {
			RegisterPlugin(plugin_data.param1.ToString(), plugin_data.param2, plugin_data.param3);
		}

        foreach (RegisterPluginData plugin_data_diag: RegisterPluginDiagAttribute.Instances) {
			RegisterPluginDiag(plugin_data_diag.param1.ToString(), plugin_data_diag.param2, plugin_data_diag.param3);
        }

        foreach (RegisterPluginDebugData plugin_data_debug: RegisterPluginDebugAttribute.Instances) {
			RegisterPluginDebug(plugin_data_debug.param1.ToString(), plugin_data_debug.param2, plugin_data_debug.param3);
        }

		// Initializing after since OnUpdate gets queued at the end of super.Init
        super.Init();
	}
}