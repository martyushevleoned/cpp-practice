# Conan

## install 
```shell
sudo apt update
sudo apt install python3 python3-venv python3-pip -y

# python venv
python3 -m venv ~/.conan_venv
source ~/.conan_venv/bin/activate

# instalation
pip install conan
which conan
deactivate

# alias
printf "\n#conan python venv\nalias conan=\"~/.conan_venv/bin/conan\"" >> ~/.bashrc
source ~/.bashrc

# check
conan --version
conan profile detect
```

## install libs
```shell
conan install . --build=missing --output-folder=build
```
