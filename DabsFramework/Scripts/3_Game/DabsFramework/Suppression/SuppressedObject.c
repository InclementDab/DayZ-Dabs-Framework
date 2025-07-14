class SuppressedObject: SerializableBase
{
	protected ref OLinkT m_Object;
	protected int m_Flags, m_Events;
	protected vector m_Transform[4];
	
	Object GetObject()
	{
		return m_Object.Ptr();
	}
		
	void SuppressedObject(notnull Object object)
	{
		m_Object = new OLinkT(object);
		
		m_Flags = object.GetFlags();
		m_Events = object.GetEventMask();
		
		vector transform[4];
		object.GetTransform(transform);
		copyarray(m_Transform, transform);
		
		object.SetScale(0);
		
		object.ClearFlags(m_Flags, true);
		object.ClearEventMask(m_Events);
		object.SetEventMask(EntityEvent.NOTVISIBLE);
		object.Update();
		
		g_Script.CallFunction(object, "DF_OnDeletedFromWorld", null, null);
		
#ifdef SERVER
		GetGame().UpdatePathgraphRegionByObject(object);
#endif
	}
	
	void ~SuppressedObject()
	{
		Object object = GetObject();
		if (!object) {
			return;
		}
		
		object.SetFlags(m_Flags, true);
		object.SetEventMask(m_Events);
		object.SetTransform(m_Transform);
		object.Update();	
			
#ifdef SERVER
		GetGame().UpdatePathgraphRegionByObject(object);
#endif
		
		// memory leak otherwise
		//delete m_Object;
	}
}