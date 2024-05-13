#include "hello_mock.grpc.pb.h"

namespace hello {

using ::testing::_;
using ::testing::AtLeast;
using ::testing::DoAll;
using ::testing::Return;
using ::testing::SaveArg;
using ::testing::SetArgPointee;
using ::testing::WithArg;

TEST(GRpcTest, test_get) {
  MockHelloServiceStub stub;
  GetResponse resp;
  resp.set_msg("world");
  EXPECT_CALL(stub, Get(_, _, _))
      .Times(AtLeast(1))
      .WillOnce(DoAll(SetArgPointee<2>(resp), Return(grpc::Status::OK)));

  GetRequest req;
  req.set_msg("hello");
  GetResponse ret_resp;
  grpc::ClientContext context;
  stub.Get(&context, req, &ret_resp);
  ASSERT_EQ(ret_resp.msg(), "world");
}

class MyMockHelloService : public HelloService::Service {
    MockHelloServiceStub *stub_;
 protected:
  virtual ::grpc::Status Get(::grpc::ServerContext* context,
                             const ::hello::GetRequest* request,
                             ::hello::GetResponse* response) {

                             }

  virtual ::grpc::Status Post(
      ::grpc::ServerContext* context,
      ::grpc::ServerReader< ::hello::PostRequest>* reader,
      ::hello::PostResponse* response) {}

}

TEST(GRpcTest, test_post) {
  auto rw = new grpc::ClientReaderWriterInterface< ::hello::PostRequest>();
}

}  // namespace hello