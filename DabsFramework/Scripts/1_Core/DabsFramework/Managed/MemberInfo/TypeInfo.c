class TypeInfo: MemberInfo
{
    static ref map<typename, ref TypeInfo> s_All = new map<typename, ref TypeInfo>();

    void TypeInfo(typename type)
    {
        if (!s_All) {
            s_All = new map<typename, ref TypeInfo>();
        }

        s_All[type] = this;
    }

    static TypeInfo ParseInternal(string file_name, int line_number, typename parsed_type)
    {
        string file_text = File.ReadAllText(file_name);
        string tokens[1024];
		int count = file_text.ParseString(tokens);
		int i = 0;
        return ParseInternal(tokens, count, i, new TypeInfo(parsed_type));
    }

    static TypeInfo ParseInternal(string tokens[], int count, inout int current, TypeInfo self)
	{	
        for (; current < count; current++) {
			// Reparse
			string token_parse;
			int token_type = tokens[current].ParseStringEx(token_parse);
			PrintFormat("ParseInternal: type=%1 value=%2", token_type, token_parse);
			switch (token_type) {
                
            }
        }

        return self;
    }

    // meh
    static TypeInfo Get(typename type)
    {
        return s_All[type];
    }
}