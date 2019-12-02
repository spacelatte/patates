#!/usr/bin/env -S docker build --compress -t pvtmert/patates -f

FROM debian as build

RUN apt update
RUN apt install -y \
	curl tree make gcc gdb

WORKDIR /data
COPY ./ ./
RUN make clean all tests

#FROM debian
#WORKDIR /data
#COPY --from=build /data/bin ./
#CMD ./patates port=80 root=/srv
