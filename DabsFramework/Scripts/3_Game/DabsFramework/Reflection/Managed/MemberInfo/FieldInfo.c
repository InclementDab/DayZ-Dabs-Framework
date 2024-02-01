enum FieldVisibility
{
	PUBLIC = 0,
	PROTECTED = 1,
	PRIVATE = 2
};

class FieldInfo: MemberInfo
{
	bool Reference;
	typename DeclaringType;
	
	FieldVisibility Visiblity;
}