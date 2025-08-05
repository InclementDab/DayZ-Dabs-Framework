class SuppressedObjectManager: Managed
{
	static const int RPC_SUPPRESS = -135295235;
	static const int RPC_UNSUPPRESS = -135295236;
	static const int RPC_UNSUPPRESS_ALL = -135295237;

	protected ref array<ref SuppressedObject> m_Objects = {};
	protected ref map<Object, SuppressedObject> m_SuppressedObjectMap = new map<Object, SuppressedObject>();
		
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
		
		rpc.Send(null, RPC_SUPPRESS, true, identity);
#endif
	}
		
	void Suppress(notnull Object object, bool inform_clients = true)
	{
		SuppressMany({ object }, inform_clients);
	}
		
	void SuppressMany(notnull array<Object> objects, bool inform_clients = true)
	{
		ScriptRPC rpc = new ScriptRPC();
		rpc.Write(objects.Count());
		for (int i = 0; i < objects.Count(); i++) {
			if (!IsSuppressible(objects[i])) {
				rpc.Write(null);
				continue;
			}

			Object suppress = objects[i];
			rpc.Write(suppress);
			
			auto suppressed_object = new SuppressedObject(suppress);
			m_Objects.Insert(suppressed_object);
			m_SuppressedObjectMap[suppress] = suppressed_object;
		}
		
#ifdef SERVER				
		if (inform_clients) {
			rpc.Send(null, RPC_SUPPRESS, true);
		}
#endif
	}
		
	void Unsupress(notnull Object object, bool inform_clients = true)
	{
		UnsupressMany({ object });
	}
	
	void UnsupressMany(notnull array<Object> objects, bool inform_clients = true)
	{
		ScriptRPC rpc = new ScriptRPC();
		rpc.Write(objects.Count());
		foreach (Object object: objects) {		
			if (!object) {
				continue;
			}

			rpc.Write(object);
			
			SuppressedObject suppressed_object = m_SuppressedObjectMap[object];
			if (suppressed_object) {
				m_Objects.RemoveItem(suppressed_object);
				m_SuppressedObjectMap.Remove(object);
			}
		}
		
#ifdef SERVER		
		if (inform_clients) {
			rpc.Send(null, RPC_UNSUPPRESS, true);
		}
#endif
	}
	
	void UnsuppressAll(bool inform_clients = true)
	{
		m_Objects.Clear();
		
#ifdef SERVER		
		if (inform_clients) {
			ScriptRPC().Send(null, RPC_UNSUPPRESS_ALL, true);
		}
#endif
	}
	
	bool IsSuppressed(notnull Object object)
	{
		return m_SuppressedObjectMap.Contains(object);
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
				int suppress_count;
				ctx.Read(suppress_count);
				
				for (int i = 0; i < suppress_count; i++) {
					Object suppress;
					if (!ctx.Read(suppress) || !suppress) {
						continue;
					}
										
					if (m_SuppressedObjectMap.Contains(suppress)) {
						continue;
					}
					
					SuppressedObject new_suppressed_object = new SuppressedObject(suppress);
					m_Objects.Insert(new_suppressed_object);
					m_SuppressedObjectMap[suppress] = new_suppressed_object;
				}								
				
				break;
			}
			
			case RPC_UNSUPPRESS: {
				int unsuppress_count;
				ctx.Read(unsuppress_count);
				
				for (int j = 0; j < unsuppress_count; j++) {
					Object unsuppress;
					if (!ctx.Read(unsuppress) || !unsuppress) {
						continue;
					}
					
					SuppressedObject suppressed_object = m_SuppressedObjectMap[unsuppress];
					if (suppressed_object) {
						m_Objects.RemoveItem(suppressed_object);
						m_SuppressedObjectMap.Remove(unsuppress);
					}
				}		
								
				break;
			}
			
			case RPC_UNSUPPRESS_ALL: {
				m_Objects.Clear();
				m_SuppressedObjectMap.Clear();
				break;
			}
		}
#endif
	}
	
	array<ref SuppressedObject> GetObjects()
	{
		return m_Objects;
	}
}