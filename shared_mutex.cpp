#include <map>
#include <string>
#include <mutex>
#include <shared_mutex>

class DnsEntry {};

class DnsCache {
	std::map<std::string, DnsEntry> entries;
	mutable std::shared_mutex entry_mutex;
public:

	DnsEntry find_entry(const std::string & domain) const
	{
		// Shared lock due to being read-only access
		// Can't acquire the lock while the are a writer
		std::shared_lock<std::shared_mutex> lk(entry_mutex);
		const auto it = entries.find(domain);
		return (it == entries.end()) ? DnsEntry() : it->second;
	}

	void update_or_add_entry(std::string const& domain, const DnsEntry & dns_info)
	{
		// Exclusive lock since we are modifying here
		// Can't acquire the lock while the are any readers or another writer
		std::lock_guard<std::shared_mutex> lk(entry_mutex);
		entries[domain] = dns_info;
	}
};

int main() {}
