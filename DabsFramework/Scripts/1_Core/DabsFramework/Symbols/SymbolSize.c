class SymbolSize: string
{
	static const SymbolSize THIN = "set:thin";
	static const SymbolSize LIGHT = "set:light";
	static const SymbolSize REGULAR = "set:regular";
	static const SymbolSize SOLID = "set:solid";
	static const ref array<SymbolSize> ALL = { THIN, LIGHT, REGULAR, SOLID };
}
typedef string SymbolSize;