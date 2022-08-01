modded class ActionConstructor
{
	override void RegisterActions(TTypenameArray actions)
	{
		super.RegisterActions(actions);
		
		foreach (typename action: RegisterAction.RegisteredTypes) {
			actions.Insert(action);
		}
	}
}