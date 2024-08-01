#include "named_pipe.h"
#include "EqStructures.h"
#include "EqAddresses.h"
#include "EqFunctions.h"
#include "Zeal.h"
#include "string_util.h"
#include <Windows.h>
#include <thread>



const std::map<int, std::string> LabelNames = {
	{1, "Name"},
	{2, "Level"},
	{3, "Class"},
	{4, "Deity"},
	{5, "Strength"},
	{6, "Stamina"},
	{7, "Dexterity"},
	{8, "Agility"},
	{9, "Wisdom"},
	{10, "Intelligence"},
	{11, "Charisma"},
	{12, "SaveVsPoison"},
	{13, "SaveVsDisease"},
	{14, "SaveVsFire"},
	{15, "SaveVsCold"},
	{16, "SaveVsMagic"},
	{17, "CurrentHP"},
	{18, "MaxHP"},
	{19, "HPPerc"},
	{20, "ManaPerc"},
	{21, "STAPerc"},
	{22, "CurrentMitigation"},
	{23, "CurrentOffense"},
	{24, "Weight"},
	{25, "MaxWeight"},
	{26, "ExpPerc"},
	{27, "AltExpPerc"},
	{28, "TargetName"},
	{29, "TargetHPPerc"},
	{30, "GroupMember1Name"},
	{31, "GroupMember2Name"},
	{32, "GroupMember3Name"},
	{33, "GroupMember4Name"},
	{34, "GroupMember5Name"},
	{35, "GroupMember1HPPerc"},
	{36, "GroupMember2HPPerc"},
	{37, "GroupMember3HPPerc"},
	{38, "GroupMember4HPPerc"},
	{39, "GroupMember5HPPerc"},
	{40, "GroupPet1HPPerc"},
	{41, "GroupPet2HPPerc"},
	{42, "GroupPet3HPPerc"},
	{43, "GroupPet4HPPerc"},
	{44, "GroupPet5HPPerc"},
	{45, "Buff0"},
	{46, "Buff1"},
	{47, "Buff2"},
	{48, "Buff3"},
	{49, "Buff4"},
	{50, "Buff5"},
	{51, "Buff6"},
	{52, "Buff7"},
	{53, "Buff8"},
	{54, "Buff9"},
	{55, "Buff10"},
	{56, "Buff11"},
	{57, "Buff12"},
	{58, "Buff13"},
	{59, "Buff14"},
	{60, "Spell1XMLName0"},
	{61, "Spell2XMLName1"},
	{62, "Spell3XMLName2"},
	{63, "Spell4XMLName3"},
	{64, "Spell5XMLName4"},
	{65, "Spell6XMLName5"},
	{66, "Spell7XMLName6"},
	{67, "Spell8XMLName7"},
	{68, "PlayerPetName"},
	{69, "PlayerPetHPPerc"},
	{70, "PlayerCurrentHPMaxHP"},
	{71, "CurrentAAPoints"},
	{72, "CurrentAAPerc"},
	{73, "LastName"},
	{74, "Title"},
	{80, "Mana/MaxMana"},
	{81, "ExpPH"},
	{82, "TargetPetOwner"},
	{124, "Mana"},
	{125, "MaxMana"},
	{134, "CastingName"},
};
const std::map<int, std::string> GaugeNames = {
   {1, "HP"},
   {2, "Mana"},
   {3, "Stamina"},
   {4, "Experience"},
   {5, "AltExp"},
   {6, "Target"},
   {7, "Casting"},
   {8, "Breath"},
   {9, "Memorize"},
   {10, "Scribe"},
   {11, "Group1HP"},
   {12, "Group2HP"},
   {13, "Group3HP"},
   {14, "Group4HP"},
   {15, "Group5HP"},
   {16, "PetHP"},
   {17, "Group1PetHP"},
   {18, "Group2PetHP"},
   {19, "Group3PetHP"},
   {20, "Group4PetHP"},
   {21, "Group5PetHP"},
   {23, "ExpPerHR"}
};



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

