#include "named_pipe.h"
#include "EqStructures.h"
#include "EqAddresses.h"
#include "EqFunctions.h"
#include "Zeal.h"
#include <Windows.h>
#include <thread>

pipe_data::pipe_data(pipe_data_type _type, std::string _data)
{
	data = _data;
	data_len = data.length();
	if (Zeal::EqGame::is_in_game() && Zeal::EqGame::get_self())
		character = Zeal::EqGame::get_self()->Name;
	else
		character = "";
	type = _type;
}

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
	pipe_data pd(pipe_data_type::log, data);
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
	name += std::to_string(GetCurrentProcessId());
	pipe_thread = std::thread([this]() {
		while (!end_thread)
		{
			HANDLE pipe_handle = CreateNamedPipeA(name.c_str(), PIPE_ACCESS_DUPLEX | FILE_FLAG_OVERLAPPED, PIPE_TYPE_BYTE | PIPE_WAIT, PIPE_UNLIMITED_INSTANCES, 5096, 5096, NMPWAIT_USE_DEFAULT_WAIT, NULL);
			if (pipe_handle != INVALID_HANDLE_VALUE)
			{
				OVERLAPPED overlapped;
				memset(&overlapped, 0, sizeof(overlapped));
				overlapped.hEvent = CreateEvent(NULL, TRUE, FALSE, NULL);
				// Connect named pipe with overlapped I/O
				if (ConnectNamedPipe(pipe_handle, &overlapped))
				{
					// Connection succeeded immediately
					pipe_handles.push_back(pipe_handle);
				}
				else
				{
					// Check if connection is pending
					DWORD error = GetLastError();
					if (error == ERROR_IO_PENDING)
					{
						// Wait for the connection with a timeout
						DWORD result = WaitForSingleObject(overlapped.hEvent, 500);
						if (result == WAIT_OBJECT_0)
						{
							// Connection completed successfully
							pipe_handles.push_back(pipe_handle);
						}
						else if (result == WAIT_TIMEOUT)
						{
							// Timeout occurred, cancel the pending operation
							CancelIo(pipe_handle);
							CloseHandle(pipe_handle);
						}
						else
						{
							// Handle error
							CloseHandle(pipe_handle);
						}
					}
					else
					{
						// Handle error
						CloseHandle(pipe_handle);
					}
				}

				// Cleanup
				CloseHandle(overlapped.hEvent);
			
			}
		}
	});
	pipe_thread.detach();
}
named_pipe::~named_pipe()
{
	end_thread = true;
	if (pipe_thread.joinable())
		pipe_thread.join();

	for (auto& h : pipe_handles)
	{
		DisconnectNamedPipe(h);
		CloseHandle(h);
	}

}

struct PipeData {
	OVERLAPPED overlapped;
	HANDLE pipe;
};

void CALLBACK WriteCompletion(DWORD dwErrorCode, DWORD dwNumberOfBytesTransfered, LPOVERLAPPED lpOverlapped) {
	PipeData* pData = reinterpret_cast<PipeData*>(lpOverlapped);
	delete pData;
}

void named_pipe::write(std::string data, pipe_data_type data_type)
{
	pipe_data pd(data_type, data);
	write(pd.serialize().dump());
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
