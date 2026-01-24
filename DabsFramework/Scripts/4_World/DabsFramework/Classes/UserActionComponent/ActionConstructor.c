modded class ActionConstructor
{
	override void RegisterActions(TTypenameArray actions)
	{
		super.RegisterActions(actions);
		
		foreach (int hash, typename action: RegisterAction.RegisteredTypes) {
			actions.Insert(action);
		}
	}
}