void log_hook(char* data)
{
	ZealService* zeal = ZealService::get_instance();
	pipe_data pd(pipe_data_type::log, data);
	if (zeal->pipe.get())
		zeal->pipe->write(pd.serialize().dump());
	zeal->hooks->hook_map["logtextfile"]->original(log_hook)(data);
}

void named_pipe::chat_msg(const char* data, int color_index)
{
	try {
		std::string sanitized_data(data);

		//sanitization: Remove non-printable characters
		sanitized_data.erase(std::remove_if(sanitized_data.begin(), sanitized_data.end(), [](unsigned char c) {
			return !std::isprint(c);
			}), sanitized_data.end());

		nlohmann::json jd = { {"type", color_index }, {"text", sanitized_data} };
		write(jd.dump(), pipe_data_type::log);
	}
	catch (const std::exception& e) {
	}
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

void named_pipe::main_loop()
{
	if (!pipe_handles.size()) //nothing is connected don't waste the cpu time getting values
		return;
	static auto last_output = GetTickCount64();
	if (GetTickCount64() - last_output > pipe_delay)
	{
		nlohmann::json label_array = nlohmann::json::array();
		for (auto& [id, name] : LabelNames)
		{
			std::string value;
			if (ZealService::get_instance()->labels_hook->GetLabel(id, value))
			{
				label_array.push_back({ {"type", id}, {"value", value} });
			}
		}
		nlohmann::json gauge_array = nlohmann::json::array();
		for (auto& [id, name] : GaugeNames)
		{
			std::string text;
			int val = ZealService::get_instance()->labels_hook->GetGauge(id, text);
			gauge_array.push_back({ {"type", id}, {"text", text}, {"value", val} });
		}

		write(label_array.dump(), pipe_data_type::label);
		write(gauge_array.dump(), pipe_data_type::gauge);

		if (Zeal::EqGame::get_self())
		{
			nlohmann::json data = { {"zone", Zeal::EqGame::get_self()->ZoneId}, {"location", Zeal::EqGame::get_self()->Position.toJson() }, {"heading", Zeal::EqGame::get_self()->Heading}, {"autoattack", (bool)(*(BYTE*)0x7f6ffe)} };
			write(data.dump(), pipe_data_type::player);
		}
		last_output = GetTickCount64();
	}
}

std::string ArgsToString(const std::vector<std::string>& vec, const std::string& delimiter) {
	if (vec.size() <= 1) {
		return "";
	}

	std::string result = vec[1];
	for (size_t i = 2; i < vec.size(); ++i) {
		result += delimiter + vec[i];
	}
	return result;
}

struct PipeData {
	OVERLAPPED overlapped;
	HANDLE pipe;
	PipeData(HANDLE h) {
		pipe = h;
		ZeroMemory(&overlapped, sizeof(OVERLAPPED));
		//overlapped.hEvent = CreateEvent(NULL, TRUE, FALSE, NULL);
	}
};

void CALLBACK WriteCompletion(DWORD dwErrorCode, DWORD dwNumberOfBytesTransfered, LPOVERLAPPED lpOverlapped) {
	PipeData* pData = reinterpret_cast<PipeData*>(lpOverlapped);
	delete pData;
}

bool IsValidHandle(HANDLE h) {
	return (h != INVALID_HANDLE_VALUE) && (GetFileType(h) != FILE_TYPE_UNKNOWN || GetLastError() == NO_ERROR);
}



bool WriteDataWithRetry(HANDLE h, const std::string& data, OVERLAPPED* pData, LPOVERLAPPED_COMPLETION_ROUTINE WriteCompletion) {
	const int maxRetries = 3;
	int attempt = 0;
	if (IsValidHandle(h))
	{
		//DWORD bytesAvailable;
		//DWORD totalBytes;
		//DWORD bytesLeftThisMessage;
		//if (PeekNamedPipe(h, NULL, 0, NULL, &bytesAvailable, &bytesLeftThisMessage)) {
		//	std::cout << "Bytes available in pipe: " << bytesAvailable << std::endl;
		//	while (bytesAvailable > 1024*16) {
		//		PeekNamedPipe(h, NULL, 0, NULL, &bytesAvailable, &bytesLeftThisMessage);
		//		Sleep(1); // Slow down if pipe is too full
		//	}
		//}
		while (attempt < maxRetries) {
			if (WriteFileEx(h, data.c_str(), static_cast<DWORD>(data.length()), pData, WriteCompletion)) {
				return true;
			}
			else {
				DWORD errorCode = GetLastError();
				char* errorMsg = nullptr;
				FormatMessageA(
					FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_IGNORE_INSERTS,
					nullptr,
					errorCode,
					MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
					reinterpret_cast<LPSTR>(&errorMsg),
					0,
					nullptr
				);

				if (errorCode != 232) //the pipe is being closed -- when closing the other end abruptly
				{
					if (errorMsg) {
						Zeal::EqGame::print_chat_hook("WriteFileEx failed on attempt %i with error %u: %s", attempt + 1, errorCode, errorMsg);
						Zeal::EqGame::print_chat_hook("Parameters [%i] [%s] [%i] [%i]", h, data.c_str(), data.length(), pData);
						LocalFree(errorMsg); // Free the buffer allocated by FormatMessage
					}
					else {
						Zeal::EqGame::print_chat_hook("WriteFileEx failed on attempt %i with error %u", attempt + 1, errorCode);
					}
				}

				Sleep(10);
				attempt++;
			}
		}
	}

	return false;
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
		PipeData* pData = new PipeData(h);

		if (h != INVALID_HANDLE_VALUE) {
			if (!WriteDataWithRetry(h, data, reinterpret_cast<LPOVERLAPPED>(pData), WriteCompletion))
			{
				DisconnectNamedPipe(h);
				CloseHandle(h);
				h = INVALID_HANDLE_VALUE;
				delete pData;
			}
		}
	}
	pipe_handles.erase(std::remove_if(pipe_handles.begin(), pipe_handles.end(), [](HANDLE x) { return x == INVALID_HANDLE_VALUE; }), pipe_handles.end());
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

void named_pipe::update_delay(unsigned new_delay)
{
	ZealService::get_instance()->ini->setValue("Zeal", "PipeDelay", new_delay);
	pipe_delay = new_delay;
	Zeal::EqGame::print_chat("pipe delay is now set to %i", pipe_delay);
}

named_pipe::named_pipe(ZealService* zeal, IO_ini* ini)
{
	if (!ini->exists("Zeal", "PipeDelay"))
		ini->setValue<int>("Zeal", "PipeDelay", 100);
	pipe_delay = ini->getValue<int>("Zeal", "PipeDelay");

	zeal->callbacks->AddGeneric([this]() { main_loop(); });
	zeal->commands_hook->Add("/pipedelay", {}, "delay between the pipe loop output in milliseconds",
		[this](std::vector<std::string>& args) {
			if (args.size() > 1)
			{
				int del = 100;
				Zeal::String::tryParse(args[1], &del);
				update_delay(del);
			}
			else
			{
				Zeal::EqGame::print_chat("an argument is required for ms delay");
			}
			return true; //return true to stop the game from processing any further on this command, false if you want to just add features to an existing cmd
		});
	zeal->commands_hook->Add("/pipe", {}, "outputs text to a pipe",
		[this](std::vector<std::string>& args) {
			nlohmann::json data = { {"text", ArgsToString(args, " ")} };
			write(data.dump(), pipe_data_type::custom);
			return true; //return true to stop the game from processing any further on this command, false if you want to just add features to an existing cmd
		});
	// zeal->hooks->Add("logtextfile", 0x5240dc, log_hook, hook_type_detour); //receiving this via print chat so we can get color indexes
	name += std::to_string(GetCurrentProcessId());
	pipe_thread = std::thread([this]() {
		while (!end_thread)
		{
			HANDLE pipe_handle = CreateNamedPipeA(name.c_str(), PIPE_ACCESS_OUTBOUND | FILE_FLAG_OVERLAPPED, PIPE_TYPE_BYTE | PIPE_WAIT | PIPE_READMODE_BYTE, PIPE_UNLIMITED_INSTANCES, 32768, 32768, NMPWAIT_USE_DEFAULT_WAIT, NULL);
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
						MessageBoxA(0, "Unhandled error in named pipe", "Pipe error", 0);
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