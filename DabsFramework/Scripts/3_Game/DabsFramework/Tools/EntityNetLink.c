class EntityNetLink: Managed
{
	protected EntityAI m_Source, m_Value;
	protected int m_Low, m_High;
	
	void EntityNetLink(notnull EntityAI source, string var_name)
	{
		m_Source = source;
		
		//@ We need the value to be assigned before our construction finished
		EnScript.SetClassVar(source, var_name, 0, this);
		
		//@ Storing the network values
		m_Source.RegisterNetSyncVariableInt(string.Format("%1.m_Low", var_name));
		m_Source.RegisterNetSyncVariableInt(string.Format("%1.m_High", var_name));
	}
	
	//@ Sets the value of the linked entity, allows for instances of items to be created and linked
	void Set(notnull EntityAI value)
	{
		if (GetGame() && !GetGame().IsServer() || !m_Source) {
			return;
		}
		
		m_Value = value;
		m_Value.GetNetworkID(m_Low, m_High);
		
		m_Source.SetSynchDirty();
	}
	
	EntityAI Get()
	{
		if (!m_Value) {
			m_Value = EntityAI.Cast(GetDayZGame().GetObjectByNetworkId(m_Low, m_High));
		}
		
		return m_Value;
	}
}