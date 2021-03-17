#
# Copyright 2015 gRPC authors.
#
# Licensed under the Apache License, Version 2.0 (the "License");
# you may not use this file except in compliance with the License.
# You may obtain a copy of the License at
#
#     http://www.apache.org/licenses/LICENSE-2.0
#
# Unless required by applicable law or agreed to in writing, software
# distributed under the License is distributed on an "AS IS" BASIS,
# WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
# See the License for the specific language governing permissions and
# limitations under the License.
#

HOST_SYSTEM = $(shell uname | cut -f 1 -d_)
SYSTEM ?= $(HOST_SYSTEM)
CXX = g++
CPPFLAGS += `pkg-config --cflags protobuf grpc`
CXXFLAGS += -std=c++17
# CXXFLAGS +=  
# CXXFLAGS += -I/usr/include
# CXXFLAGS += -Llibs
# CXXFLAGS += -lfaiss -lstdc++fs -lgomp -lpthread -lopenblas 


LDFLAGS += -L/usr/local/lib `pkg-config --libs protobuf grpc++`\
           -pthread\
           -Wl,--no-as-needed -lgrpc++_reflection -Wl,--as-needed\
           -ldl\
		   -lstdc++fs -lgomp -lopenblas -lpthread
		   

# FAISSLAGS = -I./src \
# 			-L/usr/lib/x86_64-linux-gnu \
# 			../faiss/libfaiss.a \
# 		   /usr/lib/gcc/x86_64-linux-gnu/8/libgomp.so \
# 		   /usr/lib/x86_64-linux-gnu/libpthread.so \
# 		   /usr/lib/x86_64-linux-gnu/libopenblas.so \
-lstdc++fs
PROTOC = protoc
GRPC_CPP_PLUGIN = grpc_cpp_plugin
GRPC_CPP_PLUGIN_PATH ?= `which $(GRPC_CPP_PLUGIN)`

PROTOS_PATH = ./pbschema

vpath %.proto $(PROTOS_PATH)

all: system-check faiss_rpc_server

faiss_rpc_server: faiss_rpc.pb.o faiss_rpc.grpc.pb.o faiss_grpc_serv.o faiss_grpc.o
	$(CXX) -Llibs $^ $(LDFLAGS) -lfswatch -lfaiss -lstdc++fs -lgomp -lpthread -lopenblas -o $@

#faiss_rpc_server: faiss_rpc.pb.cc faiss_rpc.grpc.pb.cc
#	$(CXX) -std=c++17 -Llibs -Iincludes -I/usr/include faiss_grpc_serv.cc faiss_index_manager.cc faiss_grpc_serv.cc $^ `pkg-config --cflags protobuf grpc` -lfaiss -lstdc++fs -lgomp -lpthread -lopenblas -pthread -Wl,--no-as-needed -lgrpc++_reflection -Wl,--as-needed -ldl -o $@

faiss_grpc_serv.o: faiss_grpc_serv.cc
	$(CXX) -Iincludes -Llibs -std=c++17 `pkg-config --cflags protobuf grpc` $^  -lfswatch -lfaiss -lstdc++fs -lgomp -lpthread -lopenblas -c -o $@

faiss_grpc.o: faiss_grpc.cc
	$(CXX) -Iincludes -Llibs -std=c++17 `pkg-config --cflags protobuf grpc` $^  -lfswatch -lfaiss -lstdc++fs -lgomp -lpthread -lopenblas -c -o $@

# faiss_grpc_serv.o:
# 	$(CXX) -std=c++17 -Iincludes -I/usr/include faiss_grpc_serv.cc `pkg-config --cflags protobuf grpc` -lfaiss -lstdc++fs -lgomp -lpthread -lopenblas  -c -o $@


%.grpc.pb.cc: %.proto
	$(PROTOC) -I $(PROTOS_PATH) --grpc_out=. --plugin=protoc-gen-grpc=$(GRPC_CPP_PLUGIN_PATH) $<

%.pb.cc: %.proto
	$(PROTOC) -I $(PROTOS_PATH) --cpp_out=. $<

clean:
	rm -f *.o *.pb.cc *.pb.h faiss_rpc_server


# The following is to test your system and ensure a smoother experience.
# They are by no means necessary to actually compile a grpc-enabled software.

PROTOC_CMD = which $(PROTOC)
PROTOC_CHECK_CMD = $(PROTOC) --version | grep -q libprotoc.3
PLUGIN_CHECK_CMD = which $(GRPC_CPP_PLUGIN)
HAS_PROTOC = $(shell $(PROTOC_CMD) > /dev/null && echo true || echo false)
ifeq ($(HAS_PROTOC),true)
HAS_VALID_PROTOC = $(shell $(PROTOC_CHECK_CMD) 2> /dev/null && echo true || echo false)
endif
HAS_PLUGIN = $(shell $(PLUGIN_CHECK_CMD) > /dev/null && echo true || echo false)

SYSTEM_OK = false
ifeq ($(HAS_VALID_PROTOC),true)
ifeq ($(HAS_PLUGIN),true)
SYSTEM_OK = true
endif
endif

system-check:
ifneq ($(HAS_VALID_PROTOC),true)
	@echo " DEPENDENCY ERROR"
	@echo
	@echo "You don't have protoc 3.0.0 installed in your path."
	@echo "Please install Google protocol buffers 3.0.0 and its compiler."
	@echo "You can find it here:"
	@echo
	@echo "   https://github.com/google/protobuf/releases/tag/v3.0.0"
	@echo
	@echo "Here is what I get when trying to evaluate your version of protoc:"
	@echo
	-$(PROTOC) --version
	@echo
	@echo
endif
ifneq ($(HAS_PLUGIN),true)
	@echo " DEPENDENCY ERROR"
	@echo
	@echo "You don't have the grpc c++ protobuf plugin installed in your path."
	@echo "Please install grpc. You can find it here:"
	@echo
	@echo "   https://github.com/grpc/grpc"
	@echo
	@echo "Here is what I get when trying to detect if you have the plugin:"
	@echo
	-which $(GRPC_CPP_PLUGIN)
	@echo
	@echo
endif
ifneq ($(SYSTEM_OK),true)
	@false
endif