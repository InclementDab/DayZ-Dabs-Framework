/*
*
*	EventBase.c
*
*	Abstracted class for all Events that run on Namalsk
*
*	You are free to mod this as you please. 
*	Just credit @InclementDab for the scripting and @Sumrak for the modeling
*	
*	That being said you are NOT free to redistribute / repack this into
*	your own mod. 
*
*/

class EventBase
{
	protected int m_EventState = -1;
	
	protected Weather m_Weather;
	protected PlayerBase m_Player;

	protected ref Timer m_ClientUpdate = new Timer(CALL_CATEGORY_SYSTEM);
	protected ref Timer m_ServerUpdate = new Timer(CALL_CATEGORY_SYSTEM);
	
	// Phase Time Remaining
	protected float m_PhaseTimeRemaining;
	protected ref Timer m_TimeRemainingTimer = new Timer(CALL_CATEGORY_SYSTEM);

	void EventBase()
	{
		m_Weather = GetGame().GetWeather();
		
		if (GetGame().IsClient() || !GetGame().IsMultiplayer()) {
			m_Player = PlayerBase.Cast(GetGame().GetPlayer());
		}
		
		if (GetGame().IsClient() || !GetGame().IsMultiplayer()) {
			m_ClientUpdate.Run(GetClientTick(), this, "UpdateClient", null, true);
		}
		
		if (GetGame().IsServer()) {
			m_ServerUpdate.Run(GetServerTick(), this, "UpdateServer", null, true);
		}
		
		m_TimeRemainingTimer.Run(1.0, this, "UpdateTimeRemaining", null, true);
	}
	
	void ~EventBase()
	{
		if (m_ClientUpdate) {
			m_ClientUpdate.Stop();
			delete m_ClientUpdate;
		}
		
		if (m_ServerUpdate) {
			m_ServerUpdate.Stop();
			delete m_ServerUpdate;
		}
		
		if (m_TimeRemainingTimer) {
			m_TimeRemainingTimer.Stop();
			delete m_TimeRemainingTimer;
		}
	}
	
	// Abstract methods
	protected void InitPhaseClient(float phase_time);
	protected void MidPhaseClient(float phase_time);
	protected void EndPhaseClient(float phase_time);
	
	protected void InitPhaseServer();
	protected void MidPhaseServer();
	protected void EndPhaseServer();

	protected void OnEventEndClient();
	protected void OnEventEndServer();
	
	// Update Loops
	protected void UpdateClient();
	protected void UpdateServer();
	
	void OnRPC(PlayerIdentity sender, Object target, int rpc_type, ParamsReadContext ctx);
	
	float GetCurrentPhaseLength()
	{
		return GetPhaseLength(m_EventState);
	}
	
	float GetPhaseLength(EventPhase phase)
	{
		switch (phase) {
			case EventPhase.INIT:	return GetInitPhaseLength();
			case EventPhase.MID: 	return GetMidPhaseLength();			
			case EventPhase.END: 	return GetEndPhaseLength();			
		}
		
		return 0;
	}
	
	float GetInitPhaseLength()
	{
		return 5.0;
	}
	
	float GetMidPhaseLength()
	{
		return 10.0;
	}
	
	float GetEndPhaseLength()
	{
		return 5.0;
	}
	
	EventPhase GetActivePhaseID()
	{
		return m_EventState;
	}
		
	void SwitchPhase(EventPhase phase, float time_remaining = 0)
	{
		if (m_EventState == phase) {
			EventDebug("Event was already in phase %1, exiting...", phase.ToString());
			return;
		}
		
		m_EventState = phase;
		EventDebug("SwitchPhase %1", m_EventState.ToString());
		
		if (GetGame().IsServer()) {		
			m_PhaseTimeRemaining = GetCurrentPhaseLength();	
			switch (m_EventState) {
				case EventPhase.INIT: {
					thread InitPhaseServer();
					break;
				}
				
				case EventPhase.MID: {
					thread MidPhaseServer();
					break;
				}
				
				case EventPhase.END: {
					thread EndPhaseServer();
					break;
				}
				
				case EventPhase.DELETE:
				default: {
					OnEventEndServer();
					break;
				}
			}
		}
		
		if (GetGame().IsClient() || !GetGame().IsMultiplayer()) {	
			m_PhaseTimeRemaining = time_remaining;		
			switch (m_EventState) {
				case EventPhase.INIT: {
					thread InitPhaseClient(time_remaining);
					break;
				}
				
				case EventPhase.MID: {
					thread MidPhaseClient(time_remaining);
					break;
				}
				
				case EventPhase.END: {
					thread EndPhaseClient(time_remaining);
					break;
				}
				
				case EventPhase.DELETE:
				default: {
					OnEventEndClient();
					break;
				}
			}
		}
	}
	
	float GetCurrentPhaseTimeRemaining()
	{
		return m_PhaseTimeRemaining;
	}
	
	// Good lord shorten this
	float GetCurrentPhaseTimeRemainingNormalized()
	{
		return m_PhaseTimeRemaining / GetCurrentPhaseLength();
	}
		
	bool EventActivateCondition()
	{
		return true;
	}
	
	vector GetEventPosition();
	
	bool JIPRunPreviousPhases()
	{
		return true;
	}
	
	float GetClientTick()
	{
		return 0.01;
	}
	
	float GetServerTick()
	{
		return 0.01;
	}
	
	// If you want the Event to have a CD, override this
	// return: value in seconds
	int GetEventCooldown()
	{
		return 0;
	}
	
	TTypenameArray GetDisallowedEvents()
	{
		return {};
	}
	
	private void UpdateTimeRemaining()
	{
		m_PhaseTimeRemaining -= 1.0;

		if (m_PhaseTimeRemaining <= 0) {
			EventDebug("Phase lasting too long! Is the server out of sync?");
			m_PhaseTimeRemaining = 0;
		}
	}
	
	void EventDebug(string msg, string param1 = "", string param2 = "", string param3 = "", string param4 = "", string param5 = "", string param6 = "", string param7 = "", string param8 = "", string param9 = "")
	{
#ifdef NAMALSK_SURVIVAL_DEBUG		
		PrintFormat("[Namalsk][" + Type() + "]: " + msg, param1, param2, param3, param4, param5, param6, param7, param8, param9);
#endif
	}
	
	void EventInfo(string msg, string param1 = "", string param2 = "", string param3 = "", string param4 = "", string param5 = "", string param6 = "", string param7 = "", string param8 = "", string param9 = "")
	{
		PrintFormat("[Namalsk][" + Type() + "]: " + msg, param1, param2, param3, param4, param5, param6, param7, param8, param9);
	}
};