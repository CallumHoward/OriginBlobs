go:
	protoc --go_out=plugins=grpc:chat chat.proto

js:
	protoc --js_out=import_style=commonjs:. \
		--grpc-web-out=import_style=commonjs,mode=grpcwebtext:. chat.proto
