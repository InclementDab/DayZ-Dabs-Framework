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
		
		rpc.Send(null, RPC_SUPPRESS, true, identity);
#endif
	}
		
	void Suppress(notnull Object object)
	{
		SuppressMany({ object });
	}
		
	void SuppressMany(notnull array<Object> objects)
	{

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
			Object suppress = suppressible_objects[i];
			rpc.Write(suppress);
			
			m_Objects.Insert(new SuppressedObject(suppressible_objects[i]));
		}
		
#ifdef SERVER				
		rpc.Send(null, RPC_SUPPRESS, true);
#endif
	}
		
	void Unsupress(notnull Object object)
	{
#ifdef SERVER		
		UnsupressMany({ object });
#endif
	}
	
	void UnsupressMany(notnull array<Object> objects)
	{
#ifdef SERVER
		ScriptRPC rpc = new ScriptRPC();
		rpc.Write(objects.Count());
		foreach (Object object: objects) {
			rpc.Write(object);
			
			// Remove from m_Objects
			foreach (auto suppressed_object: m_Objects) {
				if (suppressed_object.GetObject() == object) {
					m_Objects.RemoveItem(suppressed_object);
				}
			}
		}
		
		rpc.Send(null, RPC_UNSUPPRESS, true);
#endif
	}
	
	void UnsuppressAll()
	{
#ifdef SERVER
		ScriptRPC rpc = new ScriptRPC();
		rpc.Write(m_Objects.Count());
		foreach (auto suppressed_object: m_Objects) {
			rpc.Write(suppressed_object.GetObject());
		}
		
		rpc.Send(null, RPC_UNSUPPRESS, true);
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
				int suppress_count;
				ctx.Read(suppress_count);
				
				for (int i = 0; i < suppress_count; i++) {
					Object suppress;
					if (!ctx.Read(suppress)) {
						continue;
					}

					m_Objects.Insert(new SuppressedObject(suppress));
				}								
				
				break;
			}
			
			case RPC_UNSUPPRESS: {
				int unsuppress_count;
				ctx.Read(unsuppress_count);
				
				for (int j = 0; j < unsuppress_count; j++) {
					Object unsuppress;
					if (!ctx.Read(unsuppress)) {
						continue;
					}
					
					foreach (auto suppressed_object: m_Objects) {
						if (suppressed_object.GetObject() == unsuppress) {
							m_Objects.RemoveItem(suppressed_object);
						}
					}
				}		
								
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