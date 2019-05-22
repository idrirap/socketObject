/*
socket++
*/

#include <string>
#include <cstddef>

/// Library's namespace.
/// Every component belong in its semantic embrace.
namespace socket {
	/// Holds the essential characteristics of a socket.
	inline namespace {
		/// Socket type.
		enum class type : unsigned char {
			datagram = 0,
			raw,
			seq_paquet,
			stream
		};
		/// Socket domain.
		/// "IP" corresponds to the `internet` namespace.
		enum class domain : unsigned char {
			IP = 0,
			UNIX,
			UNSPECIFIED
		};
	}

	/// Holds the abstract classes representing generic components.
	inline namespace {
		/// Abstract class representing a generic address.
		class address {
			// attributes
			private:
				domain _domain;
			// members
			public:
				virtual ~address() = 0;
		};
		/// Abstract class representing a generic socket.
		class socket {
			virtual ~socket() = 0;
		};
	}

	namespace internet {
		/// Address protocol.
		enum class protocol : unsigned char {
			IPV4 = 0,
			IPV6,
			ICMP,
			RAW,
			TCP,
			UDP
		};
	}
	namespace _unix {} // name unix already taken
	namespace unspecified {}
}
