enum FieldVisibility
{
	PUBLIC = 0,
	PROTECTED = 1,
	PRIVATE = 2
};

class FieldInfo: MemberInfo
{
	bool Default;
	bool Reference;
	bool Const;
	typename DeclaringType;
	string DefaultValue;
	
	ref array<ref FieldInfo> Generics = {};
	
	FieldVisibility Visiblity;
	
	static FieldInfo CreateFromString(string line)
	{
		string tokens[256];
		int count = line.ParseString(tokens);
		int i = 0;
		
		for (int j = 0; j < 256; j++) {
			if (tokens[j] == ">>") {
				for (int k = 256 - 1; k > j; k--) {
					tokens[k] = tokens[k - 1];
				}
				
				tokens[j] = ">";
				tokens[j + 1] = ">";
			}
		}
		
		return CreateFromString(tokens, count, i, new FieldInfo());
	}
	
	static FieldInfo CreateFromString(string tokens[], int count, inout int current, FieldInfo self)
	{		
		for (; current < count; current++) {
			// Reparse
			string token_parse;
			int token_type = tokens[current].ParseStringEx(token_parse);
			//PrintFormat("FParseLine: type=%1 value=%2", token_type, token_parse);
			switch (token_type) {
				case 1: {
					switch (token_parse) {
						case "protected": {
							self.Visiblity = FieldVisibility.PROTECTED;
							break;
						}
						
						case "ref": {
							self.Reference = true;
							break;
						}
					
						case "<": {
							++current;
							self.Generics.Insert(FieldInfo.CreateFromString(tokens, count, current, new FieldInfo()));
							break;
						}
						
						case ">": {
							return self;
						}
						
						case "static":
						case "const": {
							self.Const = true;
							break;
						}
						
						case ";": {
							return self;
						}
						
						case "=": {
							self.Default = true;
							break;
						}
						
						// array initializer
						case "{":
						case "}": {
							if (!self.DeclaringType.IsInherited(array)) {
								Error("Array initializer found on non-array type");
								break;
							}
							
							break;
						}
						
						default: {
							PrintFormat("Unhandled token type=%1 value=%2", token_type, token_parse);
						}
					}
					break;
				}
				
				// '='
				case 2: {
					self.DefaultValue = token_parse;
					break;
				}
				
				case 3: {
					switch (token_parse) {
						default: {
							if (!self.DeclaringType) {
								self.DeclaringType = token_parse.ToType();
								break;
							}
							
							if (!self.Name) {
								self.Name = token_parse;
								break;
							}
							
							PrintFormat("Unhandled token type=%1 value=%2", token_type, token_parse);
							break;
						}
					}
					
					break;
				}
				
				default: PrintFormat("Unhandled token type=%1", token_type); 
			}
		}
		
		return self;
	}
}