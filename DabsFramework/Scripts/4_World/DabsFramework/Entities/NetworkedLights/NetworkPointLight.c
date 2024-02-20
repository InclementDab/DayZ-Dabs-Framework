class NetworkPointLight: NetworkLightBase
{	
	override ScriptedLightBase CreateLight(vector global_pos = "0 0 0", float fade_in_time_in_s = 0)
	{
		return ScriptedLightBase.CreateLight(PointLightBase, global_pos, fade_in_time_in_s);
	}
}