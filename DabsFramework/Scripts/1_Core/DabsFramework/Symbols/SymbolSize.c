typedef string SymbolSize;
class SymbolSize: string
{
	static const SymbolSize THIN = "set:thin";
	static const SymbolSize LIGHT = "set:light";
	static const SymbolSize REGULAR = "set:regular";
	static const SymbolSize SOLID = "set:solid";
	static const ref array<SymbolSize> ALL = { THIN, LIGHT, REGULAR, SOLID };
	
	string Resize(string image)
	{
		array<string> image_split = {};
		image.Split(":", image_split);
		if (image_split.Count() < 2) {
			Error("Invalid formatting");
			return image;
		}

		return string.Format("set:%1 image:%2", value, image_split[2]);
	}
}