typedef string Guid;
class Guid: string
{
	static const Guid EMPTY = "00000000-0000-0000-0000-00000000";
	static int Sequence;
	
	static Guid Generate()
	{
		string time_hex = Encoding.ToHex(DateTime.Now(), 8);
		string sequence_hex = Encoding.ToHex(Sequence++, 8);
		if (Sequence == int.MAX) { // rollover
			Sequence = 0;
		}
		
		string random_hex_1 = Encoding.ToHex(Math.RandomInt(0, int.MAX), 4);
		string random_hex_2 = Encoding.ToHex(Math.RandomInt(0, int.MAX), 4);
		return Guid.Create(time_hex, sequence_hex, random_hex_1, random_hex_2);
	}
	
	static Guid Create(string time_hex, string sequence_hex, string random_hex_1, string random_hex_2)
	{
		if (time_hex.Length() != 8) {
			Error("Invalid time hex length, must be 8 characters");
			return EMPTY;
		}
		
		if (sequence_hex.Length() != 8) {
			Error("Invalid time hex length, must be 8 characters");
			return EMPTY;
		}
		
		if (random_hex_1.Length() != 4) {
			Error("Invalid random_hex_1 length, must be 8 characters");
			return EMPTY;
		}
		
		if (random_hex_2.Length() != 4) {
			Error("Invalid random_hex_2 length, must be 8 characters");
			return EMPTY;
		}
		
		return string.Format("%1-%2-%3-%4-%5%6", time_hex, sequence_hex.Substring(0, 4), sequence_hex.Substring(4, 4), random_hex_1, random_hex_2);
	}
}