typedef map<typename, ref EventBase> TEventMap;

/*
*
*	EventManager.c
*
*	Primary Event Manager
*
*	You are free to mod this as you please. 
*	
*	That being said you are NOT free to redistribute / repack this into
*	your own mod. 
*
*/

class EventManager
{			
	// Enable / Disable the multiple event system
	protected int m_MaxEventCount, m_EventFreqMin, m_EventFreqMax;
	protected float m_NextEventIn;
	protected typename m_LastEventType;
	
	protected ref map<typename, float> m_PossibleEventTypes = new map<typename, float>();
	protected ref TEventMap m_ActiveEvents = new TEventMap();

	protected ref Timer m_ServerEventTimer = new Timer(CALL_CATEGORY_GAMEPLAY);
	protected ref Timer m_EventCooldownTimer = new Timer(CALL_CATEGORY_GAMEPLAY);
	protected ref map<typename, int> m_EventCooldowns = new map<typename, int>();
	
	protected static ref EventManager m_Instance;
	
	static EventManager Start()
	{
		m_Instance = new EventManager();
		return m_Instance;
	}
	
	static void Stop()
	{
		delete m_Instance;
	}
	
	static EventManager GetInstance()
	{
		return m_Instance;
	}
	
	void EventManager()
	{		
		DayZGame.Event_OnRPC.Insert(OnRPC);
	}
	
	void ~EventManager()
	{
		delete m_ActiveEvents;
		delete m_PossibleEventTypes;
		delete m_EventCooldownTimer;
		delete m_EventCooldowns;
		delete m_ServerEventTimer;
	}

	/*
		Run this in your init.c
	
		0 (int): Minimum time between events
		1 (int): Maximum time between events
		2 (int): Maximum amount of parallel events
	*/
	void Run(int min_between_events = 550, int max_between_events = 3500, int max_event_count = 2)
	{
		PrintToRPT("Initializing Event Manager");
		
		m_MaxEventCount = max_event_count;
		m_EventFreqMin = min_between_events;
		m_EventFreqMax = max_between_events;
		
#ifdef EVENT_MANAGER_DEBUG
		//m_EventFreqMin *= 0.05;
		//m_EventFreqMax *= 0.05;
#endif
		
		m_EventCooldownTimer.Run(1.0, this, "ServerCooldownThread", null, true);
		
		m_NextEventIn = GetNextEventTime();
		EventManagerInfo("Next selection will occur in %1 seconds", m_NextEventIn.ToString());	
		m_ServerEventTimer.Run(m_NextEventIn, this, "ServerEventThread");
		
		EventManagerInfo("EventManager is now running");
	}
	
	protected void ServerCooldownThread()
	{
		foreach (typename event_type, int event_cooldown: m_EventCooldowns) {
			m_EventCooldowns[event_type] = event_cooldown - 1;
			if (event_cooldown <= 0) {
				m_EventCooldowns.Remove(event_type);
			}
		}
	}
			
	protected void ServerEventThread()
	{	
		EventManagerInfo("Trying to select a new event...");												
		// Just a quick check to make sure we dont run the same event twice
		typename current_type = GetRandomEvent();
		while (current_type == m_LastEventType && m_PossibleEventTypes.Count() > 1) {
			EventManagerDebug("Random event selected was the same as last %1", current_type.ToString());
			current_type = GetRandomEvent();
		}
		
		m_LastEventType = current_type;
		
		//! Start new event
		StartEvent(current_type);
		
		m_NextEventIn = GetNextEventTime();
		EventManagerInfo("Next selection will occur in %1 seconds", m_NextEventIn.ToString());
		
		m_ServerEventTimer.Run(m_NextEventIn, this, "ServerEventThread");
	}
	
	void RegisterEvent(typename event_type, float frequency = 1.0)
	{
		if (frequency <= 0) {
			return;
		}
		
		m_PossibleEventTypes.Insert(event_type, frequency);
	}

