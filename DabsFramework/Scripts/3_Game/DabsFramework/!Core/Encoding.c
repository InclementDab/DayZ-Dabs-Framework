class Encoding 
{
	static const ref TStringArray HEX_BYTES = {
		"0", "1", "2", "3", "4", "5", "6", "7", "8", "9", "a", "b", "c", "d", "e", "f"
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
        return uint8.Convert(character[0].Hash());
    }
	
	/*static string ToHex(string ascii)
	{
		return Encoding.ToHex(ascii.Hash());
	}*/

	//! Converts integer to Hexadecimal string
	// \param value: integer input
	// \param fixed_length: fixed length to pad string to, -1 will do nothing
    static string ToHex(int value, int fixed_length = -1)
    {
		string result;
		
		// There might be some Math.Log2 optimization to make the code shorter
		// but im almost certain that this is the fastest way to do this		
		while (value) {
			result = HEX_BYTES[value % 16] + result;
			value >>= 4;
		}
		
		// pad the final result
		int pad_amount = fixed_length - result.Length();
		if (fixed_length != -1) {
			if (pad_amount >= 0) {
				for (int i = 0; i < pad_amount; i++) result = "0" + result;
			} else {
				result = result.Substring(-pad_amount, fixed_length);
			}			
		}
		
        return result;
    }

	static int FromHex(string hex)
	{
		int result;
		
		// Formatting
		hex.ToLower();
		hex.TrimInPlace();
		
		for (int i = 0; i < hex.Length(); i++) {
			/* The additional math at the end is to invert the direction of the for loop
			 but only for the pow, since the index operator works from the first item,
			 we want ^0 on the last digit, and ^n-1 on the first digit */
			result += HEX_BYTES.Find(hex[i]) * Math.Pow(16, Math.AbsInt(i - hex.Length() + 1));
		}

		return result;
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