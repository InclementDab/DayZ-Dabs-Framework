/*
*
*	WeatherEvent.c
*
*	An abstracted version of EventBase that accounts for weather
*
*	You are free to mod this as you please. 
*	
*	That being said you are NOT free to redistribute / repack this into
*	your own mod. 
*
*/

class WeatherEvent: EventBase
{
	override void SwitchPhase(EventPhase phase, float time_remaining = 0, Param client_data = null)
	{
		super.SwitchPhase(phase, time_remaining, client_data);
				
		if (!GetGame().IsServer()) {
			return;
		}
		
		// handle all of the WeatherEvent logic
		switch (phase) {
			case EventPhase.INIT: {
				// iterate through all weather phenomenons and request a weather change
				for (int i = EWeatherPhenomenon.OVERCAST; i <= EWeatherPhenomenon.RAIN; i++) {		
					if (GetWeatherTarget(i) != -1) {
						RequestWeatherChange(i, GetWeatherTarget(i), GetInitPhaseLength(), GetMidPhaseLength());
					}
				}
				
				break;
			}	
			
			case EventPhase.END: {
				// Reset all weather
				// This is resposible for the weather not looking totally choppy when events end
				
				// i.e. Snowfall brings fog to 0.5, HeavyFog then brings it to 1.0, after HeavyFog finishes,
				// the fog level will be brought back down to 0.5, until Snowfall returns it back to 0
				for (int j = EWeatherPhenomenon.OVERCAST; j <= EWeatherPhenomenon.RAIN; j++) {
					float highest_value_end = GetHighestRemainingWeatherValue(j);
					if (highest_value_end == 0 && j == EWeatherPhenomenon.OVERCAST) {
						highest_value_end = Math.RandomFloat(0, 0.15);
					}
					
					RequestWeatherChange(j, highest_value_end, GetEndPhaseLength(), GetEndPhaseLength());
				}
				break;
			}
			
			case EventPhase.DELETE: {
				for (int k = EWeatherPhenomenon.OVERCAST; k <= EWeatherPhenomenon.RAIN; k++) {
					WeatherPhenomenon weather_phenom = GetWeatherPhenomenon(k);
					float highest_value_delete = GetHighestRemainingWeatherValue(k);
					if (highest_value_delete == 0 && k == EWeatherPhenomenon.OVERCAST) {
						highest_value_delete = 0.15; // Keeps things spicy
					}
					
					weather_phenom.SetLimits(0, highest_value_delete);
				}
				break;
			}
		}
	}
		
	// Override all of these magical values and everything else is handled above
	float GetFogTarget()
	{
		return -1;
	}
	
	float GetRainTarget()
	{
		return -1;
	}
	
	float GetOvercastTarget()
	{
		return -1;
	}
	
	float GetWeatherTarget(EWeatherPhenomenon change_type)
	{
		switch (change_type) {
			case EWeatherPhenomenon.FOG: 		return GetFogTarget();
			case EWeatherPhenomenon.RAIN: 		return GetRainTarget();
			case EWeatherPhenomenon.OVERCAST: 	return GetOvercastTarget();
		}
		
		return -1;
	}
	
	static WeatherPhenomenon GetWeatherPhenomenon(EWeatherPhenomenon change_type)
	{
		switch (change_type) {
			case EWeatherPhenomenon.FOG: 		return GetGame().GetWeather().GetFog();
			case EWeatherPhenomenon.RAIN: 		return GetGame().GetWeather().GetRain();
			case EWeatherPhenomenon.OVERCAST: 	return GetGame().GetWeather().GetOvercast();
		}
		
		return null;
	}
	
	// Safe way of requesting a change in weather events
	protected bool RequestWeatherChange(EWeatherPhenomenon weather_type, float change, float time, float duration)
	{
		EventManagerLog.Debug(this, "Requested Weather Change %1, value %2", typename.EnumToString(EWeatherPhenomenon, weather_type), change.ToString());		
		array<EventBase> active_events = EventManager.GetInstance().GetActiveEvents();
		foreach (EventBase fog_event: active_events) {
			WeatherEvent weather_event;
			if (Class.CastTo(weather_event, fog_event)) {
				if (weather_event != this && weather_event.GetWeatherTarget(weather_type) != -1) {
					if (change < weather_event.GetWeatherTarget(weather_type) && weather_event.GetCurrentPhase() <= EventPhase.MID) {
						EventManagerLog.Debug(this, "Weather Change Request Denied");
						return false;
					}
				}
			}
		}
				
		EventManagerLog.Debug(this, "Weather Change Approved");
		WeatherPhenomenon weather_phenomenon = GetWeatherPhenomenon(weather_type);
		if (!weather_phenomenon) {
			Error("Invalid Weather Phenomenon");
			return false;
		}
				
		// adjust limits
		float low, high;
		weather_phenomenon.GetLimits(low, high);
		
		// we only want this value to go higher if the requested change is higher than something already active
		// we let the lower value changes occur in OnEventEndServer
		if (change > high) {
			EventManagerLog.Debug(this, "Changing limits to [0, %2]", low.ToString(), change.ToString());			
			weather_phenomenon.SetLimits(0, change);
		}
		
		// Set weather to value
		weather_phenomenon.Set(change, time, duration);
		return true;
	}
	
	float GetHighestRemainingWeatherValue(EWeatherPhenomenon change_type)
	{
		// This does a few important things	//
		// 1. Grabs ceiling weather value
		// 2. If this is the last running event, value will be zero (back to sunny day)
		array<EventBase> active_events = m_EventManager.GetActiveEvents();
		float value = 0;
		foreach (EventBase active_event: active_events) {
			if (!active_event || active_event == this || active_event.GetCurrentPhase() > EventPhase.MID) {
				continue;
			}
			
			WeatherEvent weather_event;
			if (Class.CastTo(weather_event, active_event)) {
				if (value <= weather_event.GetWeatherTarget(change_type)) {
					value = weather_event.GetWeatherTarget(change_type);
				}
			}	
		}
		
		return value;
	}
}