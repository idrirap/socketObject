/*
new_socket
*/

#include <bitset>
#include <cstddef>
#include <netinet/in.h>
#include <string>
#if 201703L < __cplusplus
#include <compare>
#endif

/// Library's namespace.
/// Every component belong in its semantic embrace.
namespace new_socket {
	/// Holds the essential characteristics of a socket.
	inline namespace {
		/// Socket type.
		enum class alignas(1) type : unsigned char {
			datagram = 0,
			raw,
			seq_paquet,
			stream
		};

		/// Socket domain.
		/// "IP" corresponds to the `internet` namespace.
		enum class alignas(1) domain : unsigned char {
			INET = 0,
			UNIX,
			UNSPECIFIED
		};
	}

	/// Holds the abstract classes representing generic components.
	inline namespace {
		/// Abstract class representing a generic address.
		class address {
			// attributes
			public:
				/// Static class variable holding the `domain` of the `address` subclass.
				const static domain _domain = domain::UNSPECIFIED;
			// members
			public:
				/// Destructor.
				virtual ~address() noexcept = 0;
				/// Returns the `domain` of the `address`.
				domain get_domain() const { return _domain; }
				/// Stream insertion operator.
				/// Must be redefined in instantiable subclasses.
				virtual std::ostream& operator<<(std::ostream& stream) const = 0;
		};

		/// Abstract class representing a generic socket.
		class socket {
			public:
				/// Destructor.
				virtual ~socket() noexcept = 0;
				/// ...
				virtual void receive();
				virtual void send();
				std::ostream& operator<<(std::ostream& stream) const;
				std::istream& operator>>(std::istream& stream);
		};
	}

	/// Internet related addresses and socket.
	namespace internet {
		/// Address protocol.
		enum class alignas(1) protocol : unsigned char {
			IPV4 = 0,
			IPV6,
			ICMP,
			RAW,
			TCP,
			UDP
		};

		/// Generic IP address.
		/// Template parameter is either `std::bitset<32>` or `std::bitset<128>`.
		template<typename type> class ip_address : public address {
			// attributes
			public:
				const static domain _domain = domain::INET;
			protected:
				/// IP port.
				unsigned int port;
				/// Actual IP address as a `bitset<>`.
				type data;
			// members
			public:
				// pure virtual class specifier
					/// Desctrutor.
					virtual ~ip_address() noexcept = 0;
				// rule of five
					/// Default constructor.
					ip_address() = delete;
					/// Constructor.
					ip_address(unsigned int _port, const type& _data) : port(_port), data(_data) {}
					/// Copy constructor.
					ip_address(const ip_address& other) : port(other.port), data(other.data) {}
					/// Move constructor.
					ip_address(ip_address&& other) noexcept : port(other.port), data(other.data) {}
					/// Copy assignment operator.
					ip_address& operator=(const ip_address& other) {
						ip_address tmp(other);	// re-use copy-constructor
						*this = std::move(tmp);	// re-use move-assignment
						return *this;
					}
					/// Move assignment operator.
					ip_address& operator=(ip_address&& other) noexcept {
						// take precautions against `foo = std::move(foo)`
						if(this == &other)
							return *this;
						// destroy dynamic data
						// copy data
						return *this;
					}
				// operators
					/// Subscript operator.
					/// Allows unsafe access to `data` member.
					/// Returns a mutable reference.
					bool& operator[](size_t index) { return data[index]; }
					/// Subscript operator.
					/// Allows unsafe access to `data` member.
					/// Returns an immutable reference.
					const bool& operator[](size_t index) const { return data[index]; }
					/// Function call operator
					/// Returns the value pointed by the index.
					bool operator()(size_t index) const { return data.at(index); }
					/// Bitwise negation operator.
					/// Returns the bitwise negation of the IP address.
					type operator~() { return ~data; }
					/// Bitwise conjunction operator.
					/// Returns the bitwise conjunction of the IP addresses of two `ip_address`.
					type operator&(const ip_address& other) const { return data & other.data; }
					/// Bitwise disjunction operator.
					/// Returns the bitwise disjunction of the IP addresses of two `ip_address`.
					type operator|(const ip_address& other) const { return data | other.data; }
					/// Bitwise exclusive disjunction.
					/// Returns the bitwise exclusive disjunction of the IP addresses of two `ip_address`.
					type operator^(const ip_address& other) const { return data ^ other.data; }
					/// Bitwise conjunction assignment.
					/// Assigns the bitwise conjunction of the IP addresses of two `ip_address`.
					type& operator&=(const ip_address& other) { return &(data &= other.data); }
					/// Bitwise disjunction assignment.
					/// Assigns the bitwise disjunction of the IP addresses of two `ip_address`.
					type& operator|=(const ip_address& other) { return &(data |= other.data); }
					/// Bitwise exclusive disjunction assignment.
					/// Assigns the bitwise exclusive disjunction of the IP addresses of two `ip_address`.
					type& operator^=(const ip_address& other) { return &(data ^= other.data); }
					/// Equality operator.
					/// Tests for the equality of the attributes of two `ip_address`.
					bool operator==(const ip_address& other) const {
						return data == other.data && port == other.port && _domain == other._domain;
					}
					/// Inequality operator.
					/// Tests for the inequality of the attributes of two `ip_address`.
					bool operator!=(const ip_address& other) const { return !(this == other); }
					/// Greater operator.
					/// Tests whether the address of the left operand is greater than the right operand's address.
					bool operator>(const ip_address& other) const { return data > other.data; }
					/// Lesser operator.
					/// Tests whether the address of the left operand is lesser than the right operand's address.
					bool operator<(const ip_address& other) const { return data < other.data; }
					/// Greater equal operator.
					/// Tests whether the address of the left operand is greater than or equal to the right operand's address.
					bool operator>=(const ip_address& other) const { return data >= other.data; }
					/// Lesser equal operator.
					/// Tests whether the address of the left operand is lesser than or equal to the right operand's address.
					bool operator<=(const ip_address& other) const { return data <= other.data; }
					#if 201703L < __cplusplus
					/// Three-way comparison operator.
					/// If the type of the addresses differ, returns `unordered`.
					/// Else, returns the comparison between the addresses.
					std::partial_ordering operator<=>(const ip_address& other) const {
						if(typeid(data) == typeid(other.data)) {
							if(data < other.data)
								return less;
							else if(other.data < data)
								return greater;
							else
								return equivalent;
						}
						else
							return unordered;
					}
					#endif
				// accessors
					/// Returns the port.
					unsigned int get_port() const { return port; }
					/// Returns the bitset representing the actual address.
					const type& get_address() const { return &data; }
				// mutators
					/// Set the port of the address.
					void set_port(unsigned int _port) { port = _port; }
					/// Set the actual address.
					void set_address(const type& _data) { data = _data; }
		};

