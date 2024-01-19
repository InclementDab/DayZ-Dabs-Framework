// Ternary operator like in C# or C++
/* 	Example:
	string GetOpenSound()
	{
		return Ternary<string>.If(IsOpen(), "SlidingWoodDoor_Open_SoundSet", "SlidingWoodDoor_Close_SoundSet");
	}
*/
class Ternary<Class T>
{
	private void Ternary();
	private void ~Ternary();
	
	static T If(bool state, T true_statement, T false_statement)
	{
		if (state) {
			return true_statement;
		}

		return false_statement;
	}
}