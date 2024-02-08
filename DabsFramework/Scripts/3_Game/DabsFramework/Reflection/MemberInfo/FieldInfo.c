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
	
	FieldVisibility Visiblity;
}