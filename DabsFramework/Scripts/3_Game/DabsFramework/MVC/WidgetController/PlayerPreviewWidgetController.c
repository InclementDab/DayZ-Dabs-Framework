class PlayerPreviewWidgetController : WidgetControllerTemplate<PlayerPreviewWidget>
{
    override void Set(TypeConverter type_converter)
    {
        DayZPlayer player;
        if (Class.CastTo(player, type_converter.GetObject())) {
            m_Widget.SetPlayer(player);
            m_Widget.SetModelPosition(vector.Zero);
        }
    }

    override void Get(out TypeConverter type_converter)
    {
        type_converter.Set(m_Widget.GetDummyPlayer());
    }
	
	override void SetSelection(TypeConverter type_converter)
	{
		m_Widget.UpdateItemInHands(EntityAI.Cast(type_converter.GetObject()));
	}

	override void GetSelection(out TypeConverter type_converter)
	{
		DayZPlayer player;
		if (Class.CastTo(player, m_Widget.GetDummyPlayer())) {
			type_converter.SetObject(player.GetHumanInventory().GetEntityInHands());
		} 
	}
}