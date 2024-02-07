class NodeStateMachine: map<NodeState, ref array<Node>>
{
	void NodeStateMachine()
	{
		array<NodeState> all = { NodeState.HOVER, NodeState.SELECT, NodeState.CONTEXT, NodeState.DRAG, NodeState.SUPPRESS, NodeState.EXTEND };
		foreach (NodeState state: all) {
			this[state] = {};
		}
	}
}