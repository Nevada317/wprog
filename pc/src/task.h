typedef enum {
	Action_None = 0,
	Action_WriteFlash,
	Action_WriteEEPROM,
	Action_ReadFlash,
	Action_ReadEEPROM
} action_t;

typedef struct {
	char* Filename;
	action_t Action;
	struct task_t* Next;
} task_t;

task_t* TASK_Create();
task_t* TASK_GetNext();
void TASK_Destroy();
