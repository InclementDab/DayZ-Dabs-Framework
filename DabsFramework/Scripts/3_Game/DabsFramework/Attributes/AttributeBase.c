class AttributeBase: Class
{
	// File name attribute resides in 
	protected string m_File;
	
	// Line number of attribute
	protected int m_LineNumber;
	
	protected int m_FieldLineNumber;
	ref FieldInfo Field;
	
	void AttributeBase()
	{
		string stack;
		DumpStackString(stack);
		array<string> stack_split = {};
		stack.Split(":", stack_split);
		m_LineNumber = stack_split[stack_split.Count() - 1].Trim().ToInt();
		m_FieldLineNumber = m_LineNumber + 1;
		
		array<string> stack_split_further = {};
		stack_split[stack_split.Count() - 2].Split(")", stack_split_further);
		m_File = stack_split_further[1].Trim();
		
		Print(m_LineNumber);
		Print(m_File);
		
		if (!File.Exists(m_File)) {
			Error("Couldnt find file " + m_File);
			return;
		}
		
		ParseHandle parse_handle = BeginParse(m_File);
		
		string tokens[256];
		int parse_count = ParseLine(parse_handle, m_FieldLineNumber, tokens);
		Print(parse_count);
		Print(tokens);
		// This cannot end well
		while (!parse_count) {
			m_FieldLineNumber++;
			parse_count = ParseLine(parse_handle, m_FieldLineNumber, tokens);
		}
		
		Field = ParseField(tokens, parse_count);
		
		EndParse(parse_handle);
	}
	
	static FieldInfo ParseField(string tokens[], int count)
	{
		FieldInfo field_info = new FieldInfo();
		
		for (int i = 0; i < count; i++) {
			string token_parse;
			int token_type = tokens[i].ParseStringEx(token_parse);
			switch (token_type) {
				case 1: {
					switch (token_parse) {
						case "protected": {
							field_info.Visiblity = FieldVisibility.PROTECTED;
							break;
						}
						
						case "ref": {
							field_info.Reference = true;
							break;
						}
						
						case "<": {
							//Start template
							
							break;
						}
						
						case ">": {
							
							break;
						}
					}
					break;
				}
				case 3: {
					Print(token_parse);
					break;
				}
				
				default: PrintFormat("Unhandled token type [%1] val=%2", token_type, token_parse); 
			}
		}
		
		return field_info;
	}
}