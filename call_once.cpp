#include <mutex>
#include <memory>

struct some_resource {
	void do_something() {};
};

std::shared_ptr<some_resource> resource_ptr;
std::once_flag resource_flag;

void init_resource()
{
	resource_ptr.reset(new some_resource);
}

void foo()
{
	std::call_once(resource_flag, init_resource);
	resource_ptr->do_something();
}



class ConnectionInfo {};
class ConnecitonHandle {};
class DataPacket {};
class ConnectionManager {
	void open(ConnectionInfo&) {};
};

auto manager = std::make_shared<ConnectionManager>(ConnectionManager());

class Communication {
	std::shared_ptr<ConnectionManager> manager;
	ConnectionInfo connection_details;
	ConnectionHandle connection;

	std::once_flag init_flag;

	void open_connection()
	{
		connection = manager.open(connection_details);
	}

public:
	Communication(const ConnectionInfo & cd, const std::shared_ptr<ConnectionManager> mgr)
		: connection_details(cd), manager(mgr) {}

	void send_data(const DataPacket & data) 
	{
		std::call_once(init_flag, &Communication::open_connection, this);
		connection.send_data(data);
	}
	
	DataPacket receive_data()
	{
		std::call_once(init_flag, &Communication::open_connection, this);
		return connection.receive_data();
	}
};

int main() {}