		/// IPv4 address.
		class ipv4_address : public ip_address<std::bitset<32>> {
			// attributes
			public:
				/// IPv4 local host address.
				constexpr static std::bitset<32> localhost = { INADDR_ANY },
				/// IPv4 broadcast address.
					broadcast = { INADDR_BROADCAST };
			// members
			public:
				/// Stream insertion operator.
				/// Formats the instance for the given output stream.
				std::ostream& operator<<(std::ostream& stream) const {
					stream << data << std::string(":") << std::to_string(port);
					return stream;
				}
		};

		/// IPv6 address.
		class ipv6_address : public ip_address<std::bitset<128>> {
			// attributes
			public:
				/// Contains the wildcard IPv6 address.
				inline static std::bitset<128> wildcard = std::bitset<128>(std::string(reinterpret_cast<const char*>(in6addr_any.s6_addr))),
				/// Contain the loopback IPv6 address.
					loopback = std::bitset<128>(std::string(reinterpret_cast<const char*>(in6addr_loopback.s6_addr)));
			private:
				/// Multicast hop limit.
				unsigned int multicast_hop_limit,
				/// Unicast hop limit.
					unicast_hop_limit,
				/// IPv6 traffic class and flow information.
					flow_info,
				/// Set of interfaces for a scope.
					scope_id;
				/// Interface to use for outgoing multicast packets.
				std::bitset<128> multicast_interface,
				/// Multicast packets are delivered back to the local application.
					multicast_loopback;
				/// Restrict AF_INET6 socket to IPv6 communications only.
				bool ipv6_only;
			// members
			public:
				bool join_multicast_group() { return false; }
				bool leave_multicast_group() { return false; }
				bool is_loopback() { return false; }
				bool is_multicast() { return false; }
				bool is_unicast_link_local() { return false; }
				bool is_unicast_site_local() { return false; }
				bool is_ipv4_mapped() { return false; }
				bool is_ipv4_compatible() { return !ipv6_only; }
				bool is_multicast_node_local() { return false; }
				bool is_multicast_link_local() { return false; }
				bool is_multicast_site_local() { return false; }
				bool is_multicast_organisation_local() { return false; }
				bool is_multicast_global() { return false; }
				void set_muticast_hop_limit(unsigned int hop_limit) { multicast_hop_limit = hop_limit; }
		};
	}

	namespace _unix {} // name unix already taken

	namespace unspecified {}
}
