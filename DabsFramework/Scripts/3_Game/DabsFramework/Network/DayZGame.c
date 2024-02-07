modded class DayZGame
{
	static const int RPC_REQUEST_SYNC = 54364;
	static const int RPC_NODE_SYNC = 54365;
		
	override bool OnInitialize()
	{
		if (!super.OnInitialize()) {
			return false;
		}
		
		Node.Root.AfterGameLoad();
		return true;
	}
	
	override void OnRPC(PlayerIdentity sender, Object target, int rpc_type, ParamsReadContext ctx)
	{				
		switch (rpc_type) {			
			case RPC_REQUEST_SYNC: {
				Node.Root.Synchronize(sender);
				break;
			}
				
			case RPC_NODE_SYNC: {	
				int tree_depth;
				if (!ctx.Read(tree_depth)) {
					break;
				}

				Node current = Node.Root;
				for (int i = 0; i < tree_depth; i++) {
					string uuid;
					ctx.Read(uuid);
					
					string type;
					ctx.Read(type);
					
					Node node = current[uuid];
					if (!node) {
						node = Node.Cast(type.ToType().Spawn());
						if (!node) {
							Error(string.Format("Invalid node type %1", type));
							continue;
						}
						
						current[uuid] = node;
						node.Parent = current[uuid];
					}
					
					current = current[uuid];
				}
								
				current.Read(ctx, 0);
				
				// Who do we sync back to?
				if (GetGame().IsDedicatedServer()) {
					array<PlayerIdentity> identities = {};
					GetGame().GetPlayerIndentities(identities);
					foreach (PlayerIdentity identity: identities) {
						// The client that sent the original RPC will not recieve it back - they are the most recent commit
						if (sender.GetId() != identity.GetId()) {
							current.Synchronize(identity);
						}
					}	
				}
								
				current.OnSynchronized();
				break;
			}
		}
		
		super.OnRPC(sender, target, rpc_type, ctx);
	}
}