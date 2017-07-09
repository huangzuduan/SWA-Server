@echo off


echo 开始生成

CALL protoc.exe  msg.proto --cpp_out=.

echo 生成文件完成
pause
