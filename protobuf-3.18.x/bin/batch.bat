protoc --proto_path=./ --cpp_out=./ protocol.proto
IF ERRORLEVEL 1 PAUSE