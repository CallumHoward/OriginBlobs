const { Message } = require("./chat_pb.js");
const { ChatServiceClient } = require("./chat_grpc_web_pb.js");

var client = new ChatServiceClient("http://localhost:8080");

var request = new Message();
request.setBody("Hello from the js client!");

client.sayHello(request, {}, (err, response) => {
  console.log(response.getBody());
});
