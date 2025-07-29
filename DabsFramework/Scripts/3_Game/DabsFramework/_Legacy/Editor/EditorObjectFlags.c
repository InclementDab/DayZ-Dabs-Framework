const int EFE_DEFAULT = EditorObjectFlags.BBOX | EditorObjectFlags.MAPMARKER | EditorObjectFlags.OBJECTMARKER | EditorObjectFlags.LISTITEM;
const int EFE_CAMERA_DEFAULT = EFE_DEFAULT | EditorObjectFlags.EDITOR_ONLY;
const int EFE_ATTACHMENT = 0;
const int EFE_BRUSHED = EditorObjectFlags.LISTITEM | EditorObjectFlags.BRUSHED | 0;
enum EditorObjectFlags
{
	NONE = 0,
	BBOX = 2,
	MAPMARKER = 4,
	OBJECTMARKER = 8,
	LISTITEM = 16,
	PERSISTENCE = 32,
	LOCKED = 64, 
	EDITOR_ONLY = 128, 
	SIMULATE = 256, 
	ALLOW_DAMAGE = 512,
	HIDDEN = 1024,
	NOSAVE = 2048,
	NODELETE = 4096,
	BRUSHED = 8192,
};