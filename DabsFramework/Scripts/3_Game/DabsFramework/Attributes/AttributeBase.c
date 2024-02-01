class AttributeBase: Class
{
	// File name attribute resides in 
	protected string m_File;
	
	// Line number of attribute
	protected int m_LineNumber;
	
	ref FieldInfo Field;
	
	void AttributeBase()
	{
		string stack;
		DumpStackString(stack);
		array<string> stack_split = {};
		stack.Split(":", stack_split);
		m_LineNumber = stack_split[stack_split.Count() - 1].Trim().ToInt();
		
		array<string> stack_split_further = {};
		stack_split[stack_split.Count() - 2].Split(")", stack_split_further);
		m_File = stack_split_further[1].Trim();
				
		if (!File.Exists(m_File)) {
			Error("Couldnt find file " + m_File);
			return;
		}
		
		FileHandle handle = OpenFile(m_File, FileMode.READ);
		string line_content;
		
		for (int i = m_LineNumber + 1; i > 0; i--) {
			FGets(handle, line_content);
		}
		
		Field = FParseLine(line_content);
		CloseFile(handle);
	}
	
	static FieldInfo FParseLine(string line)
	{
		
		string tokens[256];
		int count = line.ParseString(tokens);
		FieldInfo field_info = new FieldInfo();
		
		for (int i = 0; i < count; i++) {
			
			// Reparse
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