	void StartEvent(typename event_type, bool force = false)
	{
		if (!GetGame().IsServer()) {
			EventManagerInfo("StartEvent must be called on SERVER, exiting");
			return;
		}
		
		if (m_ActiveEvents[event_type]) { 
			EventManagerInfo("Could not start %1 as one is already active", event_type.ToString());
			return;
		}
				
		if (m_ActiveEvents.Count() >= m_MaxEventCount && !force) {
			EventManagerInfo("Could not start event as we reached the maximum event limit %1", m_MaxEventCount.ToString());
			return;
		}
		
		if (m_EventCooldowns.Contains(event_type) && !force) {
			EventManagerInfo("Could not start event %1 as it is on cooldown for %2 more seconds", event_type.ToString(), m_EventCooldowns[event_type].ToString());
			return;
		}
		
		EventManagerInfo("Starting event %1", event_type.ToString());
		m_ActiveEvents[event_type] = SpawnEvent(event_type);
		
		if (!m_ActiveEvents[event_type]) {
			EventManagerInfo("Failed to start event %1", event_type.ToString());
			return;
		}
		
		foreach (typename etype, EventBase ebase: m_ActiveEvents) {
			if (m_ActiveEvents[event_type].GetDisallowedEvents().Find(etype) != -1 && !force) {
				EventManagerInfo("Could not run event %1 because it conflicts with event %2...", event_type.ToString(), etype.ToString());
				DeleteEvent(event_type);
				return;
			}
		}
		
		if (!m_ActiveEvents[event_type] || !m_ActiveEvents[event_type].EventActivateCondition()) {
			DeleteEvent(event_type);
			return;
		}
		
		// Register event for cooldown
		m_EventCooldowns.Insert(event_type, m_ActiveEvents[event_type].GetEventCooldown());
		
		// start the event
		m_ActiveEvents[event_type].Start(this);
	}
	
	void CancelEvent(typename event_type)
	{
		if (!GetGame().IsServer()) {
			EventManagerInfo("CancelEvent must be called on SERVER, exiting");
			return;
		}
		
		if (!m_ActiveEvents[event_type]) {
			EventManagerInfo("Event %1 is not active", event_type.ToString());
			return;
		}
		
		SendActiveEventData(event_type, EventPhase.DELETE, 0); // 3 is cancel for clients
		DeleteEvent(event_type);
	}
	
	void DeleteEvent(typename event_type)
	{
		if (!m_ActiveEvents) {
			return;
		}
		
		delete m_ActiveEvents[event_type];
		m_ActiveEvents.Remove(event_type);
	}
	
	void OnRPC(PlayerIdentity sender, Object target, int rpc_type, ParamsReadContext ctx)
	{	
		switch (rpc_type) {
			
			case ERPCsDabsFramework.EVENT_MANAGER_UPDATE: {
				
				EventManagerUpdateParams event_update_params;
				if (!ctx.Read(event_update_params)) {
					break;
				}
								
				if (GetGame().IsClient() || !GetGame().IsMultiplayer()) {
					
					EventManagerInfo("Client received event manager update %1: %2", event_update_params.param1, event_update_params.param2.ToString());
					typename event_type = event_update_params.param1.ToType();
					int event_phase = event_update_params.param2;
					float event_phase_time = event_update_params.param3;
					
					// Case for JIP players	
					if (!m_ActiveEvents[event_type]) {
						m_ActiveEvents[event_type] = SpawnEvent(event_type);
						if (!m_ActiveEvents[event_type]) return;
					}						
					
					// Event finished
					if (event_phase == EventPhase.DELETE) {
						DeleteEvent(event_type);
						break;
					}
					
					// Play catch-up to the current phase
					if (m_ActiveEvents[event_type].JIPRunPreviousPhases()) {
						for (int i = m_ActiveEvents[event_type].GetActivePhaseID(); i < event_phase; i++) {
							m_ActiveEvents[event_type].SwitchPhase(i);
						}					
					}
					
					m_ActiveEvents[event_type].SwitchPhase(event_phase, event_phase_time);
				}
				
				break;
			}				
		}
		
		foreach (typename et, EventBase event_base: m_ActiveEvents) {
			if (event_base) {
				event_base.OnRPC(sender, target, rpc_type, ctx);
			}
		}
	}
	
