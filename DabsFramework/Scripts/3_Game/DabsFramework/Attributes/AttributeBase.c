class AttributeBase: Class
{
	protected static ref map<typename, ref array<AttributeBase>> m_AttributeBank = new map<typename, ref array<AttributeBase>>();
	
	// File name attribute resides in 
	protected File m_File;
	
	// Line number of attribute
	protected int m_LineNumber;
	
	protected typename m_ParentType;
	
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
			
			string tokens[256];
			int parse_type = line_content.ParseString(tokens);
			switch (parse_type) {
				case 4: {
					m_ParentType = tokens[1].ToType();
					break;
				}
				
				case 3: {
					if (tokens[0] == "modded") {
						m_ParentType = tokens[2].ToType();
					}
					break;
				}
			}
		}
				
		Field = FieldInfo.CreateFromString(line_content);
		CloseFile(handle);
		
		if (!m_ParentType) {
			Error("Attribute registered without parent class");
			return;
		}
		
		for (int j = 0; j < m_ParentType.GetVariableCount(); j++) {
			if (m_ParentType.GetVariableName(j) == Field.Name) {
				Field.DeclaringType = m_ParentType.GetVariableType(j);
			}
		}
		
		if (!m_AttributeBank[m_ParentType]) {
			m_AttributeBank[m_ParentType] = {};
		}
		
		m_AttributeBank[m_ParentType].Insert(this);
	}
	
	void ~AttributeBase()
	{
		if (m_AttributeBank[m_ParentType]) {
			m_AttributeBank[m_ParentType].RemoveItem(this);
		}
	}
	
	static array<AttributeBase> GetCustomAttributes(typename type)
	{
		if (m_AttributeBank.Contains(type)) {
			return m_AttributeBank[type];
		}
		
		return {};
	}
}