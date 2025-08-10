format()
{
    # apt install clang-format
    find . -type f -regex ".*\.\(c\|h\|cpp\|hpp\)$" -exec clang-format -i {} \;
}

clean()
{
    git clean -ffdx .
    git reset --hard
}

conan()
{
    # python -m venv venv
    # source venv/bin/activate
    # pip install conan
    # conan profile detect
    ~/venv/bin/conan $@
}

build()
{
    local build_dir='build/'
    local build_type='Debug'
    conan install . --build=missing --output-folder=$build_dir --settings=build_type=$build_type
    cmake -B $build_dir -DCMAKE_BUILD_TYPE=$build_type
    make -C $build_dir -j$(nproc)
}

test()
{
    ctest --test-dir build/projects --output-on-failure
}

dev-container()
{
    docker build -t dev-container -f ./utils/Dockerfile .
    docker run \
        -u $(id -u):$(id -g) \
        -v $(pwd):/cpp-practice \
        -v $(pwd)/utils/bash_aliases:/root/.bash_aliases:ro \
        -v /etc/passwd:/etc/passwd:ro \
        -v /etc/group:/etc/group:ro \
        --hostname container \
        -it --rm dev-container
}

$@