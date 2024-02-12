class Node: SerializableBase
{
	static ref NodeStateMachine States = new NodeStateMachine();
	
	// Initialize States -> Root in order
	static ref Node Root;
	
	// Only thing our constructor will ever know
	protected UUID m_UUID;
	
	// Serialized data for each node (optional)
	string DisplayName;
	Symbols Icon;
	LinearColor Color;
	
	protected NodeState m_NodeState;
		
	// Assigned by Get and Set of this class
	ref map<string, ref Node> Children = new map<string, ref Node>();
	
	ref array<NodeView> Views = {};
	
	// Assigned by Set() of parent
	Node Parent;
	
	// Same parameters as OnStateChanged
	ref ScriptInvoker State_OnChanged = new ScriptInvoker();
		
	// This constructor has default variables on purpose. it must have a parameterless alternative, and preperations. used for deserialization
	void Node(UUID uuid)
	{
		m_UUID = uuid;
	}
	
	void ~Node()
	{
		foreach (NodeView view: Views) {
			delete view;
		}
	}

	Node Find(UUID uuid)
	{
		if (Children[uuid]) {
			return Children[uuid];
		}
		
		foreach (Node child: Children) {
			Node found = child.Find(uuid);
			if (found) {
				return found;
			}
		}
		
		return null;
	}
		
	void FindMany(typename type, inout notnull array<Node> elements)
	{
		foreach (Node child: Children) {
			if (child.IsInherited(type)) {
				elements.Insert(child);
			}
			
			child.FindMany(type, elements);
		}
	}
	
	void AddState(NodeState state)
	{
		//PrintFormat("[%1] %2, add=%3", m_UUID, DisplayName, typename.EnumToString(NodeState, state));		
		States[state].Insert(this);		
		m_NodeState |= state;
		OnStateChanged(state, true);
	}
	
	void RemoveState(NodeState state)
	{		
		//PrintFormat("[%1] %2, remove=%3", m_UUID, DisplayName, typename.EnumToString(NodeState, state));
		States[state].RemoveItem(this);
		m_NodeState &= ~state;
		OnStateChanged(state, false);
	}
		
	void OnStateChanged(NodeState node_state, bool state)
	{
		State_OnChanged.Invoke(node_state, state);
	}
				
	void Synchronize(PlayerIdentity identity = null)
	{	
		ScriptRPC rpc = new ScriptRPC();
		int tree_depth = GetParentDepth();
		rpc.Write(tree_depth);

		for (int i = tree_depth - 1; i >= 0; i--) {
			Node parent = GetParentAtDepth(i); 
			rpc.Write(parent.GetUUID());
			rpc.Write(parent.Type().ToString());
		}
			
		Write(rpc, 0);
		rpc.Send(null, DayZGame.RPC_NODE_SYNC, true, identity);
	}
	
	bool Contains(UUID uuid)
	{
		array<string> full_path = {};
		uuid.Split(Directory.PATH_SEPERATOR, full_path);
		Node node = this;
		for (int i = 0; i < full_path.Count(); i++) {
			if (!node) {
				break;
			}
			
			node = node.Children[uuid];
		}
		
		return node != null;
	}
	
	bool IsAncestor(notnull Node node)
	{		
		Node parent = this;
		while (parent) {
			if (node == parent) {
				return true;
			}
			
			parent = parent.Parent;
		}
		
		return false;
	}
			
	void Add(notnull Node node)
	{
		Set(node.GetUUID(), node);
	}
	
	void Set(string uuid, notnull Node node)
	{
		if (IsAncestor(node)) {
			Error("Cannot a node as its own child!");
			return;
		}
		
		Children[uuid] = node;
		node.Parent = this;
	}
	
	void Remove(notnull Node node)
	{
		Remove(node.GetUUID());
	}
	
	void Remove(UUID uuid)
	{
		Children.Remove(uuid);
	}
	
	Node Get(string uuid)
	{
		if (Children.Contains(uuid)) {
			return Children[uuid];
		}
	
		array<string> full_path = {};
		uuid.Split(Directory.PATH_SEPERATOR, full_path);
		Node node = this;
		for (int i = 0; i < full_path.Count(); i++) {			
			node = node.Children[full_path[i]];
			if (!node) {
				break;
			}
		}
		
		return node;
	}
		
	ScriptReadWriteContext CreateCopy()
	{
		ScriptReadWriteContext ctx = new ScriptReadWriteContext();
		ctx.GetWriteContext().Write(GetFullPath());		
		Write(ctx.GetWriteContext(), 0);
		return ctx;
	}
	
	// {uuid}\\{uuid}
	string GetFullPath()
	{
		string full_path;
		for (int i = GetParentDepth(); i >= 0; i--) {
			Node node = GetParentAtDepth(i);
			full_path += node.GetUUID();
			if (i != 0) {
				full_path += Directory.PATH_SEPERATOR;
			}
		}
		
		return full_path;
	}
			
	void OnSynchronized()
	{
	}
	
	Node FindParentOfType(typename type)
	{
		Node parent = this;
		while (parent) {
			if (parent.IsInherited(type)) {
				return parent;
			}
						
			parent = parent.Parent;
		}
		
		return null;
	}
	
	Node GetParentAtDepth(int depth)
	{
		Node parent = this;
		while (depth > 0) {
			if (!parent.Parent) {
				Error("GetParentAtDepth ran out of depth " + depth);
				return parent;
			}
			
			parent = parent.Parent;
			depth--;
		}
		
		return parent;
	}
	
	int GetParentDepth()
	{
		int depth;
		Node parent = Parent;
		while (parent) {
			depth++;
			parent = parent.Parent;
		}
		
		return depth;
	}
									
	override void Write(Serializer serializer, int version)
	{		
		serializer.Write((string)m_UUID);
		serializer.Write(DisplayName);
		serializer.Write(Icon);
		serializer.Write((int)Color);
		
		serializer.Write(Children.Count());
		foreach (string uuid, Node node: Children) {			
			serializer.Write(node.GetUUID());
			serializer.Write(node.Type().ToString());
			
			node.Write(serializer, version);
		}
		
		super.Write(serializer, version);
	}
	
	override bool Read(Serializer serializer, int version)
	{
		serializer.Read(m_UUID);
		serializer.Read(DisplayName);	
		serializer.Read(Icon);
		serializer.Read(Color);
			
		int count;
		serializer.Read(count);
		for (int i = 0; i < count; i++) {
			string uuid;
			serializer.Read(uuid);
			string type;
			serializer.Read(type);
			Node node = Children[uuid];
			if (!node) {				
				node = Node.Cast(type.ToType().Spawn());
				if (!node) {
					Error("Invalid node type " + type);
					return false;
				}
				
				node.m_UUID = uuid;
				node.Parent = this;
				Children[uuid] = node;
			}
			
			node.Read(serializer, version);
			node.OnSynchronized();
		}
		
		return super.Read(serializer, version);
	}
			
	UUID GetUUID()
	{
		return m_UUID;
	}
			
	bool HasState(NodeState state)
	{
		return (m_NodeState & state) == state;
	}
	
	NodeState GetState()
	{
		return m_NodeState;
	}
	
	static array<Node> FilterNodesByState(NodeState node_state)
	{
		return States[node_state];
	}
	
	static int ClearNodesByState(NodeState node_state)
	{
		array<Node> nodes = {};
		nodes.Copy(States[node_state]);
		foreach (Node node: nodes) {
			if (node) {
				node.RemoveState(node_state);
			}
		}
		
		return nodes.Count();
	}	
	 
								
#ifdef DIAG_DEVELOPER
	void Debug()
	{
		PrintFormat("\b\r\n\n\n[DEBUGGING STATE MACHINE]");
		foreach (NodeState state, array<Node> nodes: States) {
			Print(typename.EnumToString(NodeState, state));
			foreach (Node node: nodes) {
				PrintFormat("\t%1", node.DisplayName);
			}
		}
		
		PrintFormat("\n\n\n[DEBUGGING CHILDREN NODES]");
		this.Debug(0);
	}

	protected void Debug(int depth)
	{		
		string tabs;
		for (int i = 0; i < depth; i++) {
			tabs += "\t";
		}
		
		PrintFormat("%4:%3[%1] %2: State:%5, Ico:%6, Color:%7", m_UUID, DisplayName, tabs, depth, m_NodeState, Icon, Color);
		
		foreach (string uuid, Node node: Children) {
			node.Debug(depth + 1);
		}
	}
#endif
}