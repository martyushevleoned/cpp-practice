BUILD_DIR='build'
BUILD_TYPE='Debug'

clean()
{
    rm -r $BUILD_DIR
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
		-v ~/.conan2:$home_dir/.conan2 \
		-v $(pwd)/utils.sh:$home_dir/.bashrc \
        -v $(pwd):/cpp-practice \
		-w /cpp-practice \
        --hostname container \
        -it --rm $image_name
}

$@
