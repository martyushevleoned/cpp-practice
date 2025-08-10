FROM ubuntu:24.04

ENV DEBIAN_FRONTEND=noninteractive

RUN apt update && apt install -y \
	build-essential \
	clang-format \
    cmake \
    git \
	python3-venv \
    vim

RUN python3 -m venv /venv && \
    /venv/bin/pip install conan==2.17

ENV PATH="/venv/bin:$PATH"

