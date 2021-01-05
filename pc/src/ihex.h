struct ihex_task {
	struct ihex_task* next;
	uint32_t address;
	uint32_t size;
	uint8_t* payload;
};

int ihex_read(char* filename, void (*NewByteFunction)(uint8_t, uint32_t));
