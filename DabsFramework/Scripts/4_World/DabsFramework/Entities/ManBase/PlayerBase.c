modded class PlayerBase
{
	// This is a fix for the 'InsertAll' error that exists in some mods with incorrectly configured actions
	override void GetActions(typename action_input_type, out array<ActionBase_Basic> actions)
	{
		if (!m_ActionsInitialize)
		{
			m_ActionsInitialize = true;
			InitializeActions();
		}

		// Backwards compatibility
		array<ActionBase_Basic> bcActions = m_InputActionMap.Get(action_input_type);
		if (!bcActions)
		{
			if (IsControlledPlayer())
				actions = m_InputActionMapControled.Get(action_input_type);
			else
				actions = m_InputActionMapAsTarget.Get(action_input_type);
		}
		else
		{
			if (!actions)
				actions = new array<ActionBase_Basic>();
			
			if (IsControlledPlayer()) {
				if (m_InputActionMapControled[action_input_type]) {
					actions.InsertAll(m_InputActionMapControled[action_input_type]);
				}
			} else {
				if (m_InputActionMapAsTarget[action_input_type]) {
					actions.InsertAll(m_InputActionMapAsTarget[action_input_type]);
				}
			}
			
			actions.InsertAll(bcActions);
		}
	}
}