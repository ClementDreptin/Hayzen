#pragma once

/*namespace Memory {
	void Thread(LPTHREAD_START_ROUTINE lpStartAddress);
}*/

class Memory
{
public:
	static void Thread(LPTHREAD_START_ROUTINE lpStartAddress);
private:
	Memory() {}
};