#include "named_pipe.h"
#include "EqStructures.h"
#include "EqAddresses.h"
#include "EqFunctions.h"
#include "Zeal.h"
#include <Windows.h>
#include <thread>

void ConnectCompletionRoutine(DWORD dwErrorCode, DWORD dwNumberOfBytesTransfered, LPOVERLAPPED lpOverlapped) {
	if (dwErrorCode == 0) {
		std::cout << "Client connected" << std::endl;
	}
	else {
		std::cerr << "Connect failed with error code: " << dwErrorCode << std::endl;
	}

	// Free the OVERLAPPED structure if it was allocated dynamically
	delete lpOverlapped;
}

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

bool IsPipeConnected(HANDLE hPipe) {
	DWORD dwBytesAvailable, dwBytesLeftThisMessage, dwBytesMessage;
	BOOL bSuccess = PeekNamedPipe(hPipe, NULL, 0, NULL, &dwBytesAvailable, &dwBytesLeftThisMessage);
	if (!bSuccess) {
		// Error occurred while checking pipe state
		return false;
	}
	return dwBytesAvailable > 0; // Return true if bytes are available for reading (pipe is connected)
}

named_pipe::named_pipe(ZealService* zeal)
{
	zeal->hooks->Add("logtextfile", 0x5240dc, log_hook, hook_type_detour);
	pipe_thread = std::thread([this]() {
		while (!end_thread)
		{
			HANDLE pipe_handle = CreateNamedPipeA(name, PIPE_ACCESS_DUPLEX | FILE_FLAG_OVERLAPPED, PIPE_TYPE_BYTE | PIPE_WAIT, PIPE_UNLIMITED_INSTANCES, 1024, 1024, NMPWAIT_USE_DEFAULT_WAIT, NULL);
			if (pipe_handle != INVALID_HANDLE_VALUE)
			{
				ConnectNamedPipe(pipe_handle, NULL);
				pipe_handles.push_back(pipe_handle);
			}
		}
	});
	pipe_thread.detach();
}
named_pipe::~named_pipe()
{
	for (auto& h : pipe_handles)
		CloseHandle(h);
	end_thread = true;
	pipe_thread.join();

}

struct PipeData {
	OVERLAPPED overlapped;
	HANDLE pipe;
};

void CALLBACK WriteCompletion(DWORD dwErrorCode, DWORD dwNumberOfBytesTransfered, LPOVERLAPPED lpOverlapped) {
	PipeData* pData = reinterpret_cast<PipeData*>(lpOverlapped);
	delete pData;
}

void named_pipe::write(std::string data)
{

	for (auto& h : pipe_handles)
	{
		PipeData* pData = new PipeData;
		pData->pipe = h;
		pData->overlapped.hEvent = nullptr;
		if (h != INVALID_HANDLE_VALUE) {
			if (!WriteFileEx(h, data.c_str(), data.length(), reinterpret_cast<LPOVERLAPPED>(pData), WriteCompletion))
			{
				h = INVALID_HANDLE_VALUE;
				CloseHandle(h);
			}
		}
		delete pData;
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
