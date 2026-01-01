format()
{
    # apt install clang-format
	# TODO format --fast -> `clang-format ${files from git status}`
    find . -type f -regex ".*\.\(c\|h\|cpp\|hpp\)$" -exec clang-format -i {} \;
}

clean()
{
	case "$1" in
		'') rm -r build ;;
		--force | -f) git clean -ffdx . && git reset --hard ;; 
		*) echo "unexpected option $@" ;;
	esac
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
        --hostname container \
        -it --rm dev-container
	# TODO source utils/utils.sh
}

$@
