class NodeState: int
{	
	static const NodeState EMPTY 	= 0x0;
	static const NodeState HOVER 	= 0x01;
	static const NodeState SELECT 	= 0x02;
	static const NodeState CONTEXT 	= 0x04;
	static const NodeState DRAG 	= 0x08;
	static const NodeState SUPPRESS = 0x10;
	static const NodeState EXTEND 	= 0x20;
	
	static NodeState GetNetworkMask()
	{
		return NodeState.SELECT | NodeState.SUPPRESS;
	}
	
	bool IsEmpty()
	{
		return !value;
	}
	
	bool IsHover()
	{
		return (value & HOVER) == HOVER;
	}
	
	bool IsActive()
	{
		return (value & SELECT) == SELECT;
	}
	
	bool IsContext()
	{
		return (value & CONTEXT) == CONTEXT;
	}	
	
	bool IsDrag()
	{
		return (value & DRAG) == DRAG;
	}
		
	bool IsSuppress()
	{
		return (value & SUPPRESS) == SUPPRESS;
	}
	
	bool IsExtend()
	{
		return (value & EXTEND) == EXTEND;
	}
}

typedef int NodeState;