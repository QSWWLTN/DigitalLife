mkdir Bulid
copy ..\..\..\Protobuf\*.proto
protoc.exe ./*.proto --cpp_out=./Bulid
del /F /S /Q *.proto
copy Bulid\*.* ..\..\..\Protobuf\*.*
rd /s /q Bulid