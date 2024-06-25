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
	
	override bool EmoteCondition(int stancemask)
	{
		vector head_pos;
		MiscGameplayFunctions.GetHeadBonePos(m_Player, head_pos);
		// checking for a wall in front
		Ray ray = new Ray(head_pos, m_Player.GetDirection());
		Raycast ray_cast = ray.PerformRaycastRV(m_Player, null, 0, 1.0);
		if (ray_cast) {
			return false;
		}
		
		return super.EmoteCondition(stancemask);
	}
}