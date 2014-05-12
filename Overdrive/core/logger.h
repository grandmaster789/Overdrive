#ifndef OVERDRIVE_CORE_LOGGER_H
#define OVERDRIVE_CORE_LOGGER_H

#include <boost/log/attributes.hpp>
#include <boost/log/sinks.hpp>
#include <boost/log/common.hpp>
#include <boost/log/sources/severity_logger.hpp>
#include <boost/log/utility/record_ordering.hpp>

// Boost does the heavy lifting, here we declare a somewhat improved wrapper 
// for the usage macros (the default BOOST_LOG_SEV is a bit unwieldy)

// the named severity levels should be put in a namespace, but all sorts of things break when
// you do that. Also, when attempting to make the enum an enum class, lots of stuff breaks.
enum eLogSeverity {
	LOG_DEBUG,
	LOG_INFO,
	LOG_WARNING,
	LOG_ERROR,
	LOG_FATAL
};

// add a stream conversion from loglevel to string 
// The loglevel keyword from boost::log won't accept anything outside of the enum, 
// so no additional support is required for the intended use
template <typename C, typename T>
inline std::basic_ostream<C, T>& operator<< (std::basic_ostream<C, T>& stream, eLogSeverity level) {
	static const char* const translation[] = {
		":dbg:",
		"",
		"[wrn]",
		"** ERROR **",
		"<===  FATAL  ===>"
	};

	stream << translation[level];

	return stream;
}

// create shorthand for boost macros (with mild preprocessor trickery)
#define gLog			BOOST_LOG_SEV(::overdrive::core::__gLogObject::get(), eLogSeverity::LOG_INFO)
#define gLogSev(level)	BOOST_LOG_SEV(::overdrive::core::__gLogObject::get(), eLogSeverity::LOG_##level)

namespace overdrive {
	namespace core {
		// create a global boost log singleton
		BOOST_LOG_INLINE_GLOBAL_LOGGER_DEFAULT(__gLogObject, boost::log::sources::severity_logger_mt<eLogSeverity>)

		// helper class for configuring the logger and handling flushes at shutdown
		class LogHelper {
		public:
			//always log to a file, optionally echo to console
			LogHelper(bool echo = true, const std::string& filename = "overdrive.log");
			~LogHelper();

		private:
			typedef boost::log::sinks::text_ostream_backend Backend;
			typedef boost::log::attribute_value_ordering<unsigned int, std::less<unsigned int>> OrderingAttribute;
			typedef boost::log::sinks::unbounded_ordering_queue<OrderingAttribute> Ordering;
			//typedef boost::log::sinks::asynchronous_sink<Backend, Ordering> Sink;
			typedef boost::log::sinks::synchronous_sink<Backend> Sink;

			boost::shared_ptr<Sink> mConsoleSink;
			boost::shared_ptr<Sink> mFileSink;
		};
	}
}

#endif
