#include "named_pipe.h"
#include "EqStructures.h"
#include "EqAddresses.h"
#include "EqFunctions.h"
#include "Zeal.h"
#include <Windows.h>
#include <thread>


void log_hook(char* data)
{
	ZealService* zeal = ZealService::get_instance();
	std::string string_data = data;
	pipe_data pd;
	pd.data = string_data;
	pd.data_len = string_data.length();
	pd.type = pipe_data_type::log;
	if (zeal->pipe.get())
		zeal->pipe->write(pd.serialize().dump());
	zeal->hooks->hook_map["logtextfile"]->original(log_hook)(data);
}

named_pipe::named_pipe(ZealService* zeal)
{
	zeal->hooks->Add("logtextfile", 0x5240dc, log_hook, hook_type_detour);
	pipe_handle = CreateNamedPipeA(name, PIPE_ACCESS_DUPLEX, PIPE_TYPE_BYTE | PIPE_WAIT, PIPE_UNLIMITED_INSTANCES, 1024, 1024, 0, NULL);
	ConnectNamedPipe(pipe_handle, NULL);
}
named_pipe::~named_pipe()
{
	CloseHandle(pipe_handle);

}

void named_pipe::write(std::string data)
{
	if (pipe_handle != INVALID_HANDLE_VALUE) {
		DWORD written_bytes = 0;
		WriteFile(pipe_handle, data.c_str(), data.length(), &written_bytes, NULL);
	}
}
void named_pipe::write(const char* format, ...)
{
	va_list argptr;
	char buffer[512];
	va_start(argptr, format);
	vsnprintf(buffer, 511, format, argptr);
	va_end(argptr);
	write(std::string(buffer));
}
