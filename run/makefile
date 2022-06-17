# This sample should be run after make libHEAAN.a file in lib folder

# All Target
all: clean TestServer TestClient

# Tool invocations
TestServer:
	@echo 'Building target: $@'
	@echo 'Invoking: GCC C++ Linker'
#	g++ testmkckks.cpp -o TestMKScheme -L/usr/local/lib ../lib/libHEAAN.a -lntl -lgmp -lm -std=c++11 -O2 -I/../src -pthread
#	g++ testmkckks_client.cpp -o TestClient -L/usr/local/lib ../lib/libHEAAN.a -lntl -lgmp -lm -std=c++11 -O2 -I/../src -pthread 
	g++ testmkckks_server.cpp -o TestServer -L/usr/local/lib ../lib/libHEAAN.a -lntl -lgmp -lm -std=c++11 -O2 -I/../src -pthread 
	
	@echo 'Finished building target: $@'
	@echo ' '
TestClient:
	@echo 'Building target: $@'
	@echo 'Invoking: GCC C++ Linker'
#	g++ testmkckks.cpp -o TestMKScheme -L/usr/local/lib ../lib/libHEAAN.a -lntl -lgmp -lm -std=c++11 -O2 -I/../src -pthread
	g++ testmkckks_client.cpp -o TestClient -L/usr/local/lib ../lib/libHEAAN.a -lntl -lgmp -lm -std=c++11 -O2 -I/../src -pthread 
#	g++ testmkckks_server.cpp -o TestServer -L/usr/local/lib ../lib/libHEAAN.a -lntl -lgmp -lm -std=c++11 -O2 -I/../src -pthread 
	@echo 'Finished building target: $@'
	@echo ' '

# Other Targets
clean:
	rm -rf TestServer TestClient
