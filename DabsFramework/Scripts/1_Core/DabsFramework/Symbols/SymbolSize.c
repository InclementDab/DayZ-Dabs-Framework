class SymbolSize: string
{
	static const int IDX_THIN = 0;
	static const int IDX_LIGHT = 1;
	static const int IDX_REGULAR = 2;
	static const int IDX_SOLID = 3;
	
	static const SymbolSize THIN = "set:thin";
	static const SymbolSize LIGHT = "set:light";
	static const SymbolSize REGULAR = "set:regular";
	static const SymbolSize SOLID = "set:solid";
	static const ref array<SymbolSize> ALL = { THIN, LIGHT, REGULAR, SOLID };
	
	static SymbolSize FromIdx(int idx)
	{
		switch (idx) {
			case IDX_THIN: return THIN;
			case IDX_LIGHT: return LIGHT;
			case IDX_REGULAR: return REGULAR;
			case IDX_SOLID: return SOLID;
		}
		
		return REGULAR;
	}
	
	int ToIdx()
	{
		switch (value) {
			case THIN: return IDX_THIN;
			case LIGHT: return IDX_LIGHT;
			case REGULAR: return IDX_REGULAR;
			case SOLID: return IDX_SOLID;
		}
		
		return -1;
	}
}
typedef string SymbolSize;