class AttributeBase: Class
{
	// File name attribute resides in 
	protected File m_File;
	
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
				
		if (!m_File.Exists()) {
			Error(string.Format("Couldnt find file %1", m_File));
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
			//PrintFormat("FParseLine: type=%1 value=%2", token_type, token_parse);
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
						
						case "static":
						case "const": {
							field_info.Const = true;
							break;
						}
						
						case ";": {
							return field_info;
						}
						
						case "=": {
							field_info.Default = true;
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
					field_info.DefaultValue = token_parse;
					break;
				}
				
				case 3: {
					switch (token_parse) {
						default: {
							if (!field_info.DeclaringType) {
								field_info.DeclaringType = token_parse.ToType();
								break;
							}
							
							if (!field_info.Name) {
								field_info.Name = token_parse;
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
		
		return field_info;
	}
}