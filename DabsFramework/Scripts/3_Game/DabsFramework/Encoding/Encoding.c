class Encoding 
{
	static const ref TStringArray HEX_BYTES = {
		"0", "1", "2", "3", "4", "5", "6", "7", "8", "9", "A", "B", "C", "D", "E", "F"
	};
	
    static array<int> GetBytes(string str)
    {
        array<int> result = {};
        for (int i = 0; i < str.Length(); i++) {
            result.Insert(Encoding.GetByte(str[i]));
        }
		
        return result;
    }
	
    static int GetByte(string character)
    {
        return UInt8.Convert(character[0].Hash());
    }
	
	/*static string ToHex(string ascii)
	{
		return Encoding.ToHex(ascii.Hash());
	}*/
	
    static string ToHex(int ascii)
    {
        return (HEX_BYTES[Math.Floor(ascii / 16)] + HEX_BYTES[ascii % 16]);
    }
	
    static string FromBytesHex(notnull array<int> data)
    {
        string hex = "";
        for (int i = 0; i < data.Count(); i++) {
            hex += Encoding.ToHex(data[i]);
        }
		
        return hex;
    }
	
	// todo base64 encoding so we can convert Steamid64 to DayZ ID
}