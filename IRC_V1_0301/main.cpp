
#include "Server.hpp"

int main()
{
  Server serv;
  
  try
  {
    signal(SIGINT, Server::SignalCatch);
    serv.ServerInit();
  }
  catch(const std::exception& e)
  {
    serv.CloseFds();
    std::cerr << e.what() << "\nici"<< '\n';    
  }
    return(0);
}
