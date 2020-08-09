/**
 * @fileoverview gRPC-Web generated client stub for chat
 * @enhanceable
 * @public
 */

// GENERATED CODE -- DO NOT EDIT!


/* eslint-disable */
// @ts-nocheck



const grpc = {};
grpc.web = require('grpc-web');

const proto = {};
proto.chat = require('./chat_pb.js');

/**
 * @param {string} hostname
 * @param {?Object} credentials
 * @param {?Object} options
 * @constructor
 * @struct
 * @final
 */
proto.chat.ChatServiceClient =
    function(hostname, credentials, options) {
  if (!options) options = {};
  options['format'] = 'text';

  /**
   * @private @const {!grpc.web.GrpcWebClientBase} The client
   */
  this.client_ = new grpc.web.GrpcWebClientBase(options);

  /**
   * @private @const {string} The hostname
   */
  this.hostname_ = hostname;

};


/**
 * @param {string} hostname
 * @param {?Object} credentials
 * @param {?Object} options
 * @constructor
 * @struct
 * @final
 */
proto.chat.ChatServicePromiseClient =
    function(hostname, credentials, options) {
  if (!options) options = {};
  options['format'] = 'text';

  /**
   * @private @const {!grpc.web.GrpcWebClientBase} The client
   */
  this.client_ = new grpc.web.GrpcWebClientBase(options);

  /**
   * @private @const {string} The hostname
   */
  this.hostname_ = hostname;

};


/**
 * @const
 * @type {!grpc.web.MethodDescriptor<
 *   !proto.chat.Message,
 *   !proto.chat.Message>}
 */
const methodDescriptor_ChatService_SayHello = new grpc.web.MethodDescriptor(
  '/chat.ChatService/SayHello',
  grpc.web.MethodType.UNARY,
  proto.chat.Message,
  proto.chat.Message,
  /**
   * @param {!proto.chat.Message} request
   * @return {!Uint8Array}
   */
  function(request) {
    return request.serializeBinary();
  },
  proto.chat.Message.deserializeBinary
);


/**
 * @const
 * @type {!grpc.web.AbstractClientBase.MethodInfo<
 *   !proto.chat.Message,
 *   !proto.chat.Message>}
 */
const methodInfo_ChatService_SayHello = new grpc.web.AbstractClientBase.MethodInfo(
  proto.chat.Message,
  /**
   * @param {!proto.chat.Message} request
   * @return {!Uint8Array}
   */
  function(request) {
    return request.serializeBinary();
  },
  proto.chat.Message.deserializeBinary
);


/**
 * @param {!proto.chat.Message} request The
 *     request proto
 * @param {?Object<string, string>} metadata User defined
 *     call metadata
 * @param {function(?grpc.web.Error, ?proto.chat.Message)}
 *     callback The callback function(error, response)
 * @return {!grpc.web.ClientReadableStream<!proto.chat.Message>|undefined}
 *     The XHR Node Readable Stream
 */
proto.chat.ChatServiceClient.prototype.sayHello =
    function(request, metadata, callback) {
  return this.client_.rpcCall(this.hostname_ +
      '/chat.ChatService/SayHello',
      request,
      metadata || {},
      methodDescriptor_ChatService_SayHello,
      callback);
};


/**
 * @param {!proto.chat.Message} request The
 *     request proto
 * @param {?Object<string, string>} metadata User defined
 *     call metadata
 * @return {!Promise<!proto.chat.Message>}
 *     A native promise that resolves to the response
 */
proto.chat.ChatServicePromiseClient.prototype.sayHello =
    function(request, metadata) {
  return this.client_.unaryCall(this.hostname_ +
      '/chat.ChatService/SayHello',
      request,
      metadata || {},
      methodDescriptor_ChatService_SayHello);
};


module.exports = proto.chat;

