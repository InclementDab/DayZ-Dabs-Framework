/*
*
*	EventBase.c
*
*	Abstracted class for all Events
*
*	You are free to mod this as you please. 
*	
*	That being said you are NOT free to redistribute / repack this into
*	your own mod. 
*
*/

class EventBase
{
	static const float PHASE_TIME_REMAINING_PRECISION = 1.0;
	
	protected int m_Id;
	protected EventManager m_EventManager;
	protected ref Param m_StartParams; // startup params, passed from EventManager::StartEvent
	protected bool m_IsPaused;
	protected EventPhase m_EventPhase = EventPhase.INVALID; // starting at -1 will let it naturally reach 0 when the Start function is called
	protected float m_PhaseTimeRemaining;
	
	protected Weather m_Weather;
	protected PlayerBase m_Player;

	// used for client / server update abstraction
	protected ref Timer m_ClientUpdate = new Timer(CALL_CATEGORY_SYSTEM);
	protected ref Timer m_ServerUpdate = new Timer(CALL_CATEGORY_SYSTEM);
	
	// used for Phase Time Remaining, always static at 1 second
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
	protected void InitPhaseClient(float phase_time, Param data);
	protected void MidPhaseClient(float phase_time, Param data);
	protected void EndPhaseClient(float phase_time, Param data);
	
	protected void InitPhaseServer();
	protected void MidPhaseServer();
	protected void EndPhaseServer();

	protected void OnEventEndClient();
	protected void OnEventEndServer();
	
	// Update Loops
	protected void UpdateClient();
	protected void UpdateServer();
	
	SerializableParam GetClientSyncData(EventPhase phase);
	
	void OnRPC(PlayerIdentity sender, Object target, int rpc_type, ParamsReadContext ctx);
	
	float GetCurrentPhaseLength()
	{
		return GetPhaseLength(m_EventPhase);
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
	
	EventPhase GetCurrentPhase()
	{
		return m_EventPhase;
	}
		
	void SwitchPhase(EventPhase phase, float time_remaining = 0, Param client_data = null)
	{
		if (m_EventPhase >= phase) {
			EventDebug("Event was already in phase %1, exiting...", phase.ToString());
			return;
		}
		
		m_EventPhase = phase;
		EventDebug("SwitchPhase %1", typename.EnumToString(EventPhase, m_EventPhase));
		
		if (GetGame().IsServer()) {		
			m_PhaseTimeRemaining = GetPhaseLength(phase);
			
#ifdef EVENT_MANAGER_DEBUG
			m_PhaseTimeRemaining *= 0.05;
#endif
			
			// Dispatch data to all clients
			EventManager.SendActiveEventData(Type(), GetID(), m_EventPhase, m_PhaseTimeRemaining, m_IsPaused, GetClientSyncData(m_EventPhase));
			
			if (!m_EventManager) {
				EventInfo("SwitchPhase could not find event manager");
				return;
			}
			
			switch (m_EventPhase) {
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
			switch (m_EventPhase) {
				case EventPhase.INIT: {
					thread InitPhaseClient(time_remaining, client_data);
					break;
				}
				
				case EventPhase.MID: {
					thread MidPhaseClient(time_remaining, client_data);
					break;
				}
				
				case EventPhase.END: {
					thread EndPhaseClient(time_remaining, client_data);
					break;
				}
				
				case EventPhase.DELETE:
				default: {
					OnEventEndClient();
					break;
				}
			}
		}
		
		// This was in Start() but it was not being called on clients so i moved it here
		// there may be some better ways to do this, if so, acquire cookie
		if (!m_TimeRemainingTimer.IsRunning()) {
			m_TimeRemainingTimer.Run(PHASE_TIME_REMAINING_PRECISION, this, "UpdateTimeRemaining", null, true);
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
	
	// can multiple of the same event type run?
	// when false, EventID will always be ZERO
	bool AllowParallelEvents()
	{
		return false;
	}
	
	vector GetEventPosition();
	
	/* 
		When a player joins midway through an event, should the Event Manager play 'catch-up' and run all 
		of the previously set phases? Or just the phase that it is currently active
		
		i.e. If the server is on Mid Phase and a player joins:
			if true, the client will run ClientInitPhase AND ClientMidPhase
			if false, the client will ONLY run ClientMidPhase
	*/
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
	float GetEventCooldown()
	{
		return 0;
	}
		
	// Do not call this, let the EventManager do it
	void Start(EventManager event_manager, Param start_params)
	{
		m_EventManager = event_manager;
		m_StartParams = start_params;
		
		SwitchPhase(EventPhase.INIT);
	}
	
	void SetPaused(bool state)
	{
		if (!GetGame().IsServer()) {
			EventDebug("SetPaused can only be called on server!");
			return;
		}
		
		m_IsPaused = state;
		EventManager.SendEventPauseData(Type(), GetID(), m_IsPaused);
	}
	
	bool IsPaused()
	{
		return m_IsPaused;
	}
	
	// It will immediately destroy the Event if one of these is already running
	TTypenameArray GetDisallowedEvents()
	{
		return {};
	}
		
	protected void UpdateTimeRemaining()
	{
		// Dont try to decrease value if paused
		if (IsPaused()) {
			return;
		}
		
		m_PhaseTimeRemaining -= PHASE_TIME_REMAINING_PRECISION;
		if (m_PhaseTimeRemaining <= 0) {
			m_PhaseTimeRemaining = 0;
			
			if (GetGame().IsServer()) {
				EventDebug("Attempting to naturally switch to the next phase");
				SwitchPhase(GetCurrentPhase() + 1);
				
				if (GetCurrentPhase() >= EventPhase.DELETE) {
					m_TimeRemainingTimer.Stop();
					
					// not calling this a whole cycle later causes some crashes
					// perhaps find a way to resolve?
					GetGame().GetCallQueue(CALL_CATEGORY_SYSTEM).CallLater(m_EventManager.DeleteEvent, PHASE_TIME_REMAINING_PRECISION * 1000, false, Type());
				}
			}
		}
	}
	
	void EventDebug(string msg, string param1 = "", string param2 = "", string param3 = "", string param4 = "", string param5 = "", string param6 = "", string param7 = "", string param8 = "", string param9 = "")
	{
#ifdef EVENT_MANAGER_DEBUG		
		PrintFormat("[DF][" + Type() + "]: " + msg, param1, param2, param3, param4, param5, param6, param7, param8, param9);
#endif
	}
	
	void EventInfo(string msg, string param1 = "", string param2 = "", string param3 = "", string param4 = "", string param5 = "", string param6 = "", string param7 = "", string param8 = "", string param9 = "")
	{
		PrintFormat("[DF][" + Type() + "]: " + msg, param1, param2, param3, param4, param5, param6, param7, param8, param9);
	}
	
	void SetID(int id)
	{
		EventDebug("[%1] assigned id: %2", Type().ToString(), id.ToString());
		m_Id = id;
	}
	
	int GetID()
	{
		return m_Id;
	}
}