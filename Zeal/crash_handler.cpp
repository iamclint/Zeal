#include "crash_handler.h"
#include "Zeal.h"
#include <ctime>
#include <sstream>
#include <iomanip>
#include <map>
#include <vector>
#include "miniz.h"

std::vector<DWORD> nonCrashExceptionCodes =
{
    DBG_PRINTEXCEPTION_C,  // 0x40010006, OutputDebugString exception for ASCII strings
    DBG_COMMAND_EXCEPTION,  // 0x40010009, Used for internal command execution during debugging
    DBG_PRINTEXCEPTION_WIDE_C,  // 0x4001000A, OutputDebugString exception for wide-character strings
    DBG_CONTROL_C,  // 0x40010005, Control-C exception for console applications
    EXCEPTION_BREAKPOINT,  // 0x80000003, Used by debuggers to temporarily suspend execution
    EXCEPTION_SINGLE_STEP,  // 0x80000004, Used by debuggers for single-step tracing
    STATUS_GUARD_PAGE_VIOLATION,  // 0x80000001, Occurs on a stack overflow guard page hit
    STATUS_DATATYPE_MISALIGNMENT,  // 0x80000002, Memory access is misaligned
    STATUS_STACK_OVERFLOW,  // 0xC00000FD, Stack overflow occurred but might be recoverable
    0x406D1388,  // Exception used to set thread names for debugging
    0x80000007,  // Used to wake up the system debugger
    0xe06d7363  // C++ exception, funny enough in ascii this code is 'MSC'
};

// Define a map to store exception codes and their descriptions
std::map<DWORD, std::string> exceptionCodeStrings = {
    {EXCEPTION_ACCESS_VIOLATION, "EXCEPTION_ACCESS_VIOLATION"},
    {EXCEPTION_ARRAY_BOUNDS_EXCEEDED, "EXCEPTION_ARRAY_BOUNDS_EXCEEDED"},
    {EXCEPTION_BREAKPOINT, "EXCEPTION_BREAKPOINT"},
    {EXCEPTION_DATATYPE_MISALIGNMENT, "EXCEPTION_DATATYPE_MISALIGNMENT"},
    {EXCEPTION_FLT_DENORMAL_OPERAND, "EXCEPTION_FLT_DENORMAL_OPERAND"},
    {EXCEPTION_FLT_DIVIDE_BY_ZERO, "EXCEPTION_FLT_DIVIDE_BY_ZERO"},
    {EXCEPTION_FLT_INEXACT_RESULT, "EXCEPTION_FLT_INEXACT_RESULT"},
    {EXCEPTION_FLT_INVALID_OPERATION, "EXCEPTION_FLT_INVALID_OPERATION"},
    {EXCEPTION_FLT_OVERFLOW, "EXCEPTION_FLT_OVERFLOW"},
    {EXCEPTION_FLT_STACK_CHECK, "EXCEPTION_FLT_STACK_CHECK"},
    {EXCEPTION_FLT_UNDERFLOW, "EXCEPTION_FLT_UNDERFLOW"},
    {EXCEPTION_ILLEGAL_INSTRUCTION, "EXCEPTION_ILLEGAL_INSTRUCTION"},
    {EXCEPTION_IN_PAGE_ERROR, "EXCEPTION_IN_PAGE_ERROR"},
    {EXCEPTION_INT_DIVIDE_BY_ZERO, "EXCEPTION_INT_DIVIDE_BY_ZERO"},
    {EXCEPTION_INT_OVERFLOW, "EXCEPTION_INT_OVERFLOW"},
    {EXCEPTION_INVALID_DISPOSITION, "EXCEPTION_INVALID_DISPOSITION"},
    {EXCEPTION_NONCONTINUABLE_EXCEPTION, "EXCEPTION_NONCONTINUABLE_EXCEPTION"},
    {EXCEPTION_PRIV_INSTRUCTION, "EXCEPTION_PRIV_INSTRUCTION"},
    {EXCEPTION_SINGLE_STEP, "EXCEPTION_SINGLE_STEP"},
    {EXCEPTION_STACK_OVERFLOW, "EXCEPTION_STACK_OVERFLOW"},
    // Add more exception codes as needed
};

void EnsureCrashesFolderExists() {
    // Check if 'crashes' folder exists
    DWORD dwAttrib = GetFileAttributes(L"crashes");
    if (dwAttrib == INVALID_FILE_ATTRIBUTES || !(dwAttrib & FILE_ATTRIBUTE_DIRECTORY)) {
        // Create 'crashes' folder if it does not exist
        if (!CreateDirectory(L"crashes", NULL)) {
            std::cerr << "Failed to create 'crashes' folder." << std::endl;
        }
    }
}

