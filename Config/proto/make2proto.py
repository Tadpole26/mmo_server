import time,os

for name in os.listdir("./client/"):
    full_path = os.path.join("./client/", name)
    if full_path.endswith(".proto"):
        compile_str = str()
        compile_str += "protoc "
        compile_str += name
        compile_str += " -I./client/ -I./inner/ -I./record/ --cpp_out=../../Libs/protocol "
        print(compile_str)
        os.system(compile_str)

for name in os.listdir("./inner/"):
    full_path = os.path.join("./inner/", name)
    if full_path.endswith(".proto"):
        compile_str = str()
        compile_str += "protoc "
        compile_str += name
        compile_str += " -I./client/ -I./inner/ -I./record/ --cpp_out=../../Libs/protocol "
        print(compile_str)
        os.system(compile_str)

for name in os.listdir("./record/"):
    full_path = os.path.join("./record/", name)
    if full_path.endswith(".proto"):
        compile_str = str()
        compile_str += "protoc "
        compile_str += name
        compile_str += " -I./client/ -I./inner/ -I./record/ --cpp_out=../../Libs/protocol "
        print(compile_str)
        os.system(compile_str)

time.sleep(33333)