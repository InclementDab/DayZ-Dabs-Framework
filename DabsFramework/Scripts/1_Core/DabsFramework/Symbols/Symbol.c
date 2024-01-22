typedef string Symbol;
class Symbol: string
{
	SymbolSize GetSize()
	{
		array<string> image_split = {};
		value.Split(":", image_split);
		if (image_split.Count() < 2) {
			Error("Invalid formatting");
			return SymbolSize.REGULAR;
		}
		
		return image_split[1];
	}
	
	static Symbol Format(SymbolSize size, Symbols image)
	{
		return string.Format("%1 %2", size, image);
	}
}