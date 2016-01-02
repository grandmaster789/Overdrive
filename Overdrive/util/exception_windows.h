#pragma once

#include <cstdint>
#include <string>
#include <exception>

#include "../preprocessor.h"

#if OVERDRIVE_PLATFORM == OVERDRIVE_PLATFORM_WINDOWS

namespace overdrive {
	namespace util {
		/*
			Utilities for converting a windows system level error (the kind you'd need to use GetLastError() for) into a readable exception
		*/
		class WinException:
			public std::exception
		{
		public:
			WinException(); // uses GetLastErrorCode() to fetch the last system level error
			WinException(DWORD errorCode);

			virtual const char* what() const;
			DWORD getErrorCode() const;

		private:
			DWORD mErrorCode;
		};

		struct ErrorSuccess				: public WinException { ErrorSuccess()				: WinException(ERROR_SUCCESS)			{} };
		struct ErrorAccessDenied		: public WinException { ErrorAccessDenied()			: WinException(ERROR_ACCESS_DENIED)		{} };
		struct ErrorFileNotFound		: public WinException { ErrorFileNotFound()			: WinException(ERROR_FILE_NOT_FOUND)	{} };
		struct ErrorAlreadyExists		: public WinException { ErrorAlreadyExists()		: WinException(ERROR_ALREADY_EXISTS)	{} };
		struct ErrorPathNotFound		: public WinException { ErrorPathNotFound()			: WinException(ERROR_PATH_NOT_FOUND)	{} };
		struct ErrorInvalidParameter	: public WinException { ErrorInvalidParameter()		: WinException(ERROR_INVALID_PARAMETER)	{} };
		struct ErrorModuleNotFound		: public WinException { ErrorModuleNotFound()		: WinException(ERROR_MOD_NOT_FOUND)		{} };
		struct ErrorProcedureNotFound	: public WinException { ErrorProcedureNotFound()	: WinException(ERROR_PROC_NOT_FOUND)	{} };

		// MSDN HRESULT reference:
		// https://msdn.microsoft.com/en-us/library/cc231198.aspx
		class HresultException :
			public std::exception
		{
		public:
			HresultException(HRESULT result);

			virtual const char* what() const;
			HRESULT getResult() const;

		private:
			HRESULT mResult;
		};

		void throwIfFailed(HRESULT result);
	}
}

#endif