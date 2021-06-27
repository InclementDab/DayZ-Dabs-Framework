modded class Timer
{
	protected override void OnInit(int category)
	{
		if (!GetGame()) {
			return;
		}
		
		super.OnInit(category);
	}
}