enum FieldVisibility
{
	PUBLIC = 0,
	PROTECTED = 1,
	PRIVATE = 2
};

class FieldInfo: MemberInfo
{
	bool Reference;
	bool Const;
	typename DeclaringType;
	
	FieldVisibility Visiblity;
}