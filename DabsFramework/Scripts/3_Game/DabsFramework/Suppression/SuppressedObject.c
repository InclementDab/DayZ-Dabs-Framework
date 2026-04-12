class SuppressedObject: SerializableBase
{
	protected ref OLinkT m_Object;
	protected int m_Flags, m_Events;
	protected vector m_Transform[4];
	protected bool m_UpdatedPathgraph;
	protected int m_PhysicsMask;
	
	Object GetObject()
	{
		return m_Object.Ptr();
	}
		
	void SuppressedObject(notnull Object object, bool update_pathgraph = true)
	{
		m_Object = new OLinkT(object);
		m_UpdatedPathgraph = update_pathgraph && object.CanAffectPathgraph() && g_Game.IsServer();
		m_PhysicsMask = dBodyGetInteractionLayer(object);
		
		m_Flags = object.GetFlags();
		m_Events = object.GetEventMask();
		
		vector transform[4];
		object.GetTransform(transform);
		copyarray(m_Transform, transform);
				
		//object.SetPosition(Vector(0, -100, 0) + object.GetPosition());
		object.SetScale(0.000001);		
		object.ClearFlags(m_Flags, true);
		object.ClearEventMask(m_Events);
		object.SetEventMask(EntityEvent.NOTVISIBLE);
		
		/*transform[0] = transform[0] * 0.00001;
		transform[2] = transform[1] * 0.00001;
		transform[1] = transform[2] * 0.00001;
		transform[3] = transform[3] - Vector(0, 1000, 0);
		object.SetTransform(transform);*/
		
		object.Update();
		
		// for now, this might be kinda fucked because objects that are un-deleted may not get their physics back.
		// but its required since calling SetScale(0) will absolutely destroy the heap
		// and calling SetScale(0.000001) leaves a small piece of geometry for you to step on.
		// and calling SetTransform() or SetPosition() leaves a client ghost
		dBodyDestroy(object);
		
		g_Script.CallFunction(object, "DF_OnDeletedFromWorld", null, null);
		
		if (m_UpdatedPathgraph) {
            vector clip_info[2];
            float radius = object.ClippingInfo(clip_info);
            vector radius_vector = Vector(radius, radius, radius);
            g_Game.UpdatePathgraphRegion(m_Transform[3] - radius_vector, m_Transform[3] + radius_vector);
        }
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
		// todo: crashes. un-deleted objects dont get their physics back
		//object.CreateDynamicPhysics(m_PhysicsMask);
		object.Update();	
		
		if (g_Game && m_UpdatedPathgraph) {
			g_Game.UpdatePathgraphRegionByObject(object);
		}
		
		// memory leak otherwise
		//delete m_Object;
	}
}