std::string GetModuleNameFromAddress(LPVOID address) {
    HMODULE hModule;
    DWORD_PTR dwOffset;
    char modulePath[MAX_PATH];

    // Get module handle from address
    if (GetModuleHandleEx(GET_MODULE_HANDLE_EX_FLAG_FROM_ADDRESS | GET_MODULE_HANDLE_EX_FLAG_UNCHANGED_REFCOUNT,
        reinterpret_cast<LPCWSTR>(address), &hModule) != 0) {
        // Get module file name
        if (GetModuleFileNameA(hModule, modulePath, MAX_PATH) != 0) {
            return modulePath;
        }
    }

    return "";
}

std::string ZipCrash(const std::string& folderName, const std::string& dumpFilePath, const std::string& reasonFilePath)
{
    // Zip the files
    std::time_t t = std::time(nullptr);
    std::tm tm;
    localtime_s(&tm, &t);
    std::ostringstream CrashFileName;
    CrashFileName << std::put_time(&tm, "%Y-%m-%d_%H-%M-%S");

    std::string zipFilePath = "crashes\\" + CrashFileName.str() + ".zip";
    mz_zip_archive zip_archive;
    memset(&zip_archive, 0, sizeof(zip_archive));

    if (!mz_zip_writer_init_file(&zip_archive, zipFilePath.c_str(), 0)) {
        std::cerr << "Failed to initialize zip archive." << std::endl;
        return "";
    }

    if (!mz_zip_writer_add_file(&zip_archive, "minidump.dmp", dumpFilePath.c_str(), NULL, 0, MZ_BEST_COMPRESSION)) {
        std::cerr << "Failed to add minidump to zip archive." << std::endl;
        mz_zip_writer_end(&zip_archive);
        return "";
    }

    if (!mz_zip_writer_add_file(&zip_archive, "crash_reason.txt", reasonFilePath.c_str(), NULL, 0, MZ_BEST_COMPRESSION)) {
        std::cerr << "Failed to add crash reason to zip archive." << std::endl;
        mz_zip_writer_end(&zip_archive);
        return "";
    }

    mz_zip_writer_finalize_archive(&zip_archive);
    mz_zip_writer_end(&zip_archive);

    // Clean up the original files
    DeleteFileA(dumpFilePath.c_str());
    DeleteFileA(reasonFilePath.c_str());
    RemoveDirectoryA(folderName.c_str());
    return CrashFileName.str();
}


