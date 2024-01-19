// 			     name, value
typedef Param2<string, string> HttpArgument;

class HttpArguments: array<ref HttpArgument>
{	
	string ToQuery(string query)
	{
		for (int j = 0; j < Count(); j++) {
			if (this[j].param1 == string.Empty || this[j].param2 == string.Empty) {
				RemoveOrdered(j);
			}
		}
		
		for (int i = 0; i < Count(); i++) {			
			if (i == 0) {
				query += "?";
			} else {
				query += "&";
			}
			
			query += this[i].param1 + "=" + this[i].param2;
		}
		
		return query;
	}
}