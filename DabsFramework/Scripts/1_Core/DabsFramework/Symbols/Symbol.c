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
		
		return "set:" + image_split[1];
	}
	
	Symbol Resize(SymbolSize size)
	{
		array<string> image_split = {};
		value.Split(":", image_split);
		if (image_split.Count() < 2) {
			Error("Invalid formatting");
			return value;
		}

		return string.Format("%1 image:%2", size, image_split[2]);
	}
	
	static Symbol FormatSymbol(SymbolSize size, Symbols image)
	{
		return string.Format("%1 %2", size, image);
	}
}
typedef string Symbol;