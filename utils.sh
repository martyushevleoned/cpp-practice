# set set -eol

BUILD_DIR='build/'
BUILD_TYPE='Debug'

format()
{
    # apt install clang-format
	# TODO format --fast -> `clang-format ${files from git status}`
	# exclude build_dir
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

build()
{
    conan install . --build=missing --output-folder=$BUILD_DIR --settings=build_type=$BUILD_TYPE
    cmake -B $BUILD_DIR -DCMAKE_BUILD_TYPE=$BUILD_TYPE
    make -C $BUILD_DIR -j$(nproc)
}

test()
{
    ctest --test-dir $BUILD_DIR/feature --output-on-failure
}

container()
{
	local image_name='dev'
	local home_dir='/home/ubuntu'
    docker build -t $image_name -f ./Dockerfile .
    docker run \
        -u $(id -u):$(id -g) \
        -v $(pwd):/cpp-practice \
		-v ~/.conan2:$home_dir/.conan2 \
		-v $(pwd)/utils.sh:$home_dir/.bash_aliases \
		-w /cpp-practice \
        --hostname container \
        -it --rm $image_name
}

$@
