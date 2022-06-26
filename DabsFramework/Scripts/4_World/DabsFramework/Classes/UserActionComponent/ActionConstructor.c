modded class ActionConstructor
{
	override void RegisterActions(TTypenameArray actions)
	{
		foreach (typename action: RegisterAction.RegisteredTypes) {
			Print(action);
			actions.Insert(action);
		}
		
		super.RegisterActions(actions);
	}
}