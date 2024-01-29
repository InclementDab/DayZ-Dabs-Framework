[RegisterScriptedEntity(NetworkSpotLight, "Spot Light", Symbols.FLASHLIGHT.Regular())]
class NetworkSpotLight: NetworkLight
{
	override ScriptedLightBase CreateLight(vector global_pos = "0 0 0", float fade_in_time_in_s = 0)
	{
		return ScriptedLightBase.CreateLight(SpotLightBase, global_pos, fade_in_time_in_s);
	}
}