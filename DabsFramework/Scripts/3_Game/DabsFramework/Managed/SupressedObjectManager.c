class SuppressedObjectManager: Managed
{
	static const int RPC_SUPPRESS = -135295235;
	static const int RPC_UNSUPPRESS = -135295236;

	protected ref array<ref SuppressedObject> m_Objects = {};
	
	void SuppressedObjectManager()
	{
		DayZGame.Event_OnRPC.Insert(OnRPC);
	}
	
	void DispatchInfo(PlayerIdentity identity = null)
	{
#ifdef SERVER		
		ScriptRPC rpc = new ScriptRPC();
		rpc.Write(m_Objects.Count());
		foreach (SuppressedObject object: m_Objects) {
			rpc.Write(object.GetObject());
		}
		
		rpc.Send(null, RPC_SUPPRESS, true);
#endif
	}
		
	void Suppress(notnull Object object)
	{
#ifdef SERVER
		SuppressMany({ object });
#endif
	}
	
	void SuppressMany(notnull array<Object> objects)
	{
#ifdef SERVER
		array<Object> suppressible_objects = {};
		foreach (Object object: objects) {
			if (!IsSuppressible(object)) {
				return;
			}
				
			suppressible_objects.Insert(object);
		}
		
		ScriptRPC rpc = new ScriptRPC();
		rpc.Write(suppressible_objects.Count());
		for (int i = 0; i < suppressible_objects.Count(); i++) {
			rpc.Write(suppressible_objects[i]);
			
			m_Objects.Insert(new SuppressedObject(suppressible_objects[i]));
		}
		
		rpc.Send(null, RPC_SUPPRESS, true);
#endif
	}
	
	void Unsupress(notnull Object object)
	{
#ifdef SERVER		
		ScriptRPC rpc = new ScriptRPC();
		rpc.Write(object);
		rpc.Send(null, RPC_UNSUPPRESS, true);
		
		foreach (auto suppressed_object: m_Objects) {
			if (suppressed_object.GetObject() == object) {
				m_Objects.RemoveItem(suppressed_object);
			}
		}
#endif
	}
	
	bool IsSuppressed(notnull Object object)
	{
		foreach (auto suppressed_object: m_Objects) {
			if (suppressed_object.GetObject() == object) {
				return true;
			}
		}
		
		return false;
	}
	
	static bool IsSuppressible(notnull Object object)
	{
		if (!object) {
			return false;
		}
		
		if (object.IsKindOf("House") || object.IsTree() || object.IsBush() || object.IsRock()) {
			return true;
		}

		return (object.GetType() == string.Empty) && (object.Type() == Object);
	}
	
	void OnRPC(PlayerIdentity sender, Object target, int rpc_type, ParamsReadContext ctx)
	{	
#ifndef SERVER
		switch (rpc_type) {
			case RPC_SUPPRESS: {				
				Object suppress;
				if (!ctx.Read(suppress)) {
					Error("Invalid object found to suppress");
					break;
				}
											
				m_Objects.Insert(new SuppressedObject(suppress));
				break;
			}
			
			case RPC_UNSUPPRESS: {
				Object unsuppress;
				if (!ctx.Read(unsuppress)) {
					Error("Invalid object found to unsuppress");
					break;
				}
				
				foreach (auto suppressed_object: m_Objects) {
					if (suppressed_object.GetObject() == unsuppress) {
						m_Objects.RemoveItem(suppressed_object);
					}
				}
				
				break;
			}
		}
#endif
	}
}

/*	
	void RestoreMapObject(notnull ObjectRemoverLink object_link)
	{
		Object object = m_HiddenObjects.GetKeyByValue(object_link);
		if (!object) {
			return;
		}
		
		RestoreMapObject(object);
	}
	
	void RestoreMapObject(notnull Object object)
	{
		ObjectRemoverLink object_link = m_HiddenObjects[object];
		if (!object_link) {
			return;
		}
		
		object.SetFlags(object_link.Flags, true);
		object.SetEventMask(object_link.Events);
		Print(object_link.Transform);
		object.SetTransform(object_link.Transform);
		object.Update();
		delete m_HiddenObjects[object];
		
		if (GetGame().IsDedicatedServer()) {
			GetGame().UpdatePathgraphRegionByObject(object);
		}
	}
	
	bool IsHiddenObject(notnull Object object)
	{
		return m_HiddenObjects[object] != null;
	}*/