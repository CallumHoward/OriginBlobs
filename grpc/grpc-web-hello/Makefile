all:
	protoc -I=. helloworld.proto \
		--js_out=import_style=commonjs:. \
		--grpc-web_out=import_style=commonjs,mode=grpcwebtext:.

go:
	protoc --go_out=plugins=grpc:. helloworld.proto
