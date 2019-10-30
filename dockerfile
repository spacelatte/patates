#!/usr/bin/env -S docker build --compress -t pvtmert/patates -f

FROM debian

RUN apt update
RUN apt install -y tree build-essential clang lldb

WORKDIR /data
COPY ./ ./
RUN make clean all tests