	void DispatchEventInfo(PlayerBase player)
	{
		EventManagerDebug("Sending In Progress info to %1", player.ToString());
		foreach (typename event_type, EventBase event_base: m_ActiveEvents) {
			if (event_base) {
				GetGame().RPCSingleParam(player, ERPCsDabsFramework.EVENT_MANAGER_UPDATE, new EventManagerUpdateParams(event_type.ToString(), event_base.GetActivePhaseID(), event_base.GetCurrentPhaseTimeRemaining()), true, player.GetIdentity());
			}
		}
	}
	
	static void SendActiveEventData(typename event_type, EventPhase phase_id, float time_remaining)
	{
		EventManagerDebug("Sending active Event Data: %1, Phase: %2", event_type.ToString(), typename.EnumToString(EventPhase, phase_id));
		GetGame().RPCSingleParam(null, ERPCsDabsFramework.EVENT_MANAGER_UPDATE, new EventManagerUpdateParams(event_type.ToString(), phase_id, time_remaining), true, null);
	}
	
	bool IsEventActive(typename event_type)
	{
		return m_ActiveEvents[event_type] != null;
	}
	
	EventBase SpawnEvent(typename event_type)
	{
		if (!event_type.IsInherited(EventBase))
			return null;
		
		return EventBase.Cast(event_type.Spawn());
	}
		
	typename GetRandomEvent()
	{
		if (m_PossibleEventTypes.Count() == 0) return typename;
		
		array<typename> possible_types = {};
		foreach (typename type, float freq: m_PossibleEventTypes) {
			for (int i = 0; i < freq * 100; i++) {
				possible_types.Insert(type);
			}
		}
				
		return possible_types.GetRandomElement();
	}
	
	float GetNextEventTime()
	{
		return Math.Clamp(Math.RandomInt(m_EventFreqMin, m_EventFreqMax), 10, int.MAX);
	}
	
	EventBase GetEvent(typename event_type)
	{
		return m_ActiveEvents[event_type];
	}
	
	array<EventBase> GetActiveEvents()
	{
		return m_ActiveEvents.GetValueArray();
	}
	
	void DumpInfo()
	{
		if (m_PossibleEventTypes.Count() == 0) {
			EventManagerInfo("Cannot debug Event Percentages with no events registered");
			return;
		}
		
		EventManagerInfo("New events will be selected between %1 and %2 minutes", (m_EventFreqMin / 60).ToString(), (m_EventFreqMax / 60).ToString());
		EventManagerInfo("There is a maximum event count of %1", m_MaxEventCount.ToString());
		float total_freq;
		foreach (typename typet, float freqt: m_PossibleEventTypes) {
			total_freq += freqt;
		}
		
		foreach (typename type, float freq: m_PossibleEventTypes) {
			float value = ((1 / total_freq) * freq) * 100;
			EventManagerInfo("Chance of %1 is %2 percent", type.ToString(), value.ToString());
		}
		
		EventManagerInfo("There are %1 events running", m_ActiveEvents.Count().ToString());
		
		foreach (typename typey, EventBase evnt: m_ActiveEvents) {
			EventManagerInfo("Event %1 is running in phase %2 with %3 seconds remaining", evnt.ToString(), evnt.GetActivePhaseID().ToString(), evnt.GetCurrentPhaseTimeRemaining().ToString());
		}
	}
			
	static void EventManagerDebug(string msg, string param1 = "", string param2 = "", string param3 = "", string param4 = "", string param5 = "", string param6 = "", string param7 = "", string param8 = "", string param9 = "")
	{
#ifdef EVENT_MANAGER_DEBUG		
		PrintFormat("[DF][EventManager]: " + msg, param1, param2, param3, param4, param5, param6, param7, param8, param9);
#endif
	}
	
	static void EventManagerInfo(string msg, string param1 = "", string param2 = "", string param3 = "", string param4 = "", string param5 = "", string param6 = "", string param7 = "", string param8 = "", string param9 = "")
	{
		PrintFormat("[DF][EventManager]: " + msg, param1, param2, param3, param4, param5, param6, param7, param8, param9);
	}
};