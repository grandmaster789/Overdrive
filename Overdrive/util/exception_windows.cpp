#include "exception_windows.h"

#if OVERDRIVE_PLATFORM == OVERDRIVE_PLATFORM_WINDOWS

#include <memory>

namespace overdrive {
	namespace util {
		namespace {
			// ::FormatMessage uses ::LocalAlloc to allocate memory (and doesn't free it)
			struct LocalFreeHelper {
				void operator()(LPSTR buffer) {
					::LocalFree(buffer);
				}
			};

			std::string winErrorToString(DWORD errorcode) {
				std::unique_ptr<char[], LocalFreeHelper> buffer;
				LPSTR ptr = nullptr;

				// [NOTE] Windows provides a wstring version, however the std::exception expects char's
				//		  Hence, I went with the 'A' variant instead of the 'W'
				DWORD bufferSize = ::FormatMessageA(
					FORMAT_MESSAGE_FROM_SYSTEM |
					FORMAT_MESSAGE_ALLOCATE_BUFFER |
					FORMAT_MESSAGE_IGNORE_INSERTS,
					nullptr,		// source
					errorcode,		//messageID
					0,				// languageID
					ptr,			// buffer
					0,				// size
					nullptr			// arguments
				);


				if (bufferSize) {
					buffer.reset(ptr); // manage free using unique_ptr
					return std::string(buffer.get(), buffer.get() + bufferSize); // this could yield an exception hence the fiddling
				}
				else
					return std::string("Unknown error (FormatMessage failed): ") + std::to_string(errorcode);
			}

			std::string winHresultToString(HRESULT result) {
				std::unique_ptr<char[], LocalFreeHelper> buffer;
				LPSTR ptr = nullptr;

				// [NOTE] this does not always work, especially if the message should be accessed via  a COM interface
				DWORD bufferSize = ::FormatMessageA(
					FORMAT_MESSAGE_FROM_HMODULE |
					FORMAT_MESSAGE_ALLOCATE_BUFFER |
					FORMAT_MESSAGE_IGNORE_INSERTS,
					nullptr,		// source
					LOWORD(result),	//messageID
					0,				// languageID
					ptr,			// buffer
					0,				// size
					nullptr			// arguments
				);


				if (bufferSize) {
					buffer.reset(ptr); // manage free using unique_ptr
					return std::string(buffer.get(), buffer.get() + bufferSize); // this could yield an exception hence the fiddling
				}
				else
					return std::string("Unknown error (FormatMessage failed): ") + std::to_string(result);
			}

		}

		WinException::WinException():
			mErrorCode(::GetLastError())
		{
		}

		WinException::WinException(DWORD errorCode) :
			mErrorCode(errorCode)
		{
		}

		const char* WinException::what() const {
			return winErrorToString(mErrorCode).c_str();
		}

		DWORD WinException::getErrorCode() const {
			return mErrorCode;
		}

		HresultException::HresultException(HRESULT result):
			mResult(result)
		{
		}

		const char* HresultException::what() const {
			return winHresultToString(mResult).c_str();
		}

		HRESULT HresultException::getResult() const {
			return mResult;
		}

		void throwIfFailed(HRESULT result) {
			if (result != S_OK)
				throw HresultException(result);
		}
	}
}

#endif