void WriteMiniDump(EXCEPTION_POINTERS* pep, const std::string& reason) {
    // Check for non-crash exceptions and return early if detected
    if (pep != nullptr && pep->ExceptionRecord != nullptr) {
        DWORD exceptionCode = pep->ExceptionRecord->ExceptionCode;
        for (DWORD nonCrashCode : nonCrashExceptionCodes) {
            if (exceptionCode == nonCrashCode)                 
                return;
        }
    }
    // Get the current time for a unique folder name
    EnsureCrashesFolderExists();
    std::time_t t = std::time(nullptr);
    std::tm tm;
    localtime_s(&tm, &t);
    std::ostringstream folderNameStream;
    folderNameStream << "crashes\\"
        << std::put_time(&tm, "%Y-%m-%d_%H-%M-%S");

    std::string folderName = folderNameStream.str();

    // Create the unique folder
    if (CreateDirectoryA(folderName.c_str(), NULL) || GetLastError() == ERROR_ALREADY_EXISTS) {
        // Create the mini-dump file path
        std::string dumpFilePath = folderName + "\\minidump.dmp";

        HANDLE hFile = CreateFileA(dumpFilePath.c_str(), GENERIC_WRITE, 0, NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);

        if (hFile == INVALID_HANDLE_VALUE) {
            std::cerr << "Could not create dump file." << std::endl;
            return;
        }

        // Create the mini-dump
        MINIDUMP_EXCEPTION_INFORMATION mdei;
        mdei.ThreadId = GetCurrentThreadId();
        mdei.ExceptionPointers = pep;
        mdei.ClientPointers = FALSE;
        //MiniDumpWithPrivateReadWriteMemory
        MINIDUMP_TYPE mdt = (MINIDUMP_TYPE)(MiniDumpWithHandleData | MiniDumpWithProcessThreadData | MiniDumpWithThreadInfo | MiniDumpWithUnloadedModules);

        BOOL result = MiniDumpWriteDump(GetCurrentProcess(), GetCurrentProcessId(), hFile, mdt, (pep != 0) ? &mdei : 0, 0, 0);
        std::string moduleName = "";
        std::string reasonFilePath = folderName + "\\crash_reason.txt";
        if (!result) {
            std::cerr << "Failed to write dump file." << std::endl;
        }
        else {
            // Create the reason file path
            std::ofstream reasonFile(reasonFilePath);
            if (reasonFile.is_open()) {
                reasonFile << "Unhandled exception occurred: " << reason << std::endl << std::endl;
                reasonFile << "Zeal Version: " << ZEAL_VERSION << " (" << ZEAL_BUILD_VERSION << ")" << std::endl << std::endl;
                if (pep != nullptr && pep->ExceptionRecord != nullptr) {
                    reasonFile << "Exception Code: 0x" << std::hex << pep->ExceptionRecord->ExceptionCode << std::endl;
                    if (exceptionCodeStrings.count(pep->ExceptionRecord->ExceptionCode))
                        reasonFile << "Exception String: " << std::hex << exceptionCodeStrings[pep->ExceptionRecord->ExceptionCode] << std::endl;
                    reasonFile << "Exception Address: 0x" << std::hex << pep->ExceptionRecord->ExceptionAddress << std::endl;
                    // Get and write module information
                    moduleName = GetModuleNameFromAddress(pep->ExceptionRecord->ExceptionAddress);
                    if (!moduleName.empty()) {
                        reasonFile << "Exception occurred in module: " << moduleName << std::endl;
                    }
                    else {
                        reasonFile << "Module information not available." << std::endl;
                    }
                    // Add more details as needed from pep->ExceptionRecord and pep->ContextRecord

                    Zeal::EqStructures::EQCHARINFO* char_info = Zeal::EqGame::get_char_info();
                    Zeal::EqStructures::Entity* spawn_info = (char_info ? char_info->SpawnInfo : nullptr);
                    Zeal::EqStructures::Entity* self = Zeal::EqGame::get_self();
                    reasonFile << "EQCHARINFO: 0x" << std::hex << (uint32_t)(char_info) << std::endl;
                    reasonFile << "SpawnInfo: 0x" << std::hex << (uint32_t)(spawn_info) << std::endl;
                    reasonFile << "Self: 0x" << std::hex << (uint32_t)(self) << std::dec << std::endl;
                    reasonFile << "Character: " << (char_info ? char_info->Name : "Unknown") << std::endl;
                    std::string ui_skin = (char*)0x63D3C0;
                    reasonFile << "UI Skin: " << ui_skin << std::endl;
                    int zone_id = self ? self->ZoneId : -1;
                    reasonFile << "Zone ID: " << zone_id << std::endl;
                    reasonFile << "Game state: " << Zeal::EqGame::get_gamestate() << std::endl;
                    reasonFile << "ShowSpellEffects: " << *reinterpret_cast<unsigned int*>(0x007cf290) << std::endl;
                    if (ZealService::get_instance() && ZealService::get_instance()->callbacks)
                        reasonFile << "Callbacks: " << ZealService::get_instance()->callbacks->get_trace();
                }
                reasonFile.close();
            }
        }
        CloseHandle(hFile);
        std::string CrashFileName = ZipCrash(folderName, dumpFilePath, reasonFilePath);
        if (std::filesystem::exists("crashes\\ZealCrashSender.exe"))
        {
            std::stringstream arguments;
            arguments << "\"";
            arguments << "Version: " << ZEAL_VERSION << std::endl;
            arguments << "Reason: " << reason << std::endl;
            arguments << "Exception: " << exceptionCodeStrings[pep->ExceptionRecord->ExceptionCode] << std::endl;
            arguments << "Address: 0x" << pep->ExceptionRecord->ExceptionAddress << std::endl;
            arguments << "Module Information: " << moduleName << std::endl;
            arguments << "Zipped Crash: " << CrashFileName << ".zip";
            arguments << "\" ";
            arguments << "\"" << CrashFileName << ".zip\" ";

            std::string cmdLine = "crashes\\ZealCrashSender.exe " + arguments.str();

            // Convert to writable format
            size_t bufferSize = cmdLine.size() + 1;
            char* cmdLineWritable = new char[cmdLine.size() + 1];
            strcpy_s(cmdLineWritable, bufferSize, cmdLine.c_str());

            // Set up STARTUPINFO and PROCESS_INFORMATION structures
            STARTUPINFOA si;
            ZeroMemory(&si, sizeof(si));
            si.cb = sizeof(si);

            PROCESS_INFORMATION pi;
            ZeroMemory(&pi, sizeof(pi));

            // Start the process
            CreateProcessA(
                NULL,         // No module name (use command line)
                cmdLineWritable,      // Command line
                NULL,         // Process handle not inheritable
                NULL,         // Thread handle not inheritable
                FALSE,        // Set handle inheritance to FALSE
                CREATE_NO_WINDOW, // No window
                NULL,         // Use parent's environment block
                NULL,         // Use parent's starting directory 
                &si,          // Pointer to STARTUPINFO structure
                &pi);          // Pointer to PROCESS_INFORMATION structure
        }
        else {
            std::cerr << "Could not create directory: " << folderName << std::endl;
        }
    }
}


LONG CALLBACK VectoredExceptionHandler(PEXCEPTION_POINTERS pExceptionInfo) {
    WriteMiniDump(pExceptionInfo, "VEH");
    return EXCEPTION_CONTINUE_SEARCH; // Continue searching for other handlers
}

CrashHandler::CrashHandler()
{
    exception_handler = AddVectoredExceptionHandler(0, VectoredExceptionHandler);
}
CrashHandler::~CrashHandler()
{
    RemoveVectoredExceptionHandler(exception_handler);
}
