// This autogenerated skeleton file illustrates how to build a server.
// You should copy it to another filename to avoid overwriting it.

#include "Serv.h"
#include <thrift/protocol/TBinaryProtocol.h>
#include <thrift/server/TSimpleServer.h>
#include <thrift/transport/TServerSocket.h>
#include <thrift/transport/TBufferTransports.h>
#include <concurrency/ThreadManager.h>
#include <concurrency/PosixThreadFactory.h>
#include <server/TNonblockingServer.h>

using namespace ::apache::thrift;
using namespace ::apache::thrift::protocol;
using namespace ::apache::thrift::transport;
using namespace ::apache::thrift::server;
using namespace ::apache::thrift::concurrency;

using boost::shared_ptr;

#define THREAD_NUM 2
const int g_port = 9090;

class ServHandler : virtual public ServIf {
 public:
  ServHandler() {
    // Your initialization goes here
  }

  void put(const Student& s) {
    // Your implementation goes here
    printf("put\n");
    std::cout <<s.sno <<s.sname <<s.ssex <<s.sage <<std::endl;
  }

};

#ifdef NONBLOCK_SERVER
int thrift_server_run()
{
  shared_ptr<ServHandler> handler(new ServHandler());
  shared_ptr<TProcessor> processor(new ServProcessor(handler));
  shared_ptr<TProtocolFactory> protocolFactory(new TBinaryProtocolFactory());

  shared_ptr<ThreadManager> threadManager = ThreadManager::newSimpleThreadManager(THREAD_NUM);
  shared_ptr<PosixThreadFactory> threadFactory = shared_ptr<PosixThreadFactory> (new PosixThreadFactory());
  threadManager->threadFactory(threadFactory);
  threadManager->start();
  TNonblockingServer server(processor, protocolFactory, g_port, threadManager);

  try
  {
      server.serve();
  }
  catch (TException e)
  {
      printf("Server.serve() failed\n");
      exit(-1);
  }

  return 0;
}

int main(int argc, char **argv)
{
    thrift_server_run();
    while (1)
    {
        sleep(10);
    }
    return 0;
}
#endif

#ifdef SIMPLE_SERVER
int main(int argc, char **argv) {
  int port = 9090;
  shared_ptr<ServHandler> handler(new ServHandler());
  shared_ptr<TProcessor> processor(new ServProcessor(handler));
  shared_ptr<TServerTransport> serverTransport(new TServerSocket(port));
  shared_ptr<TTransportFactory> transportFactory(new TBufferedTransportFactory());
  shared_ptr<TProtocolFactory> protocolFactory(new TBinaryProtocolFactory());

  TSimpleServer server(processor, serverTransport, transportFactory, protocolFactory);
  server.serve();
  return 0;
}
#endif

