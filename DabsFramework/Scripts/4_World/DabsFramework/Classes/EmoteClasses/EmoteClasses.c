modded class EmoteDabbing
{
	void EmoteDabbing()
	{
		m_ID = EmoteConstants.ID_EMOTE_DABBING;
		m_InputActionName = "EmoteDabbing";
		m_StanceMaskAdditive = DayZPlayerConstants.STANCEMASK_CROUCH | DayZPlayerConstants.STANCEMASK_ERECT;
		m_AdditiveCallbackUID = DayZPlayerConstants.CMD_GESTUREMOD_DABBING;
		m_HideItemInHands = true;
	}
}