class AttributeBase: Class
{
	const string CREATE_GLOBALS_TOKEN = "#InitGlobals";
	const string CREATE_ATTRIBUTE_TOKEN = "#CreateAttributes";
	
	static ref array<AttributeBase> s_All = {};

	protected static ref map<typename, ref array<AttributeBase>> m_AttributeBank = new map<typename, ref array<AttributeBase>>();
	
	// File name attribute resides in 
	protected string m_File;
	
	// Line number of attribute
	protected int m_LineNumber;
	
	// Type of declarer
	protected typename m_ParentType;
	
	// Not null, unless error
	ref TypeInfo Type;

	// Populated when appliedto a field
	ref FieldInfo Field;

	// Null
	ref MethodInfo Method;

	// Searches for #CreateAttributes and returns all data
	// returns type of info we are declared on
	// 0 -> field
	// 1 -> function
	// 2 -> method?
	static int GetStackInfo(string stack, out string file, out int line, out string function)
	{
		return -1;
	}
	
	void AttributeBase()
	{
		string stack;
		DumpStackString(stack);		
		if (stack.LastIndexOf(CREATE_GLOBALS_TOKEN) != -1) {
			ErrorEx(string.Format("Putting an attribute on a global function has undefined behavior, exiting"));
			return;
		}

		int attribute_stack_token_index = stack.LastIndexOf(CREATE_ATTRIBUTE_TOKEN);
		if (attribute_stack_token_index == -1) {
			ErrorEx(string.Format("Stack parse error, could not find %1", CREATE_ATTRIBUTE_TOKEN));
			return;
		}

		int newline_index = stack.IndexOfFrom(attribute_stack_token_index, "\n");
		if (newline_index == -1) {
			newline_index = stack.Length() - 1; // fallback
		}

		string attribute_definition_line = stack.Substring(attribute_stack_token_index, newline_index - attribute_stack_token_index);
		attribute_definition_line.Replace(CREATE_ATTRIBUTE_TOKEN, string.Empty);
		attribute_definition_line.TrimInPlace();

		string attribute_parent_type;
		attribute_definition_line.ParseStringEx(attribute_parent_type);
		
		int seperator_pos = attribute_definition_line.LastIndexOf(":");
		int attribute_file_line = attribute_definition_line.Substring(seperator_pos + 2 /* trim space and colon */, attribute_definition_line.Length() - seperator_pos - 2).Trim().ToInt();
		string attribute_file = attribute_definition_line.Substring(2 /* trim () */, seperator_pos - 2).Trim();
	
		m_File = attribute_file;
		m_LineNumber = attribute_file_line + 1;
		Print(attribute_parent_type);
		m_ParentType = attribute_parent_type.ToType();
		Print(m_ParentType);

		//Type = TypeInfo.CreateFromString(line_content);
		Field = FieldInfo.CreateFromString(line_content);
		if (m_ParentType) {
			Type = TypeInfo.ParseInternal(m_File, m_LineNumber, m_ParentType);

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
		
		if (!s_All) {
			s_All = {};
		}

		s_All.Insert(this);
	}
	
	void ~AttributeBase()
	{
		if (m_AttributeBank && m_AttributeBank[m_ParentType]) {
			m_AttributeBank[m_ParentType].RemoveItem(this);
		}

		if (s_All) {
			s_All.RemoveItem(this);
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