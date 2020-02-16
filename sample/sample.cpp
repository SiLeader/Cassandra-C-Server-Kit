//
// Created by cerussite on 2/15/20.
//

#include <cqlsvrkit/rpc_v4.hpp>

int main() {
  cqlsvrkit::RpcV4 rpc(9042);
  rpc.startAndWait();
  return 0;
}
