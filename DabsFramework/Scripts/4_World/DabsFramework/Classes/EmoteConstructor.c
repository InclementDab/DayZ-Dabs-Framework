modded class EmoteConstructor
{
	override void RegisterEmotes(TTypenameArray emotes)
	{
		super.RegisterEmotes(emotes);
		emotes.Insert(EmoteDabbing);
	}
}
